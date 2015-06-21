<!DOCTYPE html>
<html id="login">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
    <meta charset="utf-8"></meta>
    
    <!-- Styles -->
    <?php echo stylesheet_link_tag('reset', 'styles') ?>
    
    <title>Login - <?php echo get_config('appname'); ?></title>
      
  </head>
  
  <body id="login">
  
    <!--Container-->
    <div id="content" class="container animated popIn">
    
      <!-- Logo -->
      <a href="<?php url_for('root'); ?>" id="logo" class="clearfix">
      
        <?php echo image_tag('logo_big.png') ?>
        
        <span><?php echo get_config('appname'); ?></span>
        
      <!--End #logo-->
      </a>
        
      <!--Login-->
      <div class="login clearfix">
        
        <?php render($yield); ?>
      
      <!--End .login-->
      </div>
      
      <?php if(isset($flash['error'])) : ?>
        
        <!--Error-->
        <div class="error">
      
          <?php echo $flash['error']; ?>
        
        <!--End .error-->
        </div>
        
      <?php endif; ?>
      
      <?php if(isset($flash['success'])) : ?>
        
        <!--Error-->
        <div class="success">
      
          <?php echo $flash['success']; ?>
        
        <!--End .error-->
        </div>
        
      <?php endif; ?>
      
    <!--End #content-->
    </div>
  
  </body>
    
</html>