CREATE TABLE USER (
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    UserName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    PRIMARY KEY (UserId)
);

CREATE TABLE DOG (
    DogId INT NOT NULL,
    UserId VARCHAR(45) NOT NULL,
    DogName VARCHAR(45) NULL,
    Breed VARCHAR(45) NULL,
    Gender CHAR NULL,
    Weight FLOAT NULL,
    BCS VARCHAR(20) NULL,
    PRIMARY KEY (DogId), 
    FOREIGN KEY (UserId) REFERENCES USER (UserId)
);

CREATE TABLE BCSIMAGE(
    PhotoId INT NOT NULL,
    DogId INT NOT NULL,
    Photo BLOB NULl,
    PRIMARY KEY (PhotoId), 
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

CREATE TABLE  DISPENSER (
    FeedTime DATETIME not null,
    DogId INT NOT NULL,
    Amount int null,
    PRIMARY KEY (FeedTime),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);


CREATE TABLE SENSOR(
    SensTime DATETIME not null,
    DogId int not null, 
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
    FOREIGN KEY (DogId) REFERENCES Dog (DogId)
);

CREATE TABLE Activity (
    ActTime DATETIME NOT NULL,
    DogId INT NOT NULL,
    Type INT NULL DEFAULT 0,
    PRIMARY KEY (ActTime),
    FOREIGN KEY (DogId) REFERENCES DOG (DogId)
);

ALTER TABLE BCSIMAGE MODIFY PhotoId INT AUTO_INCREMENT;
ALTER TABLE BCSIMAGE MODIFY Photo LONGBLOB;


Insert into USER values ( 'yoon8720', 'king01', 'dungdunge', 'yoon8720@nate.com');
Insert into USER values ( 'kaya01', 'kwon0871', 'android369', 'kaya@gmail.com');

Insert into DOG values ( 200, 'yoon8720', 'dung_dung_e', 'shihtzu', 'M', 8.7, 'Normal');
Insert into DOG values ( 201, 'yoon8720', 'happy', 'maltese', 'M', 3.1, 'Obesity');
Insert into DOG values ( 202, 'kaya01', 'bbobbi', 'shihtzu', 'F', 2.7, 'Underweight');

Insert into BCSIMAGE values( 1, 200, null);

Insert into DISPENSER values ( '2023-11-02 10:30:00', 200, 40 );
Insert into DISPENSER values ( now(), 201, 20 );

Insert into SENSOR values ('2023-11-02 10:30:00', 200, 1.98, -0.03, -2.67, -4.94,105.35,-83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-02 10:31:00', 200, 2.98, -0.08, 1.67, 2.94,-80.34,-43.65, 62.1, 36.8, 59.1);

Insert into Activity values ('2023-11-02 10:30:00', 200, 0);
Insert into Activity values ('2023-11-02 10:31:00', 200, 2);