<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
	<head>
		<title>Codes d'acces au Site</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	</head>
	<body>
    
        <?php
	try
	{
		$bdd = new PDO('mysql:host=saaddatabase.sql-pro.online.net;dbname=saaddatabase', 'saaddatabase', 'Saad13041985');
	}
	catch (Exception $e)
	{
        	die('Erreur : ' . $e->getMessage());
	}
	if (isset($_POST['user']))
	{
		$UserName = $_POST['user'];
		$query = sprintf("SELECT * FROM UsersTable where USERNAME='%s'",$UserName);
		// Exécution de la requête
		$reponse = $bdd->query($query);
		// Vérification du résultat
		// Ceci montre la requête envoyée à MySQL ainsi que l'erreur. Utile pour déboguer.
		if (!$reponse) {
    			$message  = '<p>Requête invalide : ' . mysql_error() . "\n</p>";
    			$message .= '<p>Requête complète : ' . $query. "\n</p>";
    			die($message);
		}
		$donnees = $reponse->fetch();
		if (isset($_POST['mot_de_passe']) AND $_POST['mot_de_passe'] ==  $donnees['USERPWD']) // Si le mot de passe est bon
		{
			//securisé l'accées au autre page du site		
			session_start(); 
			$_SESSION['last_access']=time(); 
			$_SESSION['ipaddr']=$_SERVER['REMOTE_ADDR']; 
			$_SESSION['user']=$_POST['user']; 
			// On redirige vers la page d'Acceuill
			header('Location: MainPage.php');
		}else {
			echo '<p>Mot de passe incorrect</p>';
		}
	}else {
		echo '<p>user non renseigné</p>';
	}
	?>
	</body>
</html>
