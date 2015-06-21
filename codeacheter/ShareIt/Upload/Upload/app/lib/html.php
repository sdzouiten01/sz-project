<?php

/**
*  Public function to make use of the html purify library
*  example: html_purify()
*/
function html_purify($source) {

  // includes the html purify library
  require APPPATH.'/vendor/htmlpurify/HTMLPurifier.standalone.php';

  $config = HTMLPurifier_Config::createDefault();
  
  $config->set('HTML.Allowed', 'div,i,br,ol,ul,li,p,b,strong,pre,u,blockquote,em,code,a[href],i');
  //$config->set('AutoFormat.AutoParagraph', true);
  
  //links
  $config->set('URI.AllowedSchemes', array (
    'http' => true,
    'https' => true,
    'mailto' => true
  ));
  //$config->set('URI.DefaultScheme', 'http');
  //$config->set('URI.MakeAbsolute', true);
  //$config->set('URI.Base', 'http://');
  $config->set('HTML.TargetBlank', true);
  //$config->set('AutoFormat.Linkify', true);
  
  //$config->set('Output.Newline', '<br />');
  
  //$config->set('Core.EscapeInvalidTags', true);
  //$config->set('Core.EscapeNonASCIICharacters', true);
  //$config->set('Filter.ExtractStyleBlocks.Escaping', true);
 
  
  $purifier = new HTMLPurifier($config);
  $source = $purifier->purify($source);
  
  return $source;
  
}

 
function normalize($s) {
    // Normalize line endings
    // Convert all line-endings to UNIX format
    $s = str_replace("\r\n", "\n", $s);
    $s = str_replace("\r", "\n", $s);
    // Don't allow out-of-control blank lines
    $s = preg_replace("/\n{2,}/", "\n\n", $s);
    return $s;
}

function new_line($source) {

  $source = normalize($source);

  $arr = explode("\n\n", $source);
  
  //exit(print_r($arr));
  
  if(!$arr) {
    return $source;
  }
  
  $arr_new = array();
  
  foreach($arr as $se) {
    $arr_new[] = nl2br($se);
  }
  
  return implode("", $arr_new);
}

?>