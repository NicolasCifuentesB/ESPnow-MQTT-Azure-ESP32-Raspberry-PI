#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#include "time.h"
#include "DHT.h"
#include "PubSubClient.h"

// MQTT
const char* mqtt_server = "192.168.34.38";  // IP of the MQTT broker


//Topics MQTT
const char* humidity_topic = "data/station/humidity";
const char* temperature_topic = "data/station/temperature";
const char* presion_topic = "data/station/presiom";
const char* altitud_topic = "data/station/altitud";

const char* mqtt_username = "cbas"; // MQTT username
const char* mqtt_password = "1234"; // MQTT password

const char* clientID = "client_data"; // MQTT client ID


WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); 

//Pines
#define DHT11PIN 16
DHT dht(DHT11PIN, DHT11);

Adafruit_BMP085 bmp;;

//inicializacion de Pantalla
#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Inicializacion de red con clock
const char* ssid = "Redmi 9";
const char* password = "68031001";
const char* ntpServer = "co.pool.ntp.org";
const long gmtOffset_sec = 3600*-5;
const int daylightOffset_sec = 0;

//Conecta a MQTT segun la clave y usuario
void connect_MQTT(){

  if(client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }else {
    Serial.println("Connection to MQTT Broker failed...");
  }

}
//Imprime tiempo
void printLocalTime(){
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){ 
    Serial.print("Error al obtener la hora");
    return;
  } 
  display.print(&timeinfo, "Hora: %H:%M:%S");

}

void setup() {
  //Inicia serial y dht
  Serial.begin(115200);
  dht.begin();

  if (!bmp.begin())
  {
    Serial.println("BMP180 Sensor not found ! ! !");
    while (1)
    {
    
    }
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println(F("SSD1306 ok"));
  
  WiFi.begin(ssid,password);
  Serial.println("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}


void loop(){

  connect_MQTT();

  int humi = dht.readHumidity();
  float temp = dht.readTemperature();
  int pres=((bmp.readPressure()));
  float alt=bmp.readAltitude();

  //Crea string para envio
  String hs="Humedad:"+String((float)humi)+"% ";
  String ts="Temperatura:"+String((float)temp)+"C ";
  String pr="Presion:"+String((float)(pres)/100)+"hPa ";
  String at="Altitud:"+String((float)(alt))+"mts ";


  //MQTT

  //Envio Temperatura
  if (client.publish(temperature_topic, ts.c_str())) {
    Serial.println("Temperature sent!");
  }else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, ts.c_str());
  }

  //Envio Humedad
  if (client.publish(humidity_topic, hs.c_str())) {
    Serial.println("Humidity sent!");
  }else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(humidity_topic, hs.c_str());
  }

  //Envio presion
  if (client.publish(presion_topic, pr.c_str())) {
    Serial.println("Presion sent!");
  }else {
    Serial.println("Presion failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(presion_topic,pr.c_str());
  }

  //Envio altitud
  if (client.publish(altitud_topic, at.c_str())) {
    Serial.println("Presion sent!");
  }else {
    Serial.println("Altitud failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(altitud_topic, at.c_str());
  }

  client.disconnect(); //FIN MQTT
   

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);
  printLocalTime();
  

  display.setCursor(0,10);
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);
  display.print(ts);
  
  

  display.setCursor(0,20);
  display.setTextSize(1);             
  display.setTextColor(SSD1306_WHITE);
  display.print(hs);
 

  display.setCursor(0,30);
  display.setTextSize(1);            
  display.setTextColor(SSD1306_WHITE);
  display.print(pr);


  display.setCursor(0,40);
  display.setTextSize(1);            
  display.setTextColor(SSD1306_WHITE);
  display.print(at);
  


  display.display();
  delay(800);
   
}

