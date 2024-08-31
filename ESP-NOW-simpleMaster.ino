/*
**********************************************************
Simple ESP-NOW Master Example - Worked for me with M5 Core 3.
Demo code for M5 Stack with ESP-Now  -  Press A,B, or C button and send corresponding letter to a slave
device -simple character being sent.  

*/
#include <M5Unified.h>
#include "esp_now.h"
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x08, 0xB6, 0x1F, 0x8F, 0xB0, 0x15};

// Structure example to send data
// Must match the receiver structure

char message[1];

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
    M5.begin();  
    M5.Lcd.setTextColor(
        GREEN);  
    M5.Lcd.setTextSize(2);  // Set the font size
    M5.Lcd.setCursor(
        25,
        5);  
    M5.Lcd.println(
        "ESPNow Master Example");
    M5.Lcd.setCursor(3, 35);  
    M5.Lcd.println(
        "Press Button To Send Data To Slave");      
    M5.Lcd.setCursor(5, 75);
      WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
    M5.update();  // Read the press state of the key.  
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000)) {
        M5.Lcd.print('A');
        strcpy(message, "A");      
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
    } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000)) {
        M5.Lcd.print('B');
        strcpy(message, "B");
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
    } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000)) {
        M5.Lcd.print('C');
        strcpy(message, "C");
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) message, sizeof(message));
    } 
}
