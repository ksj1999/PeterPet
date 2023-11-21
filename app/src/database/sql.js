import mysql from 'mysql2';
import { spawn } from 'child_process';


require("dotenv").config();

const pool = mysql.createPool({
    host: '210.111.178.93',
    port: 3306,
    user: 'dungdung2',
    password: 'advantechlove1234',
    database: 'PeterPet',
});

const promisePool = pool.promise();

// Function to get PetId from SensorId
export const getPetIdFromSensorId = async (sensorId) => {
    const query = `SELECT PetId FROM PetSensorLinks WHERE SensorId = ${sensorId}`;
    const [rows] = await promisePool.query(query);
    return rows.length > 0 ? rows[0].PetId : null;
};

//total applyquery
export const ApplyQuery = {
    applyquery: async(Query) => {
        const sql = Query;
        const[result] = await promisePool.query(sql);
        return result;
    },
}

//select query
export const selectSql = {
    getUser: async () => {
        const sql = `select * from users`;
        const [result] = await promisePool.query(sql);
        return result;
    },
    getOwner: async() => {
        const sql = `select * from owners`;
        const [result] = await promisePool.query(sql);
        return result;
    },
    getPetWeight: async (petId) => {
        const sql = `SELECT Weight FROM Pets WHERE PetId = '${petId}'`;
        const [result] = await promisePool.query(sql);
        return result.length > 0 ? result[0].Weight : null;
    },
    getLastActivityTime: async (petId) => {
        const sql = `SELECT MAX(ActTime) as LastActivityTime FROM PetActivities WHERE PetId = '${petId}'`;
        const [result] = await promisePool.query(sql);
        return result.length > 0 && result[0].LastActivityTime ? result[0].LastActivityTime : null;
    },

    getPetDetails: async (petId) => {
        const sql = `SELECT Weight, Neuter, BCS FROM Pets WHERE PetId = '${petId}'`;
        const [result] = await promisePool.query(sql);
        return result.length > 0 ? result[0] : null;
    },

    getKcalSumLast12Hours: async (petId) => {
        const sql = `SELECT SUM(Kcal) as TotalKcal FROM PetActivities WHERE PetId = '${petId}' AND ActTime >= NOW() - INTERVAL 12 HOUR`;
        const [result] = await promisePool.query(sql);
        const totalKcal = result.length > 0 && result[0].TotalKcal ? result[0].TotalKcal : 0;
        console.log(`Total Kcal for Pet ID ${petId} in the last 12 hours: ${totalKcal}`);
        return totalKcal;
    },
    getAllPets: async () => {
        const sql = `SELECT PetId, Weight, BCS FROM Pets`;
        const [result] = await promisePool.query(sql);
        return result;
    },
    
}

// insert query
export const insertSql = {
    setUser:async (data) => {
        const sql = `insert into users values (
            "${data.UserId}",  "${data.Password}",  "${data.UserName}", "${data.Email}", now()
        )`
        console.log(data);
        await promisePool.query(sql);
    },
    setOwner: async(data) => {
        const sql = `insert into owners values (
            "${data.OwnerId}", "${data.UserId}",  "${data.Password}", 
             "${data.OwnerName}", "${data.Email}", now()
        )`
        console.log(data);
        await promisePool.query(sql);
    },
    setSensor:async (data) => {
        const sql = `insert into sensordata values ( 1,
            now(), ${data.ax}, ${data.ay},${data.az},
            ${data.gx}, ${data.gy}, ${data.gz},
            ${data.decibel}, ${data.temp}, ${data.humi}
        )`
        console.log(data);
        await promisePool.query(sql);
    },

    setDog: async (data) => {
        const sql = `INSERT INTO Pets VALUES (?, ?, ?, ?, ?,?, NULL)`;
        const values = [data.PetId, data.OwnerId,  data.Breed, data.Gender,  data.Weight, data.Neuter];
        console.log(data);
        await promisePool.query(sql, values);
    },
    setFoodDispenserAmount: async (data) => {
        const sql = `INSERT INTO FoodDispensers (FeedTime, Amount, DspId) VALUES (NOW(), ?, ?)`;
        await promisePool.query(sql, [data.Amount, data.DspId]);
    },
    setFoodDispenserAmount: async (data) => {
        const sql = `INSERT INTO FoodDispensers (FeedTime, Amount, DspId) VALUES (NOW(), ?, ?)`;
        await promisePool.query(sql, [data.Amount, data.DspId]);
        
        // Execute the Python script after the SQL query
        const pythonProcess = spawn('python', ['./fooddispenser/fooddisp.py', data.PetId, data.Amount, data.DspId]);

        pythonProcess.stdout.on('data', (data) => {
            console.log(`stdout: ${data}`);
        });

        pythonProcess.stderr.on('data', (data) => {
            console.error(`stderr: ${data}`);
        });

        pythonProcess.on('close', (code) => {
            console.log(`child process exited with code ${code}`);
        });
    },
    

    

    //Insert into Activity values ('2023-11-06 10:30:00', 'dung2', 0, 1, 0, 1);
    setActivity: async (data) => {
            const sql = `insert into PetActivities values (
                now(), '${data.PetId}', ${data.Stop},${data.Walk},
                ${data.Run}, NULL
            )`
            console.log(data);
            console.log(sql);
            await promisePool.query(sql);
        },
};

export const updateSql = {
    updatePet: async (data) => {
        const sql = 'UPDATE Pets SET BCS = ? WHERE PetId = ?';
        const values = [data.prediction, data.PetId];
        console.log(data);
        await promisePool.query(sql, values);
    },

    updateKcal: async (petId, kcal) => {
        const sql = 'UPDATE PetActivities SET Kcal = ? WHERE PetId = ?';
        const values = [kcal, petId];
        await promisePool.query(sql, values);
    },
    
};