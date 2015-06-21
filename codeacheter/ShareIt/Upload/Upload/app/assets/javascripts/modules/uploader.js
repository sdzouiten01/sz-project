//backbone module
(function(module) {
  
  // Dependencies
  var Uploads = window.App.module('uploads');
  var Dashboard = window.App.module('dashboard');
  var Messages = window.App.module('messages');
  var App = window.App;
  
  // defines initialize method
  module.initialize = function() {
  }; // end initialize
  
  // main upload view
  module.Views.main = Backbone.View.extend({
    
    el: '#swfupload',
    
    swfObject: undefined,
    
    initialize: function(arguments) {
      
      _.bindAll(this);
      
      var container = $(this.el).find('.content');
      
      var swfSettings = {
      
        // Settings
        flash_url : App.url + "/app/assets/javascripts/vendor/swfupload/swfupload.swf",
        upload_url: App.url + "/",
        post_params: {
          "PHPSESSID" : App.session_id,
        },
        file_upload_limit : 0,
        file_queue_limit : App.settings.uploads_maxsimuploads,
        file_size_limit: App.settings.uploads_maxupload,
        file_post_name: 'file',
        http_success : [200],
        
        debug: false,
        
        // Button settings
        button_width: $(this.el).parent().find('.button').outerWidth(),
        button_height: $(this.el).parent().find('.button').outerHeight(),
        button_cursor : SWFUpload.CURSOR.HAND,
        button_window_mode: SWFUpload.WINDOW_MODE.TRANSPARENT,
        button_placeholder: container.get(0),
        
        // Handlers
        file_dialog_complete_handler: this.dialogComplete,
        file_queued_handler: this.add,
        file_queue_error_handler: this.queueError,
        upload_progress_handler: this.progress,
        upload_error_handler: this.error,
        upload_success_handler: this.done,
        upload_start_handler: this.start
        
      };
      
      // extends swfupload settings
      if(arguments && arguments.swfsettings) {
        _.extend(swfSettings, arguments.swfsettings);
      }
      
      // extends this view
      _.extend(this, arguments);
      
      this.swfObject = new SWFUpload(swfSettings);
      
    },
    
    // dom events
    events: {
    },
    
    // destroys uploader
    destroy: function() {
      
      var result = this.swfObject.destroy();
      
      // restores container structure
      $(this.el).html('<span class="content"></span');
      
      return result;
      
    },
    
    // get current-upload view wrapper
    getUploadView: function(id) {
      
      return Dashboard.main.uploads.getView(id);
      
    },
    
    // delete upload view
    deleteUploadView: function(file) {
      
      Dashboard.main.uploads.removeView(file.id);
      
    },
    
    // pass file object to render
    completeUpload: function(file, data) {
      
      this.trigger('upload_complete', file, data);
      
    },
    
    // file select complete
    dialogComplete: function(selectedFiles, quededFiles) {
    
      this.swfObject.startUpload();
      
    },
    
    // handles file upload cancel
    cancelUpload: function(id) {
      
      this.swfObject.cancelUpload(id, false);
      
      this.trigger('upload_canceled', id);
    
    },
    
    // checks file size
    checkSize: function(file) {
      
      // checks if there is enough space for this file
      if(App.user.max_space > 0) {
      
        var available_space = App.user.max_space - App.user.used_space;
        
        if(file.size > available_space) {
        
          return false;
        
        }
        
      
      }
      
      return true;
      
    },
    
    // file upload added to queue
    add: function(file) {
    
      // checks if file does not exceed server maximum file size limit
      if(file.size > App.settings.uploads_maxupload) {
        
        // removes file from queue
        this.swfObject.cancelUpload(file.id, false);
        
        // invokes error caused by not enough space for upload
        this.swfObject.queueEvent("upload_error_handler", [file, -300, 'File is too big to be uploaded.']);
        
      }
      else {
    
        if(this.checkSize(file)) {
          
          // file was successfully added
          this.trigger('upload_add', file);
          
        }
        else {
        
          // removes file from queue
          this.swfObject.cancelUpload(file.id, false);
          
          // invokes error caused by not enough space for upload
          this.swfObject.queueEvent("upload_error_handler", [file, -300, '']);
          
        }
      
      }
      
    }, //end file add
    
    // file upload start
    start: function(file) {
      
      this.trigger('upload_start', file);
    
    },
    
    // file upload progress
    progress: function(file, loadedBytes, totalBytes) {
      
      var progress = Math.ceil((loadedBytes / totalBytes) * 100);
      
      var file = this.getUploadView(file.id);
      
      file.find('.value').css('width', (progress + "%"));
      
    }, //end progress
    
    // file upload complete
    done: function(file, data, response) {
      
      var json_data;
      
      try {
      
        json_data = JSON.parse(data);
        
        this.completeUpload(file, json_data);
        
        var stats = this.swfObject.getStats();
        
        if(stats.files_queued == 0) {
          
          this.trigger('upload_allcomplete', file, json_data);  
          
        }
      
      } catch (error) {
        
        // invokes error caused by json parsing errors
        this.swfObject.queueEvent("upload_error_handler", [file, 500, data]);
      
      }
      
    }, // end file upload
    
    // file queue error
    queueError: function(file, errorCode, message) {
      
      if (errorCode === SWFUpload.QUEUE_ERROR.QUEUE_LIMIT_EXCEEDED) {
      
        var message = "You have attempted to queue too many files. " + (message === 0 ? "You have reached the upload limit." : "You may select " + (message > 1 ? "up to " + message + " files." : "one file."));
        
        Messages.main.add({ type: 'error', message: message });
        
        return;
      }
    
    },
    
    // file upload error
    error: function(file, errorCode, message) {
      
      // checks if thumbnail generation failed because of memory size limit
      if(message.indexOf('memory') && message.indexOf('thumbnail')) {
        // renders the upload as the thumbnail will be the file itself
      }
      
      this.swfObject.cancelUpload(file.id);
      this.deleteUploadView(file);
      
      switch (errorCode) {
        
        // cancelled: not enough space
        case -300:
        
          if(!message)
            message = "There's not enough upload space for one or more files you selected";
            
          break;
        
      }
      
      Messages.main.add({ type: 'error', message: message });
      
      this.trigger('upload_error', file, errorCode, message);
      
      
      
    }
    
  }); // end upload view
  
  // uploadoad uploader view for re-upload
  module.Views.inline = module.Views.main.extend({
  
    // checks upload size
    checkSize: function(file) {
      
      // checks if there is enough space for this file
      if(App.user.max_space > 0) {
      
        var available_space = App.user.max_space - (App.user.used_space - this.model.get('size'));
        
        if(file.size > available_space) {
        
          return false;
        
        }
      
      }
      
      return true;
      
    },
    
    // passes parent view as a container for the upload view
    getUploadView: function(id) {
    
      var el = $(this.parent_view.el).find('.inlineupload');
      
      return el;
      
    },
    
    // handles upload start
    start: function(file) {
      
      this.trigger('upload_start', file.id);
      
    }
    
  });
    
})(window.App.module('uploader'));