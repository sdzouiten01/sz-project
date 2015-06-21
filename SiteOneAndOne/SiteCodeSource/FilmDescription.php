<!DOCTYPE html>
<html>
    <?php
	include("authcheck.php"); // on appelle le fichier
    ?>
    <head>
        <meta charset="utf-8" />
        <link rel="stylesheet" href="style.css" />
        <!--[if lt IE 9]>
        <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
        <![endif]-->
        <title>Streaming For Zouiten Family</title>
    </head>
    
    <!--[if IE 6 ]><body class="ie6 old_ie"><![endif]-->
    <!--[if IE 7 ]><body class="ie7 old_ie"><![endif]-->
    <!--[if IE 8 ]><body class="ie8"><![endif]-->
    <!--[if IE 9 ]><body class="ie9"><![endif]-->
    <!--[if !IE]><!--><body><!--<![endif]-->
        <div id="bloc_page">
            <header>
                <div id="titre_principal">
                    <img src="images/TahaLogo.jpg" alt="TahaLogo" id="logo" />
                    <h2>Streaming For Zouiten Family</h2>
                </div>
                <nav>
                    <ul>
                        <li><a href="MainPage.php">Accueil</a></li>
			<li><a href="ListeFilmsPage.php?currentpagenb=1">Liste des Films</a></li>
			<li><a href="logout.php?redirect_to=<?=$file_name?>">Déconnexion</a></li>
                    </ul>
                </nav>
            </header>
            
            <div id="banniere_image">
                <div id="banniere_description">
                    La Defense,Paris...
                </div>
            </div>
            <section>
                <article>
		    <?php
			if (isset($_GET['ID'])) // On a l'ID du Film
			{
			// Connexion à la base de données
 		    	try
		    	{
	             		$bdd = new PDO('mysql:host=saaddatabase.sql-pro.online.net;dbname=saaddatabase', 'saaddatabase', 'Saad13041985');
		    	}catch(Exception $e){
        	     		die('Erreur : '.$e->getMessage());
		    	}
			$reponse = $bdd->query('SELECT * FROM FILMSINFOS WHERE ID='. $_GET['ID']);
			$reponseGenre = $bdd->query('SELECT * FROM FILMSGENRELINKED WHERE ID = '. $_GET['ID']);
			$reponseActs = $bdd->query('SELECT * FROM FILMSACTSLINKED WHERE ID = '. $_GET['ID']);
		    	while ($donnees = $reponse->fetch())
			{
				
				echo '<h1><img src="images/ico_epingle.png" class="ico_categorie" />' . $donnees['TITRE'] . '</h1>';
				echo '<table><tr><td>';
				echo '<img src="' . $donnees['PATHJPEG'] . '" WIDTH=400 HEIGHT=600/>';
				echo '</td><td>';
				echo '<page>';
				echo '<h1>' . $donnees['TITRE'] . '</h1>';
				echo '<p>Date de Sortie : ' . $donnees['BIRTH'] . '- Sortie DVD : ' . $donnees['DATEDESORTIE']. '</p>';
				echo '<p>Distributeur	: ' . $donnees['DISTRIBUTEUR'] . '</p>';
				echo '<p>Budget	: ' . $donnees['BUDGET'] . '</p>';
				echo '<p>Recompense	: ' . $donnees['RECOMPENSES'] . '</p>';
				echo '<p>Realisareur	: ' . $donnees['REALISATEUR'] . '</p>';
				echo '<p>Genre		: ';
				while ($donneesGenre = $reponseGenre->fetch())
				{
					echo $donneesGenre['GENRE'] . ' ';
				}
				echo '</p>';
				$reponseGenre->closeCursor();
				echo '<p>Acteurs		: ';
				while ($donneesActs = $reponseActs->fetch())
				{
					echo $donneesActs['ACTEURNAME'] . ',';
				}
				echo '</p>';
				$reponseGenre->closeCursor();
				$reponseActs->closeCursor();
				echo '<object data="' . $donnees['LINKSYNOPSIS'] . '" type="text/plain" width="600" height="300"/>';
				echo '</page></td></tr></table>';
			}
			$reponse->closeCursor();
			}
			else // Il manque des paramètres, on avertit le visiteur
			{
				echo 'L "ID" du Film n est pas renseigné';
			}
		    ?>
                </article>
		<video controls> 
  			<source src="7_ans_de_seduction.m4v" />
  				<!-- Texte alternatif en cas de non prise en charge de la balise video -->
  				<p class="warning">Le format *.mp4 n'est pas pris en charge par votre navigateur</p>
		</video>
            </section>
            <footer>
		<!---a remplir Convenablement plus tard-->
		
            </footer>
        </div>
    </body>
</html>
