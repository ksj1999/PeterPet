#include <Wire.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_HTS221.h>

#define MIC_PIN A0  // 아날로그 마이크 입력 핀
#define SAMPLE_RATE 16000  // 샘플링 속도 (16kHz)
#define MIC_GAIN 50  // 마이크 민감도 조정 가능 (16배)
#define BUFFER_SIZE 256  // 샘플 버퍼 크기

//wifi 설정
String ssid = "U+Net3906";
String PASSWORD ="1C10006256";
String host = "210.111.178.93";
int port = 80; // 웹 서버 포트 입력

// 받아올 센서
float ax, ay, az;
float gx, gy, gz;
float temp, humi;

unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 1000;  // 1초에 한 번
int sampleBuffer[BUFFER_SIZE];
float magnitudeBuffer[BUFFER_SIZE / 2];

void connectWifi(){
  String join ="AT+CWJAP=\""+ssid+"\",\""+PASSWORD+"\"";
  Serial.println("Connect Wifi...");
  Serial1.println(join);
  delay(10000);
  if(Serial1.find("OK"))
  {
    Serial.print("WIFI connect\n");
  }else
  {
   Serial.println("connect timeout\n");
  }
  delay(1000);
}

void httpclient(String char_input){
  delay(100);
  Serial.println("connect TCP...");
  Serial1.println("AT+CIPSTART=\"TCP\",\""+host+"\",");
  delay(500);
  if(Serial.find("ERROR")) return;
  
  Serial.println("Send data...");
  String url=char_input;
  String cmd="GET /?ax="+url+" HTTP/1.0\r\n\r\n";

  Serial1.print("AT+CIPSEND=");
  Serial1.println(cmd.length());

  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial1.find(">"))
  {
    Serial.print(">");
  }else
  {
    Serial1.println("AT+CIPCLOSE");
    Serial.println("connect timeout");
    delay(1000);
    return;
  }
  delay(500);
       
  Serial1.println(cmd);
  Serial.println(cmd);
  delay(100);
  if(Serial.find("ERROR")) return;
  Serial1.println("AT+CIPCLOSE");
  delay(100);
}  


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

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

  connectWifi();
  delay(500);
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

  String str_output = String(ax)+"&ay="+String(ay)+"&az="+String(az)
  +"&gx="+String(gx)+"&gy="+String(gy)+"&gz="+String(gz)+"&humi="+String(humi)+"&temp="+String(temp);
  delay(1000);
  httpclient(str_output);
  delay(1000);

  //Serial.find("+IPD");
   while (Serial1.available())
   {
    char response = Serial1.read();
    Serial.write(response);
    if(response=='\r') Serial.print('\n');
    }
   Serial.println("\n==================================\n");
   delay(2000);
  }


