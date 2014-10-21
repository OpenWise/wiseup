var endpoint_messages = 'endpoint/inbox.php';
var notifications_old_count = 0;
var alerts_old_count = 0;

// messages types : 1 = Notification ; 2 = Alerts

function get_source_icon (sensor_type) {
	switch (sensor_type)	{
		case "1": return "off";
		case "2": return "umbrella";
		case "3": return "lightbulb";
		case "4": return "male";
		case "5": return "lock";
		default:
			return "circle-blank";
	}
}

function change_inbox_curr_type (inbox_type) {
	localStorage.setItem("curr_inbox_type", inbox_type);
}

/* message - alerts - class */
view_message_alerts.prototype.header = '';	
view_message_alerts.prototype.footer = '';
view_message_alerts.prototype.row_template = '';	
view_message_alerts.prototype.json_data = '';	
view_message_alerts.prototype.count = 0;
view_message_alerts.prototype.genrate_ui_message_row = function(message) {
	var html = "";
	
	html = this.row_template;
	html = html.replace('!ph_item_dt!',message.general_messages_timestamp);
	html = html.replace('!ph_item_title!',message.general_message_title);
	html = html.replace('!ph_item_body!',message.general_message_body);
	html = html.replace('!ph_item_source!',get_source_icon(message.SensorType));
	html = html.replace('!ph_msg_click_js!','view_message_alert_mark_as_read(' + message.general_message_id + ');');
	html = html.replace('!ph_message_id!',message.general_message_id);
	
	return html;
}
view_message_alerts.prototype.wrap_message_ui_container = function(view) {
	html = "";
	
	html = this.header.replace('!ph_num_of_latest_items!',this.count);
	html += view;
	html += this.footer;
	// header footer
	return html;
}
view_message_alerts.prototype.generate_messages_ui = function() {
	var messages = this.json_data;
	var html_message_container = "";
	
	this.count = 0;	
	if (messages != null) {
		for (var i=0; i < messages.messages_info.length; i++) {
			item = messages.messages_info[i];
			if (item.general_message_type_id == "2") {	 // 2 = alerts	
				html_message_container += this.genrate_ui_message_row(item);
				this.count ++;
			}
		}
	}
		 
	return this.wrap_message_ui_container (html_message_container);
}
function view_message_alerts (data) { 
	this.json_data = JSON.parse(data);
	if (this.json_data != null) {
		this.count = this.json_data.messages_info.length;
	}
	this.header = 	"<li class='dropdown-header'>" +
						"<i class='icon-warning-sign'></i>" +
						"<span id = 'messages_alerts_inner_counter'>!ph_num_of_latest_items! Alerts </span>" +
					"</li>" +
					"<li>" +
							"<a href='#' onclick='on_click_event_handler_read_all(2);'>" +
								"Mark all alerts as read" + 
							"</a>" +
						"</li>";
					;
	this.footer = 	"<li>" +
						"<a href='/inbox' onmousedown='change_inbox_curr_type(2); return;'>" +
							"See all alerts " + 
							"<i class='icon-arrow-right'></i>" +
						"</a>" +
					 "</li>" ;
	this.row_template = "<li width='100%' id = 'messages_alert_!ph_message_id!'>" +
							"<a href='#'>" +
								"<i class='icon-!ph_item_source!' style='font-size: 250%;'> </i>" +
								"<span class='msg-body'>" +
									"<span class='msg-title'>" +
										"<span class='blue'>!ph_item_title!: </span>" +
										"!ph_item_body!" +
									"</span>" +
									"<span class='msg-time'>" +
										"<i class='icon-time'></i>" +
										"<span> !ph_item_dt!</span>" +
									"</span>" +		
									"<span style='color:#e09e96;' onclick='!ph_msg_click_js!'>Mark as read</span>" +	
								"</span>" +																
							"</a>" +
						"</li>" ;
}
function view_message_alert_mark_as_read (message_id) {
	var json = new Object();
	var json_string = "";

	json.method = "set_message_as_read";
	json.message_id = message_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_messages, json_string, function (data) { 
		if (data == 'success')
		{	
			var counter = parseInt($('#wiseup-data-container-latest-messages-alerts-counter').text())-1;
			$('#wiseup-data-container-latest-messages-alerts-counter').text(counter); // set msg counter 	
			$('#messages_alerts_inner_counter').text(counter + ' Alerts');	
			$('#messages_alert_' + message_id).remove();		
				
		}
	});
}

/* message - notification - class */
view_message_notifications.prototype.header = '';	
view_message_notifications.prototype.footer = '';
view_message_notifications.prototype.row_template = '';	
view_message_notifications.prototype.json_data = '';	
view_message_notifications.prototype.count = 0;
function view_message_notifications (data) { 
	this.json_data = JSON.parse(data);	
	if (this.json_data != null) {
		this.count = this.json_data.messages_info.length;
	}
	this.header = 		"<li class='dropdown-header'>" +
							"<i class='icon-envelope-alt'></i>" +
							"<span id = 'messages_notifications_inner_counter'>!ph_num_of_latest_items! Notifications </span>" +
						"</li>" +
						"<li>" +
							"<a href='#' onclick='on_click_event_handler_read_all(1);'>" +
								"Mark all notifications as read" + 
							"</a>" +
						"</li>";
	this.footer = 		"<li>" +
							"<a href='/inbox' onmousedown='change_inbox_curr_type(1); return;'>" +
								"See all notifications" + 
								"<i class='icon-arrow-right'></i>" +
							"</a>" +
						"</li>";
	this.row_template = "<li id = 'messages_notification_!ph_message_id!'>" +
							"<a href='#' >" +
								"<i class='icon-!ph_item_source! msg-sensor-icon' style='font-size: 250%;'> </i>" +
								"<span class='msg-body'>" +
									"<span class='msg-title'>" +
										"<span class='blue'>!ph_item_title!: </span>" +
										"!ph_item_body!" +										
									"</span>" +									
									"<span class='msg-time'>" +
										"<i class='icon-time'></i>" +
										"<span> !ph_item_dt!</span>" +										
									"</span>" +	
									"<span style='color:#e09e96;' onclick='!ph_msg_click_js!'>Mark as read</span>" +
								"</span>" +	
							"</a>" +
						"</li>";
}
view_message_notifications.prototype.genrate_ui_message_row = function(message) {
	var html = "";
	
	html = this.row_template;
	html = html.replace('!ph_item_dt!',message.general_messages_timestamp);
	html = html.replace('!ph_item_title!',message.general_message_title);
	html = html.replace('!ph_item_body!',message.general_message_body);
	html = html.replace('!ph_item_source!',get_source_icon(message.SensorType));
	html = html.replace('!ph_msg_click_js!','view_message_notification_mark_as_read(' + message.general_message_id + ');');
	html = html.replace('!ph_message_id!',message.general_message_id);
	
	return html;
}
view_message_notifications.prototype.wrap_message_ui_container = function(view) {
	html = "";
	
	html = this.header.replace('!ph_num_of_latest_items!',this.count);
	html += view;
	html += this.footer;
	// header footer
	return html;
}
view_message_notifications.prototype.generate_messages_ui = function() {
	var messages = this.json_data;
	var html_message_container = "";
	
	this.count = 0;
	if (messages != null) {
		for (var i=0; i < messages.messages_info.length; i++) {
			item = messages.messages_info[i];

			if (item.general_message_type_id == "1") {	// 1 = notifications	
				html_message_container += this.genrate_ui_message_row(item);
				this.count ++;	
			}
		}
	}
	
	return this.wrap_message_ui_container (html_message_container);
}

function on_click_event_handler_read_all (message_type) {
	var json = new Object();
	var json_string = "";

	json.method = "set_all_messages_by_type_as_read";
	json.message_type = message_type;
	json_string = JSON.stringify(json);
	$.post(endpoint_messages, json_string,function (data) { 
		if (data == 'success') {		
			switch (message_type)	{
				case 1:  
					// this is removing also the footer and header of this ul which is not good
					$('#wiseup-data-container-latest-messages-notifications' + " ul li").remove(); 
					$('#wiseup-data-container-latest-messages-notifications-counter').text('0');
				break;
				case 2:  
					// this is removing also the footer and header of this ul which is not good
					$('#wiseup-data-container-latest-messages-alerts' + " ul li").remove();
					$('#wiseup-data-container-latest-messages-alerts-counter').text('0');
				break;
			}
			
		}
	}); 
}

function on_click_event_handler_read_all_post_response (data,message_type) {
alert(data);
alert(message_type);
	
}

function view_message_notification_mark_as_read (message_id) {
	var json = new Object();
	var json_string = "";
	
	json.method = "set_message_as_read";
	json.message_id = message_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_messages, json_string, function (data) { 
		if (data == 'success') {	
			var counter = parseInt($('#wiseup-data-container-latest-messages-notifications-counter').text())-1;
			$('#wiseup-data-container-latest-messages-notifications-counter').text(counter); // set msg counter 	
			$('#messages_notifications_inner_counter').text(counter + ' Notifications');	
			$('#messages_notification_' + message_id).remove();
		}
	});
}

function start_latest_message_service (view_container) {
	interval_id_get_latest_msgs = setInterval(function() { ui_get_latest_messages(view_container) }, 10000);
	ui_get_latest_messages (view_container);	
}

/* function end_favorite_sensors_service () {
	clearInterval(interval_id_get_latest_msgs);
} */

function get_last_unread_messages (fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_latest_messages";
	json_string = JSON.stringify(json);
	
	$("#wiseup-data-container-latest-messages-alerts-icon-animation").removeClass("icon-animated-bell");	// disable icon animation							
	$("#wiseup-data-container-latest-messages-notifications-icon-animation").removeClass("icon-animated-vertical");	// disable icon animation						
	$.post(endpoint_messages, json_string, fn_ptr);
}

function ui_get_latest_messages (view_container) {
	get_last_unread_messages(function (data) {
		// notifications
		var view_messages_notifications_obj = new view_message_notifications (data);
		$(view_container + 'notifications' + " ul li").remove();
		$(view_container + 'notifications' + " ul").html(view_messages_notifications_obj.generate_messages_ui ());
		$(view_container + 'notifications' + "-counter").text(view_messages_notifications_obj.count); // set msg counter 
		
		// animate icon if there is a new message		
		if (view_messages_notifications_obj.count != notifications_old_count) {
			$(view_container + 'notifications' + "-icon-animation").addClass("icon-animated-vertical"); // enable icon animation		
			notifications_old_count = view_messages_notifications_obj.count;
		}
		
		// alerts
		var view_messages_alerts_obj = new view_message_alerts (data);
		$(view_container + 'alerts' + " ul li").remove();
		$(view_container + 'alerts' + " ul").html(view_messages_alerts_obj.generate_messages_ui ());
		$(view_container + 'alerts' + "-counter").text(view_messages_alerts_obj.count); // set msg counter 
		
		 // animate icon if there is a new message
		if (view_messages_alerts_obj.count != alerts_old_count) {
			$(view_container + 'alerts' + "-icon-animation").addClass("icon-animated-bell"); // enable icon animation		
			alerts_old_count = view_messages_alerts_obj.count;
		}
		
	});
}

/* public */
function view_message_insert (message_type_id, message_source, message_title, message_body, fn_ptr) {
	var json = new Object();
	var json_string = "";

	json.method = "insert_message";	
	// json.dt = (new Date()).toISOString().replace(/(\d+-\d+-\d+)T(\d+:\d+:\d+).*/, "$1 $2");	
	json.message_type_id = message_type_id;
	json.message_source = message_source;
	json.message_title = message_title;
	json.message_body = message_body;	
	json_string = JSON.stringify(json);
	
	$.post(endpoint_messages, json_string, fn_ptr);
}