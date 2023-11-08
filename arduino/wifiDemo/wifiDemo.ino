#include <Wire.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>
#include <WiFiNINA.h> 

#define MIC_PIN A0  // 아날로그 마이크 입력 핀
#define SAMPLE_RATE 16000  // 샘플링 속도 (16kHz)
#define MIC_GAIN 50  // 마이크 민감도 조정 가능 (16배)
#define BUFFER_SIZE 256  // 샘플 버퍼 크기

//wifi 설정
char ssid[] = "U+Net3906";
char pass[] = "1C10006256";
char server[] = "210.111.178.93";
int port = 80; // 웹 서버 포트 입력

// 받아올 센서
float ax, ay, az;
float gx, gy, gz;
float temp, humi;

unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 1000;  // 1초에 한 번
int sampleBuffer[BUFFER_SIZE];
float magnitudeBuffer[BUFFER_SIZE / 2];

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

  pinMode(MIC_PIN, INPUT);
  analogReadResolution(12);

  // Wi-Fi 연결
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connection failed! Reconnecting...");
    delay(3000);
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // 센서 데이터 수집

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

      // 데시벨 값을 시리얼 모니터에 출력
      Serial.println("decibel: " + String(decibelValue, 2));
    }
  //가속도센서
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    Serial.print("ax : "); Serial.println(ax); 
    Serial.print("ay:  "); Serial.println(ay); 
    Serial.print("ax: "); Serial.println(az);
  }
  //자이로센서
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
    Serial.print("gx"); Serial.println(gx); 
    Serial.print("gy"); Serial.print(gy); 
    Serial.print("gz"); Serial.println(gz);
  }
 //온도센서
  temp = HTS.readTemperature();
  Serial.print("temp : "); Serial.println(temp);
  //습도센서
  humi = HTS.readHumidity();
  Serial.print("humi : "); Serial.println(humi);
  Serial.println("----------------------------------------------------");
  delay(2000);

    // 데이터 전송
    if (sendDataToServer()) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Data send failed");
    }

    delay(10000); // 데이터 전송 간격 설정 (10초)
  }
}

bool sendDataToServer() {
  WiFiClient client;
  
  if (client.connect(server, port)) {
    Serial.println("Connected to server");
    
    // HTTP POST 요청 생성
    String dataToSend = "Your_Sensor_Data"; 
    String postData = "data=" + dataToSend;
    String request = "POST /your_endpoint HTTP/1.1\r\n";
    request += "Host: " + String(server) + "\r\n";
    request += "Content-Length: " + String(postData.length()) + "\r\n\r\n";
    request += postData;
    
    // 요청 전송
    client.print(request);

    // 응답 확인
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    return true;
  } else {
    Serial.println("Connection to server failed");
    return false;
  }
}
