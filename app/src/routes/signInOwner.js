import express from "express";
import { selectSql } from "../database/sql";

const router = express.Router();

router.get('/', (req, res) => {
    res.render('signIn');
});

router.post('/', async (req, res) => {
    const vars = req.body;
    
    console.log(req.body);
    console.log(req.session);
    const owners = await selectSql.getOwner();

    owners.map((owner) => {
        console.log('Owner ID :', owner.OwnerId);
        console.log('Owner Password: ', owner.Password);
        console.log(' ');
        if (vars.id == owner.OwnerId && vars.password == owner.Password) {
            console.log('login success!');
            req.session.owner = { id: owner.OwnerId, role : 'owner', checkLogin: true };
            req.session.OwnerId = owner.OwnerId;
        }
    });


    if (req.session.owner == undefined) {
        console.log('login failed!');
        res.send(`<script>
                    alert('login failed!');
                    location.href='/';
                </script>`)
    } else if (req.session.owner.checkLogin ) {
        res.redirect('/sensor');
    } 
});

module.exports = router;