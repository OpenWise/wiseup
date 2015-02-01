var endpoint_graph = 'endpoint/graphs.php';

/* UI METHODS */
function generate_graph_sensor_ui (view_context, sensor_id) {
	get_graph_data_by_sensor_id (sensor_id, function (data) {
		
	});
}
	
/* SELECT METHODS */
function get_graph_data_by_sensor_id (sensor_id, fn_ptr) {
	var json = new Object();
	var json_string = "";
	
	json.method = "get_graph_data_by_sensor_id";
	json.sensor_id = sensor_id;
	json_string = JSON.stringify(json);
	
	$.post(endpoint_event, json_string, fn_ptr);
}

/* UPDATE METHODS */