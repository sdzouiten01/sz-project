<?php

/**
*  Convert human readable file size (e.g. "10K" or "3M") into bytes
*  @param  string $input
*  @return int
*/
function humanReadableToBytes($input)
{
  $units = array(
    'b' => 1,
    'k' => 1024,
    'm' => 1048576,
    'g' => 1073741824
  );
  $number = (int)$input;
  $unit = strtolower(substr($input, -1));
  
  if (isset($units[$unit])) {
    $number = $number * $units[$unit];
  }
  return $number;
}

function bytesToHumanReadbleMB($input)
{
  $number = $input;
  $number = Round($number / 1048576);
  return $number;
}

/**
*  Formarts a file size number
*  @param filesize
*/
function formatFileSize($size)
{
  $sizes = array('bytes', 'kb', 'mb', 'gb', 'tb', 'pb', 'eb', 'zb', 'yb');
  if ($size == 0) return('n/a');
  
  return (round($size/pow(1024, ($i = floor(log($size, 1024)))), 2) . ' ' . $sizes[$i]);
}
 

?>