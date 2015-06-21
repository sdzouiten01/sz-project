<?php

/**
*  Class Object to handle session interactions
*/

class Sessions
{
  
	public function __construct()
	{
	
	  $this->initialize();
	  
	}
	
	/**
	*  Initializes session
	*/
	function initialize() {
	  
	  $app = \Slim\Slim::getInstance();
	  
	  //  Reads the session id paramater sent by the request
	  $session_id = $app->request()->post('PHPSESSID');
	
	  // Initializes php session with a given session id if provided
	  if(isset($session_id) && !empty($session_id)) {
	    session_id($session_id);
	  }
	  
	  $path = $app->config('cookies.path');
	  
	  session_set_cookie_params(0, $path);
	  
	  // Starts the session
	  session_cache_limiter(false);
	  
	  session_start();
	  
	}
	
	/**
	*  Removes an entry from session data
	*  @param string
	*/
	public function remove($name) {
	  if( isset( $_SESSION[$name] ) )
	  {
	  	unset($_SESSION[$name]);
	  }
	}
	
	/**
	*  Sets a session variable
	*  @param key
	*  @param value
	*/
	public function set_var($k, $v)
	{
  	$_SESSION[$k] = $v;
	}
	
	/**
	*  Gets a session variable
	*  @param name
	*/	
	public function get_var( $name )
	{
		if( isset( $_SESSION[$name] ) )
		{
			return $_SESSION[$name];
		}
		else
		{
			return false;
		}
	}
	
	/**
	*  Ends the session
	*/
	public function end() {
	  session_destroy();
	}
	
}

?>