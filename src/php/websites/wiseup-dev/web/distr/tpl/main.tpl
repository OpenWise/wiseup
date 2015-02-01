<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title>WiseUp - Dashboard</title>

		<meta name="description" content="overview &amp; stats" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />

		<!-- basic styles -->
        <link rel="shortcut icon" href="/favicon.ico"/>
		<link href="assets/css/bootstrap.min.css" rel="stylesheet" />
		<link rel="stylesheet" href="/assets/css/font-awesome.min.css" />

		<!--[if IE 7]>
		  <link rel="stylesheet" href="/assets/css/font-awesome-ie7.min.css" />
		<![endif]-->

		<!-- page specific plugin styles -->
		<link rel="stylesheet" href="assets/css/jquery-ui-1.10.3.full.min.css" />
		<!-- fonts -->

		<link rel="stylesheet" href="/assets/css/ace-fonts.css" />

		<!-- ace styles -->

		<link rel="stylesheet" href="/assets/css/uncompressed/ace.css" />
		<link rel="stylesheet" href="/assets/css/ace-rtl.min.css" />
		<link rel="stylesheet" href="/assets/css/ace-skins.min.css" />
		<link rel="stylesheet" href="assets/css/datepicker.css" /> 
		<link rel='stylesheet' href='/assets/css/bootstrap-timepicker.css' />		
        <link rel="stylesheet" href="/assets/css/wiseup.css" />

		<!--[if lte IE 8]>
		  <link rel="stylesheet" href="/assets/css/ace-ie.min.css" />
		<![endif]-->

		<!-- inline styles related to this page -->

		<!-- ace settings handler -->

		<script src="/assets/js/uncompressed/ace-extra.js"></script>		
		
		<!-- WISEUP AREA -->
		<script src="/assets/js/uncompressed/wiseup/views.js"></script>
		<script src="/assets/js/uncompressed/wiseup/general.js"></script>
		<script src="/assets/js/uncompressed/wiseup/events.js"></script>
		<script src="/assets/js/uncompressed/wiseup/timers.js"></script>
		<script src="/assets/js/uncompressed/wiseup/graphs.js"></script>
		<script src="/assets/js/uncompressed/wiseup/inbox.js"></script>
		

		<!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->

		<!--[if lt IE 9]>
		<script src="/assets/js/html5shiv.js"></script>
		<script src="/assets/js/respond.min.js"></script>
		<![endif]-->
	</head>

	<body>
		<div id="modal_window_edit_event" class="hide">
		</div>
		<div id="modal_window_general_purpose" class="hide">
		</div>
		<div class="navbar navbar-default" id="navbar">
			<script type="text/javascript">
				// try{ace.settings.check('navbar' , 'fixed')}catch(e){}
			</script>

			<div class="navbar-container" id="navbar-container">
				<div class="navbar-header pull-left">
					<a href="#" class="navbar-brand">
						<img src="/assets/img/logo_clear.png" alt="WiseUp" height="25" /> <!-- Home Never Been Smarter --> 
					</a><!-- /.brand -->
				</div><!-- /.navbar-header -->

				<div class="navbar-header pull-right" role="navigation">
					<ul class="nav ace-nav">
						
						<li id = 'wiseup-data-container-latest-messages-alerts' class="purple">
							<a data-toggle="dropdown" class="dropdown-toggle" href="#">
								<i id="wiseup-data-container-latest-messages-alerts-icon-animation" class="icon-bell-alt icon-animated-bell"></i>
								<span id='wiseup-data-container-latest-messages-alerts-counter' class="badge badge-important"></span>
							</a>

							<ul class="pull-right dropdown-navbar navbar-pink dropdown-menu dropdown-caret dropdown-close">
						
							</ul>
						</li>

						<li id = 'wiseup-data-container-latest-messages-notifications' class="green">
							<a data-toggle="dropdown" class="dropdown-toggle" href="#">
								<i id= "wiseup-data-container-latest-messages-notifications-icon-animation" class="icon-envelope icon-animated-vertical"></i>
								<span id='wiseup-data-container-latest-messages-notifications-counter' class="badge badge-success"></span>
							</a>
							
							<ul class="pull-right dropdown-navbar navbar-green dropdown-menu dropdown-caret dropdown-close">
							
							</ul>
						</li>

						<li class="light-blue">
							<a data-toggle="dropdown" href="#" class="dropdown-toggle">
								<img class="nav-user-photo" src="assets/avatars/user.jpg" />
								<span class="user-info">
									<small>Wellcome</small>
										<?=$_SESSION['user']?>
								</span>

								<i class="icon-caret-down"></i>
							</a>

							<ul class="user-menu pull-right dropdown-menu dropdown-yellow dropdown-caret dropdown-close">
								<li>
									<a href="#">
										<i class="icon-cog"></i>
											Settings
									</a>
								</li>

								<li>
									<a href="#">
										<i class="icon-user"></i>
											Profile
									</a>
								</li>

								<li class="divider"></li>

								<li>
									<a href="/authorization/?logout=1">
										<i class="icon-off"></i>
											Logout
									</a>
								</li>
							</ul>
						</li>
					</ul><!-- /.ace-nav -->
				</div><!-- /.navbar-header -->
			</div><!-- /.container -->
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
						<li <?php print $class = (url1 == '') ? 'class="active"' : "";?>>
							<a href="/" class="dropdown-toggle">
								<i class="icon-dashboard"></i>
								<span class="menu-text"> Dashboard </span>
							</a>
						</li>

					    <li <?php print $class = (url1 == 'actions') ? 'class="active"' : "";?>>
							<a href="/actions" class="dropdown-toggle">
								<i class="icon-lightbulb"></i>
								<span class="menu-text"> Sensors & Actions </span>
							</a>
						</li>
						
						 <li <?php print $class = (url1 == 'places') ? 'class="active"' : "";?>>
							<a href="/places" class="dropdown-toggle">
								<i class="icon-globe"></i>
								<span class="menu-text"> Places </span>
							</a>
						</li>

						<li <?php print $class = (url1 == 'inbox') ? 'class="active"' : "";?>>
							<a href="/inbox" class="dropdown-toggle">						
								<i class="icon-envelope-alt"></i>
								<span class="menu-text"> Inbox </span>
							</a>
						</li>
						
						<li <?php print $class = (url1 == 'statistics') ? 'class="active"' : "";?>>
							<a href="/statistics" class="dropdown-toggle">
								<i class="icon-desktop"></i>
								<span class="menu-text"> Statistics </span>
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
 
						<ul class="breadcrumb">
							<li>
								<i class="icon-home home-icon"></i>
								<a href="#">Home</a>
							</li>
							<li class="active">Dashboard</li>
						</ul><!-- .breadcrumb -->
 
						<div class="nav-search" id="nav-search">
							<form class="form-search">
								<span class="input-icon">
									<input type="text" placeholder="Search ..." class="nav-search-input" id="nav-search-input" autocomplete="off" />
									<i class="icon-search nav-search-icon"></i>
								</span>
							</form>
						</div><!-- #nav-search -->

					</div>

					<div class="page-content">
					<?=$content?>
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
			window.jQuery || document.write("<script src='/assets/js/jquery-2.0.3.min.js'>"+"<"+"/script>");
		</script>

		<!-- <![endif]-->

		<!--[if IE]>
<script type="text/javascript">
 window.jQuery || document.write("<script src='/assets/js/jquery-1.10.2.min.js'>"+"<"+"/script>");
</script>
<![endif]-->

		<script type="text/javascript">
			if("ontouchend" in document) document.write("<script src='/assets/js/jquery.mobile.custom.min.js'>"+"<"+"/script>");
		</script>
		<script src="/assets/js/bootstrap.min.js"></script>
		<script src="/assets/js/typeahead-bs2.min.js"></script>

		<!-- page specific plugin scripts -->

		<!--[if lte IE 8]>
		  <script src="/assets/js/excanvas.min.js"></script>
		<![endif]-->

		<script src="assets/js/jquery-ui-1.10.3.full.min.js"></script>
		<!-- <script src="/assets/js/jquery-ui-1.10.3.custom.min.js"></script> -->
		<script src="/assets/js/jquery.ui.touch-punch.min.js"></script>
		<script src="assets/js/bootbox.min.js"></script>
		<script src="/assets/js/jquery.slimscroll.min.js"></script>
		<script src="/assets/js/jquery.easy-pie-chart.min.js"></script>
		<script src="/assets/js/jquery.sparkline.min.js"></script>
		<script src="/assets/js/flot/jquery.flot.min.js"></script>
		<script src="/assets/js/flot/jquery.flot.pie.min.js"></script>
		<script src="/assets/js/flot/jquery.flot.resize.min.js"></script>
		
		<script src="assets/js/fuelux/fuelux.wizard.min.js"></script>
		<script src="/assets/js/uncompressed/ace-elements.js"></script>

		<!-- wiseup scripts -->		
		<script src="/assets/js/uncompressed/wiseup/sensors.js"></script>
		<script src="/assets/js/uncompressed/wiseup/enable_scripts.js"></script>
		<script src="/assets/js/uncompressed/wiseup/messages.js"></script>
		
		<!-- ace scripts -->
		<script src="/assets/js/uncompressed/ace.js"></script>		
		<!-- <script src="assets/js/date-time/bootstrap-datepicker.min.js"></script> -->
		<script src='/assets/js/date-time/bootstrap-timepicker.min.js'></script>
		<script src='/assets/js/date-time/date.js'></script>
		
		<!-- inline scripts related to this page -->
		<script type="text/javascript">
			jQuery(function($) {
				<?=$js_scripts?>
			});
		</script>
		
	</body>
</html>
