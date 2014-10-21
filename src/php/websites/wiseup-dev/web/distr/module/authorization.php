<?

/*
 * This code will be launched after user entered login detail in the login page (login.tpl).
 * After the post request (login.tpl) to http:\\dev.fho.me\authorizarion the index.php will include this code.
 */

if(isset($_REQUEST['password'])) {
    $login = mysql_real_escape_string($_REQUEST['login']);
    $password = md5($_REQUEST['password']);
   
    $sql = mysql_query("SELECT * FROM `Profiles` WHERE `UserName`='".$login."' AND `Password`='".$password."'");
    if (mysql_num_rows($sql)) {
        $res = mysql_fetch_object($sql);
        $_SESSION['user'] = $res->UserName;
        $_SESSION['admin'] = $res->UserType;
        $_SESSION['upid'] = $res->ProfileID;
        $_SESSION['hash'] = md5($ures->UserName.$ures->Password);  
        $_SESSION['obj_user_info'] = $res;
		mysql_query("INSERT INTO `debug-php` (`id`, `timestamp`, `data`) VALUES (NULL, NOW(), '[authorization.php] ".$_SERVER['HTTP_HOST']."');");
		// redirect to the root url (i.e. dev.fho.me)
        header("Location:http://".$_SERVER['HTTP_HOST']);     
    }
}

if(isset($_REQUEST['logout'])) {
    session_destroy();
    header("Location:http://".$_SERVER['HTTP_HOST']);
}