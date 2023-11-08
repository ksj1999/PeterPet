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

float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float Temperature, Humidity;


void setup() {
  Serial.begin(9600);  // 시리얼 통신 초기화
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
    float decibelValue = 20 * log10(maxMagnitude);

    // 데시벨 값을 시리얼 모니터에 출력
    Serial.println("decibel: " + String(decibelValue, 2));
  }
  //가속도센서
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accel_x, accel_y, accel_z);
    Serial.print("accel: "); Serial.print(accel_x); Serial.print(", "); Serial.print(accel_y); Serial.print(", "); Serial.println(accel_z);
  }
  //자이로센서
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyro_x, gyro_y, gyro_z);
    Serial.print("gyro: "); Serial.print(gyro_x); Serial.print(", "); Serial.print(gyro_y); Serial.print(", "); Serial.println(gyro_z);
  }
 //온도센서
  Temperature = HTS.readTemperature();
  Serial.print("temp : "); Serial.println(Temperature);
  //습도센서
  Humidity = HTS.readHumidity();
  Serial.print("humi : "); Serial.println(Humidity);
  Serial.println("----------------------------------------------------");
  delay(2000);
}
