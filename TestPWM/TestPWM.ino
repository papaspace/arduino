
// We perform one measurement in each loop. The data gathered in each loop shall be averaged.
// After the full measurement cycle is completed, we send the measurement data via the serial port.

// Constants
float ANALOG_PORT_CONVERSION=5.0/1023.0;  // Analog port conversion factor (voltage / digital resolution)

// Measurement configuration
int led1=3;                               // (D) PWM output port (LED brightness)
float avg_A0;                             // (A) Measured voltage drop (Port A0)
float ravg_A0;
int numSamples=1024;                      // Number of measurement samples (loops)
int jSample;                              // Current measurement sample index
bool sendData;                            // If true, measurement data will be sent via the serial port

String inputStr="";

void setup()
{
  Serial.begin(115200); 
  Serial.setTimeout(1);
  delay(100);
  Serial.write("Hello, I am ready");
}

void loop()
{

  // Setup measurement
  // -------------------------------------------------------------------------------------------------
  if (Serial.available())
  {

    inputStr += Serial.readString();
    if (inputStr[inputStr.length()-1]=='\n')
    {
      // Read serial input data
      // ---------------------------------------------------------------------------------------------
      int x = inputStr.toInt(); 

      // Configure measurement
      // ---------------------------------------------------------------------------------------------
      analogWrite(led1, x);
      resetVariables();
      sendData=true;                        // Trigger serial data reply when measurement is done

      inputStr="";
    }    

  }

  // Return data and reset measurement variables
  // -------------------------------------------------------------------------------------------------
  if (jSample==numSamples-1)
  {
    if (sendData)
    {
      Serial.print(avg_A0);
      Serial.print(",");
      Serial.print(ravg_A0);
      sendData=false;
    }

    resetVariables();
  }

  // Perform continous measurement
  // --------------------------------------------------------------------------------------------------
  float act_A0=float(analogRead(A0))*ANALOG_PORT_CONVERSION;
  avg_A0+=act_A0/float(numSamples);

  // Recursive average: (a[0] + (n - 1) * average(a[1:])) / n
  jSample++;
  ravg_A0 = (act_A0 + (jSample-1) * ravg_A0) / jSample;

}

void resetVariables()
{
  jSample=0;
  avg_A0=0;
  ravg_A0=0;
}
