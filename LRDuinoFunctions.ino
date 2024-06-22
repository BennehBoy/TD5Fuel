
void initOBD(void) {
  td5.init();
  if(!td5.connectToEcu(false)) {  
    //disableOBDSensors();
  } else {
    //enableOBDSensors();
    getSensecount();
  }
}

void enableOBDSensors(void) {
  for(uint8_t i=0; i < totalsensors; i++ )  {
    if (Sensors[i].sensetype == OBD ) {
      if (Sensors[i].warnstatus == true) { // already disabled so set warnstatus to true - we'll use this as temp storage so we don't re-enable if OBD gets re-initialised
        Sensors[i].senseactive = false; // should already be false but no harm in making sure
        Sensors[i].warnstatus = false;
      } else {
        Sensors[i].senseactive = true;
        Sensors[i].warnstatus = false;
      }
    }
  }
}

void disableOBDSensors(void) {
  for(uint8_t i=0; i < totalsensors; i++ )  {
    if (Sensors[i].sensetype == OBD ) {
      if (Sensors[i].senseactive == false) { // already disabled so set warnstatus to true - we'll use this as temp storage so we don't re-enable if OBD gets re-initialised
        Sensors[i].senseactive = false;
        Sensors[i].warnstatus = true;
      } else {
        Sensors[i].senseactive = false;
        Sensors[i].warnstatus = false;
      }
    }
  }
getSensecount();
}

void getSensecount(void)
{
  sensecount = 0;
  for (uint8_t thisSensor = 0; thisSensor < totalsensors; thisSensor++) {
    if (Sensors[thisSensor].senseactive == true && Sensors[thisSensor].master == true && Sensors[thisSensor].hidden == false) { // don't count slaves
      sensecount++;
    }
  }
}

bool processHiLo(uint8_t sensor, bool toggle)
{
  // this function toggles a an error flag if the current sensor is above it's high warning parameter or below it's low warning paramater - since the display is redrawn every 250ms it appears to flash
  if (Sensors[sensor].sensevals > Sensors[sensor].sensewarnhivals || Sensors[sensor].sensevals < Sensors[sensor].sensewarnlowvals) { // if we're under the min or over the max then warn!
    if (Sensors[sensor].warnstatus == true) { // if we're already in a wanring state
      if (toggle) { // only used when being called from inside the display because we use this to flash the warning icons - tones are dealt with outside the display loop (eg if we have more sensors than we can draw at once)
        Sensors[sensor].warnstatus = false; // we toggle the value so that the warning triangle flashes based on the interval we are looping at in loop()
      }
      return (false);
    } else {
      if (toggle) {
        Sensors[sensor].warnstatus = true;
      }
      return (true);
    }
  } else { // otherwise return false
    if (toggle) {
      Sensors[sensor].warnstatus = false;
    }
    return false;
  }
  return (Sensors[sensor].warnstatus); // return the current value in the case that there's a connection issue
}

uint8_t processFaultIfSet(uint8_t sensor)
{
  // this function alternates a flag between 1 & 2 if it is set - since the display is redrawn every 250ms it appears to flash
  if (Sensors[sensor].sensefault > 0) {
    if (Sensors[sensor].sensefault == 1) {
      Sensors[sensor].sensefault = 2; // we toggle the value so that the fault icon flashes based on the interval we are looping at in loop()
    } else {
      Sensors[sensor].sensefault = 1;
    }
  }
  return (Sensors[sensor].sensefault);
}

void processFault(uint8_t sensor)
{
  // toggles the fault state of a sensor (to make our fault symbols flash)
  if (Sensors[sensor].sensefault == 2) {
    Sensors[sensor].sensefault = 2; // 2 is animation off
  } else {
    Sensors[sensor].sensefault = 1; // 1 is animation on
  }
}

void processPeak(uint8_t sensor)
{
  // stores the current value of a sensor if it is above the previously stored high value
  if (Sensors[sensor].sensevals >= Sensors[sensor].senseminvals) { // only do this if the value is above the min
    if (Sensors[sensor].sensevals <= Sensors[sensor].sensemaxvals) { // only do this if the value is below the max
      if (Sensors[sensor].sensevals > Sensors[sensor].sensepeakvals) {
        Sensors[sensor].sensepeakvals = int(Sensors[sensor].sensevals); //if we have a new max then store it
      }
    }
  }
}

String getUnits(uint8_t sensor)   // returns the units associated with the sensor
{
  switch (Sensors[sensor].senseunits) {
    case 0:
      return ("C");
    case 1:
      return ("psi");
    case 2:
      return ("lvl");
    case 3:
      return ("o");
    case 4:
      return ("rpm");
    case 5:
      return ("mgh");
    case 6:
      return ("kph");
    case 7:
      return ("lpC");
  }
  return ("");
}

String getValIfNoErr(uint8_t sensor, bool prev)   //prevents values being displayed if we are in fault state OR this is a boolean sensor (coolant level)
{
  //String text = String(Sensors[sensor].sensevals);
  // if a fault is set return an empty string

  if (Sensors[sensor].sensefault > 0) {
    return ("Er");
  }
  if (!prev) {
    return (String(Sensors[sensor].sensevals));
  } else {
    return (String(Sensors[sensor].senselastvals));
  }
}



int processConstraints(int constraint, int checkval, int retval, uint8_t index)   //
{
  if (checkval < constraint) {
    processFault(index); //fault!
    Sensors[index].sensepeakvals = Sensors[index].senseminvals; //set the peak value to minimum (tidies the display)
    retval = Sensors[index].senseminvals; //return minimum value
  } else {
    Sensors[index].sensefault = 0; // no fault
  }
  return (retval);
}

void readSensor(int currentsensor)
{
  Sensors[currentsensor].senselastvals = Sensors[currentsensor].sensevals; // stash the previous value
    if (td5.ecuIsConnected()) {
      int spd=0;
      float inj=0;
      int rpm=0;
      if(td5.getPid(&pidVehicleSpeed) > 0) {
        spd = pidVehicleSpeed.getbValue(0); // Speed
      } 
      delay(55);

      if (td5.getPid(&pidRPM) > 0) {
        rpm = pidRPM.getlValue();
      }
      delay(55);

      if (td5.getPid(&pidFuelling) > 0) {
        inj = pidFuelling.getfValue(6);
      }
      delay(55);
      
      float kgh = (inj * rpm * (5/2) * 60) / 1000000;   //injq*rpm*(5/2)*60/1000000
      float lphk;
      if (spd != 0) { // avoid divide by zero condition
        lphk = (100/spd) * (kgh * 0.85);  // 1kg of diesel = ~0.86kg.
      } else {
        lphk = 0;
      }
      Sensors[currentsensor].sensevals = int(lphk); //stash the computed value

    } // Sensor type switch
    processPeak(currentsensor);  // update the highest value if it's been exceeded - useful for graphs.

}
