/*
  Pablo Fierro
  Copyright (c) 2012
*/

// backbone set attribute by name extension
Backbone.Model.prototype.setByName = function(key, value, options) { 
  var setter = {}; 
  setter[key] = value; 
  this.set(setter, options); 
};

// sets backbone to emulate json data with server
Backbone.emulateJSON = true;

// sets backbone to emulate HTTP RESTFUL methods for legacy servers
Backbone.emulateHTTP = true;

/**
*  Application Structure Definition
*/
window.App = {
  
  settings: {},
  
  modules: {},

  // retrieve or initialize a module
  module: function() {
    
    // Create a new module reference scaffold or load an
    // existing module.
    return function(name) {
      var modules = window.App.modules;
      // If this module has already been created, return it.
      if (modules[name]) {
        return modules[name];
      }
      // Create a module and save it under this name
      return modules[name] = { 
        Views: {},
        initialize: function() {},
      };
    };
    
  }()

};

/**
*  jQuery Initializer
*/
$(function(){
  
  //runs initialize method on modules
  _.each(window.App.modules, function(module) {
  
    module.initialize();
    
  });
    
});