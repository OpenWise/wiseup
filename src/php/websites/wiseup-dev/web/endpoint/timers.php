<?php

session_start();

header ("Expires: Sun, 6 Jul 2003 08:00:00 GMT");
header ("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");

//include '../lib/db.php';
include_once ("../config.php");



// $data_base 	= 'wiseup-dev';
// $user_name 	= 'wiseup-dev';
// $password 	= 'wiseup4u@';

function parse_request_data($requestMethod, $json_data) {
	$response_stream = 'error';
	switch ($requestMethod) {
		case "debug":
			return "DEBUG";
		break;
		case "insert_timer":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			$query = "";
			$query = sprintf("SELECT timer_id FROM timers WHERE timer_is_deleted = 1"); 
			
			// SENSORS-TIMER - TO DELETE THIS TABLE
			// ADD SENSOR_ID COLUMN IN TIMERS 
			// ADD ACTION_ID COLUMN (GENERAL-ACTION) IN TIMERS
			// ADD ACTION IN GENERAL-ACTION 
			
			$query = sprintf("INSERT INTO `timers`(`timer_name`, `timer_occurrence_type`, `timer_datetime`, `timer_occurrence_days`, `timer_action`, `timer_is_enabled`) "."
							VALUES ('%s',%d,'%s',%d,%d,%d)  ", 						
																$json_data->timer_name, 
																$json_data->timer_occurrence_type,
																$json_data->timer_datetime,																
																$json_data->timer_occurrence_days,
																$json_data->timer_action,
																1); // 1 = enabled true
																
			 $db_data = mysql_query ($query);
			 $query =  sprintf("INSERT INTO `sensors-timers`(`sensor_id`,`timer_id`) "."
							 VALUES (%d,%d) ", 	$json_data->sensor_id, 	mysql_insert_id()); 
							
			$db_data = mysql_query ($query);
			$response_stream =  mysql_insert_id();
		break;		
		case "update_timer":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			$query = sprintf("UPDATE `timers`".
							"SET`timer_name` = '%s'".
								", `timer_occurrence_type` = %d".
								", `timer_datetime` = '%s'".
								", `timer_occurrence_days` = %d".
								", `timer_action` = %d".
							" WHERE timer_id = %d"
							
							,$json_data->timer_name, 
							$json_data->timer_occurrence_type,
							$json_data->timer_datetime,																
							$json_data->timer_occurrence_days,
							$json_data->timer_action,
							$json_data->timer_id
							);

			$db_data = mysql_query ($query);
			$response_stream = $query;
		break;		
		case "get_timer_info":		
			if (!isset($_SESSION['upid']))
				return 0;
				
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			
			// cannot use sprintf due to date/time formatting %...
			$query = "SELECT S.`name` as sensor_name ,`timer_name`, `timer_occurrence_type`, DATE_FORMAT(timer_datetime, '%d-%m-%Y') as `timer_date`, DATE_FORMAT(timer_datetime, '%H:%i') as `timer_time`, `timer_occurrence_days`, `timer_action` ".							
							"FROM `timers` T ".	
							" INNER JOIN `sensors-timers` ST on T.timer_id = ST.timer_id and ST.sensor_id = ".$json_data->sensor_id.						
							" INNER JOIN `Sensors` S on S.sensorid = ST.sensor_id ";
							//" WHERE timer_id = ". $json_data->timer_id;
										
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['timer_info'][] = $item;
				}
			}
			$response_stream = json_encode($response_json);
		break;	
		
		case "get_timer_list_by_sensor_id":
			if (!isset($_SESSION['upid']))
				return 0;
				
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();

			$query = "SELECT T.`timer_id` ,`timer_name`,".						
						"case `timer_occurrence_type` when 1 then 'Once' when 2 then 'Daily' when 3 then 'Reoccurring' end as  `timer_occurrence_type`,".
						"DATE_FORMAT(timer_datetime, '%d-%b-%y') as `timer_date`, DATE_FORMAT(timer_datetime, '%H:%i') as `timer_time`, `timer_occurrence_days`,".
						"case `timer_action` when 0 then 'Off' when 1 then 'On' end as `timer_action`,".
						"`timer_is_enabled`".							
						"FROM `timers` T ".
						"INNER JOIN `sensors-timers` ST on T.timer_id = ST.timer_id and ST.sensor_id = ".$json_data->sensor_id;
													
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['sensor_timers_info'][] = $item;
					
					$sensor_info_query = sprintf("SELECT `Name` FROM `Sensors` WHERE `SensorId`=%s ", $item->general_action_sensor_id);
					$sensor_info_db_data = mysql_query ($sensor_info_query);
					$sensor_item = mysql_fetch_object($sensor_info_db_data);
					$response_json['sensor_action_sensor_info'][] = $sensor_item;
					
					$services_info_query = sprintf("SELECT `service_type_id` FROM `services` WHERE `is_deleted`=0 AND`event_id`=%s ", $item->general_event_id);
					$services_info_db_data = mysql_query ($services_info_query);
					$services_item = "";
					while ($service_item = mysql_fetch_object($services_info_db_data)) {
						$services_item[] = $service_item;
					}
					
					$response_json['services_info'][] = $services_item;
				}
				
				$response_stream = json_encode($response_json);
			} else {
				$response_stream = "NO_DATA";
			}
		break;
		case "set_timer_is_enabled":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			//$query = 'abc';
			$query = sprintf("UPDATE `timers` SET `timer_is_enabled` = %d WHERE `timer_id` = %d", $json_data->timer_is_enabled, $json_data->timer_id);
			
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;	
		case "delete_timer":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			//$query = 'abc';
			$query = sprintf("UPDATE `timers` SET `timer_is_deleted` = 1 WHERE `timer_id` = %d",$json_data->timer_id);
			
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;			
	}
	
	return $response_stream;
}

function pageLoad() {
	$request_steram = file_get_contents('php://input');
	$obj_json = json_decode($request_steram);
	
	$method = $obj_json->{'method'};
	echo parse_request_data ($method, $obj_json);
}

pageLoad();

?>
