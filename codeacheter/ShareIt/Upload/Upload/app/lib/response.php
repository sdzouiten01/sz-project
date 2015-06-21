<?php

/**
* Responds with JSON type content
* @param response
* @param status
*/
function respond_with_json($response = '', $status = '200') {
  $app = \Slim\Slim::getInstance();
  $res = $app->response();
  $res['Content-Type'] = 'application/json';
  $app->halt($status, $response);
}

/**
* Responds with empty
* @param response
* @param status
*/
function respond_with_json_empty($status, $type = "application/json") {
  header("Content-Type: $type");
  header("Status: $status");
  exit("");
}

/**
*  Responds with file download
*  @param file
*  @param name
*  @param extension
*  @param mime_type
*/
function respond_with_download($file, $name, $extension, $mime_type, $inline = true) {
  
  // required for IE, otherwise Content-disposition is ignored
  if(ini_get('zlib.output_compression')) {
   
    ini_set('zlib.output_compression', 'Off');
    
  }
    
  //check if file exists
  if(!file_exists($file)) {
    return;
  }
  
  // holds allowed file type mime types
  $valid_inline_files_types = array(
    'image', 'audio', 'video'
  );
  
  // holds allowed file type extensions
  $valid_inline_files_extension = array(
    'pdf'
  );
  
  // declares content disposition
  
  $content_disposition = "Content-Disposition: attatchment; filename=\"".$name."\"";
  
  if(stringContainsArray($mime_type, $valid_inline_files_types) || stringContainsArray($extension, $valid_inline_files_extension)) {
    
    $content_disposition = "Content-Disposition: filename=\"".$name."\"";
    
  }
  
  header_remove('X-Powered-By');
  
  header("Content-Type: $mime_type");
  
  header('Content-Length: ' . filesize($file));
  
  header($content_disposition);
  
  ob_clean();
  
  flush();
  
  readfile($file);
  
  exit();
  
  // compares safe inline files
//  $inline_files = array('image');
//  if(stringContainsArray($mime_type, $inline_files)) {
//    header("Content-Disposition: inline; filename=\"".$name."\"");
//  }
//  else {
//    header("Content-Disposition: attatchment; filename=\"".$name."\"");
//  }
//  
//  header("Content-Type: $mime_type");
//  header('Content-Length: ' . filesize($file));
//  
//  ob_clean();
//  flush();
//  readfile($file);  
//  
//  exit();
  
}


?>