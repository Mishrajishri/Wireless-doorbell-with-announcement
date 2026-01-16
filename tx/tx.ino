#include <WiFi.h>
#include <esp_now.h>

// MAC addresses
uint8_t rxMAC[] = {0x00, 0x70, 0x07, 0x7C, 0x4A, 0x88};

// Pins
#define TALK_BUTTON 5
#define MIC_PIN 34
#define SPEAKER_PIN 14
#define STATUS_LED 2
#define BUZZER_PIN SPEAKER_PIN

// Removed unused Audio library

// Global state
volatile bool chimePlaying = false;
SemaphoreHandle_t chimeMutex;

// Task handles
TaskHandle_t chimeTaskHandle = NULL;

// Packet structure
typedef struct {
  int type; // 1: chime, 2: audio
  uint8_t data[240];
} packet_t;

// Chime task function
void chimeTask(void *pvParameters) {
  while (true) {
    // Wait for chime signal
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // Set chime playing flag with mutex protection
    if (xSemaphoreTake(chimeMutex, portMAX_DELAY) == pdTRUE) {
      chimePlaying = true;
      xSemaphoreGive(chimeMutex);
    }

    // Play chime with LED indication
    digitalWrite(STATUS_LED, HIGH);
    playRailwayAnnouncementChime();
    digitalWrite(STATUS_LED, LOW);

    // Clear chime playing flag with mutex protection
    if (xSemaphoreTake(chimeMutex, portMAX_DELAY) == pdTRUE) {
      chimePlaying = false;
      xSemaphoreGive(chimeMutex);
    }
  }
}

// Indian Railway announcement chime function
void playRailwayAnnouncementChime() {
  // Classic Indian Railway announcement chime - rising arpeggio
  // C5 - E5 - G5 - C6 pattern (approximately 2-3 seconds)
  tone(BUZZER_PIN, 523, 120);  // C5 - ting
  delay(150);
  tone(BUZZER_PIN, 659, 120);  // E5 - ting
  delay(150);
  tone(BUZZER_PIN, 784, 120);  // G5 - ting
  delay(150);
  tone(BUZZER_PIN, 1047, 200); // C6 - final ting (slightly longer)
  delay(250);
  noTone(BUZZER_PIN);
}


// ESP-NOW callback
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    // Serial.println("TX: Send success");
  } else {
    Serial.println("TX: Send failed");
  }
}

void OnDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  packet_t receivedPacket; // Local packet for received data
  memcpy(&receivedPacket, incomingData, sizeof(packet_t));
  if (receivedPacket.type == 1) {
    // Signal chime task to play (non-blocking in ISR)
    if (chimeTaskHandle != NULL) {
      xTaskNotifyGive(chimeTaskHandle);
    }
  }
}

// Task for audio transmission
void audioTransmitTask(void *pvParameters) {
  while (true) {
    bool isChimePlaying = false;
    // Check chimePlaying flag with mutex protection
    if (xSemaphoreTake(chimeMutex, portMAX_DELAY) == pdTRUE) {
      isChimePlaying = chimePlaying;
      xSemaphoreGive(chimeMutex);
    }

    if (!isChimePlaying && digitalRead(TALK_BUTTON) == LOW) {
      digitalWrite(STATUS_LED, HIGH); // LED on during voice transmission
      packet_t txPacket; // Local packet for transmission
      txPacket.type = 2;
      for (int i = 0; i < 240; i++) {
        txPacket.data[i] = analogRead(MIC_PIN) >> 4;
        delayMicroseconds(125); // 8kHz
      }
      esp_now_send(rxMAC, (uint8_t *)&txPacket, sizeof(packet_t));
      digitalWrite(STATUS_LED, LOW); // LED off after transmission
    } else {
      digitalWrite(STATUS_LED, LOW); // LED off when not transmitting
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Buttons and LED
  pinMode(TALK_BUTTON, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW); // Off initially

  // Create mutex for chimePlaying flag
  chimeMutex = xSemaphoreCreateMutex();
  if (chimeMutex == NULL) {
    Serial.println("Failed to create chime mutex");
    return;
  }

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
  memcpy(peerInfo.peer_addr, rxMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Create audio transmit task
  xTaskCreate(audioTransmitTask, "AudioTransmit", 2048, NULL, 1, NULL);

  // Create chime task
  xTaskCreate(chimeTask, "ChimeTask", 2048, NULL, 2, &chimeTaskHandle);

  // Indicate successful initialization
  digitalWrite(STATUS_LED, HIGH);
  Serial.println("TX unit initialized successfully");
}

void loop() {
  // TX has no buttons to check in loop - all handled by task and callbacks
}
