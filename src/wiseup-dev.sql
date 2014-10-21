-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb2+deb7u1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 25, 2014 at 12:39 PM
-- Server version: 5.5.38
-- PHP Version: 5.4.4-14+deb7u14

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `wiseup-dev`
--
CREATE DATABASE `wiseup-dev` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `wiseup-dev`;

DELIMITER $$
--
-- Procedures
--
DROP PROCEDURE IF EXISTS `sp_get_sensors_info`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_get_sensors_info`(
gateway_address bigint )
BEGIN

SELECT * FROM  `sensors-info`  WHERE `sensor_gatway_address` = gateway_address;

END$$

DROP PROCEDURE IF EXISTS `sp_update_sensor_info`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_update_sensor_info`(
address bigint,
gateway_address bigint,
owner_address bigint,
type_t smallint,
embedded_value smallint,
app_value smallint )
BEGIN
IF EXISTS (SELECT * FROM  `sensors-info`  WHERE `sensor_address` = address) THEN
BEGIN
UPDATE `sensors-info` 
SET `sensor_embedded_value` = embedded_value,
`sensor_app_value` = app_value
WHERE `sensor_address` = address;
END;
ELSE
BEGIN
INSERT INTO `wiseup-dev`.`sensors-info` (`id`, `sensor_address`, `sensor_gatway_address`, `sensor_owner_address`, `sensor_type`, `sensor_name`, `sensor_embedded_value`, `sensor_app_value`) VALUES (NULL, address, gateway_address, owner_address, type_t, 'New sensor', embedded_value, app_value);
END;
END IF;
END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `sensors-info`
--

DROP TABLE IF EXISTS `sensors-info`;
CREATE TABLE IF NOT EXISTS `sensors-info` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sensor_address` bigint(20) unsigned NOT NULL,
  `sensor_gatway_address` bigint(20) unsigned NOT NULL,
  `sensor_owner_address` bigint(20) unsigned NOT NULL,
  `sensor_type` smallint(5) unsigned NOT NULL,
  `sensor_name` varchar(64) NOT NULL,
  `sensor_embedded_value` smallint(6) NOT NULL,
  `sensor_app_value` smallint(6) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=11 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
