SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

-- --------------------------------------------------------

--
-- Structure de la table `MiniChatTable`
--

CREATE TABLE IF NOT EXISTS `MiniChatTable` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `pseudo` varchar(255) COLLATE latin1_general_ci NOT NULL,
  `message` varchar(255) COLLATE latin1_general_ci NOT NULL,
  `timesstamp` varchar(255) COLLATE latin1_general_ci NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=1006 ;

--
-- Contenu de la table `MiniChatTable`
--

INSERT INTO `MiniChatTable` (`ID`, `pseudo`, `message`, `timesstamp`) VALUES
(1, 'saad', 'Welcom to everybody !!', 'Fri, 07 Feb 2014 21:03:03 +0100');
