<?php

session_start();

header ("Expires: Sun, 6 Jul 2003 08:00:00 GMT");
header ("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");

include_once ("../config.php");

function parse_request_data($requestMethod, $json_data) {
	$response_stream = 'ERROR';
	switch ($requestMethod) {
		case "debug":
			return "DEBUG";
		break;
	/* *** GET SECTION *** */
		case "get_sensors_info":				
			$query = sprintf("call sp_get_sensors_info()  ");
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
                    $response_json['sensor_info'][] = $item;
				}
				$response_stream = json_encode($response_json);
			} else {
				$response_stream = "NO_DATA";
			}
		break;
		case "set_sensor_action":
			$query = sprintf("call sp_set_sensors_action( %s, %s )  ", $json_data->sensor_id, $json_data->action);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				$fp = fsockopen("unix:///tmp/wiseup/nrf_outgoing_queue_php", 0, $errno, $errstr, 30);
				if (!$fp) {
				    $response_stream = "$errstr ($errno)";
				} else {
					$item = mysql_fetch_object($db_data);

					// $dataJSON = json_encode($data);
					$dataJSON = '{'.'"sensor_hub_address":'.$item->sensor_hub_address.
								',"sensor_address":'.$item->sensor_address.
								',"action":'.$item->action.'}';
				    fwrite($fp, $dataJSON);
				    fclose($fp);

				    $response_stream = $dataJSON;
				}
			} else {
				$response_stream = "NO_DATA";
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
