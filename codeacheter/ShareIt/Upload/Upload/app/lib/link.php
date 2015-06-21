<?php

/**
* Returns an html anchor element
* Will read all parameters and treat them as following:
* @param slim route name
* @param anchor element text
* @param attributes array 
* example: link_for('route_name','My Route',array('class' => 'myclass'));
*/
function link_to($route, $text, $attributes = '', $parameters = array()) {
  //parses and sets attributes
  $url   = \Slim\Slim::getInstance()->urlFor($route,$parameters);
  $text  = $text;
  $attrs = '';
  if(is_array($attributes))
    $attrs = attributes_array($attributes);
    
  //Outputs the anchor element
  echo '<a href="'.$url.'" '.$attrs.' >'.$text.'</a>';   
}

/**
* Returns the url for a given route
* @param slimphp route name
* @param url parameters
* example: url_for('route_name', array('id' => "1"));
*/
function url_for($name, $parameters = array()) {
  //Outputs the url for the slimphp route
  echo \Slim\Slim::getInstance()->urlFor($name, $parameters);
}


?>