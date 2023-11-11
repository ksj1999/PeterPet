import express from 'express';
import multer from 'multer';
import { spawn } from 'child_process';
import { insertSql } from '../database/sql';
import path from 'path';

const router = express.Router();
const upload = multer({ dest: 'uploads/' }); // 업로드된 파일을 저장할 위치 설정

const runPythonScript = (imagePaths) => {
  return new Promise((resolve, reject) => {
    const pythonProcess = spawn('python', ['efficientnet.py', ...imagePaths]);
    

    let outputData = '';
    pythonProcess.stdout.on('data', (data) => {
      outputData += data.toString();
    });

    pythonProcess.stdout.on('close', () => {
      resolve(outputData);
    });

    pythonProcess.stderr.on('data', (data) => {
      reject(data.toString());
    });
  });
};

router.get('/', (req, res) => {
    res.render('upload');
});

router.post('/', upload.array('photos', 13), (req, res) => {
    const DogId = req.body.DogId;
    const photos = req.files; // 여러 업로드된 사진 정보

    if (!DogId || !photos || photos.length === 0) {
        return res.status(400).send("DogId and photos are required.");
    }

    const imagePaths = photos.map(photo => photo.path); // 여러 사진의 경로
    imagePaths.forEach(Photo => {
        insertSql.setPhoto(DogId, Photo);
    });

    // Python 스크립트 실행
    runPythonScript(imagePaths)
      .then(output => {
        console.log('Python script output:', output);
        res.render('result', { prediction: output }); // 예측 결과를 결과 페이지에 렌더링
      })
      .catch(error => {
        console.error('Error executing Python script:', error);
        res.status(500).send('Error processing image');
      });
});

export default router;
