import mysql from 'mysql2';

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
    }
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
        const sanitizedPrediction = data.prediction.replace(/\r?\n/g, ''); // Remove \r and \n
        const sql = 'UPDATE Pets SET BCS = ? WHERE PetId = ?';
        const values = [sanitizedPrediction, data.PetId];
        console.log(data);
        await promisePool.query(sql, values);
    },
};
