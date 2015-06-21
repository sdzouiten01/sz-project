<?php

/**
* Returns a rendered template file
* @param view name
* @vars  template variables
* example: render('invoices/new', .html, array('id' => 1));
*/
function render($name, $vars = array()) {   
  //Outputs the url for the slimphp route
  Slim\Slim::getInstance()->render($name.'.php', $vars);
  
}

/**
*  Returns rendered javascript html templates
*/
function render_jst($list) {
  
  $output = '';
  
  foreach($list as $source) {
    
    $name = substr(strstr($source, '_'), 1);
    
    echo '<script type="text/html" id="template-'.$name.'">';
    
    render($source, array(), false);
    
    echo '</script>';
    
  }
  
  //echo $output;
  
}

?>