<?php
/**
*  Authantication Middleware
*  Blocks page requests for protected routes, set protected routes:
*  Route->setMetadata(array('protected' => true))
*/
class Auth_Middleware extends \Slim\Middleware {

  public function call() {
  
    $app = $this->app;
    $req = $app->request();
    
    $this->generate_auth($req);
    
  }
  
  private function generate_auth($req) {
    
    $app = $this->app;
    
    $this->app->hook('slim.before.dispatch', function () use ($app, $req) {
      
      // gets current request route
      $protected = $app->router()->getCurrentRoute()->getMetadata();
      $protected = (isset($protected['protected']) && $protected['protected'] == true) ? true : false;
      
      if(!$protected) {
        return;
      }
      
      // Gets cookies
      $auth_cookie = $app->getEncryptedCookie('au');
      $token_cookie = $app->getEncryptedCookie('tc');
      
      // gets session user
      $user = $app->sessions->get_var('user');
      
      // Checks if cookies exists
      if($auth_cookie && $token_cookie) {
      
        // cookies exist, but no session user has been set (user closed browser: remember me feature)
        if(!$user) {
          
          // no data in current session, fetch and verify user
          $user = User::find_by_id($auth_cookie);
          
          // validates cookies
          if($user && $user->email === $token_cookie) {
          
            // logs user in without creating cookies(false)
            $user->login(false);
            
            //redirect
            $app->redirect($app->urlFor('root'));
            
          }
          
          // invalid cookies
          else {
          
            //Remove auth cookie and redirect to login page
            $app->deleteCookie('au');
            $app->deleteCookie('tc');
            
            //removes user data from session
            $app->sessions->remove('user');
            $app->sessions->end();
          
            if($app->router()->getCurrentRoute()->getPattern() != '/login') {
              $app->redirect($app->urlFor('logout'));
            }
            
          }
          
        }
      
      }
      
      // no cookies are presetn, check if route requires login, if not redirect to login
      else {
      
        // cookies do not exist, uploader is called from another request
        // check if SESSION_ID is passed and the session is valid the user will be in the session
        if(!$user && $app->router()->getCurrentRoute()->getPattern() != '/login') {
        
          $app->redirect($app->urlFor('login'));
          
        }
        
      }
    
    });
    
    $this->next->call();
    
  }
  
}

?>