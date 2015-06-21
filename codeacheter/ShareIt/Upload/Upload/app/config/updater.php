<?php

  // Inlcudes updater class
  include(APPPATH.'/config/initializers/updater.php');

  $updater = new Updater(APPVERSION, $app);

  $updater->update_115();

?>