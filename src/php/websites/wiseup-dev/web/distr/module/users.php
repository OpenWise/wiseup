<?
// ======================================================
// Module: Managing users for admin =====================
// ======================================================

if($_SESSION['admin'] == 1) {

//Add user on click    
if(isset($_REQUEST['add_user'])) {
    $username = mysql_real_escape_string($_REQUEST['username']);
    $name = mysql_real_escape_string($_REQUEST['name']);
    $useremail = mysql_real_escape_string($_REQUEST['useremail']);
    $tel = mysql_real_escape_string($_REQUEST['tel']);
    
    $password = mysql_real_escape_string($_REQUEST['password']);
    $bsname = mysql_real_escape_string($_REQUEST['bsname']);
    $bsid = mysql_real_escape_string($_REQUEST['bsid']);
    
    if(!empty($username)) {
        mysql_query("INSERT INTO `Profiles` (`ProfileID`, `UserName`, `Password`, `LastDateVisit`, `UserType`, `last_name`, `first_name`, `email`, `phone`) 
        VALUES (NULL, '".$username."', '".md5($password)."', '', '0', '".$name."', '', '".$useremail."', '".$tel."');");
        
        $id = mysql_insert_id();
        if(!empty($bsid)) {
          mysql_query("INSERT INTO `base-station` (`base_station_id`, `ProfileID`, `Name`, `Description`) 
          VALUES ('".$bsid."', '".$id."', '".$bsname."', '');");          
        }
        
        header("Location: http://".$_SERVER['HTTP_HOST'].'/users');
    }
}

//Edit user on click    
if(isset($_REQUEST['edit_user'])) {
    $id = intval($_REQUEST['id']);
    $username = mysql_real_escape_string($_REQUEST['username']);
    $name = mysql_real_escape_string($_REQUEST['name']);
    $useremail = mysql_real_escape_string($_REQUEST['useremail']);
    $tel = mysql_real_escape_string($_REQUEST['tel']);
    
    $password = mysql_real_escape_string($_REQUEST['password']);
    if(!empty($password)) {
        $pass_edit = "`Password`='".$username."',";
    } else $pass_edit = '';
    
    $bsname = mysql_real_escape_string($_REQUEST['bsname']);
    $bsid = mysql_real_escape_string($_REQUEST['bsid']);
    
    if(!empty($username)) {
        mysql_query("UPDATE `Profiles` SET `UserName`='".$username."', ".$pass_edit." `last_name`='".$name."', `first_name`='', `email`='".$useremail."', `phone`='".$tel."' WHERE `ProfileID`='".$id."'");
        
        echo ("UPDATE `Profiles` SET `UserName`='".$username."', ".$pass_edit." `last_name`='".$name."', `first_name`='', `email`='".$useremail."', `phone`='".$tel."' WHERE `ProfileID`='".$id."'");
        
       
        if(!empty($bsid)) {
            mysql_query("UPDATE `base-station` SET `base_station_id`='".$bsid."', `Name`='".$bsname."' WHERE `ProfileID`='".$id."'");      
            
            echo   ("UPDATE `base-station` SET `base_station_id`='".$bsid."', `Name`='".$bsname."' WHERE `ProfileID`='".$id."'");      
            
        }
        
        header("Location: http://".$_SERVER['HTTP_HOST'].'/users');
    }
}


//Delete user on click    
if(isset($_REQUEST['delete_user'])) {
    $id = intval($_REQUEST['id']);
    mysql_query("DELETE FROM `Profiles` WHERE `ProfileID` = '".$id."'");
}
    
$content = '<div class="page-header">
                <h1 id="lbl-selected-menu">  Users </h1>
            </div>
            <div class="row">
            <div class="col-xs-12">';                        

// Adding user form 
if(isset($_REQUEST['add'])) {
$content .='<form class="form-horizontal" role="form">
<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Login </label>
    <div class="col-sm-9">
        <input type="text" id="form-field-1" placeholder="Login" name="username"  class="col-xs-10 col-sm-5"/>
    </div>
</div>
<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Name </label>
    <div class="col-sm-9">
        <input type="text" id="form-field-1" placeholder="Name" name="name"  class="col-xs-10 col-sm-5"/>
    </div>
</div>


<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Email </label>
    <div class="col-sm-9">
        <input type="email" id="form-field-1" placeholder="User Email" name="useremail"  class="col-xs-10 col-sm-5"/>
    </div>
</div>
<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Telephone </label>
    <div class="col-sm-9">
        <input type="text" id="form-field-1" placeholder="Telephone Number" name="tel"  class="col-xs-10 col-sm-5"/>
    </div>
</div>
<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Password </label>
    <div class="col-sm-9">
        <input type="text" id="form-field-1" placeholder="Password" name="password" class="col-xs-10 col-sm-5"/>
    </div>
</div>
<hr/>
<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Base Station Name </label>
    <div class="col-sm-9">
        <input type="text" id="form-field-1" placeholder="Base Station Name" name="bsname" class="col-xs-10 col-sm-5"/>
    </div>
</div>
<div class="form-group">
    <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Base Station ID </label>
    <div class="col-sm-9">
        <input type="text" id="form-field-1" placeholder="Base Station ID" name="bsid" class="col-xs-10 col-sm-5"/>
    </div>
</div>

<div class="clearfix form-actions">
<div class="col-md-offset-3 col-md-9">
<input type="submit" class="btn btn-info" name="add_user" value="Add user" />
<input type="reset" class="btn" value="Reset" />

</div>
</form>';
} 
elseif(isset($_REQUEST['edit'])) {
    $id = intval($_REQUEST['id']);
    
    $sql = mysql_query("SELECT * FROM Profiles WHERE  ProfileID='".$id."'");
    $res = mysql_fetch_object($sql); 
    
     //Base station  info
     $bsql = mysql_query("SELECT * FROM `base-station` WHERE  ProfileID ='".$res->ProfileID."'");
     $bres = mysql_fetch_object($bsql);       
    
    $content .='<form class="form-horizontal" role="form">
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Login </label>
        <div class="col-sm-9">
            <input type="text" id="form-field-1" placeholder="Login" name="username" value="'.$res->UserName.'" class="col-xs-10 col-sm-5"/>
        </div>
    </div>
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Name </label>
        <div class="col-sm-9">
            <input type="text" id="form-field-1" placeholder="Name" name="name" value="'.$res->last_name.'"  class="col-xs-10 col-sm-5"/>
        </div>
    </div>
    
    
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Email </label>
        <div class="col-sm-9">
            <input type="email" id="form-field-1" placeholder="User Email" name="useremail" value="'.$res->email.'"   class="col-xs-10 col-sm-5"/>
        </div>
    </div>
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Telephone </label>
        <div class="col-sm-9">
            <input type="text" id="form-field-1" placeholder="Telephone Number" name="tel" value="'.$res->phone.'"   class="col-xs-10 col-sm-5"/>
        </div>
    </div>
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Password </label>
        <div class="col-sm-9">
            <input type="text" id="form-field-1" placeholder="Password" name="password" class="col-xs-10 col-sm-5"/>
        </div>
    </div>
    <hr/>
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Base Station Name </label>
        <div class="col-sm-9">
            <input type="text" id="form-field-1" placeholder="Base Station Name" name="bsname" value="'.$bres->base_station_id.'"  class="col-xs-10 col-sm-5"/>
        </div>
    </div>
    <div class="form-group">
        <label class="col-sm-3 control-label no-padding-right" for="form-field-1"> Base Station ID </label>
        <div class="col-sm-9">
            <input type="text" id="form-field-1" placeholder="Base Station ID" name="bsid" value="'.$bres->Name.'"  class="col-xs-10 col-sm-5"/>
        </div>
    </div>    
    <div class="clearfix form-actions">
    <div class="col-md-offset-3 col-md-9">
    <input type="hidden" name="id" value="'.$res->ProfileID.'"/>
    <input type="submit" class="btn btn-info" name="edit_user" value="Save" />   
    </div>    
    </form>';
} 
else {   

    $content .= '<a href="/users/?add=1" class="btn btn-sm btn-success">+ Add new user</a>
    <br/><br/>
    
    <table class="table table-striped table-bordered">
    <thead>
    <th class="center">#</th>
    <th>Name</th>
    <th>Base Station ID</th>
    <th>Base Station Name</th>
    <th>Action</th>
    </thead>
    <tbody>';
    $sql = mysql_query("SELECT * FROM Profiles WHERE  	UserType='0'");
    while($res = mysql_fetch_object($sql)) {
        
      //Base station  info
      $bsql = mysql_query("SELECT * FROM `base-station` WHERE  ProfileID ='".$res->ProfileID."'");
      $bres = mysql_fetch_object($bsql);
    
        $content.='<form action="/users/" method="post"><tr>
            <td class="center" style="width:50px;">'.$res->ProfileID.'</td>
            <td>'.$res->UserName.'</td>
            <td  style="width:150px;">'.$bres->base_station_id.'</td>
            <td  style="width:200px;">'.$bres->Name.'</td>
            <td style="width:150px;">
              <input type="hidden" name="id" value="'.$res->ProfileID.'"/>
              <input type="submit" name="edit" class="btn btn-sm btn-primary" value="edit"/> 
              <input type="submit" name="delete_user" class="btn btn-sm btn-danger" value="delete"/> 
           </td>';
        }    
    $content.='</tr></form></tbody>
    </table>';
 }  

  $content .= '</div></div>';                      
 }
                     