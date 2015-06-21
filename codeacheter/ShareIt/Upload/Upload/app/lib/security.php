<?php

/**
*  Generates secure hash
*/
function generate_hash($value, $rounds = 8)
{

	$work = str_pad($rounds, 2, '0', STR_PAD_LEFT);

	// Bcrypt expects the salt to be 22 base64 encoded characters including
	// dots and slashes. We will get rid of the plus signs included in the
	// base64 data and replace them with dots.
	if (function_exists('openssl_random_pseudo_bytes'))
	{
		$salt = openssl_random_pseudo_bytes(16);
	}
	else
	{
		$salt = substr(str_shuffle(str_repeat('0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ', 5)), 0, 40);
	}
  
	$salt = substr(strtr(base64_encode($salt), '+', '.'), 0 , 22);

	return crypt($value, '$2a$'.$work.'$'.$salt);
}

/**
*  Checks hash
*  @param javascript
*/
function check_hash($value, $hash)
{
	return crypt($value, $hash) === $hash;
}

/**
*  Encrypts string
*/
function decrypt($string) {
  $secret_key = \Slim\Slim::getInstance()->config('secret_key');
  return rtrim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, md5($secret_key), base64_decode($string), MCRYPT_MODE_CBC, md5(md5($secret_key))), "\0");
}

/**
*  Encrypts string
*/
function encrypt($string) {
  $secret_key = \Slim\Slim::getInstance()->config('secret_key');
  return base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_256, md5($secret_key), $string, MCRYPT_MODE_CBC, md5(md5($secret_key))));
}

/**
*  Returns a random alphanumeric character
*/
function rand_alphanumeric() {

  // ascii digits
  $subsets[0] = array('min' => 48, 'max' => 57); 
  
  // ascii uppercase English letters
  $subsets[1] = array('min' => 65, 'max' => 90); 
  
  // ascii lowercase English letters
  $subsets[2] = array('min' => 97, 'max' => 122); 
  
  // random choice between lowercase, uppercase, and digits
  $s = rand(0, 2);
  $ascii_code = rand($subsets[$s]['min'], $subsets[$s]['max']);
  
  return chr( $ascii_code );
  
}

/**
*  Generates unique set of characteres code
*/
function generate_asciiuid($lenght = 15) {
  
  $code = "";
  
  
  for($i = 0; $i < $lenght; $i++) {
    
    $code .= rand_alphanumeric();  
    
  }
  
  return $code;
  
}

/**
*  Generates a secure token based on a secret key
*/
function generate_token() {

  $secret_key = \Slim\Slim::getInstance()->config('secret_key');
  
  //generates random characters
  $prefix = "";
  
  for ($i=0; $i<8; $i++) {
    $prefix .= rand_alphanumeric();
  }
  
  //appends these characters to md5 generated from secret key
  return $prefix . md5($prefix . $secret_key);
  
}

/**
*  Verifies a given token based on secret key
*  @param token
*/
function verify_token($token) {

  $secret_key = \Slim\Slim::getInstance()->config('secret_key');
  
  //gets random generated characters
  $prefix = substr($token, 0, 8);
  
  //generates md5 from prefix
  $md5 = md5($prefix . $secret_key);
  
  //if match return true
  if($prefix . $md5 == $token) {
    return true;
  }
  
  return false;
  
}

?>