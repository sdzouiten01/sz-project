<?php

/**
*  Generates file categories html list
*/
function generate_categories() {
  
  $html = "<ul>".PHP_EOL;
  
  $app = \Slim\Slim::getInstance();
  
  $categories = array(
    'all',
    'audio',
    'image',
    'video',
    'document',
    'other'
  );
  
  // generates each category html
  foreach($categories as $category) {
    $class = ($category == 'all') ? "active" : "";
    $href = $app->urlFor('uploads_pagination', array( 'page' => 1, 'filter' => $category));
    $html .= '<li data-type="'.$category.'" class="'.$class.'"><a href="'.$href.'"> <span class="icon '.$category.'"></span> '.ucfirst($category).' <span class="counter">12</span> </a></li>'.PHP_EOL;
    
  }
  
  $html .= "<ul>";
  
  return $html;
  
}

/**
*  Generates upload url
*/
function get_url($upload) {
  
  if($upload->url) {
    return $upload->url;
  }

  return app_url()."/".$upload->uid;
  
}

/**
*  Returns upload protected status
*/
function is_protected($upload) {
  
  if(!$upload->password || ($upload->password && check_upload_access($upload->uid))) {
    
    return false;
    
  }
  
  return true;
  
}

/**
*  Returns json from flash messages
*/
function get_flashMessages() {
  
  $app = \Slim\Slim::getInstance();
  
  $flash = $app->view()->getData('flash');
  
  $messages = $flash->getMessages();
  
  $response = array();
  
  foreach($messages as $key => $val) {
    
    $response[] = array('type' => $key, 'message' => $val);
    
  }
  
  return json_encode($response);
  
}

/**
*  Returns settings data schema
*/
function get_settingsSchema($key = '') {
  
  $schema = array(
    
    'admin_email' => array( 'label' => __('Administrator Email'), 'type' => 'text'),
    
    'appname' => array( 'label' => __('Application Name'), 'type' => 'text'),
    
    'url' => array( 'label' => __('Application Url, <small>without http(s)://</small>'), 'type' => 'text'),
    
    'timezone' => array( 'label' => __('Timezone'), 'type' => 'select'),
    
    'default_maxspace' => array( 'label' => __('Default User Maxspace (bytes)'), 'type' => 'int'),
    
    'uploads_maxupload' => array( 'label' => __('Server Max Upload Limit (bytes)'), 'type' => 'int'),
    
    'uploads_maxsimuploads' => array( 'label' => __('Maximum Simultaneous Uploads'), 'type' => 'int'),
    
    'uploads_perpage' => array( 'label' => __('Uploads per Page'), 'type' => 'int'),

    'allow_newusers' => array( 'label' => __('Allow new user registration'), 'type' => 'bool')
    
  );
  
  if($key) {
  
    return $schema[$key];
    
  }
  
  return $schema;
  
}

/**
 * Returns upload preview capability
 */
function upload_supports_preview($upload) {

  if($upload->thumbnail != 'none') {
  
    return true;
    
  }
  
  return false;
  
}

?>