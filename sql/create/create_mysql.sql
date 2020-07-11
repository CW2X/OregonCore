-- --------------------------------------------- --
-- MaNGOS Script to Create MySQL User and Tables --
-- --------------------------------------------- --

GRANT USAGE ON * . * TO 'mangos'@'localhost' IDENTIFIED BY 'mangos' WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0 ;

CREATE DATABASE `world` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `characters` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `realmd` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

GRANT ALL PRIVILEGES ON `world` . * TO 'mangos'@'localhost' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON `characters` . * TO 'mangos'@'localhost' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON `realmd` . * TO 'mangos'@'localhost' WITH GRANT OPTION;