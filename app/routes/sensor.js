import express from 'express';
import { insertSql } from '../database/sql';

const router = express.Router();

router.get('/', (_req, res) => {
    res.render('sensor', {data: []});
})

router.post('/', (req, res) => {
    const vars = req.body;
    const data = {
        ax: vars.ax,
        ay: vars.ax,
        az: vars.ax,
        gx: vars.ax,
        gy: vars.ax,
        gz: vars.ax,
        decibel: vars.ax,
        temp: vars.ax,
        humi: vars.ax,
    };
    insertSql.setSensor(data);
});

module.exports = router;