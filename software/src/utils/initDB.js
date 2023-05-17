const dbStructure = require('../misc/db-structure.json');
const eventLogger = require('../misc/eventLogger');

module.exports = (db) => {
    let sql = `USE ${dbStructure.defaultDB.dbName}`;     // Does the DB exist?
    db.query(sql, (err) => {
        
        if (err) {
            if (err.code == 'ER_BAD_DB_ERROR') {    // If DB doesn't exist

                eventLogger("MySQL", "WARNING!", `Dabatabese "${dbStructure.defaultDB.dbName}" doesn't exist. Creating "${dbStructure.defaultDB.dbName}" dabatabese...`, "");

                sql = `CREATE DATABASE ${dbStructure.defaultDB.dbName}`;
                db.query(sql, err => {
                    if (err) {
                        eventLogger("MySQL", "ERROR!", `Unable to create "${dbStructure.defaultDB.dbName}" database`, "");
                        throw err;
                    }
                    eventLogger("MySQL", "i", `Database "${dbStructure.defaultDB.dbName}" created`, "");

                    let sql = `USE ${dbStructure.defaultDB.dbName}`;
                    db.query(sql, (err) => {
                        if (err) {
                            eventLogger("MySQL", "ERROR!", `Unable to select "${dbStructure.defaultDB.dbName}" database`, "");
                            throw err;
                        }
                        eventLogger("MySQL", "i", `Default database "${dbStructure.defaultDB.dbName}" selected`, "");
                        selectUserTable();
                    });

                    
                })

            } else {
                eventLogger("MySQL", "ERROR!", `Unable to create "${dbStructure.defaultDB.dbName}" database`, "");
                throw err;
            }

        } else {
            eventLogger("MySQL", "i", `Default database "${dbStructure.defaultDB.dbName}" selected`, "");
            selectUserTable();
        }
    });





    function selectUserTable() {      // Does "users" table exist?
        sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName}`
        db.query(sql, (err) => {
            if (err) {

                if (err.code = 'ER_NO_SUCH_TABLE') {

                    eventLogger("MySQL", "WARNING!", `Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" doesn't exist. Creating "${dbStructure.defaultDB.Users.tableName}" table...`, "");

                    let columns = `${dbStructure.defaultDB.Users.id} int AUTO_INCREMENT UNIQUE NOT NULL,
                                ${dbStructure.defaultDB.Users.mail} VARCHAR(255) UNIQUE NOT NULL,
                                ${dbStructure.defaultDB.Users.name} VARCHAR(255) NOT NULL,
                                ${dbStructure.defaultDB.Users.passwordHash} VARCHAR(255) NOT NULL,
                                ${dbStructure.defaultDB.Users.active} BOOLEAN NOT NULL DEFAULT '0',
                                ${dbStructure.defaultDB.Users.activateUID} VARCHAR(255),
                                PRIMARY KEY (${dbStructure.defaultDB.Users.id})`
                    sql = `CREATE TABLE ${dbStructure.defaultDB.Users.tableName}(${columns})`
                    db.query(sql, err => {
                    if (err) {
                        eventLogger("MySQL", "ERROR!", `Unable to create "${dbStructure.defaultDB.Users.tableName}" table`, "");
                        throw err;
                    }
                        eventLogger("MySQL", "i", `Table "${dbStructure.defaultDB.Users.tableName}" created`, "");
                        selectVathultTable();
                    });

                } else {
                    eventLogger("MySQL", "ERROR!", `Unable to create "${dbStructure.defaultDB.dbName}" database`, "");
                    throw err;
                }

            } else {
                eventLogger("MySQL", "i", `Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" found`, "");
                selectVathultTable();
            }
        });
    }



    function selectVathultTable() {       // Does "vathults" table exist?
        sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName}`
        db.query(sql, (err) => {
            if (err) {

                if (err.code = 'ER_NO_SUCH_TABLE') {

                    eventLogger("MySQL", "WARNING!", `Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" doesn't exist. Creating "${dbStructure.defaultDB.Vathults.tableName}" table...`, "");

                    let columns = `${dbStructure.defaultDB.Vathults.id} int AUTO_INCREMENT UNIQUE NOT NULL,
                                ${dbStructure.defaultDB.Vathults.serialNumber} VARCHAR(255) UNIQUE NOT NULL,
                                ${dbStructure.defaultDB.Vathults.userID} int NOT NULL,
                                ${dbStructure.defaultDB.Vathults.name} VARCHAR(255),
                                ${dbStructure.defaultDB.Vathults.lastSeen} DATETIME,
                                ${dbStructure.defaultDB.Vathults.messages} LONGTEXT,
                                ${dbStructure.defaultDB.Vathults.flow} Int,
                                ${dbStructure.defaultDB.Vathults.waterAlert} BOOLEAN,
                                ${dbStructure.defaultDB.Vathults.input} VARCHAR(255),
                                ${dbStructure.defaultDB.Vathults.output1} BOOLEAN,
                                ${dbStructure.defaultDB.Vathults.output2} BOOLEAN,
                                ${dbStructure.defaultDB.Vathults.output3} BOOLEAN,
                                ${dbStructure.defaultDB.Vathults.output4} BOOLEAN,
                                ${dbStructure.defaultDB.Vathults.wifiStrength} Int,
                                ${dbStructure.defaultDB.Vathults.wifiSSID} VARCHAR(256),
                                PRIMARY KEY (${dbStructure.defaultDB.Vathults.id}),
                                FOREIGN KEY (${dbStructure.defaultDB.Vathults.userID}) REFERENCES ${dbStructure.defaultDB.Users.tableName}(${dbStructure.defaultDB.Users.id}) ON DELETE CASCADE ON UPDATE CASCADE`;
                    sql = `CREATE TABLE ${dbStructure.defaultDB.Vathults.tableName}(${columns})`
                    db.query(sql, err => {
                    if (err) {
                        eventLogger("MySQL", "ERROR!", `Unable to create "${dbStructure.defaultDB.Vathults.tableName}" table`, "");
                        throw err;
                    }
                        eventLogger("MySQL", "i", `Table "${dbStructure.defaultDB.Vathults.tableName}" created`, "");
                    });


                } else {
                    eventLogger("MySQL", "ERROR!", `Unable to create "${dbStructure.defaultDB.dbName}" database`, "");
                    throw err;
                }

            } else {
                eventLogger("MySQL", "i", `Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" found`, "");
            }
        });
    }
}

