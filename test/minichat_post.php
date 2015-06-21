<?php
// Connexion à la base de données
session_start(); 
try
{
	$bdd = new PDO('mysql:host=db516866107.db.1and1.com;dbname=db516866107', 'dbo516866107', 'Newsaad13041985');
}
catch(Exception $e)
{
        die('Erreur : '.$e->getMessage());
}

// Insertion du message à l'aide d'une requête préparée
$req = $bdd->prepare('INSERT INTO MiniChatTable (pseudo, message,timesstamp) VALUES(?, ?,?)');
$req->execute(array($_SESSION['user'], $_POST['message'],date(DATE_RFC2822)));

// Redirection du visiteur vers la page du minichat
header('Location: MainPage.php');
?>
