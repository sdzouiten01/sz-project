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
            <section>
		<table><tr><td>
                <article>
                    <h1><img src="images/ico_epingle.png" class="ico_categorie" />Salut La Famille !!</h1>
                    <p>Ce site permet a toute la famille de pouvoir regarder les films dont je dispose</p>
                    <p>il permet aussi de pertager ceux que vous avez et construire une library commune</p>
                    <p>J'espere que ça vous plaait</p>
		    <p>choisisez un des Films ci-dessous et bonne sceance films .... :)</p>
		    <h1><img src="images/photo.png" class="ico_categorie" />Commentaires/MiniChat</h1>
		    <form action="minichat_post.php" method="post">
        	    <p>
        	    <label for="pseudo">Pseudo  : </label>
		    <?php
			echo $_SESSION['user']
		    ?><br />
        	    <label for="message">Message : </label><textarea name="message" id="message" cols="30" rows="5"></textarea><br />
        	    <input type="submit" value="Envoyer" />
		    </p>
		    </form>
		    <p>-----------------------------------------</p>
	 	    <?php
		    // Connexion à la base de données
 		    try
		    {
	             $bdd = new PDO('mysql:host=saaddatabase.sql-pro.online.net;dbname=saaddatabase', 'saaddatabase', 'Saad13041985');
		    }catch(Exception $e){
        	     die('Erreur : '.$e->getMessage());
		    }
		    // Récupération des 2 derniers messages
		    $reponse = $bdd->query('SELECT pseudo, message, timesstamp FROM MiniChatTable ORDER BY ID DESC LIMIT 0, 2');
		    // Affichage de chaque message (toutes les données sont protégées par htmlspecialchars)
		    while ($donnees = $reponse->fetch())
			{
				echo '<p><strong>' . htmlspecialchars($donnees['pseudo']) . ':</strong> '. htmlspecialchars($donnees['timesstamp']) . '</p>';
				$Lignes = explode("\n", htmlspecialchars($donnees['message']));
				foreach($Lignes as $element)
				{
    					echo '<p>' .htmlspecialchars($element) . '</p>';
				}
			}
		    $reponse->closeCursor();
		    ?>
                </article>
		</td><td>
                <aside>
			<h1> Infos Pour La Famille</h1>
				<p id="photo_zozor"><img src="images/saadPhoto.jpg" alt="Photo de Saad" /></p>
				<p>Cette page est toujours en construction ! j'essairai de l'ameliorer !</p>
				<p>Pour ça ! n'hesitez pas a m'envoyer vos remarques</p>
				<p><a href="mailto:sdzouiten01@gmail.com"><script type="text/javascript">
				/* <![CDATA[ */
				(function(){try{var s,a,i,j,r,c,l,b=document.getElementsByTagName("script");l=b[b.length-1].previousSibling;a=l.getAttribute('data-cfemail');if(a){s='';r=parseInt(a.substr(0,2),16);for(j=2;a.length-j;j+=2){c=parseInt(a.substr(j,2),16)^r;s+=String.fromCharCode(c);}s=document.createTextNode(s);l.parentNode.replaceChild(s,l);}}catch(e){}})();
					/* ]]> */
			</script>Envoyez-moi un e-mail !</a></p>
		</aside>
		</td></tr></table>
            </section>
            
            <footer>
		<!---a remplir Convenablement plus tard-->
            </footer>
        </div>
    </body>
</html>
