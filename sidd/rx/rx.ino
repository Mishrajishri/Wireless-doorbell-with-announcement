#include <WiFi.h>
#include <esp_now.h>
#include <SPIFFS.h>
#include <Audio.h>
#include <driver/i2s.h>

// MAC addresses
uint8_t txMAC[] = {0x00, 0x70, 0x07, 0x7C, 0x4A, 0x88};

// Pins
#define DOORBELL_BUTTON 4
#define TALK_BUTTON 5
#define MIC_PIN 34
#define SPEAKER_PIN 14  // Changed to match I2S data_out_num
#define STATUS_LED 2

// Audio
Audio audio;

// Packet structure
typedef struct {
  int type; // 1: chime, 2: audio
  uint8_t data[240];
} packet_t;

packet_t packet;

// ESP-NOW callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Optional: handle send status
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&packet, incomingData, sizeof(packet));
  if (packet.type == 1) {
    // Play chime
    audio.connecttoFS(SPIFFS, "/doorbellsound.mp3");
    audio.loop();
  } else if (packet.type == 2) {
    // Play voice
    size_t bytes_written;
    i2s_write(I2S_NUM_1, packet.data, 240, &bytes_written, portMAX_DELAY);
  }
}

// Task for audio transmission
void audioTransmitTask(void *pvParameters) {
  while (true) {
    if (digitalRead(TALK_BUTTON) == LOW) {
      packet.type = 2;
      for (int i = 0; i < 240; i++) {
        packet.data[i] = analogRead(MIC_PIN) >> 4;
        delayMicroseconds(125); // 8kHz
      }
      esp_now_send(txMAC, (uint8_t *)&packet, sizeof(packet));
    } else {
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  if (!SPIFFS.exists("/doorbellsound.mp3")) {
    Serial.println("doorbellsound.mp3 not found in SPIFFS");
    return;
  }

  // Buttons and LED
  pinMode(DOORBELL_BUTTON, INPUT_PULLUP);
  pinMode(TALK_BUTTON, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW); // Off initially

  // Audio for chime
  audio.setPinout(12, 13, SPEAKER_PIN);
  audio.setVolume(10);

  // I2S for voice
  i2s_config_t i2s_config_voice = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 8000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64
  };
  i2s_pin_config_t pin_config_voice = {
    .bck_io_num = 12,
    .ws_io_num = 13,
    .data_out_num = 14,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  i2s_driver_install(I2S_NUM_1, &i2s_config_voice, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &pin_config_voice);

  // WiFi
  WiFi.mode(WIFI_STA);

  // ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Add peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, txMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Create audio transmit task
  xTaskCreate(audioTransmitTask, "AudioTransmit", 2048, NULL, 1, NULL);

  // Indicate successful initialization
  digitalWrite(STATUS_LED, HIGH);
  Serial.println("RX unit initialized successfully");
}

void loop() {
  // Doorbell button
  static bool lastDoorbell = HIGH;
  bool currentDoorbell = digitalRead(DOORBELL_BUTTON);
  if (lastDoorbell == HIGH && currentDoorbell == LOW) {
    // Debounce
    delay(50);
    // Play chime locally
    audio.connecttoFS(SPIFFS, "/doorbellsound.mp3");
    // Send to TX
    packet.type = 1;
    esp_now_send(txMAC, (uint8_t *)&packet, sizeof(packet));
  }
  lastDoorbell = currentDoorbell;

  // Audio loop
  audio.loop();
}
