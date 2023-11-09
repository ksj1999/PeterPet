CREATE TABLE USER (
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    UserName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    RegDate DATETIME NOT NULL,
    PRIMARY KEY (UserId)
);

CREATE TABLE DOG (
    DogId VARCHAR(45) NOT NULL,
    UserId VARCHAR(45) NOT NULL,
    Breed VARCHAR(45) NULL,
    Gender CHAR NULL,
    Weight FLOAT NULL,
    BCS VARCHAR(20) NULL,
    PRIMARY KEY (DogId), 
    FOREIGN KEY (UserId) REFERENCES USER (UserId)
);

CREATE TABLE BCSIMAGE(
    PhotoId INT AUTO_INCREMENT NOT NULL,
    DogId VARCHAR(45) NOT NULL,
    Photo LONGBLOB NULl,
    PRIMARY KEY (PhotoId), 
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);


CREATE TABLE SENSOR(
    SensTime DATETIME not null,
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
    PRIMARY KEY (SensTime),
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
    PRIMARY KEY (FeedTime),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

Insert into USER values ('yoon8720', 'king01', 'dungdunge', 'yoon8720@nate.com', now());
Insert into USER values ('kaya01', 'kwon0871', 'android369', 'kaya@gmail.com', now());

Insert into DOG values ('dung2', 'yoon8720', 'shihtzu', 'M', 8.7, 'Normal');
Insert into DOG values ('happy', 'yoon8720', 'maltese', 'M', 3.1, 'Obesity');
Insert into DOG values ('bbobbi', 'kaya01', 'shihtzu', 'F', 2.7, 'Underweight');

Insert into BCSIMAGE values(1, 'dung2', null);

Insert into DISPENSER values ('2023-11-02 10:30:00', 'dung2', 40);
Insert into DISPENSER values (now(), 'dung2', 20);

Insert into SENSOR values ('2023-11-02 10:30:00', 'dung2', 1.98, -0.03, -2.67, -4.94, 105.35, -83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values (now(), 'dung2', 2.98, -0.08, 1.67, 2.94, -80.34, -43.65, 62.1, 36.8, 59.1);

Insert into Activity values ('2023-11-02 10:30:00', 'dung2', 0);
Insert into Activity values ('2023-11-02 10:31:00', 'dung2', 2);

Insert into ActKcal values ('2023-11-02 10:31:00', 'dung2', 2, 150);
Insert into ActKcal values (now(), 'happy', 1, 100);
