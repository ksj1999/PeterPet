import express from 'express';
import { selectSql } from '../database/sql';

const router = express.Router();

router.get('/', async (req, res) => {
    if (req.session.user == undefined) {
        res.redirect('/')
    } else {
        const user = await selectSql.getUser();
        res.render('select', {
            title: "User",
            user,
        })
    };
});

module.exports = router;
