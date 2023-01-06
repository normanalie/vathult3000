#include <PCF8574.h>
#include <Adafruit_SSD1306.h>

#define INPUT_BOARD_ADDRESS 0x20       // 000 (@ bin)
#define OUTPUT_BOARD_ADDRESS 0x21      // 001 (@ bin)
#define INTERFACE_BOARD_ADDRESS 0x22   // 010 (@ bin)
#define OLED_DISPLAY_ADDRESS 0x3C      // 0x3C or 0x3D (! CAREFUL ! : Address on hardware =/= Address to use)
#define WATER_INFILTRATION_PIN 8       // Analog Water Infilitration Sensor

#define SDA 3   // GPIO 1
#define SCL 4   // GPIO 2

#define INT_1B 5         // Interrupt pin or Interface Board

void IRAM_ATTR inter();
bool interrupt = false;


PCF8574 inputBoard(INPUT_BOARD_ADDRESS, SDA, SCL);
PCF8574 outputBoard(OUTPUT_BOARD_ADDRESS, SDA, SCL);
PCF8574 interfaceBoard(INTERFACE_BOARD_ADDRESS, SDA, SCL, INT_1B, inter);
Adafruit_SSD1306 oledDisplay(128, 64, &Wire, -1);


void setup() {
  
  // Init serial link
  Serial.begin(9600);
  Serial.println(F("========================================================================"));
  Serial.println(F("          VATHULT3000 v0.1 : Smart multi zones watering system          "));
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
  Serial.print(INPUT_BOARD_ADDRESS, HEX);
  Serial.print(F(" : "));
  if (inputBoard.begin()){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    //TODO: Proper error handling
    while(1);
  }

  // Init Output Board
  Serial.print(F("Output Board Init @ 0x"));
  Serial.print(OUTPUT_BOARD_ADDRESS, HEX);
  Serial.print(F(" : "));
  if (outputBoard.begin()){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    //TODO: Proper error handling
    while(1);
  }

  // Init Interface Board
  Serial.print(F("Interface Board Init @ 0x"));
  Serial.print(INTERFACE_BOARD_ADDRESS, HEX);
  Serial.print(F(" : "));
  if (interfaceBoard.begin()){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    //TODO: Proper error handling
    while(1);
  }

  // Init OLED Display
  Serial.print(F("OLED Display Init @ 0x"));
  Serial.print(OLED_DISPLAY_ADDRESS, HEX);
  Serial.print(F(" : "));
  if (oledDisplay.begin(SSD1306_SWITCHCAPVCC, OLED_DISPLAY_ADDRESS)){
    Serial.println(F("Success !"));
  } else {
    Serial.println(F("Failure..."));
    Serial.println(F("Program shutdown"));
    //TODO: Proper error handling
    while(1);
  }




  // SCREEN TEST
  oledDisplay.println("System : Opperational!");  // print() == Write; println() == Write and Line break at the end of the string
  

}

void loop() {

  int waterInfiltrationValue = analogRead(WATER_INFILTRATION_PIN);  // Note :  Voltage measured is assigned to a value between 0 and 4095 (0V - 3.3V)
  
  if (waterInfiltrationValue >= 620) {   // 620 ~ 0.5V
    Serial.println(F("! WARNING ! = Water infiltration detected! Program shutdown."));
    //TODO: Put system in a secure state.
    while(1);
  }


  // Buttons test
  if(interrupt){
    interrupt = false;
    if(!pcf.digitalRead(P0)){
      Serial.println("RIGHT>>");
    }
    if(!pcf.digitalRead(P1)){
      Serial.println("CENTER");
    }
    if(!pcf.digitalRead(P2)){
      Serial.println("<<LEFT");
    }
  }
  
}



void IRAM_ATTR inter() {
  interrupt = true;
  return;
}


