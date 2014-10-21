<?php

header ("Expires: Sun, 6 Jul 2003 08:00:00 GMT");
header ("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");

include '../lib/db.php';

session_start();

$data_base 	= 'wiseup-dev';
$user_name 	= 'wiseup-dev';
$password 	= 'wiseup4u@';

function parse_request_data($requestMethod, $json_data) {
	$response_stream = 'error';
	switch ($requestMethod) {
		case "debug":
			return "DEBUG";
		break;
	/* *** GET SECTION *** */
		case "get_graph_data_by_sensor_id":
			if (!isset($_SESSION['upid']))
				return 0;
				
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();

			$query = sprintf("SELECT sensor_log_data_id, sensor_value ".
							"FROM ( SELECT sensor_log_data_id, sensor_value FROM `sensor-log-data` ".
									"WHERE sensor_id = $s ".
									"ORDER BY `sensor_log_data_id` DESC LIMIT 50 ) AS tbl ", $json_data->sensor_id);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['sensor_graph_data'][] = $item;
				}
				$response_stream = json_encode($response_json);
			}
		break;
	/* *** SET SECTION *** */
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
