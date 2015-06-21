<?php
		if (isset($_POST['GENRE']) and $_POST['GENRE'] !=  'ALL')
		{
			$STRGENRE='&GENRE=' . $_POST['GENRE'];
		}else{
			$STRGENRE='';
		}
		if (isset($_POST['TITRE']) and $_POST['TITRE'] !=  '')
		{
			$STRTITRE='&TITRE=' . $_POST['TITRE'];
		}else{
			$STRTITRE='';
		}
		if (isset($_POST['REALI']) and $_POST['REALI'] !=  '')
		{
			$STRREALI='&REALI=' . $_POST['REALI'];
		}else{
			$STRREALI='';
		}
		$LINK='Location: ListeFilmsPage.php?currentpagenb=1&RechecheWidget=1' . $STRGENRE . $STRTITRE . $STRREALI;
		header($LINK);
?>
