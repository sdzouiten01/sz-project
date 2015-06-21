<?php 
session_start(); 
$session_timeout = 3600;
if(!isset($_SESSION['last_access']) || !isset($_SESSION['ipaddr']) || !isset($_SESSION['user'])) 
{ 
  $_SESSION=array(); 
  session_destroy(); 
  header("Location: index.php"); 
  die(); 
} 

if(time()-$_SESSION['last_access']>$session_timeout) 
{ 
  $_SESSION=array(); 
  session_destroy(); 
  header("Location: index.php"); 
  die(); 
} 
if($_SERVER['REMOTE_ADDR']!=$_SESSION['ipaddr']) 
{ 
  $_SESSION=array(); 
  session_destroy(); 
  header("Location: index.php"); 
  die(); 
} 
$_SESSION['last_access']=time(); 
?>
