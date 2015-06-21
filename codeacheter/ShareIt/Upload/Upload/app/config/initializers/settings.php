<?php

// Gets settings
$settings = Settings::all();

$output = array();

foreach($settings as $item) {
  $output[$item->setting_name] = $item->setting_value;
}

$app->config('settings', $output);

// Determines directory
set_config('dir', substr(parse_url(app_url(), PHP_URL_PATH), 1));

// Defines default timezone
date_default_timezone_set($output['timezone']);

// if dir is not root
if(get_config('dir')) {
  // Define session/cookies path
  $app->config('cookies.path', '/'.get_config('dir').'/');
}

?>