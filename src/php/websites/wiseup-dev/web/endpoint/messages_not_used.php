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
		case "get_latest_messages":		
			if (!isset($_SESSION['upid']))
				return 0;
				
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();
			
			$query = "SELECT M.general_message_id,".
							"case ".
							" when DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%d-%m-%Y') = DATE_FORMAT(NOW(),'%d-%m-%Y') then DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%H:%i')".							
							"else DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%e %b %Y  %H:%i')".
							"end as general_messages_timestamp,".		
							"M.general_message_source, M.general_message_type_id, M.general_message_title, M.general_message_body, S.SensorType ".
							"FROM `user-messages` M ".
							"JOIN Sensors S ON 	S.SensorId = M.general_message_source ".
							"WHERE M.profile_id = " .$_SESSION['upid']. " and is_read = 0 ".	
							"ORDER BY M.general_messages_timestamp ASC ";
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['messages_info'][] = $item;
				}
			}
			$response_stream = json_encode($response_json);
		break;
		case "set_message_as_read":
			if (!isset($_SESSION['upid']))
				return 0;
			
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();
			
			$query = sprintf("UPDATE `user-messages` SET is_read = 1 WHERE profile_id = %d and general_message_id = %s",$_SESSION['upid'],$json_data->message_id);
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;
		case "set_all_messages_by_type_as_read":
			if (!isset($_SESSION['upid']))
				return 0;
			
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();
			
			$query = sprintf("UPDATE `user-messages` SET is_read = 1 WHERE profile_id = %d and general_message_type_id =  %s",$_SESSION['upid'],$json_data->message_type);
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;
		
		case "insert_message":
			if (!isset($_SESSION['upid']))
				return 0;
			
			$wiseup_db = new db('localhost', $data_base, $user_name, $password);
			$wiseup_db->connectDB();
			$query = sprintf("INSERT INTO `user-messages`(`general_messages_timestamp`, `general_message_type_id`, `general_message_source`, `general_message_title`, `general_message_body`, `is_read`) "."
							VALUES (%d,'%s',%s,%s,'%s','%s',%s)", 	$_SESSION['upid'],
																time(), 
																$json_data->message_type_id, 
																$json_data->message_source,
																$json_data->message_title,
																$json_data->message_body,
																0);
			$db_data = mysql_query ($query);
			$response_stream = $json_data->message_source;
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
