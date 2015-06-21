<?php

// Defines app version
define('APPVERSION', 1.15);

// Require the slim framework to handle HTTP requests
require APPPATH.'/Slim/Slim.php';
\Slim\Slim::registerAutoloader();

// Require PHP active record
require APPPATH.'/vendor/php-activerecord/ActiveRecord.php';

// Includes all files from folder function
function include_from($dir, $ext='php') {
  global $app;
  $opened_dir = OPENDIR($dir);
  while($element=READDIR($opened_dir)) {
    $fext=SUBSTR($element,STRLEN($ext)*-1);
    if(($element!='.') && ($element!='..') && ($fext==$ext)){
      include($dir.$element);
    }
  }
  closedir($opened_dir);
}

// Initialize slim with custom settings
$app = new \Slim\Slim(array(
  
  //security
  'secret_key' => 'supp190p-kp01',
  
  //cookies
  'cookies.secret_key' => 'supp190p',
  'cookies.lifetime' => time() + 86400 * 365 * 2,
  'cookies.httponly' => true,
  
  // views
  'templates.path' => APPPATH.'/views/'
  
));

// Loads application localization
require APPPATH.'/config/localization.php';

// Returns an app settings value
function get_config($key) {
  $settings = \Slim\Slim::getInstance()->config('settings');

  if(isset($settings[$key])) {
    return $settings[$key];
  }
  else {
    return NULL;
  }

}

// Sets an app seting value
function set_config($key, $value) {
  $settings = \Slim\Slim::getInstance()->config('settings');
  $settings[$key] = $value;
  \Slim\Slim::getInstance()->config('settings', $settings);
}

// Returns a settings value
function get_setting($key) {  
  return \Slim\Slim::getInstance()->config($key);
}

// Returns the absolute app path
function app_url($uri = '') {
  
  $enviroment = \Slim\Slim::getInstance()->environment();
  
  // prevents double slash
  return $enviroment['slim.url_scheme'].'://'.str_replace("//", "", get_config('url').$uri);
  
}

// Returns the absolute app path
function app_path() {
  return APPPATH;
}

// Returns the absolute app dir
function app_dir() {
  return APPDIR;
}

// Includes all lib files
include_from(APPPATH.'/lib/');

// Includes all helpers
include_from(APPPATH.'/helpers/');

// combile all javascript files(will output scripts)
// javascript_join('vendor/', 'vendor/wysihtml5/', 'ui', 'app', 'vendor/swfupload/', 'modules/');

// Detect installation
$config_file = APPDIR.'/config.php';

if(!file_exists($config_file)) $config_file = APPPATH.'/config/config.php';

if(file_exists($config_file)) {

  require $config_file;

  // detects shareit 1
  if(isset($config)) {
    require APPPATH.'/config/initializers/setup.php';
  }
  
  // Initializes ActiveRecord
  ActiveRecord\Config::initialize(function($cfg) use ($app, $database_connection)
  {
    $cfg->set_model_directory(APPPATH.'/models');
    $cfg->set_connections(array(
      'development' => $database_connection
    ));
  });
  
  // Includes route initializer
  include(APPPATH.'/config/initializers/settings.php');

  // requires updater
  require APPPATH.'/config/updater.php';
  
  // Includes route initializer
  include(APPPATH.'/config/initializers/routes.php');
  
  // Declares new app sessions
  $app->sessions = new Sessions();
  
  // Initializes all application routes
  intialize_routes();
  
  // calls app auth middleware
  $app->add(new Auth_Middleware());
  
  // Runs the current instance of the slim application
  $app->run();
  
}

else {
  require APPPATH.'/config/initializers/setup.php';
}

?>