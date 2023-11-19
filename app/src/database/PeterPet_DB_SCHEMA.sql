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

CREATE TABLE SensorData(
    SensorId INT NOT NULL,
    time DATETIME NOT NULL,
    ax FLOAT NULL,
    ay FLOAT NULL,
    az FLOAT NULL,
    gx FLOAT NULL,
    gy FLOAT NULL,
    gz FLOAT NULL,
    decibel FLOAT NULL,
    temp FLOAT NULL,
    humi FLOAT NULL,
    Primary key(SensorId)
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
    DspId INT NOT NULL,
    FeedTime DATETIME not null,
    Amount INT NULL,
    EatKcal Float NUll DEFAULT 0,
    PRIMARY KEY (DspId)
);

CREATE TABLE PetSensorLinks(
    SensorId INT NOT NULL,
    PetId VARCHAR(45) NOT NULL,
    PRIMARY KEY (SensorId),
    FOREIGN KEY (SensorId) REFERENCES SensorData(SensorId),
    FOREIGN KEY (PetId) REFERENCES Pets(PetId)
);


CREATE TABLE PetDispenserLinks(
    DspId INT NOT NULL,
    PetId VARCHAR(45) NOT NULL,
    PRIMARY KEY (DspId),
    FOREIGN KEY (DspId) REFERENCES FoodDispensers(DspId),
    FOREIGN KEY (PetId) REFERENCES Pets(PetId)
);