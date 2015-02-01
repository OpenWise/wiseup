<?

require_once($_SERVER['DOCUMENT_ROOT'].'/distr/phpmailer/class.phpmailer.php');

if(isset($_REQUEST['forgot'])) {
  
    $email = mysql_real_escape_string($_REQUEST['email']);
    
    $sql = mysql_query("SELECT * FROM `Profiles` WHERE `email`='".$email."'");
    if(mysql_num_rows($sql) && $email!='') {
        
        $res = mysql_fetch_object($sql);
         
         //generate new password
         $new_pass = md5(rand(0,10000));
         $new_pass = substr($new_pass,0,5);   
         
         mysql_query("UPDATE `Profiles` SET `Password` = '".$new_pass."' WHERE id='".$res->id."'");
            
        $logo = $_SERVER['DOCUMENT_ROOT'].'/assets/img/wiseup.png';
        $admin_email = 'info@wiseuphome.com';
        
        $text_mail = '<img src="'.$logo.'" alt="WiseUp" width="298" height="117" /><br/><br/>
        
        Good day. <br/><br/>
        <strong>Your login:</strong> '.$res->login.' <br/>
        <strong>Your new password:</strong> '.$new_pass.'  <br/><br/>
        
        You can go to <a href="http://'.$_SERVER['HTTP_HOST'].'">login page</a> and sign using your password and login. Later you can change it. 
        
        <br/><br/>
        
        Thanks!
        
        <br/><br/>
        <i>WiseUpHome.com</i>';
        
                $mail = new PHPMailer();          
                $mail->CharSet = "utf-8";
                // from email
                $mail->From = $admin_email;
                // from name
                $mail->FromName = 'WiseUp';
                $mail->AddReplyTo($admin_email,'WiseUp');
                $mail->SetFrom($admin_email,'WiseUp');
                // letter theme
                $mail->Subject = 'Retrieve Password';
                
                // alt text
                $mail->AltBody = $text;
                // attach logo, text
                $mail->MsgHTML($text_mail);                
                $mail->AddAttachment($logo);           
                
                //add email
                $mail->AddAddress($res->email);
                
                if($mail->Send()) {
                  $content = '<center><div class="green">Instructions were send to your email, please check it.</div></center>';   
                }
    } else {
        $content = '<center><div class="red">Error! There is no such email.</div> <br/><a href="/">Back to main page &raquo;</a></div></center>';   
    }
    
}

