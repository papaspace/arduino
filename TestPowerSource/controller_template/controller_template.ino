#include <Arduino.h>



#define RS 12.2     //shuntwiderstand

#define outMax 253  //PWM Limit
#define outMin 2
#define iMax 6      //current Limit
#define iMin 0.2      

// TODO pin defines
#define v_pin  A0           // Ua
#define i_pin1 A1           // U1 
#define i_pin2 A2           // U2
#define pwm_pin 5           // PWM output pin

#define PS 8                // muss richtig gesetz werden in setup
#define f_cpu 1600000       // am arduino überprüfen!
#define f_timer f_cpu/8
//#define comp 5
#define fPWM 970
#define AVG_PWM_PERIOD 50
double dT =  0.0001;

//double dT = comp * ((double)   PS/f_timer));
uint8_t comp =  ceil((dT/PS) * f_timer);
uint16_t N_AVG = 100;             //Samples average analog in
uint16_t INTERVAL = N_AVG*10;     //intervall serial out  
uint32_t INTERVAL2 =  50000;      //intervall someting ...

double sample_time = N_AVG * dT;


volatile uint16_t timerCount = 0;
volatile uint32_t timerCount2 = 0;
volatile uint16_t sumCount = 0;
volatile uint32_t v_sum_A0 = 0;      //type of this needs to change with n_avg
volatile uint32_t v_sum_A1 = 0;      //type of this needs to change with n_avg
volatile int64_t i_sum = 0; 


double errSum = 0;          //Integralanteilr
double lastErr = 0;         //Fehle für differentielen anteil
double error = 0; 

//Kritische Parameter für Zigle nichols eingeben
#define Ku 200.0        // kritische verstärkung Zigler-Nichols
#define Tu 1.0          // Periodendauer ZS

//Reglerparameter
double Kp = Ku*0.45;
double Ki = Kp/(0.83*Tu);
double Kd = 0.0;

//double Kp = Ku*0.33;
//double Ki = 0.66*Ku/Tu;
//double Kd = 0.0;//0.105*Ku*Tu;

//double Kp = 500.0;
//double Ki = 0.0;
//double Kd = 0.0;


//Stellgröße, wirkt auf Regeltrecke
volatile int16_t duty_cycle = 0;
       

double i_set = iMin+(iMax-iMin)*0.5;      //Stellgröße
double v_avg_A0 = 0;     //Mittelwert Spannung (Ausgang)
double v_avg_A1 = 0;     //Mittelwert Spannung (Shunt)
double i_avg_RS = 0;     //Mitlwert Strom
int tik = 0;
      
uint8_t enablePID = 1;

String output="";

void setup() {
  Serial.begin(115200); //Serielle verbindung
  
  // Adjusts the pin to input mode
  pinMode(v_pin, INPUT);
  pinMode(i_pin1, INPUT);
  pinMode(i_pin2, INPUT);
  
  // Adjusts the pin to output mode
  pinMode(pwm_pin, OUTPUT);

  // Adjusts the PWM frequency to 980.39 Hz.
  // By default arduino PWM frequency is 490.20 Hz, 
  // but this value is not high enough to have a stable DC output voltage in low duty cycles.
  TCCR2B = TCCR2B & B11111000 | B00000011;
  //analogWrite(pwm_pin, 120);



  //timer 1 für konstantes Zeitintervall konfigurieren
  TCCR1B =  1<< CS11 ;  // Prescaler auf 8;
  OCR1A = comp;         // Overflow nach ~ 0.104 ms
  TIMSK1 = 1<<OCIE1A;   // Interrupt bei Timer > OCR1A
  sei(); //global enable interrupts
}

void loop() {  
  //Hier soll nichts sein!
}

double PID(double i_real)
{
    // Step 2: Measure output voltage in dependance of duty cycle
    if (false)
    {
      tik+=1;
      if (tik>10)
      {
        duty_cycle = duty_cycle + 10;
        if (duty_cycle>255)
        {
          duty_cycle=0;
        }
        analogWrite(pwm_pin,duty_cycle);  
  
        tik=0;
      }
      return 13.0;
    }
    else
    {
      error = i_set - i_real;

      //TODO
      //errSum += 0; //Sum up error terms times dT
      {
        double dT = N_AVG * 104E-6;
        errSum += error * dT;
      }

      //Anti Windup
      //if(errSum > outMax) errSum= outMax*sample_time; //Limit output range
      //else if(errSum < outMin) errSum= outMin*sample_time;

      //TODO
      //double dErr = 1; //Time differential
      double dErr = (error - lastErr) / dT;

      //PID output Sum
      duty_cycle = Kp * error + Kd * dErr + Ki * errSum;

      //Limit output range
      if(duty_cycle > outMax) duty_cycle= outMax; //Limit output range
      else if(duty_cycle < outMin) duty_cycle= outMin;
      
      if(enablePID){
        analogWrite(pwm_pin,duty_cycle);  
      }  
      lastErr = error;
      return error;
    }

}

ISR(TIMER1_COMPA_vect)
{
      // Sum N_AVG terms of ADC results and average
      sumCount++;
      if(sumCount == N_AVG)
      {
        v_avg_A0 = 5.0/1023.0*((double)v_sum_A0/(double)N_AVG);
        v_avg_A1 = 5.0/1023.0*((double)v_sum_A1/(double)N_AVG);
        i_avg_RS = 1000.0*5.0/1023.0*((double)i_sum/((double)N_AVG));
        
        error = PID(i_avg_RS);
        
        // Reset variables
        v_sum_A0 = 0;
        v_sum_A1 = 0;
        i_sum = 0;
        sumCount = 0;
      }
      //TODO
      //v_avg += 0;  //Read analog Voltage
      //i_avg += 0;  //Read analog Current
      {
        double Ua = analogRead(v_pin);
        double U1 = analogRead(i_pin1);
        double U2 = analogRead(i_pin2);
        v_sum_A0 += Ua;
        v_sum_A1 += U1;
        i_sum += (U1 - Ua)/RS;       // Note: To improve accuracy, we postpone division by RS when calculating i_avg_f
      }

    // Increment the timer count
    timerCount2++;
    // Intervall2: do something every 5s
      if (timerCount2 >= INTERVAL2)   //do something every INTERVAL * 0.00002 s
      {
          timerCount2 = 0; // Reset timer count for the next interval
        i_set += (double) (iMax - iMin)/10;
        if(i_set>iMax) i_set = iMin;
    }
    
    timerCount++;

    // At INTERVAL: Print output via serial port
    if (timerCount >= INTERVAL)
    {
      double error = PID(i_avg_RS);
      timerCount = 0; // Reset timer count for the next interval

      String output2 = "";
      output2+=String(tik)+",";
      output2+=String(duty_cycle)+",";
      output2+=String(v_avg_A0)+",";
      output2+=String(v_avg_A1)+",";
      output2+=String(i_avg_RS)+",";
      output2+=String(i_set)+",";
      output2+=error;

      output=output2;

      Serial.println(output);
    }
  
}
