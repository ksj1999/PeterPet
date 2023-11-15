import express from "express";

const router = express.Router();

router.get('/', (req, res) => {
    const userId = req.session.userId;
    res.render('userMain', { userId: userId });
    console.log(userId);
});

module.exports = router;
