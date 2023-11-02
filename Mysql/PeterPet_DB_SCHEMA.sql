CREATE TABLE USER (
    Id INT NOT NULL,
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    UserName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    PRIMARY KEY (Id)
);

CREATE TABLE DOG (
    DogId INT NOT NULL,
    UId INT NOT NULL,
    DogName VARCHAR(45) NULL,
    Breed VARCHAR(45) NULL,
    Gender CHAR NULL,
    Weight FLOAT NULL,
    Photo BLOB NULL,
    PRIMARY KEY (DogId), 
    FOREIGN KEY (UId) REFERENCES USER (Id)
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

Insert into USER values ( 100, 'yoon8720', 'king01', 'dungdunge', 'yoon8720@nate.com');
Insert into USER values ( 101, 'kaya01', 'kwon0871', 'android369', 'kaya@gmail.com');

Insert into DOG values ( 200, 100, 'dung_dung_e', 'shihtzu', 'M', 8.7, NULL);
Insert into DOG values ( 201, 100, 'happy', 'maltese', 'M', 3.1, NULL);
Insert into DOG values ( 202, 101, 'bbobbi', 'shihtzu', 'F', 2.7, NULL);

Insert into DISPENSER values ( '2023-11-02 10:30:00', 200, 40 );
Insert into DISPENSER values ( now(), 201, 20 );

Insert into SENSOR values ('2023-11-02 10:30:00', 200, 1.98, -0.03, -2.67, -4.94,105.35,-83.68, 52.1, 36.8, 59.1);
Insert into SENSOR values ('2023-11-02 10:31:00', 200, 2.98, -0.08, 1.67, 2.94,-80.34,-43.65, 62.1, 36.8, 59.1);

Insert into Activity values ('2023-11-02 10:30:00', 200, 0);
Insert into Activity values ('2023-11-02 10:31:00', 200, 2);