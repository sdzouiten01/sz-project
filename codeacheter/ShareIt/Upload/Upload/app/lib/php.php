<?php

/**
* Implode an array with the key and value pair giving
* a glue, a separator between pairs and the array
* to implode.
* @param string $glue The glue between key and value
* @param string $separator Separator between pairs
* @param array $array The array to implode
* @return string The imploded array
*/
function attributes_array($array) {
  if ( ! is_array( $array ) ) return $array;
  
  $string = '';
  $glue = '"';
  foreach ( $array as $key => $val ) {
    $string .= "{$key}={$glue}{$val}{$glue}";  
  }
  return $string;    
}

/**
*  Returns the exceprt of a string
*  @param string
*  @param lenght
*  @param trailing
*  example: javascript_include_tag('script1','script2',etc...)
*/
function truncate($str, $length=140, $trailing='...') {
  // take off chars for the trailing
  $length-=mb_strlen($trailing);
  if (mb_strlen($str)> $length)
  {
    // string exceeded length, truncate and add trailing dots
    return mb_substr($str,0,$length).$trailing;
  }
  else
  {
    // string was already short enough, return the string
    $res = $str;
    return $res;
  }
}

/**
*  Calls a given function
*  @param function
*  example: call(function())
*/
function call($callable) {
  call_user_func($callable);
}

/**
*  Returns an array of files within a directory
*  @param directory
*  example: dir_files('example/path')
*/
function dir_files($dir, $supported_ext = array('js')) {
  $files = array();
  $dir = @opendir($dir);
  while ($file = @readdir($dir)){
    if (!is_dir($dir.$file)){
      $splitted = explode('.', $file);
      $ext = strtolower($splitted[count($splitted)-1]);
      if (in_array($ext, $supported_ext)) $files[] = $file;
    }
  }
  @closedir($dir);
  sort($files);
  return $files;
}

/**
*  Removes directory
*/
function directory_remove($directory, $empty = false) {

  if(substr($directory,-1) == '/')
  {
    $directory = substr($directory,0,-1);
  }
  if(!file_exists($directory) || !is_dir($directory))
  {
    return false;
  }
  elseif(is_readable($directory))
  {
    $handle = opendir($directory);
    while (false !== ($item = readdir($handle)))
    {
      if($item != '.' && $item != '..')
      {
        $path = $directory.'/'.$item;
        if(is_dir($path)) 
        {
      	  recursive_remove_directory($path);
        }
        else{
      	  unlink($path);
        }
      }
    }
    closedir($handle);
    if($empty == false)
    {
      if(!rmdir($directory))
      {
        return false;
      }
    }
  }
  return true;
  
}

/**
*  Returns array of files in directory
*/
function directory_files($directory) {

  $files = scandir($directory);
  
  $length = count($files);
  
  for ($i = 0; $i < $length; $i++) {
    
    $file = $files[$i];
    
    if(substr($file, 0, 1) == '.') {
      
      unset($files[$i]);
      
    }
    
  }
  
  return $files;
  
}

/**
*  Cleans file name
*/
function clean_filename($file) {
  
  if(startsWith($file, '.')) {
    
    $file = substr($file, 1);
    
  }

  $valid_chars_regex = '.A-Z0-9_ !@#$%^&()+={}\[\]\',~`-';
  
  return preg_replace('/[^'.$valid_chars_regex.']|\.+$/i', "", $file);
  
}

/**
*  String starts with comparison
*/
function startsWith($haystack, $needle)
{
    return !strncmp($haystack, $needle, strlen($needle));
}

/**
*  Returns a boolean comparing if string ends with string
*  @param string
*  @param test
*  example: endswith(string, test)
*/
function endswith($string, $test) {
  $strlen = strlen($string);
  $testlen = strlen($test);
  if ($testlen > $strlen) return false;
  return substr_compare($string, $test, -$testlen) === 0;
}

/**
*  Returns a boolean checking if array contains string
*  @param string
*  @param array
*/
function stringContainsArray($source, $target) {
  
  $source = strtolower($source);
  
  foreach($target as $substr) {
  
    $substr = strtolower($substr);
    
    if(strstr($source, strtolower($substr))) {
      
      return true;
    
    }
    
  }
  
  return false;
  
}

?>