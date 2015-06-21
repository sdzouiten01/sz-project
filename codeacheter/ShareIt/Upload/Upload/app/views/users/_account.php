<div id="account" class="modal">

  <div class="overlay">
  
    <div class="dialog animated slideIn">
      
      <div class="content clearfix">
        
        <h2 class="title">Your Account Settings</h2>
        
        <!--Avatar-->
        <div class="field editavatar">
        
          <div class="avatar">
          
            <div class="shadow">
              
              <% if(avatar) {  %>
            
                <img src="<%= avatar %>">
                
              <% } else { %>
              
                <?php image_tag('avatar.png'); ?>
              
              <% } %>
              
            </div>
            
          </div>
          
          <a class="button">Change</a>
        
        </div>
        
        <!--Avatar-->
        <div class="field avatar_field">
          <label>Avatar</label>
          <input type="text" name="avatar" value="<%= avatar %>">
        </div>
        
        <!--Email-->
        <div class="field">
          <label>Email</label>
          <input type="email" name="email" value="<%= email %>">
        </div>
        
        <!--Username-->
        <div class="field">
          <label>Username</label>
          <input type="text" name="username" value="<%= username %>">
        </div>
        
        <?php if($user->hasRights()) : ?>
        
        <!--Max space-->
        <div class="field">
          <label>Upload Space</label>
          <input type="number" class="digitonly" name="max_space" value="<%= max_space %>">
        </div>
        
        <?php endif; ?>
        
        <!--Password-->
        <div class="field">
          <label>Change Password</label>
          <input type="password" name="password">
        </div>
        
        <!--Password-->
        <div class="field">
          <label>Password Confirm</label>
          <input type="password" name="password_confirm">
        </div>
        
        <div class="message"></div>
        
      </div>
      
      <div class="bar clearfix">
      
        <a class="button close right">Close</a>
        
        <a class="button blue save right">Save</a>
      
      </div>
      
    </div>
    
  </div>
  
</div>