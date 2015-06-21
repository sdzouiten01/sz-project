//backbone module
(function(module) {
  
  // Dependencies
  var Uploads = window.App.module('uploads');
  var Dashboard = window.App.module('dashboard');
  
  // defines initialize method
  module.initialize = function() {
  }; // end initialize
  
  // main upload view
  module.Views.main = Backbone.View.extend({
    
    el: '#fileupload',
    
    initialize: function() {
      
      this.uploads = this.options.uploads;
      
      //initializes file uploader
      $(this.el).fileupload({
        dataType: 'json',
        autoUpload: true,
        dropZone: $('body'),
        sequentialUploads: true,
        
        done: this.done,
        progress: this.progress,
        
        fail: function(e, data) {
        },
        
        add: this.add,
        
        dragover: function(e) {
          var dropZone = $('body'),
          timeout = window.dropZoneTimeout;
          
          if (!timeout) {
            dropZone.addClass('in');
          } else {
            clearTimeout(timeout);
          }
          if (e.target === dropZone[0]) {
            dropZone.addClass('hover');
          } else {
            dropZone.removeClass('hover');
          }
          window.dropZoneTimeout = setTimeout(function () {
            window.dropZoneTimeout = null;
            dropZone.removeClass('in hover');
          }, 100);
        }
        
      });
      
    },
    
    // dom events
    events: {
    },
    
    // file upload add
    add: function(e, data) {
      
      $.each(data.files, function(index, file) {
      
        // adds item to collection
        var upload = Uploads.list.add({ type: 'staged' }).last();
        // attaches file data to view
        data.context = Dashboard.main.uploads.getView(upload.cid);
        
      });
      
      //sends file upload
      data.submit();
      
    }, //end file add
    
    // file upload progress
    progress: function (ev, data) {
      
      //if file data DOM has been set
      if (data.lengthComputable && data.context) {
      
        var loader = $(data.context).find('.value');
        
        var progress = parseInt(data.loaded / data.total * 100, 10);
        
        loader.css('width', progress + "%");
        
      }
      
    }, //end progress
    
    // file upload complete
    done: function(e, data) {
      
      if(data.context) {
      
        console.log(data);
      
        data.context.each(function(index) {
          
          var context = $(this);
          
          // response from server
          //var file = ($.isArray(data.result) && data.result[index]) || {error: 'emptyResult'};
          
          var file = data.result;
          
          // passes response to uploads.js module
          Uploads.list.updateModel(context.data('cid'), file);
        
        });
        
        
        
      }
      
    } // end file upload
    
  }); // end upload view
    
})(window.App.module('uploader_html5'));