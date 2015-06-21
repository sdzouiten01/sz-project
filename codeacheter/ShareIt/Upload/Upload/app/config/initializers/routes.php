<?php

/**
* Initializes application routes
*/
function intialize_routes() {
  global $app;

  // stores routes declaration files, they will be loaded in order
  $routes = array(
    'auth.php',
    'ftp.php',
    'hooks.php',
    'login.php',
    'settings.php',
    'folders.php',
    'users.php',
    'uploads.php'
  );
  
  foreach($routes as $route) {
    include(APPPATH.'/routes/'.$route);
  }

}

?>