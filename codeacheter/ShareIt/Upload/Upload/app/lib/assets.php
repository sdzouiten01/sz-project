<?php

/** 
*  Returns an html stylesheet link
*  Will read all parameters and treat them as stylesheets
*  @param stylesheet
*  example: stylesheet_link_tag('style1','style2',etc...)
*/
function stylesheet_link_tag() {
  $args = func_get_args();
  /*loops through all the passed arguments*/
  foreach ($args as $style) {
    echo '<link rel="stylesheet" href="'.app_url().'/app/assets/css/'.$style.'.css" type="text/css">'.PHP_EOL;
  }    
}

/**
*  Returns an html script tag
*  Will read all parameters and treat them as scripts 
*  @param javascript
*  example: javascript_include_tag('script1','script2',etc...)
*/
function javascript_include_tag() {

  $args = func_get_args();
  
  //script tag function
  $script_tag = function($file) {
    echo '<script src="'.app_url().'/app/assets/javascripts/'.$file.'" type="text/javascript"></script>'.PHP_EOL;
  };
  
  /*loops through all the passed arguments*/
  foreach ($args as $script) {
  
    // check if script is folder
    if(endswith($script, '/')) {
    
      $inner_files = dir_files(app_path().'/assets/javascripts/'.$script);
      foreach($inner_files as $file) {
        $script_tag($script.$file);
      }
      
    }
    else {
      $script_tag($script.'.js');
    }
  }
  
}

function javascript_join() {


  $args = func_get_args();
  
  $output = "";
  
  //script tag function
  $script_tag = function($file) use(&$output) {
  
    $contents = file_get_contents(APPPATH.'/assets/javascripts/'.$file);
    
    $output .= $contents;
    
  };
  
  /*loops through all the passed arguments*/
  foreach ($args as $script) {
  
    // check if script is folder
    if(endswith($script, '/')) {
    
      $inner_files = dir_files(app_path().'/assets/javascripts/'.$script);
      foreach($inner_files as $file) {
        $script_tag($script.$file);
      }
      
    }
    else {
      $script_tag($script.'.js');
    }
  }
  
  exit(print_r($output));
  
}

/**
*  Returns an html image tag 
*  @param image
*  example: image_tag('image1','image2',etc...)
*/
function image_tag() {
  $args = func_get_args();
  /*loops through all the passed arguments*/
  foreach ($args as $image) {
    echo '<img src="'.app_url().'/app/assets/images/'.$image.'">';
  }    
}

?>