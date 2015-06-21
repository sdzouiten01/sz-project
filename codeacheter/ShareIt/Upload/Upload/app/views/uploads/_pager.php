<%

prev_page = currentPage;
next_page = currentPage;

if(currentPage - 1 >= firstPage) {
  prev_page -= 1;
}
if(currentPage + 1 <= totalPages) {
  next_page += 1;
}

%>

<a href="<?php url_for('uploads_pagination', array('page' => '<%= prev_page %>', 'filter' => '<%= currentFilter %>')); ?>" class="prev <% if(currentPage == firstPage) print("disabled") %>">&laquo; Previous Page</a>
<a href="<?php url_for('uploads_pagination', array('page' => '<%= next_page %>', 'filter' => '<%= currentFilter %>')); ?>" class="next <% if(currentPage == totalPages) print("disabled") %>">Next Page &raquo;</a>