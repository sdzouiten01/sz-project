<?php

/**
*  PUT
*  Single user update route
*/
$app->put('/user/:id', function($id) use($app) {
  
  $user = User::find_by_id($app->sessions->get_var('user')->id);
  
  if(!$user->role == 'admin') {  
  
    $app->halt(403, 'You are not authorized to perform this action.');
  
  }
  
  $user = User::find_by_id($id);
  
  if($user) {
    
    //Gets the request parameters from a json format sent by backbonejs
    $parameters = json_decode($app->request()->post("model"), true);
    
    // unnecessary attribute
    unset($parameters['privileges']);
    
    // validates max space
    $parameters['max_space'] = intval($parameters['max_space']);
    
    // validates uniqueness of email
    if($parameters['email'] != $user->email) {
      if(User::exists(array('email' => $parameters['email']))) {
        $app->halt(400, 'The chosen email adress is already chosen');
      }
    }
    
    // validates email
    if (!preg_match('|^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$|i', $parameters['email'])) {
      $app->halt(400, __('login_invalidemail'));
    }
    
    // validates uniqueness of username
    if($parameters['username'] != $user->username) {
      if(User::exists(array('username' => $parameters['username']))) {
        $app->halt(400, 'The chosen username has been taken already');
      }
    }
    
    // validates username
    $parameters['username'] = str_replace(" ", "", $parameters['username']);
    if($parameters['username'] == "") {
      $app->halt(400, "Please enter a username");
    }
    if(!ctype_alnum($parameters['username'])) {
      $app->halt(400, __('login_invaliduser'));
    }
    
    $password = isset($parameters['password']) ? $parameters['password'] : '';
    $password_confirm = isset($parameters['password_confirm']) ? $parameters['password_confirm'] : '';
    
    if($password != "" || $password_confirm != "") {
      
      // attempt on password change
      if($password != $password_confirm) {
      
        // passwords don't match
        $app->halt(400, "Passwords don't match");
        
      }
      else {
        
        $password = generate_hash($password);
        
        $parameters['password'] = $password;
        
        unset($parameters['password_confirm']);
        
      }
      
    }
    
    $user->update_attributes($parameters);
    
    $user->save_session();
    
    respond_with_json($user->to_json(array('except' => array('password', 'reset_token', 'reseted_at'))));
    
  }
  
  else {
  
    $app->notFound();
    
  }
  
})->setMetadata(array( 'protected' => true ));


/**
*  PUT
*  Users update route
*/
$app->put('/users/:id', function($id) use($app) {
  
  $user = User::find_by_id($app->sessions->get_var('user')->id);
  
  if(!$user->role == 'admin') {  
  
    $app->halt(403, 'You are not authorized to perform this action.');
  
  }
  
  $user = User::find_by_id($id);
  
  if($user) {
    
    //Gets the request parameters from a json format sent by backbonejs
    $parameters = json_decode($app->request()->post("model"), true);
    
    // validates max space
    $parameters['max_space'] = intval($parameters['max_space']);
    
    $user->update_attributes($parameters);
    
    respond_with_json($user->to_json(array('except' => array('password', 'reset_token', 'reseted_at'))));
    
  }
  
  else {
  
    $app->notFound();
    
  }
  
})->setMetadata(array( 'protected' => true ));


/**
*  DELETE
*  Users delete route
*/
$app->delete('/users/:id', function($id) use($app) {
  
  $user = User::find_by_id($app->sessions->get_var('user')->id);
  
  if(!$user->role == 'admin') {  
  
    $app->halt(403, 'You are not authorized to perform this action.');
  
  }
  
  $user = User::find_by_id($id);
  
  if($user) {
    
    $user->delete();
    
  }
  
  else {
    
    $app->notFound();
    
  }
  
})->setMetadata(array( 'protected' => true ));

?>