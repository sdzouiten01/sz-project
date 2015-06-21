<?php

class Upload extends ActiveRecord\Model {
  
  /**
  *  Mass assign protected attributes
  */
  static $attr_accessible = array('name', 'password', 'expiration', 'expiration_value', 'folder_id');

  /**
  *  Getters
  */
  static $getters = array('password');
  
  /**
  *  Setters
  */
  static $setters = array('password', 'name');
  
  /**
  *  Belongs to user relationship
  */
  static $belongs_to = array(
    array('user')
  );
  
  /**
  *  Determines if upload is an image file
  */
  function isImage() {
  
    $thumbnail_extensions = array('jpg', 'jpeg', 'png', 'gif', 'bmp');
    if( ($this->type == 'image') && in_array($this->extension, $thumbnail_extensions)) {
    
      return true;
      
    }
    
    return false;
    
  }
  
  /**
  *  Removes files associated with this upload
  */
  function destroy_files() {
    
    try {
      
      // removes thumbnail
      if($this->thumbnail && file_exists(app_dir().'uploads/thumbnails/thumb_'.$this->file)) {
        unlink(app_dir().'uploads/thumbnails/thumb_'.$this->file);
      }
      
      // removes file
      if(file_exists(app_dir().'uploads/'.$this->file)) {
        unlink(app_dir().'uploads/'.$this->file);
      }
      
      return true;
            
    }
    catch (exception $ex) {
      
      return false;
      
    }
    
  }
  
  /**
  *  Generates thumbnail for image type uploads
  */
  function generate_thumbnail() {
  
    if($this->isImage()) {
      
      $thumbnail = new Thumbnail(170, 160, app_dir().'uploads/', $this->file, $this->extension);
      
      $thumbnail->generateThumbnail();
      
      $this->thumbnail = 'original';
      
      if($thumbnail->getCreated()) {
        $this->thumbnail = 'cropped';
      }
      
    }
    
  }
  
  /**
  *  Process upload delete
  */
  static $before_destroy = array('before_destroy');
  function before_destroy() {
  
    $this->destroy_files();
    
    // removes used file space from user
    $this->user->used_space -= $this->size;
    $this->user->uploads_count -= 1;
    $this->user->save();
  
  }
  
  /**
  *  Process upload update
  */
  static $before_save = array('before_save');
  function before_save() {
    
    $this->generate_thumbnail();
    
  }
  
  /**
  *  Process new uploads
  */
  static $after_create = array('after_create');
  function after_create() {
    
    $this->user->used_space += $this->size;
    $this->user->uploads_count += 1;
    $this->user->save();
    
  }
  
  
  /**
  *  Cleans name
  */
  function set_name($name) {
    
    $clean_name = clean_filename($name);
    
    if(trim($clean_name) == "") {
      
      $this->assign_attribute('name', $this->read_attribute('name'));
    
    }
    else {
      
     $this->assign_attribute('name', $clean_name);
      
    }
    
  }
  
  /**
  *  Encrypts password
  */
  function set_password($plaintext) {
    
    if(trim($plaintext) != "") {
    
      $this->assign_attribute('password', encrypt($plaintext));
      
    }
    
    else {
      
      $this->assign_attribute('password', NULL);
      
    }
    
  }
  
  /**
  *  Decrypts password
  */
  function get_password() {
    
    if($this->read_attribute('password')) {
    
      return decrypt($this->read_attribute('password'));
      
    }
    
  }
  
  /**
  *  Gets password hash
  */
  function get_passwordhash() {
    
    return $this->read_attribute('password');
    
  }
  
  /**
  *  Gets upload url link
  */
  function get_link() {
    
    return app_url()."/".$this->uid;
    
  }

}

?>