<?php

/**
* Returns an html input element & label
* Will read all parameters and treat them as following:
* @param input name
* @param input value
* @param label text
* @param attrs
* example: input_for('email','demo.com',array('class' => 'myclass'));
*/
function input_for() {
  $args = func_get_args();
  //parses and sets attributes
  $name  = $args[0];
  $value = $args[1];
  $text  = $args[2];
  $attrs = '';
  
  //parses attributes into html format
  if(count($args) >= 4 && $args[3] && is_array($args[3]))
    $attrs = attributes_array($args[3]);
    
  //Outputs the input element
  echo '<label for="'.$name.'">'.$text.'</label>';   
  echo '<input name="'.$name.'" value="'.$value.'" '.$attrs.' >';   
}


/**
* Returns an html select element with options and a selected value if provided
* Will read all parameters and treat them as following:
* @param select name
* @param options: key -> value
* @param current option
* @param attrs
* example: input_for('email','demo.com',array('class' => 'myclass'));
*/
function select_for($name, $options = array(), $current, $attrs = array()) {
  //Starts by creating the select html tag with it's name and attributes
  //$output = '<select name="'.$name.'" '.attributes_array($attrs).'>';
  $output = '<select name="'.$name.'">';
  //Adds each item in the options array and adds the selected if the current value matches a option value
  foreach($options as $key => $value) {
    if($value != $current)
      $output .= '<option value="'.$value.'">'.$key.'</option>'; 
    else
      $output .= '<option selected="selected" value="'.$value.'">'.$key.'</option>'; 
  }  
  $output .= '</select>';  
  return $output;
  
}

?>