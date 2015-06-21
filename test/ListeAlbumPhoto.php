    <?php
	include("authcheck.php"); // on appelle le fichier
    ?>
<!DOCTYPE html>
<html>
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
						<li><a href="AlbumPhoto.php">Album Photo</a></li>
						<li><a href="logout.php">Déconnexion</a></li>
                    </ul>
                </nav>
            </header>
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
				echo '<pcherche id="Recherche">';
			}else
			{
				echo '<pcherche id="Recherche" class="Cache">';
			}
		?>
			<form  action="RecherchePaneAlbum.php" method="post" >	
		<?php	
			if(isset($_GET['ALBUMTMP']))
			{
				echo '<p>Template Album Name         	: <input type="text" name="ALBUMTMP" value="' . $_GET['ALBUMTMP'] . '">';
			}else{
				echo '<p>Template Album Name         	: <input type="text" name="ALBUMTMP">';
			}
		?>
			<input type="submit" value="LookForIt" class="bouton_rouge" />
			</form></pcherche>
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
                    <p>Ce site permet a toute la famille de pouvoir regarder les films & photo dont je dispose</p>
                    <p>il permet aussi de partager ceux que vous avez et construire une library commune</p>
                    <p>J'espere que ça vous plait</p>
			<?php
			if (!isset($_GET['currentpagenb'])) // Il manque des paramètres, on avertit le visiteur
			{
				include("authcheck.php"); // on appelle le fichier
				header("Location: AlbumPhoto.php");
			}else{
				$currentpagenb=$_GET['currentpagenb'];
			}
		    	// Connexion à la base de données
 		    	try
		    	{
	             		$bdd = new PDO('mysql:host=db516866107.db.1and1.com;dbname=db516866107', 'dbo516866107', 'Newsaad13041985');
		    	}catch(Exception $e){
        	     		die('Erreur : '.$e->getMessage());
		    	}
			//
			//'SELECT * FROM FILMSINFOS WHERE ID < 50 ORDER BY ID ASC'
			// . $QueryRecherche
			//Construction Query Scope
			
			if (isset($_GET['ALBUMTMP']))
			{
				$booleanin=0;
				$QueryRecherche='';
				$QueryRecherche=' WHERE ALBUMNAME Like \'' . $_GET['ALBUMTMP'] . '%\'';;
			}else{
				$QueryRecherche='';
			}
			$QUERYALBUM='select distinct ALBUMNAME AS ALBUMNAME, count( PHOTONAME ) as NBPHOTO, user as USER from ALBUMTABLE group by ALBUMNAME';
			$QueryTest = 'SELECT COUNT( DISTINCT ALBUMTABLE.ALBUMNAME) AS NBALBUMPHOTO FROM ALBUMTABLE';
			//Selection
			echo '<page><table>';
			// Selection Pages
			$reponse = $bdd->query($QueryTest);
			while ($donnees = $reponse->fetch())
			{
				$NBALBUM = $donnees['NBALBUMPHOTO'];
				echo '<a class="totalalbum">NB Album Photo : ' . $NBALBUM . '</a></br>';
			}
			$reponse->closeCursor();
						
			//WHERE FILMSINFOS.ALBUMNAME Like "VIETNAME%"
		    $reponse = $bdd->query($QUERYALBUM);
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
				echo '<p>ALBUM		: ' . $donnees['ALBUMNAME'] . '</p>';
				echo '</td><td><textfilm>';
				echo '<p>NB PHOTO : ' . $donnees['NBPHOTO'] . '</p>';
				echo '<p>de	: ' . $donnees['USER'] . '</p>';
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
			
			<form action="upload.php" method="post" enctype="multipart/form-data">
				<p>Album Name         	: <input type="text" name="ALBUMTMP"></br>
				Select Images to upload ( jpg Format ):</br>
				<input type="file" name="fileToUpload" id="fileToUpload" multiple></br>
				<input type="submit" value="Create Album" name="submit">
			</form>
                </article>
            </section>
            <footer>
            </footer>
        </div>
    </body>
</html>
