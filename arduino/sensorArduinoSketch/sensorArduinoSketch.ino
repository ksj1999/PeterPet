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
float temp, humi;
float decibel;

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
  delay(1000);
  Serial.begin(9600);  // 시리얼 통신 초기화
  Serial1.begin(9600);
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
  //마이크 센서  
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
    decibel = 20 * log10(maxMagnitude);
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
  delay(2000);

  Serial.println("Connecting to web server...");
  Serial1.println("AT+CIPSTART=\"TCP\",\"" + serverIP + "\"," + String(serverPort));

    if (waitForResponse("OK", 10000)) {
    Serial.println("TCP connected");
  } else {
    Serial.println("TCP connection timeout");
    Serial1.println("AT+CIPCLOSE");
    delay(1000);
    return;
  }
  delay(2000);
  
  Serial.println("----------------------------------------------------");

  Serial.println("Send data...");

  String query = "Query=INSERT%20INTO%20SensorData%20VALUES%20(now(),1,%20" + String(ax, 2) +
                 "," + String(ay, 2) +
                 "," + String(az, 2) +
                 "," + String(gx, 2) +
                 "," + String(gy, 2) +
                 "," + String(gz, 2) +
                 "," + String(decibel, 2) +
                 "," + String(temp, 2) +
                 "," + String(humi, 2) +
                 ");";

  Serial.println(query);
  int contentLength = query.length();
  Serial.println(contentLength);


  Serial1.print("AT+CIPSEND=");
  Serial1.println(contentLength + 124); // Adjust as needed

  delay(2000);

  if (waitForResponse(">", 10000)) {
   Serial.println("Ready to send");
  } else {

    delay(1000);
    return;
  }

  Serial.println("----------------------------------------------------");

  delay(5000);

  Serial1.println("POST /sensor HTTP/1.1");
  Serial1.println("Host: " + serverIP);
  Serial1.println("Content-Type: application/x-www-form-urlencoded");
  Serial1.print("Content-Length: ");
  Serial1.println(contentLength);
  Serial1.println("");
  Serial1.println(query);

  Serial.println("Data sent. Waiting for response...");
  while(Serial1.available()){
    Serial.write(Serial1.read());
    Serial.print(".");
  }
  delay(20000);

  if (Serial1.find("ERROR")) {
    Serial.println("Send error");
    delay(1000);
    return;
  }

  Serial1.println("AT+CIPCLOSE");
 Serial.println("\n==================================\n");
  delay(2000);
}
