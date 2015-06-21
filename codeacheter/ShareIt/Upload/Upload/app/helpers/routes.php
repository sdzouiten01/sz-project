<?php

/**
*  Handles upload
*/
function process_upload($available_space = '') {
  
  $app = \Slim\Slim::getInstance();
  
  // require Upload Library
  require APPPATH.'/vendor/Upload/Autoloader.php';
  \Upload\Autoloader::register();
  
  // Creates new uploader instance
  $dir = app_dir().'uploads/';
  $storage = new \Upload\Storage\FileSystem($dir);
  
  // create file object from upload
  $file = new \Upload\File('file', $storage);
  
  // Validate file upload
  if(is_int($available_space)) {
  
    $file->addValidations(array(
      new \Upload\Validation\Size($available_space)
    ));
    
  }
  
  // sets a unique secure file name
  $file->setName(generate_token().'-'.$file->getName());
  
  // move file into location
  try {
    
    $file->upload();
    
    // returns file info object
    $file_info = new FileInfo($dir.$file->getNameWithExtension());
    
    return $file_info;
    
  } catch(\Exception $e) {
    
    $errors = $file->getErrors();
    
    // swfupload does not accept error http header and data response
    exit(__($errors[0]));
    
    // using correct http heeaders
    //$app->halt(500, __($errors[0]));
    
  }
  
}

/**
*  Paginates uplaods collections
*/ 
function generate_uploads($page = 1, $filter = 'all', $folder = '') {
  
  $app = \Slim\Slim::getInstance();
  
  $settings = $app->config('settings');
  
  $page = (int)$page;
  
  // gets total elements until this page
  $offset = ($page - 1) * $settings['uploads_perpage'];
  
  if($offset < 0) {
    
    $app->halt(400);
    
  }
  
  // TODO: implement option if all users can see all uploads or only admins can see all uploads
  
  // check if user exists
  $user = User::find_by_id($app->sessions->get_var('user')->id);
  if(!$user) {
    //$app->halt(403);
  }
  
  $uploads_public = false;
  
  $parameters = array(
    'offset' => $offset,
    'limit' => $settings['uploads_perpage'],
    'order' => 'created_at desc',
    'conditions' => array('type = ?', $filter)
  );

  if(!$uploads_public) {
    $parameters['conditions'] = array('type = ? AND user_id = ?', $filter, $app->sessions->get_var('user')->id);
  }
  
  // checks filter
  if($filter == 'all') {
    
    $parameters['conditions'] = array('user_id = ?', $app->sessions->get_var('user')->id);
    
  }

  // adds folder condition
  if($folder) {
    $parameters['conditions'][0] = $parameters['conditions'][0] . ' AND folder_id = ?';
    $parameters['conditions'][] = $folder;
  }
  
  // calculates the amount of pages required
  $total = Upload::count(array('conditions' => $parameters['conditions']));
  
  // calculate total pages
  $total_pages = -1;
  if($settings['uploads_perpage'] > 0) {
    $total_pages = Ceil($total / $settings['uploads_perpage']);
  }
  
  // queries for the uploads corresponding to the current page
  $uploads = Upload::all($parameters);
  
  // pagination data sent to the front end
  $response = array(
    'firstPage' => 1,
    'totalItems' => intval($total),
    'offset' => $offset,
    'totalPages' => $total_pages,
    'perPage' => $settings['uploads_perpage'],
    'currentPage' => $page,
    'items' => encode_json_models($uploads, array('except' => array('file')))
  );
  
  return $response;
  
}

/**
*  Search uploads for query
*/
function search_uploads($query) {
  
  $app = \Slim\Slim::getInstance();
  
  // TODO: implement option if all users can see all uploads or only admins can see all uploads
  
  $uploads_public = false;
  
  $parameters = array(
    'order' => 'created_at desc',
    'conditions' => array('name LIKE CONCAT("%", ? ,"%") AND user_id = ?', $query, $app->sessions->get_var('user')->id)
    //'conditions' => array('name LIKE CONCAT("%", ? ,"%")', $query)
  );
  
  if(!$uploads_public) {
  }
  
  // queries for the uploads corresponding to the current page
  $uploads = Upload::all($parameters);
  
  $response = array(
    'items' => encode_json_models($uploads, array('except' => array('file'))),
    'query' => $query,
    'totalItems' => Upload::count($parameters)
  );
  
  return json_encode($response);
  
}

/**
*  Generates users
*/
function generate_users() {
  
  $app = \Slim\Slim::getInstance();

  $id = $app->sessions->get_var('user')->id;

  $users = User::all(array(
    'conditions' => array("id != $id")
  ));
  
  return encode_json_models($users, array('except' => array('password', 'reset_token', 'reseted_at')));
  
}

/**
*  Checks upload password access
*/
function check_upload_access($id) {

  $app = \Slim\Slim::getInstance();
  
  $upload_access = $app->sessions->get_var('upload_access');
  
  if(is_array($upload_access)) {
    
    if(in_array($id, $upload_access)) {
    
      return true;
      
    }
  
  }
  
  return false;
  
}

/**
*  Adds upload to upload password access
*/
function add_upload_access($id) {
  
  $app = \Slim\Slim::getInstance();
  
  $file_acess = array();
  
  if(is_array($app->sessions->get_var('upload_access'))) {
    $file_acess = $app->sessions->get_var('upload_access');
  }
  
  $file_acess[] = $id;
  $app->sessions->set_var('upload_access', $file_acess);
  
}

/**
*  Creates short link
*/
function generate_shortLink($link) {
  
  try {
    
    $params = array(
      
      'longUrl' => "$link"
      
    );
    
    $url = 'https://www.googleapis.com/urlshortener/v1/url?key=AIzaSyBY4uB-jg9ONalue2Aq0A6xpbVlaFpzS9A';
    
    // Create cURL
    $ch = curl_init();
    
    curl_setopt($ch,CURLOPT_URL, $url);
    curl_setopt($ch,CURLOPT_POST,1);
    curl_setopt($ch,CURLOPT_POSTFIELDS, json_encode($params));
    curl_setopt($ch,CURLOPT_HTTPHEADER,array( "Content-Type: application/json" ));
    curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
    
    // Execute the post
    $response = curl_exec($ch);
    
    // Close the connection
    curl_close($ch);
    
    // Return the result
    $response = json_decode($response);
    
    return $response->id;
  
  }
  
  catch(Exception $ex) {  
    
    return "";
  
  }
  
}

/**
 * Checks Upload Expiration
 */
function upload_has_expired($upload) {

  // checks upload expiration
  $expiration_type = $upload->expiration;
  $expiration_value = $upload->expiration_value;

  switch ($expiration_type) {
  
    // expire by date (by days)
    case 'date':
      
      if(strtotime(date("Y-m-d H:i:s")) > strtotime("+$expiration_value day", strtotime($upload->created_at->format("Y-m-d H:i:s")))) {
        
        return true;
        
      }
    
      break;
      
    // expire by downloads
    case 'downloads':
      
      if($upload->downloads >= $expiration_value) {
        
        return true;
        
      }
    
      break;
    
    
  }

  return false;

}

?>