<div class="upload <%= type %> <% if(type != 'staged' && user_id != window.App.user.id) print("notowned") %>" id="upload-<%= id %>" data-cid="<%= cid %>" data-swfid="<%= id %>">

  <!--Border-->
  <div class="border">
    
    <% if(type == 'staged') { %>
      
      <div class="progress">
      
        <div class="bar">
          <span class="value" style="width: 0%"></span>
        </div>
        
        <span class="label">Uploading...</span>
        
      </div>
    
    <% } else { %>

    <!--Thumb-->
    <div class="thumb <%= type %> <%= thumbnail %> <%= extension %>"> 
      
      <% if(thumbnail != "none") { %>
        
        <img src="<?php url_for('upload_thumbnail', array('id' => '<%= uid %>')); ?>">
      
      <% } %>
    
    <!--End #thumbnail-->  
    </div>
    
    <!--Edit-->
    <div class="editform">
      
      <!--Upload-->
      <div class="uploader">
        
        <span id="uploader-<%= id %>">
        
          <span class="content"></span>
        
        </span>
        
        <a class="button blue"> <span class="icon cloud"></span>Change File</a>
        
      </div>
      
      <div class="select">
      
        <span class="label">
          
          <% if(expiration != 'none') {
        
            print(expiration.ucfirst())
          
          } else {
            
            print("Expiration")
          
          } %>
        
        </span>
        
        <select name="expiration">
          <option value="none" <% if(expiration == 'none') print("selected") %>>None</option>
          <option value="date" <% if(expiration == 'date') print("selected") %>>By Date(days)</option>
          <option value="downloads" <% if(expiration == 'downloads') print("selected") %>>By Downloads</option>
        </select>
        
        <span class="icon drop"></span>
        
      </div>
      
      <!--Expiration Value-->
      <input placeholder="Expiration Value" type="text" name="expiration" class="<% if(expiration == 'none') print("hidden") %> expiration_edit" value="<% if(expiration_value) print(expiration_value) %>">
      
      <!--Password-->
      <input placeholder="Password" type="text" name="password" class="password" value="<% if(password) print(password) %>">
      
      
    </div>
    
    <!--Ttitle-->
    <div class="title clearfix">
      
      <input type="checkbox" class="check" title="Select this file">
      
      <a class="title" title="<%- name %> (Click to edit)"><%- name %></a>
      
      <input class="edit" type="text" name="name" value="">
      
     <!--End .title--> 
    </div>
    
    <% } %>
    
    <!--Delete-->
    <a class="delete" title="(Click To Delete)"></a>
  
  <!--End .border-->
  </div>

<!--End file-->
</div>