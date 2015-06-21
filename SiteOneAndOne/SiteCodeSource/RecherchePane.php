<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
	<head>
		<title>Codes d'accès au serveur central de la NASA</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	</head>
	<body>
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
	</body>
</html>
