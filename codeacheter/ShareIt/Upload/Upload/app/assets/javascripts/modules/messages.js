//backbone module
(function(module) {

  // defines initialize method
  module.initialize = function() {
    
    var messages = new module.Views.main();
    
    this.main = messages.collection;
    
  };
  
  module.Views.message = Backbone.View.extend({
    
    className: "message",
    
    events: {
      
      'click .close': 'close'
      
    },
    
    render: function() {
      
      $(this.el).text(this.model.get('message')).addClass(this.model.get('type'));
      
      $(this.el).attr('id', 'message-'+ this.model.cid);
      
      // appends close button
      $(this.el).append('<a class="close">x</a>');
      
      // hides message after 5 seconds
      _this = this;
      
      if(!this.model.get('only')) {
      
        setTimeout(function() {
        
          if($(document).find('#message-'+_this.model.cid).length > 0) {
          
            _this.close();
          
          }
          
        }, 5000);
        
      }
      
      return this;
      
    },
    
    close: function(e) {
      
      if($(this.el).length > 0) {
      
        $(this.el).fadeOut('fast', function() { $(this).remove(); });
      
        this.model.collection.remove(this.model);
      
      }
      
    }
    
  });
  
  module.Views.main = Backbone.View.extend({
    
    el: '#messages',
    
    initialize: function() {
      
      // sets initial uploads objects
      this.collection = new Backbone.Collection();
      
      this.collection.on('add', this.onAdd, this);
      this.collection.on('reset', this.onReset, this);
      
      this.collection.reset(window.App.messages);
      
    },
    
    onModelChange: function(model) {
      
      console.log("model has changed");
      
    },
    
    onReset: function() {
      
      $(this.el).html('');
      
      var context = this;
      
      this.collection.each(function(model) {
        
        context.onAdd(model);
        
      });
      
    },
    
    onAdd: function(model) {
      
      if(model.get('only')) {
       
         $(this.el).html(''); 
        
      }
      
      var view = new module.Views.message({ model: model });
      
      $(view.el).hide();
      
      $(this.el).append(view.render().el);
      
      $(view.el).fadeIn('fast');
      
    }
    
  });

})(window.App.module('messages'));