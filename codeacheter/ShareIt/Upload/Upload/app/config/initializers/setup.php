<?php
  
// Loads upgrader detector
require APPPATH.'/config/initializers/upgrader.php';

// Loads installer
require APPPATH.'/config/initializers/installer.php';

session_start();

/**
*  Check uploads folder integrity
*/
$app->hook('slim.before', function() use ($app) {
  
  $folders = array('uploads/', 'uploads/thumbnails/', 'uploads/ftp/');
  
  foreach($folders as $folder) {
    
   if(!is_dir(APPDIR.$folder)) {
     
     mkdir(APPDIR.$folder);
     
   }
    
  }
  
  // Creates .htaccess file
  $htaccess_content = "RewriteEngine On
  
  # Some hosts may require you to use the `RewriteBase` directive.
  # If you need to use the `RewriteBase` directive, it should be the
  # absolute physical path to the directory that contains this htaccess file.
  #
  # RewriteBase /
  
  Options -indexes
  RewriteCond %{REQUEST_FILENAME} !-f
  RewriteRule ^(.*)$ index.php [QSA,L]";
  $file = fopen(APPDIR.'.htaccess','w+');
  fwrite($file, $htaccess_content);
  fclose($file);
  
});

$app->map('/', function() use($app) {
  
  // declares installation section
  $section = 1;
  
  // creates new installer class
  $installer = new Installer();
  
  // view options
  $view_options = array(
    'isUpgrade' => $installer->upgrader->isUpgrade,
  );
  
  if($app->request()->isPost()) {
    
    $parameters = $app->request()->post();
    
    $section = $parameters['section'];
    unset($parameters['section']);
    
    // section 1 post
    if($section == 1) {
    
      $errors = false;
      
      if(trim($parameters['dbhost']) == "" || trim($parameters['dbname']) == "" || trim($parameters['dbuser']) == "" || trim($parameters['dbpass']) == "") {
        $app->flashNow('error', __('login_emptyfields'));
        $errors = true;
      }
      
      if(!$errors) {
        
        if(!$installer->buildConnection($parameters)) {
          
          $app->flashNow('error', __('install_dberror'));  
          
        }
        
        // connection is sucessful
        else {
        
          
          // detects if will upgrade
          $performUpgrade = (isset($parameters['perform_upgrade']) && $parameters['perform_upgrade'] == 1) ? true : false;
          
          if($installer->upgrader->isUpgrade && $performUpgrade) {
            
            $old_uploads = $installer->upgrader->getOldUploads();
            $old_users = $installer->upgrader->getOldUsers();
            
          }
          
          $installer->cleanTables();
        
          $installer->generateTables();
          
          // will populate tables from old version
          if($installer->upgrader->isUpgrade && $performUpgrade) {
          
            $tables = array(
              'uploads' => '',
              'users' => ''
            );
          
            // will process all files before being inserted into the database
            // file metadata will be set
            foreach($old_uploads as $upload) {
              
              $file_info = new FileInfo(APPDIR.'uploads/'.$upload['file']);
              
              // Creates new upload instance
              $upload['name'] = clean_filename($file_info->getFilename());
              $upload['extension'] = $file_info->getExtension();
              $upload['size'] = $file_info->getSize();
              $upload['type'] = $file_info->getFileType();
              $upload['mime_type'] = $file_info->getMimetype();
              
              
              // checks if has thumbnail and moves i
              if($upload['thumbnail'] == 'cropped') {
              
                if(!is_dir(APPDIR.'/uploads/thumbnails/')) {
                  
                  mkdir(APPDIR.'/uploads/thumbnails/');
                  
                }
                
                $file_info = new FileInfo(APPDIR.'/uploads/thumbs/_thumb'.$upload['file']);
                
                $file_info->move(APPDIR.'/uploads/thumbnails/thumb_'.$upload['file']);
                
              }
              
              if($upload['thumbnail'] == 'none' && $file_info->supportsThumbnail()) {
                $upload['thumbnail'] = 'original';
              }
              
              // sets password
              if(trim($upload['password']) != "") {
                $upload['password'] = encrypt($upload['password']);
              }
              
              $tables['uploads'] .= $installer->upgrader->prepareAsSQL('uploads', $installer->upgrader->prepareAttributes($upload));
              
            }
            
            // prepares users
            foreach($old_users as $user) {
              
              $tables['users'] .= $installer->upgrader->prepareAsSQL('users', $installer->upgrader->prepareAttributes($user));
              
            }
            
            // populates tables
            $installer->populateTables($tables);
            
            // stores data
            $_SESSION['performed_upgrade'] = true;
            
          }
          
          // stores data
          $_SESSION['connection_string'] = $installer->connection_string;
          
          // goes to application settings section
          $section = 2;
          
        }
        
      }
      
      
    } // end section 1
    
    // section 2 post
    else if($section == 2) {
    
      $final_settings = array(
        'uploads_perpage'=> 20,
        'uploads_maxsimuploads' => 5,
        'allow_newusers' => 1
      );
      $parameters = array_merge($parameters, $final_settings);

      // adds version number
      $parameters['version_number'] = APPVERSION;
      
      $_SESSION['settings'] = $parameters;
      
      $sql = '';
      foreach($parameters as $key => $value) {
        $key = "'".$key."'";
        $value = "'".$value."'";
        $sql .= "INSERT INTO `settings`(setting_name, setting_value) VALUES ($key, $value);";
      }
      
      if(!$installer->buildConnection($_SESSION['connection_string'])) {
        
        $app->flashNow('error', __('install_dberror'));
        
      }
      else {
        
        $installer->populateTables(array('settings' => $sql));
        
        if(isset($_SESSION['performed_upgrade'])) {
        
          unlink(APPDIR.'/config.php');
        
          $installer->createConfigFile();
          
          session_destroy();
          
          $app->redirect($app->urlFor('root'));
        
        }
        
        else {
        
          $section = 3;
          
        }
        
      }
      
    }
    
    // section 3  post
    else if($section == 3) {
    
      $email = $app->request()->post('email');
      $username = $app->request()->post('username');
      $password = $app->request()->post('password');
      $password_confirm = $app->request()->post('password_confirm');
      $hasErrors = false;
      
      
      // validates empty
      if(trim($email) == "" || trim($password) == "" || trim($password_confirm) == "" || trim($username) == "") {
        
        $app->flashNow('error', __("login_emptyfields"));
        
        $hasErrors = true;
        
      }
      
      // validates email
      if (!preg_match('|^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$|i', $email)) {
        
        $app->flashNow('error', __('login_invalidemail'));
        
        $hasErrors = true;
        
      }
      
      // validates username
      if(trim($username) != "" && !ctype_alnum($username)) {
        
        $app->flashNow('error', __('login_invaliduser'));
        
        $hasErrors = true;
        
      }
      
      // validates password match
      if($password != $password_confirm) {
      
        $app->flashNow('error', __('login_passwordmatch'));
        
        $hasErrors = true;
        
      }
      
      if($hasErrors == false) {
        
        $settings = $_SESSION['settings'];
      
        // registers user
        $user = array(
          
          'email' => $email,
          'username' => trim($username),
          'password' => generate_hash($password),
          'created_at' => date('Y-m-d H:i:s'),
          'updated_at' => date('Y-m-d H:i:s'),
          'role' => 'admin',
          'max_space' => $settings['default_maxspace']
        
        );
                
        $user = $installer->upgrader->prepareAsSQL('users', $installer->upgrader->prepareAttributes($user));
        
        if(!$installer->buildConnection($_SESSION['connection_string'])) {
          
          $app->flashNow('error', __('install_dberror'));
          
        }
        else {
        
          $installer->populateTables(array('users' => $user));
          
          $installer->createConfigFile();
          
          session_destroy();
          
          $app->redirect($app->urlFor('root'));
        
        }
          
      }
      
    }
  
  }
  
  // renders section 2
  if($section == 2) {
    
    $max_server_upload = 0;
    if(function_exists('ini_get')) {
      $max_server_upload = humanReadableToBytes(ini_get('upload_max_filesize'));
    }
    
    $settings = array(
      
      'admin_email' => array( 'label' => __('Administrator Email'), 'type' => 'text', 'default' => 'admin@server.com'),
      
      'appname' => array( 'label' => __('Application Name'), 'type' => 'text', 'default' => 'ShareIt 2.0'),
      
      'url' => array( 'label' => __('Application Url, <small>without http(s)://</small>'), 'type' => 'text', 'default' => substr($app->request()->getHost().$app->request()->getPath(), 0, -1)),
      
      'timezone' => array( 'label' => __('Timezone'), 'type' => 'select', 'default' => ''),
      
      'default_maxspace' => array( 'label' => __('Default User Maxspace (bytes)'), 'type' => 'int', 'default' => '0'),
      
      'uploads_maxupload' => array( 'label' => __('Server Max Upload Limit (bytes)'), 'type' => 'int', 'default' => $max_server_upload)
      
    );
    
    $view_options['settings'] = $settings;
    
  } // end section 2
  
  $view_options['section'] = $section;
  
  // checks if folder is writeable
  if(!is_writable(APPPATH.'/config/')) {
    $app->flashNow('error', 'Application folder is not writeable');
  }
  
  //renders installer
  $app->render('layouts/installer.php', $view_options);
  
})->via('GET', 'POST')->name('root');

// Stops normal app flow and runs installer routes and initializers
$app->run();

exit();

?>