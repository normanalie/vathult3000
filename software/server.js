if (process.env.NODE_ENV !== 'production') {   // Si on est en mode développement, utiliser le fichier .env pour variables de l'environnement
    require('dotenv').config();
}

const express = require('express');
const app = express();
const bcrypt = require('bcrypt');
const passport = require('passport');
const flash = require('express-flash');
const session = require('express-session');
const methodOverride = require('method-override');


const initializePassport = require('./passport-config');
initializePassport(
    passport,
    email => users.find(user => user.email === email),
    id => users.find(user => user.id === id)
);

const users = [];   // A remplacer par la base SQL pour les utilisateurs


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



app.get('/', (req, res) => {      //
    res.render('homepage.ejs');
})


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
        users.push({
            id: Date.now().toString(),
            name: req.body.name,
            email: req.body.email,
            password: hashedPassword
        });
        res.redirect('/login');
    } catch {
        res.redirect('/register');
    }
})

app.post('/validate/<user_id>/<uid>', (req, res) => {      // EMPTY [A programmer]

})

app.get('/dashboard', checkAuthenticated, (req, res) => {
    res.render('dashboard.ejs', {name: req.user.name} );
})


app.get('/devices', checkAuthenticated, (req, res) => {
    res.render('devices.ejs');
})

app.get('/device/1', checkAuthenticated, (req, res) => {   // Remplacer le "1" par une variable qui s'adapte en en fonction de l'ID de l'appareil 
    res.render('device-info.ejs');
})

app.post('/device/<device_id>/setoutput/<output_id>/<state>', checkAuthenticated, (req, res) => {  // EMPTY [A programmer par rapport à l'ESP]

})

app.post('/device/<device_id>/setsource/<source>', checkAuthenticated, (req, res) => {             // EMPTY [A programmer par rapport à l'ESP]

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

app.listen(3000);  // Voir le site sur "localhost:3000"
