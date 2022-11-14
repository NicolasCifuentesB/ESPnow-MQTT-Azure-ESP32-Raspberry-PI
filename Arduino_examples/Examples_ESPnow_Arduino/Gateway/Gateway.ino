/*
 * Includes the following libraries:
 * Adafruit SSD1306
 * Adafruit GFX Library
 * Adafruit BusIO
 */
#include "WiFi.h"
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels



// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

typedef struct struct_message {
    int id; // must be unique for each sender board
    float dato1;
    float dato2;
    float dato3;
    float dato4;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2};

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].dato1 = myData.dato1;
  boardsStruct[myData.id-1].dato2 = myData.dato2;
  boardsStruct[myData.id-1].dato3 = myData.dato3;
  boardsStruct[myData.id-1].dato4 = myData.dato4;
  
  Serial.printf("%f",boardsStruct[myData.id-1].dato1);
  Serial.printf("%f",boardsStruct[myData.id-1].dato2);
  Serial.printf("%f",boardsStruct[myData.id-1].dato3);
  Serial.printf("%f",boardsStruct[myData.id-1].dato4);
  Serial.println();
}


void Nodo1() {
  oled.setCursor(0, 10);       // set position to display
  oled.println("oreintacion X: "+ String(boardsStruct[0].dato1)); // set text 

  oled.setCursor(0, 20);       // set position to display
  oled.println("orientacion Y: "+String(boardsStruct[0].dato2)); // set text  

  oled.setCursor(0, 30);       // set position to display
  if(boardsStruct[0].dato3 == 0){
    oled.println("el objeto esta lejos"); // set text
  }
  else{
    oled.println("el objeto esta cerca"); // set text
  }
   
  oled.setCursor(0, 40);       // set position to display
  oled.println("T: "+String(boardsStruct[0].dato4)+" C"); // set text

}

void Nodo2(){
  
  oled.setCursor(0, 10);       // set position to display
  oled.println("Humedad: "+String(boardsStruct[1].dato1)); // set text 

  oled.setCursor(0, 20);       // set position to display
  oled.println("Temperatura: " +String(boardsStruct[1].dato2)+" C"); // set text  

  oled.setCursor(0, 30);       // set position to display
  oled.println("Presion: "+String(boardsStruct[1].dato3) +" hPa"); // set text 

  oled.setCursor(0, 40);       // set position to display
  oled.println("Altitud: "+String(boardsStruct[1].dato4) + " mts"); // set text

}

void setup() {
   pinMode(4,INPUT);
   Serial.begin(9600);

   WiFi.mode(WIFI_STA);
   Serial.println(WiFi.macAddress());
   
  // put your setup code here, to run once:
  
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }
  
  if (esp_now_init() != ESP_OK) {
  Serial.println("Error initializing ESP-NOW");
  return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color

  if(digitalRead(4)== HIGH)
  {
    oled.setCursor(0, 0);       // set position to display
    oled.println("Nodo 1"); // set text
    Nodo1();
  }
  else 
  {
    oled.setCursor(0, 0);       // set position to display
    oled.println("Nodo 2"); // set text
    Nodo2();
  }
  
  oled.display();  
}
