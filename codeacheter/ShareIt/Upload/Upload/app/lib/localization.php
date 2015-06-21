<?php


/**
*  Returns a localized message
*/
function __($input) {
  
  global $localization_dict;
  
  if(isset($localization_dict[$input])) {
    
    return $localization_dict[$input];
    
  } else {
    
    return $input;
    
  }
  
}

/**
*  Returns localization dictionary
*/
function localized_dict() {
  
  global $localization_dict;
  
  return $localization_dict;
  
}

?>