<!--Data Fetching-->
<script>
  window.App.loadedUploads = <?php echo json_encode($uploads); ?>;
</script>

<!--Uploads-->
<div id="uploads" class="lists <?php echo $user->role ?>">
  
  <!--Items-->
  <div class="items clearfix">
    
  <!--End .items-->  
  </div>

<!--End #uploads-->
</div>

<?php if($user->hasRights()) : ?>
  
  <?php if(isset($users)) : ?>
  
  <!--Data Fetching-->
  <script>
    window.App.loadedUsers = <?php echo $users; ?>;
  </script>
  
  <?php endif; ?>

<?php endif; ?>

<!--FileUpload-->
<!--<form method="POST" action="<?php url_for('upload_new'); ?>" enctype="multipart/form-data" >
  
  <input type="file" name="file" value="" multiple="">
  
  <input type="submit" value="submit">
  
</form>-->

<!--JST-->
<?php 
  
  render_jst(array(
    'uploads/_upload',
    'uploads/_upload_inline',
    'uploads/_pager',
    'settings/_settings',
    'users/_account'
  ));

?>

<!--JST-->
<?php if($user->hasRights()) : ?>
  
  <?php render_jst(array( 'users/_users', 'users/_user' )); ?>

<?php endif; ?>
