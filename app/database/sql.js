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
        const sql = `select * from user`;
        const [result] = await promisePool.query(sql);
        return result;
    },
}

// insert query
export const insertSql = {
    setUser: async (data) => {
        const sql = `insert into user values (
            "${data.UserId}",  "${data.Password}",  "${data.UserName}", "${data.Email}"
        )`
        console.log(data);
        await promisePool.query(sql);
    },
};