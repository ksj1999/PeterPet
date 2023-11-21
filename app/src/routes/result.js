import express from "express";

const router = express.Router();

router.get('/', (req, res) => {
    const petId = req.session.petId;
    res.render('result', { petId: petId });
    console.log(petId);
    console.log(req.session);
});


module.exports = router;
