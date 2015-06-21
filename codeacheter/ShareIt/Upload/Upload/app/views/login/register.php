<form method="post" action="<?php url_for('register'); ?>">
  
  <!--Email-->
  <p class="email field">
    <input tabindex="1" id="email" type="text" name="email" value="<?php if(isset($_POST['email'])) echo $_POST['email']; ?>" placeholder="email@example.com" autofocus />
    <a href="<?php url_for('login'); ?>" tabindex="2" class="action button register" title="Click to log-in with your existing account.">Login</a>
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

</form>