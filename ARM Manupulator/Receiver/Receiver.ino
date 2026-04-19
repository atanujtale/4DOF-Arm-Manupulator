#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 150
#define SERVOMAX 600

typedef struct {
  uint16_t j1x, j1y, j2x, j2y;
} JoystickData;

JoystickData incoming;

uint16_t toServo(uint16_t val) {
  return map(val, 0, 4095, SERVOMIN, SERVOMAX);
}

// ✅ Updated signature for ESP32 core 3.x
void onReceive(const esp_now_recv_info* info, const uint8_t* data, int len) {
  memcpy(&incoming, data, sizeof(incoming));

  pca.setPWM(0, 0, toServo(incoming.j1x)); // Base
  pca.setPWM(1, 0, toServo(incoming.j1y)); // Shoulder
  pca.setPWM(2, 0, toServo(incoming.j2x)); // Elbow
  pca.setPWM(3, 0, toServo(incoming.j2y)); // Gripper
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onReceive);

  Wire.begin(21, 22);
  pca.begin();
  pca.setPWMFreq(50);

  Serial.println("Receiver ready");
}

void loop() {
  delay(10);
}