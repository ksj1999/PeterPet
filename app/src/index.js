import express from 'express';
import logger from 'morgan';
import path from 'path';
import expressSession from "express-session";

import homeRouter from './routes/home';
import signUpRouter from './routes/signUp';
import signInAdminRouter from './routes/signInAdmin';
import signInOwnerRouter from './routes/signInOwner';
import ownerDashRouter from './routes/ownerDash';

import userMainRouter from './routes/userMain';
import sensorRouter from './routes/sensor';
import regOwnerRouter from './routes/regOwner';
import regPetRouter from './routes/regPet';
import uploadRouter from './routes/upload';
import resultRouter from './routes/result';

import qrRouter from './routes/qr';
import cron from 'node-cron';
import { CalorieCalculator, FeedManager } from './routes/feedCalculator'; // adjust the path if necessary
import { selectSql } from './database/sql'; // Adjust the path according to your project structure
import { insertSql } from './database/sql'; // Adjust the path according to your project structure


const expressSanitizer = require("express-sanitizer");

// fs and https 모듈 가져오기
const https = require("https");
const fs = require("fs");

// certificate와 private key 가져오기
// ------------------- STEP 2
const options = {
  key: fs.readFileSync("./src/config/cert.key"),
  cert: fs.readFileSync("./src/config/cert.crt"),
};

const PORT = 8000;
const app = express();

app.use(express.static(path.join(__dirname, '/src')));
app.use(express.urlencoded({ extended: true })) 
app.use(express.json()); 
app.use(
    expressSession({
        secret: "my key",
        resave: true,
        saveUninitialized: true,
    })
);
app.use(expressSanitizer());

app.set('views', path.join(__dirname, './views'));
app.set('view engine', 'hbs');

app.use(express.static(path.join(__dirname, 'public')));

app.use(logger('dev'));

app.use('/', homeRouter);
app.use('/signUp', signUpRouter);
app.use('/signIn', signInAdminRouter);
app.use('/signInOwner', signInOwnerRouter);
app.use('/ownerDash',ownerDashRouter);

app.use('/userMain', userMainRouter);
app.use('/regOwner', regOwnerRouter);
app.use('/regPet', regPetRouter);
app.use('/upload', uploadRouter);
app.use('/result', resultRouter);

app.use('/sensor', sensorRouter);

app.use('/qr', qrRouter);

// Schedule task to run at 10:50:30 AM and PM every day 
// sec, min, hour
cron.schedule('59 24 23,11 * * *', async () => {
  console.log('Starting scheduled feed calculation...');

  try {
      const pets = await selectSql.getAllPets(); // Make sure this method exists
      console.log(`Found ${pets.length} pets for feed calculation.`);

      for (const pet of pets) {
          console.log(`Calculating feed for Pet ID: ${pet.PetId}, Weight: ${pet.Weight}, BCS: ${pet.BCS}`);
    
          const nowKcal = await selectSql.getKcalSumLast12Hours(pet.PetId);
          console.log(`Now Kcal for Pet ID: ${pet.PetId}: ${nowKcal}`);
    
          const calorieCalculator = new CalorieCalculator(new Date(), pet.PetId, pet.Weight, pet.BCS);
          const der = calorieCalculator.calculateDer(nowKcal)[0]; // Assuming calculateDer returns [der, actLevel]

          const feedEnergyContent = 3785; // kcal/g, adjust as necessary
          const feedManager = new FeedManager(new Date(), pet.PetId, pet.Weight, pet.BCS, der);
          const amount = feedManager.dailyAmount(feedEnergyContent);
          console.log(`Calculated feed amount for Pet ID ${pet.PetId}: ${amount} grams`);

          await insertSql.setFoodDispenserAmount({ PetId: pet.PetId, Amount: amount }); // Adjust this method in your insertSql object
          console.log(`Feed amount for Pet ID ${pet.PetId} saved to FoodDispensers table.`);
      }

      console.log('Scheduled feed calculation completed successfully.');
  } catch (error) {
      console.error('Error in feed calculation cron job:', error);
  }
});

// http 서버는 8000번 포트로 실행
app.listen(PORT, () => {
    console.log(`Server started on port ${PORT}`);
  });
  
  // https 의존성으로 certificate와 private key로 새로운 서버를 시작
  https.createServer(options, app).listen(5000, () => {
    console.log(`HTTPS server started on port 5000`);
  });