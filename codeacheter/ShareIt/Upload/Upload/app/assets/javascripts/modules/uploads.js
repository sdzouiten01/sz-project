//backbone module
(function(module) {
  
  // Dependencies
  var Dashboard = window.App.module('dashboard');
  var Upload = window.App.module('upload');
  var Uploader = window.App.module('uploader');
  var Messages = window.App.module('messages');
  var App = window.App;
  
  // defines initialize method
  module.initialize = function() {
    
    
  }; // end initialize
  
  // paginator
  module.paginator = Backbone.View.extend({
    
    tag: 'div',
    className: 'paginator',
    
    events: {
      
      'click a': 'click'
      
    },
    
    initialize: function(arguments) {
    
      _.extend(this, arguments);
      
    },
  
    render: function() {
      
      if(this.totalPages > 1) {
        
        var html = _.template($('#template-pager').html(), this);
        
        $(this.el).html(html);
        
      }
      else {
      
        this.remove();
        
      }
      
      return this;
      
    },
    
    // handles el click
    click: function(e) {
      
      if(!$(e.target).hasClass('disabled')) {
      
        // adds hash tag to url
        window.location.hash = ($(e.target).attr('href')).replace("/"+window.App.settings.dir, "");
      
      }
      
      e.preventDefault();
      
    },
    
    navigateClosest: function() {
      
      if(this.collection.length == 0 && this.totalPages > 1) {
        
        // if last page
        if(this.currentPage == this.totalPages) {
          window.App.router.openPage(this.currentPage-1, this.currentFilter);
        }
        
        else {
          window.App.router.openPage('', '');
          window.App.router.openPage(this.currentPage, this.currentFilter);
        }
        
      }
      
    }
    
  });
  
  // collection
  module.collection = Backbone.Collection.extend({
  
    model: Upload.model,
    
    initialize: function() {
      
      this.on('selected', this.onSelect, this);
      
    },
    
    currentFilter: 'all',
    
    // holds last selected item index
    lastSelected: '',
    
    url: function() {

      var url = App.url + '/page/' + this.paginator.currentPage + '/' + this.currentFilter;

      if(this.currentFolder) url += '/' + this.currentFolder;

      return url;
    
    },
    
    // process server response data from pagination
    parse: function(response) {
      
      this.paginator = new module.paginator(response);
      this.paginator.currentFilter = this.currentFilter;
      this.paginator.collection = this;
    
      return JSON.parse(response.items);;
      
    },
    
    // handles collection selected event
    onSelect: function(model) {
      
      this.lastSelected = this.indexOf(model);
      
    },
    
    // search collection
    search: function(query) {
      
      _this = this;
    
      // if query is not empty
      if(/\S/.test(query) && query != this.searchQuery) {
      
        this.searchQuery = query;
      
        $.ajax({
          type: 'POST',
          url: App.url + '/search',
          data: { query: query },
          dataType: 'json',
          success: function(data) {
            
            _this.reset(JSON.parse(data.items));
            
            _this.trigger('searched');
            
          }
          
        });
      
      }
      
      
      // TODO: 
      else if(!/\S/.test(query) && query != this.searchQuery) {
        
        this.searchQuery = '';
        
        _this.trigger('searched_cleared');
        
        // reloads last page
        this.paginate(this.paginator.currentFilter, this.paginator.currentPage);
        
      }
      
    },
    
    // paginates collection
    paginate: function(type, page, folder) {
      
      var _this = this;
    
      this.currentFilter = type;

      this.currentFolder = folder;
    
      this.paginator.currentPage = page;
      
      this.fetch({
      
        success: function(collection, response) {
          
          _this.trigger('paginated');
          
          _this.deselectAll();
          
        }
        
      });      
      
    },

    clearPagination: function() {

      this.currentFilter = '';
      this.currentFolder = '';
      this.paginator.currentPage = 1;

    },
    
    // updates collection model from uploader
    updateModel: function(cid, attributes) {
    
      //swaps staged model with response model
      var model = this.getByCid(cid);
      
      //staged_model.attributes = attributes;
      model.set(attributes, { silent: true });
      
      model.trigger('fetched');
      
    },
    
    // gets all selected models
    getSelected: function() {
      
      return this.filter(function(data) {
        
        return data.isSelected;
        
      });
      
    },
    
    // gets last selected upload
    getLastSelected: function() {
      
      var i = this.length;
      while(i--) {
        
        var model = this.at(i);
        
        if(model.isSelected) return i;
        
      }
      
    },
    
    // gets last selected upload
    getFirstSelected: function() {
      
      var i = 0;
      while(i < this.length) {
        
        var model = this.at(i);
        
        if(model.isSelected) return i;
        
        i++;
        
      }
      
    },
    
    // deselects all uploads
    deselectAll: function() {
      
      this.each(function(model) {
        
        model.isSelected = false;
        
      });
      
      this.trigger('selected');
      
    }
    
  });
  
  // main uploads view
  module.Views.uploads = Backbone.View.extend({
    
    el: '#uploads',
    
    // events
    events: {
    },
    
    initialize: function() {
      
      //collection bindings
      this.collection.on('reset', this.onReset, this);
      this.collection.on('add', this.addUpload, this);
      this.collection.on('paginated', this.paginated, this);
      this.collection.on('destroy', this.onDestroy, this);
      
      // sets initial uploads objects
      this.collection.reset(this.collection.parse(window.App.loadedUploads));
      
    },
    
    // handles model destroy event
    onDestroy: function(model) {
      
      this.refreshEmptyMessage();
    
    },
    
    // handles collection changes
    onReset: function() {
      
      this.render(this.collection, this);
      
    },
    
    // renders paginator
    refreshPaginator: function() {
      
      if(!this.collection.searchQuery)
        $(this.el).append(this.collection.paginator.render().el);
      
    },
    
    // renders all collection items
    render: function(uploads, context) {
    
      $(this.el).html('');
      
      uploads.each(function(model) {
        
        context.renderOne(model);
        
      });
      
      // renders paginator for collection
      this.refreshPaginator();
      
      this.refreshEmptyMessage();
      
    },
    
    // handles upload addition
    addUpload: function(model) {
      
      this.renderOne(model);
      
    },
    
    // renders a single item
    renderOne: function(model) {
      
      this.refreshEmptyMessage();
      
      var view = new Upload.Views.main({ model: model }).render().el;
      
      $(view).hide();
      
      var index = this.collection.indexOf(model);
      
      if(index == $(this.el).children().length) {
        
        $(this.el).append(view);
        
      }
      else {
        
        $(this.el).prepend(view);
        
      }
      
      $(view).fadeIn('slow');
      
      return view;
      
    },
    
    // refresh empty uploads message
    refreshEmptyMessage: function() {
      
      if(this.collection.length > 0) {
        
        $(this.el).find('.empty').remove();
        
      }
      else {
        
        $(this.el).html('<div class="empty">There are no uploads in this category</div>');
        
      }
      
    },
    
    // returns an upload view based on cid
    getView: function(id) {
      return $(this.el).find('#upload-'+id);
    },
    
    // removes upload vie wbased on cid
    removeView: function(id) {
      this.getView(id).fadeOut(function() { $(this).remove(); });
    }
    
  }); // end uploads view
    
})(window.App.module('uploads'));