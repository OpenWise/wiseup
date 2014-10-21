<? 
// =================== Removing cache ========================
header ("Expires: Sun, 6 Jul 2003 08:00:00 GMT");
header ("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");
 
//========================== Mysql base options ===============
//host
define('bd_host','localhost');
//base
define('bd_name','wiseup');
//mysql user
define('bd_user','root');
//password
define('bd_password','sql123');

//========================== Connecting to Mysql Base  =========================
@mysql_connect(bd_host, bd_user, bd_password) or $error['mysql'] = mysql_errno() . ": " . mysql_error(). "<br>";
@mysql_select_db(bd_name) or die(mysql_errno());

mysql_query("SET NAMES utf-8");

//========================== Constants =========================================
date_default_timezone_set('Europe/Moscow');
define ('root_dir',			$_SERVER['DOCUMENT_ROOT']."/websites/wiseup-dev/web");
define ('root_link', 		$_SERVER['HTTP_HOST']."/websites/wiseup-dev/web");
define ('web_entry_point', 	$_SERVER['HTTP_HOST']."/websites/wiseup-dev/web/index.php");
