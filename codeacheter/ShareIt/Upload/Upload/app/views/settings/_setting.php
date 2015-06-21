<div class="user clearfix" id="<%= id %>">

  <!--Avatar-->
  <div class="avatar">
    
    <div class="shadow">
    
      <% if(avatar) { %>
        
        <img src="<%= avatar %>">
      
      <% } else { %>
        
        <?php image_tag('avatar.png'); ?>
      
      <% } %>
    
    </div>
    
    
  <!--End .avatar-->  
  </div>
  
  <!--Info-->
  <a class="email">
  
    <%= email %>
  
    <span class="role">- <%= role %></span>
  
  </a>
  
  <!--Edit-->
  <div class="editform clearfix">
    
    <div class="select">
    
      <span class="label">
        
        <%= role.ucfirst() %>
      
      </span>
      
      <select name="role">
        <option value="user" <% if(role == 'user') print("selected") %>>User</option>
        <option value="admin" <% if(role == 'admin') print("selected") %>>Admin</option>
      </select>
      
      <span class="icon drop"></span>
      
    </div>
    
    <input type="text" name="max_space" value="<%= max_space %>" class="digitsonly">
    
  </div>
  
  <!--Delete-->
  <a class="delete" title="Delete user"></a>
  
</div>