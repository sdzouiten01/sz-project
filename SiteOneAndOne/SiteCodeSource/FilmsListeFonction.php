<div class="pages">
<?php
// Connexion à la base de données
try
{
$bdd = new PDO('mysql:host=saaddatabase.sql-pro.online.net;dbname=saaddatabase', 'saaddatabase', 'Saad13041985');
}catch(Exception $e){
die('Erreur : '.$e->getMessage());		    	}
$reponse = $bdd->query('SELECT COUNT(*) AS ALLFILM FROM FILMSINFOS');
while ($donnees = $reponse->fetch())
{
echo '<span class="total">' . $donnees['ALLFILM'] . '</span>';
$reponse->closeCursor();
}
?>
    <span class="total">
        Page 1 de 38
    </span>
    <span class="current">
        1
    </span>
    <a class="page" title="2" href="#">
        2
    </a>
    <a class="last" title="Dernière ››" href="#">
        Dernière ››
    </a>
</div>
<div class="clr"></div>
