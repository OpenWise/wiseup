var mongodb = require('mongodb');
var console;

function MongoAdapter(host, db) {
    var self = this;
    mongodb.MongoClient.connect("mongodb://" + host + "/" + db, function(err, database) {
        if (err) {
            console.error(err.message);
            throw err;
        }
        console.info("connected to mongodb on " + host + "/" + db);
        self.db = database;
    });
}

MongoAdapter.prototype.GetAllSensors = function(callback) {
    this.db.collection('sensors').find({}, function(err, cursor) {
        if (err) {
            callback(err, null);
        } else {
            cursor.toArray(function(err, arr) {
                if (err) {
                    callback(err, null);
                } else {
                    callback(null, arr);
                }
            });
        }
    });
}

MongoAdapter.prototype.WriteSensorValue = function(sensor, callback) {
    this.db.collection('sensors').update({
        _id: sensor.id
    }, {
        $set: {
            hub: sensor.hub,
            type: sensor.type
        },
        $push: {
            values: {
                $each: [{
                    value: sensor.value,
                    ts: sensor.ts
                }],
                $sort: {
                    ts: -1
                },
                $slice: 10
            }
        }
    }, {
        upsert: true
    }, function(err, modified, status) {
        if (err) {
            console.error("error occured in mongo update for sensor " + sensor + ": " + err);
            if (callback) {
                callback(err);
            }
        } else {
            console.log("sucess in mongo update: modified " + modified + ", status: " + JSON.stringify(status));
            if (callback) {
                callback();
            }
        }
    });
}



function MongoFactory(c) {
    console = c;
    return MongoAdapter;
}

module.exports = MongoFactory;
