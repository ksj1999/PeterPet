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

const PORT = 5000;
const app = express();

app.use(express.static(path.join(__dirname, '/src')));
app.use(express.urlencoded({ extended: false })) 
app.use(express.json()); 
app.use(
    expressSession({
        secret: "my key",
        resave: true,
        saveUninitialized: true,
    })
);

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
app.use('/result', resultRouter)

app.use('/sensor', sensorRouter);

app.listen(PORT, ()=> {
    console.log(`Server is running at http://localhost:${PORT}`)
});