#include <WiFi.h>
#include <esp_now.h>
#include <driver/i2s.h>

// MAC addresses - UPDATE THESE with your actual ESP32 MAC addresses
uint8_t txMAC[] = {0x80, 0xF3, 0xDA, 0x63, 0x57, 0x58};
uint8_t rxMAC[] = {0x00, 0x70, 0x07, 0x7C, 0x4A, 0x88}; // This device's MAC (for reference)

// Pins
#define DOORBELL_BUTTON 4
#define SPEAKER_PIN 14
#define STATUS_LED 2

// Removed unused Audio library

// Packet structure
typedef struct {
  int type; // 1: chime, 2: audio
  uint8_t data[240];
} packet_t;

// Audio queue for non-blocking playback
QueueHandle_t audioQueue;

// Audio playback task function
void audioPlaybackTask(void *pvParameters) {
  packet_t audioPacket;
  while (true) {
    // Wait for audio data from queue
    if (xQueueReceive(audioQueue, &audioPacket, portMAX_DELAY) == pdTRUE) {
      // Play the audio data
      digitalWrite(STATUS_LED, HIGH); // LED on during playback
      size_t bytes_written;
      i2s_write(I2S_NUM_1, audioPacket.data, 240, &bytes_written, portMAX_DELAY);
      digitalWrite(STATUS_LED, LOW); // LED off after playback
    }
  }
}




// ESP-NOW callback
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    // Serial.println("RX: Send success");
  } else {
    Serial.println("RX: Send failed");
  }
}

void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  packet_t receivedPacket; // Local packet for received data
  memcpy(&receivedPacket, incomingData, sizeof(packet_t));
  if (receivedPacket.type == 2) {
    // Send audio data to playback task (non-blocking in ISR)
    xQueueSend(audioQueue, &receivedPacket, 0);
  }
}



void setup() {
  Serial.begin(115200);

  // Buttons and LED
  pinMode(DOORBELL_BUTTON, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW); // Off initially

  // Create audio queue for non-blocking playback
  audioQueue = xQueueCreate(10, sizeof(packet_t)); // Buffer up to 10 audio packets
  if (audioQueue == NULL) {
    Serial.println("Failed to create audio queue");
    return;
  }

  // I2S for voice
  i2s_config_t i2s_config_voice = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 8000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 32
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

  // Create audio playback task
  xTaskCreate(audioPlaybackTask, "AudioPlayback", 2048, NULL, 2, NULL);

  // Indicate successful initialization
  digitalWrite(STATUS_LED, HIGH);
  Serial.println("RX unit initialized successfully");
}

void loop() {
  // Non-blocking doorbell button debouncing
  static bool lastDoorbell = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50; // 50ms debounce

  bool currentDoorbell = digitalRead(DOORBELL_BUTTON);
  if (currentDoorbell != lastDoorbell) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Button state has been stable for debounceDelay ms
    if (lastDoorbell == HIGH && currentDoorbell == LOW) {
      // Button pressed (after debouncing)
      digitalWrite(STATUS_LED, HIGH); // LED on during chime send
      packet_t chimePacket;
      chimePacket.type = 1;
      esp_now_send(txMAC, (uint8_t *)&chimePacket, sizeof(packet_t));
      digitalWrite(STATUS_LED, LOW); // LED off after send
    }
  }

  lastDoorbell = currentDoorbell;
}
