
// We perform one measurement in each loop. The data gathered in each loop shall be averaged.
// After the full measurement cycle is completed, we send the measurement data via the serial port.

// Constants
float ANALOG_PORT_CONVERSION=5.0/1023.0;  // Analog port conversion factor (voltage / digital resolution)

// Measurement configuration
int DOUT_PWM=3;                           // (D) PWM output port (LED brightness)
float avg_A0;                             // (A) Measured voltage drop (Port A0)
float ravg_A0;
float ravg_A1;
int numSamples=1023;                      // Number of measurement samples (loops)
int jSample;                              // Current measurement sample index
bool sendData;                            // If true, measurement data will be sent via the serial port

String inputStr="";

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
  // Read serial input data incrementally until escape character '\n', then call reset()
  // ------------------------------------------------------------------------------------------------------------------
  if (Serial.available())
  {
    inputStr+=Serial.readString();

    if (inputStr[inputStr.length()-1]=='\n' && inputStr.length()>3)
    {
      reset();                              // Reset measurement
      sendData=true;                        // Trigger serial data reply when measurement is done
      inputStr="";
    }    
  }  

  // Return data and reset measurement variables
  // ------------------------------------------------------------------------------------------------------------------
  if (jSample==numSamples-1)
  {
    if (sendData)
    {
      Serial.print(ravg_A0, 6);
      Serial.print(",");
      Serial.print(ravg_A1, 6);
      //Serial.print(",IRD[uA],");
      //Serial.print((avg_A0-avg_A1)/RD*1.0E6);
      sendData=false;
    }

    //reset();
  }  

  // Perform continous measurement
  // ------------------------------------------------------------------------------------------------------------------
  float act_A0=float(analogRead(A0))*ANALOG_PORT_CONVERSION;
  float act_A1=float(analogRead(A1))*ANALOG_PORT_CONVERSION;
  avg_A0+=act_A0/float(numSamples);

  // Recursive average: (a[0] + (n - 1) * average(a[1:])) / n
  jSample++;
  ravg_A0 = (act_A0 + (jSample-1) * ravg_A0) / jSample;
  ravg_A1 = (act_A1 + (jSample-1) * ravg_A1) / jSample;
  
}

void reset()
{
  // Measurement setup
  DOUT_PWM=inputStr.substring(0,2).toInt();
  int PWM_DUTY_CYCLE=inputStr.substring(2).toInt(); 

  //analogWrite(DOUT_PWM, PWM_DUTY_CYCLE);              // Normal PWM
  OCR2B=PWM_DUTY_CYCLE;                               // Fast PWM

  // Reset variables
  jSample=0;
  avg_A0=0;
  ravg_A0=0;
  ravg_A1=0;

  delayMicroseconds(160);
}
