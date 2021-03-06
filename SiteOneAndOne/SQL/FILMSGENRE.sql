-- phpMyAdmin SQL Dump
-- version 3.3.10
-- http://www.phpmyadmin.net
--
-- Serveur: saaddatabase.sql-pro.online.net
-- Généré le : Lun 03 Mars 2014 à 19:21
-- Version du serveur: 5.1.41
-- Version de PHP: 5.3.2-1ubuntu4.7

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données: `saaddatabase`
--

-- --------------------------------------------------------

--
-- Structure de la table `FILMSGENRE`
--

CREATE TABLE IF NOT EXISTS `FILMSGENRE` (
  `TITRE` varchar(255) COLLATE latin1_general_ci NOT NULL COMMENT 'Titre du Film',
  `GENRE` varchar(255) COLLATE latin1_general_ci NOT NULL COMMENT 'Genre'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci COMMENT='Liren FIlms Genre';

--
-- Contenu de la table `FILMSGENRE`
--

INSERT INTO `FILMSGENRE` (`TITRE`, `GENRE`) VALUES
('30+minutes+maximum', 'Action'),
('Armageddon', 'Action'),
('Captain+America++First+Avenger', 'Action'),
('Casino+Royale', 'Action'),
('Chaos', 'Action'),
('Constantine', 'Action'),
('District+9+', 'Action'),
('Doom', 'Action'),
('Duelist', 'Action'),
('Equilibrium', 'Action'),
('Fast+and+Furious+5+Fast+Five', 'Action'),
('Hercule', 'Action'),
('Highlander', 'Action'),
('Hypertension', 'Action'),
('Jason+Bourne+3+-+La+Vengeance+dans+la+Peau', 'Action'),
('Karate+Kid', 'Action'),
('Kill+Bill+1', 'Action'),
('Kill+Bill+2', 'Action'),
('Kingdom+Of+Heaven', 'Action'),
('L''Arme+Fatale', 'Action'),
('L''attaque+du+metro+123', 'Action'),
('Law+Abiding+Citizen', 'Action'),
('Le+Baiser+Mortel+Du+Dragon', 'Action'),
('Le+Roi+Arthur', 'Action'),
('Machete', 'Action'),
('Max+Payne', 'Action'),
('Mensonges+d''Etat', 'Action'),
('Mise+a+Prix', 'Action'),
('Miss+Detective', 'Action'),
('Oss+117+Le+Caire+Nid+D''espions', 'Action'),
('OSS+117+-+Le+Caire+nid+d''espions', 'Action'),
('Prince+of+Persia++les+sables+du+temps', 'Action'),
('Robin+des+Bois', 'Action'),
('Scarface', 'Action'),
('spiderman+3', 'Action'),
('The+Expendables', 'Action'),
('The+Forbidden+Kingdom=', 'Action'),
('X-Men+Origins,+Wolverine', 'Action'),
('Arthur+3+La+Guerre+Des+Deux+Mondes', 'Animation'),
('big+buck+bunny', 'Animation'),
('Final+Fantasy,+les+creatures+de+l''esprit', 'Animation'),
('Gang+De+Requins', 'Animation'),
('Kerity+la+maison+des+contes', 'Animation'),
('La+Princesse+Et+La+Grenouille', 'Animation'),
('Lascars', 'Animation'),
('Moi,+moche+et+mechant', 'Animation'),
('Sinbad+-+La+Legende+Des+Septs+Mers', 'Animation'),
('Summer+Wars', 'Animation'),
('Valse+avec+Bachir', 'Animation'),
('Volt++Star+Malgre+Lui', 'Animation'),
('10000', 'Aventure'),
('30+minutes+maximum', 'Aventure'),
('Arthur+3+La+Guerre+Des+Deux+Mondes', 'Aventure'),
('Australia', 'Aventure'),
('BenX', 'Aventure'),
('Big+Fish', 'Aventure'),
('Braveheart', 'Aventure'),
('Captain+America++First+Avenger', 'Aventure'),
('Duelist', 'Aventure'),
('Gang+De+Requins', 'Aventure'),
('Gullivers+Travels', 'Aventure'),
('H2g2+Le+Guide+Du+Voyageur+Galactique', 'Aventure'),
('Hercule', 'Aventure'),
('Into+the+Wild', 'Aventure'),
('Into+The+Wild', 'Aventure'),
('Jason+Bourne+3+-+La+Vengeance+dans+la+Peau', 'Aventure'),
('la+momie+3', 'Aventure'),
('Las+Vegas+Parano', 'Aventure'),
('Les+Trois+Royaumes', 'Aventure'),
('Prince+of+Persia++les+sables+du+temps', 'Aventure'),
('Robin+des+Bois', 'Aventure'),
('RTT', 'Aventure'),
('Sinbad+-+La+Legende+Des+Septs+Mers', 'Aventure'),
('The+Forbidden+Kingdom=', 'Aventure'),
('A+la+recherche+du+bonheur', 'Biopic'),
('Braveheart', 'Biopic'),
('Marie+Antoinette', 'Biopic'),
('MONGOL', 'Biopic'),
('The+Devils+Double', 'Biopic'),
('Valse+avec+Bachir', 'Biopic'),
('301+La+Legende+De+Superplus+Maximus', 'Comedie'),
('30+minutes+maximum', 'Comedie'),
('40+Ans+Toujours+Puceau', 'Comedie'),
('4freres1', 'Comedie'),
('4freres2', 'Comedie'),
('4+mariages+&+1+enterrement', 'Comedie'),
('$5+a+Day', 'Comedie'),
('7+ans+de+seduction', 'Comedie'),
('Ah+!+si+j''etais+riche', 'Comedie'),
('A+la+recherche+du+bonheur', 'Comedie'),
('Alvin+et+les+Chipmunks+2+-+2009+[]', 'Comedie'),
('American+Beauty+(Fr)', 'Comedie'),
('American+Pie', 'Comedie'),
('Au+bout+de+la+nuit+(Fr)', 'Comedie'),
('Barbershop', 'Comedie'),
('BenX', 'Comedie'),
('Bigard+Mon+Psy+Va+Mieux', 'Comedie'),
('big+buck+bunny', 'Comedie'),
('Big+Fish', 'Comedie'),
('Big+Mommas+House', 'Comedie'),
('Braquage+a+l''Italienne', 'Comedie'),
('Braqueurs+Amateurs', 'Comedie'),
('Bruce+Tout+Puissant', 'Comedie'),
('Burlesque', 'Comedie'),
('Burn+After+Reading', 'Comedie'),
('Case+Depart', 'Comedie'),
('Cashback', 'Comedie'),
('Cendrillon+&+le+prince+(pas+trop)+charmant', 'Comedie'),
('Ce+que+veulent+les+femmes', 'Comedie'),
('Ce+Soir+Je+Dors+Chez+Toi', 'Comedie'),
('Charlie,+les+filles+lui+disent+merci', 'Comedie'),
('College+Road+Trip', 'Comedie'),
('Crazy+stupid+love', 'Comedie'),
('Dieu+Est+Grand,+Je+Suis+Toute+Petite', 'Comedie'),
('Dinner+for+Schmucks', 'Comedie'),
('Dodgeball+(Meme+Pas+Mal+!)', 'Comedie'),
('dogma', 'Comedie'),
('Donne-moi+ta+main', 'Comedie'),
('En+cloque+mode+d''emploi', 'Comedie'),
('enferme+dehors', 'Comedie'),
('Everyone+Says+I+Love+You', 'Comedie'),
('Fee+Malgre+Lui+-+2009', 'Comedie'),
('Fight+Club+fr', 'Comedie'),
('Film+-+Leffet+Papillon+(The+Butterfly+Effect)', 'Comedie'),
('FIRST+SUNDAY', 'Comedie'),
('Florence+Foresti+Fait+Des+Sketches+A+La+Cigale', 'Comedie'),
('florence+foresti+mother+fucker', 'Comedie'),
('Forrest+Gump+[FR]', 'Comedie'),
('Frangins+malgre+eux', 'Comedie'),
('Fr+-+MERLIN+', 'Comedie'),
('Funerailles+D+Enfer+2011', 'Comedie'),
('Funny+People', 'Comedie'),
('Gad+Elmaleh+-+Papa+est+en+haut', 'Comedie'),
('Gad+Elmaleh+Papa+Est+En+Haut', 'Comedie'),
('Gang+De+Requins', 'Comedie'),
('Girls+and+Sex', 'Comedie'),
('Grolandsat+CD1', 'Comedie'),
('Grolandsat+CD2', 'Comedie'),
('grownups+xvid-', 'Comedie'),
('Gullivers+Travels', 'Comedie'),
('H2g2+Le+Guide+Du+Voyageur+Galactique', 'Comedie'),
('Hot+Tub+Time+Machine', 'Comedie'),
('Il+Ne+Faut+Jurer+De+Rien', 'Comedie'),
('I+Love+You+Phillip+Morris', 'Comedie'),
('Intouchables', 'Comedie'),
('Its+Kind+Of+A+Funny+Story', 'Comedie'),
('Je+vais+bien,+ne+t''en+fais+pas+2006', 'Comedie'),
('Jumping+the+Broom', 'Comedie'),
('Killer+Elite+R5', 'Comedie'),
('King+Guillaume', 'Comedie'),
('Kiss+and+Kill', 'Comedie'),
('k-pax', 'Comedie'),
('La+Fesse+-+LaFesse+Gauche', 'Comedie'),
('La+Guerre+des+Peres', 'Comedie'),
('la+jungle', 'Comedie'),
('La+Personne+Aux+Deux+Personnes', 'Comedie'),
('L''Arme+Fatale', 'Comedie'),
('L''Arnacoeur', 'Comedie'),
('L''arnaque+Fr', 'Comedie'),
('Las+Vegas+Parano', 'Comedie'),
('La+Tour+Montparnasse+Infernale', 'Comedie'),
('La+Verite+si+je+mens+1', 'Comedie'),
('La+Vie+De+d+Gale', 'Comedie'),
('Le+fanteme+de+mon+ex-fiancee', 'Comedie'),
('leloup', 'Comedie'),
('Leon+Version+Longue', 'Comedie'),
('Le+pari', 'Comedie'),
('Les+Beaux+Gosses', 'Comedie'),
('Lesbian+Vampire+Killers', 'Comedie'),
('Les+Derniers+Jours+Du+Monde', 'Comedie'),
('Le+Seigneur+Des+Anneaux+3+-+Le+Retour+Du+Roi+Version+Longue', 'Comedie'),
('Le+Seigneur+Des+Anneaux+3+-+Le+Retour+Du+Roi+Version+Longue+part', 'Comedie'),
('Le+seminaire', 'Comedie'),
('Le+sourire+de+Mona+Lisa', 'Comedie'),
('L''Italien', 'Comedie'),
('Little+Miss+Sunshine', 'Comedie'),
('LMDF', 'Comedie'),
('Love+And+Other+Drugs', 'Comedie'),
('Love+&+the+city', 'Comedie'),
('L''univers+de+Stephen+Hawking+-+Voyager+à+travers+le+temps', 'Comedie'),
('macgrube', 'Comedie'),
('Manhattan', 'Comedie'),
('Mauvaise+foi', 'Comedie'),
('Megamind+2010', 'Comedie'),
('Merlin+(1999)+Part+2+Sur+2+-+De+Steve+Barron', 'Comedie'),
('Mise+a+Prix', 'Comedie'),
('Miss+Detective', 'Comedie'),
('Mon+babysitter', 'Comedie'),
('Mr+Brooks+(Fr)', 'Comedie'),
('National+Geographic+-+La+Furie+De+La+Nature', 'Comedie'),
('National+Geographic+-+Les+Coulisses+Du+Vatican', 'Comedie'),
('National+Geographic+-+Tresors+Des+Profondeurs', 'Comedie'),
('Once+LiMiTED++XviD', 'Comedie'),
('Oss+117+Le+Caire+Nid+D''espions', 'Comedie'),
('OSS+117+-+Le+Caire+nid+d''espions', 'Comedie'),
('OSS+117+-+Rio+ne+repond+plus', 'Comedie'),
('Panique+aux+Funerailles', 'Comedie'),
('Paul', 'Comedie'),
('Persepolis+(Fr)', 'Comedie'),
('P+S+I+Love+You', 'Comedie'),
('Ramona+et+Beezus', 'Comedie'),
('Rango+2011', 'Comedie'),
('Rec+2007', 'Comedie'),
('Rendez+vous+l''ete+prochain', 'Comedie'),
('RTT', 'Comedie'),
('Scarface+VF', 'Comedie'),
('Shaun+Of+The+Dead', 'Comedie'),
('Slevin', 'Comedie'),
('Slumdog+Millionaire', 'Comedie'),
('Sweet+and+Lowdown', 'Comedie'),
('Thank+you+for+smoking=', 'Comedie'),
('The+41-Year-Old+Virgin', 'Comedie'),
('The+Artist+(2011)', 'Comedie'),
('The+Joneses', 'Comedie'),
('The+Ten', 'Comedie'),
('The+weather+man', 'Comedie'),
('The+Worlds+Greatest+Dad', 'Comedie'),
('Titanic+II', 'Comedie'),
('Touch+of+Evil+(1958)+[VO]', 'Comedie'),
('Tout+ce+qui+brille+', 'Comedie'),
('Tout+sur+Jamel', 'Comedie'),
('Toy+Boy', 'Comedie'),
('Une+Annee+De+Beaux+Gosses', 'Comedie'),
('Up+By+Smoky+TT', 'Comedie'),
('Up+By+Smoky+UFE', 'Comedie'),
('Venus+Noir', 'Comedie'),
('Very+Bad+Things', 'Comedie'),
('Volt++Star+Malgre+Lui', 'Comedie'),
('Volver', 'Comedie'),
('Willow-LYD', 'Comedie'),
('W+T', 'Comedie'),
('You+Again+2010', 'Comedie'),
('Zoom', 'Comedie'),
('American+Girls', 'Divers'),
('no+mans+land', 'Divers'),
('Capitalism+A+Love+Story', 'Documentaire'),
('Food+inc+-+[]', 'Documentaire'),
('L+Age+de+la+stupidite', 'Documentaire'),
('L''experience', 'Documentaire'),
('Religolo', 'Documentaire'),
('Valse+avec+Bachir', 'Documentaire'),
('American+Beauty', 'Drame'),
('Australia', 'Drame'),
('Barbershop', 'Drame'),
('Bruce+Tout+Puissant', 'Drame'),
('Burlesque', 'Drame'),
('Chaos', 'Drame'),
('Dirty+Pretty+Things', 'Drame'),
('Flags+Of+Our+Fathers', 'Drame'),
('Ghost+Dog', 'Drame'),
('Ghost+Dog+The+Way+of+the+Samurai', 'Drame'),
('Green+Zone', 'Drame'),
('Hell', 'Drame'),
('Hors+La+Loi', 'Drame'),
('Into+the+Wild', 'Drame'),
('Into+The+Wild', 'Drame'),
('Jeanne+d''Arc', 'Drame'),
('Karate+Kid', 'Drame'),
('Kill+Bill+1', 'Drame'),
('La+Derniere+Tentation+Du+Christ', 'Drame'),
('La+Haine', 'Drame'),
('Law+Abiding+Citizen', 'Drame'),
('Le+Grand+Bleu', 'Drame'),
('L''Elite+De+Brooklyn', 'Drame'),
('Little+Miss+Sunshine', 'Drame'),
('Marie+Antoinette', 'Drame'),
('Memoire+D''une+Geisha', 'Drame'),
('Rendez+vous+l''ete+prochain', 'Drame'),
('Requiem+for+a+Dream', 'Drame'),
('Scarface', 'Drame'),
('The+Butterfly+Effect', 'Drame'),
('The+Devils+Double', 'Drame'),
('The+Girl+Next+Door', 'Drame'),
('The+Joneses', 'Drame'),
('The+Notebook', 'Drame'),
('The+Tourist', 'Drame'),
('The+Visitor', 'Drame'),
('The+Wrestler', 'Drame'),
('Un+Prophete', 'Drame'),
('valkyrie', 'Drame'),
('Valse+avec+Bachir', 'Drame'),
('Va+Vis+Et+Deviens', 'Drame'),
('Wall+Street+l''argent+ne+dort+jamais', 'Drame'),
('Wall+Street+Money+Never+Sleeps', 'Drame'),
('We+Own+The+Night', 'Drame'),
('30+jours+de+nuit', 'Epouvante-horreur'),
('Constantine', 'Epouvante-horreur'),
('Cube+2+hypercube', 'Epouvante-horreur'),
('Doom', 'Epouvante-horreur'),
('Insidious', 'Epouvante-horreur'),
('Lesbian+Vampire+Killers', 'Epouvante-horreur'),
('Pan''s+Labyrinth', 'Epouvante-horreur'),
('Saw+I', 'Epouvante-horreur'),
('Shaun+Of+The+Dead', 'Epouvante-horreur'),
('The+Girl+Next+Door', 'Epouvante-horreur'),
('The+Grudge+2', 'Epouvante-horreur'),
('themist', 'Epouvante-horreur'),
('Casino+Royale', 'Espionnage'),
('Jason+Bourne+3+-+La+Vengeance+dans+la+Peau', 'Espionnage'),
('L''Enquête+-+The+International', 'Espionnage'),
('Le+Silencieux', 'Espionnage'),
('Mensonges+d''Etat', 'Espionnage'),
('Oss+117+Le+Caire+Nid+D''espions', 'Espionnage'),
('OSS+117+-+Le+Caire+nid+d''espions', 'Espionnage'),
('OSS+117+-+Rio+ne+repond+plus', 'Espionnage'),
('Karate+Kid', 'Famille'),
('Ramona+et+Beezus', 'Famille'),
('Volt++Star+Malgre+Lui', 'Famille'),
('Zoom', 'Famille'),
('Awake', 'Fantastique'),
('Bruce+Tout+Puissant', 'Fantastique'),
('Constantine', 'Fantastique'),
('dogma', 'Fantastique'),
('Highlander', 'Fantastique'),
('k-pax', 'Fantastique'),
('la+momie+3', 'Fantastique'),
('Le+fanteme+de+mon+ex-fiancee', 'Fantastique'),
('Pan''s+Labyrinth', 'Fantastique'),
('Predictions', 'Fantastique'),
('spiderman+3', 'Fantastique'),
('The+Butterfly+Effect', 'Fantastique'),
('X-Men+Origins,+Wolverine', 'Fantastique'),
('Zoom', 'Fantastique'),
('Apocalypse+Now', 'Guerre'),
('Arthur+3+La+Guerre+Des+Deux+Mondes', 'Guerre'),
('Australia', 'Guerre'),
('Braveheart', 'Guerre'),
('Flags+Of+Our+Fathers', 'Guerre'),
('Hors+La+Loi', 'Guerre'),
('La+Guerre+des+Peres', 'Guerre'),
('Le+Dernier+Samourai', 'Guerre'),
('Les+insures', 'Guerre'),
('MONGOL', 'Guerre'),
('Valse+avec+Bachir', 'Guerre'),
('Braveheart', 'Historique'),
('Hors+La+Loi', 'Historique'),
('Jeanne+d''Arc', 'Historique'),
('Kingdom+Of+Heaven', 'Historique'),
('La+Derniere+Tentation+Du+Christ', 'Historique'),
('Le+Dernier+Samourai', 'Historique'),
('Le+Roi+Arthur', 'Historique'),
('Marie+Antoinette', 'Historique'),
('MONGOL', 'Historique'),
('valkyrie', 'Historique'),
('Everyone+Says+I+Love+You', 'Musical'),
('Sweet+and+Lowdown', 'Musical'),
('The+Visitor', 'Musical'),
('Hercule', 'Peplum'),
('Chaos', 'Policier'),
('Dragon+Rouge=', 'Policier'),
('Fast+and+Furious+5+Fast+Five', 'Policier'),
('Ghost+Dog', 'Policier'),
('Ghost+Dog+The+Way+of+the+Samurai', 'Policier'),
('L''Arme+Fatale', 'Policier'),
('L''attaque+du+metro+123', 'Policier'),
('L''Elite+De+Brooklyn', 'Policier'),
('Mise+a+Prix', 'Policier'),
('Slevin', 'Policier'),
('Un+Prophete', 'Policier'),
('Very+Bad+Things', 'Policier'),
('We+Own+The+Night', 'Policier'),
('Australia', 'Romance'),
('Burlesque', 'Romance'),
('Ce+Soir+Je+Dors+Chez+Toi', 'Romance'),
('Charlie,+les+filles+lui+disent+merci', 'Romance'),
('Crazy+stupid+love', 'Romance'),
('Dieu+Est+Grand,+Je+Suis+Toute+Petite', 'Romance'),
('Donne-moi+ta+main', 'Romance'),
('Everyone+Says+I+Love+You', 'Romance'),
('Girls+and+Sex', 'Romance'),
('L''Arnacoeur', 'Romance'),
('Le+fanteme+de+mon+ex-fiancee', 'Romance'),
('Le+Grand+Bleu', 'Romance'),
('Le+sourire+de+Mona+Lisa', 'Romance'),
('Love+And+Other+Drugs', 'Romance'),
('Manhattan', 'Romance'),
('Memoire+D''une+Geisha', 'Romance'),
('Mon+babysitter', 'Romance'),
('MONGOL', 'Romance'),
('P+S+I+Love+You', 'Romance'),
('Rendez+vous+l''ete+prochain', 'Romance'),
('Slumdog+Millionaire', 'Romance'),
('The+Notebook', 'Romance'),
('The+Visitor', 'Romance'),
('30+jours+de+nuit', 'Thriller'),
('88+minutes', 'Thriller'),
('Awake', 'Thriller'),
('Casino+Royale', 'Thriller'),
('Da+Vinci+Code', 'Thriller'),
('Dirty+Pretty+Things', 'Thriller'),
('District+9+', 'Thriller'),
('Dragon+Rouge=', 'Thriller'),
('Equilibrium', 'Thriller'),
('Fast+and+Furious+5+Fast+Five', 'Thriller'),
('Fermats+Room', 'Thriller'),
('Ghost+Dog', 'Thriller'),
('Ghost+Dog+The+Way+of+the+Samurai', 'Thriller'),
('Hypertension', 'Thriller'),
('Jason+Bourne+3+-+La+Vengeance+dans+la+Peau', 'Thriller'),
('Kill+Bill+1', 'Thriller'),
('Kill+Bill+2', 'Thriller'),
('Kiss+and+Kill', 'Thriller'),
('L''attaque+du+metro+123', 'Thriller'),
('Law+Abiding+Citizen', 'Thriller'),
('Le+Baiser+Mortel+Du+Dragon', 'Thriller'),
('L''Elite+De+Brooklyn', 'Thriller'),
('L''Enquête+-+The+International', 'Thriller'),
('Mensonges+d''Etat', 'Thriller'),
('Mise+a+Prix', 'Thriller'),
('Predictions', 'Thriller'),
('Scarface', 'Thriller'),
('Sheitan', 'Thriller'),
('Shutter+Island', 'Thriller'),
('Slevin', 'Thriller'),
('The+Devils+Double', 'Thriller'),
('The+Girl+Next+Door', 'Thriller'),
('The+Tourist', 'Thriller'),
('valkyrie', 'Thriller'),
('We+Own+The+Night', 'Thriller');
