<form method="post" action="<?php url_for('reset_token', array('token' => $token)); ?>">
  
  <!--Password-->
  <p class="password field">
    <input tabindex="2" id="password" type="password" name="password" value="" placeholder="New password" autofocus />
  </p>
  
  <!--Password-->
  <p class="password_confirm field">
    <input tabindex="2" id="password_confirm" type="password" name="password_confirm" value="" placeholder="Type your new password again" />
  </p>
  
  <!--Submit-->
  <input type="submit" tabindex="5" class="button blue" value="Reset" />

</form>