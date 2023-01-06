#define INPUT_BOARD_ADDRESS 0x20       // 000 (@ bin)
#define OUTPUT_BOARD_ADDRESS 0x21      // 001 (@ bin)
#define INTERFACE_BOARD_ADDRESS 0x22   // 010 (@ bin)
#define OLED_DISPLAY_ADDRESS 0x3C      // 0x3C or 0x3D (! CAREFUL ! : Address on hardware =/= Address to use)
#define WATER_INFILTRATION_PIN D8       // Analog Water Infilitration Sensor

#define SDA D5   // GPIO 1
#define SCL D6  // GPIO 2

#define INT_1B D7         // Interrupt pin or Interface Board