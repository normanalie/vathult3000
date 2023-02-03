## Dev environnement
1. Download code and open ./software folder in your workspace
2. Add a .env file
   1. Add ```SESSION_SECRET=developpement_key``` in this file
   2. Save
3. In a shell run  ```npm install -dev``` to install dependencies
4. Install ```XAMPP``` on your computer (https://www.apachefriends.org/fr/index.html)
   1. Click on ```Start``` for ```Apache``` 
   2. Click on ```Start``` for ```MySQL``` 
   3. Click on ```Admin``` for ```MySQL``` to open phpMyAdmin
5. Create a DB called ```vathult3000``` in phpMyAdmin and create the ```Users``` table by following the same structure in the ```db-structure.json``` file 
6. In a shell run ```node .``` to start the server
7. Go to ```localhost:3000```