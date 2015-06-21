<?php


/**
*  FTP Uploads check
*/
$app->hook('slim.before.dispatch', function() use ($app) {
  
  $uploads_dir = app_dir().'uploads/';
  $directory = app_dir().'uploads/ftp/';

  // checks for files in ftp uploads directory
  $files = directory_files($directory);
  
  // process files
  if(count($files) >= 1) {
    
    foreach($files as $file_name) {
    
      $file = new FileInfo($directory.$file_name);
      
      // allows uploads into user folder only
      if($file->getType() == 'file') {
        
        // $file->delete(); prevents files from being deleted
        
      }
      
      else {
        
        // looks for user
        $user = User::find_by_email($file_name);
        
        if($user) {
        
          $subfiles = directory_files($directory.$file_name);
          
          foreach($subfiles as $subfile) {
            
            if(!is_dir($directory.$subfile)) {
              
              $file_info = new FileInfo($directory.$file_name.'/'.$subfile);
              
              // checks available space
              if($user->max_space > 0 && $user->availableSpace() > $file_info->getSize() || $user->max_space == 0) {
              
                // Creates new upload instance
                $upload = new Upload(array(
                  'name' => clean_filename($file_info->getFilename()),
                  'extension' => $file_info->getExtension(),
                  'size' => $file_info->getSize(),
                  'user_id' => $user->id,  
                  'uid' => generate_asciiuid(),
                  'file' => $file_info->getUploadName(),
                  'type' => $file_info->getFileType(),
                  'mime_type' => $file_info->getMimetype()
                ), false);
                
                // performs the file upload and saves the upload instance
                $file_info->move($uploads_dir.$file_info->getUploadName());
                
                // assigns short url to upload
                $upload->url = generate_shortLink($upload->get_link());
                
                $upload->save();
                
                if($app->sessions->get_var('user') && $app->sessions->get_var('user')->id == $user->id) {
                  $app->flashNow('notice', __('uploap_ftpnotice'));
                  $upload->user->save_session();
                }
              
              }
              
            }
            
          }
        
        }
        
        // deletes folder
        // directory_remove($directory.$file_name);
        
      }
            
    }
    
  }
  
});

?>