<?php

class Uploader {
  
  /**
  *  @var array
  */
  protected $options;

  
  /**
  *  Constructor
  *  @param options
  */
  function __construct($options=null) {
    
    //loads default values
    $this->options = array(
    
      //file routes
      'upload_dir' => dirname($_SERVER['SCRIPT_FILENAME']).'/uploads/',
      'param_name' => 'files',
      // The php.ini settings upload_max_filesize and post_max_size
      // take precedence over the following max_file_size setting:
      'max_file_size' => null,
      'min_file_size' => 1,
      'accept_file_types' => '/.+$/i',
      // Set the following option to false to enable resumable uploads:
      'discard_aborted_uploads' => true,
      
    );
    
    if ($options) {
      $this->options = array_replace_recursive($this->options, $options);
    }
    
  }
  
  /**
  *  Cleans file name
  *  @param matches
  */
  protected function upcount_name_callback($matches) {
    $index = isset($matches[1]) ? intval($matches[1]) + 1 : 1;
    $ext = isset($matches[2]) ? $matches[2] : '';
    return ' ('.$index.')'.$ext;
  }
  protected function upcount_name($name) {
    return preg_replace_callback('/(?:(?: \(([\d]+)\))?(\.[^.]+))?$/', array($this, 'upcount_name_callback'), $name, 1);
  }
  protected function trim_file_name($name, $type, $index) {
    // Remove path information and dots around the filename, to prevent uploading
    // into different directories or replacing hidden system files.
    // Also remove control characters and spaces (\x00..\x20) around the filename:
    $file_name = trim(basename(stripslashes($name)), ".\x00..\x20");
    // Add missing file extension for known image types:
    if (strpos($file_name, '.') === false && preg_match('/^image\/(gif|jpe?g|png)/', $type, $matches)) {
      $file_name .= '.'.$matches[1];
    }
    if ($this->options['discard_aborted_uploads']) {
      while(is_file($this->options['upload_dir'].$file_name)) {
        $file_name = $this->upcount_name($file_name);
      }
    }
    return $file_name;
  }
  
  /**
  *  Validates file upload
  *  @param file upload
  *  @param file object
  *  @pram error
  *  @param file index
  */
  protected function validate($uploaded_file, $file, $error, $index) {
    
    //returns error if already has
    if ($error) {
      $file->error = $error;
      return false;
    }
    
    //verify file name
    if (!$file->name) {
      $file->error = 'missingFileName';
      return false;
    }
    
    //verify allowed file types
    if (!preg_match($this->options['accept_file_types'], $file->name)) {
      $file->error = 'acceptFileTypes';
      return false;
    }
    
    //gets file size
    if ($uploaded_file && is_uploaded_file($uploaded_file)) {
      $file_size = filesize($uploaded_file);
    } else {
      $file_size = $_SERVER['CONTENT_LENGTH'];
    }
    
    //checks for maximum upload file size
    if ($this->options['max_file_size'] && ($file_size > $this->options['max_file_size'] ||  $file->size > $this->options['max_file_size']) ) {
      $file->error = 'maxFileSize';
      return false;
    }
    
    //verify minimum file size
    if ($this->options['min_file_size'] && $file_size < $this->options['min_file_size']) {
      $file->error = 'minFileSize';
      return false;
    }
    
    //returns valid if no errors were detected
    return true;
  }
  
  /**
  *  Returns an html script tag 
  *  @param file object
  *  @param file name
  *  @param file size
  *  @param file type
  *  @pram errors
  *  @param file index
  */
  protected function handle_file_upload($uploaded_file, $name, $size, $type, $error, $index, $stage) {
    
    //holds a new file object
    $file = new stdClass();
    $file->name = $this->trim_file_name($name, $type, $index);
    $file->size = intval($size);
    $file->type = $type;
    $file->id = uniqid();
    $file->file_path = md5(mt_rand(0,1000) . implode(getdate()) . $file->name. 'file') .'-'.$file->name;
    
    //checks if current file is a valid upload
    if ($this->validate($uploaded_file, $file, $error, $index)) {
    
      //change this: prepend id to path
      $file_path = $this->options['upload_dir'].$file->file_path;
      
      //resumeable uploads
      $append_file = !$this->options['discard_aborted_uploads'] && is_file($file_path) && $file->size > filesize($file_path);
      
      //clears cache
      clearstatcache();
      
      //proceed if file is actual file upload
      if ($uploaded_file && is_uploaded_file($uploaded_file)) {
        // multipart/formdata uploads (POST method uploads)
        if ($append_file) {
          file_put_contents(
            $file_path,
            fopen($uploaded_file, 'r'),
            FILE_APPEND
          );
        } else {
          move_uploaded_file($uploaded_file, $file_path);
        }
      }
      
      else {
        // Non-multipart uploads (PUT method support)
        file_put_contents(
          $file_path,
          fopen('php://input', 'r'),
          $append_file ? FILE_APPEND : 0
        );
      }
      
      $file_size = filesize($file_path);
      
      //checks for file upload abort
      if ($file_size === $file->size) {
        $file->token = generate_token();
      }
      else if ($this->options['discard_aborted_uploads']) {
        //deletes file if aborted
        unlink($file_path);
        $file->error = 'abort';
      }
      
      //sets file attributes
      $file->size = $file_size;
      
    }
    
    return $file;
  }
  
  /**
  *  Handles File Post Uploads
  */
  public function post($callback=null, $stage = false) {
    
    //holds file upload object
    $upload = isset($_FILES[$this->options['param_name']]) ? $_FILES[$this->options['param_name']] : null;
    
    //will hold all the file information
    $upload_info = array();
    
    if ($upload && is_array($upload['tmp_name'])) {
    
      // param_name is an array identifier like "files[]",
      // $_FILES is a multi-dimensional array:
      foreach ($upload['tmp_name'] as $index => $value) {
        
        //adds a new file object to the info array
        $upload_info[] = $this->handle_file_upload(
          //file
          $upload['tmp_name'][$index],
          //name
          isset($_SERVER['HTTP_X_FILE_NAME']) ? $_SERVER['HTTP_X_FILE_NAME'] : $upload['name'][$index],
          //size
          isset($_SERVER['HTTP_X_FILE_SIZE']) ? $_SERVER['HTTP_X_FILE_SIZE'] : $upload['size'][$index],
          //type
          isset($_SERVER['HTTP_X_FILE_TYPE']) ? $_SERVER['HTTP_X_FILE_TYPE'] : $upload['type'][$index],
          //errors
          $upload['error'][$index],
          //index
          $index,
          //stage
          $stage
        );
      }
    }
    
    elseif ($upload || isset($_SERVER['HTTP_X_FILE_NAME'])) {
      // param_name is a single object identifier like "file",
      // $_FILES is a one-dimensional array:
      
      //adds a new file object to the info array
      $upload_info[] = $this->handle_file_upload(
        //file
        isset($upload['tmp_name']) ? $upload['tmp_name'] : null,
        //name
        isset($_SERVER['HTTP_X_FILE_NAME']) ? $_SERVER['HTTP_X_FILE_NAME'] : (isset($upload['name']) ? $upload['name'] : null),
        //size
        isset($_SERVER['HTTP_X_FILE_SIZE']) ? $_SERVER['HTTP_X_FILE_SIZE'] : (isset($upload['size']) ? $upload['size'] : null),
        //type
        isset($_SERVER['HTTP_X_FILE_TYPE']) ? $_SERVER['HTTP_X_FILE_TYPE'] : (isset($upload['type']) ? $upload['type'] : null),
        //error
        isset($upload['error']) ? $upload['error'] : null,
        null,
        //stage
        $stage
      );
    }
    
    //calls function callback if provided with one
    if(is_callable($callback)) {
      call_user_func($callback, $upload_info);
    }
    
    //for security removes sensitive attributes
    foreach($upload_info as $file) {
      unset($file->file_path);
      unset($file->id);
    }
    
    header('Vary: Accept');
    
    //encodes the output file object
    $json = json_encode($upload_info);
    
    //handles redirect request parameter
    $redirect = isset($_REQUEST['redirect']) ? stripslashes($_REQUEST['redirect']) : null;
    if ($redirect) {
      header('Location: '.sprintf($redirect, rawurlencode($json)));
      return;
    }
    
    //if supported by server, outputs application/json content type
    if (isset($_SERVER['HTTP_ACCEPT']) &&
      (strpos($_SERVER['HTTP_ACCEPT'], 'application/json') !== false)) {
      header('Content-type: application/json');
    } else {
      header('Content-type: text/plain');
    }
    
    //outputs the encoded json
    echo $json;
    
  }
  
  /**
  *  Handles File Delete
  */
  public function delete($file) {
    $file = $this->options['upload_dir'].$file;
    if(is_file($file)) {
      unlink($file);  
    }
  }
    
}

?>