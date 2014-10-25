var endpoint_sensor = 'endpoint/sensor.php';
var request_interval = 2000;

var interval_id_get_favorite_sensors = -1;
var interval_id_get_sensors = -1;
var interval_id_get_sensor_info = -1;

/* ui_builder_sensor class */
function ui_builder_sensor (sensor) { 
	this.sensor = sensor;
}
ui_builder_sensor.prototype.sensor = '';
ui_builder_sensor.prototype.ui_router_sensor = function () {
	switch(this.sensor.sensor_family_type) {
		case '1': // temperature
			return this.ui_temperature_object ();
		case '2': // ui_luminance_object
			return this.ui_luminance_object ();
        case '3': // motion
			return this.ui_movement_object ();
		case '4': // relay
			return this.ui_switch_object ();
	}
};
ui_builder_sensor.prototype.ui_temperature_object = function () {
	var html = ui_temperature.replace('!ph_sensor_web_value!', this.sensor.value);
	html = html.replace('!ph_sensor_name!',this.sensor.sensor_name);
	html = html.replace('!ph_sensor_footer!', this.ui_sensor_footer_object (this.sensor));
	
	return html;
};
ui_builder_sensor.prototype.ui_luminance_object = function () {
	var html = ui_luminance.replace('!ph_sensor_web_value!', this.sensor.value);
	html = html.replace('!ph_sensor_name!',this.sensor.sensor_name);
	html = html.replace('!ph_sensor_footer!', this.ui_sensor_footer_object (this.sensor));
	
	return html;
};
ui_builder_sensor.prototype.ui_movement_object = function () {
	var html = ui_movement.replace('!ph_sensor_name!',this.sensor.sensor_name);
	html = html.replace('!ph_sensor_footer!', this.ui_sensor_footer_object (this.sensor));
	
    if (this.sensor.value == 1) {
		html = html.replace('!ph_sensor_web_value!', "Move");
		html = html.replace('!ph_sensor_icon!', "<i class='icon-male wiseup-gray-icon' style='color:#0083C9;'></i>");
    } else {
		html = html.replace('!ph_sensor_web_value!', "Idle");
		html = html.replace('!ph_sensor_icon!', "<i class='icon-male wiseup-gray-icon'></i>");
    }
	
	return html;
};
ui_builder_sensor.prototype.ui_switch_object = function () {
	var html = ui_switch.replace('!ph_sensor_name!',this.sensor.sensor_name);
	html = html.replace('!ph_sensor_footer!', this.ui_sensor_footer_object (this.sensor));
	var text = "";
	
	if (this.sensor.action == 0 && this.sensor.value == 0) {
		html = html.replace('!ph_sensor_web_value!', "Off");
	} else if (this.sensor.action == 1 && this.sensor.value == 1) {
		html = html.replace('!ph_sensor_web_value!', "On");
	} else if (this.sensor.action == 1 && this.sensor.value == 0) {
		html = html.replace('!ph_sensor_web_value!', "On <i class='icon-spinner icon-spin orange bigger-125'></i>");
	} else if (this.sensor.action == 0 && this.sensor.value == 1) {
		html = html.replace('!ph_sensor_web_value!', "Off <i class='icon-spinner icon-spin orange bigger-125'></i>");
	}
	
	// onmouseover='this.style.color=\"#0083C9\";' onmouseout='this.style.color=\"#C3C3C3\";'
    if (this.sensor.value == 1) {
		html = html.replace('!ph_sensor_icon!', "<i class='icon-off wiseup-gray-icon' style='cursor:pointer;color:" + get_switch_color_by_values (this.sensor) + ";' onclick='this.style.color=\"#D15B47\";switch_type_click_handle(this, " + this.sensor.sensor_id + ",0);'></i>");
    } else {
		html = html.replace('!ph_sensor_icon!', "<i class='icon-off wiseup-gray-icon' style='cursor:pointer;color:" + get_switch_color_by_values (this.sensor) + ";' onclick='this.style.color=\"#87B87F\";switch_type_click_handle(this, " + this.sensor.sensor_id + ",1);'></i>");
    }
    
	return html;
};
ui_builder_sensor.prototype.ui_magnet_object = function () {
	var html = ui_movement.replace('!ph_sensor_name!',this.sensor.Name);
	html = html.replace('!ph_sensor_footer!', this.ui_sensor_footer_object (this.sensor));
	
    if (this.sensor.SensorWebValue == 1) {
		html = html.replace('!ph_sensor_web_value!', "Open");
		html = html.replace('!ph_sensor_icon!', "<i class='icon-unlock wiseup-gray-icon' style='color:#0083C9;'></i>");
    } else {
		html = html.replace('!ph_sensor_web_value!', "Close");
		html = html.replace('!ph_sensor_icon!', "<i class='icon-lock wiseup-gray-icon'></i>");
    }
	
	return html;
};
ui_builder_sensor.prototype.ui_sensor_footer_object = function () {
	var action = "";
	
	/*if (this.sensor.Favourite == 1) {
		action = "onclick='this.style.color=\"#ABBAC3\"; set_sensor_favorite(" + this.sensor.SensorId + ", 0, function () { });'";
	} else {
		action = "onclick='this.style.color=\"#F79263\"; set_sensor_favorite(" + this.sensor.SensorId + ", 1, function () { });'";
	}*/
	
	return	"<div class='wiseup-sensor-settings'>" +
				"<i class='icon-star wiseup-favorite-icon' style='color:" + get_favorite_icon_color (this.sensor) + ";cursor:pointer;' " + action + "></i>" +
			"</div>" +
			"<div class='wiseup-sensor-favorite'>" +
				// "<i class='icon-cog wiseup-settings-icon' style='cursor:pointer;' onclick='this.style.color=\"#87B87F\"; sensor_settings_click_handle(this, " + this.sensor.SensorId + ");'></i>" +
				"<i class='icon-cog wiseup-settings-icon' style='cursor:pointer;' onclick='this.style.color=\"#87B87F\";'></i>" +
			"</div>";
};

function start_favorite_sensors_service (view_container) {
	interval_id_get_favorite_sensors = setInterval(function() { ui_binder_favorite_sensors(view_container) }, request_interval);
	ui_binder_favorite_sensors (view_container);
}

function end_favorite_sensors_service () {
	clearInterval(interval_id_get_favorite_sensors);
}

function generate_view_sensor_from_data (data) {
	var json = JSON.parse(data);
	// alert(data);
	var container = "<div class='infobox-container'>";
	if (json != null) {
		var sensors_count = json.sensor_info.length;
		
		for (var index = 0; index < sensors_count; index++) {
			var sensor = json.sensor_info[index];
			var sensor_builder = new ui_builder_sensor (sensor);
			container += sensor_builder.ui_router_sensor ();
		}
	} 

	return container + "</div>";
}

function ui_binder_favorite_sensors (view_container) {
	get_all_favorite_sensors (function (data) {
		$(view_container).html(generate_view_sensor_from_data (data));
	});
}

function ui_wrapper_place (data_view, place_name, place_description) {
	var html = "<div class='col-sm-6'><div class='widget-box'><div class='widget-header'><h4>" + place_name + "</h4></div><div class='widget-body'>";
	html += "<div class='widget-main'><div class='infobox-container'>";
	html += data_view;
	html += "</div></div></div></div></div>";
	
	return html;
}

function get_favorite_icon_color (sensor) {
	if (sensor.Favourite == '1') {
		return "#F79263";
	} else {
		return "#ABBAC3";
	}
}

function get_switch_color_by_values (sensor) {
	if (sensor.action == 0 && sensor.value == 0) {
		return "#C3C3C3";
	} else if (sensor.action == 1 && sensor.value == 1) {
		return "#0083C9";
	} else if (sensor.action == 1 && sensor.value == 0) {
		return "#87B87F";
	} else if (sensor.action == 0 && sensor.value == 1) {
		return "#D15B47";
	}
}

function switch_type_click_handle (obj, sensor_id, current_value) {
	set_sensor_web_value (sensor_id, current_value, function (data) {
		if (data == "success") {
		}
	});
}

/* SELECT METHODS */
function get_all_favorite_sensors (fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_sensors_info";
	json_string = JSON.stringify(json);
	
	$.post(endpoint_sensor, json_string, fn_ptr);
}

function get_all_sensors (fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_all_sensors";
	json_string = JSON.stringify(json);
	
	$.post(endpoint_sensor, json_string, fn_ptr);
}

function get_sensor_info (sensor_id, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_sensor_info";
	json.sensor_id = sensor_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_sensor, json_string, fn_ptr);
}

/* UPDATE METHODS */
function set_sensor_favorite (sensor_id, favorite, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "set_sensor_favorite";
	json.sensor_id = sensor_id;
	json.favorite = favorite;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_sensor, json_string, fn_ptr);
}

function set_sensor_web_value (sensor_id, action, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "set_sensor_action";
	json.sensor_id = sensor_id;
	json.action = action;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_sensor, json_string, fn_ptr);
}
