import express from "express";
import { insertSql } from "../database/sql";

const router = express.Router();

router.get('/', (req, res) => {
    const userId = req.session.userId;
    res.render('regPet', { userId: userId });
    console.log(userId);
});

router.post('/', (req, res) => {
    const vars = req.body;

    const data = {
        DogId: vars.dogid,
        UserId: req.session.userId,
        Breed: vars.breed,
        Gender: vars.gender,
        Weight: vars.weight,  
    };

    insertSql.setDog(data);
    res.redirect('/upload');
});

module.exports = router;