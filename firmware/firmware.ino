/*
============== VÂTHULT3000 ==============
--- Smart multi-zones watering system ---

Firmware V0.1
 - Developped by: Norman ALIE, Julian DOLOIR, Victor MEULIN
 - Society: ISTY
 - Contact: Norman ALIE <mail@normanalie.fr>
 - License: CC BY-SA

Requirements:
 - ESP with WiFi capabilities
 - CPU Frequency: 160MHz or higher
 - Erase Flash: USe "All flash content" the first time or to reset wifi creditentials memory

For informations about wiring see the "hardware" folder.
For informations about controll see "software" folder.
*/

/* SELECT BOARD */
#define ESP32  // Production version
//#define ESP8266  // Prototype version

/* MQTT Config */
#include "mqtt_conf.h"
/*
#define MQTT_SERVER   "<server address>"
#define MQTT_PORT     <port>
#define MQTT_USER     "<user>"
#define MQTT_PASSWORD "<password>"
*/

#include <WiFiManager.h>
#include <PubSubClient.h>
#include "flags.h"
#include "interface.h"
#include "actuators.h"
#include "flows.h"

#ifdef ARDUINO
  #define SDA A4
  #define SCL A5
#endif
#ifdef ESP8266
  #define SDA D4
  #define SCL D5
#endif
#ifdef ESP32
  #define SDA 1
  #define SCL 2
#endif

// WiFi
WiFiClient espClient;
WiFiManager wm;
// MQTT
PubSubClient mqtt_client(espClient);
String mqtt_client_id = "vathult3000_";

Screen screen = Screen(SDA, SCL, states);
Keyboard keyboard = Keyboard(0b0100010, SDA, SCL, -1);
Actuators actuators = Actuators(0b0100000, SDA, SCL);

void index_html();
void menu_navigate();
void mqtt_reconnect();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup(){
  Serial.begin(9600);
  // I/O
  screen.begin()
  keyboard.begin();
  if(!actuators.begin()) screen.error = "Output not found";
  // WiFi
  WiFi.mode(WIFI_STA);  // For sanity
  wm.setConfigPortalBlocking(false); 
  wm.setAPStaticIPConfig(IPAddress(192,168,1,1), IPAddress(192,168,1,1), IPAddress(255,255,255,0));  // Capttive portal IP
  wm.setTitle("VATHULT3000");
  wm.setClass("invert");  // Captive-portal dark theme
  const char* wm_menu[] = {"wifi","sep","restart","exit"};
  wm.setMenu(wm_menu, 4);
  states[STATE_WIFI] = wm.autoConnect("VATHULT3000_Setup");
  //MQTT
  mqtt_client_id += String(random(0xffff), HEX);
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt_client.setCallback(mqtt_callback);
}

void loop(){
  wm.process();  // WiFi non-blocking loop

  static unsigned long t = millis();
  if(millis()-t > 100){
    // Update WiFi Status
    if(wm.getLastConxResult() == 3){
      states[STATE_WIFI] = 1;
    } else {
      states[STATE_WIFI] = 0;
    }
    // Update MQTT
    if(states[STATE_WIFI]){
      if(!mqtt_client.connected()){
          mqtt_reconnect();
      } else {
        mqtt_client.loop();
      }
    }
    // Update Pump status
    if(states[STATE_SOURCE] == SOURCE_TAP){
      states[STATE_PUMP] = HIGH;
    } else {
      states[STATE_PUMP] = LOW;
    }
    // Update Outputs status
    actuators.write_states(states);
  }
  screen.update();
  keyboard.update();
  menu_navigate();
}

void menu_navigate(){
  static unsigned long t = millis();
  if(millis()-t > 150){
    if(keyboard.isOnlyPressed(BTN_LEFT)){
      screen.nav_left();
    }else if(keyboard.isOnlyPressed(BTN_RIGHT)){
      screen.nav_right();
    }else if(keyboard.isOnlyPressed(BTN_CENTER)){
      screen.nav_center();
    }
    t = millis();
  }
  return;
}


void mqtt_reconnect(){
  if(!mqtt_client.connected()){
    if(mqtt_client.connect(mqtt_client_id.c_str(), MQTT_USER, MQTT_PASSWORD)){  // Attempt to connect
      Serial.println("MQTT Connected");
      mqtt_client.publish("/nodejs/mqtt/3/", "Hello from ESP");
      mqtt_client.subscribe("/nodejs/mqtt/3/");
    } else {
      Serial.println("MQTT Failed to connect");
      Serial.println(mqtt_client.state());
      screen.error = "MQTT Error";
    }
  }
  return;
}

void mqtt_callback(char* topic, byte* payload, unsigned int length){
  Serial.print("MQTT Message arrived on [");
  Serial.print(topic);
  Serial.println("]");
  for(int c=0; c<length; c++){
    Serial.print((char)payload[c]);
  }
  Serial.println();
  return;
}

