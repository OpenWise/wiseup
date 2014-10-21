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
		// used at inbox
		case "get_inbox_messages":		
			if (!isset($_SESSION['upid']))
				return 0;
							
			$query = "SELECT M.general_message_id,".
							"case ".
							" when DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%d-%m-%Y') = DATE_FORMAT(NOW(),'%d-%m-%Y') then DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%H:%i')".							
							"else DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%e %b %Y  %H:%i')".
							"end as general_messages_timestamp,".					
							"M.general_message_source, M.general_message_type_id, M.general_message_title, M.general_message_body, S.SensorType, case M.is_read when TRUE then '1' when FALSE then '0' end as is_read ".
							"FROM `user-messages` M ".							
							"JOIN Sensors S ON 	M.profile_id = ". $_SESSION['upid'] . " and S.SensorId = M.general_message_source ";
							
			if ($json_data->message_type > 0)										
				$query .= sprintf("WHERE M.general_message_type_id = %d ",$json_data->message_type);	
			// $query = sprintf("select * from (select * from ( %s ORDER BY M.general_messages_timestamp DESC limit %d)as x order by x.general_messages_timestamp ASC limit %d) as y order by y.general_messages_timestamp DESC",$query,$json_data->from,$json_data->total);	

			$query .= sprintf(" ORDER BY M.general_messages_timestamp DESC limit %d,%d",$json_data->from,$json_data->total);	
			
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['messages_info'][] = $item;
				}
			}
			$response_stream = json_encode($response_json);
		break;	
		
		// used at top side messages UI
		case "get_latest_messages":		
			if (!isset($_SESSION['upid']))
				return 0;
							
			// this must be nested select
			$query = "SELECT * FROM ".
					 "(SELECT M.general_message_id,".
							"case ".
							" when DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%d-%m-%Y') = DATE_FORMAT(NOW(),'%d-%m-%Y') then DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%H:%i')".							
							"else DATE_FORMAT(FROM_UNIXTIME(M.general_messages_timestamp),'%e %b %Y  %H:%i')".
							"end as general_messages_timestamp,".		
							"M.general_message_source, M.general_message_type_id, M.general_message_title, M.general_message_body, S.SensorType, is_read ".
							"FROM `user-messages` M ".
							"JOIN Sensors S ON 	S.SensorId = M.general_message_source ".
							"WHERE M.profile_id = " .$_SESSION['upid']. " ".	
							"ORDER BY M.general_messages_timestamp DESC ".
							"LIMIT 5 ) AS X ".
							"WHERE is_read = 0 ";
							
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				while ($item = mysql_fetch_object($db_data)) {
					$response_json['messages_info'][] = $item;
				}
			}
			$response_stream = json_encode($response_json);
		break;	
		// used for Inbox	
		case "set_messages_status":
			if (!isset($_SESSION['upid']))
				return 0;
			
			$query = sprintf("update `user-messages` set `is_read` = %s where profile_id = %d and general_message_id in (%s)",$json_data->set_to_status,$_SESSION['upid'],$json_data->msgs_list);
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;	
		// used for top side messages UI	
		case "set_message_as_read":
			if (!isset($_SESSION['upid']))
				return 0;
						
			$query = sprintf("UPDATE `user-messages` SET is_read = 1 WHERE profile_id = %d and general_message_id = %s",$_SESSION['upid'],$json_data->message_id);
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;
		// todo : need to set only the TOP 5 messages to READ status
		case "set_all_messages_by_type_as_read":
			if (!isset($_SESSION['upid']))
				return 0;
						
			$query = sprintf("UPDATE `user-messages` SET is_read = 1 WHERE profile_id = %d and general_message_type_id =  %s",$_SESSION['upid'],$json_data->message_type);
			$db_data = mysql_query ($query);
			$response_stream = 'success';
		break;
		
		// not used - insert message is being done from the API
		// case "insert_message":
			// if (!isset($_SESSION['upid']))
				// return 0;
			
			// $query = sprintf("INSERT INTO `user-messages`(`general_messages_timestamp`, `general_message_type_id`, `general_message_source`, `general_message_title`, `general_message_body`, `is_read`) "."
							// VALUES (%d,'%s',%s,%s,'%s','%s',%s)", 	$_SESSION['upid'],
																// time(), 
																// $json_data->message_type_id, 
																// $json_data->message_source,
																// $json_data->message_title,
																// $json_data->message_body,
																// 0);
			// $db_data = mysql_query ($query);
			// $response_stream = $json_data->message_source;
		// break;		
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
