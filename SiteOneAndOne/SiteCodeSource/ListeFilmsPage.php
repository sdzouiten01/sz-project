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
                    <h1>Taha</h1>
                    <h2>Streaming For Zouiten Family</h2>
                </div>
                <nav>
                    <ul>
                        <li><a href="MainPage.php">Accueil</a></li>
			<li><a href="ListeFilmsPage.php?currentpagenb=1&RechecheWidget=1">Liste des Films</a></li>
			<li><a href="logout.php?redirect_to=<?=$file_name?>">Déconnexion</a></li>
                    </ul>
                </nav>
            </header>
            
            <div id="banniere_image">
                <div id="banniere_description">
                    La Defense,Paris...
                </div>
            </div>
	<script src="http://code.jquery.com/jquery-1.7.1.min.js"></script>
	<style type="text/css">
		#Recherche {
			background:url('images/separateur.png') repeat-x bottom, url('images/ombre.png') repeat-x top;
		}
		#Recherche.floatable {
			position: fixed;
			top: 0;
			background: url('images/fond_jaune.png');
		}
		#Recherche.Cache
		{
			display: none;
			visibility: hidden;
		}
	 </style>
		<?php
			if(!isset($_GET['RechecheWidget']))
			{
				$_GET['RechecheWidget']=1;
			}
			if ( $_GET['RechecheWidget']==1)
			{ 
				echo '<pfilm id="Recherche">';
			}else
			{
				echo '<pfilm id="Recherche" class="Cache">';
			}
		?>
			<form action="RecherchePane.php" method="post">	
		<?php	
			if(isset($_GET['TITRE']))
			{
				echo '<p>Titre         	: </p><p><input type="text" name="TITRE" value="' . $_GET['TITRE'] . '"><br></p>';
			}else{
				echo '<p>Titre         	: </p><p><input type="text" name="TITRE"><br></p>';
			}
			if(isset($_GET['REALI']))
			{
				echo '<p>Réalisateur         	: </p><p><input type="text" name="REALI" value="' . $_GET['REALI'] . '"><br></p>';
			}else{
				echo '<p>Réalisateur         	: </p><p><input type="text" name="REALI"><br></p>';
			}
			echo '<p>Genre   		: ';
			if(isset($_GET['GENRE']))
			{
				echo '<select name="GENRE">';
				echo '<option value="'.$_GET['GENRE'].'">' . $_GET['GENRE'] .'</option>';
			}else{
				echo '<select name="GENRE">	';
				echo '<option value="ALL">ALL</option>';
			}	
		?>			
						<option value="Animation">Animation</option>
						<option value="Aventure">Aventure</option>
						<option value="Biopic">Biopic</option>
						<option value="Comedie">Comedie</option>
						<option value="Divers">Divers</option>
						<option value="Documentaire">Documentaire</option>
						<option value="Drame">Drame</option>Drame
						<option value="Epouvante-horreur">Epouvante-horreur</option>
						<option value="Espionnage">Espionnage</option>
						<option value="Famille">Famille</option>
						<option value="Fantastique">Fantastique</option>
						<option value="Guerre">Guerre</option>
						<option value="Historique">Historique</option>
						<option value="Musical">Musical</option>
						<option value="Peplum">Peplum</option>
						<option value="Policier">Policier</option>
						<option value="Romance">Romance</option>
						<option value="Thriller">Thriller</option>
						<option value="ALL">ALL</option>
					</select></p>
					<p><input type="submit" value="LookForIt" class="bouton_rouge" /></p>
				</form>
        	    	</pfilm>
	<script type="text/javascript">
		// listen for scroll
		var positionElementInPage = $('#Recherche').offset().top;
		$(window).scroll(
			function() {
				if ($(window).scrollTop() >= positionElementInPage) {
					// fixed
					$('#Recherche').addClass("floatable");
				} else {
					// relative
					$('#Recherche').removeClass("floatable");
				}
			}
		);
	</script>  
	<section>
                <article>
                    <h1><img src="images/ico_epingle.png" class="ico_categorie" />Liste des Films</h1>
                    <p>Ce site permet a toute la famille de pouvoir regarder les films dont je dispose</p>
                    <p>il permet aussi de pertager ceux que vous avez et construire une library commune</p>
                    <p>J'espere que ça vous plaait</p>
		    <p>choisisez un des Films ci-dessous et bonne sceance films .... :)</p>
			<?php
			if (!isset($_GET['currentpagenb'])) // Il manque des paramètres, on avertit le visiteur
			{
				include("authcheck.php"); // on appelle le fichier
				header("Location: MainPage.php");
			}else{
				$currentpagenb=$_GET['currentpagenb'];
			}
		    	// Connexion à la base de données
 		    	try
		    	{
	             		$bdd = new PDO('mysql:host=saaddatabase.sql-pro.online.net;dbname=saaddatabase', 'saaddatabase', 'Saad13041985');
		    	}catch(Exception $e){
        	     		die('Erreur : '.$e->getMessage());
		    	}
			//Construction Query Scope
			if (isset($_GET['TITRE']) or isset($_GET['REALI']) or isset($_GET['GENRE']))
			{
				$booleanin=0;
				$QueryRecherche='';
				
			if(isset($_GET['GENRE']))
			{
				$QueryRecherche = $QueryRecherche . ', FILMSGENRELINKED WHERE FILMSGENRELINKED.ID = FILMSINFOS.ID';
				$QueryRecherche = $QueryRecherche . ' AND FILMSGENRELINKED.GENRE = \'' . $_GET['GENRE'] . '\'';
				$booleanin = $booleanin + 1;
			}
			if(isset($_GET['REALI']))
			{
				if ( $booleanin != 0 )
				{
					$QueryRecherche = $QueryRecherche . ' AND FILMSINFOS.REALISATEUR LIKE \'' . $_GET['REALI'] . '%\'';
				}else{
					$QueryRecherche = $QueryRecherche . ' WHERE FILMSINFOS.REALISATEUR LIKE \'' . $_GET['REALI'] . '%\'';
				}
				$booleanin = $booleanin +1 ;
			}

			if(isset($_GET['TITRE']))
			{
				if ( $booleanin != 0 )
				{
					$QueryRecherche = $QueryRecherche . ' AND FILMSINFOS.TITRE LIKE \'' . $_GET['TITRE'] .  '%\'';
				}else{
					$QueryRecherche = $QueryRecherche . ' WHERE FILMSINFOS.TITRE LIKE \'' . $_GET['TITRE'] .  '%\''; '%\'';
				}
				$booleanin = $booleanin +1 ;
			}
			$Query=$QueryRecherche;
			}else{
				$QueryRecherche='';
			}
			$Query = 'SELECT DISTINCT FILMSINFOS.* FROM FILMSINFOS' . $QueryRecherche;
			$QueryTest= 'SELECT COUNT(*) AS ALLFILMNB FROM (' . $Query . ') TRTNBFILM';
			//Selection
			echo '<page><table>';
			$reponse = $bdd->query($QueryTest);
			// Selection Pages 
			while ($donnees = $reponse->fetch())
			{
				$ALLFILMNB = $donnees['ALLFILMNB'];
				$NBPAGE = ceil((int)$ALLFILMNB/50);
				echo '<a class="totalfilm">NB Films : ' . $ALLFILMNB . '</a></br>';
				echo '<a class="total">Page 1 de '. $NBPAGE .' : </a> ';
				if (isset($_GET['GENRE']) and $_GET['GENRE'] !=  'ALL')
				{
					$STRGENRE='&GENRE=' . $_GET['GENRE'];
				}else{
					$STRGENRE='';
				}
				if (isset($_GET['TITRE']) and $_GET['TITRE'] !=  '')
				{
					$STRTITRE='&TITRE=' . $_GET['TITRE'];
				}else{
					$STRTITRE='';
				}
				if (isset($_GET['REALI']) and $_GET['REALI'] !=  '')
				{
					$STRREALI='&REALI=' . $_GET['REALI'];
				}else{
					$STRREALI='';
				}
				for ($i = 1; $i <= $NBPAGE; $i++) {
		echo'<a class="page" title="' . $i .'" href="ListeFilmsPage.php?currentpagenb='.$i.'&RechecheWidget=1' . $STRGENRE . $STRTITRE . $STRREALI.'">' . ' ' . $i . ' ' .'</a>';
				}
			$reponse->closeCursor();
			}
			//
			//'SELECT * FROM FILMSINFOS WHERE ID < 50 ORDER BY ID ASC'
			$QueryShow= $Query . ' LIMIT ' . ($currentpagenb-1)*50 . ', ' . $currentpagenb*50;
		    	$reponse = $bdd->query($QueryShow);
			$IDORDER = 1;
		    	while ($donnees = $reponse->fetch())
			{
				if( $IDORDER%2 == 0 )
				{
					echo '<td>';
				}
				else
				{
					echo '<tr><td>';
				}
				echo '<pfilm><table><tr><td>';
				echo '<img src="' . $donnees['PATHPNG'] . '" class="ico_categorie"/>';
				echo '</td><td><textfilm>';
				echo '<h1><a href="FilmDescription.php?ID=' . $donnees['ID']. '">' . $donnees['TITRE'] . '</a></h1>';
				echo '<p>Titre		: ' . $donnees['TITRE'] . '</p>';
				echo '<p>Date de Sortie : ' . $donnees['BIRTH'] . '</p>';
				echo '<p>Recompense	: ' . $donnees['RECOMPENSES'] . '</p>';
				echo '<p>Réalisateur	: ' . $donnees['REALISATEUR'] . '</p>';
				echo '</textfilm></td></tr></table></pfilm>';
				if($IDORDER%2 == 0)
				{
					echo '</td></tr>';
				}
				else
				{
					echo '</td>';
				}
				$IDORDER=$IDORDER+1;
			}
			echo '</table></page>';
		    	$reponse->closeCursor();
		    ?>
                </article>
            </section>
            <footer>
            </footer>
        </div>
    </body>
</html>
