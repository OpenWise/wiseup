<? 
if(isset($_REQUEST['add_sensor_to_place'])) {
    $rid = intval($_REQUEST['rid']);
    $sid = intval($_REQUEST['sensor']);
    
    mysql_query("UPDATE `Sensors` SET place='".$rid."' WHERE SensorId ='".$sid."'");    
}

if(isset($_REQUEST['add_place'])) {
    $place_name = mysql_real_escape_string($_REQUEST['place']);
    
    mysql_query("INSERT INTO `Rooms` (`ProfileID`, `Title`) VALUES (NULL, '".$place_name."');");
    header("Location:http://".$_SERVER['HTTP_HOST'].'/places');
}

$functions = '';
$content = '<div class="page-header">
                <h1 id="lbl-selected-menu">  Places </h1>
			</div>
            <div class="row">
				<div class="col-sm-12">
					<form action="" method="post">
						<div style="width:25%">
							<div class="input-group">
								<input type="text" id="form-field-1" placeholder="Place name" name="place" value=""  class="form-control"/>
								<span class="input-group-btn"><input type="submit" class="btn btn-sm btn-info" name="add_place" value="Add" /></span> 
							</div> 
						</div>
					</form>
					<br/><br/>
				</div>';

$places_query = mysql_query("SELECT * FROM `Rooms` WHERE `ProfileId`='".$_SESSION['upid']."'");
while($place = mysql_fetch_object($places_query)) {
    // sensors select
    $select = '<select name="sensor" class="form-control">';
    $sensors_query = mysql_query("SELECT * FROM `Sensors` WHERE `place` = '0' ");
    while($sensors = mysql_fetch_object($sensors_query)) {
        $select .= '<option value="'.$sensors->SensorId.'">'.stripslashes($sensors->Name).'</option>';
    }    
    $select .= '</select>';
	$content .= '<div class="col-sm-4">
					<div class="widget-box">
						<div class="widget-header">
							<h4>				
								'.$place->Title.'
							</h4>
						</div>
						<div class="widget-body">
							<div class="widget-main">
								<div class="infobox-container" id="place'.$place->RoomId.'"></div>
                        <div>
							<form action="" method="post">
								<div class="input-group">
									'.$select.'                
									<span class="input-group-btn">&nbsp; <input type="submit" name="add_sensor_to_place" value="+ Add sensor" class="btn btn-sm btn-info"/></span>              
								</div>
								<input type="hidden" name="rid" value="'.$place->RoomId.'"/>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>';
	
	$functions.='get_devices_for_place('.$place->RoomId.'); setInterval(function(){  get_devices_for_place('.$place->RoomId.') }, 5000);';
}

$content .= '</div>';
$js_scripts = $functions;

?>
