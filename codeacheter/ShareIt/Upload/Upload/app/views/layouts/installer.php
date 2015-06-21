<!DOCTYPE html>
<html id="installer">
  <head>
    <meta charset="utf-8"></meta>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
    
    <!-- Styles -->
    <link rel="stylesheet" href="app/assets/css/reset.css" type="text/css" />
    <link rel="stylesheet" href="app/assets/css/styles.css" type="text/css" />
    
    <title>ShareIt 2.0 Setup</title>
    
  </head>
  
  <body id="installer">
    
    <!--Container-->
    <div id="content" class="container">
    
      <!-- Logo -->
      <a id="logo" class="clearfix">
      
        <img src="app/assets/images/logo_big.png">
        
        <span>ShareIt 2.0 Setup</span>
        
      <!--End #logo-->
      </a>
      
      <form method="POST" class="fields">
      
        <input type="hidden" name="section" value="<?php echo $section; ?>">
        
        <?php switch($section) : case 1: ?>
      
        <p class="message">
          
          ShareIt 2.0 will prepare the database for its usage, please fill all the fields in this form.
          
        </p>
      
        <!--Database-->
        <div class="field">
          <label>Database Host</label>
          <input type="text" name="dbhost" placeholder="localhost" value="<?php if(isset($_POST['dbhost'])) echo $_POST['dbhost']; ?>" autofocus>
        </div>
        
        <!--Database-->
        <div class="field">
          <label>Database Name</label>
          <input type="text" name="dbname" placeholder="database" value="<?php if(isset($_POST['dbname'])) echo $_POST['dbname']; ?>">
        </div>
        
        <!--Database-->
        <div class="field">
          <label>Database User</label>
          <input type="text" name="dbuser" placeholder="user" value="<?php if(isset($_POST['dbuser'])) echo $_POST['dbuser']; ?>">
        </div>
        
        <!--Database-->
        <div class="field">
          <label>Database User Password</label>
          <input type="password" name="dbpass">
        </div>
        
        <!--Upgrade-->
        <?php if($isUpgrade): ?>
          
          <label class="inlinecheck upgrade">
            <input type="checkbox" name="perform_upgrade" value="1" checked>Upgrade Detected:
            <small>Choose to import your previous uploads and users</small>
          </label>
        
        <?php endif; ?>
        
        <!--Submit-->
        <input type="submit" tabindex="5" class="button blue" value="Continue" />
        
        <?php break; case 2: ?>
          
          <!--App settings-->
        
          <?php foreach($settings as $key => $scheme) : ?>
            
            <div class="field">
            
            <?php switch($scheme['type']) : case "text": ?>
            
              <label><?php echo $scheme['label']; ?></label>
              <input type="text" class="" name="<?php echo $key; ?>" value="<?php echo isset($scheme['default']) ? $scheme['default'] : ''; ?>">
              
            <?php case "select": ?>
            
              <?php if($key == 'timezone'): ?>
              
              <label><?php echo $scheme['label']; ?></label>
              <select name="<?php echo $key; ?>">
                <?php echo generate_timezones($scheme['default']); ?>
              </select>
              
              <?php endif; ?>
              
            <?php break; ?>
            
            <?php case "int": ?>
            
              <label><?php echo $scheme['label']; ?> <small>0 for unlimited</small></label>
              <input type="number" class="" name="<?php echo $key; ?>" value="<?php echo isset($scheme['default']) ? $scheme['default'] : ''; ?>">
            
            <?php break; ?>
            
            <?php endswitch; ?>
            
            </div>
          
          <?php endforeach; ?>
          
          <!--Submit-->
          <input type="submit" tabindex="5" class="button blue" value="Done" />
          
          <?php break; case 3: ?>
            
            <!--Email-->
            <p class="email field">
              <input tabindex="1" id="email" type="text" name="email" value="<?php if(isset($_POST['email'])) echo $_POST['email']; ?>" placeholder="email@example.com" autofocus />
            </p>
            
            <!--Username-->
            <p class="username field">
              <input tabindex="3" id="username" type="text" name="username" value="<?php if(isset($_POST['username'])) echo $_POST['username']; ?>" placeholder="Username" />
            </p>
            
            <!--Password-->
            <p class="password field">
              <input tabindex="4" id="password" type="password" name="password" value="" placeholder="password" />
            </p>
            
            <!--Password-->
            <p class="password_confirm field">
              <input tabindex="5" id="password_confirm" type="password" name="password_confirm" value="" placeholder="Type your password again" />
            </p>
            
            <!--Submit-->
            <input type="submit" tabindex="5" class="button blue" value="Register" />
          
          <?php break; ?>
        
        <?php endswitch; ?>
      
      </form>
      
      <?php $messages = json_decode(get_flashMessages(), true); foreach($messages as $message) : ?>
      
        <!--Message-->
        <div class="message <?php echo $message['type']; ?>">
          
          <?php echo $message['message']; ?>
        
        </div>
      
      <?php endforeach; ?>
      
    <!--End #content-->
    </div>
  
  </body>
    
</html>