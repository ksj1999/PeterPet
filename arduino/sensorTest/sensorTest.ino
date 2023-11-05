#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>
float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float Temperature, Humidity;

void setup() {
  Serial.begin(9600);
  if (!IMU.begin()) { //LSM9DSI센서 시작
    Serial.println("LSM9DSI센서 오류!");
    while (1);
  }
  
  if (!HTS.begin()) { //HTS221센서 시작
    Serial.println("HTS221센서 오류!");
    while (1);
  }
}
void loop() {
  //가속도센서
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel_x, accel_y, accel_z);
    Serial.print("가속도 센서 = "); Serial.print(accel_x); Serial.print(", "); Serial.print(accel_y); Serial.print(", "); Serial.println(accel_z);
  }
  //자이로센서
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyro_x, gyro_y, gyro_z);
    Serial.print("자이로 센서 = "); Serial.print(gyro_x); Serial.print(", "); Serial.print(gyro_y); Serial.print(", "); Serial.println(gyro_z);
  }
 //온도센서
  Temperature = HTS.readTemperature();
  Serial.print("온도센서 = "); Serial.println(Temperature);
  //습도센서
  Humidity = HTS.readHumidity();
  Serial.print("습도센서 = "); Serial.println(Humidity);
  Serial.println("----------------------------------------------------");
  delay(2000);
}
