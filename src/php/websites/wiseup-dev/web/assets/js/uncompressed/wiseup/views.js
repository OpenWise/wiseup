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
							
var view_sensors = 	"<div class='page-header'>" + 
						"<h1 id='lbl-selected-menu'> Favorite </h1>" + 
					"</div>" + 
					"<div class='row'>" + 
						"<div class='col-xs-12'>" + 
							"<div class='col-sm-12 infobox-container' id='wiseup-data-container-sensors'></div>" + 
							"<div class='col-sm-12 infobox-container' id='wiseup-data-container-foreign-sensors'></div>" + 
						"</div>" + 
					"</div>";

var view_dashboard = 	"<div class='breadcrumbs' id='breadcrumbs'>" + 
							"<script type='text/javascript'>" +
								"try{ace.settings.check('breadcrumbs' , 'fixed')}catch(e){}" +
							"</script>" +
							"<ul class='breadcrumb'>" + 
								"<li>" + 
									"<i class='icon-home home-icon'></i><a href='#'>Home</a></li><li class='active'>Dashboard" + 
								"</li>" + 
							"</ul>" +
							"<div class='nav-search' id='nav-search'>" + 
								"<form class='form-search'>" + 
									"<span class='input-icon'>" + 
										"<input type='text' placeholder='Search ...' class='nav-search-input' id='nav-search-input' autocomplete='off' />" + 
										"<i class='icon-search nav-search-icon'></i>" + 
									"</span>" + 
								"</form>" + 
							"</div>" +
						"</div>" +
						"<div class='page-content'>" + view_favorite_sensors + "</div>";
						
var view_sensors_and_actions = 	"<div class='breadcrumbs' id='breadcrumbs'>" + 
									"<script type='text/javascript'>" + 
										"try{ace.settings.check('breadcrumbs' , 'fixed')}catch(e){}" + 
									"</script>" + 
									"<ul class='breadcrumb'>" + 
										"<li>" + 
											"<i class='icon-home home-icon'></i>" + 
											"<a href='#'>Home</a>" + 
										"</li>" + 
										"<li class='active'>Sensors & Actions" + 
										"</li>" + 
									"</ul>" + 
									"<div class='nav-search' id='nav-search'>" + 
										"<form class='form-search'>" + 
											"<span class='input-icon'>" + 
												"<input type='text' placeholder='Search ...' class='nav-search-input' id='nav-search-input' autocomplete='off' />" + 
												"<i class='icon-search nav-search-icon'></i>" + 
											"</span>" + 
										"</form>" + 
									"</div>" + 
								"</div>" + 
								"<div class='page-content'>" + view_sensors + "</div>";

var modal_window_timer = 	"<div id='modal-wizard-timer' class='modal'>" +
								"<div class='modal-dialog'>" +
									"<div class='modal-content'>" +
									
									
"<div class='modal-body'>"+
"<button type='button' class='bootbox-close-button close' style='margin-top: -10px;'>×</button>"+
"<div class='bootbox-body'>"+
"<div class='widget-main'>"+

"<h3 class='orange'>QBox Bedroom</h3>"+

"</div>"+
"</div>"+
"</div>"+

"<div class='modal-footer'>"+
"<button data-bb-handler='success' type='button' class='btn btn-sm btn-success'><i class='icon-ok'></i> Save</button>"+
"<button data-bb-handler='danger' type='button' class='btn btn-sm btn-danger'> Cancel</button>"+
"</div>"+
								
										
									"</div>" +
								"</div>" +
							"</div>";			
							
							
/*var modal_window_timer = 	"<div id='modal-wizard-timer' class='modal'>" +
								"<div class='modal-dialog'>" +
									"<div class='modal-content'>" +
										"<div class='modal-header' data-target='#modal-step-contents'>" +
											"<ul class='wizard-steps'>" +
												"<li data-target='#modal-step1' class='active'>" +
													"<span class='step'>1</span>" +
													"<span class='title'>Details</span>" +
												"</li>" +
												"<li data-target='#modal-step2'>" +
													"<span class='step'>2</span>" +
													"<span class='title'>Action</span>" +
												"</li>" +
												"<li data-target='#modal-step3'>" +
													"<span class='step'>3</span>" +
													"<span class='title'>Service</span>" +
												"</li>" +
												"<li data-target='#modal-step4'>" +
													"<span class='step'>4</span>" +
													"<span class='title'>Summary</span>" +
												"</li>" +
											"</ul>" +
										"</div>" +
										"<div class='modal-body step-content' id='modal-step-contents'>" +
											"<div class='step-pane active' id='modal-step1'>" +
												"<div class='left'>" +
													"<h5 class='grey'>Please select time and date for this timer.</h5>" +
												"</div>" +
											"</div>" +
											"<div class='step-pane' id='modal-step2'>" +
												"<div class='left'>" +
													"<h5 class='grey'>Please select action for this timer.</h5>" +
													"<h5 class='grey'>All available actions for this timer are listed bellow.</h5>" +
												"</div>" +
											"</div>" +
											"<div class='step-pane' id='modal-step3'>" +
												"<div class='left'>" +
													"<h5 class='grey'>Please select service witch will be used for this timer.</h5>" +
													"<h5 class='grey'>All available services for this timer are listed bellow.</h5>" +
												"</div>" +
											"</div>" +
											"<div class='step-pane' id='modal-step4'>" +
												"<div class='center'>" +
													"<h4 class='blue'>Summary table</h4>" +
												"</div>" +
											"</div>" +
										"</div>" +
										"<div class='modal-footer wizard-actions'>" +
											"<button class='btn btn-sm btn-prev'>" +
												"<i class='icon-arrow-left'></i>Prev" +
											"</button>" +
											"<button class='btn btn-success btn-sm btn-next' data-last='Finish '>Next" +
												"<i class='icon-arrow-right icon-on-right'></i>" +
											"</button>" +
											"<button class='btn btn-danger btn-sm pull-left' data-dismiss='modal'>" +
												"<i class='icon-remove'></i>Cancel" +
											"</button>" +
										"</div>" +
									"</div>" +
								"</div>" +
							"</div>" +
							"<script type='text/javascript'>" +
								"$('#modal-wizard-timer .modal-header').ace_wizard();" +
								"$('#modal-wizard-timer .wizard-actions .btn[data-dismiss=modal]').removeAttr('disabled');" +
							"</script>";	*/						
var modal_window_event = 	"<div id='modal-wizard-event' class='modal'>" +
								"<div class='modal-dialog'>" +
									"<div class='modal-content'>" +
										"<div class='modal-header' data-target='#modal-step-contents-event'>" +
											"<ul class='wizard-steps'>" +
												"<li id='modal-header-step1' data-target='#modal-step1' class='active'>" +
													"<span class='step'>1</span>" +
													"<span class='title'>Event</span>" +
												"</li>" +
												"<li id='modal-header-step2' data-target='#modal-step2'>" +
													"<span class='step'>2</span>" +
													"<span class='title'>Sensor</span>" +
												"</li>" +
												"<li id='modal-header-step3' data-target='#modal-step3'>" +
													"<span class='step'>3</span>" +
													"<span class='title'>Service</span>" +
												"</li>" +
												"<li id='modal-header-step4' data-target='#modal-step4'>" +
													"<span class='step'>4</span>" +
													"<span class='title'>Summary</span>" +
												"</li>" +
											"</ul>" +
										"</div>" +
										"<div class='modal-body step-content' id='modal-step-contents-event'>" +
											"<div class='step-pane active' id='modal-step1'>" +
												"<div class='left'>" +
													"<h5 class='grey'>Please select event you wan to apply on this sensor.</h5>" +
													"<h5 class='grey'>All available events for this sensor are listed bellow.</h5>" +
													"<hr />" +
													"<span><b>Selected event: </b></span>&nbsp;<span id='selected_wiseup_event' class='blue'></span>" +
													"<div id='wiseup-data-container-events-step1'></div>" +
												"</div>" +
											"</div>" +
											"<div class='step-pane' id='modal-step2'>" +
												"<div class='left'>" +
													"<h5 class='grey'>Please select sensor witch will be triggered on this event.</h5>" +
													"<h5 class='grey'>All available sensor for this event are listed bellow.</h5>" +
													"<hr /> " +
												  /*"<div class='checkbox'> " +
														"<label>" +
															"<input onclick='set_dis_en_action (this, \"#wiseup-data-container-events-step2\");' name='form-field-checkbox' class='ace ace-checkbox-2' type='checkbox' />" +
															"<span class='lbl'>&nbsp;No action required on this event.</span> " +
														"</label> " +
													"</div>" + */
													"<span style='margin-left:15px;'><b>Selected sensor: </b></span>&nbsp;<span class='blue' id='selected_wiseup_sensor'></span>&nbsp;&nbsp;&nbsp;" +
													"<span style='margin-left:15px;'><b>Selected action: </b></span>&nbsp;<b><span id='selected_wiseup_action'></span></b>" +
													"<div id='wiseup-data-container-events-step2'></div>" +
												"</div>" +
											"</div>" +
											"<div class='step-pane' id='modal-step3'>" +
												"<div class='left'>" +
													"<h5 class='grey'>Please select service witch will be used for this event.</h5>" +
													"<h5 class='grey'>All available services for this event are listed bellow.</h5>" +
													"<div id='wiseup-data-container-events-step3'></div>" +
												"</div>" +
											"</div>" +
											"<div class='step-pane' id='modal-step4'>" +
												"<div class='left'>" +
													"<h4 class='blue'>Event summary</h4>" +
													"<div id='wiseup-data-container-events-step4'></div>" +
												"</div>" +
											"</div>" +
										"</div>" +
										"<div class='modal-footer wizard-actions'>" +
											"<button class='btn btn-sm btn-prev'>" +
												"<i class='icon-arrow-left'></i>Prev" +
											"</button>" +
											"<button class='btn btn-success btn-sm btn-next' data-last='Finish '>Next" +
												"<i class='icon-arrow-right icon-on-right'></i>" +
											"</button>" +
											"<button onclick='sensor_settings_click_handle (this, summary.wizard_sensor_id);' class='btn btn-danger btn-sm pull-left' data-dismiss='modal'>" +
												"<i class='icon-remove'></i>Cancel" +
											"</button>" +
										"</div>" +
									"</div>" +
								"</div>" +
							"</div>" +
							"<script type='text/javascript'>" +
								"$('#modal-wizard-event .modal-header').ace_wizard('selectItem', { step: 1 }).on('change', function(e, info){ " +
									"event_step_change (info);" +
								"}).on('finished', function(){ " +
									"event_step_finished ();" +
								"});" +
								"$('#modal-wizard-event .wizard-actions .btn[data-dismiss=modal]').removeAttr('disabled');" +
							"</script>";
							
var view_sensor_info = 	"<div class='page-content'>" +
							"<div class='page-header'>" +
								"<h1>Sensor Information<small><i class='icon-double-angle-right'></i> Editor</small></h1>" +
							"</div>" +
							"<div class='row'>" +
								"<div class='col-xs-12'>" +
									"<div>" +
										"<div id='user-profile-1' class='user-profile row'>" +
											"<div class='col-xs-12 col-sm-4 center' id='wiseup-data-container-sensor-async-update'>" + // image + value area
											"</div>" +
											"<div class='col-xs-12 col-sm-8'>" +
												"<div class='widget-box transparent'>" +
													"<div class='widget-header widget-header-flat'>" +
														"<h4 class='lighter'>" +
															"<i class='icon-book light-grey'></i>Details</h4>" +
													"</div>" +
													"<div class='widget-body' id='wiseup-data-container-sensor-details'>" +
														"<div class='space-12'></div>" +
														"<div class='profile-user-info profile-user-info-striped' id='wiseup-data-container-sensor-details'>" + // detail area
														"!ph_sensor_details!" +
														"</div>" +
													"</div>" +
												"</div>" +
												"<br />" +
												"<div class='widget-box transparent'>" +
													"<div class='widget-header widget-header-flat'>" +
														"<h4 class='lighter'>" +
															"<i class='icon-check light-green'></i>Features</h4>" +
													"</div>" +
													"<div class='widget-body'>" +
														"<div class='widget-main no-padding'>" +
															"<table class='table table-bordered table-striped'>" +
																"<thead class='thin-border-bottom'>" +
																	"<tr>" +
																		"<th>" +
																			"<i class='icon-caret-right blue'></i>Name</th>" +
																		"<th class='hidden-480'>" +
																			"<i class='icon-caret-right blue'></i>Settings</th>" +
																		"<th class='hidden-480'>" +
																			"<i class='icon-caret-right blue'></i>Enabled</th>" +
																	"</tr>" +
																"</thead>" +
																"<tbody>!ph_sensor_feature_list!</tbody>" + // features area
															"</table>" +
														"</div>" +
													"</div>" +
												"</div>" +
												"<div class='space-20'></div>" +
												/* "<div class='hr hr2 hr-double'></div>" +
												"<div class='space-6'></div>" +
												"!ph_sensor_actions!" + */
											"</div>" +
										"</div>" +
									"</div>" +
								"</div>" +
							"</div>" +
							/*"<div class='row'>" + // graph area
								"<div class='col-sm-12'>" +
									"<div class='widget-box transparent'>" +
										"<div class='widget-header widget-header-flat'>" +
											"<h4 class='lighter'>" +
												"<i class='icon-signal'></i>Graph</h4>" +
										"</div>" +
										"<div class='widget-body' id='wiseup-data-container-sensor-graph'>" +
										"</div>" +
									"</div>" +
								"</div>" +
							"</div>" +*/
							"<br />" +
							"<div class='row'>" +
								"<div class='col-sm-6'>" +
									"<div class='widget-box transparent'>" +
										"<div class='widget-header widget-header-flat'>" +
											"<h4 class='lighter'>" +
												"<i class='icon-bolt orange'></i>Events</h4>" +
										"</div>" +
										"<div class='widget-body'>" +
											"<div class='widget-main no-padding'>" +
												"<table class='table table-bordered table-striped'>" +
													"<thead class='thin-border-bottom'>" +
														"<tr>" +
															"<th>" +
																"<i class='icon-caret-right blue'></i>Name</th>" +
															"<th>" +
																"<i class='icon-caret-right blue'></i>Summary</th>" +
															"<th class='hidden-480'>" +
																"<i class='icon-caret-right blue'></i>Enabled</th>" +
															"<th data-rel='tooltip' data-placement='top' title='Place enable/disable option on the dashboard.'>" +
																"<i class='icon-caret-right blue'></i>Favorite</th>" +
														"</tr>" +
													"</thead>" +
													"<tbody id='wiseup-data-container-sensor-event-list-content'></tbody>" + // events area
												"</table>" +
											"</div>" +
										"</div>" +
									"</div>" +
									"<h4 class='lighter'>" +
										"<i class='icon-hand-right icon-animated-hand-pointer orange'></i>" +
										"<a href='#modal-wizard-event' data-toggle='modal' onclick=\"trigger_event_wizard (!ph_sensor_id!, '!ph_sensor_name!', !ph_sensor_type!, '#wiseup-data-container-events');\" class='pink'> Add new event </a>" +
									"</h4>" +
								"</div>" +
								"<div class='col-sm-6'>" +
									"<div class='widget-box transparent'>" +
										"<div class='widget-header widget-header-flat'>" +
											"<h4 class='lighter'>" +
												"<i class='icon-bell-alt orange2'></i>Timers</h4>" +
										"</div>" +
										"<div class='widget-body'>" +
											"<div class='widget-main no-padding'>" +
												"<table class='table table-bordered table-striped'>" +
													"<thead class='thin-border-bottom'>" +
														"<tr>" +
															"<th><i class='icon-caret-right blue'></i>Name</th>" +
															"<th><i class='icon-caret-right blue'></i>Occurrence</th>" +
															"<th><i class='icon-caret-right blue'></i>Date</th>" +
															"<th><i class='icon-caret-right blue'></i>Time</th>" +
															"<th><i class='icon-caret-right blue'></i>Days</th>" +
															"<th><i class='icon-caret-right blue'></i>Action</th>" +
															"<th colspan=2><i class='icon-caret-right blue'></i>Enabled</th>" +	
														"</tr>" +
													"</thead>" +
													"<tbody id='wiseup-data-container-sensor-timer-list-content'></tbody>" + // timers area
												"</table>" +
											"</div>" +
										"</div>" +
									"</div>" +
									"<h4 class='lighter'>" +
										"<i class='icon-hand-right icon-animated-hand-pointer orange2'></i>" +
										"<a href='#modal-wizard-timer' onclick=\"onclick_add_timer(!ph_sensor_id!,'!ph_sensor_name!');\" data-toggle='modal' class='pink'> Add new timer </a>" +
									"</h4>" +
								"</div>" +
							"</div>" +
							modal_window_event +
							"<script type='text/javascript'>" +
								"$('[data-rel=tooltip]').tooltip();" +
							"</script>" +
						"</div>";
var view_event_row = 	"<tr>" +
							"<td><span class='wiseup_bold_on_hover'>!ph_event_name!</span></td>" +
							"<td class='center'>!ph_event_summary_icon_features!</td>" +
							"<td class='center'><input type='checkbox' class='ace' onclick=\"on_click_enable_handler(!ph_event_enable_script_id!,!ph_event_state!);\" !ph_event_enabled!><span class='lbl'></span></td>" +
							"<td class='center'><input type='checkbox' class='ace' onclick=\"on_click_favorite_handler(!ph_event_enable_script_id!,!ph_event_enable_script_favorite_value!);\" !ph_event_enable_script_favorite!><span class='lbl'></span></td>" +
						"</tr>";
					
var view_timer_row = 	"<tr>" +
							"<td><span class='wiseup_bold_on_hover'>!ph_row_timer_name!</span></td>" +							
							"<td><span class='wiseup_bold_on_hover'>!ph_row_timer_occurrence_type!</span></td>" +
							"<td><span class='wiseup_bold_on_hover'>!ph_row_timer_date!</span></td>" +														
							"<td class='center' ><span class='wiseup_bold_on_hover'>!ph_row_timer_time!</span></td>" +													
							"<td class='center' ><span class='wiseup_bold_on_hover'>!ph_row_timer_occurrence_days!</span></td>" +
							"<td class='center' ><span class='wiseup_bold_on_hover'>!ph_row_timer_action!</span></td>" +							
							"<td class='center'><input type='checkbox' class='ace' onclick=\"on_click_timer_enable_handler(!ph_row_timer_id!,!ph_row_timer_state!);\" !ph_row_timer_enabled!><span class='lbl'></span></td>" +
							"<td class='center'><i class='icon-trash red' onclick=\"on_click_timer_delete_handler(!ph_row_timer_id!);\"></i><span class='lbl'></span></td>" +
						"</tr>";
					/*<input type='checkbox' class='ace' onclick=\"on_click_timer_enable_handler(!ph_row_timer_id!,!ph_row_timer_state!);\" !ph_row_timer_enabled!>*/							
/* UI */
ui_temperature = 	"<div class='infobox infobox-blue'>" +
						"<i class='icon-umbrella wiseup-gray-icon'></i>" +
						"<div class='infobox-data'>" +
							"<span class='wiseup-infobox-data-number viseup-blue-value'>!ph_sensor_web_value! C<sup>0</sup></span>" +
							"<div class='infobox-content'>!ph_sensor_name!</div>" +
						"</div>" +
						"!ph_sensor_footer!" +
					"</div>";
ui_luminance =		"<div class='infobox infobox-blue'>" +
						"<i class='icon-lightbulb wiseup-gray-icon'></i>" +
						"<div class='infobox-data'>" +
							"<span class='wiseup-infobox-data-number viseup-blue-value'>!ph_sensor_web_value! %</span>" +
							"<div class='infobox-content'>!ph_sensor_name!</div>" +
						"</div>" +
						"!ph_sensor_footer!" +
					"</div>";
ui_movement =		"<div class='infobox infobox-blue'>" +
						"!ph_sensor_icon!" +
						"<div class='infobox-data viseup-blue-value'>" +
							"<span class='wiseup-infobox-data-number viseup-blue-value'>!ph_sensor_web_value!</span>" +
							"<div class='infobox-content'>!ph_sensor_name!</div>" +
						"</div>" +
						"!ph_sensor_footer!" +
					"</div>";
ui_switch =			"<div class='infobox infobox-blue'>" +
						"!ph_sensor_icon!" +
						"<div class='infobox-data viseup-blue-value'>" +
							"<span class='wiseup-infobox-data-number viseup-blue-value'>!ph_sensor_web_value!</span>" +
							"<div class='infobox-content'>!ph_sensor_name!</div>" +
						"</div>" +
					"</div>";
ui_magnet =			"<div class='infobox infobox-blue'>" +
						"!ph_sensor_icon!" +
						"<div class='infobox-data viseup-blue-value'>" +
							"<span class='wiseup-infobox-data-number viseup-blue-value'>!ph_sensor_web_value!</span>" +
							"<div class='infobox-content'>!ph_sensor_name!</div>" +
						"</div>" +
						"!ph_sensor_footer!" +
					"</div>";