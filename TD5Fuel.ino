#include <SPI.h>

#if defined ARDUINO_MAPLEMINI_F103CB || defined ARDUINO_BLUEPILL_F103CB || defined ARDUINO_BLUEPILL_F103C8 || defined ARDUINO_BLACKPILL_F303CC
#include "LRDuinoDefsMM.h"
#define Serial SerialUSB
#define ARCH_DEFINED
#endif

//#define ENCODER

#ifndef ARCH_DEFINED
#error Unsupported core \ board combination
#endif

// uncommment the define that matches your display type
//#define USE_SSD1306
#define USE_SSD1331
//#define USE_SSD1351

#include <Adafruit_GFX.h>
#include "Macros.h"

#if defined USE_SSD1306
#include <Adafruit_SSD1306.h>
#define SCREEN_CHOSEN
Adafruit_SSD1306 display1(SCREENWIDTH, SCREENHEIGHT, &SPI, OLED_DC, OLED_RESET, OLEDCS_1);
#endif

#if defined USE_SSD1331
#include <Adafruit_SSD1331.h>
#define SCREEN_CHOSEN
Adafruit_SSD1331 display1(OLEDCS_1, OLED_DC, OLED_RESET);
#endif

#if defined USE_SSD1351
#include <Adafruit_SSD1351.h>
#define SCREEN_CHOSEN
Adafruit_SSD1351 display1(SCREENWIDTH, SCREENHEIGHT, &SPI, OLEDCS_1, OLED_DC, OLED_RESET);
#endif

#ifndef SCREEN_CHOSEN
#error Please choose a screen type by uncommenting one choice only eg "#define USE_SSD1306"
#endif

#include "LRDuinoGFX.h"
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSansBoldOblique18pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>
#include "td5comm.h"


// #include <Buttons.h>


Td5Comm td5; // setup ODBC object

// Button btn_enter(SELBUT, LOW);

#include "td5sensors.h"

// Insert code to read settings in here, and also set default states of sensors to hidden if hw is not present
// end settings reading section

uint8_t sensecount = 0;

// the follow variable is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long previousMillis = 0;        // will store last time the displays were updated
unsigned long menuMillis = 0;
unsigned long inptimeoutMillis = 0;

  int secindex = 0;
  bool secfull = false;
  int seconds[60] = {};
  int minindex = 0;
  bool minfull = false;
  int minutes[60] = {};

#define textScale 1
#define fontX 5
#define fontY 9

void setup()
{
  //start serial connection
  //Serial.begin(57600);  //uncomment to send serial debug info
  


#if defined ARDUINO_ARCH_STM32
#if defined BOARD_maple_mini || defined BOARD_generic_stm32f103c
  // do nothing
#else
  analogReadResolution(12); //12 bits
#endif
#endif


  pinMode(SELBUT, INPUT_PULLUP);
  // Pin setup
  pinMode(OLEDCS_1, OUTPUT);
  digitalWrite(OLEDCS_1, HIGH);

  display1.begin(8000000); //construct our displays
  SCREEN_CLEAR();   // clears the screen and buffer
  SCREEN_DISPLAY(); //output to the screen to avoid adafruit logo
 
  getSensecount(); //check how many sensors we have (in case any are programatically disabled

  initOBD(); // this also fires getSensecount()

} // End Setup

void loop()
{
  unsigned long currentMillis = millis(); //store the time
  int minavg = 0;
  int secavg = 0;
  int i = 0;
  // USER INPUT


  if (currentMillis - previousMillis > INTERVAL) { // only read the sensors and draw the screen if 250 millis have passed
    previousMillis = currentMillis; // save the last time we updated

    if(td5.ecuIsConnected())  {       // keep ecu alive
      if(td5.getLastReceivedPidElapsedTime() > KEEP_ALIVE_TIME) {
        td5.getPid(&pidKeepAlive);  
      }     
      if (td5.getConsecutiveLostFrames() > 3) { // shutdown in case of too many frames lost
        //disableOBDSensors();
        td5.disconnectFromEcu();
        SCREEN_CLEAR();
      }
    }


  // Get teh OBD data
  readSensor(0);
  /*
  // do seconds & minute handling to obtain an average - TODO, discount values where vehicle is stationary.
  secindex++;
  if(secindex==60) {
    secindex=0;
    secfull=true;
    minindex++;
  }
  seconds[secindex]=Sensors[0].sensevals;
  int avgloop = 0;
  if(secfull) {
    avgloop=60;
  } else {
    avgloop=secindex;
  }
  for(i=1; i<avgloop+1; i++)
  {
    secavg = secavg + seconds[i];
  }
  secavg = secavg / avgloop;
  minutes[minindex]=secavg;

 if(minindex==60) {
    minindex=0;
    minfull=true;
  }

  if(minfull) {
    avgloop=60;
  } else {
    avgloop=minindex;
  }
  for(i=1; i<avgloop+1; i++)
  {
    minavg = minavg + minutes[i];
  }
  minavg = minavg / avgloop;

Sensors[0].sensevals=minavg;
*/

  // draw the display
  drawDISPLAY(display1, 1);
  }


} // Void Loop()
