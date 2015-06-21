<?php

/**
*  Detects upgrade from version 1
*/
class Upgrader {

  /**
  *  Database connection
  */
  protected $connection;
  
  /**
  *  Holds old config
  */
  public $old_config;
  
  /**
  *  Upgrade
  */
  public $isUpgrade;
  
  
  public function __construct()
  {
  
    if($this->detectOlderInstall()) {
      
      $this->isUpgrade = true;
    
      $config = $this->old_config;
      
      // loads database config from shareit 1.0
      $database_config = 'mysql://'.$config['dbuser'].':'.$config['dbpass'].'@'.$config['dbhost'].'/'.$config['dbname'];
      
      // Initializes ActiveRecord
      ActiveRecord\Config::initialize(function($cfg) use ($database_config)
      {
        $cfg->set_connections(array(
          'import' => $database_config
        ));
      });
      
      $this->connection = ActiveRecord\ConnectionManager::get_connection('import');
    
    }
    
    else {
      
      $this->isUpgrade = false;
      
    }
    
  }
  
  /**
  *  Detects older install
  */
  public function detectOlderInstall() {
  
    if(file_exists(APPDIR.'/config.php')) {
      
      include APPDIR.'/config.php';
      
      if(!isset($config)) {
      
        exit('Invalid Configuration File.');
        
      }
      else {
      
        $this->old_config = $config;
        
        return true;
        
      }
      
    }
    
    return false;
  
  }
  
  /**
  *  Fetches old uploads from database
  */
  public function getOldUploads() {
    
    $uploads = array();
    
    $this->connection->query_and_fetch('SELECT * FROM uploads', function($attributes) use(&$uploads) {
      
      // determine user_id
      $attributes['user_id'] = $attributes['author'];
      
      //determines uid
      $attributes['uid'] = $attributes['url'];
      
      $attributes['created_at'] = $attributes['date'];
      $attributes['updated_at'] = $attributes['date'];
      
      unset($attributes['url'],$attributes['id'], $attributes['type'], $attributes['date'], $attributes['folder'], $attributes['shorturl'], $attributes['author']);
      
      // determines thumbnail
      $attributes['thumbnail'] = 'none';
      if(file_exists(APPDIR.'/uploads/thumbs/_thumb'.$attributes['file'])) {
        $attributes['thumbnail'] = 'cropped';
      }
      
      // inserts user
      $uploads[] = $attributes;
      
    });
    
    return $uploads;
    
  }
  
  /**
  *  Fetches old users from database
  */
  public function getOldUsers() {
    
    $users = array();
    
    // fetches users
    $this->connection->query_and_fetch('SELECT * FROM users', function($attributes) use(&$users) {
      
      // determine role
      $attributes['role'] = $attributes['admin'] ? 'admin' : 'user';
      unset($attributes['admin']);
      
      // determine username
      $attributes['username'] = $attributes['name'];
      unset($attributes['name']);
      
      // sets a temporal email
      $attributes['email'] = $attributes['username']."@shareit.com";
      
      // determine usedspace
      $attributes['used_space'] = $attributes['space'];
      unset($attributes['space']);
      
      // determine maxspace
      $attributes['max_space'] = intval($attributes['maxspace']);
      unset($attributes['maxspace']);
      
      $attributes['imported'] = true;
      
      $attributes['id'] = intval($attributes['id']);
      
      // inserts user
      $users[] = $attributes;
      
    });
    
    return $users;
    
  }
  
  /**
  *  Prepares attributes for database
  */
  public function prepareAttributes($attributes) {
    
    foreach($attributes as $key => $value) {
    
      $attributes[$key] = "'".$value."'";
    
    }
    
    return $attributes;
    
  }
  
  /**
  *  Returns array as SQL
  */
  public function prepareAsSQL($table, $attributes) {
    
    // generates sql
    $columns = implode(", ",array_keys($attributes));
    $values  = implode(", ", array_values($attributes));
    $sql = "INSERT INTO `$table`($columns) VALUES ($values);";
    return $sql;
    
  }
  
}

?>