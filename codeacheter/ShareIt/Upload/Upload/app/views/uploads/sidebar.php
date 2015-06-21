<!--Multiple Selection-->
<div id="multipleselected" class="section hidden">

  <!--Title-->
  <h2 class="title"> With Selected </h2>

  <!--Content-->
  <div class="content">
    
    <a class="button delete">Delete</a>
    <!-- <a class="button moveto">Move To Folder</a> -->

    <div class="select">

      <span class="icon folder"></span>
    
      <span class="label">

        Asign Folder
      
      </span>
      
      <select name="expiration">

        <option value="-1">Asign Folder</option>

        <?php foreach($folders as $folder) : ?>

        <option value="<?php echo $folder->id ?>"><?php echo $folder->name ?></option>

        <?php endforeach ?>

      </select>
      
      <span class="icon drop"></span>
      
    </div>

  <!--End .content-->
  </div>

<!--End .section-->
</div>

<!--Navigation-->
<div id="navigation" class="section">

  <!--Title-->
  <h2 class="title"> Categories <span class="desc"> - File types </span></h2>

  <!--Content-->
  <div class="content">
    
    <?php echo generate_categories(); ?>

  <!--End .content-->
  </div>

<!--End .section-->
</div>

<!--Progressbar-->
<div id="spacebar" class="section">

  <!--Title-->
  <h2 class="title"> Space <span class="desc"> - Available storage </span></h2>

  <!--Content-->
  <div class="content">
    
    <div class="progress">
    
      <div class="bar">
      
        <span class="value" style="width: 0%"></span>
        
      </div>
      
      <span class="label"> <span class="used_space"></span> out of <span class="max_space"><?php echo formatFileSize($user->max_space); ?></span> </span>
      
    </div>

  <!--End .content-->
  </div>

<!--End .section-->
</div>

<!--Folders-->
<div id="folders" class="section">

  <!--Title-->
  <h2 class="title"> Folders <a class="delete button hidden" title="Delete current folder">-</a> <a class="create button" title="Create folder">+</a></h2>

  <!--Content-->
  <div class="content">

    <input class="hidden" type="text" name="label" placeholder="press enter to save" data-optval="<?php url_for('uploads_pagination', array( 'page' => 1, 'filter' => 'all', 'folder' => "%%" )) ?>">

    <div class="select">

      <span class="icon folder"></span>
    
      <span class="label">

        Folders
      
      </span>
      
      <select name="folders">

        <option id="option_initial" value="<?php url_for('uploads_pagination', array( 'page' => 1, 'filter' => 'all' )) ?>">All</option>

        <?php foreach($folders as $folder) : ?>

        <option id="option_<?php echo $folder->id ?>" value="<?php url_for('uploads_pagination', array( 'page' => 1, 'filter' => 'all', 'folder' => $folder->id )) ?>"><?php echo $folder->name ?></option>

        <?php endforeach ?>

      </select>
      
      <span class="icon drop"></span>
      
    </div>

  <!--End .content-->
  </div>

<!--End .section-->
</div>