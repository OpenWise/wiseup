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
		case "check_deamon_alive":				
			$fp = fsockopen("unix:///tmp/wiseup/deamon_admin_php", 0, $errno, $errstr, 30);
			if (!$fp) {
				$answer = "false";
			} else {
				$dataJSON = '{"request":1}';
				fwrite ($fp, $dataJSON);
				$answer = fread ($fp, 128);
				fclose ($fp);
			}
			
			$response_json['is_alive'] = $answer;
			$response_stream = json_encode($response_json);
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
