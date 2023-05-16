/*
 *  - Initialize Passport.js
 */

const eventLogger = require('../misc/eventLogger');
const dbStructure = require('../misc/db-structure.json');
const LocalStrategy = require('passport-local').Strategy;
const bcrypt = require('bcrypt');
const passport = require('passport');


module.exports = (db) => {
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
}