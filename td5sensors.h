#pragma once

// This is all the parameters and variables for our sensors
// define sensor types so we can switch on these for sensor reading
#define OBD           7
// OBD PIDS
#define OBDRPM        9
#define OBDINJ        10
#define OBDSPD        11
#define OBDCON        12

// when multiple sensors displayed on one screen the master sensor has this ID
#define MASTER        99

// Definitions for out of bounds warning values
#define NOLOWARN      -9999
#define NOHIWARN      9999

typedef struct
{
  bool senseactive;
  bool master;
  uint8_t slaveID;
  uint8_t senseorder;
  bool warnstatus;
  uint8_t sensefault;
  const unsigned char* senseglyphs;
  int sensevals;
  int senselastvals;
  const uint8_t senseunits;
  const int sensemaxvals;
  const int8_t senseminvals;
  int sensepeakvals;
  int sensewarnhivals;
  int sensewarnlowvals;
  const char sensename[13];
  uint8_t sensetype;
  uint8_t sensepin;
  bool hidden;
} SingleSensor;

SingleSensor Sensors[4] = {
 //active  master slaveID  senseorder  warnstatus  sensefault senseglyphs sensevals  senselastvals  units maxvals minvals peakvals warnhivals warnlovals  sensename       sensetype  PIN/PID  hidden
  {false,  true,  MASTER,  0,          false,      0,         Gauge,      0,         -999,             7,    20,     0,      0,       NOHIWARN,  NOLOWARN,   "l/100km",      OBD,       OBDCON,  false}, // Fuel Consumption (composite PID)
  {false,   true, MASTER,  0,          true,      0,         Gauge,      0,         -999,             4,    4500,   0,      0,       NOHIWARN,  NOLOWARN,   "RPM (OBD)",    OBD,       OBDRPM,  false}, // RPM
  {false,   true, MASTER,  0,          true,      0,         Gauge,      0,         -999,             5,    100,    0,      0,       NOHIWARN,  NOLOWARN,   "Inj mg/s",     OBD,       OBDINJ,  false}, // Injection Quantity
  {false,   true, MASTER,  0,          true,      0,         Gauge,      0,         -999,             6,    200,    -30,    0,       NOHIWARN,  NOLOWARN,   "Speed (OBD)",  OBD,       OBDSPD,  false}  // Roadspeed 
};

const uint8_t totalsensors = 4; //this is the actual number of definitions above
