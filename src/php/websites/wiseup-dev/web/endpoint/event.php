<?php

session_start();

header ("Expires: Sun, 6 Jul 2003 08:00:00 GMT");
header ("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");

include_once ("../config.php");

//include '../lib/db.php';
// $data_base 	= 'wiseup-dev';
// $user_name 	= 'wiseup-dev';
// $password 	= 'wiseup4u@';

function create_action_query ($action_type, $action_sensor_id) {
	switch ($action_type) {
		case 0:
			return "";
		case 1: // ON
			return sprintf("UPDATE Sensors SET `SensorWebValue`=%s WHERE SensorId=%s", 1, mysql_real_escape_string($action_sensor_id));
		case 2: // OFF
			return sprintf("UPDATE Sensors SET `SensorWebValue`=%s WHERE SensorId=%s", 0, mysql_real_escape_string($action_sensor_id));
		break;
		default:
		break;
	}
	
	return "";
}

function parse_request_data($requestMethod, $json_data) {
	$response_stream = 'ERROR';
	switch ($requestMethod) {
		case "debug":
			return "DEBUG";
		break;
	/* *** GET SECTION *** */
		case "get_event_list_by_sensor_id":
			if (!isset($_SESSION['upid']))
				return 0;
				
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();

			$query = sprintf("SELECT WE.event_name, ".
								"WE.event_description, ".
								"GE.general_event_id, ".
								"GE.general_event_name, ".
								"GE.general_event_type, ".
								"GE.general_risk_type_id, ".
								"GE.is_enabled, ".
								"GE.enable_script_id, ".
								"GA.general_action_type, ".
								"GA.general_action_sensor_id, ".
								"ES.is_favorite AS `is_favorite_enable_script`, ".
								"GSET.general_static_event_type_name, ".
								"GSET.general_static_event_type_description, ".
								"ERT.event_risk_description ".
								"FROM `general-events` GE ".
								"JOIN `wiseup-events` WE ON GE.general_event_type = WE.event_id ". 
								"JOIN `general-static-event-type` GSET ON GSET.general_static_event_type_id = WE.wiseup_event_type_id ".
								"JOIN `event-risk-type` ERT ON ERT.event_risk_id = GE.general_risk_type_id ".
								"JOIN `enable-script` ES ON ES.enable_script_id = GE.enable_script_id ".
								"JOIN `general-actions` GA ON GA.general_action_id = GE.general_action_id ".
								"WHERE GE.general_event_sensor_id = %s ", $json_data->sensor_id);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['sensor_events_info'][] = $item;
					
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
		case "get_wiseup_events_list_by_sensor_type":
			if (!isset($_SESSION['upid']))
				return 0;
				
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();

			$query = sprintf("SELECT `event_id`, `event_name`, `event_description` FROM `wiseup-events` WHERE `sensor_type`=%s ", $json_data->sensor_type);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['wiseup_events_info'][] = $item;
				}
				$response_stream = json_encode($response_json);
			} else {
				$response_stream = "NO_DATA";
			}
		break;
		case "get_sensors_by_list_type":
			if (!isset($_SESSION['upid']))
				return 0;
				
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			
			$query = sprintf("SELECT `SensorId`, `Name`, `SensorType`  FROM `Sensors` WHERE `SensorType` IN (%s)", $json_data->sensor_types);
			
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['sensors_small_info'][] = $item;
				}
				$response_stream = json_encode($response_json);
			} else {
				$response_stream = "NO_DATA";
			}
		break;
		case "get_event_info":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			
			$query = sprintf("SELECT GE.general_event_name, GA.general_action_id, GA.general_action_type, S.Name, GA.general_action_sensor_id, WE.event_name ".
							"FROM `general-events` GE ".
							"LEFT JOIN `general-actions` GA ON GA.general_action_id = GE.general_action_id ".
							"LEFT JOIN `Sensors` S ON S.SensorId = GE.general_event_sensor_id ".
							"LEFT JOIN `wiseup-events` WE ON WE.event_id = GE.general_event_type ".
							"WHERE GE.general_event_id=%s", $json_data->event_id);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['event_info'][] = $item;
				}
			} else {
				$response_json['event_info'][] = "NO_DATA";
			}
			
			$query = sprintf("SELECT S.service_id, ST.service_type_id, ST.service_type_description ".
							"FROM `services` S ".
							"JOIN `service-type` ST ON S.service_type_id = ST.service_type_id ".
							"WHERE S.is_deleted=0 AND S.event_id=%s", $json_data->event_id);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['event_service_info'][] = $item;
				}
			} else {
				$response_json['event_service_info'][] = "NO_DATA";
			}
			
			$query = sprintf("SELECT `service_type_id`, `service_type_description` ".
							"FROM `service-type` ".
							"WHERE `is_enabled`=1");
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['services_info'][] = $item;
				}
			} else {
				$response_json['services_info'][] = "NO_DATA";
			}
			
			$response_stream = json_encode($response_json);
		break;
	/* *** SET SECTION *** */
		case "update_wiseup_event":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			
			$query = sprintf("SELECT ".
								"GE.general_event_id, ".
								"GE.general_action_id, ".
								"GA.general_action_type, ".
								"GA.general_action_sensor_id ".
								"FROM `general-events` GE ".
								"JOIN `general-actions` GA ON GA.general_action_id = GE.general_action_id ".
								"WHERE GE.general_event_id = %s ", $json_data->event_id);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				$event = mysql_fetch_object($db_data);
				$action_query = create_action_query ($json_data->selected_action_type_id, $event->general_action_sensor_id);
				// 1) Update action. 
				$query = sprintf("UPDATE `general-actions` SET `general_action_type`=%s, `general_action_query`='%s' WHERE `general_action_id`=%s ", 
																														$json_data->selected_action_type_id, 
																														$action_query,
																														$event->general_action_id);
				$db_data = mysql_query ($query);
				$general_action_id = mysql_insert_id();
				
				// 2) Update services.
				// 2.1) Get all service binded to this event.
				$query = sprintf("SELECT S.service_id, ST.service_type_id, ST.service_type_description ".
								"FROM `services` S ".
								"JOIN `service-type` ST ON S.service_type_id = ST.service_type_id ".
								"WHERE `event_id`=%s", $json_data->event_id);
				$db_data = mysql_query ($query);
				$row_count = mysql_num_rows($db_data);
				if ($row_count > 0) {
					while ($item = mysql_fetch_object($db_data)) {
						$services_list[] = $item;
					}
				}
				
				$response_stream = "";
			
				// 2.2) Get the list of services to update.
				$json_services_list = explode(',', $json_data->selected_service_list, -1);
				
				/*for ($i = 0; $i < count($services_list); $i++) {
					$response_stream = $response_stream.$services_list[$i]->service_type_id.",";
				}*/
				/*for ($i = 0; $i < count($json_services_list); $i++) {
					$response_stream .= $json_services_list[$i].",";
				}*/
				
				// 2.3) Note that client send services from index 1 to n but here we get it form 0 to n-1
				for ($service_id_index = 0; $service_id_index < count($json_services_list); $service_id_index++) {
					$found = 0;
					$current_service_type_id = $service_id_index + 1;
					$current_service_is_active = $json_services_list[$service_id_index];
					for ($j = 0; $j < count($services_list) && $found == 0; $j++) {
						if ($current_service_type_id ==  $services_list[$j]->service_type_id) {
							$found = 1;
							if ($current_service_is_active == '1') {
								$query = sprintf("UPDATE `services` SET `is_deleted`=0 WHERE `service_id`=%s ", $services_list[$j]->service_id);
							} else {
								$query = sprintf("UPDATE `services` SET `is_deleted`=1 WHERE `service_id`=%s ", $services_list[$j]->service_id);
							}
							$db_data = mysql_query ($query);
							$general_action_id = mysql_insert_id();
						}
					}
					
					if ($found == 0) {
						if ($current_service_is_active == '1') {
							// Create new service.
							$query = sprintf("INSERT INTO `services`(`service_id`, `service_type_id`, `service_timestamp`, `event_id`, `is_deleted`) ".
											"VALUES (NULL, %s, %s, %s, 0)", $current_service_type_id, time(), $json_data->event_id);
							$db_data = mysql_query ($query);
							$ret_service_id = mysql_insert_id();
						} else {
							// Do nothing.
						}
					}
				}
				
				$response_stream = "update_wiseup_event_success";
			} else {
				$response_stream = "NO_DATA";
			}
		break;
		case "create_wiseup_event":
			if (!isset($_SESSION['upid']))
				return 0;
			
			//$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			//$wiseup_db->connectDB();
			
			$action_query = create_action_query($json_data->action_type, $json_data->action_sensor_id);
			
			// 1) Create new action.
			$query = sprintf("INSERT INTO `general-actions`(`general_action_id`, `next_general_action_id`, `general_action_type`, `general_action_sensor_id`, `general_action_query`, `is_enabled`) ".
							"VALUES (NULL, %s, %s, %s, '%s', %s)", 0, $json_data->action_type, $json_data->action_sensor_id, $action_query, 1);
			$db_data = mysql_query ($query);
			$general_action_id = mysql_insert_id();
			
			// 2) Create new general event.
			$query = sprintf("INSERT INTO `general-events`(`general_event_id`, `general_event_name`, `general_event_sensor_id`, `general_action_id`, ".
															"`general_event_type`, `general_risk_type_id`, `general_event_interval`, `general_event_timestamp`, `enable_script_id`, `is_enabled`) ".
							"VALUES (NULL, '%s', %s, %s, %s, %s, %s, %s, %s, %s)", $json_data->event_name, $json_data->sensor_id, $general_action_id, $json_data->wiseup_event_type, 1, 0, time(), 0, 1);
			$db_data = mysql_query ($query);
			$general_event_id = mysql_insert_id();
			
			// 3) Create new enable and disable scripts.
			$enable_query = sprintf("UPDATE `general-events` SET `is_enabled`=1 WHERE `general_event_id`=%s", $general_event_id);
			$disable_query = sprintf("UPDATE `general-events` SET `is_enabled`=0 WHERE `general_event_id`=%s", $general_event_id);
			$query = sprintf("INSERT INTO `enable-script`(`enable_script_id`, `profile_id`, `enable_script`, `disable_script`, `is_favorite`) VALUES (NULL, %s, '%s', '%s', %s)", $_SESSION['upid'], $enable_query, $disable_query, 0);
			$db_data = mysql_query ($query);
			$enable_script_id = mysql_insert_id();
			
			// 4) Update created general event with new enable_script_id.
			$query = sprintf("UPDATE `general-events` SET `enable_script_id`=%s WHERE general_event_id=%s", $enable_script_id, $general_event_id);
			$db_data = mysql_query ($query);
			
			// 5) Create service and attach event to it.
			$list = explode(',', $json_data->service_type_list, -1);
			for ($i = 0; $i < count($list); $i++) {
				$service_id = $list[$i];
				$query = sprintf("INSERT INTO `services`(`service_id`, `service_type_id`, `service_timestamp`, `event_id`, 	`is_deleted`) ".
								"VALUES (NULL, %s, %s, %s, 0)", $service_id, time(), $general_event_id);
				$db_data = mysql_query ($query);
				$ret_service_id = mysql_insert_id();
				
				if ($ret_service_id == 0) {
					return $query;
				}
			}
			
			$response_stream = "create_wiseup_event_success";
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
