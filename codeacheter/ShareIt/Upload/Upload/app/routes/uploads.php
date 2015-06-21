<?php

/**
*  GET
*  Upload index route
*/
$app->get('/', function() use ($app) {
  
  $uploads = generate_uploads();

  $user = $app->sessions->get_var('user');
  $folders = Folder::all(array('user_id' => $user->id));

  $options = array(
    'yield' => 'uploads/index',
    'sidebar' => 'uploads/sidebar',
    'body_id' => 'dashboard',
    'uploads' => $uploads,
    'folders' => $folders
  );
  
  if($app->sessions->get_var('user')->privileges) {
  
    $options['users'] = generate_users();
    
  }
  
  //renders main layout
  $app->render('layouts/application.php', $options);
  
})->name('root')->setMetadata(array( 'protected' => true ));


/**
*  GET
*  Uploads pagination route
*/
$app->get('/page/:page(/:filter)(/:folder)', function($page = 1, $filter = 'all', $folder = '') use($app) {

  // will only serve ajax requests
  if(!$app->request()->isAjax()) {
    $app->redirect($app->urlFor('root')."#page/$page/$filter/$folder");
  }
  
  respond_with_json(json_encode(generate_uploads($page, $filter, $folder)));
  

})->name('uploads_pagination')->setMetadata(array( 'protected' => true ));


/**
*  GET
*  Uploads search
*/
$app->post('/search', function() use($app) {

  if(!$app->request()->isAjax()) {
    $app->notFound();
  }
  
  $query = $app->request()->post("query");
  
  $uploads = search_uploads($query);
  
  respond_with_json($uploads);
  
})->name('uploads_search')->setMetadata(array( 'protected' => true ));


/**
*  POST
*  Upload post route
*/
$app->post('/', function() use ($app) {
  
  $user = User::find_by_id($app->sessions->get_var('user')->id);
  
  if(!$user) {
    
    $app->halt(403, 'You are not authorized to perform this action.');
    
  }
  
  // handles upload
  $file = process_upload($user->availableSpace());
  
  // Creates new upload instance
  $upload = new Upload(array(
    'name' => clean_filename($file->getNameFromSecureLocation()),
    'extension' => $file->getExtension(),
    'size' => $file->getSize(),
    'user_id' => $user->id,
    'uid' => generate_asciiuid(),
    'file' => $file->getFilename(),
    'type' => $file->getFileType(),
    'mime_type' => $file->getMimetype()
  ), false);
  
  // assigns short url to upload
  $upload->url = generate_shortLink($upload->get_link());
  
  $upload->save();
  
  // save user session information
  $upload->user->save_session();
  
  respond_with_json($upload->to_json(array('except' => array('file'))));
  
})->name('upload_new')->setMetadata(array( 'protected' => true ));


/**
*  POST
*  Upload reupload post route
*/
$app->post('/:id', function($id) use($app) {
  
  $upload = Upload::find_by_id($id);
  
  // TODO permisions verification if admin or shared or if is user created
  if($upload) {
    
    // checks if the user has limited upload space and if upload is bigger
    $user = User::find_by_id($app->sessions->get_var('user')->id);
    
    if(!$user) {
      
      $app->halt(403, 'You are not authorized to perform this action.');
      
    }
    
    // handles upload maximum size is available space - upload size
    $file = process_upload($user->availableSpace($upload->size));
    
    // removes upload size from user used space
    $user->used_space -= $upload->size;
    
    // Creates new upload instance
    $attributes = array(
      'name' => clean_filename($file->getNameFromSecureLocation()),
      'extension' => $file->getExtension(),
      'size' => $file->getSize(),
      'file' => $file->getFilename(),
      'type' => $file->getFileType(),
      'mime_type' => $file->getMimetype(),
      'thumbnail' => 'none'
    );
    
    // remove files associated with this file
    $upload->destroy_files();
    
    // save update attributes
    $upload->update_attributes($attributes, false);
    
    $user->used_space += $upload->size;
    $user->save();
    $user->save_session();
    
    // prevents changes from editing the upload by excluding these fields
    respond_with_json($upload->to_json(array('except' => array('file', 'expiration', 'expiration_value', 'password'))));
    
  }
  
  else {
    
    $app->halt(404, "Upload does not exist");
    
  }
  
})->name('upload_post')->setMetadata(array( 'protected' => true ));


/**
*  POST
*  Upload share route
*/
$app->post('/share/:id', function($id) use($app) {
  
  // will only serve ajax requests
  if(!$app->request()->isAjax()) {
    $app->redirect($app->urlFor('root'));
  }
  
  $upload = Upload::find_by_id($id);
  
  // TODO permisions verification if admin or shared or if is user created
  if($upload) {
    
    //Gets the request parameters from a json format sent by backbonejs
    $parameters = $app->request()->post();
    
    $to_email = $parameters['email'];
    $message = $parameters['message'];
    
    // validates to_email
    if(trim($to_email) == "") {
      $app->halt(400, __('login_emptyfields'));
    }
    
    // validates email(s)
    $emails = explode(",", $to_email);
    foreach($emails as $email) {
      
      if (!preg_match('|^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$|i', $email)) {
        
        $app->halt(400, __('login_invalidemail'));
        
      }
      
    }
    
    // determines from email adress
    $user = $app->sessions->get_var('user');
    if($user) {
      $from_email = $user->email;
    }
    else {
      
      $from_email = $parameters['from_email'];
      
      // validates email
      if (!preg_match('|^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$|i', $from_email)) {
        
        $app->halt(400, __('login_invalidemail'));
        
      }
      
    }
    
    if(notify_share($from_email, $to_email, $upload, $message)) {
      
      $app->halt(200);
      
    }
    
  }
  
  else {
    
    $app->halt(404, "Upload does not exist");
    
  }
  
})->name('upload_email');


/**
*  PUT
*  Upload update route
*/
$app->put('/:id', function($id) use($app) {
  
  $upload = Upload::find_by_id($id);
  
  // TODO permisions verification if admin or shared or if is user created
  if($upload) {
    
    //Gets the request parameters from a json format sent by backbonejs
    $parameters = json_decode($app->request()->post("model"), true);
    
    // validates fields
    // TODO: implement this in model, or in helpers
    if(trim($parameters['name']) == '') {
    
      $parameters['name'] = $upload->name;
      
    }

    if($parameters['expiration'] != 'none' && (trim($parameters['expiration_value']) == "" || $parameters['expiration_value'] == 0)) {
    
      $parameters['expiration'] = 'none';
      $parameters['expiration_value'] = NUll;
      
    }
    if($parameters['expiration'] == 'none') {
      $parameters['expiration_value'] = NUll;
    }
    
    $upload->set_attributes($parameters);
    
    $upload->save();
    
    if($upload->is_valid()) {
    
      respond_with_json($upload->to_json(array('except' => array('file'))));
      
    }
    
    else {
    
      $app->halt(400, 'Incomplete parameters');
      
    }
    
  }
  
  else {
    
    $app->halt(404, "Upload does not exist");
    
  }
  
})->name('upload_update')->setMetadata(array( 'protected' => true ));

/**
*  DELETE
*  Upload delete route
*/
$app->delete('/:id', function($id) use($app) {
  
  $upload = Upload::find_by_id($id);
  
  // TODO permisions verification if admin or shared or if is user created
  if($upload) {
    
    $upload->delete();
    
    $upload->user->save_session();
    
  }
  
  else {
    
    $app->halt(404, "Upload does not exist");
    
  }
  
})->name('upload_delete')->setMetadata(array( 'protected' => true ));


/**
*  GET
*  Upload thumbnail get route
*/
$app->get('/thumbnail/:id', function($id) use($app) {
  
  $upload = Upload::find_by_uid($id);
  
  if($upload && $upload->isImage() && $upload->thumbnail != 'none') {
    
    // a thumbnail was created for this upload
    if($upload->thumbnail == 'cropped') {
       
      respond_with_download(app_dir().'uploads/thumbnails/thumb_'.$upload->file, $upload->name, $upload->extension, $upload->mime_type);  
    
    }
    
    // no thumbnail was created for this upload
    else {
      
      respond_with_download(app_dir().'uploads/'.$upload->file, $upload->name, $upload->extension, $upload->mime_type);  
      
    }
    
  }
  
})->name('upload_thumbnail');


/**
*  GET
*  Upload get route
*/
$app->get('/:id(/:file)', function($id, $file = '') use ($app) {

  $upload = Upload::find_by_uid($id);
  
  if(is_null($upload)) {
    $app->notFound();
  }
  
  // checks if its upload user viewing the upload
  $increase = true;
  if($app->sessions->get_var('user')) {
    
    $user = User::find_by_id($app->sessions->get_var('user')->id);
    if($user && $upload->user_id == $user->id) {
      
      $increase = false;
      
    }
  }

  // checks expiration
  if(upload_has_expired($upload)) {
    $app->notFound();
  }
  
  if($file) {
      
    // checks if file is same as file upload name
    if($upload->name != $file) {
      
      $app->notFound();
      
    }
    
    // checks password permissions    
    if(!$upload->password || ($upload->password && check_upload_access($upload->uid))) {
      
      if($increase == true) {
        $upload->update_attribute('downloads', $upload->downloads + 1);
      }
      
      respond_with_download(app_dir().'uploads/'.$upload->file, $upload->name, $upload->extension, $upload->mime_type, false);
      
    }
      
  }
  
  if($increase == true) {
    $upload->update_attribute('views', $upload->views + 1);
  }
  
  //renders main layout
  $app->render('layouts/upload.php',array(
    'yield' => 'uploads/single',
    'body_id' => 'upload',
    'upload' => $upload,
  ));

})->name('upload_view');


/**
*  POST
*  Upload password acess post route
*/
$app->post('/:id/password', function($id) use ($app) {
  
  $upload = Upload::find_by_uid($id);
  
  if(!$upload) {
    $app->notFound();
  }
  
  $password = $app->request()->post('password');
  
  if($upload->password) {
    
    if($upload->get_passwordhash() == encrypt($password)) {
      
      // sets session access for file
      add_upload_access($upload->uid);
      
    }
    
    else {
      
      $app->flash('error', __('upload_wrongpass'));
      
    }
    
    $app->redirect($app->urlFor('upload_view', array('id' => $upload->uid)));
    
  }
  
  else {
  
    $app->redirect($app->urlFor('upload_view', array('id' => $upload->uid)));
    
  }
  
  

})->name('upload_password');

?>