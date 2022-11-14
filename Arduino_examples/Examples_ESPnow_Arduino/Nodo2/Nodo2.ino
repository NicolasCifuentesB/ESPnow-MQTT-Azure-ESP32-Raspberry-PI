/*
 * Includes the following libraries:
 * Adafruit BMP085
 * Adafruit Unified Sensor
 * Adafruit BusIO
 */
#include <DHT.h>
#include <Wire.h>
#include "WiFi.h"
#include <esp_now.h>
#include <Adafruit_BMP085.h>

uint8_t broadcastAddress[] = {0x84, 0xCC, 0xA8, 0x64, 0xFD, 0xD8};

typedef struct struct_message {
    int id; // must be unique for each sender board
    float dato1;
    float dato2;
    float dato3;
    float dato4;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

Adafruit_BMP085 bmp;
DHT dht(4, DHT11);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  Serial.println(F("DHT11 test!"));
  
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
    while (1) {}
  }
  
}

void loop() {
  
  String hum="";
  String tem="";
  String pre="";
  String alt="";
  
  myData.id=2;
  myData.dato1 = dht.readHumidity();
  myData.dato2 = dht.readTemperature();
  
  myData.dato3 = bmp.readPressure();
  myData.dato4 = bmp.readAltitude();

  hum="Humedad: "+String(myData.dato1);
  tem="Temperatura: " + String(myData.dato2) +" C";
  pre="Presion: " + String(myData.dato3) +" hPa";
  alt="Altitud: " + String(myData.dato4) +" mts";

  Serial.println(hum);
  Serial.println(tem);
  Serial.println(pre);
  Serial.println(alt);
  
  Serial.println("");


  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  
  delay(500);
}
