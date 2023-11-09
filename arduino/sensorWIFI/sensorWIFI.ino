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
float temp, humi, decibel;

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
    delay(10);
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Connecting to WiFi...");

  Serial1.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");

  if (waitForResponse("OK", 10000)) {
    Serial.println("WiFi connected");
  } else {
    Serial.println("WiFi connection timeout");
    while (1);
  }
  delay(1000);
}

void loop() {

  //decibel
    unsigned long currentMillis = millis();
    // 1초에 한 번만 샘플링
      lastSampleTime = currentMillis;

      // 아날로그 입력으로부터 샘플링
      for (int i = 0; i < BUFFER_SIZE; i++) {
        sampleBuffer[i] = analogRead(MIC_PIN);
        delayMicroseconds(1000000 / SAMPLE_RATE);
      }

      // FFT를 사용하여 주파수 영역으로 변환 -> 결과 데이터는 magnitudeBuffer에 저장됨
      for (int i = 0; i < BUFFER_SIZE / 2; i++) {
        magnitudeBuffer[i] = sampleBuffer[i];
      }

      // 주파수 스펙트럼의 주파수 및 데시벨 출력
      float maxMagnitude = 0;
      for (int i = 0; i < (BUFFER_SIZE / 2); i++) {
        float magnitude = magnitudeBuffer[i];
        if (magnitude > maxMagnitude) {
          maxMagnitude = magnitude;
        }
      }

      // 데시벨 값 계산
      float decibel = 20 * log10(maxMagnitude);
  //가속도센서
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }
  //자이로센서
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }
 //온도센서
  temp = HTS.readTemperature();

  //습도센서
  humi = HTS.readHumidity();


  Serial.println("Connecting to web server...");
  Serial1.println("AT+CIPSTART=\"TCP\",\"210.111.178.93\",80");
  delay(2000);

  if (Serial1.find("ERROR")) {
    Serial.println("Connect error");
    delay(1000);
    return;
  }

  Serial.println("Send data...");

  String query = "Query=INSERT%20INTO%20sensor%20VALUES%20(now(),%20" + String(ax, 2) +
                ",%20" + String(ay, 2) +
                ",%20" + String(az, 2) +
                ",%20" + String(gx, 2) +
                ",%20" + String(gy, 2) +
                ",%20" + String(gz, 2) +
                ",%20" + String(decibel, 2) +
                ",%20" + String(temp, 2) +
                ",%20" + String(humi, 2) +
                ");";
                
  int contentLength = query.length();

  Serial1.print("AT+CIPSEND=");
  Serial1.println(contentLength + 124); // Adjust as needed

  delay(2000);

  if (Serial1.find(">")) {
    Serial.println("Ready to send");
  } else {
    Serial1.println("AT+CIPCLOSE");
    Serial.println("AT_CIPSEND connect timeout");
    delay(1000);
    return;
  }

  delay(5000);

  Serial1.println("POST /home HTTP/1.1");
  Serial1.println("Host: 210.111.178.93");
  Serial1.println("Content-Type: application/x-www-form-urlencoded");
  Serial1.print("Content-Length: ");
  Serial1.println(contentLength);
  Serial1.println("");
  Serial1.println(query);

  Serial.println("Data sent. Waiting for response...");

  delay(5000);

  if (Serial1.find("ERROR")) {
    Serial.println("Send error");
    delay(1000);
    return;
  }

  while (Serial1.available()) {
    char response = Serial1.read();
    Serial.write(response);
    if (response == '\r') {
      Serial.print('\n');
    }
  }

  Serial1.println("AT+CIPCLOSE");
  Serial.println("\n==================================\n");
  delay(2000);
}
