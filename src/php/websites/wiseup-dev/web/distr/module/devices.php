<?
$bsql = mysql_query("SELECT * FROM `base-station` WHERE  	ProfileID ='".$_SESSION['upid']."'");
$bres = mysql_fetch_object($bsql);

$devices = '';
$sql = mysql_query("SELECT * FROM EndStation WHERE BaseStationID='".$bres->base_station_id."'");
while($res = mysql_fetch_object($sql)) {
    $devices.='<div class="col-xs-12 col-sm-4"><div class="widget-box"> <div class="widget-header widget-header-blue widget-header-flat"><h4 class="lighter">'.$res->Title.'</h4></div></div>
                <div class="widget-body"><div class="widget-main"> ... </div></div>
    </div>';
}
$content = '<div class="page-header">
							<h1 id="lbl-selected-menu">  Devices </h1>
						</div>
						<div class="row">
							     '.$devices.'
                        </div>';