//Maple Mini & BluePill

#ifndef LRDuinoDefsMM_h
#define LRDuinoDefsMM_h

#define NUM_DISPLAYS 1
#define INTERVAL    1000
#define BUT_DELAY   100
#define MAX_DEPTH   3
#define DIVISOR     4095
#define MENUTIMEOUT 10000

// HW SPI
#define OLED_MOSI   PA7   // SPI1_MOSI - connect to D1 on SSD1306
#define OLED_CLK    PA5   // SPI1_SCK - connect to D0 on SSD1306
#define OLEDCS_1    PB5
#define OLED_RESET  PB3
#define OLED_DC     PA15

// user input
#define SELBUT      PC13

//K-Line - Serial3
#define K_OUT       PB10  //TX3
#define K_IN        PB11  //RX3


#endif
