<?php

/**
*  POST, GET
*  Login route
*/
$app->map('/login', function() use ($app) {
  
  if($app->request()->isPost()) {
    
    //If valid login, set auth cookie and redirect
    $email = $app->request()->post('email');
    $password = $app->request()->post('password');
    
    //performs login attempt
    $user = User::find_by_email($email);
    
    if(($user && check_hash($password, $user->password)) || ($user && $user->imported && (md5('_password_'.$password) == $user->password))) {

      if($user->activation_token) {
        $app->flashNow('error', 'Account has not been activated yet');
        $app->redirect($app->urlFor('login'));
      }
      else {

        //logs user in
        $user->login();
        
        //redirect
        $app->redirect($app->urlFor('root'));

      }
      
    }
    
    // check if username/password matches records
    else {
      
      $user = User::find_by_username($email);
      if(($user && check_hash($password, $user->password)) || ($user && $user->imported && (md5('_password_'.$password) == $user->password))) {

        if($user->activation_token) {
          $app->flash('error', 'Account has not been activated yet');
          $app->redirect($app->urlFor('login'));
        }
        else {

          //logs user in
          $user->login();
          
          //redirect
          $app->redirect($app->urlFor('root'));

        }
        
      }
      
    }
    
    $app->flashNow('error', __('login_wronglogin'));
  
  }
  
  // renders
  $app->render('layouts/login.php', array(
    'yield' => 'login/main'
  ));
  
})->via('GET', 'POST')->name('login');

// Register route based on option
if(get_config('allow_newusers')) {

/**
*  POST, GET
*  User register route
*/
$app->map('/register', function() use ($app) {

  if($app->request()->isPost()) {
  
    $email = $app->request()->post('email');
    $username = $app->request()->post('username');
    $password = $app->request()->post('password');
    $password_confirm = $app->request()->post('password_confirm');
    $hasErrors = false;
    
    
    // validates empty
    if(trim($email) == "" || trim($password) == "" || trim($password_confirm) == "" || trim($username) == "") {
      
      $app->flashNow('error', __("login_emptyfields"));
      
      $hasErrors = true;
      
    }
    
    // validates email
    if (!preg_match('|^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$|i', $email)) {
      
      $app->flashNow('error', __('login_invalidemail'));
      
      $hasErrors = true;
      
    }
    
    // validates username
    if(trim($username) != "" && !ctype_alnum($username)) {
      
      $app->flashNow('error', __('login_invaliduser'));
      
      $hasErrors = true;
      
    }
    
    // validates password match
    if($password != $password_confirm) {
    
      $app->flashNow('error', __('login_passwordmatch'));
      
      $hasErrors = true;
      
    }
    
    $user = User::find_by_email($email);
    
    if($hasErrors == false) {
    
      if(!$user) {
      
        // Checks unique username
        $user = User::find_by_username($username);
        
        if(!$user) {
        
          // TODO: Create user via confirmation email( generate link and activate it from link)
          
            // registers user
            $user = User::create(array(
              
              'email' => $email,
              'username' => $username,
              'password' => $password,
              'activation_token' => generate_token()
            
            ));
            
            // $user->login();

            $app->flash('success', 'An activation code has been sent to your email.');
            
            $app->redirect($app->urlFor('login'));
          
        }
        
        else {
          
          $app->flashNow('error', __('login_takenuser'));
          
        }
        
      }
      else {
        
        $app->flashNow('error', __('login_takenemail'));
        
      }
      
    }
  
  }
  
  // renders
  $app->render('layouts/login.php', array(
    'yield' => 'login/register'
  ));
  
})->via('GET', 'POST')->name('register');

}

/**
 * GET
 * User activation route
 */
$app->get('/activate/:token', function($token) use ($app) {

  if(verify_token($token)) {
  
    $user = User::find_by_activation_token($token);

    if(!$user) $app->redirect($app->urlFor('root'));

    $user->activate();

    $user->login();

    $app->redirect($app->urlFor('root'));

  }

})->name('activate');

/**
*  POST, GET
*  User reset password route
*/
$app->map('/reset', function() use ($app) {
  
  if($app->request()->isPost()) {
    
    $hasErrors = false;
    
    $email = $app->request()->post('email');
    
    if (!preg_match('|^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$|i', $email)) {
      
      $app->flash('error', __('login_invalidemail'));
      
      $hasErrors = true;
    
    }
    
    if($hasErrors == false) {
    
      $user = User::find_by_email($email);
      
      if(!$user) {
        
        $app->flash('error', __('login_emailexist'));
        
      }
      else {
        
        $user->reset();
        
        $app->flash('success', __('login_reset'));
        
      }
      
    }
    
    $app->redirect($app->urlFor('reset'));
    
  }

  $app->render('layouts/login.php', array(
    'yield' => 'login/reset'
  ));
  
})->via('GET', 'POST')->name('reset');

/**
*  POST, GET
*  User reset token route
*/
$app->map('/reset/:token', function($token) use($app) {
  
  // reset tokens lasts 5 days
  if(verify_token($token)) {
  
    $user = User::find_by_reset_token($token);
    
    if($user) {
      
      // verify token expiration
      if(strtotime(date("Y-m-d H:i:s")) > strtotime("+5 day", strtotime($user->reseted_at->format("Y-m-d H:i:s")))) {
        
        // clears out token
        $user->update_attributes(array(
          'reset_token' => "",
          'reseted_at'  => ""
        ));
        
        $app->notFound();
        
      }
      
      else {
        
        if($app->request()->isPost()) {
          
          $hasErrors = false;
          
          $password = $app->request()->post('password');
          $password_confirm = $app->request()->post('password_confirm');
          
          // validates password match
          if($password != $password_confirm) {
          
            $app->flashNow('error', __('login_passwordmatch'));
            $hasErrors = true;
            
          }
          
          // validates empty
          if($password == "" || $password_confirm == "") {
            
            $app->flashNow('error', __("login_emptyfields"));
            
            $hasErrors = true;
            
          }
          
          if($hasErrors === false) {
            
            // changes password
            $user->update_attributes(array(
              'password'    => generate_hash($password),
              'reset_token' => "",
              'reseted_at'  => ""
            ));
            
            $app->flash('success', __('login_resetsucess'));
            
            $app->redirect($app->urlFor('login'));
            
          }
          
        }
        
        $app->render('layouts/login.php', array(
          'yield' => 'login/reset_token',
          'token' => $token
        ));
      
      }
      
    }
    
    else {
      
      $app->notFound();
      
    }
  
  }
  else {
    
    $app->notFound();
    
  }
  
})->via('GET', 'POST')->name('reset_token');


/**
*  GET
*  User logout route
*/
$app->get('/logout', function () use($app) {

  //Remove auth cookie and redirect to login page
  $app->deleteCookie('au');
  $app->deleteCookie('tc');
  
  //removes user data from session
  $app->sessions->remove('user');
  $app->sessions->end();
  
  $app->redirect($app->urlFor('login'));
  
})->name('logout');

?>