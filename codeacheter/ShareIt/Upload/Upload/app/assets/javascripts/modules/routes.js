//backbone module
(function(module) {

  // Dependencies
  var Uploads = window.App.module('uploads');
  var Uploader = window.App.module('uploader');
  var Sidebar = window.App.module('sidebar');
  var Dashboard = window.App.module('dashboard');
  var Users = window.App.module('users');
  var Settings = window.App.module('settings');
  var Account = window.App.module('account');
  
  // defines initialize method
  module.initialize = function() {
  
    window.App.router = new module.main();
    
    // starts routers
    Backbone.history.start({
      
      root: App.settings.dir
      
    });
    
  };
  
  // main router
  module.main = Backbone.Router.extend({
    
    routes: {
      "page/:page/:filter": "filterType",
      "page/:page/:filter/:folder": "filterType",
      "users": "showUsers",
      "settings": "showSettings",
      "account": "showAccount"
    },
    
    filterType: function(page, filter, folder) {
      
      Dashboard.main.uploads.collection.paginate(filter, page, folder);
      
      $(Dashboard.main.sidebar.el).find('.active').removeClass('active');
      
      $(Dashboard.main.sidebar.el).find('li[data-type='+filter+']').addClass('active');

      if(folder) {
        $('#folders select #option_'+folder).attr('selected', '');
        $('#folders select').trigger('changeLabel');
        $('#folders').find('.button.delete').show();
      }
      
    },
    
    openPage: function(page, filter) {
      
      this.navigate("page/"+page+"/"+filter, { trigger: true });
      
    },
    
    showUsers: function() {
      
      // create user manager if user is admin
      if(App.user.role == 'admin') {
        var users = new Users.Views.main();
      }
      
    },
    
    showSettings: function() {
      
      if(App.user.role == 'admin') {
        var settings = new Settings.Views.main();
      }
      
    },
    
    showAccount: function() {
    
      var account = new Account.Views.main();
      
    }
    
  });
    
})(window.App.module('routes'));