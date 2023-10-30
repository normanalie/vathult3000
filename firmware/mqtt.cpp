#include "PubSubClient.h"
#include "mqtt.h"
#include <ArduinoJson.h>

bool mqtt_states_buf[5] = {0};

void mqtt_callback(char* topic, byte* payload, unsigned int length){
  String json = "";
  Serial.print("MQTT Message arrived on [");
  Serial.print(topic);
  Serial.println("]");
  for(int c=0; c<length; c++){
    Serial.print((char)payload[c]);
    json += (char)payload[c];
  }
  Serial.println();

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json.c_str());
    mqtt_states_buf[0] = doc["input"]=="rain" ? 1 : 0;
    mqtt_states_buf[1] = doc["output1"]; 
    mqtt_states_buf[2] = doc["output2"]; 
    mqtt_states_buf[3] = doc["output3"]; 
    mqtt_states_buf[4] = doc["output4"]; 
  return;
}

MQTT::MQTT(Client& wifiClient, String deviceId): PubSubClient(wifiClient){
  this->deviceId = deviceId;
  this->mqttTopic = "vathult3000/"+this->deviceId;
} 

void MQTT::setup(){
  this->setServer(MQTT_SERVER, MQTT_PORT);
  this->setCallback(mqtt_callback);
  return;
}


bool MQTT::reconnect(){
  if(!this->connected()){
    if(this->connect(this->deviceId.c_str(), MQTT_USER, MQTT_PASSWORD)){  // Attempt to connect
      this->subscribe("vathult3000/test/set");
      Serial.print("MQTT Connected on: ");
      Serial.println(this->mqttTopic);
      Serial.print("MQTT Subscribed on: ");
      Serial.println(String(this->mqttTopic+"/set").c_str());
      return 1;
    } else {
      Serial.println("MQTT Failed to connect");
      Serial.println(this->state());
      return 0;
    }
  }
}


void MQTT::send(){
  String message = this->construct_message();
  this->publish(this->mqttTopic.c_str(), message.c_str());
  return;
}

String MQTT::construct_message(){
    char json[200] = "";
    sprintf(json, 
      "{ \"output1\":%d,"
        "\"output2\":%d,"
        "\"output3\":%d,"
        "\"output4\":%d,"
        "\"input\":\"%s\","
        "\"flow\":%d,"
        "\"infiltration\":%d,"
        "\"errors\":%s,"
        "\"wifi_strength\":%d,"
        "\"wifi_ssid\": \"%s\" }\0", 
      states[STATE_OUTPUT1], 
      states[STATE_OUTPUT2], 
      states[STATE_OUTPUT3], 
      states[STATE_OUTPUT4], 
      get_input_state(),
      states[STATE_FLOW],
      states[STATE_INFILTRATION],
      get_errors_json(),
      get_wifi_rssi(),
      get_wifi_ssid()
      );
    return String(json);
}

String MQTT::get_input_state(){
  if(states[STATE_SOURCE] == SOURCE_RAIN){
    return "rain";    
  }
  else if(states[STATE_SOURCE] == SOURCE_TAP){
    return "tap";
  }
  return "none";
}

String MQTT::get_errors_json(){
  String json = "[";
  for(int i = 0; i<ERRORS_LEN; i++){
    if(errors[i] != ""){
      json += "{ \"data\":"+errors[i]+" }";
    }
  }
  json += "]";
  return json;
}

String MQTT::get_wifi_ssid(){
  //return String(WiFi.SSID());
  return "fakessid";
}

int8_t MQTT::get_wifi_rssi(){
  int32_t rssi = WiFi.RSSI();
  return (int8_t)6+(rssi/20);  // Return a level between 0 and 5
}

bool MQTT::get_input(){
  return mqtt_states_buf[0];
}

void MQTT::get_outputs(bool outputs[4]){
  for(int i = 0; i<4; i++){
    outputs[i] = mqtt_states_buf[i+1];
  }
  return;
}