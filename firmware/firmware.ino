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
 - CPU Frequency: 160MHz
 - Erase Flash: USe "All flash content" the first time or to reset wifi creditentials memory

For informations about wiring see the "hardware" folder.
For informations about controll see "software" folder.
*/

/* SELECT BOARD */
//#define ESP32  // Production version
#define ESP8266  // Prototype version
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
  #define SDA 0
  #define SCL 1
#endif

// WiFi
WiFiManager wm;
// MQTT
PubSubClient mqtt_client(wm);

Screen screen = Screen(SDA, SCL, states);
Keyboard keyboard = Keyboard(0b0100010, SDA, SCL, -1);
Actuators actuators = Actuators(0b0100000, SDA, SCL);

void index_html();
void menu_navigate();

void setup(){
  Serial.begin(9600);
  // I/O
  screen.begin();
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
}

void loop(){
  wm.process();  // WiFi non-blocking loop

  static unsigned long t = millis();
  if(millis()-t > 100){
    // Update WiFi Status
    if(wm.getLastConxResult() == 3){
      states[STATE_WIFI] = 1;
      if(!mqtt_client.connected()){
        String client_id = "vathult3000_";
        client_id += String(random(0xffff), HEX);
        if(mqtt_client.connect(client_id.c_str(), MQTT_USER, MQTT_PASSWORD))
      }
    } else {
      states[STATE_WIFI] = 0;
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

