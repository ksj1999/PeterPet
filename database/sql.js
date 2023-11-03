import mysql from 'mysql2';

require("dotenv").config();

const pool = mysql.createPool({
    host: 'localhost',
    port: 3306,
    user: 'root',
    password: 'gging00100!',
    database: 'peterpet',
});

const promisePool = pool.promise();

export const selectSql = {
    getUser: async () => {
        const sql = `select * from user`;
        const [result] = await promisePool.query(sql);
        return result;
    },
}

export const deleteSql = {
    deleteUser: async (data) => {
        console.log('delete user UserId =', data);
        const sql = `delete from User where UserId=${data.UserId}`
        console.log(sql);
        await promisePool.query(sql);
    },
};