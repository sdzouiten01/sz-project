<form method="post" action="<?php url_for('login'); ?>">
  
  <!--Email-->
  <p class="email field">
    <input tabindex="1" id="email" type="text" name="email" value="<?php if(isset($_POST['email'])) echo $_POST['email']; ?>" placeholder="email@example.com or username" autofocus />

    <?php if(get_config('allow_newusers')) : ?>

    <a href="<?php url_for('register'); ?>" tabindex="3" class="action button register" title="Click to sign up for a user account.">Register</a>

    <?php endif ?>
    
  </p>
  
  <!--Password-->
  <p class="password field">
    <input tabindex="2" id="password" type="password" name="password" value="" placeholder="password" />
    <a href="<?php url_for('reset'); ?>" tabindex="4" class="action button reset" title="Click to receive password reset instructions to the specified email adress.">Reset</a>
  </p>
  
  <!--Submit-->
  <input type="submit" tabindex="5" class="button blue" value="Login" />

</form>