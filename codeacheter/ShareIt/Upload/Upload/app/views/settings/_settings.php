<div id="settings" class="modal">

  <div class="overlay">
  
    <div class="dialog animated slideIn">
      
      <div class="content">
        
        <h2 class="title">Settings</h2>
        
        <% _.each(attributes, function(value, key) { %>
        
          <div class="field">
          
            <% scheme = schema[key]; %>
            
            <% if(scheme.type == 'text') { %>
            
              <label><%= scheme.label %></label>
              <input name="<%= key %>" type="text" value="<%= value %>">
            
            <% } %>
            
            <% if(key == 'timezone') { %>
              
              <label><%= scheme.label %></label>
              <div class="select">
              
                <span class="label">
                  
                  <%= value %>
                
                </span>
                
                <select name="<%= key %>">
                  <?php echo generate_timezones(); ?>
                </select>
                
                <span class="icon drop"></span>
                
              </div>
            
            <% } %>
            
            <% if(scheme.type == 'int') { %>
              
              <label><%= scheme.label %></label>
              <input name="<%= key %>" class="digitsonly" type="number" value="<%= value %>">
            
            <% } %>

            <% if(scheme.type == 'bool') { %>

              <label><input type="checkbox" name="<%= key %>" <%= value.toChecked() %> > <%= scheme.label %> </label>

            <% } %>
          
          </div>
          
        
        <% }); %>
        
      </div>
      
      <div class="bar clearfix">
      
        <a class="button right cancel">Cancel</a>
        
        <a class="button blue right save">Save</a>
      
      </div>
      
    </div>
    
  </div>
  
</div>