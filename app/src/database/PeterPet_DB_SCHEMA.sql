CREATE TABLE Users (
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    UserName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    RegDate DATETIME,
    PRIMARY KEY (UserId)
);

CREATE TABLE Owners(
    OwnerId VARCHAR(45) NOT NULL,
    UserId VARCHAR(45) NOT NULL,
    Password VARCHAR(45) NOT NULL,
    OwnerName VARCHAR(45) NOT NULL,
    Email VARCHAR(45) NOT NULL,
    RegDate DATETIME,
    PRIMARY KEY (OwnerId),
    FOREIGN KEY(UserId) REFERENCES Users(UserId)
);

CREATE TABLE Pets(
    PetId VARCHAR(45) NOT NULL,
    OwnerId VARCHAR(45) NOT NULL,
    Breed VARCHAR(45) NULL,
    Gender CHAR NULL,
    Weight FLOAT NULL,
    Neuter boolean null, 
    BCS VARCHAR(20) NULL,
    PRIMARY KEY (PetId), 
    FOREIGN KEY (OwnerId) REFERENCES Owners(OwnerId)
);

CREATE TABLE PetSensorLinks(
    SensorId INT NOT NULL,
    PetId VARCHAR(45) NOT NULL,
    PRIMARY KEY (SensorId),
    FOREIGN KEY (PetId) REFERENCES Pets(PetId)
);


CREATE TABLE PetDispenserLinks(
    DspId INT NOT NULL,
    PetId VARCHAR(45) NOT NULL,
    PRIMARY KEY (DspId),
    FOREIGN KEY (PetId) REFERENCES Pets(PetId)
);

CREATE TABLE SensorData(
    time DATETIME NOT NULL,
    SensorId INT NOT NULL,
    ax FLOAT NULL,
    ay FLOAT NULL,
    az FLOAT NULL,
    gx FLOAT NULL,
    gy FLOAT NULL,
    gz FLOAT NULL,
    decibel FLOAT NULL,
    temp FLOAT NULL,
    humi FLOAT NULL,
    Primary key(time),
    FOREIGN KEY (SensorId) REFERENCES PetSensorLinks(SensorId)
);


CREATE TABLE PetActivities(
    ActTime DATETIME NOT NULL,
    PetId VARCHAR(45) NOT NULL,
    Stop INT NULL DEFAULT 0,
	Walk INT NULL DEFAULT 0,
	Run INT NULL DEFAULT 0,
	Kcal FLOAT NULL DEFAULT 0,
    PRIMARY KEY (ActTime),
    FOREIGN KEY (PetId) REFERENCES Pets(PetId)
);

CREATE TABLE PetCalorieNeeds(
    time DATETIME NOT NULL,
	PetId VARCHAR(45) NOT NULL,
    ActLevel INT NOT NULL,
    NeedKcal FLOAT NULL DEFAULT 0,
    PRIMARY KEY (time),
    FOREIGN KEY (PetId) REFERENCES Pets(PetId)
);

CREATE TABLE FoodDispensers(
    FeedTime DATETIME not null,
    Amount INT NULL,
    EatKcal Float NUll DEFAULT 0,
    DspId INT NOT NULL,
    Primary Key(FeedTime),
    FOREIGN KEY (DspId) REFERENCES PetDispenserLinks(DspId)
);



-- insert into PetSensorLinks values (
--     1,
--     'dung2'
-- );
