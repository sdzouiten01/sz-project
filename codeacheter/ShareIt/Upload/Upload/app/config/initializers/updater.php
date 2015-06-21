<?php

/**
*  Application Updater
*/
class Updater {
  
  /**
  *  Database connection
  */
  protected $connection;
  
  /**
  *  Connection String
  */
  public $connection_string;

  /**
  *  Current Version
  */
  protected $version;
  
  
  /**
  *  Constructor Method
  */
  public function __construct($version, $app) {

    $this->version = $version;
    $this->app = $app;

    $this->connection = ActiveRecord\ConnectionManager::get_connection('development');
    
  }

  /**
  *  v.1.15 Update
  */
  public function update_115() {

    // inserts version into settings table
    $version_number = get_config('version_number');
    if($version_number && $version_number >= 1.15) {
      return;
    }
    $query = "CREATE TABLE IF NOT EXISTS `folders` (
              `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
              `user_id` int(11) NOT NULL,
              `parent_id` int(11) DEFAULT NULL,
              `name` text NOT NULL,
              `created_at` int(11) NOT NULL,
              `updated_at` int(11) NOT NULL,
              PRIMARY KEY (`id`)
            );";
    $query .= "INSERT INTO `settings` (`id`, `setting_name`, `setting_value`) VALUES ('null', 'version_number', '$this->version');";
    $query .= "INSERT INTO `settings` (`id`, `setting_name`, `setting_value`) VALUES ('null', 'allow_newusers', 1);";
    $query .= "ALTER TABLE `uploads` ADD folder_id int(11);";
    $query .= "ALTER TABLE `users` ADD activation_token varchar(60) DEFAULT NULL;";
    $this->connection->query($query);
    // moves config file to the root dir
    if(file_exists(APPPATH.'/config/config.php')) {
      copy(APPPATH.'/config/config.php', APPDIR.'/config.php');
      unlink(APPPATH.'/config/config.php');
    }

  }

}

?>