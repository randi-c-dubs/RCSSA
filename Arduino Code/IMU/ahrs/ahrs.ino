#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_L3GD20_U.h>

/* Assign a unique ID to the sensors */
Adafruit_9DOF                 dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
const int SAMPLES = 11;
int count = 0;
/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
//  Serial.println(F("Adafruit 9 DOF Pitch/Roll/Heading Example")); Serial.println("");
  
  /* Initialise the sensors */
  initSensors();
}

float median(float array[SAMPLES])
{
  int j;
  float val;
  for (int i = 0; i < SAMPLES; i++)
  {
    val = array[i];
    j = i;
    while (j > 0 && array[j-1] > val)
    {
      array[j] = array[j-1];
      j--;
    }
    array[j] = val;
  }
  return array[int(SAMPLES/2)];
}

float average(float array[SAMPLES])
{
  int j;
  float total;
  for (int i = 0; i < SAMPLES; i++)
  {
    total += array[i];
  }
  return total/SAMPLES;
}
/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  float roll[SAMPLES], pitch[SAMPLES], heading[SAMPLES];
  float acc_x[SAMPLES], acc_y[SAMPLES], acc_z[SAMPLES];

  for (int i = 1; i < SAMPLES; i++)
  {
   roll[i-1] = roll[i];
   pitch[i-1] = pitch[i];
   heading[i-1] = heading[i];
   
   acc_x[i-1] = acc_x[i];
   acc_y[i-1] = acc_y[i];
   acc_z[i-1] = acc_z[i];
  }
 /* Read the accelerometer and magnetometer */
  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);

  /* Use the new fusionGetOrientation function to merge accel/mag data */
  if (dof.fusionGetOrientation(&accel_event, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    if (count == 0) {
      for (int i = 0; i < SAMPLES; i++)
      {
       roll[i] = orientation.roll;
       pitch[i] = orientation.pitch;
       heading[i] = orientation.heading;
       acc_x[i] = accel_event.acceleration.x;
       acc_y[i] = accel_event.acceleration.y;
       acc_z[i] = accel_event.acceleration.z;
      }
      count += 1;
    } else {
        roll[SAMPLES-1] = orientation.roll;
        pitch[SAMPLES-1] = orientation.pitch;
        heading[SAMPLES-1] = orientation.heading;
        acc_x[SAMPLES-1] = accel_event.acceleration.x;
        acc_y[SAMPLES-1] = accel_event.acceleration.y;
        acc_z[SAMPLES-1] = accel_event.acceleration.z;      
    }
   } 
 
  /* Print the average*/
  Serial.print(F("$$$"));
  Serial.print(average(roll));
  Serial.print(F(" "));
  Serial.print(average(pitch));
  Serial.print(F(" "));
  Serial.print(average(heading));
  Serial.print(F(" "));
  Serial.print(average(acc_x));
  Serial.print(F(" "));
  Serial.print(average(acc_y));
  Serial.print(F(" "));
  Serial.print(average(acc_z));
  Serial.println(F(" "));

  delay(100);
}
