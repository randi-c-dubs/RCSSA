#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
/* After this code is run the values should be written to a text file and then that 
 *  text file should be read by another file and the smallest and largest values for the imu
 *  should be read in and then that range should bethe stablized calibration
 *  when the other code should detect changed in these values and output an appropriate offest to
 *  insure the position of the imu falls between the designated values.
 */
/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();

/* Assign a unique ID to these sensors */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


 
float AccelMinX, AccelMaxX;
float AccelMinY, AccelMaxY;
float AccelMinZ, AccelMaxZ;



 
float MagMinX, MagMaxX;
float MagMinY, MagMaxY;
float MagMinZ, MagMaxZ;


 
long lastDisplayTime;
 
void setup(void) 
{
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("LSM303 Calibration"); Serial.println("");
  
  /* Initialise the accelerometer */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  /* Initialise the magnetometer */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  lastDisplayTime = millis();
}
 
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t accelEvent; 
  sensors_event_t magEvent; 
  
  accel.getEvent(&accelEvent);
  mag.getEvent(&magEvent);
  
  if (accelEvent.acceleration.x < AccelMinX)
  /* If the detected acceleration value is less than the defined Acceleration value*/
  /* AccelMinX_change = AccelMinX - accelEvent.acceleration.x;
  */
  AccelMinX = accelEvent.acceleration.x;
  
  if (accelEvent.acceleration.x > AccelMaxX) 
  /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* AccelMaxX_change =  accelEvent.acceleration.x-AccelMinX ;
  */
  AccelMaxX = accelEvent.acceleration.x;
  
  if (accelEvent.acceleration.y < AccelMinY)
   /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* AccelMinY_change = AccelMinY - accelEvent.acceleration.y;
  */
  AccelMinY = accelEvent.acceleration.y;
  
  if (accelEvent.acceleration.y > AccelMaxY) 
    /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* AccelMaxY_change = accelEvent.acceleration.y-AccelMaxY ;
  */ 
  AccelMaxY = accelEvent.acceleration.y;
 
  if (accelEvent.acceleration.z < AccelMinZ)
   /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* AccelMinZ_change = AccelMinZ - accelEvent.acceleration.z;
  */
  AccelMinZ = accelEvent.acceleration.z;

  
  if (accelEvent.acceleration.z > AccelMaxZ) 
   /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* AccelMaxZ_change = accelEvent.acceleration.z-AccelMaxZ ;
  */ 
  
  AccelMaxZ = accelEvent.acceleration.z;
 
  if (magEvent.magnetic.x < MagMinX) 

 /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* MagMinX_change = MagMinX - magEvent.magnetic.x ;
  */ 
  
  MagMinX = magEvent.magnetic.x;
  if (magEvent.magnetic.x > MagMaxX) 

   /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* MagMaxX_change = magEvent.magnetic.x-MagMinX  ;
  */
  MagMaxX = magEvent.magnetic.x;
  
  if (magEvent.magnetic.y < MagMinY)
   /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* MagMinY_change = MagMinY - magEvent.magnetic.y ;
  */ 
  MagMinY = magEvent.magnetic.y;
  if (magEvent.magnetic.y > MagMaxY) 
    /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* MagMaxY_change = magEvent.magnetic.y-MagMinY  ;
  */
  MagMaxY = magEvent.magnetic.y;
 
  if (magEvent.magnetic.z < MagMinZ)
  /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* MagMinZ_change = MagMinZ - magEvent.magnetic.z ;
  */ 
  MagMinZ = magEvent.magnetic.z;
  if (magEvent.magnetic.z > MagMaxZ) 
   /* If the detected acceleration value is greater than the defined Acceleration value*/
  /* MagMaxZ_change = magEvent.magnetic.z-MagMinZ  ;
  */
  MagMaxZ = magEvent.magnetic.z;
 
  if ((millis() - lastDisplayTime) > 1000)  // display once/second
  {
    Serial.print("Accel Minimums: "); Serial.print(AccelMinX);Serial.print("  ");Serial.print(AccelMinY); Serial.print("  "); Serial.print(AccelMinZ); Serial.println();
    Serial.print("Accel Maximums: "); Serial.print(AccelMaxX); Serial.print("  ");Serial.print(AccelMaxY); Serial.print("  "); Serial.print(AccelMaxZ); Serial.println();
    Serial.print("Mag Minimums: "); Serial.print(MagMinX); Serial.print("  ");Serial.print(MagMinY); Serial.print("  "); Serial.print(MagMinZ); Serial.println();
    Serial.print("Mag Maximums: "); Serial.print(MagMaxX); Serial.print("  ");Serial.print(MagMaxY); Serial.print("  "); Serial.print(MagMaxZ); Serial.println(); Serial.println();
    lastDisplayTime = millis();
  /* Write Data to a CSV file*/

  
  }
}
