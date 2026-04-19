#include <esp_now.h>
#include <WiFi.h>

// ← Paste your receiver MAC here
uint8_t receiverMAC[] = {0xE0, 0x8C, 0xFE, 0x32, 0xB6, 0x1C};

#define JOY1_X 34
#define JOY1_Y 35
#define JOY2_X 32
#define JOY2_Y 33

typedef struct {
  uint16_t j1x, j1y, j2x, j2y;
} JoystickData;

JoystickData data;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  Serial.println("Transmitter ready");
}

void loop() {
  data.j1x = analogRead(JOY1_X);
  data.j1y = analogRead(JOY1_Y);
  data.j2x = analogRead(JOY2_X);
  data.j2y = analogRead(JOY2_Y);

  esp_now_send(receiverMAC, (uint8_t*)&data, sizeof(data));
  delay(20);
}