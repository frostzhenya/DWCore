/*
MySQL Data Transfer
Source Host: localhost
Source Database: realmd
Target Host: localhost
Target Database: realmd
Date: 21.01.2013 15:15:22
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for account
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(11) unsigned NOT NULL auto_increment COMMENT 'Identifier',
  `username` varchar(32) NOT NULL default '',
  `sha_pass_hash` varchar(40) NOT NULL default '',
  `gmlevel` tinyint(3) unsigned NOT NULL default '0',
  `sessionkey` longtext,
  `v` longtext,
  `s` longtext,
  `email` text,
  `joindate` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `last_ip` varchar(30) NOT NULL default '0.0.0.0',
  `failed_logins` int(11) unsigned NOT NULL default '0',
  `locked` tinyint(3) unsigned NOT NULL default '0',
  `last_login` timestamp NOT NULL default '0000-00-00 00:00:00',
  `active_realm_id` int(11) unsigned NOT NULL default '0',
  `expansion` tinyint(3) unsigned NOT NULL default '0',
  `mutetime` bigint(40) unsigned NOT NULL default '0',
  `locale` tinyint(3) unsigned NOT NULL default '0',
  `os` varchar(4) NOT NULL default '',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `idx_username` (`username`),
  KEY `idx_gmlevel` (`gmlevel`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Account System';

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `account` VALUES ('1', 'ADMINISTRATOR', 'a34b29541b87b7e4823683ce6c7bf6ae68beaaac', '3', 'CA83F3D396B7017908057979897D5702E20C09F5BF970E72A511E9FF71DEDF99C207CFEFE7AD1AAF', '29DDABF0C80169FF210D4EF22790688BF688E674818661CCFE32C087B3F5C60A', '887232A62ECF7BC1E227BAE837EB3DAE4B6C6DD56487ED452F85EC8A51491F19', '', '2006-04-25 13:18:56', '192.168.12.100', '0', '0', '2012-01-22 22:17:52', '0', '2', '0', '8', 'niW');
INSERT INTO `account` VALUES ('2', 'GAMEMASTER', '7841e21831d7c6bc0b57fbe7151eb82bd65ea1f9', '2', '', '0', '0', '', '2006-04-25 13:18:56', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0', '');
INSERT INTO `account` VALUES ('3', 'MODERATOR', 'a7f5fbff0b4eec2d6b6e78e38e8312e64d700008', '1', '', '0', '0', '', '2006-04-25 13:19:35', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0', '');
INSERT INTO `account` VALUES ('4', 'PLAYER', '3ce8a96d17c5ae88a30681024e86279f1a38c041', '0', '', '0', '0', '', '2006-04-25 13:19:35', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0', '');
INSERT INTO `account` VALUES ('5', 'LOVEPSONE', '549fad275db9cb8458e3bc14575684d7a38a3c72', '3', 'F41C73D6BF52DDBA7EAECED4D2F6FFD9EB14FB4A3889BE8C57FC34995BB3E832778ADE809928344A', '0423C5016BCF71854E440CF2DBF8BDE49D583D035A009874BBCCBD61BF866CE3', 'EF89C4599030D9E14906E7E6A7794B04639FDFEC51BF4431E6588459CAE9AAC3', 'lovepsone@mail.ru', '2012-01-22 22:20:39', '127.0.0.1', '0', '0', '2013-01-15 17:54:15', '0', '2', '0', '8', 'niW');
