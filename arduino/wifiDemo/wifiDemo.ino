#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>

float temp, humi;
float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;

String ssid = "U+Net3906";
String PASSWORD = "1C10006256";
String host = "192.168.219.100:8787";

void connectWifi() {
  String join = "AT+CWJAP=\"" + ssid + "\",\"" + PASSWORD + "\"";
  Serial.println("Connect Wifi...");
  Serial1.println(join);
  delay(10000);
  if (Serial1.find("OK")) {
    Serial.println("WIFI connect");
  } else {
    Serial.println("connect timeout");
  }
  delay(1000);
}

void httpclient(String char_input) {
  delay(100);
  Serial.println("connect TCP...");
  Serial1.println("AT+CIPSTART=\"TCP\",\"" + host + "\",8787");
  delay(500);
  if (Serial1.find("ERROR")) return;

  Serial.println("Send data...");
  String url = char_input;
  String cmd = "GET /process.php?temp=" + url + " HTTP/1.0\r\n\r\n";
  Serial1.print("AT+CIPSEND=");
  Serial1.println(cmd.length());
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if (Serial1.find(">")) {
    Serial.println(">");
  } else {
    Serial1.println("AT+CIPCLOSE");
    Serial.println("connect timeout");
    delay(1000);
    return;
  }
  delay(500);

  Serial1.println(cmd);
  Serial.println(cmd);
  delay(100);
  if (Serial1.find("ERROR")) return;
  Serial1.println("AT+CIPCLOSE");
  delay(100);
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  connectWifi();
  delay(500);

  if (!IMU.begin()) { // LSM9DSI sensor initialization
    Serial.println("LSM9DSI sensor error!");
    while (1);
  }

  if (!HTS.begin()) { // HTS221 sensor initialization
    Serial.println("HTS221 sensor error!");
    while (1);
  }
}

void loop() {
  temp = HTS.readTemperature();
  humi = HTS.readHumidity();
  String str_output = String(temp) + "&humi=" + String(humi);
  delay(1000);
  httpclient(str_output);
  delay(1000);

  while (Serial1.available()) {
    char response = Serial1.read();
    Serial.write(response);
    if (response == '\r') {
      Serial.print('\n');
    }
  }
  Serial.println("\n==================================\n");
  delay(2000);
}
