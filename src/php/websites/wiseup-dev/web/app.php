<php?
 
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title>WiseUp - Dashboard</title>

		<meta name="description" content="overview &amp; stats" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />

		<!-- basic styles -->
        <link rel="shortcut icon" href="/favicon.ico"/>
		<link href="/websites/wiseup-dev/web/assets/css/bootstrap.min.css" rel="stylesheet" />
		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/font-awesome.min.css" />

		<!--[if IE 7]>
		  <link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/font-awesome-ie7.min.css" />
		<![endif]-->

		<!-- page specific plugin styles -->
		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/jquery-ui-1.10.3.full.min.css" />
		<!-- fonts -->

		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/ace-fonts.css" />

		<!-- ace styles -->

		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/uncompressed/ace.css" />
		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/ace-rtl.min.css" />
		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/ace-skins.min.css" />
		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/datepicker.css" /> 
		<link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/bootstrap-timepicker.css" />		
        <link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/wiseup.css" />

		<!--[if lte IE 8]>
		  <link rel="stylesheet" href="/websites/wiseup-dev/web/assets/css/ace-ie.min.css" />
		<![endif]-->

		<!-- inline styles related to this page -->

		<!-- ace settings handler -->

		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/ace-extra.js"></script>		
		
		<!-- WISEUP AREA -->
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/views.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/general.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/events.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/timers.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/graphs.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/inbox.js"></script>
		

		<!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->

		<!--[if lt IE 9]>
		<script src="/websites/wiseup-dev/web/assets/js/html5shiv.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/respond.min.js"></script>
		<![endif]-->
	</head>

	<body>
		<div id="modal_window_edit_event" class="hide">
		</div>
		<div id="modal_window_general_purpose" class="hide">
		</div>

		<div class="main-container" id="main-container">
			<script type="text/javascript">
				// try{ace.settings.check('main-container' , 'fixed')}catch(e){}
			</script>

			<div class="main-container-inner">
				<a class="menu-toggler" id="menu-toggler" href="#">
					<span class="menu-text"></span>
				</a>

				<div class="sidebar" id="sidebar">
					<script type="text/javascript">
						// try{ace.settings.check('sidebar' , 'fixed')}catch(e){}
					</script>

					<div class="sidebar-shortcuts" id="sidebar-shortcuts">
						<div class="sidebar-shortcuts-large" id="sidebar-shortcuts-large">
							<button class="btn btn-success">
								<i class="icon-signal"></i>
							</button>

							<button class="btn btn-info">
								<i class="icon-pencil"></i>
							</button>

							<button class="btn btn-warning">
								<i class="icon-group"></i>
							</button>

							<button class="btn btn-danger">
								<i class="icon-cogs"></i>
							</button>
						</div>

						<div class="sidebar-shortcuts-mini" id="sidebar-shortcuts-mini">
							<span class="btn btn-success"></span>

							<span class="btn btn-info"></span>

							<span class="btn btn-warning"></span>

							<span class="btn btn-danger"></span>
						</div>
					</div><!-- #sidebar-shortcuts -->

					<ul class="nav nav-list">
						<li onclick="menu_change (1);">
							<a href="#">
								<i class="icon-dashboard"></i>
								<span class="menu-text"> Dashboard </span>
							</a>
						</li>

					</ul><!-- /.nav-list -->

					<div class="sidebar-collapse" id="sidebar-collapse">
						<i class="icon-double-angle-left" data-icon1="icon-double-angle-left" data-icon2="icon-double-angle-right"></i>
					</div>

					<script type="text/javascript">
						// try{ace.settings.check('sidebar' , 'collapsed')}catch(e){}
					</script>
				</div>

				<div class="main-content" id="wiseup-data-container-main-content">
				
					<div class="breadcrumbs" id="breadcrumbs">
						<script type="text/javascript">
							try{ace.settings.check('breadcrumbs' , 'fixed')}catch(e){}
						</script>
 
						<div class="nav-search" id="nav-search">
							<form class="form-search">
								<span class="input-icon">
									<input type="text" placeholder="Search ..." class="nav-search-input" id="nav-search-input" autocomplete="off" />
									<i class="icon-search nav-search-icon"></i>
								</span>
							</form>
						</div><!-- #nav-search -->
					</div>

					<div class="page-content" id="app_content">
					</div><!-- /.page-content -->
					
				</div><!-- /.main-content -->
			</div><!-- /.main-container-inner -->

			<a href="#" id="btn-scroll-up" class="btn-scroll-up btn btn-sm btn-inverse">
				<i class="icon-double-angle-up icon-only bigger-110"></i>
			</a>
		</div><!-- /.main-container -->

		<!-- basic scripts -->

		<!--[if !IE]> -->

		<script type="text/javascript">
			window.jQuery || document.write("<script src='/websites/wiseup-dev/web/assets/js/jquery-2.0.3.min.js'>"+"<"+"/script>");
		</script>

		<!-- <![endif]-->

		<!--[if IE]>
		<script type="text/javascript">
		 window.jQuery || document.write("<script src='/websites/wiseup-dev/web/assets/js/jquery-1.10.2.min.js'>"+"<"+"/script>");
		</script>
		<![endif]-->

		<script type="text/javascript">
			if("ontouchend" in document) document.write("<script src='/websites/wiseup-dev/web/assets/js/jquery.mobile.custom.min.js'>"+"<"+"/script>");
		</script>
		<script src="/websites/wiseup-dev/web/assets/js/bootstrap.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/typeahead-bs2.min.js"></script>

		<!-- page specific plugin scripts -->

		<!--[if lte IE 8]>
		  <script src="/websites/wiseup-dev/web/assets/js/excanvas.min.js"></script>
		<![endif]-->

		<script src="/websites/wiseup-dev/web/assets/js/jquery-ui-1.10.3.full.min.js"></script>
		<!-- <script src="/websites/wiseup-dev/web/assets/js/jquery-ui-1.10.3.custom.min.js"></script> -->
		<script src="/websites/wiseup-dev/web/assets/js/jquery.ui.touch-punch.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/bootbox.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/jquery.slimscroll.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/jquery.easy-pie-chart.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/jquery.sparkline.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/flot/jquery.flot.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/flot/jquery.flot.pie.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/flot/jquery.flot.resize.min.js"></script>
		
		<script src="/websites/wiseup-dev/web/assets/js/fuelux/fuelux.wizard.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/ace-elements.js"></script>

		<!-- wiseup scripts -->		
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/sensors.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/enable_scripts.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/wiseup/messages.js"></script>
		
		<!-- ace scripts -->
		<script src="/websites/wiseup-dev/web/assets/js/uncompressed/ace.js"></script>		
		<!-- <script src="/websites/wiseup-dev/web/assets/js/date-time/bootstrap-datepicker.min.js"></script> -->
		<script src="/websites/wiseup-dev/web/assets/js/date-time/bootstrap-timepicker.min.js"></script>
		<script src="/websites/wiseup-dev/web/assets/js/date-time/date.js"></script>
		
		<!-- inline scripts related to this page -->
		<script type="text/javascript">
			jQuery(function($) {
				$("#btn_logout").click(function(){
					var endpoint_security = '/websites/wiseup-dev/web/endpoint/security.php';
					var json = new Object();
					var json_string = "";
					
					json.method = "logout";
					json_string = JSON.stringify(json);
					
					$.post(endpoint_security, json_string, function (data) {
						if (data == "TRUE") {
							window.location.href = "/websites/wiseup-dev/web";
						}
					});
				});
				
				$('#app_content').load('/websites/wiseup-dev/web/views/dashboard.html');
				
				start_favorite_sensors_service ("#wiseup-data-container-favorite-sensors");
				// start_favorite_enable_scrpts_service ("#wiseup-data-container-favorite-enable");
				// start_latest_message_service ("#wiseup-data-container-latest-messages-");
			});
			
			function menu_change (menu_id) {
				stop_all_services ();
				switch (menu_id) {
					case 1:
						$('#app_content').load('/websites/wiseup-dev/web/views/dashboard.html');
						start_favorite_sensors_service ("#wiseup-data-container-favorite-sensors");
						// start_favorite_enable_scrpts_service ("#wiseup-data-container-favorite-enable");
						break;
					case 2:
						$('#app_content').load('/websites/wiseup-dev/web/views/sensors_and_actions.html');
						start_sensors_service ("#wiseup-data-container-sensors");
						break;
					case 3:
						$('#app_content').load('/websites/wiseup-dev/web/views/places.html');
						break;
					case 4:
						$('#app_content').load('/websites/wiseup-dev/web/views/inbox.html');
						start_inbox_service ("#wiseup-data-container-inbox");
						break;
					case 5:
						$('#app_content').load('/websites/wiseup-dev/web/views/statistics.html');
						break;
					default:
						break;
				}
			}
			
			function stop_all_services () {
				end_favorite_sensors_service ();
				// end_sensors_service ();
				// end_sensor_info_service ();
			}
		</script>
		
	</body>
</html>
