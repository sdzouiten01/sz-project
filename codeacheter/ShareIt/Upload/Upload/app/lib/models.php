<?php

/**
* Converts an phpactiverecord collection into an array
* currently not supported natively by phpactiverecord
* @param source
*/
function models_array($source, $options = array()) {
  $output = array();
  foreach($source as $item) {
    $output[] = $item->to_array($options);
  }
  return $output;
}

/**
* Returns json encoded phpactiverecord models
* @param source
*/
function encode_json_models($source, $options = array()) {
  if($source) {
    //Parses array to json content
    $json = json_encode(models_array($source, $options));
    return $json;
  }
}

/**
*  Assigns each model attributes value a templatized value of it's key name "<%= key %>"
*  @param model
*  @param optional tagStart
*  @param optional tagEnd
*  example: templatizeModel($model, '{', '}')
*/
function templatizeModel($model, $tagStart = '<%= ', $tagEnd = ' %>') {
  //holds the model attributes
  $attributes = $model->attributes();
  //function to templatize value
  $templatize_key = function($key) use($tagStart, $tagEnd) {
    //assigns templatized value
    $value = $tagStart.$key.$tagEnd;
    return $value;
  };
  //function to templatize model
  $templatize_model = function($source, $prefix = '') use($tagStart, $tagEnd) {
    $attributes = $source->attributes();
    //assigns each model attribute
    foreach($attributes as $key => $value) {
      //assigns templatized value
      $value = $tagStart.$prefix.$key.$tagEnd;
      $source->assign_attribute($key, $value, false);
    }
  };
  //loops throuhg model
  array_walk_recursive($attributes, function($item, $key) use($model, $templatize_key, $templatize_model) {
    //walks throguh all attributes and sets to a template echo syntaxis
    if(gettype($item) === 'object') {
      //item is a nested model, repeat process
      $templatize_model($item, $key.'.');
    }
    else {
      //assigns the current key
      $model->assign_attribute($key, $templatize_key($key), false);
    }
  });
}

?>