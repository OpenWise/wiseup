const redis = require('redis');
const events = require('events');
const moment = require('moment');
var console;
//{id:1108152157441,hub:4328719365,type:1,value:20}
function RedisAdapter(sensorChannel, port, host, options) {
    this.subscribe = redis.createClient(port, host, options);
    this.publish = redis.createClient(port, host, options);
    this.client = redis.createClient(port, host, options);
    events.EventEmitter.call(this);

    //this.eventEmitter = new events.EventEmitter();
    this.subscribe.subscribe(sensorChannel);
    var self = this;
    this.subscribe.on("message", function(channel, message) {
        switch (channel) {
            case sensorChannel:
                var j = JSON.parse(message);
                j.ts = moment().format();
                console.log("redis adapter received message from channel: " + channel + " " + message);
                self.client.set(j.id, j.value, function(err, reply) {
                    if (err) {
                        console.error(err.message);
                    } else {
                        console.log("OK: redis set " + j.id);
                    }
                });
                self.emit(j.id, j.value);
                self.emit(sensorChannel, j);
                break;
        }
    });
    console.info("redis adapter subscribed for channel " + sensorChannel);
}

RedisAdapter.prototype.GetSensorValue = function(sid, callback) {
    this.client.get("" + sid, callback);
}

RedisAdapter.prototype.__proto__ = events.EventEmitter.prototype;

function RedisFactory(c) {
    console = c;
    return RedisAdapter;
}

module.exports = RedisFactory;
