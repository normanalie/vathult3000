/* 
OLED Display Program (Test)

- Supposed to show main menu only
- Sections haven't yet been arranged in an order

TODO : Time showed on menu should be the real time, not a static string
*/ 

#include <U8g2lib.h>
#include <PCF8574.h>

// All the arrays below are generated from images using image2cpp website


// 'icon_wifi', 11x11px
const unsigned char bitmap_icon_wifi [] PROGMEM = {
	0x00, 0x00, 0x3f, 0x80, 0x7f, 0xc0, 0xc0, 0x60, 0x9f, 0x20, 0x3f, 0x80, 0x60, 0xc0, 0x4e, 0x40, 
	0x1f, 0x00, 0x11, 0x00, 0x04, 0x00
};
// 'icon_dashboard', 11x11px
const unsigned char bitmap_icon_dashboard [] PROGMEM = {
	0x40, 0x20, 0x40, 0x20, 0x40, 0x40, 0x40, 0x40, 0x44, 0x40, 0x4a, 0x80, 0x4a, 0x80, 0x51, 0x00, 
	0x40, 0x00, 0xff, 0xe0, 0x40, 0x00
};
// 'icon_flow_meters', 11x11px
const unsigned char bitmap_icon_flow_meters [] PROGMEM = {
	0x3f, 0x80, 0x44, 0x40, 0xa0, 0xa0, 0x90, 0x20, 0x88, 0x20, 0xc4, 0x60, 0x80, 0x20, 0x80, 0x20, 
	0xa0, 0xa0, 0x44, 0x40, 0x3f, 0x80
};
// 'icon_settings', 11x11px
const unsigned char bitmap_icon_settings [] PROGMEM = {
	0x01, 0x80, 0x03, 0x00, 0x03, 0x20, 0x03, 0xe0, 0x07, 0xc0, 0x0e, 0x00, 0x1c, 0x00, 0x38, 0x00, 
	0x70, 0x00, 0xe0, 0x00, 0xc0, 0x00
};
// 'icon_valves', 11x11px
const unsigned char bitmap_icon_valves [] PROGMEM = {
	0x00, 0xe0, 0x00, 0xe0, 0x01, 0xe0, 0x01, 0xc0, 0x03, 0xc0, 0x03, 0x80, 0x0f, 0x80, 0x11, 0x00, 
	0x15, 0x00, 0xff, 0xe0, 0xff, 0xe0
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 240)
const unsigned char* bitmap_icons[5] = {
	bitmap_icon_dashboard,
	bitmap_icon_flow_meters,
	bitmap_icon_settings,
	bitmap_icon_valves,
	bitmap_icon_wifi
};



// 'status_icon_warning', 10x10px
const unsigned char bitmap_status_icon_warning [] PROGMEM = {
	0x0c, 0x00, 0x12, 0x00, 0x12, 0x00, 0x2d, 0x00, 0x2d, 0x00, 0x4c, 0x80, 0x40, 0x80, 0x8c, 0x40, 
	0x80, 0x40, 0xff, 0xc0
};
// 'status_icon_error', 10x10px
const unsigned char bitmap_status_icon_error [] PROGMEM = {
	0x3f, 0x00, 0x40, 0x80, 0xa1, 0x40, 0x92, 0x40, 0x8c, 0x40, 0x8c, 0x40, 0x92, 0x40, 0xa1, 0x40, 
	0x40, 0x80, 0x3f, 0x00
};
// 'status_icon_network_good', 20x10px
const unsigned char bitmap_status_icon_network_good [] PROGMEM = {
	0xfe, 0x00, 0x70, 0x44, 0x00, 0x70, 0x28, 0x00, 0x70, 0x10, 0x07, 0x70, 0x10, 0x07, 0x70, 0x10, 
	0x07, 0x70, 0x10, 0x77, 0x70, 0x10, 0x77, 0x70, 0x17, 0x77, 0x70, 0x17, 0x77, 0x70
};
// 'status_icon_no_network', 20x10px
const unsigned char bitmap_status_icon_no_network [] PROGMEM = {
	0xfe, 0x30, 0x30, 0x44, 0x38, 0x70, 0x28, 0x1c, 0xe0, 0x10, 0x0f, 0xc0, 0x10, 0x07, 0x80, 0x10, 
	0x07, 0x80, 0x10, 0x0f, 0xc0, 0x10, 0x1c, 0xe0, 0x10, 0x38, 0x70, 0x10, 0x30, 0x30
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 192)
const unsigned char* bitmap_status_icons[4] = {
	bitmap_status_icon_error,
	bitmap_status_icon_network_good,
	bitmap_status_icon_no_network,
	bitmap_status_icon_warning
};



// 'separator', 128x3px
const unsigned char bitmap_separator [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'scrollbar', 4x50px
const unsigned char bitmap_scrollbar [] PROGMEM = {
	0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 
	0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 
	0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 
	0x00, 0x00
};
// 'selector_bg', 128x16px
const unsigned char bitmap_selector_bg [] PROGMEM = {
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
	0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 448)
const int bitmap_allArray_LEN = 4;
const unsigned char* bitmap_ui_style[3] = {
	bitmap_scrollbar,
	bitmap_selector_bg,
	bitmap_separator
};




/* ------------------ End generated bitmaps from image2cpp --------------------------------- */

#define NUM_ITEMS 5        // Number of items in the list
#define MAX_ITEM_LENGTH 20 // Maximum characters for the item name


char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {  // array with item names
	{"Dashboard"},
	{"Flow Meters"},
	{"Settings"},
	{"Valves"},
	{"Wi-Fi"}
 };



#define BUTTON_UP_PIN P0       // Pin for UP button 
#define BUTTON_SELECT_PIN P1   // Pin for SELECT button
#define BUTTON_DOWN_PIN P2     // Pin for DOWN button

#define INTERFACE_BOARD_ADDRESS 0x22   // 010 (@ bin)

#define SDA 3   // GPIO 1
#define SCL 4   // GPIO 2

#define INT_1B 5         // Interrupt pin or Interface Board

PCF8574 interfaceBoard(INTERFACE_BOARD_ADDRESS);
//Adafruit_SSD1306 oledDisplay(128, 64, &Wire, -1);
//U8GLIB_SSD1306_128X64 oledDisplay(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);   // U8glib: Fast I2C / TWI
U8G2_SSD1306_128X64_NONAME_1_HW_I2C oledDisplay (U8G2_R0);     // U8g2: U8G2_SSD1306_128X64_NONAME = OLED Display w/o known brand; 1 = Buffer Size; HW_I2C = Hardware I2C; U8G2_R0= No rotation

int button_up_clicked = 0;      // Only perform action when button is clicked, and wait until another press
int button_select_clicked = 0;  // Same as above
int button_down_clicked = 0;    // Same as above

int item_selected = 0;      // Which item in the menu is selected
int item_sel_previous;      // Previous item - used in the menu screen to draw the item before the selected one
int item_sel_next;          // Next item - used in the menu screen to draw next item after the selected one

int current_screen = 0;     // 0 = Menu, 1 = Screenshot

int wifi_status = 1;        // 0 = No Wi-Fi ; 1 = Working Wi-Fi
int warnings = 0;           // Number of warnings happened
int errors = 0;             // Number of errors happened
char ui_time[6] = "12:12" ;  // {EXPERIMANTAL} Change this to the actual time, not a static one

void setup() {

  Serial.begin(115200);    // Arduino = 9600; ESP32 = 115200

  // Config Interface Board
  interfaceBoard.pinMode(P0, INPUT);
  interfaceBoard.pinMode(P1, INPUT);
  interfaceBoard.pinMode(P2, INPUT);

  interfaceBoard.begin();
  oledDisplay.begin();       // (i) Optional for U8glib 

  oledDisplay.setColorIndex(1);   // Set the color to white

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
    //while(1);
  }
}


void loop() {

  //Serial.print(current_screen);

  if (current_screen == 0) { // MENU SCREEN

    // Up and down buttons only work for the menu screen
    if ((interfaceBoard.digitalRead(BUTTON_UP_PIN) == HIGH) && (button_up_clicked == 0)) { // Up button clicked - jump to previous menu item
      item_selected = item_selected - 1;      // Select previous item
      button_up_clicked = 1;                  // Set button to clicked to only perform the action once
      if (item_selected < 0) {                // If first item was selected, jump to last item
        item_selected = NUM_ITEMS-1;
      }

    } else if ((interfaceBoard.digitalRead(BUTTON_DOWN_PIN) == HIGH) && (button_down_clicked == 0)) { // Down button clicked - jump to next menu item
      item_selected = item_selected + 1;       // Select next item
      button_down_clicked = 1;                 // Set button to clicked to only perform the action once
      if (item_selected >= NUM_ITEMS) {        // Last item was selected, jump to first menu item
        item_selected = 0;
      }
    } 

     
    if ((interfaceBoard.digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 1)) {     // Unclick 
      button_up_clicked = 0;
    }
    if ((interfaceBoard.digitalRead(BUTTON_DOWN_PIN) == LOW) && (button_down_clicked == 1)) { // Unclick
      button_down_clicked = 0;
    }
    
  }

  
  // When a section is selected
  if ((interfaceBoard.digitalRead(BUTTON_SELECT_PIN)) && (button_select_clicked == 1)) { // Select button clicked, jump between screens
    button_select_clicked = 1;                             // Set button to clicked to only perform the action once
    if (current_screen == 0) {current_screen = 1;}         // Menu items screen --> Screenshots screen
    else {current_screen = 0;}                             // Screenshots screen --> Menu items screen
  }
  if ((!interfaceBoard.digitalRead(BUTTON_SELECT_PIN)) && (button_select_clicked == 0)) { // Unclick 
    button_select_clicked = 0;
  }

  // Set correct values for the previous and next items
  item_sel_previous = item_selected - 1;
  if (item_sel_previous < 0) {item_sel_previous = NUM_ITEMS - 1;} // Previous item would be below first = make it the last
  item_sel_next = item_selected + 1;  
  if (item_sel_next >= NUM_ITEMS) {item_sel_next = 0;} // Next item would be after last = make it the first

  //current_screen = 0;

  // Interface Draw
  oledDisplay.firstPage();   // Required for page drawing mode for u8g library
  do {

    // Wi-Fi status icon
    if (wifi_status == 1) {
      oledDisplay.drawBitmap(2, 2, 20/8, 10, bitmap_status_icons[1]);
    } else {
      oledDisplay.drawBitmap(2, 2, 20/8, 10, bitmap_status_icons[2]);
    }

    // Warnings status icon
    oledDisplay.drawBitmap(43, 2, 10/8, 10, bitmap_status_icons[3]);
    oledDisplay.setFont(u8g_font_7x14);
    oledDisplay.drawStr(35, 12, (char*)warnings );

    // Errors status icon
    oledDisplay.drawBitmap(74, 2, 10/8, 10, bitmap_status_icons[0]);
    oledDisplay.setFont(u8g_font_7x14);
    oledDisplay.drawStr(66, 12, (char*)errors );

    // Time
    oledDisplay.setFont(u8g_font_7x14);
    oledDisplay.drawStr(94, 11, ui_time);

    // Separator bar
    oledDisplay.drawBitmap(0, 12, 128/8, 3, bitmap_ui_style[2]);


    if (current_screen == 0) { // MENU SCREEN

      // Selected item BG
      oledDisplay.drawBitmap(0, 32, 128/8, 16, bitmap_ui_style[1]);

      // Previous item as icon + label
      oledDisplay.setFont(u8g_font_7x14);
      oledDisplay.drawStr(19, 27, menu_items[item_sel_previous]); 
      oledDisplay.drawBitmap( 4, 17, 11/8, 11, bitmap_icons[item_sel_previous]);          

      // Selected item as icon + label in bold font
      oledDisplay.setFont(u8g_font_7x14B);    
      oledDisplay.drawStr(19, 44, menu_items[item_selected]);   
      oledDisplay.drawBitmap( 4, 35, 11/8, 11, bitmap_icons[item_selected]);     

      // Next item as icon + label
      oledDisplay.setFont(u8g_font_7x14);     
      oledDisplay.drawStr(19, 61, menu_items[item_sel_next]);   
      oledDisplay.drawBitmap( 4, 51, 11/8, 11, bitmap_icons[item_sel_next]);  

      // Scrollbar background
      oledDisplay.drawBitmap(124, 14, 4/8, 50, bitmap_ui_style[0]);

      // Scrollbar handle
      oledDisplay.drawBox(125, 50/NUM_ITEMS * item_selected, 3, 50/NUM_ITEMS); 

    /*
    } else if (current_screen == 1) { // SCREENSHOTS SCREEN
      oledDisplay.drawBitmapP( 0, 0, 128/8, 64, bitmap_screenshots[item_selected]); // Draw screenshot
    */

    }   

  } while ( oledDisplay.nextPage() ); // Required for page drawing mode with u8g library

}
