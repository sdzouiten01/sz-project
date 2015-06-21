<?php


/**
*  Check uploads folder integrity
*/
$app->hook('slim.before', function() use ($app) {

  $path = app_dir();
  
  $folders = array('uploads/', 'uploads/thumbnails/', 'uploads/ftp/');
  
  foreach($folders as $folder) {
    
   if(!is_dir($path.$folder)) {
     
     mkdir($path.$folder);
     
   }
    
  }
  
});

/**
*  Default template dadta
*/
$app->hook('slim.before.dispatch', function() use ($app) {
  
  // flash notices
  $flash = $app->view()->getData('flash');
  if($flash) $app->view()->setData('flash', $flash);
  
  // Prevents users removed from database but are logged in access the application
  $user = $app->sessions->get_var('user');
  
  if($user) {
    
    $user = User::find_by_id($app->sessions->get_var('user')->id);
    
    if(!$user) {
    
      if($app->router()->getCurrentRoute()->getPattern() != '/logout') {
      
        $app->redirect($app->urlFor('logout'));
      }
      
    }
    
    // passes user to templates and saves it to session
    $user = $user->save_session();
    $app->view()->setData('user', $user);
    
  }
  
});

/**
*  Not found view
*/
$app->notFound(function() use ($app) {
  
  $app->render('layouts/404.php');
  
});

?>