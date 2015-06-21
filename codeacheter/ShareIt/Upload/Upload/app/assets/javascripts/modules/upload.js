//backbone module
(function(module) {
  
  // Dependencies
  var Dashboard = window.App.module('dashboard');
  var Uploader = window.App.module('uploader');
  var Messages = window.App.module('messages');
  var App = window.App;
  
  // defines initialize method
  module.initialize = function() {
  }; // end initialize

  // model
  module.model = Backbone.Model.extend({
    
    initialize: function() {
      
      this.isSelected = false;
      
      this.isUploading = false;
      
      this.on('error', this.onError, this);
      
    },
    
    url: function() {
       
       return (App.url + '/' + this.get('id'));
      
    },
    
    onError: function(model, error, options) {
      
      if(error.responseText) {
      
        Messages.main.add({ type: 'error', message: error.responseText + ' - Error Code: ' + error.status });
        
      }
      
      else {
        
        Messages.main.add({ type: 'error', message: message });
        
      }
      
    }
    
  });
  
  // main upload view
  module.Views.main = Backbone.View.extend({
    
    // uploader object for re-upload
    uploader: undefined,
    
    initialize: function(arguments) {
      
      _.bindAll(this);
      
      // binds model fetched event
      this.model.on('fetched', this.onFetched, this);
      
      // binds model changed evnet
      this.model.on('change', this.update, this);
      
      // binds destroyed event
      this.model.on('destroy', this.remove, this);
      
      this.model.on('change:selected', this.select, this);
      
    },
    
    // dom events
    events: {
      'change input.check': 'select',
      "change input[name=name]": 'changeName',
      "change select[name=expiration]": 'changeExpiration',
      "change input[name=expiration]": 'changeExpirationVal',
      "change input[name=password]": 'changePassword',
      'click .title a': 'edit',
      'click a.delete': 'delete',
      'click .thumb': 'open'
    },
    
    // handles thumbnail click
    open: function(e) {
    
      if(e.shiftKey || e.metaKey || e.ctrlKey) {
      
        this.select(e);
        
      }
      
      else {
        
        window.open(this.model.get('uid'), "_blank");
        
      }
      
    },
    
    // handles model name change
    changeName: function(e) {
      this.model.set('name', $(e.target).val(), {silent: true });
    },
    
    // handles model expiration change
    changeExpiration: function(e) {
      
      // displays expiration edit input if an option is selected
      var val = $(e.target).val();
      var edit_input = $(this.el).find('input[name=expiration]');
      if(val != 'none') {
        edit_input.show();
      }
      else {
        edit_input.hide();
      }
      edit_input.focus();
      
      // updates model expiration attribute
      this.model.set('expiration', val, {silent: true });
      
    },
    
    changeExpirationVal: function(e) {
      
      this.model.set('expiration_value', $(e.target).val(), {silent: true });
      
    },
    
    // handles password edit
    changePassword: function(e) {
      
      this.model.set('password', $(e.target).val(), { silent: true });
      
    },
    
    // handles model attributes changes
    update: function() {
      
      //holds model attributes and adds cid to the list
      var attributes = this.model.toJSON();
      attributes['cid'] = this.model.cid;
      
      //replaces the current view html
      var html = _.template($('#template-upload').html(), attributes);
      var old_view = $(this.el);        
      this.setElement($(html));
      old_view.replaceWith($(this.el));
      
    },
    
    // handles model response update
    onFetched: function() {
      
      $(this.el).find('.label').text('Processing...');
      
      // TOOD: if file is image type use imageloaded to wait for thumb loading
      
      // delays for progress animation
      setTimeout(this.update, 750);
      
    },
    
    // handles title edit
    edit: function(e) {
    
      e.preventDefault();
      
      this.active();
      
    },
    
    // actives editing mode
    active: function() {
      
      if(!this.model.isSelected)
        this.select();
      
      $(this.el).find('input.edit').val(this.model.get('name')).show().focus().select();
      
      $(this.el).addClass('onedit');
      
      this.reupload_create();
      
      this.bindEditEvents();
      
    },
    
    // deactivates editing mode
    inactive: function() {
      
      this.select();
      
      // destroys uploader instance
      this.uploader.destroy();
      
      $(this.el).removeClass('onedit');
      
      $(this.el).find('input.edit').blur().hide();
      
      this.unbindEditEvents();
      
    },
    
    // saves model
    save: function() {
      
      if(this.model.isUploading)
        return;
    
      this.inactive();
      
      this.model.save();
      
    },
    
    // binds the outside element events
    bindEditEvents: function () {
    
      var _this = this;
      
      $(this.el).bind("click.upload" + this.model.id, function (e) {
        return e.stopPropagation()
      });
      
      // finishes upload edit when a click outside the upload or keyup event is detected
      _.defer(function () {
      
        $(document).bind("click.upload" + _this.model.id, function () {
          
          _this.save();
        
        }).bind("keyup.upload" + _this.model.id, function (e) {
        
          var code = (e.keyCode ? e.keyCode : e.which);
        
          if (code === 27 || code == 13) {
            
            _this.save();
            
          }
          
        })
      
      });
      
    },
    
    // unbinds the outside element events
    unbindEditEvents: function() {
      
      $(this.el).unbind('.upload'+this.model.id);
      $(document).unbind('.upload'+this.model.id);
      
    },
    
    // handles delete click
    delete: function(e) {
      
      // checks if upload is currently uploading
      if(this.model.get('type') == 'staged') {
        
        // cancel file upload passing it to uploader module        
        Dashboard.main.uploader.cancelUpload(this.model.id);
        
        this.remove();
        
      }
      
      else {
      
        // cancels inline upload if uploading
        if(this.model.isUploading) {
          
          this.inline_upload_view.cancel();
          
        }
      
        this.model.destroy({ wait: true });
        
      }
      
      this.unbind();
      
      e.preventDefault();
      
    },
    
    // handles view remove
    remove: function() {
      
      $(this.el).fadeOut(function() { $(this).remove(); });
    
    },
    
    // handles model select
    select: function(e, force) {
      
      if(!force) {
      
        $(this.el).toggleClass('selected');
        
        this.model.isSelected = this.model.isSelected ? false : true;
        
        $(this.el).find('input.check').attr('checked', this.model.isSelected);
        
      }
      else {
        
        $(this.el).addClass('selected');
        $(this.el).find('input.check').attr('checked', true);
        this.model.isSelected = true;
        
      }
      
      if(e && e.shiftKey) {
        
        // last selected upload
        var last_index = this.model.collection.getLastSelected();
        
        var first_index = this.model.collection.getFirstSelected();
        
        if(first_index != last_index) {
        
          // select all items from first to last
          for (var i = first_index+1; i < last_index; i++) {
          
            this.model.collection.at(i).trigger('change:selected', '', true);
            
          }
          
          
        }
        
      }
      
      // triggers parent collection selected event
      this.model.collection.trigger('selected', this.model);
      
    },
    
    // renders single upload view
    render: function() {
      
      //holds model attributes and adds cid to the list
      var attributes = this.model.toJSON();
      attributes['cid'] = this.model.cid;
      var html = _.template($('#template-upload').html(), attributes);
      
      this.setElement($(html));
      
      return this;
      
    },
    
    // re-upload featue
    reupload_create: function() {
      
      this.uploader = new Uploader.Views.inline({
        
        el: '#uploader-'+this.model.id,
        model: this.model,
        parent_view: this,
        // uploader settings
        swfsettings: {
          upload_url: this.model.url(),
          file_queue_limit: 1,
          post_params: {
            "PHPSESSID" : App.session_id,
            "upload_id": this.model.id
          }
        }
      
      });
      
      this.uploader.on('upload_start', this.reupload_start, this);
      this.uploader.on('upload_complete', this.reupload_complete, this);
      this.uploader.on('upload_error', this.reupload_error, this);
      this.uploader.on('upload_canceled', this.reupload_error, this);
      
    },
    
    // updates model from re-upload
    updateReUpload: function(data) {
      
      $(this.el).removeClass('uploading');
      
      // triggers the model's collection reupload event
      this.model.collection.trigger('reupload_completed', this.model, data.size);
      
      this.model.set(data, {silent: true });
      
      $(this.el).find('input.edit').val(this.model.get('name'));
      
      this.model.isUploading = false;
      
      this.inline_upload_view.remove();
      
    },
    
    // re-upload upload complete
    reupload_complete: function(file, data) {
      
      $(this.inline_upload_view.el).find('.label').text('Processing...');
      
      _.delay(this.updateReUpload, 100, data);
      
    },
    
    // re-upload add
    reupload_add: function() {
    },
    
    // re-upload start
    reupload_start: function(id) {
      
      $(this.el).addClass('uploading');
      
      this.inline_upload_view = new module.Views.inline({ parent_view: this, file_id: id });
      
      $(this.el).find('.editform').prepend(this.inline_upload_view.render().el);
      
      this.model.isUploading = true;
      
    },
    
    reupload_error: function(file, errorCode, message) {
      
      $(this.el).removeClass('uploading');
      
      this.model.isUploading = false;
      
      if(this.inline_upload_view) {
        this.inline_upload_view.remove();
      }
      
    }
    
  }); // end upload view
  
  // inline upload (re-upload)
  module.Views.inline = Backbone.View.extend({
    
    initialize: function(arguments) {
      
      this.parent_view = arguments.parent_view;
      this.file_id = arguments.file_id;
      
    },
    
    events: {
      
      'click .cancel': 'cancel'
      
    },
    
    cancel: function() {
    
      // cancel file upload passing it to uploader module        
      this.parent_view.uploader.cancelUpload(this.file_id);
      
    },
  
    render: function() {
    
      var attributes = { id: this.file_id };
      var html = _.template($('#template-upload_inline').html(), attributes);
      
      this.setElement($(html));
      
      return this;
      
    }
    
  }); // end inline upload view
    
})(window.App.module('upload'));