<!DOCTYPE html>
<html id="single">

  <head>
  
    <meta charset="utf-8"></meta>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
    
    <title><?php echo $upload->name.' - '. get_config('appname'); ?></title>
    
    <!-- Scripts -->
    <?php javascript_include_tag('vendor/_jquery', 'ui') ?>
    
    <!-- Styles -->
    <?php stylesheet_link_tag('reset', 'styles') ?>
    
  </head>
  
  <body>
    
    <?php if(!is_protected($upload) && upload_supports_preview($upload)): ?>
    
    <!--Topbar-->
    <div id="topnav">
      
      <div class="container">
        
        <div class="actions clearfix">
          
          <?php render('uploads/_sharebar'); ?>
        
        </div>
        
        <a class="title" title="<?php echo htmlentities($upload->name); ?>"><?php echo htmlentities($upload->name); ?></a>
        
      </div>
      
    </div>
    
    <?php endif; ?>
    
    <!--Content-->
    <div id="content" class="container <?php echo $upload->thumbnail; ?>">
      
      <?php render($yield); ?>
    
    <!--End #content-->
    </div>
  
  </body>
    
</html>