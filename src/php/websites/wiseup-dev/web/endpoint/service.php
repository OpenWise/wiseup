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
		case "get_services_list":
			if (!isset($_SESSION['upid']))
				return 0;
				
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();

			$query = sprintf("SELECT `service_type_id`, `service_type_description` FROM `service-type`");
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['services_info'][] = $item;
				}
				$response_stream = json_encode($response_json);
			}
		break;
		case "get_enabled_services_list":
			if (!isset($_SESSION['upid']))
				return 0;
				
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();

			$query = sprintf("SELECT `service_type_id`, `service_type_description` FROM `service-type` WHERE `is_enabled`=1");
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['services_info'][] = $item;
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
