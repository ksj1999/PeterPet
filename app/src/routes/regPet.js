import express from "express";
import { insertSql } from "../database/sql";

const router = express.Router();

router.get('/', (req, res) => {
    const ownerId = req.session.ownerId;
    res.render('regPet', { ownerId: ownerId });
    console.log(ownerId);
});

router.post('/', (req, res) => {
    const vars = req.body;

    const data = {
        DogId: vars.dogid,
        OwnerId: req.session.ownerId,
        Breed: vars.breed,
        Gender: vars.gender,
        Weight: vars.weight,  
        Neuter: vars.Neuter
    };

    insertSql.setDog(data);
    res.redirect('/upload');
});

module.exports = router;