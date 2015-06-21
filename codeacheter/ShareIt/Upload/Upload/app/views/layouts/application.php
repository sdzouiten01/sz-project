<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8"></meta>
    
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
    
    <!-- Styles -->
    <?php stylesheet_link_tag('reset', 'styles') ?>
    
    <!-- Scripts -->
    <?php //javascript_include_tag('vendor/', 'vendor/wysihtml5/', 'ui', 'app') ?>
    
    <!--SWFUpload-->
    <?php //javascript_include_tag('vendor/swfupload/') ?>
    
    <!--Modules-->
    <?php //javascript_include_tag('modules/') ?>
    
    <?php javascript_include_tag('minified'); ?>
    
    <!--Enviroment Settings-->
    <script>
      window.App.version = <?php echo APPVERSION; ?>;
      window.App.user = <?php echo $user->to_json(); ?>;
      window.App.settings = <?php echo json_encode(get_setting('settings')); ?>;
      window.App.url = "<?php echo app_url(); ?>";
      window.App.session_id = "<?php echo session_id(); ?>";
      window.App.messages = <?php echo get_flashMessages(); ?>;
      
      <?php if($user->hasRights()) : ?>
        window.App.settings.schema = <?php echo json_encode(get_settingsSchema()); ?>;
      <?php endif; ?>
      
    </script>
    
    <title><?php echo get_config('appname'); ?></title>
    
  </head>
  
  <body id="<?php if(isset($body_id)) echo $body_id; ?>">
    
    <!--Top Navigation-->
    <div id="topnav">
    
      <div class="container">
      
        <!-- Logo -->
        <a href="<?php url_for('root'); ?>" id="logo">
          <?php echo image_tag('logo.png') ?>
          <h1><?php echo htmlentities(get_config('appname')); ?></h1>
        <!--End #logo-->
        </a>
        
        <!--Userbar-->
        <div id="userbar" class="clearfix">
        
          <!--Username-->
          <div class="drop user button">
          
            <span class="icon drop">
              
              <?php echo htmlentities($user->username ? $user->username : $user->email); ?>
              
            </span>
            
            <div class="dropdown">
              
              <ul>
                
                <?php if($user->hasRights()) : ?>
                  <li><a href="#users"><span class="icon users"></span>Users</a></li>
                  <li><a href="#settings"><span class="icon gear"></span>App Settings</a></li>
                  <li><a id="checkupdates" data-url="http://demo.pablofierro.me/shareit/versionnumber.json"><span class="icon updates"></span>Check Updates</a></li>
                <?php endif; ?>
                
                <li><a href="#account"><span class="icon gear"></span>Account Settings</a></li>
                
                <li><a href="<?php url_for('logout'); ?>"><span class="icon logout"></span>Logout</a></li>
                
              </ul>
            
            </div>
            
          </div>
          
          <!--Avatar-->
          <div class="avatar">
          
            <div class="shadow">
              
              <?php if($user->avatar) : ?>
            
                <img src="<?php echo $user->avatar; ?>">
                
              <?php else: ?>
              
                <?php image_tag('avatar.png'); ?>
              
              <?php endif; ?>
              
            </div>
            
          </div>
          
          <input id="search" class="search" type="text" placeholder="search here">
          
          <!--Upload-->
          <div id="upload">
            
            <span id="swfupload">
              
              <span class="content"></span>
            
            </span>
            
            <a class="button blue"> <span class="icon cloud"></span> Upload</a>
            
          </div>
          
        <!--End #userbar-->
        </div>
        
      <!--End .container-->
      </div>
      
    <!--End #topnav-->
    </div>
    
    <!--Content-->
    <div id="content" class="container">
    
      <!--Sidebar-->
      <div id="sidebar">
        
        <?php render($sidebar); ?>
      
      <!--End #sidebar-->
      </div>
      
      <!--Aside-->
      <div id="aside">
        
        <!--Messages-->
        <div id="messages">
        </div>
        
        <?php render($yield); ?>
      
      <!--End #aside-->
      </div>
    
    <!--End #content-->
    </div>
    
    <!--Footer-->
    <div id="footer" class="clearfix">
    
      <div class="container">
      
        Shareit
      
      </div>
    
    </div>
  
  </body>
    
</html>