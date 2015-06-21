<?php

/**
* Adds a certain number of days to a given date
* @param inputdate DateTime
* @param days
*/
function date_add_days($date, $days) {
  $new_date = clone $date;
  date_add($new_date, date_interval_create_from_date_string("$days day"));
  return $new_date;
}

function plural($num) {
	if ($num != 1)
		return "s";
}

/**
*  Returns a formated version of a date
*  @param date
*  example: datetime_relative($date)
*/
function datetime_relative($date, $show_timespan = true) {
  if($date instanceOf DateTime === false) {
    return $date;
  }
  $timestamp = $date->getTimestamp();
	$diff = time() - $timestamp;
	//seconds
	if ($diff<60) {
	  if($show_timespan === true) {
		  if($diff === 0) {
			  return "Just now";
		  }
		  else {
    		return "less than a minute ago";
    	}
    }
    else
      return "Today";
	}
	//minutes
	$diff = round($diff/60);
	if ($diff<60) {
	  if($show_timespan === true)
	    return $diff . " minute" . plural($diff) . " ago";
	  else
	    return "Today";
	}
	//hours
	$diff = round($diff/60);
	if ($diff<24) {
	  if($show_timespan === true)
	    return $diff . " hour" . plural($diff) . " ago";
	  else
	    return "Today";
	}
	//days
	$diff = round($diff/24);
	if ($diff <= 1) {
	  if($show_timespan === true)
	    return $diff . " day" . plural($diff) . " ago";
	  else
	    return "Yesterday";
	}
	//more than 1 day
	return date("l, j F Y", $timestamp);
}

/**
*  Returns a string version of a date
*  @param date
*  example: stringDate($date)
*/
function stringDate($date, $format = 'Y-m-d H:i:s' ) {
  if($date instanceOf DateTime === false) {
    return $date;
  }
  $timestamp = $date->getTimestamp();
  return date($format, $timestamp);
}


/**
*  Returns Timezones HTML options elements
*/
function generate_timezones($current = '') {

  $output = '';
  
  $timezone_identifiers = DateTimeZone::listIdentifiers();
  
  foreach ($timezone_identifiers as $timezone) {
    
    $class = ($current == $timezone) ? 'selected' : '';
    
    $output .= '<option value="'.$timezone.'"'.$class.'>'.$timezone.'</option>';
    
  }
  
  return $output;
  
}

?>