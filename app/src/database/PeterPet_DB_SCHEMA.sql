CREATE TABLE User (
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    UserName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    RegDate DATETIME,
    PRIMARY KEY (UserId)
);

CREATE TABLE Owner (
    OwnerId VARCHAR(45) NOT NULL,
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    OwnerName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    RegDate DATETIME,
    PRIMARY KEY (OwnerId),
    FOREIGN KEY(UserId) REFERENCES USER (UserId)
);

CREATE TABLE DOG (
    DogId VARCHAR(45) NOT NULL,
    OwnerId VARCHAR(45) NOT NULL,
    Breed VARCHAR(45) NULL,
    Gender CHAR NULL,
    Weight FLOAT NULL,
    Neuter boolean null, 
    BCS VARCHAR(20) NULL,
    PRIMARY KEY (DogId), 
    FOREIGN KEY (OwnerId) REFERENCES Owner(OwnerId)
);

CREATE TABLE BCSIMAGE(
    PhotoId INT AUTO_INCREMENT NOT NULL,
    DogId VARCHAR(45) NOT NULL,
    Photo LONGBLOB NULl,
    PRIMARY KEY (PhotoId), 
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);


CREATE TABLE SENSOR(
    time DATETIME not null,
    DogId VARCHAR(45) NOT NULL,
    ax FLOAT NULL,
    ay FLOAT NULL,
    az FLOAT NULL,
    gx FLOAT NULL,
    gy FLOAT NULL,
    gz FLOAT NULL,
    decibel FLOAT NULL,
    temp FLOAT NULL,
    humi FLOAT NULL,
    PRIMARY KEY (time),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

CREATE TABLE Activity (
    ActTime DATETIME NOT NULL,
    DogId VARCHAR(45) NOT NULL,
    Stop INT NULL DEFAULT 0,
	Walk INT NULL DEFAULT 0,
	Run INT NULL DEFAULT 0,
	Kcal FLOAT NULL DEFAULT 0,
    PRIMARY KEY (ActTime),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

CREATE TABLE ActKcal(
    Date DATE NOT NULL,
	DogId VARCHAR(45) NOT NULL,
    ActLevel INT NOT NULL,
    NeedKcal FLOAT NULL DEFAULT 0,
    PRIMARY KEY (Date),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

CREATE TABLE  DISPENSER (
    FeedTime DATETIME not null,
    DogId VARCHAR(45) NOT NULL,
    Amount INT NULL,
    EatKcal Float NUll DEFAULT 0,
    PRIMARY KEY (FeedTime),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

Insert into USER values ('COMPANY_A', '1234', 'MinSeon-Kim', 'yoon8720@nate.com', now());
Insert into USER values ('COMPANY_B', '1234', 'SeoYoung-Yun', 'kaya@gmail.com', now());

Insert into Owner values ('kaya', 'COMPANY_A', '1234', 'SeongJun-Kwon', 'alm1young@gmail.com', now());
Insert into Owner values ('dungdung2', 'COMPANY_A', '1234', 'SeoYoung-Yun', 's38ounn@gmail.com', now());
Insert into Owner values ('bbaro', 'COMPANY_A', '1234', 'HyeonJi-Park', 'amslji@gmail.com', now());

Insert into DOG values ('dung2', 'kaya', 'shihtzu', 'M', 8.7, true, 'Normal');
Insert into DOG values ('happy', 'dungdung2', 'maltese', 'M', 3.1, true, 'Obesity');
Insert into DOG values ('bbobbi', 'bbaro', 'shihtzu', 'F', 2.7, false,'Underweight');

Insert into BCSIMAGE values(1, 'dung2', null);

Insert into DISPENSER values ('2023-11-02 10:30:00', 'dung2', 40, 200);
Insert into DISPENSER values ('2023-11-02 22:30:00', 'dung2', 40, 200);
Insert into DISPENSER values ('2023-11-03 10:30:00', 'dung2', 20, 100);
Insert into DISPENSER values ('2023-11-03 22:30:00', 'dung2', 40, 200);
Insert into DISPENSER values ('2023-11-04 10:30:00', 'dung2', 20, 100);
Insert into DISPENSER values ('2023-11-04 22:30:00', 'dung2', 30, 150);
Insert into DISPENSER values ('2023-11-05 10:30:00', 'dung2', 10, 50);
Insert into DISPENSER values ('2023-11-05 22:30:00', 'dung2', 40, 200);
Insert into DISPENSER values ('2023-11-06 10:30:00', 'dung2', 40, 200);

Insert into SENSOR values ('2023-11-06 10:30:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:31:00', 'dung2', 2.98, -0.08, 1.67, 2.94, -80.34, -43.65, 62.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:32:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:33:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:34:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:35:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:36:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:37:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:38:00', 'dung2', 2.98, -0.08, 1.67, 2.94, -80.34, -43.65, 62.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:39:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:40:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:41:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:42:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:43:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:44:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:45:00', 'dung2', 2.98, -0.08, 1.67, 2.94, -80.34, -43.65, 62.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:46:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:47:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-06 10:48:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);

Insert into Activity values ('2023-11-06 10:30:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:31:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:32:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:33:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:34:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:35:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:36:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:37:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:38:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:39:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:40:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:41:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:42:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:43:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:44:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:45:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:46:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:47:00', 'dung2', 0, 0, 1, 10);
Insert into Activity values ('2023-11-06 10:48:00', 'dung2', 0, 1, 0, 1);
Insert into Activity values ('2023-11-06 10:49:00', 'dung2', 1, 0, 0, 0);
Insert into Activity values ('2023-11-06 10:50:00', 'dung2', 1, 0, 0, 0);
Insert into Activity values ('2023-11-06 10:51:00', 'dung2', 1, 0, 0, 0);
Insert into Activity values ('2023-11-06 10:52:00', 'dung2', 1, 0, 0, 0);

Insert into ActKcal values ('2023-10-29 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-10-30 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-10-31 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-11-01 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-11-02 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-11-03 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-11-04 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-11-05 10:30:00', 'dung2', 2, 150);
Insert into ActKcal values ('2023-11-06 10:30:00', 'dung2', 2, 150);
