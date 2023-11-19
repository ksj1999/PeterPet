import express from 'express';
import { exec } from 'child_process';
import { ApplyQuery } from '../database/sql';

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
        // Execute the sensor data insertion query
        const sensorResult = await ApplyQuery.applyquery(vars.Query);
        all_data.push('Sensor Query:');
        all_data.push(vars.Query);
        all_data.push('Sensor Result:');
        all_data.push(JSON.stringify(sensorResult));

        // Extract values from the Query string
        const regex = /'([^']*)'|\b(\d+\.\d+|\d+)\b/g;
        let matches, values = [];
        while ((matches = regex.exec(vars.Query))) {
            values.push(matches[1] || matches[2]);
        }

        console.log("Extracted values:", values);

        // Ensure you have the correct number of arguments
        if (values.length !== 11) {
            throw new Error("Incorrect number of arguments extracted from the query.");
        }

        // 현재 시간을 YYYY-MM-DD HH:MM:SS 형식으로 변환
        const currentTime = new Date();
        const formattedTime = currentTime.toISOString().replace('T', ' ').substring(0, 19);

        // Extract values from the values array
        const [time, SensorId, ax, ay, az, gx, gy, gz, decibel, temp, humi] = values;

        // Construct the command with arguments for the Python script
        const pythonCommand = `python randomf\\randomforest.py ${formattedTime} ${SensorId} ${ax} ${ay} ${az} ${gx} ${gy} ${gz}`;

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

            const data = {
                PetId: PetId,
                Stop: stop,
                Walk: walk,
                Run: run
            };

            all_data.push(`Python script output: ${stdout}`);
            res.render('sensor', { data: all_data });
        });

    } catch (error) {
        console.error('Error:', error.message);
        all_data.push('There was an error.');
        all_data.push('Please check.');
        res.render('sensor', { data: all_data });
    }
});

module.exports = router;
