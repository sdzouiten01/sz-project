<!--Direct Link-->
<a href="<?php url_for('upload_view', array('id' => $upload->uid, 'file' => rawurlencode($upload->name))); ?>" class="button blue">Direct Link</a>

<!--Email-->
<div class="button emailshare">
  
  Email
  
  <!--Emailform-->
  <form action="<?php url_for('upload_email', array('id' => $upload->id)); ?>" method="POST" class="ajaxform">
  
    <!--Email-->
    <div class="field">
      
      <label>Email Adress(es) <small>separated by comma</small></label>
      <input type="text" name="email" placeholder="email@m.com, email@m.com">
      
    </div>
    
    <?php if(!isset($user)) : ?>
    
      <!--Email-->
      <div class="field">
        
        <label>Your Email Adress</label>
        <input type="email" name="from_email" placeholder="your@email.com">
        
      </div>
    
    <?php endif; ?>
    
    <!--Message-->
    <div class="field">
    
      <label>Message</label>
      <textarea name="message" placeholder="optional"></textarea>
      
    </div>
    
    <input type="submit" class="button" value="Send">
    
  <!--End .emailshare-->
  </form>
  
<!--End .email-->
</div>

<!--Twitter-->
<a class="button share twitter" data-width="655" data-height="251" href="https://twitter.com/intent/tweet?text=<?php echo rawurlencode($upload->name); ?> - <?php echo rawurlencode(get_url($upload)); ?>" target="_blank" title="Share on Twitter">
  
  <span class="icon twitter"></span>
  
</a>

<!--Googleplus-->
<a class="button share googleplus" data-width="655" data-height="251" href="https://plus.google.com/share?url=<?php echo rawurlencode(get_url($upload)); ?>" target="_blank" title="Share on Google+">
  
  <span class="icon googleplus"></span>
  
</a>

<!--Facebook-->
<a class="button share facebook" data-width="655" data-height="251" href="http://www.facebook.com/sharer.php?u=<?php echo rawurlencode(get_url($upload)); ?>" target="_blank" title="Share on Facebook">
  
  <span class="icon facebook"></span>
  
</a>