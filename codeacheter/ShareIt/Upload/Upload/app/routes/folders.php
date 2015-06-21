<?php

/**
 * POST
 * New folder route
 */
$app->post('/folders', function() use ($app) {

  //Gets the request parameters from a json format sent by backbonejs
  $parameters = json_decode($app->request()->post("model"), true);

  $user = $app->sessions->get_var('user');

  $folder = Folder::create(array(
    'name' => trim($parameters['name']),
    'user_id' => $user->id
  ));

  respond_with_json($folder->to_json());
  
});

/**
 * POST
 * Assign folders route
 */
$app->post('/folders/:id', function($id) use ($app) {

  //Gets the request parameters from a json format sent by backbonejs
  $parameters = json_decode($app->request()->post("model"), true);

  $user = $app->sessions->get_var('user');

  $folder = Folder::create(array(
    'name' => trim($parameters['name']),
    'user_id' => $user->id
  ));

  respond_with_json($folder->to_json());
  
});


/**
 * DELETE
 * Delete folder route
 */
$app->delete('/folders/:id', function($id) use ($app) {

  $folder = Folder::find_by_id($id);

  if(!$folder) $app->notFound();

  $folder->delete();

  $uploads_in_folder = Upload::all(array('folder_id' => $id));

  if(!$uploads_in_folder) return;

  foreach($uploads_in_folder as $upload) {
    $upload->folder_id = null;
    $upload->save();
  }

});

?>