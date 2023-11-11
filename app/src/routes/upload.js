import express from "express";
import multer from "multer";
import { insertSql } from "../database/sql";

const router = express.Router();

const storage = multer.memoryStorage();
const upload = multer({ storage: storage });

router.get('/', (req, res) => {
    res.render('upload', { data: " " });
});


router.post("/", upload.array("photos", 13), (req, res) => {
    const DogId = req.body.DogId;
    const photos = req.files;

    if (!DogId || !photos) {
        return res.status(400).send("DogId and photos are required.");
    }

    photos.forEach((photo) => {
        const Photo = photo.buffer;
        insertSql.setPhoto(DogId, Photo);
    });

    res.redirect('/');
});

export default router;
