const redis = require('redis');
const chance = require('chance').Chance();

publish = redis.createClient(6379, '127.0.0.1');

var pub = function() {
    var msg = {
        "id": chance.pick(['1108152157441', '1108152157442', '1108152157443', '1108152157444', '1108152157445']),
        "hub": 4328719365,
        "type": chance.integer({
            min: 0,
            max: 3
        }),
        "value": chance.integer({
            min: 0,
            max: 20
        })
    }
    console.log("publishing ", msg);
    publish.publish("SENSOR-INFO", JSON.stringify(msg));
}

setInterval(pub, 2000);
