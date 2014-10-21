var endpoint_inbox = 'endpoint/inbox.php';

var view_inbox_messages_obj,view_inbox_settings_obj ; 

// messages types : 1 = Notification ; 2 = Alerts

/*setting class*/
view_inbox_settings.prototype.from = 0;
view_inbox_settings.prototype.total = 10; // total per page
view_inbox_settings.prototype.jump = 10; // each page jumps by amount of messages
function view_inbox_settings (){
}

/*messages class*/

view_inbox_messages.prototype.view_container_name ='';
view_inbox_messages.prototype.json_data = '';	
view_inbox_messages.prototype.row_template = '';	
view_inbox_messages.prototype.count_total = 0;
view_inbox_messages.prototype.count_unread = 0;
view_inbox_messages.prototype.current_visible_type = 1;

function view_inbox_messages (data) { 
	this.json_data = JSON.parse(data);	
	this.current_visible_type = localStorage.getItem("curr_inbox_type");
	this.row_template = "<div class='message-item !ph_message_unread!'>"+
								"<label class='inline'>"+
									"<input id = '!ph_cb_id!' type='checkbox' class='ace' value='!ph_cb_msg_id!'/>"+
									"<span class='lbl'></span>"+
								"</label>"+

								"<i class='message-star icon-!ph_message_source! orange'></i>"+
								"<span class='sender' title=''>!ph_message_sender!</span>"+
								"<span class='time' style='text-align:right; width:140px;'>!ph_message_dt!</span>"+

								"<span class='summary'>"+
								"<i class='!ph_message_type!' style = 'margin-right:3px;'></i>"+
									"<span class='text'>"+
										"!ph_message!"+
									"</span>"+
								"</span>"+
							"</div>";
}
view_inbox_messages.prototype.get_ui_inbox_wrapper = function(view) {
	html = 	
				"<div class='tabbable'>"+
					"<ul id='inbox-tabs' class='inbox-tabs nav nav-tabs padding-16 tab-size-bigger tab-space-1'>"+
						"<li id = 'wiseup_inbox_tab_type_0'>"+
							"<a data-toggle='tab' href='#inbox' data-target='inbox' onclick='switch_inbox_type(0); return;'>"+
								"<i class='blue icon-inbox bigger-130'></i>"+
								"<span class='bigger-110'>Inbox</span>"+
								"<span id = 'wiseup_inbox_unread_0' class='bigger-110'></span>"+
							"</a>"+
						"</li>"+

						"<li id = 'wiseup_inbox_tab_type_2'>"+
							"<a data-toggle='tab' href='#sent' data-target='sent' onclick='switch_inbox_type(2); return;'>"+
								"<i style = 'color:#d15b47;' class='red icon-bell-alt bigger-130 '></i>"+
								"<span class='bigger-110'>Alerts</span>"+
								"<span id = 'wiseup_inbox_unread_2' class='bigger-110'></span>"+
							"</a>"+
						"</li>"+

						"<li id = 'wiseup_inbox_tab_type_1'>"+
							"<a data-toggle='tab' href='#draft' data-target='draft' onclick='switch_inbox_type(1); return;'>"+
								"<i class='green icon-envelope bigger-130'></i>"+
								"<span class='bigger-110'>Notifications</span>"+
								"<span id = 'wiseup_inbox_unread_1' class='bigger-110'></span>"+
							"</a>"+
						"</li>"+
					"</ul>"+

					"<div class='tab-content no-border no-padding'>"+
						"<div class='tab-pane in active'>"+
							"<div class='message-container'>"+
								"<div id='id-message-list-navbar' class='message-navbar align-center'>"+
									"<div class='message-bar'>"+
										"<div class='message-infobar' id='id-message-infobar'>"+
											"<span class='grey bigger-110' id ='wiseup_data_container_inbox_unread'> </span>"+
										"</div>"+

										"<div class='message-toolbar hide'>"+
											"<div class='inline position-relative align-center'>"+
												"<a href='#' class='btn-message btn btn-xs dropdown-toggle' data-toggle='dropdown'>"+
													"<span class='bigger-110'>Action</span>"+
													"<i class='icon-caret-down icon-on-right'></i>"+
												"</a>"+

												 "<ul class='dropdown-menu dropdown-lighter dropdown-caret dropdown-125'>"+
													"<li>"+
														"<a href='#' onclick='inbox_mark_message_status(1); return;'>"+
															"<i class='icon-eye-open blue'></i>"+
															"&nbsp; Mark as read"+
														"</a>"+
													"</li>"+

													"<li>"+
														"<a href='#' onclick='inbox_mark_message_status(0); return;'>"+
															"<i class='icon-eye-close green'></i>"+
															"&nbsp; Mark unread"+
														"</a>"+
													"</li>"+													
												"</ul>"+
											"</div>"+
										"</div>"+
									"</div>"+

									"<div>"+
										"<div class='messagebar-item-left'>"+
											"<label class='inline middle'>"+
												"<input type='checkbox' id='id-toggle-all' class='ace' />"+
												"<span class='lbl'></span>"+
											"</label>"+

											"&nbsp;"+
											"<div class='inline position-relative'>"+
												"<a href='#' data-toggle='dropdown' class='dropdown-toggle'>"+
													"<i class='icon-caret-down bigger-125 middle'></i>"+
												"</a>"+

												"<ul class='dropdown-menu dropdown-lighter dropdown-100'>"+
													"<li>"+
														"<a id='id-select-message-all' href='#'>All</a>"+
													"</li>"+

													"<li>"+
														"<a id='id-select-message-none' href='#'>None</a>"+
													"</li>"+

													"<li class='divider'></li>"+

													"<li>"+
														"<a id='id-select-message-unread' href='#'>Unread</a>"+
													"</li>"+

													"<li>"+
														"<a id='id-select-message-read' href='#'>Read</a>"+
													"</li>"+
												"</ul>"+
											"</div>"+
										"</div>"+

									"</div>"+
								"</div>"+
								
								"<div class='message-list-container'>"+
									"<div id= 'wiseup-data-container-inbox-messages' class='message-list' id='message-list'>"+									
										"!ph_all_messages!" +	
									"</div>"+
								"</div><!-- /.message-list-container -->"+

								"<div class='message-footer clearfix'>"+
									"<div class='pull-left' id='wiseup_data_container_inbox_total'></div>"+

									"<div class='pull-right'>"+
										"<div id = 'wiseup_data_container_inbox_paging' class='inline middle'></div>"+

										"&nbsp; &nbsp;"+
										"<ul class='pagination middle'>"+
											"<li>"+ 
												"<span>"+																							
													"<i id = 'btn_prev_inbox_messages' class='icon-caret-left bigger-140 middle'  onclick = 'get_prev_mesages(); return;' ></i>"+
												"</span>"+
											"</li>"+

											"<li>"+
												"<span>"+
													"<i id ='btn_next_inbox_messages' class='icon-caret-right bigger-140 middle' style = 'cursor: pointer;' onclick = 'get_next_mesages(); return;'></i>"+
												"</span>"+
											"</li>"+
										"</ul>"+
									"</div>"+
								"</div>"+							
							"</div><!-- /.message-container -->"+
						"</div><!-- /.tab-pane -->"
					"</div><!-- /.tab-content -->"+
				"</div><!-- /.tabbable -->";

				
	return html;
}
view_inbox_messages.prototype.generate_ui_inbox_row = function(message) {
	var html = "";
	html = this.row_template;
	html = html.replace('!ph_message_sender!',message.general_message_title);
	html = html.replace('!ph_message_dt!',message.general_messages_timestamp);
	html = html.replace('!ph_message!',message.general_message_body);
	html = html.replace('!ph_message_source!',get_message_source_icon(message.SensorType));
	html = html.replace('!ph_message_type!',get_message_type_icon(message.general_message_type_id));
	html = html.replace('!ph_cb_msg_id!',message.general_message_id);


	if (message.is_read == 0) //0= unread
	{
		html = html.replace('!ph_message_unread!','message-unread');
	}
	return html;
}
view_inbox_messages.prototype.generate_ui_inbox_conent = function() {
	var messages = this.json_data;
	var html_inbox_container = 	"";
	
	if (messages != null) {
		this.count_total = 0;
		this.count_unread = 0;
		for (var i=0; i < messages.messages_info.length; i++) {
			item = messages.messages_info[i];	
			// 0 = both (alerts(2) and notifications(1))
			if (this.current_visible_type == 0 || item.general_message_type_id  == this.current_visible_type) 
			{	
				html_inbox_container += this.generate_ui_inbox_row(item);
				this.count_total ++;
				if (item.is_read == 0) //0= unread
				{
					this.count_unread++;
				}
			}
			
		}
	}
		
	return html_inbox_container;
}

function switch_inbox_type (inbox_type) {
	view_inbox_messages_obj.current_visible_type = inbox_type;
	localStorage['curr_inbox_type'] = inbox_type; 
	ui_get_inbox_messages (view_inbox_messages_obj.view_container_name);
}

function get_message_source_icon (source_type) {
	switch (source_type)	{
		case "1": return "off";
		case "2": return "umbrella";
		case "3": return "lightbulb";
		case "4": return "male";
		case "5": return "lock";
		default:
			return "circle-blank";
	}
}

function get_message_type_icon (message_type) {
	switch (message_type)	{
		case "1": return "green icon-envelope";
		case "2": return "red icon-bell-alt";
		default:
			return "circle-blank";
	}
}

function get_inbox_messages (fn_ptr) {
	var json = new Object();
	var json_string = "";

	json.method = "get_inbox_messages";
	json.message_type = localStorage['curr_inbox_type'];
	json.from = view_inbox_settings_obj.from;
	json.total = view_inbox_settings_obj.total;	
	json_string = JSON.stringify(json);
			
	$.post(endpoint_inbox, json_string, fn_ptr);
}

function bind_ui_inbox_content () {
	$('#wiseup-data-container-inbox-messages').html(view_inbox_messages_obj.generate_ui_inbox_conent());
	$('#inbox-tabs li.active').removeClass("active");
	wiseup_inbox_tab_type_0
	$('#wiseup_inbox_tab_type_'+view_inbox_messages_obj.current_visible_type).addClass("active");
	
	if (view_inbox_messages_obj.count_unread == 0)
		$('#wiseup_inbox_unread_'+ view_inbox_messages_obj.current_visible_type).html('');
	else
		$('#wiseup_inbox_unread_'+ view_inbox_messages_obj.current_visible_type).html(' (' + view_inbox_messages_obj.count_unread + ')');
				
	$('#wiseup_data_container_inbox_total').text(view_inbox_messages_obj.count_total + ' total messages');	
	
	// paging
	var from, to;
	from = view_inbox_settings_obj.from + 1;
	to = (parseInt(view_inbox_settings_obj.from) + parseInt(view_inbox_messages_obj.count_total)).toString();
	$('#wiseup_data_container_inbox_paging').text(from + ' - ' + to);	
	
	if (view_inbox_settings_obj.from >= view_inbox_settings_obj.jump)
	{
		$('#btn_prev_inbox_messages').css('cursor', 'pointer');				
	}
	else
	{
		$('#btn_prev_inbox_messages').closest('li').addClass('disabled');
	}
	
	load_ace_inbox_scripts();
}

function ui_get_inbox_messages (view_container) {
	get_inbox_messages(function (data) {		
		view_inbox_messages_obj = new view_inbox_messages (data);	
		view_inbox_messages_obj.view_container_name = view_container;
		$(view_container).html(view_inbox_messages_obj.get_ui_inbox_wrapper());
		bind_ui_inbox_content();
	});
}

function start_inbox_service (view_container) {
	//interval_id_get_latest_msgs = setInterval(function() { ui_get_inbox_messages(view_container) }, 15000);
	view_inbox_settings_obj = new view_inbox_settings();
	view_inbox_settings_obj.from = 0;
	view_inbox_settings_obj.total = 10;
	ui_get_inbox_messages (view_container);
}

function load_ace_inbox_scripts () {
	// Adding the script tag to the head as suggested before
    var head = document.getElementsByTagName('head')[0];
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = 'assets/js/uncompressed/ace-inbox.js';

    // Fire the loading
    head.appendChild(script);
}

function inbox_mark_message_status (to_status) {
	var msgs_list ='';
	$('.message-item input[type=checkbox]:checked').each(function(){
		msgs_list += $(this).val() + ',';

					//this.checked = true;
					//$(this).closest('.message-item').addClass('selected');
					//count++;
	});
	msgs_list = msgs_list.slice(0, -1); // remove last comma

	var json = new Object();
	var json_string = "";
	
	json.method = "set_messages_status";
	json.msgs_list = msgs_list;
	json.set_to_status = to_status;
	json_string = JSON.stringify(json);
			
	$.post(endpoint_inbox, json_string, function (data) {
		if (data == 'success')
		{ 
			// $('.message-item input[type=checkbox]:checked').each(function(){					
				// if (to_status == 0) // unread
					// $(this).closest('.message-item').addClass('message-unread');
				// else // read
					// $(this).closest('.message-item').removeClass('message-unread');
			// });		
			ui_get_inbox_messages(view_inbox_messages_obj.view_container_name);
			
		}	
	});
}

function get_next_mesages () {
	view_inbox_settings_obj.from += view_inbox_settings_obj.jump;
	ui_get_inbox_messages (view_inbox_messages_obj.view_container_name);	
}

function get_prev_mesages () {
	if (view_inbox_settings_obj.from < view_inbox_settings_obj.jump)
		return;	
	view_inbox_settings_obj.from -= view_inbox_settings_obj.jump;
	ui_get_inbox_messages (view_inbox_messages_obj.view_container_name);
}
