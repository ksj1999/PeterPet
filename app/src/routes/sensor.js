import express from 'express';
import { exec } from 'child_process';
import { ApplyQuery, insertSql, getPetIdFromSensorId } from '../database/sql';

const express = require('express');
const { CalorieCalculator, FeedManager } = require('./feedCalculator'); // Import the new module

const router = express.Router();

function translatePredictionToActivity(prediction) {
    let stop = 0, walk = 0, run = 0;
    if (prediction !== null && !isNaN(prediction)) {
        switch (prediction) {
            case 0:
                stop = 1;
                break;
            case 1:
                walk = 1;
                break;
            case 2:
                run = 1;
                break;
        }
    }
    return [stop, walk, run];
}

router.get('/', (_req, res) => {
    res.render('sensor', { data: [] });
});

router.post('/', async (req, res) => {
    const vars = req.body;
    console.log("Received POST data:", vars);
    let all_data = [];

    try {
        const sensorResult = await ApplyQuery.applyquery(vars.Query);
        all_data.push('Sensor Query:');
        all_data.push(vars.Query);
        all_data.push('Sensor Result:');
        all_data.push(JSON.stringify(sensorResult));

        const regex = /'([^']*)'|\b(\d+\.\d+|\d+)\b/g;
        let matches, values = [];
        while ((matches = regex.exec(vars.Query))) {
            values.push(matches[1] || matches[2]);
        }

        console.log("Extracted values:", values);

        if (values.length !== 10) {
            throw new Error("Incorrect number of arguments extracted from the query.");
        }

        const currentTime = new Date();
        const formattedTime = currentTime.toISOString().replace('T', ' ').substring(0, 19);
        const [sensorId, ax, ay, az, gx, gy, gz, decibel, temp, humi] = values.map(Number);

        const pythonCommand = `python randomf\\randomforest.py ${formattedTime} ${sensorId} ${ax} ${ay} ${az} ${gx} ${gy} ${gz}`;

        exec(pythonCommand, async (error, stdout, stderr) => {
            if (error) {
                console.error(`Execution error: ${error}`);
                all_data.push('Error executing Python script');
                res.render('sensor', { data: all_data });
                return;
            }

            console.log(`Python script raw output: ${stdout}`);
            const predictionMatch = stdout.trim().match(/\d+/);
            const prediction = predictionMatch ? parseInt(predictionMatch[0], 10) : null;
            console.log(`Received prediction: ${prediction}`);

            const [stop, walk, run] = translatePredictionToActivity(prediction);

            const petId = await getPetIdFromSensorId(sensorId);
            if (!petId) {
                all_data.push('No PetId found for given SensorId.');
                res.render('sensor', { data: all_data });
                return;
            }

            try {
                await insertSql.setActivity({
                    PetId: petId,
                    Stop: stop,
                    Walk: walk,
                    Run: run
                });
                all_data.push('Activity data saved to database.');
            } catch (dbError) {
                console.error('Database error:', dbError.message);
                all_data.push('Error saving activity data to database.');
            }

            // Render the response with all the data
            res.render('sensor', { data: all_data });
        });

    } catch (error) {
        console.error('Error:', error.message);
        all_data.push('There was an error.');
        all_data.push('Please check.');
        res.render('sensor', { data: all_data });
    }
});

// New route for calculating feed
router.post('/calculateFeed', (req, res) => {
    const { acttime, dogid, weight, bcs, nowKcal, feed } = req.body;

    const calorieCalculator = new CalorieCalculator(acttime, dogid, weight, bcs);
    const feedManager = new FeedManager(acttime, dogid, weight, bcs, nowKcal);

    const needKcal = Math.round(calorieCalculator.calculateDer(nowKcal)[0], 2);
    const actLevel = calorieCalculator.calculateDer(nowKcal)[1];
    const amount = Math.round(feedManager.dailyAmount(feed), 2);
    const eatKcal = Math.round(amount / frequency, 2); // Make sure 'frequency' is defined or passed

    res.json({ needKcal, actLevel, amount, eatKcal });
});


module.exports = router;