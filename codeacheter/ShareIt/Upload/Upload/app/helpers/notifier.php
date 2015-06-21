<?php


/**
*  Sends user password reset email
*/
function notify_reset($user) {
  
  if(!$user->email)
    return false;

  $app = \Slim\Slim::getInstance();
  
  $message = "Hello, <br>";
  
  $message .= "<br> You have requested a link to reset your password. <br>";
  
  $message .= "To reset your password please click the following link: <br>";
  
  $reset_link = app_url() . "/reset/" . $user->reset_token;
  
  $message .= '<a href="'.$reset_link.'">'.$reset_link.'</a> <br>';
  
  $message .= "<br> The reset link will expire after 5 days. <br>Thanks";
  
  $subject = get_config('appname') . ": Account password reset";
  
  mail($user->email, $subject, $message, get_notifier_headers(get_config('admin_email')));
      
}

/**
*  Sends user created notification
*/
function notify_user_create($user) {
  
  $app = \Slim\Slim::getInstance();
  
  $message = "Hello, <br>";
  
  $message .= '<br> A new user has been created on <a href="'.app_url().'">' . get_config('appname') . '</a> <br>';
  
  $message .= "Username: " . $user->username . '<br>';
  $message .= "Email adress: " . $user->email . '<br>';
  
  $subject = get_config('appname') . " : New User Created";
  
  // notifies all admin users of user create
  $admins_list = '';
  $admins = User::all(array(
    'conditions' => array("role =  ?", "admin"),
    'select' => "email"
  ));
  foreach($admins as $admin) {
    $admins_list .= $admin->email.',';
  }
  
  mail($admins_list, $subject, $message, get_notifier_headers(get_config('admin_email')));
      
}

/**
 * Sends activation email
 */
function notify_activation($user) {

  $app = \Slim\Slim::getInstance();
  
  $message = "Hello, <br>";
  
  $message .= '<br> To activate your <a href="'.app_url().'">' . get_config('appname') . '</a> account, please click the following link:<br>';
  
  $activation_link = app_url() . "/activate/" . $user->activation_token;
  
  $message .= '<a href="'.$activation_link.'">'.$activation_link.'</a>';

  $subject = get_config('appname') . " : Account Activation";
  
  mail($user->email, $subject, $message, get_notifier_headers(get_config('admin_email')));

}

/**
*  Sends upload share email
*/
function notify_share($from_email, $emails, $upload, $ad_message = '') {

  $app = \Slim\Slim::getInstance();
  
  $message = "Hello, <br>";
  
  $message .= '<br> '.$from_email.' has shared a file with you from <a href="'.app_url().'">' . get_config('appname') . '</a> <br>';
  
  if($ad_message) {
    $message .= '<br> Attatched Message: <br>';
    $message .= htmlentities($ad_message).'<br>';
  }
  
  $message .= "<br> $upload->name <br>";
  
  $message .= '<a href="'.get_url($upload).'">'.get_url($upload).'</a>';
  
  $subject = $from_email . " shared a file with you";
  
  return mail($emails, $subject, $message, get_notifier_headers($from_email));
      
}

/**
*  Returns default email notifier headers
*/
function get_notifier_headers($from_email) {
  
  $headers = "Reply-To: $from_email\r\n";
  $headers .= "Return-Path: $from_email\r\n";
  $headers .= "From: $from_email\r\n";
  $headers .= 'Organization: '.get_config('appname')."\r\n";
  
  $headers .= 'MIME-Version: 1.0' . "\r\n";
  $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
  
  return $headers;
  
}


?>