<!DOCTYPE html>
<html id="notfound">
  <head>
    
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
  
    <meta charset="utf-8"></meta>
    
    <!-- Styles -->
    <?php echo stylesheet_link_tag('reset', 'styles') ?>
    
    <title>Not Found - <?php echo get_config('appname'); ?></title>
      
  </head>
  
  <body id="login">
  
    <!--Container-->
    <div id="content" class="container">
    
      <!-- Logo -->
      <a href="<?php url_for('root'); ?>" id="logo">
      
        <?php echo image_tag('logo_big.png') ?>
        
        <span><?php echo get_config('appname'); ?></span>
        
      <!--End #logo-->
      </a>
      
      <h1>Oops...Not Found</h1>
      
      <a class="button blue" href="<?php echo app_url(); ?>"><span class="icon back"></span>Go Home</a>
      
    <!--End #content-->
    </div>
  
  </body>
    
</html>