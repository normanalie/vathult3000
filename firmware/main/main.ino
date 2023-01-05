#include <PCF8574.h>
#define INPUT_BOARD_ADRESS 0x20       // 000 (@ bin)
#define OUTPUT_BOARD_ADRESS 0x21      // 001 (@ bin)
#define INTERFACE_BOARD_ADRESS 0x22   // 010 (@ bin)
#define WATER_INFILTRATION_PIN 8      // Analog Water Infilitration Sensor


PCF8574 inputBoard(INPUT_BOARD_ADRESS);
PCF8574 outputBoard(OUTPUT_BOARD_ADRESS);
PCF8574 interfaceBoard(INTERFACE_BOARD_ADRESS);


void setup() {
  
  // Init serial link
  Serial.begin(9600);
  Serial.println(F("========================================================================"));
  Serial.println(F("       VATHULT3000 v0.1 : Smart multi zones watering system             "));
  Serial.println(F("========================================================================"));
  Serial.println("");

  // Config Input Board
  inputBoard.pinMode(P0, INPUT);
  inputBoard.pinMode(P1, INPUT);
  inputBoard.pinMode(P2, INPUT);
  inputBoard.pinMode(P3, INPUT);
  inputBoard.pinMode(P4, INPUT);
  inputBoard.pinMode(P5, INPUT);

  // Config Output Board
  outputBoard.pinMode(P0, OUTPUT, HIGH);  //Active LOW logic
  outputBoard.pinMode(P1, OUTPUT, HIGH);
  outputBoard.pinMode(P2, OUTPUT, HIGH);
  outputBoard.pinMode(P3, OUTPUT, HIGH);
  outputBoard.pinMode(P4, OUTPUT, HIGH);
  outputBoard.pinMode(P5, OUTPUT, HIGH);
  outputBoard.pinMode(P6, OUTPUT, HIGH);
  outputBoard.pinMode(P7, OUTPUT, HIGH);

  // Config Interface Board
  interfaceBoard.pinMode(P0, INPUT);
  interfaceBoard.pinMode(P1, INPUT);
  interfaceBoard.pinMode(P2, INPUT);


  // Init Input Board
  Serial.print(F("Input Board Init @ 0x"));
  Serial.print(INPUT_BOARD_ADRESS, HEX);
  Serial.print(F(" : "));
  if (inputBoard.begin()){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    while(1);
  }

  // Init Output Board
  Serial.print(F("Output Board Init @ 0x"));
  Serial.print(OUTPUT_BOARD_ADRESS, HEX);
  Serial.print(F(" : "));
  if (outputBoard.begin()){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    while(1);
  }

  // Init Interface Board
  Serial.print(F("Interface Board Init @ 0x"));
  Serial.print(INTERFACE_BOARD_ADRESS, HEX);
  Serial.print(F(" : "));
  if (interfaceBoard.begin()){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    while(1);
  }
  

}

void loop() {

  int waterInfiltrationValue = analogRead(WATER_INFILTRATION_PIN);  // Note :  Voltage measured is assigned to a value between 0 and 4095 (0V - 3.3V)
  
  if (waterInfiltrationValue >= 620) {   // 620 ~ 0.5V
    Serial.println(F("! WARNING ! = Water infiltration detected! Program shutdown."));
    while(1);
  }

}
