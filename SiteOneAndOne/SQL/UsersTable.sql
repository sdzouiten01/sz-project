
-- Version de PHP: 5.3.2-1ubuntu4.7

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

-- --------------------------------------------------------

--
-- Structure de la table `UsersTable`
--

CREATE TABLE IF NOT EXISTS `UsersTable` (
  `ID` int(11) NOT NULL,
  `USERNAME` char(32) COLLATE latin1_general_ci NOT NULL,
  `USERPWD` varchar(40) COLLATE latin1_general_ci NOT NULL,
  `USERMAIL` varchar(40) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

--
-- Contenu de la table `UsersTable`
--

INSERT INTO `UsersTable` (`ID`, `USERNAME`, `USERPWD`, `USERMAIL`) VALUES
(1, 'saad', 'saad13041985', 'sdzouiten01@gmail.com'),
(2, 'ali', 'ali13041985', 'XXX'),
(3, 'khalid', 'khalid13041985', 'XXX'),
(4, 'amal', 'amal13041985', 'XXX'),
(5, 'ghita', 'ghita13041985', 'XXX');
