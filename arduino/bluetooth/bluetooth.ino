#include <ArduinoBLE.h> 
#include <Arduino_LSM9DS1.h> 

// UUid for Service
const char* UUID_serv = "84582cd0-3df0-4e73-9496-29010d7445dd";
// UUids for accelerometer characteristics (I separated x, y and z values)
const char* UUID_ax   = "84582cd1-3df0-4e73-9496-29010d7445dd";
const char* UUID_ay   = "84582cd2-3df0-4e73-9496-29010d7445dd";
const char* UUID_az   = "84582cd3-3df0-4e73-9496-29010d7445dd";
// UUids for gyroscope characteristics (as above)
const char* UUID_gx   = "84582cd4-3df0-4e73-9496-29010d7445dd";
const char* UUID_gy   = "84582cd5-3df0-4e73-9496-29010d7445dd";
const char* UUID_gz   = "84582cd6-3df0-4e73-9496-29010d7445dd";

// BLE Service
BLEService myService(UUID_serv); 

// BLE Characteristics
BLEFloatCharacteristic  chAX(UUID_ax,  BLERead|BLENotify);
BLEFloatCharacteristic  chAY(UUID_ay,  BLERead|BLENotify);
BLEFloatCharacteristic  chAZ(UUID_az,  BLERead|BLENotify);
BLEFloatCharacteristic  chGX(UUID_gx,  BLERead|BLENotify);
BLEFloatCharacteristic  chGY(UUID_gy,  BLERead|BLENotify);
BLEFloatCharacteristic  chGZ(UUID_gz,  BLERead|BLENotify);

void setup() 
  {
  Serial.begin(115200);
  Serial.println(BLE.address());
  
  uint32_t t=millis();
  while (!Serial) // wait 5 seconds for serial connection
    {
    if ((millis()-t) > 5000) break;
    }
  bool err=false;
  pinMode(LED_BUILTIN, OUTPUT); // onboard led
  digitalWrite(LED_BUILTIN, LOW); // led off

  // init IMU
  if (!IMU.begin()) 
    {
    Serial.println("IMU: failed");
    err=true;
    }
  Serial.println("IMU: ok");

  // init BLE
  if (!BLE.begin()) 
    {
    Serial.println("BLE: failed");
    err=true;
    }
  Serial.println("BLE: ok");

  // error: flash led forever
  if (err)
    {
    Serial.println("Init error. System halted");
    while(1)
      {
      digitalWrite(LED_BUILTIN, HIGH); // led on
      delay(500);
      digitalWrite(LED_BUILTIN, LOW); // led off
      delay(500);  
      } 
    }

  // BLE service
  // correct sequence:
  // set BLE name > advertised service > add characteristics > add service > set initial values > advertise

  // Set BLE name
  BLE.setLocalName("Settorezero_IMU");
  BLE.setDeviceName("Arduino"); // Arduino is the default value on this module
  
  // Set advertised Service
  BLE.setAdvertisedService(myService);
  
  // Add characteristics to the Service
  myService.addCharacteristic(chAX);
  myService.addCharacteristic(chAY);
  myService.addCharacteristic(chAZ);
  myService.addCharacteristic(chGX);
  myService.addCharacteristic(chGY);
  myService.addCharacteristic(chGZ);
  
  // add service to BLE
  BLE.addService(myService);
  
  // characteristics initial values
  chAX.writeValue(0);
  chAY.writeValue(0);
  chAZ.writeValue(0);
  chGX.writeValue(0);
  chGY.writeValue(0);
  chGZ.writeValue(0);

 
  // start advertising
  BLE.advertise();
  Serial.println("Advertising started");
  }

void loop() 
  {
  static long preMillis = 0;
  
  // listen for BLE centrals devices
  BLEDevice central = BLE.central();

  // central device connected?
  if (central) 
    {
    digitalWrite(LED_BUILTIN, HIGH); // turn on the onboard led
    Serial.print("Connected to central: ");
    Serial.println(central.address()); // central device MAC address
    
    // while the central is still connected to peripheral:
    while (central.connected()) 
      {
      
      long curMillis = millis();
      if (preMillis>curMillis) preMillis=0; // millis() rollover?
      if (curMillis - preMillis >= 10) // check values every 10mS
        {
        preMillis = curMillis;
        updateValues(); // call function for updating value to send to central
        }
      } // still here while central connected

    // central disconnected:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    } // no central
}

void updateValues() 
  {
  uint8_t averages=10; // average on this values count (accelerometer and gyroscope)
 
  // accelerometer averaged values/actual values
  static float ax=0;
  static float ay=0;
  static float az=0;
  float ax1, ay1, az1;
  
  // gyroscope averaged values/actual values
  static float gx=0;
  static float gy=0;
  static float gz=0;
  float gx1, gy1, gz1; 
  

  static uint8_t i_a=0; // accelerometer readings counter
  static uint8_t i_g=0; // gyroscope readings counter
 
  // read accelerometer values if available
  if (IMU.accelerationAvailable()) 
    {
    i_a++;
    IMU.readAcceleration(ax1, ay1, az1);
    ax+=ax1;
    ay+=ay1;
    az+=az1;
    if (i_a==averages) // send average over BLE
      {
      ax/=averages;
      ay/=averages;
      az/=averages;
      Serial.println("Accelerometer: "+String(ax)+","+String(ay)+","+String(az));
      chAX.writeValue(ax);
      chAY.writeValue(ay);
      chAZ.writeValue(az); 

      ax=0;
      ay=0;
      az=0;
      i_a=0;
      }
    }

  // read gyroscope values if available
  if (IMU.gyroscopeAvailable())
    {
    i_g++;
    IMU.readGyroscope(gx1, gy1, gz1);
    gx+=gx1;
    gy+=gy1;
    gz+=gz1;
    if (i_g==averages) // send average over BLE
      {
      gx/=averages;
      gy/=averages;
      gz/=averages;
      Serial.println("Gyroscope: "+String(gx)+","+String(gy)+","+String(gz));
      chGX.writeValue(gx);
      chGY.writeValue(gy);
      chGZ.writeValue(gz);
      gx=0;
      gy=0;
      gz=0;
      i_g=0; 
      }
    }
}