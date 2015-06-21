<?php
define('APPPATH', realpath(__DIR__).'/app');
define('APPDIR', realpath(__DIR__).'/');

// Require the slim framework to handle HTTP requests
require APPPATH.'/Slim/Slim.php';

//With default settings
$app = new \Slim\Slim();

//GET route
$app->get('/hello/:name', function ($name) {
    echo "Hello, $name";
});

//POST route
$app->post('/person', function () {
    //Create new Person
});

//PUT route
$app->put('/person/:id', function ($id) {
    //Update Person identified by $id
});

//DELETE route
$app->delete('/person/:id', function ($id) {
    //Delete Person identified by $id
});

$app->run();

?>