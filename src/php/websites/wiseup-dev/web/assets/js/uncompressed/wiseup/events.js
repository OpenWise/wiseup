var endpoint_event = 'endpoint/event.php';
var endpoint_service = 'endpoint/service.php';

/* New event details */
var states = null;
var events = null;
var sensors = null;
var services = null;
var summary = null;
var edit_event = new edit_event_content();

/* view_wiseup_events class */
function view_wiseup_events (sensor_type, view_container) {
	this.view_container = view_container;
	this.sensor_type = sensor_type;
	this.ui_wiseup_event = 	"<div class='radio' data-rel='tooltip' data-placement='left' title='!ph_wiseup_event_description!' onclick=\"set_selected_wiseup_event (!ph_wiseup_event_id!, '!ph_wiseup_event_name!');\">" +
								"<label>" +
									"<input name='form-field-radio' type='radio' class='ace' />" +
									"<span class='lbl'> !ph_wiseup_event_name!</span>" +
								"</label>" +
							"</div>";
	this.javascript = 		"<script type='text/javascript'>" +
								"$('[data-rel=tooltip]').tooltip();" +
							"</script>";
}
view_wiseup_events.prototype.view_container = '';
view_wiseup_events.prototype.json = '';
view_wiseup_events.prototype.sensor_type = '';
view_wiseup_events.prototype.javascript = '';
view_wiseup_events.prototype.ui_wiseup_event = '';
view_wiseup_events.prototype.view_wiseup_event = '';
view_wiseup_events.prototype.get_events_list = function () {
	get_wiseup_events_list_by_sensor_type (this.sensor_type, function (data) {
		if (data != "") {
			events.json = JSON.parse(data);
			events.view_wiseup_event = events.generate_ui_step_select_event();
			$(events.view_container).html(events.view_wiseup_event);
		}
	});
}
view_wiseup_events.prototype.generate_ui_step_select_event = function () {
	var html_view = "<div class='control-group'>";
	
	if (this.json != null) {
		var wiseup_events_count = this.json.wiseup_events_info.length;
		
		for (var index = 0; index < wiseup_events_count; index++) {
			var event = this.json.wiseup_events_info[index];
			var html_ui_event = this.ui_wiseup_event;
			html_ui_event = html_ui_event.replace('!ph_wiseup_event_id!', event.event_id);
			html_ui_event = html_ui_event.replace('!ph_wiseup_event_name!', event.event_name);
			html_ui_event = html_ui_event.replace('!ph_wiseup_event_name!', event.event_name);
			html_ui_event = html_ui_event.replace('!ph_wiseup_event_description!', event.event_description);
			html_view += html_ui_event;
		}
	} 
	
	return html_view + "</div>" + this.javascript;
}

/* view_wiseup_sensors class */
function view_wiseup_sensors (sensor_type, view_container) {
	this.view_container = view_container;
	this.sensor_type = sensor_type;
	this.ui_wiseup_sensor =	"<div class='btn-group'>" +
								"<button data-toggle='dropdown' class='btn !ph_sensor_color! dropdown-toggle' " +
								"data-rel='tooltip' data-placement='top' title='!ph_sensor_type!'><i class='icon-!ph_sensor_icon!' />&nbsp;!ph_sensor_name!" +
									"<span class='icon-caret-down icon-on-right'></span>" +
								"</button>" +
								"<ul class='dropdown-menu dropdown-default'>" +
									"<li><a href='#' onclick='set_wiseup_action(1, \"On\", !ph_sensor_id!, \"!ph_sensor_name!\", \"!ph_sensor_icon!\");'><b class='green'>On</b></a></li>" +
									"<li><a href='#' onclick='set_wiseup_action(2, \"Off\", !ph_sensor_id!, \"!ph_sensor_name!\", \"!ph_sensor_icon!\");'><b class='red'>Off</b></a></li>" +
									"<li><a href='#' onclick='set_wiseup_action(0, \"Nothing\", !ph_sensor_id!, \"!ph_sensor_name!\", \"!ph_sensor_icon!\");'>Nothing</a></li>" +
								"</ul>" +
							"</div>";
	this.javascript = 		"<script type='text/javascript'>" +
								"$('[data-rel=tooltip]').tooltip();" +
							"</script>";
}
view_wiseup_sensors.prototype.view_container = '';
view_wiseup_sensors.prototype.json = '';
view_wiseup_sensors.prototype.sensor_type = '';
view_wiseup_sensors.prototype.javascript = '';
view_wiseup_sensors.prototype.ui_wiseup_sensor = '';
view_wiseup_sensors.prototype.view_wiseup_sensor = '';
view_wiseup_sensors.prototype.calculate_sensor_types = function () {
	switch (this.sensor_type) {
		case 1:
			return "";
		case 2:
			return "";
		case 3:
			return "";
		case 4:
			return "1";
		case 5:
			return "";
		case 6:
			return "";
		default:
			return "";
	}
}
view_wiseup_sensors.prototype.get_sensors_list = function () {
	var sensor_types = this.calculate_sensor_types ();
	get_sensors_list_by_sensor_types (sensor_types, function (data) {
		if (data != "NO_DATA") {
			sensors.json = JSON.parse(data);
			if (sensors.json != null) {
				sensors.view_wiseup_sensor = sensors.generate_ui_step_select_sensor();
				$(sensors.view_container).html(sensors.view_wiseup_sensor);
			}
		}
	});
}
view_wiseup_sensors.prototype.generate_ui_step_select_sensor = function () {
	var html_view = "<div class='widget-main'><p>";
	var colors = new Array("btn-white", "btn-warning tooltip-warning", "btn-success tooltip-succes", "btn-danger tooltip-error", "btn-info tooltip-info");
	var sensor_type_name = new Array("","Switch","","","","");
	var sensor_type_icon = new Array("","off","","","","");
	
	if (this.json != null) {
		var sensors_count = this.json.sensors_small_info.length;
		
		for (var index = 0; index < sensors_count; index++) {
			var sensor = this.json.sensors_small_info[index];
			var html_ui_sensor = this.ui_wiseup_sensor;
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_name!', sensor.Name);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_name!', sensor.Name);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_name!', sensor.Name);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_name!', sensor.Name);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_icon!', sensor_type_icon[sensor.SensorType]);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_icon!', sensor_type_icon[sensor.SensorType]);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_icon!', sensor_type_icon[sensor.SensorType]);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_icon!', sensor_type_icon[sensor.SensorType]);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_type!', sensor_type_name[sensor.SensorType]);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_color!', colors[0]);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_id!', sensor.SensorId);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_id!', sensor.SensorId);
			html_ui_sensor = html_ui_sensor.replace('!ph_sensor_id!', sensor.SensorId);			
			html_view += html_ui_sensor;
		}
	} 
	
	return html_view + "</p></div>" + this.javascript;
}

/* view_wiseup_services class */
function view_wiseup_services (view_container) {
	this.view_container = view_container;
	this.ui_wiseup_service =	"<div class='checkbox'> " +
									"<i class='icon-!ph_service_icon! icon-1x' />&nbsp;&nbsp;" +
									"<label>" +
										"<input onclick='set_service_flag (this, !ph_service_index!)' name='form-field-checkbox' class='ace ace-checkbox-2' type='checkbox' />" +
										"<span class='lbl'>&nbsp;!ph_service_name!</span>" +
									"</label> " +
								"</div>";
	this.javascript = 			"<script type='text/javascript'>" +
									"$('[data-rel=tooltip]').tooltip();" +
								"</script>";
}
view_wiseup_services.prototype.view_container = '';
view_wiseup_services.prototype.json = '';
view_wiseup_services.prototype.javascript = '';
view_wiseup_services.prototype.ui_wiseup_service = '';
view_wiseup_services.prototype.view_wiseup_service = '';
view_wiseup_services.prototype.get_services_list = function () {
	get_wiseup_services_list (function (data) {
		services.json = JSON.parse(data);
		services.view_wiseup_service = services.generate_ui_step_select_service();
		$(services.view_container).html(services.view_wiseup_service);
	});
}
view_wiseup_services.prototype.generate_ui_step_select_service = function () {
	var html_view = "<div class='widget-main'><p>";
	var service_type_icon = new Array("","envelope-alt blue","facebook-sign blue","cloud blue","twitter light-blue","google-plus red", "envelope blue", "comment blue");
	
	if (this.json != null) {
		var services_count = this.json.services_info.length;
		
		for (var index = 0; index < services_count; index++) {
			var service = this.json.services_info[index];
			var html_ui_service = this.ui_wiseup_service;
			html_ui_service = html_ui_service.replace('!ph_service_icon!', service_type_icon[service.service_type_id]);
			html_ui_service = html_ui_service.replace('!ph_service_name!', service.service_type_description);
			html_ui_service = html_ui_service.replace('!ph_service_index!', service.service_type_id);
			html_view += html_ui_service;
		}
	} 
	
	return html_view + "</p></div>" + this.javascript;
}

/* event_wizard_state_machine class */
function event_wizard_state_machine () {
}
event_wizard_state_machine.prototype.ui_step_select_event = function () {
	if (events.json == null) {
		events.get_events_list();
	}
}
event_wizard_state_machine.prototype.ui_step_select_service = function () {
	if (events.json == null) {
		services.get_services_list();
	}
}
event_wizard_state_machine.prototype.ui_step_select_sensor = function () {
	if (events.json == null) {
		sensors.get_sensors_list();
	}
}
event_wizard_state_machine.prototype.ui_step_finish = function () {
	summary.generate_summary_view();
}

/* wiseup_event_context class */
function wiseup_event_context (view_container) {
	this.wizard_selected_list_of_services = new Array(false, false, false, false, false, false, false, false);
	this.view_container = view_container;
	this.ui_wiseup_selected_event   =	"!ph_selected_event!";
	this.ui_wiseup_selected_sensor  = 	"!ph_wizard_selected_sensor_for_action_icon!!ph_wizard_selected_sensor_for_action_name!";
	this.ui_wiseup_selected_action  =	"<span>And</span>&nbsp;<b><span style='color:!ph_action_color!;'>!ph_selected_action!</span></b>&nbsp;action will be taken.";
	this.ui_wiseup_selected_service =	"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i class='icon-!ph_service_icon! icon-1x' />&nbsp;&nbsp;<b><span class='lbl'>&nbsp;!ph_service_name!</span></b>";
	this.javascript = 			"<script type='text/javascript'>" +
								"</script>";
}
wiseup_event_context.prototype.view_container = '';
wiseup_event_context.prototype.ui_wiseup_selected_event = '';
wiseup_event_context.prototype.ui_wiseup_selected_sensor = '';
wiseup_event_context.prototype.ui_wiseup_selected_action = '';
wiseup_event_context.prototype.ui_wiseup_selected_service = '';
wiseup_event_context.prototype.wizard_selected_wiseup_event_name = '';
wiseup_event_context.prototype.wizard_selected_wiseup_event_id = null;
wiseup_event_context.prototype.wizard_selected_wiseup_event_action_disabled = null;
wiseup_event_context.prototype.wizard_sensor_id = null;
wiseup_event_context.prototype.wizard_sensor_name = null;
wiseup_event_context.prototype.wizard_selected_sensor_for_action_id = null;
wiseup_event_context.prototype.wizard_selected_sensor_for_action_name = null;
wiseup_event_context.prototype.wizard_selected_sensor_for_action_icon = null;
wiseup_event_context.prototype.wizard_selected_action_type = null;
wiseup_event_context.prototype.wizard_selected_action_name = null;
wiseup_event_context.prototype.wizard_selected_list_of_services = null;
wiseup_event_context.prototype.generate_summary_view = function () {
	var html_view = "<div class='widget-main'><p>";
	var html_view_sensor = this.ui_wiseup_selected_sensor;
	var html_view_action = this.ui_wiseup_selected_action;
	var service_type_icon = new Array("", "envelope-alt blue", "facebook-sign blue", "cloud blue", "twitter light-blue", "google-plus red", "envelope blue", "comment blue");
	var service_type_name = new Array("", "Mail", "Facebook", "DBLog", "Twitter", "Google+", "SMS", "Notification");
	
	if (this.wizard_selected_wiseup_event_name == '') {
		html_view += this.ui_wiseup_selected_event.replace('!ph_selected_event!', "<span>You have <b>NOT selected</b></span>&nbsp;<b><span class='blue'></span></b>&nbsp;event.<hr />");
	} else {
		html_view += this.ui_wiseup_selected_event.replace('!ph_selected_event!', "<span>You have selected</span>&nbsp;<b><span class='blue'>" + this.wizard_selected_wiseup_event_name + "</span></b>&nbsp;event." + "<hr />");
	}
	
	if (this.wizard_selected_action_type != null) {
		html_view_sensor = html_view_sensor.replace('!ph_wizard_selected_sensor_for_action_icon!', "<span>Upon the the event sensor</span>&nbsp;<button class='btn btn-white btn-xs'><i class='icon-" + this.wizard_selected_sensor_for_action_icon + "' /></button>&nbsp;<b>");
		html_view_sensor = html_view_sensor.replace('!ph_wizard_selected_sensor_for_action_name!', "<span class='blue'>" + this.wizard_selected_sensor_for_action_name + "</span></b>&nbsp;will be triggered.");
		
		html_view += html_view_sensor + "<hr />";
		html_view_action = html_view_action.replace('!ph_selected_action!', this.wizard_selected_action_name);
		
		switch (this.wizard_selected_action_type) {
			case 0: html_view_action = html_view_action.replace('!ph_action_color!', "black");
				break;
			case 1: html_view_action = html_view_action.replace('!ph_action_color!', "green");
				break;
			case 2: html_view_action = html_view_action.replace('!ph_action_color!', "red");
				break;
		}
		
		html_view_action += "<hr />";
	} else {
		html_view_sensor = html_view_sensor.replace('!ph_wizard_selected_sensor_for_action_icon!', "<span>You <b>have not</b> selected sensor and action for this event.</span>");
		html_view_sensor = html_view_sensor.replace('!ph_wizard_selected_sensor_for_action_name!', "");
		html_view_action = "<span></span>";
		html_view += html_view_sensor + "<hr />";
	}
	
	html_view += html_view_action;
	html_view += "<span>These services will be applied to the event.</span><br />";
	
	for (var index = 0; index < this.wizard_selected_list_of_services.length; index++) {
		if (this.wizard_selected_list_of_services[index] == true) {
			var html_view_service = this.ui_wiseup_selected_service;
			html_view_service = html_view_service.replace('!ph_service_icon!', service_type_icon[index]);
			html_view_service = html_view_service.replace('!ph_service_name!', service_type_name[index]);
			html_view += html_view_service + "<br />";
		}
	} html_view += "<hr />";
	
	html_view += "</p></div>" + this.javascript;
	$(this.view_container).html(html_view);
}

function edit_event_content () {

}
edit_event_content.prototype.event_id = null;
edit_event_content.prototype.selected_services = null;
edit_event_content.prototype.selected_action_type = null;

function event_step_change (info) {
	var step = 0;
	
	if (info.direction == "next") {
		step = info.step + 1;
	} else if (info.direction == "previous") {
		step = info.step - 1;
	}
	
	switch (step) {
		case 1:
			states.ui_step_select_event ();
			break;
		case 2:
			states.ui_step_select_sensor ();
			break;
		case 3:
			states.ui_step_select_service ();
			break;
		case 4:
			states.ui_step_finish ();
			break;
		default:
			break;
	}
}

function event_step_finished (info) {
	var is_event_valid = true;
	var is_event_selected = false;
	var is_action_selected = false;
	var is_service_selected = false;
	
	// 1) Check for selected event.
	if (summary.wizard_selected_wiseup_event_id != null) {
		is_event_selected = true;
	}
	
	// 2) Check for selected action.
	if (summary.wizard_selected_action_type == null || summary.wizard_selected_action_type == 0) {
		summary.wizard_selected_sensor_for_action_id = 0;
		summary.wizard_selected_action_type = 0;
	} else {
		is_action_selected = true;
	}
	
	// 3) Generate list of services.
	var service_list = "";
	for (var i = 0; i < summary.wizard_selected_list_of_services.length; i++) {
		if (summary.wizard_selected_list_of_services[i] == true) {
			service_list += (i) + ",";
		}
	}
	
	// 3.1) Check for selected services.
	if (service_list != "") {
		is_service_selected = true;
	}
	
	if (is_action_selected == false && is_service_selected == false) {
		is_event_valid = false;
		alert ("This event don't have action and no service. Please use one of them otherwise this event is useless.");
		return;
	} 
	
	if (is_event_selected == false) {
		is_event_valid = false;
		alert ("You have not selected event. Please use one of the evnts on the first step.");
		return;
	}
	
	if (is_event_valid == true) {
		create_wiseup_event (	summary.wizard_selected_sensor_for_action_id, 
								summary.wizard_selected_action_type, 
								summary.wizard_sensor_id, 
								summary.wizard_sensor_name + " - " + summary.wizard_selected_wiseup_event_name, 
								summary.wizard_selected_wiseup_event_id, 
								service_list,
								function (data) {
			if (data == "create_wiseup_event_success") {
				$("#modal-wizard-event").modal('hide');
				sensor_settings_click_handle (this, summary.wizard_sensor_id);
			} else {
				alert ("Couldn't create even, please try again.");
			}
		});
	}
}

function set_selected_wiseup_event (id, name) {
	summary.wizard_selected_wiseup_event_id = id;
	summary.wizard_selected_wiseup_event_name = name;
	$("#selected_wiseup_event").html(name);
}

/*function set_dis_en_action (obj, view_container) {
	if (obj.checked == true) {
		summary.wizard_selected_wiseup_event_action_disabled = true;
		$(view_container).html("");
	} else if (obj.checked == false) {
		summary.wizard_selected_wiseup_event_action_disabled = false;
		sensors.get_sensors_list();
	}
}*/

function set_service_flag (obj, index) {
	summary.wizard_selected_list_of_services[index] = obj.checked;
}

function set_wiseup_action (action_type, action_name, sensor_id, sensor_name, sensor_icon) {
	summary.wizard_selected_action_type = action_type;
	summary.wizard_selected_action_name = action_name;
	summary.wizard_selected_sensor_for_action_id = sensor_id;
	summary.wizard_selected_sensor_for_action_icon = sensor_icon;
	summary.wizard_selected_sensor_for_action_name = sensor_name;
	$("#selected_wiseup_action").html(action_name);
	$("#selected_wiseup_sensor").html(sensor_name);
	
	switch (action_type) {
		case 0:
			$("#selected_wiseup_action").css({ 'color': 'black' });
			break;
		case 1:
			$("#selected_wiseup_action").css({ 'color': 'green' });
			break;
		case 2:
			$("#selected_wiseup_action").css({ 'color': 'red' });
			break;
	}
}

/* UI METHODS */
function get_events_container_view (view_container, sensor_id) {
	get_events_list_by_sensor_id (sensor_id, function (data) {
		var service_type_icon = new Array("", "envelope-alt blue", "facebook-sign blue", "cloud blue", "twitter light-blue", "google-plus red", "envelope blue", "comment blue");
		var service_type_name = new Array("", "Mail", "Facebook", "DBLog", "Twitter", "Google+", "SMS", "Notification");
		var container = "";
		var javascript = 	"<script type='text/javascript'>" +
								"$('[data-rel=tooltip]').tooltip();" +
							"</script>";
		
		if (data != "NO_DATA") {
			var json = JSON.parse(data);
			var events_count = json.sensor_events_info.length;
			for (var index = 0; index < events_count; index++) {
				var event_html_row = view_event_row;
				var event = json.sensor_events_info[index];
				var action_sensor = json.sensor_action_sensor_info[index];
				var event_name = "<span onclick=\"onclick_event_handle(" + event.general_event_id + ");\">" + event.event_name + "</span>";
				event_html_row = event_html_row.replace('!ph_event_name!', event_name);
				event_html_row = event_html_row.replace('!ph_event_enable_script_id!', event.enable_script_id);
				//commented by sharon - is this by accident? - event_html_row = event_html_row.replace('!ph_event_enable_script_id!', event.enable_script_id);
				
				/* ** SAMMARY ** */
				var html_icon_sammary = "";
				switch (event.general_action_type) {
					case '0': // NOTING
					break;
					case '1': // ON
						html_icon_sammary += "&nbsp;<i class='icon-off green' data-rel='tooltip' data-placement='top' title='" + action_sensor.Name + " (ON)' />&nbsp;";
					break;
					case '2': // OFF
						html_icon_sammary += "&nbsp;<i class='icon-off red' data-rel='tooltip' data-placement='top' title='" + action_sensor.Name + " (OFF)' />&nbsp;";
					break;
					default:
					break;
				}
				
				var services = json.services_info[index];
				for (var service_index = 0; service_index < services.length; service_index++) {
					var service = services[service_index];
					html_icon_sammary += "&nbsp;<i class='icon-" + service_type_icon[service.service_type_id] + "' data-rel='tooltip' data-placement='top' title='" + service_type_name[service.service_type_id] + "' />&nbsp;";
				}
				
				event_html_row = event_html_row.replace('!ph_event_summary_icon_features!', html_icon_sammary);
				/* **** */
				
				if (event.is_enabled == 1) {
					event_html_row = event_html_row.replace('!ph_event_enabled!', "checked=''");
					event_html_row = event_html_row.replace('!ph_event_state!', '0');
				} else {
					event_html_row = event_html_row.replace('!ph_event_enabled!', "");
					event_html_row = event_html_row.replace('!ph_event_state!', '1');
				}
				
				if (event.is_favorite_enable_script == 1) {
					event_html_row = event_html_row.replace('!ph_event_enable_script_favorite!', "checked=''");
					event_html_row = event_html_row.replace('!ph_event_enable_script_favorite_value!', '0');
				} else {
					event_html_row = event_html_row.replace('!ph_event_enable_script_favorite!', "");
					event_html_row = event_html_row.replace('!ph_event_enable_script_favorite_value!', '1');
				}
				
				container += event_html_row + javascript;
			}
		}
		
		$(view_container).html(container);
	});
}

function onclick_event_handle (event_id) {
	get_event_info_by_event_id (event_id, function (data) {
		var json = JSON.parse(data);
		if (json != null) {
			edit_event.selected_services = new Array(false, false, false, false, false, false, false, false);
			edit_event.event_id = event_id;
			var actions_list = new Array ("Nothing", "On", "Off");
			var actions_color = new Array ("black", "green", "red");
			var service_type_icon = new Array("","envelope-alt blue","facebook-sign blue","cloud blue","twitter light-blue","google-plus red", "envelope blue", "comment blue");
			var action_row = 			"<option value='!ph_action_value!' class='!ph_color!' !ph_is_selected!><b>!ph_action_name!</b></option>";
			var service_row = 			"<div class='checkbox'> " +
											"<i class='icon-!ph_service_icon! icon-1x' />&nbsp;&nbsp;" +
											"<label>" +
												"<input onclick='edit_event_service_onclick(this, !ph_service_index!)' name='form-field-checkbox' class='ace ace-checkbox-2' type='checkbox' !ph_is_service_used! />" +
												"<span class='lbl'>&nbsp;!ph_service_name!</span>" +
											"</label> " +
										"</div>";
			var edit_event_content = 	"<div class='widget-main'><p>" +
											"<h3 class='orange'>!ph_sensor_name!</h3>" +
											"<h7 class='blue'>!ph_event_type_name!</h7><br /><br />" +
											"<span>Please select the new action</span>" +
											"<select onchange=\"edit_event_action_onchange(this);\" class='form-control'>" +
												"!ph_actions!" +
											"</select><br />" +
											"<span>Please select new sevices</span>" +
											"!ph_service_list!";
										"</p></div>";
			
			// List all available services.
			var services_count = json.services_info.length;
			var event_services_count = json.event_service_info.length;
			var html_services = "";
			for (var index = 0; index < services_count; index++) {
				var service = json.services_info[index];
				var html_ui_service = service_row;
				
				// Does this service used by event? TODO - Create separate method for this code
				var is_service_used = false;
				for (var i = 0; i < event_services_count; i++) {
					var event_service = json.event_service_info[i];
					if (event_service.service_type_id == service.service_type_id) {
						is_service_used = true;
					}
				}
				
				if (is_service_used == true) {
					html_ui_service = html_ui_service.replace('!ph_is_service_used!', "checked='true'");
					edit_event.selected_services[service.service_type_id] = true;
				} else {
					html_ui_service = html_ui_service.replace('!ph_is_service_used!', '');
				}
				
				html_ui_service = html_ui_service.replace('!ph_service_icon!', service_type_icon[service.service_type_id]);
				html_ui_service = html_ui_service.replace('!ph_service_name!', service.service_type_description);
				html_ui_service = html_ui_service.replace('!ph_service_index!', service.service_type_id);
				html_services += html_ui_service;
			}
			
			// Set events details.
			var event = json.event_info[0];
			edit_event_content = edit_event_content.replace('!ph_sensor_name!', event.Name);
			edit_event_content = edit_event_content.replace('!ph_event_type_name!', event.event_name);
			edit_event_content = edit_event_content.replace('!ph_service_list!', html_services);
			
			var html_ui_actions = "";
			for (var index = 0; index < actions_list.length; index++) {
				var html_ui_action = action_row;
				html_ui_action = html_ui_action.replace('!ph_action_value!', index);
				html_ui_action = html_ui_action.replace('!ph_action_value!', index);
				html_ui_action = html_ui_action.replace('!ph_color!', actions_color[index]);
				html_ui_action = html_ui_action.replace('!ph_action_name!', actions_list[index]);
				
				if (index == event.general_action_type) {
					edit_event.selected_action_type = index;
					html_ui_action = html_ui_action.replace('!ph_is_selected!', "selected='true'");
				} else {
					html_ui_action = html_ui_action.replace('!ph_is_selected!', "");
				}
				
				html_ui_actions += html_ui_action;
			}
			edit_event_content = edit_event_content.replace('!ph_actions!', html_ui_actions);
			
			bootbox.dialog({
				message: edit_event_content,
				buttons: 			
				{
					"success" :
					 {
						"label" : "<i class='icon-ok'></i> Save",
						"className" : "btn-sm btn-success",
						"callback": function() {
							// 1) Get selected services.
							var selected_services = "";
							for (var index = 1; index <= edit_event.selected_services.length; index++) {
								if (edit_event.selected_services[index] == true) {
									selected_services += '1,';
								} else {
									selected_services += '0,';
								}
							}
							
							update_wiseup_event (edit_event.event_id,
												edit_event.selected_action_type,
												selected_services,
												function (data) {
													// alert (data);
												});
						}
					},
					"danger" :
					{
						"label" : "Cancel",
						"className" : "btn-sm btn-danger",
						"callback": function() {
							edit_event.event_id = null;
							edit_event.selected_services = null;
							edit_event.selected_action_type = null;
						}
					}
				}
			});
		}
	});
}

function edit_event_action_onchange (obj) {
	var selectedValue = obj.options[obj.selectedIndex].value;
	edit_event.selected_action_type = selectedValue;
}

function edit_event_service_onclick (obj, service_type_id) {
	edit_event.selected_services[service_type_id] = obj.checked;
	/*var selected_services = "";
	for (var index = 1; index < edit_event.selected_services.length; index++) {
		if (edit_event.selected_services[index] == true) {
			selected_services += '1,';
		} else {
			selected_services += '0,';
		}
	}
	alert (selected_services);*/
}

function wizard_clean_up (view_container) {
	$(view_container + "-step1").html("");
	$(view_container + "-step2").html("");
	$(view_container + "-step3").html("");
	$(view_container + "-step4").html("");
	
	events.json = "";
	sensors.json = "";
	services.json = "";
}

function trigger_event_wizard (sensor_id, sensor_name, sensor_type, view_container) {
	states = new event_wizard_state_machine ();
	events = new view_wiseup_events (sensor_type, view_container + "-step1");
	sensors = new view_wiseup_sensors (sensor_type, view_container + "-step2");
	services = new view_wiseup_services (view_container + "-step3");
	summary = new wiseup_event_context (view_container + "-step4");
		
	wizard_clean_up (view_container);
	// TODO - summary cleanup.
	summary.wizard_sensor_id = sensor_id;
	summary.wizard_sensor_name = sensor_name;
	
	events.get_events_list();
	sensors.get_sensors_list();
	services.get_services_list();
}
	
/* SELECT METHODS */
function get_events_list_by_sensor_id (sensor_id, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_event_list_by_sensor_id";
	json.sensor_id = sensor_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}

function get_wiseup_events_list_by_sensor_type (sensor_type, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_wiseup_events_list_by_sensor_type";
	json.sensor_type = sensor_type;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}

function get_sensors_list_by_sensor_types (sensor_types, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_sensors_by_list_type";
	json.sensor_types = sensor_types;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}

function get_wiseup_services_list (fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_enabled_services_list";
	json_string = JSON.stringify(json);
	
	$.post(endpoint_service, json_string, fn_ptr);
}

function get_event_info_by_event_id (event_id, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_event_info";
	json.event_id = event_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}

/* UPDATE METHODS */
function create_wiseup_event (action_sensor_id, action_type, sensor_id, event_name, wiseup_event_type, service_list, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "create_wiseup_event";
	json.action_sensor_id = action_sensor_id;
	json.action_type = action_type;
	json.sensor_id = sensor_id;
	json.event_name = event_name;
	json.wiseup_event_type = wiseup_event_type;
	json.service_type_list = service_list;	
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}

function update_wiseup_event (event_id, selected_action_type_id, selected_service_list, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "update_wiseup_event";
	json.event_id = event_id;
	json.selected_action_type_id = selected_action_type_id;
	json.selected_service_list = selected_service_list;	
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}