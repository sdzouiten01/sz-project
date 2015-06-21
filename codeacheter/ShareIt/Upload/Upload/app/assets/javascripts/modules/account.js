//backbone module
(function(module) {

  // defines initialize method
  module.initialize = function() {
    
  };
  
  module.model = Backbone.Model.extend({
  
    initialize: function() {
      
    },
    
    url: function() {
    
      return (App.url + '/user/' + this.get('id'));
      
    }
    
  });
  
  module.Views.main = Backbone.View.extend({
    
    el: '#account',
    
    processKey: true,
    
    events: {
      
      'click .overlay': 'hideModal',
      'click .button.close': 'delete',
      'keyup': 'onKeyUp',
      'click .editavatar .button': 'handleAvatarChange',
      'change input': 'onChange',
      'click .button.save': 'save',
    
    },
    
    initialize: function() {
    
      this.model = new module.model(window.App.user);
    
      this.render();
      
    },
    
    render: function() {
    
      var attributes = this.model.toJSON();
      var html = _.template($('#template-account').html(), attributes);
      
      this.setElement($(html));
      
      this.rendered = true;
      
      $('body').append($(this.el)).addClass('noscroll');
      
    },
    
    hideModal: function(e) {
      
      if(e.toElement.className == 'overlay') {
        
        this.delete();
      
      }
      
    },
    
    // handles save button click event
    save: function(e) {
      
      _this = this;
      
      if(confirm("Are you sure you want to save this changes ?")) {
        
        this.processKey = false;
        
        this.model.save({},{ success: function() {
        
          window.location.href = window.App.url;
          
        }, error: function(model, response) {
          
          _this.processKey = true;
        
          $(_this.el).find('.message').html('<span class="error">'+response.responseText+'</span>');
          
        }});
        
      }
      else {
        this.processKey = true;
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
        
        // enter
        case 13:
          if(this.processKey) {
            this.save();
          }
          break;
        
      }
      
    },
    
    handleAvatarChange: function(e) {
      $(this.el).find('.avatar_field').toggle();
    },
    
    // handle inputs change events
    onChange: function(e) {
    
      var target = $(e.target);
      var value = target.val();
      var name = target.attr('name');
      
      this.model.set(name, value, { silent: true });
      
      // previews avatar
      if(name == 'avatar') {
        
        $(this.el).find('.avatar img').attr('src', value);
        
      }
      
    }
    
  });


})(window.App.module('account'));