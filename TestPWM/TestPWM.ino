
// We perform one measurement in each loop. The data gathered in each loop shall be averaged.
// After the full measurement cycle is completed, we send the measurement data via the serial port.

// Constants
float ANALOG_PORT_CONVERSION=5.0/1024.0;  // Analog port conversion factor (voltage / digital resolution)

// Measurement configuration
int led1=3;                               // (D) PWM output port (LED brightness)
float avg_A0;                             // (A) Measured voltage drop (Port A0)
int numSamples=10000;                     // Number of measurement samples (loops)
int jSample;                              // Current measurement sample index
bool sendData;                            // If true, measurement data will be sent via the serial port

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
    // Read serial input data
    // -----------------------------------------------------------------------------------------------
    delay(50);                            // Wait for the serial buffer to be completely filled
    int x=Serial.readString().toInt();

    // Configure measurement
    // -----------------------------------------------------------------------------------------------
    analogWrite(led1, x);
    resetVariables();
    sendData=true;                        // Trigger serial data reply when measurement is done
  }

  // Return data and reset measurement variables
  // -------------------------------------------------------------------------------------------------
  if (jSample==numSamples-1)
  {
    if (sendData)
    {
      Serial.print(avg_A0*ANALOG_PORT_CONVERSION);
      sendData=false;
    }

    resetVariables();
  }

  // Perform continous measurement
  // --------------------------------------------------------------------------------------------------
  int val_A0=analogRead(A0);
  avg_A0+=val_A0/float(numSamples);
  jSample++;

}

void resetVariables()
{
  jSample=0;
  avg_A0=0;
}
