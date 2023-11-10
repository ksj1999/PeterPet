#include <Wire.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>

#define MIC_PIN A0  // Analog microphone input pin
#define SAMPLE_RATE 16000  // Sampling rate (16kHz)
#define MIC_GAIN 50  // Microphone sensitivity adjustment (16x)
#define BUFFER_SIZE 256  // Sample buffer size

unsigned long lastSampleTime = 0;
int sampleBuffer[BUFFER_SIZE];
float magnitudeBuffer[BUFFER_SIZE / 2];

float ax, ay, az;
float gx, gy, gz;
float temp, humi, decibelValue;

String ssid = "Sy";  
String password = "01075799717"; 
String serverIP = "210.111.178.93";
int serverPort = 80;

bool waitForResponse(const char* expectedResponse, unsigned long timeout, bool printResponse = true) {
  unsigned long startTime = millis();
  String response;
  while (millis() - startTime < timeout) {
    while (Serial1.available()) {
      char c = Serial1.read();
      response += c;
      if (printResponse) {
        Serial.write(c);
      }
      if (response.indexOf(expectedResponse) != -1) {
        return true;
      }
    }
    delay(1000);
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // Start software serial communication with ESP8266
  if (!IMU.begin()) {
    Serial.println("LSM9DSI sensor error!");
    while (1);
  }

  if (!HTS.begin()) {
    Serial.println("HTS221 sensor error!");
    while (1);
  }
  while (!Serial);

  pinMode(MIC_PIN, INPUT);
  analogReadResolution(12);

  Serial1.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  Serial.println("Connecting to WiFi...");

  if (waitForResponse("OK", 10000)) {
    Serial.println("WiFi connected");
  } else {
    Serial.println("WiFi connection timeout");
    while (1);
  }
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();
  lastSampleTime = currentMillis;

  for (int i = 0; i < BUFFER_SIZE; i++) {
    sampleBuffer[i] = analogRead(MIC_PIN);
    delayMicroseconds(1000000 / SAMPLE_RATE);
  }

  for (int i = 0; i < BUFFER_SIZE / 2; i++) {
    magnitudeBuffer[i] = sampleBuffer[i];
  }

  float maxMagnitude = 0;
  for (int i = 0; i < (BUFFER_SIZE / 2); i++) {
    float magnitude = magnitudeBuffer[i];
    if (magnitude > maxMagnitude) {
      maxMagnitude = magnitude;
    }
  }

  float decibelValue = 20 * log10(maxMagnitude);

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }

  temp = HTS.readTemperature();
  humi = HTS.readHumidity();

  String dataToSend = "insert into sensor values (now()," + String(ax) + "," + String(ay) + "," + String(az) +
                      "," + String(gx) + "," + String(gy) + "," + String(gz) +
                      "," + String(decibelValue) + "," + String(temp) + "," + String(humi) + ");";

  Serial.println("Connecting to the web server...");
  Serial1.println("AT+CIPSTART=\"TCP\",\"210.111.178.93\",80");
  Serial.println("AT+CIPSTART=\"TCP\",\"210.111.178.93\",80");

  delay(2000);

  if (Serial1.find("ERROR")) {
    Serial.println("Connection error");
    delay(1000);
    return;
  }

  Serial.println("Sending data...");
  delay(5000);
  Serial1.print("AT+CIPSEND=");
  Serial.print("AT+CIPSEND=");
  int contentLength = dataToSend.length() + 4; // additional newlines and carriage returns
  Serial1.println(contentLength);
  Serial.println(contentLength);
  delay(2000);

  if (Serial1.find(">")) {
    Serial.println("Ready to send");
  } else {
    Serial1.println("AT+CIPCLOSE");
    Serial.println("AT_CIPSEND connect timeout");
    delay(1000);
    return;
  }
  delay(500);

  Serial1.println("POST /home HTTP/1.1");
  Serial1.println("Host: 210.111.178.93");
  Serial1.println("Content-Type: application/x-www-form-urlencoded");
  Serial1.println("Content-Length: " + String(contentLength));
  Serial1.println("\n");
  Serial1.println(dataToSend);

  Serial.println("POST /home HTTP/1.1");
  Serial.println("Host: 210.111.178.93");
  Serial.println("Content-Type: application/x-www-form-urlencoded");
  Serial.println("Content-Length: " + String(contentLength));
  Serial1.println("\n");
  Serial.println(dataToSend);

  delay(5000);
  Serial.println("Data sent. Waiting for response...");

  delay(500); 
  if (Serial1.find("ERROR")) {
    Serial.println("Send error");
    delay(1000);
    return;
    }
  Serial1.println("AT+CIPCLOSE");
  Serial.println("AT+CIPCLOSE");

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
  