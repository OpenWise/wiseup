var endpoint_timer = 'endpoint/timers.php';
 
var edit_timer_content = 
"<h3 class='orange'>!ph_sensor_name!</h3>" +	
								"<div class='row'>"+
									"<div class='form-group'>"+
										"<div class='col-sm-9'>"+
											"<label class='control-label' style='float:left; padding-right:5px;'for='tb_timer_name'> Timer Name </label>"+
											"<input type='text' id='tb_timer_name' placeholder='Timer Name' class='col-xs-10 col-sm-5' value = '!ph_timer_name!' />"+
										"</div>"+
									"</div>"+
								
									"<div class='space-4'></div>"+
						
									"<div class='row'>"+
										"<div class='col-xs-12'>" +
											"<div class='col-sm-6'>" +	
												"<div class=''>" +	
												"<!-- date picker -->"+
													"<label for='tb_date_picker' >Date</label>"+	
												"</div>"+										
												"<div class='input-group'>"+												
													"<input class='form-control date-picker' id='tb_date_picker' type='text' data-date-format='dd-mm-yyyy' value = '!ph_timer_date!' />"+
													"<span class='input-group-addon'>"+
														"<i class='icon-calendar bigger-110'></i>"+
													"</span>"+
												"</div>"+
											"</div>"+
											"<div class='col-sm-6'>" +	
												"<!-- time picker -->"+											
												"<div class=''>"+	
													"<label for='tb_time_picker'; float:left;'>Time</label>"+										
												"</div>"+
												"<div class='input-group bootstrap-timepicker'>"+										
													"<input id='tb_time_picker' type='text' class='form-control' value = '!ph_timer_time!' />"+
													"<span class='input-group-addon'>"+
														"<i class='icon-time bigger-110'></i>"+
													"</span>"+
												"</div>"+
											"</div>"+
										"</div>"+								
									"</div>"+
									
									"<div class='space-4'></div>"+
									
									"<!-- timing options radio buttons -->"+									
									"<div class='col-xs-12 col-sm-4'>"+
										"<div class='control-group'>"+
											"<label class='control-label bolder blue'>Timing Options:</label>"+
											"<div class='radio'>"+
												"<label>"+
													"<input name='timer-type-radio' type='radio' value = '1' class='ace' />"+
													"<span class='lbl'> Once</span>"+
												"</label>"+
											"</div>"+
											"<div class='radio'>"+
												"<label>"+
													"<input name='timer-type-radio' type='radio' value = '2' class='ace' />"+
													"<span class='lbl'> Daily</span>"+
												"</label>"+
											"</div>"+
											"<div class='radio'>"+
												"<label>"+
													"<input name='timer-type-radio' type='radio' value = '3' class='ace' />"+
													"<span class='lbl'> Recurring</span>"+
												"</label>"+
											"</div>"+												
										"</div>"+
									"</div>"+	
										
									"<!-- timing options recurring checkboxes -->"+
									"<div class='col-xs-12 col-sm-8'>"+
										"<div class='control-group'>"+
											"<div class='col-xs-12 col-sm-12' >"+
												"<label class='control-label bolder blue'>Recurring:</label>"+
											"</div>"+											
											"<div class='col-xs-2 col-sm-5'>"+
												"<div class='checkbox'>"+
													"<label>"+
														"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
														"<span class='lbl'> Sunday</span>"+
													"</label>"+
												"</div>"+
												"<div class='checkbox'>"+
													"<label>"+
														"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
														"<span class='lbl'> Monday</span>"+
													"</label>"+
												"</div>"+
												"<div class='checkbox'>"+
													"<label>"+
														"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
														"<span class='lbl'> Tuesday</span>"+
													"</label>"+
												"</div>"+
												"<div class='checkbox'>"+
													"<label>"+
														"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
														"<span class='lbl'> Wensday</span>"+
													"</label>"+
												"</div>"+
											"</div>"+
											"<div class='col-xs-2 col-sm-5'>"+
												"<div class='checkbox'>"+
													"<label>"+
														"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
														"<span class='lbl'> Thursday</span>"+
													"</label>"+
												"</div>"+

												"<div class='checkbox'>"+
													"<label>"+
														"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
														"<span class='lbl'> Friday</span>"+
													"</label>"+
												"</div>"+
																					
											"<div class='checkbox'>"+
												"<label>"+
													"<input name='timer_occurrence-checkbox' type='checkbox' class='ace' />"+
													"<span class='lbl'> Saturday</span>"+
												"</label>"+
											"</div>"+
											"</div>"+	
										"</div>"+
									"</div>"+ "<!-- timing options recurring checkboxes - end -->"+										
									"<!-- action radio buttons -->"+									
									"<div class='col-xs-12 col-sm-4'>"+
										"<div class='control-group'>"+
											"<div class='radio'>"+
											"<label class='control-label bolder blue'>Action:</label>"+
												"<label>"+
													"<input name='timer-action-radio' type='radio' value = '0' class='ace' />"+
													"<span class='lbl'> Off </span>"+
												"</label>"+
												"<label>"+
													"<input name='timer-action-radio' type='radio' value = '1' class='ace' />"+
													"<span class='lbl'> On</span>"+
												"</label>"+
											"</div>"+																								
										"</div>"+
									"</div>"+			
								"</div><!-- /. end row -->";
								
/* UI METHODS */							
function onclick_add_timer (sensor_id,sensor_name) {

	// clear place_holders when opening NEW timer window
	var content = edit_timer_content;
	content = content.replace('!ph_sensor_name!',sensor_name);	
	content = content.replace('!ph_timer_name!','');
	content = content.replace('!ph_timer_date!','');
	content = content.replace('!ph_timer_time!','');

	bootbox.dialog({
		message: content,
		buttons: 			
		{
			"success" :
			 {
			
				"label" : "<i class='icon-ok'></i> Save",
				"className" : "btn-sm btn-success",
				"callback": function() {
				
				var days_binary = "0";
					$('input[name=timer_occurrence-checkbox]').each(function () {
						days_binary +=  (this.checked ? "1" : "0") ;
					});			
				 
					var json = new Object();
					var json_string = "";
																
					json.method = "insert_timer";
					json.sensor_id = sensor_id;
					json.timer_name = $('#tb_timer_name').val();
					json.timer_occurrence_type = $("input:radio[name='timer-type-radio']:checked").val();
					json.timer_datetime = Date.parse($('#tb_date_picker').val()).toString("yyyy-MM-dd") + ' ' + $('#tb_time_picker').val() +':00';
					json.timer_occurrence_days = parseInt(days_binary,2);
					json.timer_action = $("input:radio[name='timer-action-radio']:checked").val();
					
					json_string = JSON.stringify(json);
					
					$.post(endpoint_timer, json_string, function (data) { 
					
						if (data == 'success') {	
						}
					});
					
				}
			},
			"danger" :
			{
				"label" : "Cancel",
				"className" : "btn-sm btn-danger",
				"callback": function() {
			
				}
			}
		}
	});


$('#tb_date_picker').datepicker({ dateFormat: 'dd-mm-yy' ,autoclose:true}).next().on(ace.click_event, function(){
	$(this).prev().focus();
});	

$('#tb_time_picker').timepicker({
					minuteStep: 1,
					showSeconds: false,
					showMeridian: false
				}).next().on(ace.click_event, function(){
					$(this).prev().focus();
				});			
}

function get_timer_container_view (view_container, sensor_id) {
	get_timers_list_by_sensor_id (sensor_id, function (data) {

		var days_string = ['SUN','MON','TUE','WED','THU','FRI','SAT'];				
		var container = "";
		var javascript = 	"<script type='text/javascript'>" +
								"$('[data-rel=tooltip]').tooltip();" +
							"</script>";

		if (data != "NO_DATA") {
			var json = JSON.parse(data);
			var timers_count = json.sensor_timers_info.length;
			
			for (var index = 0; index < timers_count; index++) {
				var timer_html_row = view_timer_row;
				var timer = json.sensor_timers_info[index];

				var timer_name = "<span onclick=\"onclick_timer_handle(" + sensor_id + "," + timer.timer_id + ");\">" + timer.timer_name + "</span>";				
				timer_html_row = timer_html_row.replace('!ph_row_timer_name!', timer_name);
				timer_html_row = timer_html_row.replace('!ph_row_timer_occurrence_type!', timer.timer_occurrence_type);
				timer_html_row = timer_html_row.replace('!ph_row_timer_date!', timer.timer_date);
				timer_html_row = timer_html_row.replace('!ph_row_timer_time!', timer.timer_time);				
				timer_html_row = timer_html_row.replace('!ph_row_timer_action!', timer.timer_action);
				
				timer_html_row = timer_html_row.replace('!ph_row_timer_id!', timer.timer_id);
				timer_html_row = timer_html_row.replace('!ph_row_timer_id!', timer.timer_id); // needed twice
				timer_html_row = timer_html_row.replace('!ph_row_timer_state!', timer.timer_is_enabled);
				
				// append occurrence days names
				var occurance_days_bin = parseInt(timer.timer_occurrence_days, 10).toString(2);

				var occurrence_days_str="";
				for (var i = occurance_days_bin.length-1; i >= 0 ; i--) {
					if (occurance_days_bin[i] == '1' )
						occurrence_days_str =  days_string[(7 - occurance_days_bin.length) + i] + ',' + occurrence_days_str;
				}
				
				timer_html_row = timer_html_row.replace('!ph_row_timer_occurrence_days!', '<i class="red icon-calendar" data-rel="tooltip" data-placement="top" title='+ occurrence_days_str +'></i>');

				if (timer.timer_is_enabled == 1) {
					timer_html_row = timer_html_row.replace('!ph_row_timer_enabled!', "checked=''");
				} else {
					timer_html_row = timer_html_row.replace('!ph_row_timer_enabled!', "");
				}
				
				container += timer_html_row ;
			}
			container += javascript;
		}
		
		$(view_container).html(container);
	});
}

function onclick_timer_handle (sensor_id,timer_id) {

	get_timer_info_by_id (sensor_id,timer_id, function (data) {

		var json = JSON.parse(data);
		
		if (json != null) {
				var timer = json.timer_info[0];
				var content = edit_timer_content;
				
				content = content.replace('!ph_sensor_name!',timer.sensor_name);	
				content = content.replace('!ph_timer_name!',timer.timer_name);
				content = content.replace('!ph_timer_date!',timer.timer_date);
				content = content.replace('!ph_timer_time!',timer.timer_time);

				bootbox.dialog({
				message: content,
				buttons: 			
				{
					"success" :
					 {
						"label" : "<i class='icon-ok'></i> Save",
						"className" : "btn-sm btn-success",
						"callback": function() {
							
							var days_binary = "0";
								$('input[name=timer_occurrence-checkbox]').each(function () {
									days_binary +=  (this.checked ? "1" : "0") ;
								});
														 
								var json = new Object();
								var json_string = "";
																			
								json.method = "update_timer";
								json.timer_id = timer_id;								
								json.timer_name = $('#tb_timer_name').val();
								json.timer_occurrence_type = $("input:radio[name='timer-type-radio']:checked").val();
								json.timer_datetime = Date.parse($('#tb_date_picker').val()).toString("yyyy-MM-dd") + ' ' + $('#tb_time_picker').val() +':00';
								json.timer_occurrence_days = parseInt(days_binary,2);
								json.timer_action = $("input:radio[name='timer-action-radio']:checked").val();
								
								json_string = JSON.stringify(json);
								
								$.post(endpoint_timer, json_string, function (data) { 
									if (data == 'success') {	
									}
								});
						}
					},
					"danger" :
					{
						"label" : "Cancel",
						"className" : "btn-sm btn-danger",
						"callback": function() {
							
						}
					}
				}
			});
			
			$('#tb_date_picker').datepicker({ dateFormat: 'dd-mm-yy', autoclose:true}).next().on(ace.click_event, function(){
				$(this).prev().focus();
			});	
			$('#tb_time_picker').timepicker({
								minuteStep: 1,
								showSeconds: false,
								showMeridian: false
							}).next().on(ace.click_event, function(){
								$(this).prev().focus();
							});	
							
				// decimal to binary			
				var occurance_days_bin = parseInt(timer.timer_occurrence_days, 10).toString(2);
				// populate days checkboxes according to the binary sequence from right to left
				$($("input[name=timer_occurrence-checkbox]").get().reverse()).each(function (i) {
					if (occurance_days_bin[(occurance_days_bin.length-1)-i] == '1' )
						this.checked = true;
				});
				
				// check the correct "occurrence type" radio button
				$('input:radio[name="timer-type-radio"][value="' + timer.timer_occurrence_type + '"]').first().attr('checked','checked');
				
				// check the correct "action" radio button
				$('input:radio[name="timer-action-radio"][value="' + timer.timer_action + '"]').first().attr('checked','checked');			
		}
	});
	
	
}

function get_timer_info_by_id (sensor_id,timer_id, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_timer_info";
	json.timer_id = timer_id;
	json.sensor_id = sensor_id;
	json_string = JSON.stringify(json);yevg	
	$.post(endpoint_timer, json_string, fn_ptr);
}

/* SELECT METHODS */
function get_timers_list_by_sensor_id (sensor_id, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_timer_list_by_sensor_id";
	json.sensor_id = sensor_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_timer, json_string, fn_ptr);
}

/* UPDATE METHODS */  
function on_click_timer_enable_handler (timer_id, timer_curr_is_enabled ) {
	var json = new Object();
	var json_string = "";
	
	json.method = "set_timer_is_enabled";
	json.timer_id = timer_id;
	json.timer_is_enabled = 1 - timer_curr_is_enabled; // set as opposite
	json_string = JSON.stringify(json);

	$.post(endpoint_timer, json_string, function (data) {
		if (data == 'success')
		{			
		}
		
	});
}
function on_click_timer_delete_handler (timer_id) {
	var json = new Object();
	var json_string = "";
	alert('x');
	json.method = "delete_timer";
	json.timer_id = timer_id;
	json_string = JSON.stringify(json);

	$.post(endpoint_timer, json_string, function (data) {
		if (data == 'success')
		{			
		}
		
	});
}