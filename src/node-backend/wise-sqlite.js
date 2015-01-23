var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database('wiseupdb.db');
var check;
db.serialize(function() {
                        
    db.run("CREATE TABLE IF NOT EXISTS `sensor_info` (" +
            "`sensor_id`	            UNSIGNEDBIGINT NOT NULL PRIMARY KEY," +
            "`sensor_hub_address`	    UNSIGNEDBIGINT NOT NULL," +
            "`sensor_address`	        SMALLINT NOT NULL," +
            "`sensor_name`	            VARCHAR(128) NOT NULL," +
            "`sensor_family_type`	    SMALLINT NOT NULL," +
            "`registration_datetime`	INTEGER NOT NULL," +
            "`update_interval`	        SMALLINT NOT NULL," +
            "`available`	            TINYINT NOT NULL);");
            
    db.run("CREATE TABLE IF NOT EXISTS `sensor_data_history` (" +
            "`record_id`	            INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," +
            "`sensor_id`	            UNSIGNEDBIGINT NOT NULL," +
            "`time_stamp`	            INTEGER NOT NULL," +
            "`value`	                INTEGER NOT NULL);");
            
    db.run("CREATE TABLE IF NOT EXISTS `sensor_data` (" +
            "`sensor_id`	            UNSIGNEDBIGINT NOT NULL PRIMARY KEY," +
            "`time_stamp`	            UNSIGNEDBIGINT NOT NULL," +
            "`value`	                INTEGER NOT NULL);");
            
    db.run("CREATE TABLE IF NOT EXISTS `sensor_action` (" +
            "`sensor_id`	            UNSIGNEDBIGINT NOT NULL PRIMARY KEY," +
            "`action`	                INTEGER NOT NULL);");
});

db.close();