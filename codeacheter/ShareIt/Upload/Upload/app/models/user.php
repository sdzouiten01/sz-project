<?php 
class User extends ActiveRecord\Model {
  
  /**
  *  Has many uploads association
  */
  static $has_many = array(
    array('uploads')
  );
  
  /**
  *  Process user delete
  */
  static $before_destroy = array('before_destroy');
  function before_destroy() {
    
    // destroys all uploads related to this user
    foreach($this->uploads as $upload) {
      $upload->delete();
    }
    
  }
  
  /**
  *  Process user creat
  */
  static $before_create = array('before_create');
  function before_create() {
    
    //adds user privileges
    $this->password = generate_hash($this->password);
    
    // remove spaces from username
    $this->username = str_replace(" ", "", $this->username);
    
    // assigns default space limit
    $this->max_space = get_config('default_maxspace');
    
    // notifies user create
    notify_user_create($this);

    // notify activation
    notify_activation($this);
  
  }
  
  /**
  *  Before save
  */
  static $before_save = array('before_save');
  function before_save() {
    
    // prevents negative numbers
    if($this->uploads_count < 0) $this->uploads_count = 0;
    if($this->used_space < 0) $this->used_space = 0;
    
  }
  
  /**
  *  Determines user priviledges
  */
  function hasRights() {
  
    if($this->role == 'admin') {
      
      return true;
      
    }
    
    return false;
  }
  
  /**
  *  Performs user login
  */
  function login($set_cookies = true) {
  
    if($set_cookies) {
    
      $app = \Slim\Slim::getInstance();
      
      //sets cookies
      $app->setEncryptedCookie('au', $this->id);
      $app->setEncryptedCookie('tc', $this->email);
      
    }
    
    $this->save_session();
    
  }
  
  /**
  *  Saves user to session
  */
  function save_session() {
    
    $app = \Slim\Slim::getInstance();
    
    $user = clone $this;
    
    //adds user to session data
    $user->unset_attributes(array(
      'password',
      'created_at',
      'updated_at',
      'reset_token',
      'reseted_at'
    ));
    
    //adds user privileges
    $user->assign_attribute('privileges', $user->hasRights());
    
    $app->sessions->set_var('user', $user);
    
    return $user;
    
  }
  
  /**
  *  Resets user password
  */
  function reset() {
  
    $this->update_attributes(array(
      'reset_token' => generate_token(),
      'reseted_at' => date('Y-m-d H:i:s')
    ));
    
    notify_reset($this);
    
  }

  /**
   * Activates User
   */
  function activate() {
    $this->update_attributes(array(
      'activation_token' => ""
    ));
  }
  
  /**
  *  Gets available space
  */
  function availableSpace($withOutSize = 0) {
  
    if($this->max_space > 0) {
    
      $available_space = $this->max_space - ($this->used_space - $withOutSize);
      
      return (int)$available_space;
      
    }
    
    return false;
    
  }
  
  
}
?>