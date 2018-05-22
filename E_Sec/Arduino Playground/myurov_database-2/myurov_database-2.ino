#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <HMC5883L.h>

// -----------------------------------------------------------------------------------------

   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
HMC5883L compass;

int error = 0;

void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  delay(500);
}


void setup(void) 
{
  Serial.begin(9600);
  compass = HMC5883L(); // Construct a new HMC5883 compass. 
  
  if(!bmp.begin())
  {
    while(1);
  }
   
  displaySensorDetails();
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
   error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0); // If there is an error, print it out.
 
}

void loop(void) 
{
//  char start = 'H';
 //  Serial.print(start);
   
  sensors_event_t event;
  bmp.getEvent(&event);
 
  
  if (event.pressure)
  {
    
 int pressure = event.pressure;
 float mercury = 0;
 mercury = pressure * 0.029529980164712; // Convert hectopascals to in of mercury.
    Serial.print(mercury);
    Serial.print(",");
 //   Serial.println(" inHg");

    float temperature;
    bmp.getTemperature(&temperature);
 
    int temp2 = temperature * 1.8000 + 32;
    Serial.print(temp2);
    Serial.print(",");

    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

    int alt = bmp.pressureToAltitude(seaLevelPressure, event.pressure)* 3.2808;
 
    Serial.print(alt);
    Serial.print(",");

  }
  else
  {

  }
  delay(1000);
  
  MagnetometerRaw raw = compass.ReadRawAxis();
 
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  

  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

 
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
 
  float declinationAngle = 0.0526; //0.0457
  heading += declinationAngle;
  
 
  if(heading < 0)
    heading += 2*PI;
    

  if(heading > 2*PI)
    heading -= 2*PI;
   

  float headingDegrees = heading * 180/M_PI; 

 
  Output(raw, scaled, heading, headingDegrees);

 
}

void Output(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees)
{

   Serial.print(raw.XAxis);
   Serial.print(",");
  
   Serial.print(raw.YAxis);
   Serial.print(",");
 
   Serial.print(raw.ZAxis);
   Serial.print(",");

   Serial.print(scaled.XAxis);
   Serial.print(",");
 
   Serial.print(scaled.YAxis);
   Serial.print(",");
   
   Serial.print(scaled.ZAxis);
   Serial.print(",");

   Serial.print(heading);
   Serial.print(",");

   Serial.println(headingDegrees);
//   Serial.println(",");
 //int start = 101;
 //  Serial.println(start);

}
