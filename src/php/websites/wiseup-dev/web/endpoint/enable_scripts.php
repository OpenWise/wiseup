<?php

session_start();

header ("Expires: Sun, 6 Jul 2003 08:00:00 GMT");
header ("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");

include_once ("../config.php");

function parse_request_data($requestMethod, $json_data) {
	$response_stream = 'error';
	switch ($requestMethod) {
		case "debug":
			return "DEBUG";
		break;
	/* *** GET SECTION *** */
		case "get_favorite_enable_scripts":
			if (!isset($_SESSION['upid']))
				return 0;
			
			$query = sprintf("SELECT ES.enable_script_id, ES.enable_script, ES.disable_script, GE.is_enabled AS `value`, GE.general_event_name AS `enable_script_name` FROM `enable-script` ES ".
							"JOIN `general-events` GE ON GE.enable_script_id = ES.enable_script_id WHERE `is_favorite`=1 AND `profile_id`=%s", $_SESSION['upid']);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['enable_scripts_info'][] = $item;
				}
			} else {
				$response_json['enable_scripts_info'][] = "";
			}
			
			$response_stream = json_encode($response_json);
		break;
	/* *** SET SECTION *** */
		case "set_event_script_favorite_value":
			if (!isset($_SESSION['upid']))
				return 0;
			
			$query = sprintf("UPDATE `enable-script` SET `is_favorite`=%s WHERE `enable_script_id`=%s", $json_data->favorite_value, $json_data->event_id);
			$db_data = mysql_query ($query);
			
			$response_stream = "success";
		break;
		case "execute_enable_script":
			if (!isset($_SESSION['upid']))
				return 0;
			
			/*
			 * 1) Get the en/dis script for current script.
			 * 2) Execute the script.
			 * 3) Update the enable-script table with current value.
			 */
			$query = sprintf("SELECT `enable_script`, `disable_script` FROM `enable-script` WHERE `enable_script_id`=%s", $json_data->en_script_id);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				$item = mysql_fetch_object($db_data);
				
				if ($json_data->value == 1) {
					$execution_query = $item->enable_script;
				} else {
					$execution_query = $item->disable_script;
				}
				
				$response_stream = $execution_query;
				$db_data = mysql_query ($execution_query);
				
				$response_stream = "success";
			}
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
