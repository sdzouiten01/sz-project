<?php
		// RecherchePaneAlbum.php
		if (isset($_POST['ALBUMTMP']) and $_POST['ALBUMTMP'] !=  'ALL')
		{
			$STRALBUMTMP='&ALBUMTMP=' . $_POST['ALBUMTMP'];
		}else{
			$STRALBUMTMP='';
		}
		
		$LINK='Location: AlbumPhoto.php?currentpagenb=1&RechecheWidget=1' . $STRALBUMTMP;
		header($LINK);
?>
