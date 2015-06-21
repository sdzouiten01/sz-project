<?php

/**
*  Application Installer
*/
class Installer {
  
  /**
  *  Upgrade detector
  */
  public $upgrader;
  
  /**
  *  Database connection
  */
  protected $connection;
  
  /**
  *  Connection String
  */
  public $connection_string;
  
  
  /**
  *  Constructor Method
  */
  public function __construct() {
  
    // initializes new upgrade detector
    $this->upgrader = new Upgrader();
    
  }
  
  /**
  *  Builds database connection
  */
  public function buildConnection($args) {
  
    if(is_string($args)) {
    
      $database_config = $args;
      
    }
    
    else {
    
      // loads database config from shareit 1.0
      $database_config = 'mysql://'.$args['dbuser'].':'.$args['dbpass'].'@'.$args['dbhost'].'/'.$args['dbname'];
    
    }
    
    $this->connection_string = $database_config;
    
    try {
    
      // Initializes ActiveRecord
      ActiveRecord\Config::initialize(function($cfg) use ($database_config)
      {
        $cfg->set_connections(array(
          'development' => $database_config
        ));
      });
      
      $this->connection = ActiveRecord\ConnectionManager::get_connection('development');
      
      return true;
    
    } catch (Exception $ex) {
      
      return false;
      
    }
    
  }
  
  /**
  *  Clean tables
  */
  public function cleanTables() {
    
    $query = '';
    
    $tables = array(
      'users',
      'uploads',
      'settings'
    );
    
    foreach($tables as $table) {
      
      $query .= 'DROP TABLE IF EXISTS '.$table.';';
      
    }
    
    $this->connection->query($query);
    
  }
  
  /**
  *  Populates Database with tables
  */
  public function generateTables() {
  
    // TODO: update definitions
    
    $query = '';
    
    // holds table schema
    $tables = array(
      
      'uploads' => "CREATE TABLE `uploads` (
        `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
        `user_id` int(11) DEFAULT NULL,
        `uid` varchar(60) NOT NULL DEFAULT '',
        `name` text,
        `file` text,
        `created_at` datetime NOT NULL,
        `updated_at` datetime NOT NULL,
        `extension` text,
        `mime_type` text,
        `type` varchar(120) NOT NULL DEFAULT '',
        `url` text,
        `thumbnail` enum('none','cropped','original') DEFAULT 'none',
        `size` bigint(11) DEFAULT '0',
        `downloads` int(11) NOT NULL DEFAULT '0',
        `views` int(11) NOT NULL DEFAULT '0',
        `password` tinytext,
        `expiration` enum('none','downloads','date') DEFAULT 'none',
        `expiration_value` int(11) DEFAULT NULL,
        `folder_id` int(11) DEFAULT NULL,
        PRIMARY KEY (`id`)
      );",
      
      'users' => "CREATE TABLE `users` (
        `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
        `username` varchar(60) NOT NULL DEFAULT '',
        `email` varchar(100) NOT NULL DEFAULT '',
        `password` varchar(100) NOT NULL DEFAULT '',
        `role` enum('admin','user') NOT NULL DEFAULT 'user',
        `avatar` text,
        `updated_at` datetime NOT NULL,
        `created_at` datetime NOT NULL,
        `max_space` bigint(11) NOT NULL DEFAULT '0',
        `used_space` bigint(11) NOT NULL DEFAULT '0',
        `uploads_count` int(11) NOT NULL DEFAULT '0',
        `reset_token` varchar(60) DEFAULT NULL,
        `reseted_at` datetime DEFAULT NULL,
        `imported` tinyint(1) DEFAULT '0',
        `activation_token` varchar(60) DEFAULT NULL,
        PRIMARY KEY (`id`),
        UNIQUE KEY `email` (`email`)
      );",
      
      'settings' => "CREATE TABLE `settings` (
        `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
        `setting_name` varchar(100) NOT NULL DEFAULT '',
        `setting_value` varchar(100) NOT NULL DEFAULT '',
        PRIMARY KEY (`id`),
        UNIQUE KEY `setting_name` (`setting_name`)
      );",
      
      'folders' => "CREATE TABLE `folders` (
        `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
        `user_id` int(11) NOT NULL,
        `parent_id` int(11) DEFAULT NULL,
        `name` text NOT NULL,
        `created_at` int(11) NOT NULL,
        `updated_at` int(11) NOT NULL,
        PRIMARY KEY (`id`)
      );"
      
    );
    
    // create tables
    foreach($tables as $table) {
      
      $query .= $table;
      
    }
    
    $this->connection->query("$query");
    
  }
  
  
  /**
  *  Populates Tables with data
  */
  public function populateTables($tables) {
    
    $query = '';
    
    // create tables
    foreach($tables as $table) {
      
      $query .= $table;
      
    }
    
    $this->connection->query("$query");
    
  }
  
  /**
  *  Creates Config File
  */
  public function createConfigFile($name = 'config.php') {
    
    $contents = '<?php /* Created By Installer */ $database_connection = "'.$this->connection_string.'"; ?>';
    
    // Writes data
    if(is_writable(APPDIR)) {
    
      $file = fopen(APPDIR.$name,'w');
      fwrite($file, $contents);
      fclose($file);
    
    }
    
    else {
      
      return false;
      
    }
    
  }

}

?>