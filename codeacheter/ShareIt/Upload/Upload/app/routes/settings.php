<?php

/**
*  GET
*  Version number get
*/
$app->get('/versionnumber.json', function() use($app) {
  
  $callback = $app->request()->get('callback');
  
  $response = array('versionnumber' => APPVERSION);
  
  $response = json_encode($response);
  
  $response = $callback.'('.$response.')';
  
  respond_with_json($response);
  
})->name('versionnumber');


/**
*  PUT
*  Settings update route
*/
$app->put('/settings', function() use($app) {
  
  $user = User::find_by_id($app->sessions->get_var('user')->id);
  
  if(!$user->role == 'admin') {  
  
    $app->halt(403, 'You are not authorized to perform this action.');
  
  }
  
  //Gets the request parameters from a json format sent by backbonejs
  $parameters = json_decode($app->request()->post("model"), true);
  
  $settings_schema = get_settingsSchema();
  
  foreach($parameters as $new_setting => $value) {
    
    $setting = Settings::find_by_setting_name($new_setting);
    
    $scheme = $settings_schema[$new_setting];
    
    if($scheme['type'] == 'int') {
      $value = intval($value);
    }

    if($scheme['type'] == 'bool') {
      $value = (bool) $value;
    }
    
    $setting->setting_value = $value;
    
    $setting->save();
    
  }
  
})->setMetadata(array( 'protected' => true ));

?>