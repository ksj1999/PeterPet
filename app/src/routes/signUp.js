import express from "express";
import { insertSql } from "../database/sql";

const router = express.Router();

router.get('/', (req, res) => {
    res.render('signUp');
});

router.post('/', (req, res) => {
    const vars = req.body;

    const data = {
        UserId: vars.id,
        Password: vars.password,
        UserName: vars.name,
        Email: vars.email
    };

    insertSql.setUser(data);
    res.redirect('/signIn');
});
module.exports = router;