//backbone module
(function(module) {

  // defines initialize method
  module.initialize = function() {
    
  };
  
  module.model = Backbone.Model.extend({
  
    initialize: function() {
      
    },
    
    url: function() {
    
      return (App.url + '/users/' + this.get('id'));
      
    }
    
  });
  
  module.collection = Backbone.Collection.extend({
    
    model: module.model,
    
    // search by email
    searchByEmail: function(email) {
      
      return _(this.filter(function(data) {
        
        return (data.get('email').indexOf(email) !== -1);
        
      }));
    }
    
  });
  
  module.Views.user = Backbone.View.extend({
    
    initialize: function() {
      
      this.model.on('destroy', this.onDestroy, this);
      this.model.on('sync', this.update, this);
      
    },
    
    events: {
      'click .email': 'onEmailClick',
      'click .delete': 'onDelete',
      'change input[name=max_space]': 'onSpaceChange',
      'change select[name=role]': 'onRoleChange',
    },
    
    // handles max_space change event
    onSpaceChange: function(e) {
      
      this.model.set('max_space', $(e.target).val(), {silent: true });
      
    },
    
    // handles role select change event
    onRoleChange: function(e) {
      
      this.model.set('role', $(e.target).val(), {silent: true });
      
    },
    
    // renders single user view
    render: function() {
      
      //holds model attributes and adds cid to the list
      var attributes = this.model.toJSON();
      var html = _.template($('#template-user').html(), attributes);
      
      this.setElement($(html));
      
      return this;
      
    },
    
    // handles model attributes changes
    update: function() {
      
      //holds model attributes and adds cid to the list
      var attributes = this.model.toJSON();
      
      //replaces the current view html
      var html = _.template($('#template-user').html(), attributes);
      var old_view = $(this.el);        
      this.setElement($(html));
      old_view.replaceWith($(this.el));
      
    },
    
    onDestroy: function() {
      
      $(this.el).remove();
      
    },
    
    onDelete: function() {
      
      if(confirm("Are you sure you wan to delete this user ? All uploads by this user will be lost")) {
      
        this.model.destroy({ wait: true });
      
      }
      
    },
    
    onEmailClick: function() {
      
      this.activate();
      
    },
    
    save: function() {
      
      this.deactivate();
      
      this.model.save();
      
    },
    
    activate: function() {
      
      $(this.el).addClass('edit');
      
      this.bindEditEvents();
      
    },
    
    deactivate: function() {
      
      $(this.el).removeClass('edit');
      
      this.unbindEditEvents();
      
    },
    
    // binds the outside element events
    bindEditEvents: function () {
    
      var _this = this;
      
      $(this.el).bind("click.user" + this.model.id, function (e) {
        return e.stopPropagation();
      });
      
      // finishes upload edit when a click outside the upload or keyup event is detected
      _.defer(function () {
      
        $(document).bind("click.user" + _this.model.id, function () {
          
          _this.save();
        
        }).bind("keyup.user" + _this.model.id, function (e) {
        
          var code = (e.keyCode ? e.keyCode : e.which);
        
          if (code === 27 || code == 13) {
            
            _this.save();
            
          }
          
        })
      
      });
      
    },
    
    // unbinds the outside element events
    unbindEditEvents: function() {
      
      $(this.el).unbind('.user'+this.model.id);
      $(document).unbind('.user'+this.model.id);
      
    }
    
  });
  
  module.Views.main = Backbone.View.extend({
    
    el: '#users',
    
    events: {
      
      'click .overlay': 'hideModal',
      'click .button.close': 'delete',
      'keyup .search': 'onSearch',
      'keyup': 'onKeyUp'
    
    },
    
    initialize: function() {
      
      // sets initial uploads objects
      this.collection = new module.collection();
      
      this.collection.on('add', this.onAdd, this);
      this.collection.on('reset', this.onReset, this);
      
      this.collection.reset(window.App.loadedUsers);
      
    },
    
    onSearch: function(e) {
      
      var val = $(e.target).val();
      
      this.renderCollection(this.collection.searchByEmail(val));
      
    },
    
    render: function() {
    
      var html = _.template($('#template-users').html(), { count: this.collection.length });
      
      this.setElement($(html));
      
      this.rendered = true;
      
      $('body').append($(this.el)).addClass('noscroll');
      
    },
    
    onReset: function() {
      
      if(!this.rendered) {
        
        this.render();
        
      }
      
      this.renderCollection(this.collection);
      
    },
    
    renderCollection: function(collection) {
      
      $(this.el).find('.users').html('');
      
      var context = this;
      
      collection.each(function(model) {
        
        context.onAdd(model);
        
      });
      
    },
    
    onAdd: function(model) {
      
      var view = new module.Views.user({ model: model });
      
      $(this.el).find('.users').append(view.render().el);
      
    },
    
    hideModal: function(e) {
      
      if(e.toElement.className == 'overlay') {
        
        this.delete();
      
      }
      
    },
    
    delete: function(e) {
      
      window.App.router.navigate('/');
      
      this.remove();
      
      $('body').removeClass('noscroll');
      
    },
    
    onKeyUp: function(e) {
    
      var code = (e.keyCode ? e.keyCode : e.which);
      
      switch (code) {
       
       // escape 
        case 27:
          this.delete();
          break;
        
      }
      
    }
    
  });


})(window.App.module('users'));