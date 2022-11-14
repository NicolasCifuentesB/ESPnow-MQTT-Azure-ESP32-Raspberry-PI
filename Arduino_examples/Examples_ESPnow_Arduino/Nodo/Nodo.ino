/*
 * Includes the following libraries:
 * Adafruit BNO055
 * Adafruit Unified Sensor
 * Adafruit BusIO
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <esp_now.h>
#include "WiFi.h"

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

  
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup(void) 
{
  pinMode(4,INPUT);
  pinMode(23,OUTPUT);
  Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  
  Serial.println("Orientation Sensor Test"); Serial.println("");

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
    
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

void loop(void) 
{
  /* Get a new sensor event */ 
  int8_t temp = bno.getTemp();
  float l;
  float X;
  float Y;
  sensors_event_t event; 
  bno.getEvent(&event);

  X=event.orientation.x,4;
  Y=event.orientation.y,4;

  
  if(digitalRead(4)== HIGH)
  {
    l=0;
    digitalWrite(23,LOW);
    
  }
  else 
  {
    l=1;
    digitalWrite(23,HIGH);
  }
  myData.id=1;
  myData.dato1=X;
  myData.dato2=Y;
  myData.dato3=l;
  myData.dato4=float(temp);
  
  Serial.println("Orientation X: "+String(X)+"\tY: "+String(Y));
  Serial.println("Temperature: "+String(temp)+" C");
  Serial.println(myData.dato3);

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
