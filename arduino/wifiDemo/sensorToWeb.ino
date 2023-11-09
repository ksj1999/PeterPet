#include <Wire.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>

#define MIC_PIN A0  // 아날로그 마이크 입력 핀
#define SAMPLE_RATE 16000  // 샘플링 속도 (16kHz)
#define MIC_GAIN 50  // 마이크 민감도 조정 가능 (16배)
#define BUFFER_SIZE 256  // 샘플 버퍼 크기

unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 1000;  // 1초에 한 번
int sampleBuffer[BUFFER_SIZE];
float magnitudeBuffer[BUFFER_SIZE / 2];

float ax, ay, az;
float gx, gy, gz;
float temp, humi, decibelValue;

String ssid = "Sy";    // WiFi 네트워크 이름
String password = "01075799717";  // WiFi 네트워크 비밀번호
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
  Serial1.begin(9600);  // ESP8266와 소프트웨어 시리얼 통신 시작
  if (!IMU.begin()) { //LSM9DSI센서 시작
    Serial.println("LSM9DSI센서 오류!");
    while (1);
  }
  
  if (!HTS.begin()) { //HTS221센서 시작
    Serial.println("HTS221센서 오류!");
    while (1);
  }
  while (!Serial);

  // 아날로그 입력 설정
  pinMode(MIC_PIN, INPUT);

  // 아날로그 입력 해상도 설정 (12비트)
  analogReadResolution(12);


  // ESP8266 모듈을 WiFi에 연결
  Serial1.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  Serial.println("Connecting to WiFi...");

  // WiFi 연결 대기
  if (waitForResponse("OK", 10000)) {
    Serial.println("WIFI connected");
  } else {
    Serial.println("WIFI connection timeout");
    while (1);
  }
  delay(1000);
}

void loop() {
  //decibel
    unsigned long currentMillis = millis();
    // 1초에 한 번만 샘플링
    if (currentMillis - lastSampleTime >= sampleInterval) {
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
      float decibelValue = 20 * log10(maxMagnitude);
    }
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

  String dataToSend = "ax=" + String(ax) + "&ay=" + String(ay) + "&az=" +String(az) 
  + "&gx=" + String(gx) + "&gy=" + String(gy)+ "&gz=" + String(gz)
  + "&decibel=" + String(decibelValue) + "&temp=" + String(temp) + "&humi=" + String(humi);


  Serial.println("Connecting to web server...");
  Serial1.println("AT+CIPSTART=\"TCP\",\"" + serverIP + "\"," + serverPort);
  delay(2000);
  if (Serial1.find("ERROR")) {
    Serial.println("Connect error");
    delay(1000);
    return;
  }
  Serial1.println("AT+CIPSTATUS");

  Serial.println("Send data...");
  delay(5000); 
  Serial1.print("AT+CIPSEND=");
  Serial1.println(dataToSend.length());
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

  Serial1.print("POST / HTTP/1.1\r\n");
  Serial1.print("Host: " + serverIP + "\r\n");
  Serial1.print("Content-Type: application/x-www-form-urlencoded\r\n");
  Serial1.print("Content-Length: " + String(dataToSend.length()) + "\r\n");
  Serial1.print("\r\n");
  Serial1.print(dataToSend);

  Serial.println(dataToSend);

  delay(500);  // 추가: AT+CIPCLOSE 전에 잠시 기다리기
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
