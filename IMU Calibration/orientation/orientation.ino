/*
   Name: Miguel Tamayo
   Date: 06/13/2022

   Orientation.ino
   File to test relative orientation of the BNO055 module
   Relative Orientation means that the orientation is zeroed
   out at the point of start of the sensor instead of the magnetic
   fields of Earth


   Before using this, the sensor has to be calibrated using
   the restore_offsets example from the BNO055 library
*/


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>

/*
   Connections
   SCL -> A5
   SDA -> A4
   VDD -> 3-5V DC
   GND -> Common Ground
*/


// Set the delay between fresh samples
#define BNO055_SAMPLERATE_DELAY_MS (100)

// bno055 instance                    ID ADDRESS
Adafruit_BNO055 bno = Adafruit_BNO055(8, 0x28);

/*
   setup function (runs on start up)
*/
void setup() {
  Serial.begin(9600); // communication baud rate
//  Serial.println("BNO055 Relative Orientation Test"); Serial.println("");
  delay(1000);

  // change the mode of the sensor to IMU (relative position)
  // initialize the sensor with relative position
  if (!bno.begin(bno.OPERATION_MODE_IMUPLUS))
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  int eeAddress = 0;
  long bnoID = 8;

  EEPROM.get(eeAddress, bnoID);

  adafruit_bno055_offsets_t calibrationData;
  sensor_t sensor;

  // check for the sensor's calibration data in memory
  bno.getSensor(&sensor);
  if (bnoID != sensor.sensor_id)
  {
    delay(500);
  }
  else
  {
    eeAddress += sizeof(long);
    EEPROM.get(eeAddress, calibrationData);
    bno.setSensorOffsets(calibrationData);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  sensors_event_t event;
  bno.getEvent(&event);
}

void loop() {
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  Serial.print(euler.x()); Serial.print(","); Serial.print(euler.y()); Serial.print(","); Serial.println(euler.z());

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
