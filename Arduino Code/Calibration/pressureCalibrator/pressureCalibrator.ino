
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

// Begin class with selected address
// available addresses (selected by jumper on board) 
// default is ADDRESS_HIGH

//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77

MS5803 sensor(ADDRESS_HIGH);

//Create variables to store results
double pressure_abs_max,pressure_abs_min;
double pressure_relative_max,pressure_relative_min; 
double altitude_delta_max,altitude_delta_min;
double pressure_baseline_max,pressure_baseline_min;

// Create Variable to store altitude in (m) for calculations;
double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)

void setup() {
    Serial.begin(9600);
    //Retrieve calibration constants for conversion math.
    sensor.reset();
    sensor.begin();
    Serial.println("Altimeter Calibration")
    pressure_baseline = sensor.getPressure(ADC_4096);
    
}

void loop() {
  
  // To measure to higher degrees of precision use the following sensor settings:
  // ADC_256 
  // ADC_512 
  // ADC_1024
  // ADC_2048
  // ADC_4096
    

//abs pressure is measured in mbar 
//in order for it to be valid in the hypsometric equation it must be converted into
//pascalls
  
  // Read pressure from the sensor in mbar.
  pressure_abs = sensor.getPressure(ADC_4096);
  //If the pressure is less then the last pressure detected
  //The new minimum is the most recent detected pressure 
  if(pressure_abs < pressure_abs_min)
{
  pressure_abs_min = sensor.getPressure(ADC_4096);
  pressure_MAX = 100*pressure_abs_min;
}
 
 //If the pressure is greater then the max pressure detected 
 //then the detected value becomes the new max
 if(pressure_abs > pressure_abs_max)
{
  pressure_abs_max = sensor.getPressure(ADC_4096);
  pressure_MAX = 100*pressure_abs_max;
} 
  // Convert abs pressure with the help of altitude into relative pressure
  // This is used in Weather stations.
  pressure_relative = sealevel(pressure_abs, base_altitude);
  
  // Taking our baseline pressure at the beginning we can find an approximate
  // change in altitude based on the differences in pressure.   
  altitude_delta = altitude(pressure_abs , pressure_baseline);
  
  // Report values via UART
  
  Serial.print("Maximum Pressure (pa)= ");
  Serial.println(pressure_MAX);
   
  Serial.print("Minimum Pressure (pa)= ");
  Serial.println(pressure_MIN); 
  
  Serial.print("Altitude change (m) = ");
  Serial.println(altitude_delta); 


  delay(1000);

  }
  
  
// Thanks to Mike Grusin for letting me borrow the functions below from 
// the BMP180 example code. 

 double sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return(P/pow(1-(A/44330.0),5.255));
}


double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return(44330.0*(1-pow(P/P0,1/5.255)));
}
  
