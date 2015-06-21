<?php

class FileInfo extends \SplFileInfo
{

  /**
  * Lookup hash to convert file units to bytes
  * @var array
  */
  protected static $units = array(
    'b' => 1,
    'k' => 1024,
    'm' => 1048576,
    'g' => 1073741824
  );
  
  /**
  * File
  * @var string
  */
  protected $file;
  
  /**
  * File extension (without leading dot)
  * @var string
  */
  protected $extension;
  
  /**
  * File mimetype (e.g. "image/png")
  * @var string
  */
  protected $mimetype;
  
  /**
  * File size
  * @var int
  */
  protected $size;
  
  /**
  * Initial file type
  * @var string
  */
  protected $initial_type;
  
  /**
  * File type
  * @var string
  */
  protected $type;
  
  /**
  * Secure file location
  * @var string
  */
  protected $secure_location;
  
  /**
  * Constructor
  */
  public function __construct($file)
  {
  
    parent::__construct($file);
    
    $this->file = $file;
    $this->type = 'other';
    $this->name = parent::getFilename();
    
    $this->secure_location = md5(mt_rand(0,1000) . implode(getdate()) . $this->name) . '-' . $this->name;
    
  }
  
  /**
  * Get file extension (without leading dot)
  * @return string
  */
  public function getExtension()
  {
    return strtolower(pathinfo($this->file, PATHINFO_EXTENSION));
  }
  
  /**
   * Get file upload name
   * @return string
   */
  public function getUploadName()
  {
    return $this->secure_location;
  }
  
  /**
  *  Get file name from secure location in file name
  */
  public function getNameFromSecureLocation() {
  
    return substr(strstr($this->name, '-'), 1);
    
  }
  
  /**
  * Get mimetype
  * @return string
  */
  public function getMimetype()
  {
  
    // mime types list
    $mime_types = array(
      'hqx'   => 'application/mac-binhex40',
      'cpt'   => 'application/mac-compactpro',
      'csv'   => 'text/x-comma-separated-values',
      'bin'   => 'application/macbinary',
      'dms'   => 'application/octet-stream',
      'lha'   => 'application/octet-stream',
      'lzh'   => 'application/octet-stream',
      'exe'   => 'application/octet-stream',
      'class' => 'application/octet-stream',
      'psd'   => 'image/vnd.adobe.photoshop',
      'so'    => 'application/octet-stream',
      'sea'   => 'application/octet-stream',
      'dll'   => 'application/octet-stream',
      'oda'   => 'application/oda',
      'pdf'   => 'application/pdf',
      'ai'    => 'application/postscript',
      'eps'   => 'application/postscript',
      'ps'    => 'application/postscript',
      'smi'   => 'application/smil',
      'smil'  => 'application/smil',
      'mif'   => 'application/vnd.mif',
      'xls'   => 'application/excel',
      'ppt'   => 'application/powerpoint',
      'wbxml' => 'application/wbxml',
      'wmlc'  => 'application/wmlc',
      'dcr'   => 'application/x-director',
      'dir'   => 'application/x-director',
      'dxr'   => 'application/x-director',
      'dvi'   => 'application/x-dvi',
      'gtar'  => 'application/x-gtar',
      'gz'    => 'application/x-gzip',
      'php'   => 'text/x-php',
      'php4'  => 'application/x-httpd-php',
      'php3'  => 'application/x-httpd-php',
      'phtml' => 'application/x-httpd-php',
      'phps'  => 'application/x-httpd-php-source',
      'js'    => 'application/x-javascript',
      'swf'   => 'application/x-shockwave-flash',
      'sit'   => 'application/x-stuffit',
      'tar'   => 'application/x-tar',
      'tgz'   => 'application/x-tar',
      'xhtml' => 'application/xhtml+xml',
      'xht'   => 'application/xhtml+xml',
      'zip'   => 'application/x-zip',
      'mid'   => 'audio/midi',
      'midi'  => 'audio/midi',
      'mpga'  => 'audio/mpeg',
      'mp2'   => 'audio/mpeg',
      'mp3'   => 'audio/mp3',
      'aif'   => 'audio/x-aiff',
      'aiff'  => 'audio/x-aiff',
      'aifc'  => 'audio/x-aiff',
      'ram'   => 'audio/x-pn-realaudio',
      'rm'    => 'audio/x-pn-realaudio',
      'rpm'   => 'audio/x-pn-realaudio-plugin',
      'ra'    => 'audio/x-realaudio',
      'rv'    => 'video/vnd.rn-realvideo',
      'wav'   => 'audio/x-wav',
      'bmp'   => 'image/bmp',
      'gif'   => 'image/gif',
      'jpeg'  => 'image/jpeg',
      'jpg'   => 'image/jpeg',
      'jpe'   => 'image/jpeg',
      'png'   => 'image/png',
      'tiff'  => 'image/tiff',
      'tif'   => 'image/tiff',
      'css'   => 'text/css',
      'html'  => 'text/html',
      'htm'   => 'text/html',
      'shtml' => 'text/html',
      'txt'   => 'text/plain',
      'text'  => 'text/plain',
      'log'   => 'text/plain',
      'rtx'   => 'text/richtext',
      'rtf'   => 'text/rtf',
      'xml'   => 'text/xml',
      'xsl'   => 'text/xml',
      'mpeg'  => 'video/mpeg',
      'mpg'   => 'video/mpeg',
      'mpe'   => 'video/mpeg',
      'qt'    => 'video/quicktime',
      'mov'   => 'video/quicktime',
      'avi'   => 'video/x-msvideo',
      'movie' => 'video/x-sgi-movie',
      'doc'   => 'application/msword',
      'docx'  => 'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
      'xlsx'  => 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
      'word'  => 'application/msword',
      'xl'    => 'application/excel',
      'eml'   => 'message/rfc822',
      'json'  => 'application/json'
    );
    
    $default_mimetype = "application/octet-stream";
    
    // Fileinfo extension is loaded
    if(function_exists('finfo_open')) {
      
      if (!isset($this->mimeType)) {
      
        $finfo = new \finfo(FILEINFO_MIME);
        
        $mimetype = $finfo->file($this->file);
        
        $mimetypeParts = preg_split('/\s*[;,]\s*/', $mimetype);
        
        $this->mimetype = strtolower($mimetypeParts[0]);
        
        unset($finfo);
        
      }
      
    }
    else {
      
      $extension = $this->getExtension();
      
      $this->mimetype = $default_mimetype;
      
      // match extension with mime type list
      if(array_key_exists($extension, $mime_types)) {
        
        $this->mimetype = $mime_types[$extension];
        
      }
      
    }
    
    return $this->mimetype;
    
  }
  
  /**
  * Assigns a file category
  * @return string
  */
  public function getFileType()
  {
  
    $type = $this->getMimetype();
    
    // image
    if(strstr($type, 'image')) {
        $this->type = 'image';
    }
    
    // video
    if(strstr($type, 'video')) {
        $this->type = 'video';
    }
    
    // audio
    if(strstr($type, 'audio')) {
        $this->type = 'audio';
    }
    
    $doc_list = array('pdf', 'text', 'word', 'excel', 'pages', 'numbers');
    if(stringContainsArray($type, $doc_list) || stringContainsArray($this->initial_type, $doc_list) || in_array($this->getExtension(), $doc_list)) {
      $this->type = 'document';
    }
    
    return $this->type;
    
  }
  
  /**
  *  Returns supports thumbnail bool
  */
  function supportsThumbnail() {
  
    $thumbnail_extensions = array('jpg', 'jpeg', 'png', 'gif', 'bmp');
    if( ($this->getFileType() == 'image') && in_array($this->getExtension(), $thumbnail_extensions)) {
    
      return true;
      
    }
    
    return false;
    
  }
  
  /**
  *  Copies file to new location
  */
  public function move($location)
  {
  
    if(file_exists($this->file)) {
      
      if(copy($this->file, $location)) {
      
        unlink($this->file);
        
        return true;
        
      }
      
    }
    
    return false;
  
  }
  
  /**
  *  Deletes file
  */
  public function delete() {
    
    if(file_exists($this->file)) {
      
      if(unlink($this->file)) {
        
        return true;
        
      }
      
    }
    
    return false;
    
  }
  
}

?>