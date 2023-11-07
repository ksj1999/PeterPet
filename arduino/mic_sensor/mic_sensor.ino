// 아날로그 소리의 데시벨을 계산해 BLE를 통해 다른 장치로 전송하는 코드

#include <ArduinoBLE.h>

#define MIC_PIN A0  // 아날로그 마이크 입력 핀
#define SAMPLE_RATE 16000  // 샘플링 속도 (16kHz)
#define MIC_GAIN 50  // 마이크 민감도 조정 가능 (16배)
#define BUFFER_SIZE 256  // 샘플 버퍼 크기

BLEService audioService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEFloatCharacteristic decibelCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

int sampleBuffer[BUFFER_SIZE];
float magnitudeBuffer[BUFFER_SIZE / 2];

void setup() {
  Serial.begin(9600);  // 시리얼 통신 초기화
  while (!Serial);

  // BLE 초기화
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // BLE 서비스 및 특성 설정
  BLE.setLocalName("BLE Decibel Meter");
  BLE.setAdvertisedService(audioService);

  audioService.addCharacteristic(decibelCharacteristic);
  BLE.addService(audioService);

  decibelCharacteristic.setValue(0); // 초기 값 설정

  // BLE 광고 시작
  BLE.advertise();

  // 아날로그 입력 설정
  pinMode(MIC_PIN, INPUT);

  // 아날로그 입력 해상도 설정 (12비트)
  analogReadResolution(12);
}

void loop() {            
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
  Serial.println("Decibel Value: " + String(decibelValue, 2) + " dB");

  // BLE 특성에 데시벨 값을 업데이트
  decibelCharacteristic.setValue(decibelValue);
  BLE.poll();  // 데이터 전송
}
