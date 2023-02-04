## Dev environnement
1. Download code and open ./software folder in your workspace
2. Add a .env file
   1. Add ```SESSION_SECRET=developpement_key``` in this file
   2. Save
3. In a shell run  ```npm install -dev``` to install dependencies
4. Install ```XAMPP``` on your computer and run it (https://www.apachefriends.org/fr/index.html)
   1. Click on ```Start``` for ```Apache``` 
   2. Click on ```Start``` for ```MySQL``` 
5. In a shell run ```node init-db``` to initialize the DB, then ```node .``` to start the server
6. Go to ```localhost:3000```