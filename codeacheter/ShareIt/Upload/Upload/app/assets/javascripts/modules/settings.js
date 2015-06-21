//backbone module
(function(module) {

  // defines initialize method
  module.initialize = function() {
    
  };
  
  module.model = Backbone.Model.extend({
  
    initialize: function() {
      
    },
    
    isNew: function() { 
      return false;
    },
    
    url: function() {
    
      return (App.url + '/settings');
      
    }
    
  });
  
  module.Views.main = Backbone.View.extend({
    
    el: '#users',
    
    processKey: true,
    
    events: {
      
      'click .overlay': 'hideModal',
      'click .button.cancel': 'delete',
      'keyup': 'onKeyUp',
      'click .button.save': 'save',
      'change input': 'handleChange',
      'change select': 'handleChange'
    
    },
    
    initialize: function() {
      
      this.model = new module.model(window.App.settings);
    
      this.render();
      
    },
    
    render: function() {
      
      var attributes = this.model.attributes;
      
      var schema = attributes.schema;

      // removes not necessary settings
      delete attributes.schema;
      delete attributes.dir;
      if(attributes.version_number) delete attributes.version_number;
      
      var html = _.template($('#template-settings').html(), { attributes: attributes, schema: schema });
      
      this.setElement($(html));
      
      this.rendered = true;
      
      $('body').append($(this.el)).addClass('noscroll');
      
      // selects current time zone
      $(this.el).find('select[name=timezone]').find('option[value="'+this.model.get('timezone')+'"]').attr('selected', true);
      
    },
    
    // handles input change
    handleChange: function(e) {
      
      var target = $(e.target);
      var value = target.val();
      var name = target.attr('name');

      // checkbox
      if($(e.target).attr('type') == 'checkbox') {
        value = $(e.target).is(':checked') ? 1 : 0;
      }
      
      this.model.set(name, value, { silent: true });
      
    },
    
    // handles save button click event
    save: function(e) {
      
      if(confirm("Are you sure you want to save this changes ?")) {
        
        this.processKey = false;
        
        this.model.save({},{ success: function() {
          
          window.location = window.App.url;
          
        }});
        
      }
      else {
        this.processKey = true;
      }
      
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
    
    delete: function() {
      
      window.App.router.navigate('/');
      
      this.remove();
      
      $('body').removeClass('noscroll');
      
    },
    
    onKeyUp: function(e) {
    
      var code = (e.keyCode ? e.keyCode : e.which);
      
      switch (code) {
        
        // enter
        case 13:
          if(this.processKey) {
            this.save();
          }
          break;
       
       // escape 
        case 27:
          this.delete();
          break;
        
      }
      
    }
    
  });


})(window.App.module('settings'));