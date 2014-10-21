var interval_id_get_favorite_sensors = -1;
var interval_id_get_sensors = -1;

/* VIEWS ID */
// modified by sharon
//var view_main_container = "#wiseup-data-container-main-content";
var view_main_container = "#app_content";


/* VIEWS */
var view_favorite_sensors =	"<div class='row'>" +
								"<div class='col-xs-12'>" +
									"<div class='col-sm-6'>" +
										"<div class='widget-box transparent '>" +
											"<div class='widget-header widget-header-flat'>" +
												"<h4 class='lighter'>" +
												"<i class='icon-star wiseup-favorite-icon'></i>Favorite Sensors</h4>" +
											"</div>" +
											"<div class='widget-body'>" +
												"<div class='widget-main no-padding'>" +
													"<br />" +
													"<div class='infobox-container' id='wiseup-data-container-favorite-sensors'></div>" +
												"</div>" +
											"</div>" +
										"</div>" +
										"<div class='widget-box transparent '>" +
											"<div class='widget-header widget-header-flat'>" +
												"<h4 class='lighter'>" +
												"<i class='icon-star wiseup-favorite-icon'></i>Favorite Enable/Disable Features</h4>" +
											"</div>" +
											"<div class='widget-body'>" +
												"<div class='widget-main no-padding'>" +
													"<br />" +
													"<div class='infobox-container' id='wiseup-data-container-favorite-enable'></div>" +
												"</div>" +
											"</div>" +
										"</div>" +
									"</div>" +
									"<div class='col-sm-6 infobox-container' id='wiseup-data-container-foreign-sensors'></div>" +
								"</div>" +
							"</div>";
var view_sensors = "<div class='page-header'><h1 id='lbl-selected-menu'>  Favorite </h1></div><div class='row'><div class='col-xs-12'><div class='col-sm-12 infobox-container' id='wiseup-data-container-sensors'></div><div class='col-sm-12 infobox-container' id='wiseup-data-container-foreign-sensors'></div></div></div>";
var view_dashboard = 	"<div class='breadcrumbs' id='breadcrumbs'>" + 
							"<script type='text/javascript'>" +
								"try{ace.settings.check('breadcrumbs' , 'fixed')}catch(e){}" +
							"</script>" +
							"<ul class='breadcrumb'><li><i class='icon-home home-icon'></i><a href='#'>Home</a></li><li class='active'>Dashboard</li></ul>" +
							"<div class='nav-search' id='nav-search'><form class='form-search'><span class='input-icon'><input type='text' placeholder='Search ...' class='nav-search-input' id='nav-search-input' autocomplete='off' /><i class='icon-search nav-search-icon'></i></span></form></div>" +
						"</div>" +
						"<div class='page-content'>" + view_favorite_sensors + "</div>";
var view_sensors_and_actions = "<div class='breadcrumbs' id='breadcrumbs'><script type='text/javascript'>try{ace.settings.check('breadcrumbs' , 'fixed')}catch(e){}</script><ul class='breadcrumb'><li><i class='icon-home home-icon'></i><a href='#'>Home</a></li><li class='active'>Sensors & Actions</li></ul><div class='nav-search' id='nav-search'><form class='form-search'><span class='input-icon'><input type='text' placeholder='Search ...' class='nav-search-input' id='nav-search-input' autocomplete='off' /><i class='icon-search nav-search-icon'></i></span></form></div></div><div class='page-content'>" + view_sensors + "</div>";

/* MENU STATE */
var menu_states = { "DASHBOARD": 1, "SENSORS_AND_ACTIONS": 2, "PLACES": 3, "INBOX": 4,  "STATISTICS": 5}
var current_menu_state = 0;

// Method for updating current active menu. (ie. menu_states["DASHBOARD"])
function update_menu_state (state) {
	current_menu_state = state;
}

// Method for injecting HTML view into main data container (#wiseup-data-container-main-content)
function set_view_main (view) {
	$(view_main_container).html(view);
}