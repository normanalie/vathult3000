/*
============== VÂTHULT3000 ==============
--- Smart multi-zones watering system ---

Firmware V1
 - Developped by: Norman ALIE, Julian DOLOIR, Victor MEULIN
 - Society: ISTY
 - Contact: Norman ALIE <mail@normanalie.fr>
 - License: CC BY-SA

Requirements:
 - Custom PCB based on ESP32-C3 (see hardware folder)
 - CPU Frequency: 160MHz or higher
 - Erase Flash: Use "All flash content" the first time or to reset wifi creditentials memory

For informations about wiring see the "hardware" folder.
For informations about controll see "software" folder.
*/

/* SELECT BOARD */
#define ESP32  // Production version

/* HARDWARE CONFIG */
#define FLOW_PIN 4
#define INFILTRATION_PIN 3
#define ACTUATORS_ADDR 0b0100000

#include <WiFiManager.h>
#include "mqtt.h"
#include "flags.h"
#include "actuators.h"
#include "flows.h"

#ifdef ESP32
  #define SDA 5
  #define SCL 6
#endif


// WiFi
WiFiClient espClient;
WiFiManager wm;

MQTT mqtt_client = MQTT(espClient, "test");

Actuators actuators = Actuators(ACTUATORS_ADDR, SDA, SCL);
Flows flow = Flows(FLOW_PIN);


void setup_io();
void setup_wifi();
void setup_captive_portal();
void index_html();



void setup(){
  Serial.begin(9600);
  delay(1500);
  Serial.println("=========== VATHULT3000 ==========");
  setup_io();
  setup_wifi();
  mqtt_client.setup();
}

void loop(){
  wm.process();  // WiFi non-blocking loop

  static unsigned long t = millis();
  if(millis()-t > 100){
    // Update WiFi Status
    if(wm.getWLStatusString() == "WL_CONNECTED"){  // WiFi connected 
      states[STATE_WIFI] = 1;
    } else {
      states[STATE_WIFI] = 0;
    }
    // Update MQTT
    if(states[STATE_WIFI]){
      mqtt_client.reconnect();
    }
    // Update Pump status
    if(states[STATE_SOURCE] == SOURCE_TAP){
      states[STATE_PUMP] = HIGH;
    } else {
      states[STATE_PUMP] = LOW;
    }
    // Update Outputs status
    actuators.write_states(states);
    // Update flow sensor
    states[STATE_FLOW] = flow.read_pin(FLOW_PIN, 100);
    // Update infiltration sensor
    states[STATE_INFILTRATION] = analogRead(INFILTRATION_PIN);
  }
}

void setup_io(){
  actuators.begin();
  return;
}

void setup_wifi(){
  WiFi.mode(WIFI_STA);  // For sanity
  setup_captive_portal();
}

void setup_captive_portal(){
    wm.setConfigPortalBlocking(false); 
    wm.setAPStaticIPConfig(IPAddress(192,168,1,1), IPAddress(192,168,1,1), IPAddress(255,255,255,0));  // Capttive portal IP
    // UI
    wm.setTitle("VATHULT3000");
    wm.setClass("invert");  // Captive-portal dark theme
    const char* wm_menu[] = {"wifi","sep","restart","exit"};  // Captive portal menu buttons (sep = separator)
    wm.setMenu(wm_menu, 4);
    // Start
    states[STATE_WIFI] = wm.autoConnect("VATHULT3000_Setup");
    return;
}
