const sys 		= require("sys");
const redis 	= require('redis');
const subscribe = redis.createClient();
const publish 	= redis.createClient();

subscribe.subscribe('SENSOR-INFO');
subscribe.on("message", function(channel, message) {
	// Use SSE to update the UI with the sensor value.
	// sys.puts (message);
	publish.publish("SENSOR-ACTION", message);
});
