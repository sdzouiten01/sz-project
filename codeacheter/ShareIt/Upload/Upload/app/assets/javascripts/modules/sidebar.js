//backbone module
(function(module) {

  // Dependencies
  var Uploads = window.App.module('uploads');
  var Uploader = window.App.module('uploader');
  var Dashboard = window.App.module('dashboard');
  var Routes = window.App.module('routes');
  var App = window.App;
  
  // defines initialize method
  module.initialize = function() {
  
  }; // end initialize
  
  // main upload view
  module.Views.main = Backbone.View.extend({
    
    el: '#sidebar',
    
    initialize: function() {

      _this = this;
      
      // creates the spacebar handler
      this.spacebar = new module.Views.space();
      
      // creates categories navigation handler
      this.navigator = new module.Views.navigation();

      // creates folders navigation handler
      this.folders = new module.Views.folders({
        parent: _this
      });
      
      // creates categories navigation handler
      this.multiple = new module.Views.multiple();
      
    },
    
    // dom events
    events: {
    },
    
    upload: function(e) {
    }
    
  }); // end main view
  
  // spacebar view
  module.Views.space = Backbone.View.extend({
    
    el: '#spacebar',
    
    used_percentage: 0,
    
    initialize: function() {
    
      this.usedSpaceChanged();
      
    },
    
    // changes bar value
    usedSpaceChanged: function() {
    
      this.used_percentage = (App.user.used_space * 100) / App.user.max_space;
      
      $(this.el).find('.value').css('width', (this.used_percentage + '%'));
      
      $(this.el).find('.used_space').html(formatFileSize(App.user.used_space));
      
    }
    
  }); // end space view
  
  // categories navigation view
  module.Views.navigation = Backbone.View.extend({
    
    el: '#navigation',
    
    events: {
      'click a': 'filter'
    },
    
    initialize: function() {
      
    },
    
    // changes bar value
    filter: function(e) {

      var value = $(e.target).attr('href');

      var folder = Dashboard.main.uploads.collection.currentFolder;

      if(folder) value += "/" + folder;
      
      window.location.hash = value.replace("/"+App.settings.dir, "");
      
      e.preventDefault();
      
      
    }
    
  }); // end categories view
  
  // multiple selected view
  module.Views.multiple = Backbone.View.extend({
    
    el: '#multipleselected',
    
    initialize: function() {
      
    },
    
    events: {
      'click .button.delete': 'delete',
      'change .select': 'changeFolder'
    },
    
    show: function() {
      
      $(this.el).show();
      
      this.selected_uploads = Dashboard.main.uploads.collection.getSelected();
      
      $(this.el).find('.title').text(this.selected_uploads.length + ' selected uploads');

      // selects current category
      var folder = this.selected_uploads[0].get('folder_id');

      var some_folder_count = _.filter(this.selected_uploads, function(item) {
        return item.get('folder_id') == folder;
      });

      if(this.selected_uploads.length == some_folder_count.length) this.selectFolder(folder);
      else this.selectFolder("-1");
      
    },

    // selects folder
    selectFolder: function(value) {

      $(this.el).find('select option[value="'+ value +'"]').attr('selected', '');
      $(this.el).find('select').trigger('changeLabel');

    },
    
    hide: function() {
    
      $(this.el).hide();
      
      this.selected_uploads = null;
      
    },
        
    delete: function(e) {
      
      _.each(this.selected_uploads, function(model) {
        
        model.destroy({ wait: true });
        
      });
      
    },

    changeFolder: function(e) {

      var folder_id = $(e.target).val();

      _.each(this.selected_uploads, function(model) {

        model.save({ folder_id: folder_id }, { wait: true, silent: true });

      });

    }
    
  });

  // categories navigation view
  module.Views.folders = Backbone.View.extend({
    
    el: '#folders',
    
    events: {
      'change select': 'filter',
      'click .create': 'showNew',
      'click .delete': 'deleteCurrent',
      'keyup input'  : 'handleNew'
    },
    
    initialize: function(args) {

      _.extend(this, args);
      
    },
    
    // changes bar value
    filter: function(e) {

      var value = $(e.target).val();

      if(value == 'All') {
        window.location.hash = "";
        $(this.el).find('.button.delete').hide();
        return;
      }

      else {
        window.location.hash = value.replace("/"+App.settings.dir, "");
        $(this.el).find('.button.delete').show();
      }
      
      e.preventDefault();
      
      
    },

    // displays new folder input
    showNew: function(e) {
      $(this.el).find('input').show().focus();
    },

    // handles new folder create
    handleNew: function(e) {

      _this = this;

      var code = (e.keyCode ? e.keyCode : e.which);

      if (code === 27) {
        $(e.target).val('').hide();
        return;
      }

      if(code == 13) {

        var name = $(e.target).val();
        $(e.target).val('').hide();

        if(!/\S/.test(name)) {
          return;
        }

        var folder = Backbone.Model.extend({
          url: function() {
            return App.url + '/folders';
          }
        });

        folder = new (folder)({
          name: name
        });

        folder.save({}, { success: function(model) {

          var value = $(e.target).data('optval').replace('%%', model.id);

          $(_this.el).find('select').append('<option id="option_'+ model.id +'" value="'+ value +'">'+ model.get('name') +'</option>');

          $(_this.parent.multiple.el).find('select').append('<option value="'+ model.id +'">'+ model.get('name') +'</option>');

        }});

      }

    },

    // delete current
    deleteCurrent: function(e) {

      if(confirm("Are you sure you want to delete the current folder ?")) {
        window.location.hash = "";
        var folder_id = Dashboard.main.uploads.collection.currentFolder;
        if(!folder_id) return;

        // remove elements
        $(this.el).find('#option_'+folder_id).remove();
        $(this.parent.multiple.el).find('option[value="'+folder_id+'"]').remove();

        // select default options
        $(this.el).find('#option_initial').attr('selected', '');
        $(this.parent.multiple.el).find('option[value="-1"]').attr('selected', '');

        $(this.el, this.parent.multiple.el).find('select').trigger('changeLabel');

        $(this.el).find('.button.delete').hide();

        _.each(this.selected_uploads, function(model) {

          model.save({ folder_id: folder_id }, { wait: true, silent: true });

        });

        var folder = Backbone.Model.extend({          
          url: function() {
            return App.url + '/folders/'+this.id;
          }
        });

        folder = new (folder)({
          id: folder_id,
          name: name
        });

        folder.destroy({ success: function(model, response) {

          window.location.hash = "";
          window.App.router.openPage(1, 'all');

        }});


      }
    }
    
  }); // end categories view
  
    
})(window.App.module('sidebar'));