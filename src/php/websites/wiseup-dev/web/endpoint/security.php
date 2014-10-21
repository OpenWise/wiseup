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
		case "login":						
			$password_md5 = md5 ($json_data->password);
			$query = sprintf("SELECT * FROM `Profiles` WHERE `UserName`='%s' AND `Password`='%s'", 
										$json_data->username, 
										$password_md5);
			$db_data = mysql_query ($query);
			$row_count = mysql_num_rows($db_data);
			if ($row_count > 0) {
				$tbl_data = mysql_fetch_object($db_data);
				$_SESSION['user'] 			= $tbl_data->UserName;
				$_SESSION['admin'] 			= $tbl_data->UserType;
				$_SESSION['upid'] 			= $tbl_data->ProfileID;
				$_SESSION['hash'] 			= md5 ($tbl_data->UserName.$tbl_data->Password);  
				$_SESSION['obj_user_info'] 	= $tbl_data;
				
				$response_stream = "TRUE";
			} else {
				$response_stream = "FALSE";
			}
		break;
		case "logout":
			session_destroy();
			$response_stream = "TRUE";
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
