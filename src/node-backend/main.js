const express = require('express');

const LIGHTS = 1;
const TEMPERATURE = 2;

var rand = function(max) {
    return Math.floor(Math.random() * max);
}

var Pubsub = function() {
    this.sensorValues = {};
    this.listeners = {};
    this.subscriberId = 0;
    this.publish = function() {
        //for each subscriber - notify of the new value of the sensor
        for (subscriber in this.listeners) {
            console.log("publish to subscriber " + subscriber);
            var sid = this.listeners[subscriber].id;
            if (!this.sensorValues[sid]) {
                //if this sensor id is requested the first time generate an initial random value
                this.sensorValues[sid] = rand(100);
            }
            this.listeners[subscriber].invoke("{ sid: " + sid + ", value: " + this.sensorValues[sid] + " }");
        }

        //increment sensor values by random and publish after 2 seconds
        var self = this;
        setTimeout(function() {
            for (k in self.sensorValues) {
                self.sensorValues[k] += rand(10);
            }
            self.publish();
        }, 2000);
    };
    this.subscribe = function(sid, callback) {
        //subscriberId - subscriber id for use in unsubscribe
        //id - the sensor to subscribe to
        //invoke - the callback function
        this.listeners[this.subscriberId] = {
            id: sid,
            invoke: callback
        };
        return this.subscriberId++;
    };
    this.unsubscribe = function(id) {
        delete this.listeners[id];
    };
    this.publish();
}

var app = express();

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE');
    next();
});

var apiRouter = express.Router();
var sseRouter = express.Router();

app.use('/api', apiRouter);
app.use('/sse', sseRouter);

apiRouter.route('/').get(function(req, res) {
    res.end('hello sse - this is the rest api base url');
});

var db = {};
db.sensors = {
    1: {
        id: 1,
        name: "leaving room lights",
        type: LIGHTS
    },
    2: {
        id: 2,
        name: "leaving room temprature",
        type: TEMPERATURE
    },
    3: {
        id: 3,
        name: "kitchen lights",
        type: LIGHTS
    }
};

apiRouter.route('/sensors').get(function(req, res) {
    res.json(db.sensors);
});

apiRouter.route('/sensors:id').get(function(req, res) {
    var sensor = db.sensors[req.params.id];
    if (sensor) {
        res.json(sensor);
    } else {
        res.status(500).send("no such sensor id");
    }
});

var pubsub = new Pubsub();
var msgid = 0;

sseRouter.route('/sensor/:id').get(function(req, res) {
    console.log("received new listener for sensor id " + req.params.id);
    req.socket.setTimeout(Infinity);
    res.writeHead(200, {
        'Content-Type': 'text/event-stream',
        'Cache-Control': 'no-cache',
        'Connection': 'keep-alive'
    });
    res.write('\n');
    var subid = pubsub.subscribe(req.params.id, function(data) {
        console.log("received event: " + data + "\n");
        res.write("id: " + msgid++ +"\n");
        res.write("data: " + data);
        res.write("\n\n");
    });
    req.on("close", function() {
        pubsub.unsubscribe(subid);
    });
});

var server = app.listen(5678, function() {
    console.log('Listening on port %d', server.address().port);
});

process.on('uncaughtException', function(err) {
    console.log(err);
});
