<form method="post" action="<?php url_for('reset'); ?>">
  
  <p class="message">
    
    An email will be sent to your account with instructions on how to reset your password.
    
  </p>
  
  <!--Email-->
  <p class="email field">
    <input tabindex="1" id="email" type="text" name="email" value="<?php if(isset($_POST['email'])) echo $_POST['email']; ?>" placeholder="email@example.com" autofocus />
    <a href="<?php url_for('login'); ?>" tabindex="3" class="action button register" title="Click to log-in with your existing account.">Login</a>
  </p>
  
  <!--Submit-->
  <input type="submit" tabindex="5" class="button blue" value="Reset" />

</form>