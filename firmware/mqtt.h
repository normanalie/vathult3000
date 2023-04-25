#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include "flags.h"
#include "mqtt_conf.h"
#include <WiFiManager.h>
/*
#define MQTT_SERVER   "<server address>"
#define MQTT_PORT     <port>
#define MQTT_USER     "<user>"
#define MQTT_PASSWORD "<password>"
*/

void mqtt_callback(char* topic, byte* payload, unsigned int length);

class MQTT: public PubSubClient{
  public:
    MQTT(Client& wifiClient, String deviceId);
    void setup();
    bool reconnect();
    void send();
    void get();
  private:
    String construct_message();
    String get_input_state();
    String get_errors_json();
    String get_wifi_ssid();
    int8_t get_wifi_rssi();
    String deviceId;
    String mqttTopic;
};

#endif