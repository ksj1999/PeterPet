import express from "express";
import { insertSql } from "../database/sql";

const router = express.Router();

router.get('/', (req, res) => {
    res.render('regPet', { data: " " });
});

router.post('/', (req, res) => {
    const vars = req.body;

    const data = {
        DogId: vars.dogid,
        UserId: vars.userid,
        DogName: vars.dogname,
        Breed: vars.breed,
        Gender: vars.gender,
        Weight: vars.weight,  
    };

    insertSql.setDog(data);
    res.redirect('/upload');
});

module.exports = router;