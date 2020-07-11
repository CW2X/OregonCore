-- --------------------------------------------- --
-- MaNGOS Script to Remove MySQL User and Tables --
-- --------------------------------------------- --

REVOKE ALL PRIVILEGES ON * . * FROM 'mangos'@'localhost';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'mangos'@'localhost';
REVOKE GRANT OPTION ON `world` . * FROM 'mangos'@'localhost';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'mangos'@'localhost';
REVOKE GRANT OPTION ON `characters` . * FROM 'mangos'@'localhost';

REVOKE ALL PRIVILEGES ON `realmd` . * FROM 'mangos'@'localhost';
REVOKE GRANT OPTION ON `realmd` . * FROM 'mangos'@'localhost';

DROP USER 'mangos'@'localhost';

DROP DATABASE IF EXISTS `world` ;
DROP DATABASE IF EXISTS `characters` ;
DROP DATABASE IF EXISTS `realmd` ;