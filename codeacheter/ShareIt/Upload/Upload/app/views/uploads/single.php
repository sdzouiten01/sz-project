<?php if(!is_protected($upload)): ?>
  
  <?php if(!is_protected($upload) && upload_supports_preview($upload)): ?>

  <div id="upload-<?php echo $upload->id; ?>" class="single upload <?php echo $upload->type; ?>">

    <?php if($upload->type == 'image'): ?>
  
    <!--Thumb-->
    <div class="thumb">
      
      <?php if($upload->isImage()) : ?>
      
        <img src="<?php url_for('upload_view', array('id' => $upload->uid, 'file' => rawurlencode($upload->name))); ?>">
      
      <?php endif; ?>
    
    <!--End #thumbnail-->  
    </div>

    <?php endif ?>
  
  </div>
  
  <?php else : ?>
  
  <div class="uploadcontainer">
    
    <!-- Logo -->
    <a href="<?php url_for('root'); ?>" id="logo" class="clearfix">
    
      <?php echo image_tag('logo_big.png') ?>
      
      <span><?php echo get_config('appname'); ?></span>
      
    <!--End #logo-->
    </a>
    
    <!--Upload-->
    <div id="upload-<?php echo $upload->id; ?>" class="single upload <?php echo $upload->type; ?> nopreview">
      
      <div class="border">
    
        <!--Thumb-->
        <div class="thumb <?php echo $upload->type . " " . $upload->thumbnail . " " . $upload->extension ?>">
          
          <?php //if($upload->isImage()) : ?>
          
            <!-- <img src="<?php //url_for('upload_view', array('id' => $upload->uid, 'file' => rawurlencode($upload->name))); ?>"> -->
          
          <?php //endif; ?>
        
        <!--End #thumbnail-->  
        </div>
        
        <!--Ttitle-->
        <div class="title clearfix">
        
          <a class="title" title="<?php echo htmlentities($upload->name) ?> (Click to edit)"><?php echo htmlentities($upload->name) ?></a>
          
         <!--End .title--> 
        </div>
      
      <!--End .border-->
      </div>
      <!--Actions-->
      <div class="actions">
      
        <a class="button blue download" href="<?php url_for('upload_view', array('id' => $upload->uid, 'file' => rawurlencode($upload->name))); ?>">Download</a>
      
        <!--Sharebar-->
        <div class="sharebar button">
          
          Share
          
          <div class="buttons animated slideIn">
          
            <?php render('uploads/_sharebar'); ?>
          
          </div>
          
        <!--End .sharebar-->
        </div>
      
      <!--End .actions-->
      </div>
    
    <!--End .upload-->
    </div>
  
  <!--End .container-->
  </div>
  
  <?php endif; ?>

<?php else: ?>

  <form class="protected" method="POST" action="<?php url_for('upload_password', array('id' => $upload->uid)); ?>">
    
    <input type="password" name="password" value="" placeholder="Password" autofocus/>
    
    <input type="submit" class="button blue" value="Download">
    
    <?php if(isset($flash['error'])) : ?>
      
      <div class="error">
        
        <?php echo $flash['error']; ?>
        
      </div>
    
    <?php endif; ?>
    
  </form>
  
<?php endif; ?>