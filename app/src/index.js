import express from 'express';
import logger from 'morgan';
import path from 'path';
import expressSession from "express-session";

import loginRouter from '../routes/login';
import sensorRouter from '../routes/sensor';
import regPetRouter from '../routes/regPet';
import uploadRouter from '../routes/upload';

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

app.set('views', path.join(__dirname, '../views'));
app.set('view engine', 'hbs');

app.use(logger('dev'));

app.use('/', loginRouter);
app.use('/regPet', regPetRouter);
app.use('/upload', uploadRouter);
app.use('/sensor', sensorRouter);

app.listen(PORT, ()=> {
    console.log(`Server is running at http://localhost:${PORT}`)
});