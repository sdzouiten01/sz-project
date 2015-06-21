//backbone module
(function(module) {

  // Dependencies
  var Uploads = window.App.module('uploads');
  var Uploader = window.App.module('uploader');
  var Sidebar = window.App.module('sidebar');
  var Messages = window.App.module('messages');
  var Users = window.App.module('users');
  var App = window.App;
  
  // defines initialize method
  module.initialize = function() {
    
    module.main = new (module.Views.main)();
    
  }; // end initialize
  
  // main upload view
  module.Views.main = Backbone.View.extend({
    
    el: '#dashboard',
    
    initialize: function() {
      
      // renders the uploads view
      this.uploads = new (Uploads.Views.uploads)({
        collection: new Uploads.collection()
      });
      
      // creates new uploader handler
      this.uploader = new Uploader.Views.main();
      
      // creates the sidebar handler
      this.sidebar = new Sidebar.Views.main();
      
      // binds uploads collection completed upload event
      this.uploads.collection.on('destroy', this.onUploadDestroyed, this);
      this.uploads.collection.on('reupload_completed', this.onReuploadCompleted, this);
      this.uploads.collection.on('selected', this.onSelected, this);
      this.uploads.collection.on('searched', this.onSearched, this);
      this.uploads.collection.on('searched_cleared', this.onSearchedCleared, this);
      
      // binds uploader events
      this.uploader.on('upload_complete', this.onUploadCompleted, this);
      this.uploader.on('upload_start', this.onUploadStart, this);
      
    },
    
    // dom events
    events: {
      
      'keyup #search': 'onSearch',
      'click #checkupdates': 'checkUpdates'
      
    },
    
    // handles search input value change event
    onSearch: _.debounce(function(e) {
      
      this.uploads.collection.search($(this.el).find('#search').val());
      
    }, 150),
    
    // handles collection searched event
    onSearched: function() {
      
      this.uploads.collection.deselectAll();
      
      Messages.main.add({ type: 'notice', message: this.uploads.collection.length + " Search results", only: true });
      
    },
    
    onSearchedCleared: function() {
      
      Messages.main.reset();
      
    },
    
    // handles upload selected event
    onSelected: function() {
    
      if(this.uploads.collection.getSelected().length >= 1) {
        
        this.sidebar.multiple.show();
        
      }
      
      else {
      
        this.sidebar.multiple.hide();
        
      }
      
    },
    
    // handles upload delete
    onUploadDestroyed: function(upload) {
      
      // updates user used space
      App.user.used_space -= upload.get('size');
      
      this.sidebar.spacebar.usedSpaceChanged();
      
      // navigate to nearest page if all uploads of current page were deleted
      if(this.uploads.collection.length == 0) {
        
        this.sidebar.multiple.hide();
        
        this.uploads.collection.paginator.navigateClosest();
        
      }

      // hides multiple selected
      if(this.uploads.collection.getSelected().length <= 0) {
        this.sidebar.multiple.hide();
      }
      
    },
    
    // handles upload start
    onUploadStart: function(file) {
      
      // inserts upload in collection
      this.uploads.collection.add({ id: file.id, type: 'staged' }, { at: 0 });
      
    },
    
    // handles uploader complete event
    onUploadCompleted: function(file, data) {
    
      // updates used space
      App.user.used_space += data.size;
        
      this.sidebar.spacebar.usedSpaceChanged();
      
      var file_view = this.uploads.getView(file.id);
      
      // checks if upload is same type as current filter
      if(this.uploads.collection.currentFilter != 'all' && data.type != this.uploads.collection.currentFilter) {
      
        this.uploads.collection.remove(this.uploads.collection.getByCid(file_view.data('cid')));
        
        file_view.remove();
        
        this.uploads.refreshEmptyMessage();
        
        // TODO: update category counter
        
        Messages.main.add({ type: 'notice', message: "Upload added to " + data.type + " category" });
        
        return;  
        
      }
      
      this.uploads.collection.updateModel(file_view.data('cid'), data);
      
    },
    
    // handles upload re-upload event
    onReuploadCompleted: function(upload, new_size) {
      
      // updates user used space      
      App.user.used_space -= upload.get('size');
      App.user.used_space += new_size;
      
      this.sidebar.spacebar.usedSpaceChanged();
      
    },
    
    // checks for updates against server
    checkUpdates: function(e) {
      
      $.getJSON($(e.target).data('url')+'?callback=?', function(data) {
        if(data.versionnumber > window.App.version) {
          Messages.main.add({ type: 'notice', message: "New updates available, make sure you download them from your CodeCanyon downloads page" });
        }
        else {
          Messages.main.add({ type: 'notice', message: "No updates available" });
        }
      });
      
      
    }
    
  }); // end main view
    
})(window.App.module('dashboard'));