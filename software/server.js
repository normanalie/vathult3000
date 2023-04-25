if (process.env.NODE_ENV !== 'production') {   // If dev mode enabled, use ".env" file for environment variables and XAMPP for fake MySQL server
    require('dotenv').config();
    eventLogger("SERVER", "WARNING!", "Dev Mode enabled!", "~");
    eventLogger("SERVER", "Dev # i", "Server running on http://localhost:3000", "~")
}


const express = require('express');
const app = express();
const bcrypt = require('bcrypt');
const passport = require('passport');
const flash = require('express-flash');
const session = require('express-session');
const methodOverride = require('method-override');
const mysql = require('mysql');
const mysqlLogID = require('./mysql-log-id.json');
const dbStructure = require('./db-structure.json');

const mqtt = require('mqtt');
const mqttLogID = require('./mqtt-log-id.json');

const LocalStrategy = require('passport-local').Strategy;



const db = mysql.createConnection({        // Init MySQL connection
    host: mysqlLogID.devUser.host,
    user: mysqlLogID.devUser.user,
    password: mysqlLogID.devUser.password
})


db.connect(err => {                        // Connect to MySQL
    if (err) {
        eventLogger("MySQL", "ERROR!", "Unable to connect to MySQL", "");
        throw err
    }
    eventLogger("MySQL", "i", "MySQL connection established", "");
})


defaultDBCheck();                          // Select and check default website DB 


/* MQTT */

const mqttHost = mqttLogID.defaultUser.host
const mqttPort = mqttLogID.defaultUser.port
const mqttClientID = `mqtt_${Math.random().toString(16).slice(3)}`

const mqttConnectURL = `mqtt://${mqttHost}:${mqttPort}`


const mqttApp = mqtt.connect(mqttConnectURL, {
    mqttClientID,
    clean: mqttLogID.defaultUser.clean,
    connectTimeout: mqttLogID.defaultUser.connectTimeout,
    username: mqttLogID.defaultUser.username,
    password: mqttLogID.defaultUser.password,
    reconnectPeriod: mqttLogID.defaultUser.reconnectPeriod,
})

let mqttTopicBase = 'vathult3000';

mqttApp.on('connect', () => {

    eventLogger("MQTT", "i", "MQTT connection established", "");

    const mqttTopic = `${mqttTopicBase}/+`

    mqttApp.subscribe([mqttTopic], (err) => {

        if (err) {
            eventLogger("MQTT", "ERROR!", `Failed to subscribe to topic(s) [${mqttTopic}]`, "");
            throw err;
        }
        eventLogger("MQTT", "i", `Subscribed to topic [${mqttTopic}]`, "");
    });

    mqttApp.publish(`${mqttTopicBase}/TEST`, '(Test) MQTT Boot Message', { qos: 0, retain: false }, (error) => {  //Test message
        if (error) {
            eventLogger("MQTT", "ERROR!", `Failed to send test to topic(s) [${mqttTopic}]`, "");
            console.error(error)
        }
    });
})

mqttApp.on('message', (topic, payload) => {

    let message = payload.toString();
    eventLogger("MQTT", "<--", `[${topic}] ${message}`, "");

    const devicetype = topic.split('/')[0];

    if (devicetype == mqttTopicBase && topic != `${mqttTopicBase}/TEST`) {
        const serialNumber = topic.slice(12);
        message = JSON.parse(message);

        
        let sql = `UPDATE ${dbStructure.defaultDB.Vathults.tableName}
                   SET ${dbStructure.defaultDB.Vathults.flow} = '${message.flow}',
                       ${dbStructure.defaultDB.Vathults.waterAlert} = '${message.water_alert}',
                       ${dbStructure.defaultDB.Vathults.input} = '${message.input}',
                       ${dbStructure.defaultDB.Vathults.output1} = '${message.output1}',
                       ${dbStructure.defaultDB.Vathults.output2} = '${message.output2}',
                       ${dbStructure.defaultDB.Vathults.output3} = '${message.output3}',
                       ${dbStructure.defaultDB.Vathults.output4} = '${message.output4}',
                       ${dbStructure.defaultDB.Vathults.wifiStrength} = '${message.wifi_strength}',
                       ${dbStructure.defaultDB.Vathults.wifiSSID} = '${message.wifi_ssid}',
                       ${dbStructure.defaultDB.Vathults.messages} = '${JSON.stringify(message.messages)}'
                   WHERE ${dbStructure.defaultDB.Vathults.serialNumber} = '${serialNumber}'`;
        
        db.query(sql, (err) => {
            if (err) {
                eventLogger("MySQL", "ERROR!", `Unable to save Vathults datas (SN: ${serialNumber})`, "");
                console.log(err);
            } else {
                eventLogger("MySQL", "i", `Vathults Data successfully registered (SN: ${serialNumber})`, "");
            }
        })
    }
})


// Passport.JS

const authentificateUser = async (email, password, done) => {
    let user;
    let post = { mail: email }
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName} WHERE ${dbStructure.defaultDB.Users.mail} = '${email}'`;
    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "Email search failure", "");
            return console.log(err);
        } else if (results.length == 0) {
            return done(null, false, { message: 'No user with that email' });
        }

        user = { id: results[0].id, name: results[0].name, passwordHash: results[0].password_hash };

        try {

            if (await bcrypt.compare(password, user.passwordHash)) {
                return done(null, user);
            } else {
                return done(null, false, { message: 'Incorrect password' });
            }

        } catch (e) {
            return done(e);
        }
    });
}

passport.use(new LocalStrategy({ usernameField: 'email', passwordField: 'password' }, authentificateUser));

passport.serializeUser((user, done) => done(null, user.id));
passport.deserializeUser((id, done) => {

    let post = { id: id }
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName} WHERE ${dbStructure.defaultDB.Users.id} = '${id}'`;
    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "ID search failure");
            return console.log(err);
        } else if (results.length == 0) {
            return done(null, false);
        }
        return done(null, results[0]);
    });
});


// [END] Passport.JS


app.set('view-engine', 'ejs');
app.use(express.urlencoded({ extended: false }));
app.use(flash());
app.use(session({
    secret: process.env.SESSION_SECRET,
    resave: false,
    saveUninitialized: false
}))
app.use(passport.initialize());
app.use(passport.session());
app.use(methodOverride('_method'));


eventLogger("SERVER", "BOOT", "Server started", "~");





/* HOME PAGE */
app.get('/', (req, res) => {
    res.render('homepage.ejs');
})



/* LOGIN / LOGOUT / REGISTER PART */
app.get('/login', checkNotAuthenticated, (req, res) => {
    res.render('login.ejs');
})

app.post('/login', checkNotAuthenticated, passport.authenticate('local', {   // Log in an user
    successRedirect: '/dashboard',
    failureRedirect: '/login',
    failureFlash: true
}))


app.delete('/logout', (req, res, next) => {  // Logout an user
    req.logOut(err => {
        if (err) { return next(err) }
    });
    res.redirect('/login');
})


app.get('/register', checkNotAuthenticated, (req, res) => {
    res.render('register.ejs');
})

app.post('/register', checkNotAuthenticated, async (req, res) => {      // Register an user

    try {
        const hashedPassword = await bcrypt.hash(req.body.password, 10)

        let columns = `${dbStructure.defaultDB.Users.mail},
                       ${dbStructure.defaultDB.Users.name},
                       ${dbStructure.defaultDB.Users.passwordHash},
                       ${dbStructure.defaultDB.Users.active},
                       ${dbStructure.defaultDB.Users.activateUID}`;

        let values = `'${req.body.email}',
                      '${req.body.name}',
                      '${hashedPassword}',
                      '0',
                      '${generateRandomString(30)}'`;
        let sql = `INSERT INTO ${dbStructure.defaultDB.Users.tableName} (${columns}) VALUES (${values})`;
        db.query(sql, (err) => {
            if (err) {
                eventLogger("MySQL", "ERROR!", "Unable to register new user", "");
                console.log(err);
                res.redirect('/register');
            } else {
                eventLogger("MySQL", "i", `New user "${req.body.name}" successfully registered`, "");
                res.render('confirm-user.ejs', { mail: req.body.email });
            }
        })

    } catch {
        res.redirect('/register');
    }
})

app.get('/confirm-user', checkNotAuthenticated, (req, res) => {
    res.render('confirm-user-without-mail.ejs');
})

app.post('/send-confirmation-mail', checkNotAuthenticated, (req, res) => {  // TODO: [A programmer]
    // Send a confirmation mail
})

app.post('/validate/:userID/:validateID', (req, res) => {      // TODO: [A programmer]
    // Set user's "active" state to "true" in DB

    let post = { id: req.params.userID }
    let validateID = { vid : req.params.validateID }
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName} WHERE ${dbStructure.defaultDB.Users.id} = '${req.params.userID}'`;

    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "User search failure", "");
            res.redirect('/');
            return console.log(err);
        } else {
            const userInfo = results;

            if (userInfo[0].activate_uid == validateID.vid) {

                sql = `UPDATE ${dbStructure.defaultDB.Users.tableName} SET ${dbStructure.defaultDB.Users.active} = '1'
                       WHERE ${dbStructure.defaultDB.Users.id} = '${req.params.userID}'`;

                db.query(sql, async (err, results) => {

                    if (err) {
                        eventLogger("MySQL", "ERROR!", "User activate state set failure", "");
                        res.redirect('/');
                        return console.log(err);
                    } else {
                        res.redirect('/login');   
                    }
                });

            } else {
                eventLogger("MySQL", "ERROR!", "The Validate ID sent and owned don't match", "");
                res.redirect('/');
            }
        }
    });
})

app.get('/dashboard', checkAuthenticated, (req, res) => {
    res.render('dashboard.ejs', { name: req.user.name });
})



/* USER PART */
app.get('/user-profile', checkAuthenticated, (req, res) => {
    res.render('user-profile.ejs', { name: req.user.name });
})

app.get('/change-name', checkAuthenticated, (req, res) => {
    res.render('change-name.ejs', { name: req.user.name });
})

app.get('/change-email', checkAuthenticated, (req, res) => {
    res.render('change-email.ejs', { name: req.user.name });
})

app.get('/change-password', checkAuthenticated, (req, res) => {
    res.render('change-password.ejs', { name: req.user.name });
})

app.post('/save-new-password', checkAuthenticated, (req, res) => {   // TODO: [A programmer]

})

app.post('/save-new-name', checkAuthenticated, (req, res) => {      // TODO: [A programmer]

})

app.post('/save-new-email', checkAuthenticated, (req, res) => {     // TODO: [A programmer]

})



/* DEVICE PART */
app.get('/devices', checkAuthenticated, (req, res) => {      // Show device list


    let vathultList;
    let post = { id: req.user.id }
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName} WHERE ${dbStructure.defaultDB.Vathults.userID} = '${req.user.id}'`;

    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "Devices search failure", "");
            res.redirect('/dashboard');
            return console.log(err);
        } else {
            vathultList = results;
            res.render('devices.ejs', { vathultList: vathultList });
        }
    });
})

app.get('/new-device', checkAuthenticated, (req, res) => {
    res.render('new-device.ejs');
})

app.post('/new-device', checkAuthenticated, (req, res) => {    // Add a new device in the DB

    try {

        let columns = `${dbStructure.defaultDB.Vathults.serialNumber},
                       ${dbStructure.defaultDB.Vathults.userID}
                       ${req.body.deviceName ? `, ${dbStructure.defaultDB.Vathults.name}` : ""}`;

        let values = `'${req.body.serialNumber}',
                      '${req.user.id}'
                      ${req.body.deviceName ? `, '${req.body.deviceName}'` : ""}`;

        let sql = `INSERT INTO ${dbStructure.defaultDB.Vathults.tableName} (${columns}) VALUES (${values})`;
        db.query(sql, (err) => {
            if (err) {
                eventLogger("MySQL", "ERROR!", "Unable to save new Vathults", "");
                console.log(err);
                res.redirect('/new-device');
            } else {
                eventLogger("MySQL", "i", `New Vathults "${req.body.serialNumber}" successfully registered`, "");
                res.redirect('/devices');
            }
        })

    } catch {
        res.redirect('/new-device');
    }

    // Save new device in DB. Set inputs/outputs to "null" (await device's real state)
})

app.post('/device/:deviceID/remove', checkAuthenticated, (req, res) => {    // Delete a device

    let post = { id: req.params.deviceID }
    let sql = `DELETE FROM ${dbStructure.defaultDB.Vathults.tableName} WHERE ${dbStructure.defaultDB.Vathults.id} = '${req.params.deviceID}'`;

    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "Failed to delete the device from the list", "");
            res.redirect('/devices');
            return console.log(err);
        } else {
            eventLogger("MySQL", "i", `Vathults with ID "${req.params.deviceID}" successfully deleted`, "");
            res.redirect('/devices');
        }
    });
})

app.get('/device/:deviceID', checkAuthenticated, (req, res) => {     // Show a device

    let vathultInfo;
    let post = { id: req.params.deviceID }
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName} WHERE ${dbStructure.defaultDB.Vathults.id} = '${req.params.deviceID}'`;

    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "Device info search failure", "");
            res.redirect('/dashboard');
            return console.log(err);
        } else {
            vathultInfo = results;
            res.render('device-info.ejs', { vathultInfo: vathultInfo });
        }
    });
})

app.post('/device/:deviceID/setoutputs/', checkAuthenticated, (req, res) => {  // Update device's values
    
    let vathultInfo;
    let anyChanges = false;
    const infosToChange = ["input", "output1", "output2", "output3", "output4"];
    let newInfo = {
        input: null,
        output1: null,
        output2: null,
        output3: null,
        output4: null
    };


    let sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName} WHERE ${dbStructure.defaultDB.Vathults.id} = '${req.params.deviceID}'`;
    let post = {id: req.params.deviceID};

    db.query(sql, post, async (err, results) => {
        if (err) {
            eventLogger("MySQL", "ERROR!", "Device info search failure", "");
            res.redirect('/dashboard');
            return console.log(err);
        } else {
            vathultInfo = results;
            

            for (i=0; i<infosToChange.length; i++) {     //  Check info sent by the form

                if (infosToChange[i] == "input") {       //  Non boolean values manager (On pourra mettre un Switch a la place si y'en a plus a l'avenir)
                    if (req.body.input != results[0].input && req.body.input != null) {
                        newInfo.input = req.body.input;
                        anyChanges = true;
                    }
                } else if (req.body[infosToChange[i]]) {        // Boolean values manager

                    if (results[0][infosToChange[i]] == 1) {
                        newInfo[infosToChange[i]] = 0;
                    } else {
                        newInfo[infosToChange[i]] = 1;
                    }
                    anyChanges = true;
                }
            };


            if (!anyChanges) {
                res.redirect(`/device/${req.params.deviceID}`);
                return;
            } 

            let sql = `UPDATE ${dbStructure.defaultDB.Vathults.tableName} SET `;

            let infoChanged = 0;
            for (i=0; i< infosToChange.length; i++) {      // SQL query creation
                
                if (newInfo[infosToChange[i]] != null) {

                    if (infoChanged > 0) {
                        sql = sql + ", ";
                    }
                    sql = sql + `${dbStructure.defaultDB.Vathults[infosToChange[i]]} = '${newInfo[infosToChange[i]]}'`;
                    infoChanged++;
                }
            }

            sql = sql + ` WHERE ${dbStructure.defaultDB.Vathults.id} = '${req.params.deviceID}'`
        
            db.query(sql, (err) => {
                if (err) {
                    eventLogger("MySQL", "ERROR!", `Unable to update Vathults datas (SN: ${results[0].serial_number})`, "");
                    console.log(err);
                } else {
                    eventLogger("MySQL", "i", `Vathults Data successfully updated (SN: ${results[0].serial_number})`, "");
                    res.redirect(`/device/${req.params.deviceID}`);
                }
            });
        }
    });
})


/* Erro 404 handler */
app.use((req, res) => {
    res.status(404);
    res.render('error-404.ejs');
    return;
})



function checkAuthenticated(req, res, next) {   // Check if an user is logged
    if (req.isAuthenticated()) {
        return next();
    }

    res.redirect('/login');
}

function checkVerifiedAccount(req, res, next) {
    if (req.user.active == 1) {
        return next();
    }

    res.redirect('/confirm-user');
}

function checkNotAuthenticated(req, res, next) {  // Check if an user isn't logged
    if (req.isAuthenticated()) {
        return res.redirect('/');
    }

    next();
}

function defaultDBCheck() {   // Check if the DB "exists"
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

function eventLogger(source, type, message, borderCharSource) {    // Log structure example : "[2023/01/28 - 12:41:53] ~(Source)~ [Type] - Message"
    const currentDate = new Date;
    const year = `${currentDate.getFullYear()}`;
    const months = `${(currentDate.getMonth()+1) < 10 ? "0" : ""}${currentDate.getMonth()+1}`;
    const date = `${currentDate.getDate() < 10 ? "0" : ""}${currentDate.getDate()}`;
    const hours = `${currentDate.getHours() < 10 ? "0" : ""}${currentDate.getHours()}`;
    const minutes = `${currentDate.getMinutes() < 10 ? "0" : ""}${currentDate.getMinutes()}`;
    const seconds = `${currentDate.getSeconds() < 10 ? "0" : ""}${currentDate.getSeconds()}`;

    console.log(`[${year}/${months}/${date} - ${hours}:${minutes}:${seconds}] ${borderCharSource}(${source})${borderCharSource} [${type}] - ${message}`);
}

function generateRandomString(length) {
    let result = '';
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    const charactersLength = characters.length;

    for (let i = 0; i < length; i++) {
      result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }

    return result;
};

app.listen(3000);  // Voir le site sur "localhost:3000"
