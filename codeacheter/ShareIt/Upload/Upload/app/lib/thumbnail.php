<?php

class Thumbnail {
  
  /**
  *  Thumb default max dimensions
  *  Obtained for a 8M memory limit calculations from
  *  http://www.dotsamazing.com/en/labs/phpmemorylimit
  *  @var array
  */
  protected static $max_dimensions = array(
    'width' => 984,
    'height' => 984
  );
  
  /**
  *  Supported formats
  */
  protected $supported_files = array(
    'png',
    'jpg',
    'jpeg',
    'gif'
  );

  /**
  *  Thumb width
  *  @var int
  */
  protected $width;
  
  /**
  *  Thumb height
  *  @var int
  */
  protected $height;
  
  /**
  *  Thumb image ratio
  *  @var int
  */
  protected $ratio;
  
  /**
  *  Thumb source file path
  *  @var int
  */
  protected $source_file;
  
  /**
  *  Thumb source directory
  *  @var int
  */
  protected $location;
  
  /**
  *  Thumb target file
  *  @var int
  */
  protected $file;
  
  /**
  *  Thumb file name
  *  @var int
  */
  protected $name;
  
  /**
  *  Thumb file extension
  *  @var string
  */
  protected $extension;
  
  /**
  *  Created thumbnail
  *  @var bool
  */
  protected $created;
  
  /**
  *  Source image file info
  *  @var array
  */
  protected $source_info;
  
  /**
  *  Constructor
  *  @param width
  *  @param height
  */
  public function __construct($width, $height, $location, $name, $extension) {
    
    $this->height = $height;
    $this->width = $width;
    $this->location = $location;
    $this->name = $name;
    $this->ratio = $this->height / $this->width;
    $this->extension = $extension;
    
    $this->created = false;
    
    //sets thumbnail name
    $this->file = $this->location . 'thumbnails/thumb_' . $this->name;
    
    // sets thumbnail source file
    $this->source_file = $this->location . $this->name;
    $this->source_info = getimagesize($this->source_file);
    
  }
  
  /**
  *  Returns thumbnail name
  *  @return string
  */
  public function getName() {
    
    if($this->created == true) {
    
      return 'thumb_' . $this->name;
    
    }
      
    return $this->name;
    
  }
  
  /**
  *  Returns thumbnail created state
  *  @return string
  */
  public function getCreated() {
    
    return $this->created;
    
  }
  
  /**
  *  Returns the image object
  *  @return image
  */
  private function getImageObject() {
    
    $img_object;
    
    // determines image format
    switch ($this->extension) {
      
      
      // jpg
      case 'jpg':
      case 'jpeg':
      
        $img_object = imagecreatefromjpeg($this->source_file);
      
      break;
      
      // png
      case 'png':
        
        $img_object = imagecreatefrompng($this->source_file);
      
      break;
      
      // gif
      case 'gif':
        
        $img_object = imagecreatefromgif($this->source_file);
      
      break;
      
    }
    
    return $img_object;
    
  }
  
  /**
  *  Saaves thumbnail file
  *  @param image
  */
  private function saveThumbnail($img_object) {
    
    // saves thumbnail file
    switch ($this->extension) {
      
      // jpg
      case 'jpg':
      case 'jpeg':
        
        imagejpeg($img_object, $this->file, 100);
      
      break;
      
      // png
      case 'png':
        
        imagepng($img_object, $this->file);
      
      break;
      
      // gif
      case 'gif':
        
        imagegif($img_object, $this->file);
      
      break;
      
    }
    
  }
  
  
  /**
  *  Generates image thumbnail from image file
  */
  public function generateThumbnail() {
  
    // checks supported formats
    if(!in_array($this->extension, $this->supported_files)) {
      return;
    }
    
    // checks if library is installed
    if (!function_exists("gd_info")) {
      return;
    }
        
    // exists if image is too big
    if($this->sourceImageTooBig()) {
      return;
    }
    
    // gets image dimensions
    
    $new_height = $this->height;
    $new_width = $this->width;
    
    $width = $this->source_info[0];
    $height = $this->source_info[1];
    
    // checks image is smaller than desired dimensions
    if($width < $this->width && $height < $this->height) {
      
      return;
      
    }
    
    // gets image ratio
    $img_ratio = $height / $width;
    
    if($img_ratio > $this->ratio) {
    
      $x = intval($width * $new_height / $height);
      
      if($x < $new_width) {
        
        $new_height = intval($height * $new_width / $width);
        
      }
      
      else {
        
        $new_width = $x;
        
      }
    
    }
    
    else {
      
      $x = intval($height * $new_width / $width);
      
      if ($x < $new_height)
      {
      
        $new_width = intval($width * $new_height / $height);
      
      } 
      
      else
      {
      
        $new_height = $x;
      
      }
      
    }
    
    // gets image object
    $img_object = $this->getImageObject();
    
    // creates target image object
    $img_object_target = imagecreatetruecolor($this->width, $this->height);
    
    // holds resized image object
    $resized_image = imagecreatetruecolor($new_width, $new_height);
    
    switch ($this->extension) {
      
      
      // jpg
      case 'jpg':
      case 'jpeg':
      
      break;
      
      // png
      case 'png':
        
        imagealphablending($img_object_target, false );
        imagesavealpha($img_object_target, true );
        
        imagealphablending($resized_image, false );
        imagesavealpha($resized_image, true );
      
      break;
      
      // gif
      case 'gif':
      
      break;
      
    }
    
    
    // resize image
    imagecopyresampled($resized_image, $img_object, 0, 0, 0, 0, $new_width, $new_height, $width, $height);
    
    // makes the final cropped thumbnail
    imagecopy($img_object_target, $resized_image, 0, 0, 0, 0, $new_width, $new_height);
    
    // saves image
    $this->saveThumbnail($img_object_target);
    
    // destroys image object
    imagedestroy($img_object);
    imagedestroy($img_object_target);
    imagedestroy($resized_image);
    
    // sets thumbnail created status
    $this->created = true;
    
  }
  
  /**
  *  Determines if source image is too big for memory
  *  @return bool
  */
  private function sourceImageTooBig() {
    
    $width = $this->source_info[0];
    $height = $this->source_info[1];
    $bits = $this->source_info['bits'];
    $tweakfactor = 1.6;
    
  	if (function_exists('memory_get_usage')) {
  	
  		$available_memory = humanReadableToBytes(ini_get('memory_limit')) - memory_get_usage();
  		
  		$image_memory = Ceil($width * $height * $bits * $tweakfactor);
  		
  		if($image_memory > $available_memory) {
  		  
  		  $limit = humanReadableToBytes(ini_get('memory_limit'));
  		  
  		  $needed_memory = $limit + $image_memory;
  		
  		  if($this->setMemeoryLimit($needed_memory) === false) {
  		    
  		    // not able to change memory return false to not create the thumbnail
  		    return true;
  		    
  		  }
  		  
  		}
  		
  		$available_memory = humanReadableToBytes(ini_get('memory_limit')) - memory_get_usage();
  		
  		return (bool) ($image_memory > $available_memory);
  		
  	}
  	
  	return (bool) (($width * $height) > ($this->max_dimensions['width'] * $this->max_dimensions['height']));
  	
  }
  
  /**
  *  Dynamically sets the memory limit to be able to create the thumbnail
  *  @param bytes
  */
  private function setMemeoryLimit($value) {
    
    if(function_exists('ini_set')) {
      
      $needed = bytesToHumanReadbleMB($value);
      
      if(ini_set('memory_limit', $needed.'M') === false) {
        
        // not allowed to change settings
        return false;
        
      }
      
      else {
        
        return true;
        
      }
      
    }
    
    return false;
    
  }
    
}

?>