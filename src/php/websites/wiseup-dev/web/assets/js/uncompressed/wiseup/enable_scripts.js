var endpoint_enable_scripts = 'endpoint/enable_scripts.php';
var enable_scripts_view_container = "";
var interval_id_get_enable_scripts_info = -1;

/* ui_builder_enable_script class */
function ui_builder_enable_script (en_script) { 
	this.en_script = en_script;
	this.ui_en_script = 	"<div class='infobox infobox-blue' style='float:left;'>" +
								"!ph_en_script_icon!" +
								"<div class='infobox-data viseup-blue-value'>" +
									"<span class='wiseup-infobox-data-number viseup-blue-value'>!ph_en_script_value!</span>" +
									"<div class='infobox-content'>!ph_en_script_name!</div>" +
								"</div>" +
							"</div>";
}
ui_builder_enable_script.prototype.en_script = '';
ui_builder_enable_script.prototype.ui_en_script = '';
ui_builder_enable_script.prototype.ui_en_script_object = function () {
	var html = this.ui_en_script.replace('!ph_en_script_name!',this.en_script.enable_script_name);
	
	if (this.en_script.value == '1') {
		html = html.replace('!ph_en_script_icon!', "<i class='icon-check wiseup-orange-2-icon' style='cursor:pointer;' onmouseout=\"this.style.color='#FEB902'\" onmouseover=\"this.style.color='#C3C3C3'\" onclick=\"on_click_enable_handler(" + this.en_script.enable_script_id + ",0);\"></i>");
		html = html.replace('!ph_en_script_value!', "Enabled");
	} else {
		html = html.replace('!ph_en_script_icon!', "<i class='icon-check wiseup-gray-icon' style='cursor:pointer;' onmouseout=\"this.style.color='#C3C3C3'\" onmouseover=\"this.style.color='#FEB902'\" onclick=\"on_click_enable_handler(" + this.en_script.enable_script_id + ",1);\"></i>");
		html = html.replace('!ph_en_script_value!', "Disabled");
	}
	
	return html;
};

function start_favorite_enable_scrpts_service (view_container) {
	enable_scripts_view_container = view_container;
	ui_binder_favorite_enable_scripts (view_container);
}

function generate_view_enable_scripts_from_data (data) {
	var json = JSON.parse(data);
	var container = "<div class='infobox-container'>";
	if (json != null) {
		var en_script_count = json.enable_scripts_info.length;
		
		for (var index = 0; index < en_script_count; index++) {
			var en_script = json.enable_scripts_info[index];
			if (en_script != "") {
				var en_script_builder = new ui_builder_enable_script (en_script);
				container += en_script_builder.ui_en_script_object ();
			}
		}
	} 

	return container + "</div>";
}

function ui_binder_favorite_enable_scripts (view_container) {
	get_all_favorite_enable_scripts (function (data) {
		$(view_container).html(generate_view_enable_scripts_from_data (data));
	});
}

function on_click_enable_handler (en_script_id, enable) {
	execute_en_script (en_script_id, enable, function (data) {
		ui_binder_favorite_enable_scripts(enable_scripts_view_container);
	});
}

function on_click_favorite_handler (event_id, value) {
	set_event_favorite_value (event_id, value, function (data) {
		
	});
}

/* SELECT METHODS */
function get_all_favorite_enable_scripts (fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_favorite_enable_scripts";
	json_string = JSON.stringify(json);
	
	$.post(endpoint_enable_scripts, json_string, fn_ptr);
}

/* UPDATE METHODS */
function execute_en_script (en_script_id, value, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "execute_enable_script";
	json.en_script_id = en_script_id;
	json.value = value;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_enable_scripts, json_string, fn_ptr);
}

function set_event_favorite_value (event_id, value, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "set_event_script_favorite_value";
	json.event_id = event_id;
	json.favorite_value = value;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_enable_scripts, json_string, fn_ptr);
}