if (process.env.NODE_ENV !== 'production') {   // If dev mode enabled, use ".env" file for environment variables and XAMPP for fake MySQL server
    require('dotenv').config();
    console.log(`~(SERVER)~ [WARNING!] - Dev Mode enabled!`);
    console.log(`~(SERVER)~ [Dev # i] - Website link : "localhost:3000"`);
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
const LocalStrategy = require('passport-local').Strategy;



const db = mysql.createConnection({        // Init MySQL connection
    host: mysqlLogID.devUser.host,
    user: mysqlLogID.devUser.user,
    password: mysqlLogID.devUser.password
})


db.connect(err => {                        // Connect to MySQL
    if (err) {
        console.log(`(MySQL) [ERROR!] - Unable to connect to MySQL`)
        throw err
    }
    console.log(`(MySQL) [i] - MySQL connection established`);
})


defaultDBCheck();                          // Select and check default website DB 



// Passport.JS

const authentificateUser = async (email, password, done) => {
    let user;
    let post = {mail: email}
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName} WHERE ${dbStructure.defaultDB.Users.mail} = '${email}'`;
    db.query(sql, post, async (err, results) => {
        if (err) {
            console.log(`(MySQL) [ERROR!] - Email search failure`);
            return console.log(err);
        } else if (results.length == 0) {
            return done(null, false, { message: 'No user with that email'});
        }

        user = {id: results[0].id, name: results[0].name, passwordHash: results[0].password_hash};
        
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

passport.use(new LocalStrategy ({ usernameField: 'email', passwordField: 'password' }, authentificateUser));

passport.serializeUser((user, done) => done(null, user.id));
passport.deserializeUser((id, done) => {

    let post = {id: id}
    let sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName} WHERE ${dbStructure.defaultDB.Users.id} = '${id}'`;
    db.query(sql, post, async (err, results) => {
        if (err) {
            console.log(`(MySQL) [ERROR!] - ID search failure`);
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
app.use(session( {
    secret: process.env.SESSION_SECRET,
    resave: false,
    saveUninitialized: false
}))
app.use(passport.initialize());
app.use(passport.session());
app.use(methodOverride('_method'));


console.log(`~(SERVER)~ [BOOT] - Server started`)





/* HOME PAGE */
app.get('/', (req, res) => {
    res.render('homepage.ejs');
})



/* LOGIN / LOGOUT / REGISTER PART */
app.get('/login', checkNotAuthenticated, (req, res) => {
    res.render('login.ejs' );
})

app.post('/login', checkNotAuthenticated, passport.authenticate('local', {
    successRedirect: '/dashboard',
    failureRedirect: '/login',
    failureFlash: true
}))


app.delete('/logout', (req, res, next) => {
    req.logOut( err => {
        if (err) {return next(err)}
    });
    res.redirect('/login');
})


app.get('/register', checkNotAuthenticated, (req, res) => {
    res.render('register.ejs' );
})

app.post('/register', checkNotAuthenticated, async (req, res) => {

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
                      'abcd1234'`;
        let sql = `INSERT INTO ${dbStructure.defaultDB.Users.tableName} (${columns}) VALUES (${values})`;
        db.query(sql, (err) => {
            if (err) {
                console.log(`(MySQL) [ERROR!] - Unable to register new user`);
                console.log(err);
                res.redirect('/register');
            } else {
                console.log(`(MySQL) [i] - New user "${req.body.name}" successfully registered`);
                res.redirect('/login');
            }
        })

    } catch {
        res.redirect('/register');
    }
})

app.post('/send-confirmation-mail', checkNotAuthenticated, (req, res) => {  // TODO: [A programmer]
    // Send a confirmation mail
})

app.post('/validate/:user-id/:uid', (req, res) => {      // TODO: [A programmer]
    // Set user's "active" state to "true" in DB
})

app.get('/dashboard', checkAuthenticated, (req, res) => {
    res.render('dashboard.ejs', {name: req.user.name} );
})



/* USER PART */
app.get('/user-profile', checkAuthenticated, (req, res) => {
    res.render('user-profile.ejs', {name: req.user.name} );
})

app.get('/change-name', checkAuthenticated, (req, res) => {
    res.render('change-name.ejs', {name: req.user.name} );
})

app.get('/change-email', checkAuthenticated, (req, res) => {
    res.render('change-email.ejs', {name: req.user.name} );
})

app.get('/change-password', checkAuthenticated, (req, res) => {
    res.render('change-password.ejs', {name: req.user.name} );
})

app.post('/save-new-password', checkAuthenticated, (req, res) => {   // TODO: [A programmer]
    
})

app.post('/save-new-name', checkAuthenticated, (req, res) => {      // TODO: [A programmer]
    
})

app.post('/save-new-email', checkAuthenticated, (req, res) => {     // TODO: [A programmer]
    
})



/* DEVICE PART */
app.get('/devices', checkAuthenticated, (req, res) => {
    res.render('devices.ejs');
})

app.get('/new-device', checkAuthenticated, (req, res) => {
    res.render('new-device.ejs');
})

app.get('/save-new-device', checkAuthenticated, (req, res) => {  // TODO: [A programmer]
    // Save new device in DB. Set inputs/outputs to "null" (await device's real state)
})

app.get('/device/1', checkAuthenticated, (req, res) => {   // TODO: Remplacer le "1" par une variable qui s'adapte en en fonction de l'ID de l'appareil 
    res.render('device-info.ejs');
})

app.post('/device/:device-id>/setoutput/:output-id/:state', checkAuthenticated, (req, res) => {  // TODO: [A programmer par rapport à l'ESP-32]
    // Set device output state
})

app.post('/device/:device-id/setsource/:source', checkAuthenticated, (req, res) => {             // TODO: [A programmer par rapport à l'ESP-32]
    // Set device water source
})



/* Erro 404 handler */
app.use((req, res) => {
    res.status(404);
    res.render('error-404.ejs');
    return;
})



function checkAuthenticated(req, res, next) {
    if (req.isAuthenticated()) {
        return next();
    }

    res.redirect('/login');
}

function checkNotAuthenticated(req, res, next) {
    if (req.isAuthenticated()) {
        return res.redirect('/');
    }

    next();
}

function defaultDBCheck() {
    let sql = `USE ${dbStructure.defaultDB.dbName}`;
    db.query(sql, (err) => {
        if (err) {
            console.log(`(MySQL) [ERROR!] - Default database "${dbStructure.defaultDB.dbName}" not found`);     // Check if Default DB exists
            throw err;
        }
        console.log(`(MySQL) [i] - Default database "${dbStructure.defaultDB.dbName}" selected`);
    });

    sql = `SELECT * FROM ${dbStructure.defaultDB.Users.tableName}`                                             // Check if table "Users" in Default DB exists
    db.query(sql, (err) => {
        if (err) {
            console.log(`(MySQL) [ERROR!] - Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" not found`);
            throw err;
        }
        console.log(`(MySQL) [i] - Table "${dbStructure.defaultDB.Users.tableName}" in "${dbStructure.defaultDB.dbName}" found`);
    });

    sql = `SELECT * FROM ${dbStructure.defaultDB.Vathults.tableName}`                                         // Check if table "Vathults" in Default DB exists
    db.query(sql, (err) => {
        if (err) {
            console.log(`(MySQL) [ERROR!] - Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" not found`);
            throw err;
        }
        console.log(`(MySQL) [i] - Table "${dbStructure.defaultDB.Vathults.tableName}" in "${dbStructure.defaultDB.dbName}" found`);
    });

}

app.listen(3000);  // Voir le site sur "localhost:3000"
