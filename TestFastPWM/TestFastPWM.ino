
// We perform one measurement in each loop. The data gathered in each loop shall be averaged.
// After the full measurement cycle is completed, we send the measurement data via the serial port.

// Constants
float ANALOG_PORT_CONVERSION=5.0/1024.0;  // Analog port conversion factor (voltage / digital resolution)
int led1=3;                               // (D) PWM output port (LED brightness control)
float RD=220.0;                           // LED pre-resistor [Ohm]

// Measurement configuration
float avg_A0;                             // (A) Measured voltage drop (Port A0)
float avg_A1;                             // (A) Measured voltage drop (Port A1)
int numSamples=1024;                      // Number of measurement samples (loops)
int jSample;                              // Current measurement sample index
bool sendData;                            // If true, measurement data will be sent via the serial port

void setup()
{
  Serial.begin(115200); 
  Serial.setTimeout(1);
  delay(100);
  Serial.write("Hello, I am ready");

  // Setup fast PWM
  // -------------------------------------------------------------------------------------------------
  // PWM_freq = clock_freq / (256 * prescaler) =
  //          = 16 MHz / (256 * prescaler)
  //
  // Additionally required settings:
  // Register setting               Timer no.  Bitmap
  // ------------------------------+---------+-------------------------------------------------------
  // Compare output mode      (COM)        2    10 : non-inverted PWM
  // Waveform generation mode (WGM)        2   011 : fast PWM

  pinMode(3, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = (0<<CS22) | (0<<CS21) | (1<<CS20);
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
    //analogWrite(led1, x);
    OCR2B=x;
    resetVariables();
    sendData=true;                        // Trigger serial data reply when measurement is done
  }

  // Return data and reset measurement variables
  // -------------------------------------------------------------------------------------------------
  if (jSample==numSamples-1)
  {
    if (sendData)
    {
      Serial.print("A0[V],");
      Serial.print(avg_A0);
      Serial.print(",A1[V],");
      Serial.print(avg_A1);
      Serial.print(",IRD[uA],");
      Serial.print((avg_A0-avg_A1)/RD*1.0E6);
      sendData=false;
    }

    resetVariables();
  }

  // Perform continous measurement
  // --------------------------------------------------------------------------------------------------
  avg_A0+=analogRead(A0)/float(numSamples)*ANALOG_PORT_CONVERSION;
  avg_A1+=analogRead(A1)/float(numSamples)*ANALOG_PORT_CONVERSION;

  jSample++;

}

void resetVariables()
{
  jSample=0;
  avg_A0=0;
  avg_A1=0;
}
