<?php
		try
		{
			$bdd = new PDO('mysql:host=db516866107.db.1and1.com;dbname=db516866107', 'dbo516866107', 'Newsaad13041985');
		}
		catch (Exception $e)
		{
				echo '<p>Error connection</p>';
				die('Erreur : ' . $e->getMessage());
		}
		if (isset($_POST['user']))
		{
			$UserName = $_POST['user'];
			$query = sprintf("SELECT * FROM UsersTable where USERNAME='%s'",$UserName);
			// Ex�cution de la requ�te
			$reponse = $bdd->query($query);
			// V�rification du r�sultat
			// Ceci montre la requ�te envoy�e � MySQL ainsi que l'erreur. Utile pour d�boguer.
			if (!$reponse) {
					$message  = '<p>Requ�te invalide : ' . mysql_error() . "\n</p>";
					$message .= '<p>Requ�te compl�te : ' . $query. "\n</p>";
					die($message);
			}
			$donnees = $reponse->fetch();
			if (isset($_POST['mot_de_passe']) AND $_POST['mot_de_passe'] ==  $donnees['USERPWD']) // Si le mot de passe est bon
			{
				//securis� l'acc�es au autre page du site		
				session_start(); 
				$_SESSION['last_access']=time(); 
				$_SESSION['ipaddr']=$_SERVER['REMOTE_ADDR']; 
				$_SESSION['user']=$_POST['user']; 
				header('Location: MainPage.php');
				exit;
			}else {
				echo '<p>Mot de passe incorrect</p>';
			}
		}else {
			echo '<p>user non renseign�</p>';
		}
?>