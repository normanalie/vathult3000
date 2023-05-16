/*
 *  - Check if the current DB is operational  
 */

const dbStructure = require('../misc/db-structure.json');
const eventLogger = require('../misc/eventLogger');

module.exports = (db) => {

    let sql = `USE ${dbStructure.defaultDB.dbName}`;
    db.query(sql, (err) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", `Default database "${dbStructure.defaultDB.dbName}" not found`, "");     // Check if Default DB exists
            throw err;
        }
        eventLogger("MySQL", "i", `Default database "${dbStructure.defaultDB.dbName}" selected`, "");
    });

    sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName}`                                             // Check if table "Users" in Default DB exists
    db.query(sql, (err) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", `Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" not found`, "");
            throw err;
        }
        eventLogger("MySQL", "i", `Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" found`, "");
    });

    sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName}`                                         // Check if table "Vathults" in Default DB exists
    db.query(sql, (err) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", `Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" not found`, "");
            throw err;
        }
        eventLogger("MySQL", "i", `Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" found`, "")
    });
}