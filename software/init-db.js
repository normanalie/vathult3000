const mysql = require('mysql');
const mysqlLogID = require('./mysql-log-id.json');
const dbStructure = require('./db-structure.json');


const db = mysql.createConnection({        // Init MySQL connection
    host: mysqlLogID.devUser.host,
    user: mysqlLogID.devUser.user,
    password: mysqlLogID.devUser.password
})


db.connect(err => {                         // Connect to MySQL
    if (err) {
        console.log(`(MySQL) [ERROR!] - Unable to connect to MySQL`)
        throw err
    }
    console.log(`(MySQL) [i] - MySQL connection established`);
})


process.on('exit', function (code) {     // Say if the code ended sucessfully
    if (code == 0)
        return console.log(`=[END-PROCESS]= Process ended successfully with code ${code}`);
});



let sql = `USE ${dbStructure.defaultDB.dbName}`;     // Does the DB exist?
db.query(sql, (err) => {
    
    if (err) {
        if (err.code == 'ER_BAD_DB_ERROR') {    // If DB doesn't exist

            console.log(`(MySQL) [WARNING!] - Dabatabese "${dbStructure.defaultDB.dbName}" doesn't exist. Creating "${dbStructure.defaultDB.dbName}" dabatabese...`)
    
            sql = `CREATE DATABASE ${dbStructure.defaultDB.dbName}`
            db.query(sql, err => {
                if (err) {
                    console.log(`(MySQL) [ERROR!] - Unable to create "${dbStructure.defaultDB.dbName}" database`)
                    throw err;
                }
                console.log(`(MySQL) [i] - Database "${dbStructure.defaultDB.dbName}" created`);

                let sql = `USE ${dbStructure.defaultDB.dbName}`;
                db.query(sql, (err) => {
                    if (err) {
                        console.log(`(MySQL) [ERROR!] - Unable to select "${dbStructure.defaultDB.dbName}" database`);     // Select DB for next operations
                        throw err;
                    }
                    console.log(`(MySQL) [i] - Default database "${dbStructure.defaultDB.dbName}" selected`);
                    selectUserTable();
                });

                
            })

        } else {
            console.log(`(MySQL) [ERROR!] - Unable to create "${dbStructure.defaultDB.dbName}" database`);
            throw err;
        }

    } else {

        console.log(`(MySQL) [i] - Default database "${dbStructure.defaultDB.dbName}" selected`);
        //console.log(err);
        selectUserTable();
    }
});





function selectUserTable() {      // Does "users" table exist?
    sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName}`
    db.query(sql, (err) => {
        if (err) {

            if (err.code = 'ER_NO_SUCH_TABLE') {

                console.log(`(MySQL) [WARNING!] - Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" doesn't exist. Creating "${dbStructure.defaultDB.Users.tableName}" table...`);

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
                    console.log(`(MySQL) [ERROR!] - Unable to create "${dbStructure.defaultDB.Users.tableName}" table`)
                    throw err;
                }
                    console.log(`(MySQL) [i] - Table "${dbStructure.defaultDB.Users.tableName}" created`);
                    selectVathultTable();
                });

            } else {
                console.log(`(MySQL) [ERROR!] - Unable to create "${dbStructure.defaultDB.dbName}" database`);
                throw err;
            }

        } else {
            console.log(`(MySQL) [i] - Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" found`);
            selectVathultTable();
        }
    });
}



function selectVathultTable() {       // Does "vathults" table exist?
    sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName}`
    db.query(sql, (err) => {
        if (err) {

            if (err.code = 'ER_NO_SUCH_TABLE') {

                console.log(`(MySQL) [WARNING!] - Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" doesn't exist. Creating "${dbStructure.defaultDB.Vathults.tableName}" table...`);

                let columns = `${dbStructure.defaultDB.Vathults.id} int AUTO_INCREMENT UNIQUE NOT NULL,
                               ${dbStructure.defaultDB.Vathults.serialNumber} VARCHAR(255) UNIQUE NOT NULL,
                               ${dbStructure.defaultDB.Vathults.userID} int NOT NULL,
                               ${dbStructure.defaultDB.Vathults.name} VARCHAR(255) NOT NULL,
                               ${dbStructure.defaultDB.Vathults.lastSeen} DATETIME,
                               ${dbStructure.defaultDB.Vathults.errors} int,
                               ${dbStructure.defaultDB.Vathults.pump} BOOLEAN,
                               ${dbStructure.defaultDB.Vathults.input1} int,
                               ${dbStructure.defaultDB.Vathults.input2} int,
                               ${dbStructure.defaultDB.Vathults.output1} BOOLEAN,
                               ${dbStructure.defaultDB.Vathults.output2} BOOLEAN,
                               ${dbStructure.defaultDB.Vathults.output3} BOOLEAN,
                               ${dbStructure.defaultDB.Vathults.output4} BOOLEAN,
                               PRIMARY KEY (${dbStructure.defaultDB.Vathults.id})`;
                sql = `CREATE TABLE ${dbStructure.defaultDB.Vathults.tableName}(${columns})`
                db.query(sql, err => {
                if (err) {
                    console.log(`(MySQL) [ERROR!] - Unable to create "${dbStructure.defaultDB.Vathults.tableName}" table`)
                    throw err;
                }
                    console.log(`(MySQL) [i] - Table "${dbStructure.defaultDB.Vathults.tableName}" created`);
                    exitProcess(0);
                });


            } else {
                console.log(`(MySQL) [ERROR!] - Unable to create "${dbStructure.defaultDB.dbName}" database`);
                throw err;
            }

        } else {
            console.log(`(MySQL) [i] - Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" found`);
            exitProcess(0);
        }
    });
}


// End "init-db" process if everything ended successfully
function exitProcess(isFailure) {
    process.exit(isFailure)
}
