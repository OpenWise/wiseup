<?php

/*
 * $_SERVER['HTTP_HOST'] 		- ykiveish-ubuntu.cloudapp.net
 * $_SERVER['DOCUMENT_ROOT'] 	- /var/www
 */

/*
 * Routing service page.
 */

session_start();
 
include_once($_SERVER['DOCUMENT_ROOT']."/websites/wiseup-dev/web/config.php");

// check if session exists
if (!isset($_SESSION['upid'])) {
	// redirect to main application page
	include (root_dir."/auth.php");
} else {
	// session doesn't exist, create session
	include (root_dir."/app.php");
}

// header("Location: http://".web_entry_point);
 
?>
