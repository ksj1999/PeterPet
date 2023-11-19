import express from "express";
import { insertSql } from "../database/sql";

const router = express.Router();

router.get('/', (req, res) => {
    const ownerId = req.session && req.session.ownerId;

    res.render('regPet', { ownerId: ownerId });
    console.log(ownerId);
    
});

router.post('/', (req, res) => {
    const vars = req.body;
    const neuterValue = parseInt(vars.neuter, 10);
    console.log(neuterValue);

    const data = {
        PetId: vars.petId,
        OwnerId: req.session && req.session.ownerId,
        Breed: vars.breed,
        Gender: vars.gender,
        Weight: vars.weight,  
        Neuter: neuterValue
    };

    req.session.petId = vars.petId;
    insertSql.setDog(data);
    res.redirect('/qr');
});

module.exports = router;