<?


if(isset($_REQUEST['registration'])) {
    $error = '';
    $login = mysql_real_escape_string($_REQUEST['login']);
    $name = mysql_real_escape_string($_REQUEST['name']);
    $email = mysql_real_escape_string($_REQUEST['email']);
    $password = mysql_real_escape_string($_REQUEST['password']);
    $repassword = mysql_real_escape_string($_REQUEST['repassword']);
    $tel = mysql_real_escape_string($_REQUEST['tel']);
    $baseid = mysql_real_escape_string($_REQUEST['baseid']);
    
    if(!empty($login)) {
        //checking such login and userName
        $sql = mysql_query("SELECT * FROM `Profiles` WHERE  `UserName`='".$login."'");
        if(mysql_num_rows($sql)>0) {
            $error .= '<br/><span style="color:red;">User with such login is exists, please choose another.</span>';
        }
        
        if($password!=$password) {
            $error .= '<br/><span style="color:red;">Password and repeat password should be the same.</span>';
        }
        
        if($error == '') {
            mysql_query("INSERT INTO `Profiles` (`ProfileID`, `UserName`, `Password`, `LastDateVisit`, `UserType`, `last_name`, `first_name`, `email`, `phone`) 
            VALUES (NULL, '".$login."', '".md5($password)."', '', '0', '".$name."', '', '".$email."', '".$tel."');");
            
            $id = mysql_insert_id();
            if(!empty($bsid)) {
              mysql_query("INSERT INTO `base-station` (`base_station_id`, `ProfileID`, `Name`, `Description`) 
              VALUES ('".$baseid."', '".$id."', '', '');");          
            }
            
            $content = 'Success! Now you can login using your login and password.<br/><br/>
            <center><a href="/">Login page</a></center>';
        } else {
            $content = '<b style="color:red;">Error!</b><br/>'.$error.'
            <center><a href="/">Login page</a></center>';
        }
    }
    
    
}

