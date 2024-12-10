Step 2 ================================================================================================================

Performed measurement of output current IL as function of duty cycle with two different configurations:

- 1st configuration (101 nF, 100 kOhm --> tau = 10 ms)
    ==> Output current starts at duty cycle 100/255 and does not reach 40 mA output
        Suspected too much current flows via low-pass filters!
    ==> Stored results as Step 2.1

- 2nd configuration (10 nF, 1 MOhm    --> tau = 10 ms)
    ==> Goal: maximize the current through the buck converter by maximizing resistors of the low-pass filters!
        Reaching 35 mA (Arduino maximum is 40 mA)
    ==> Works better than 1st config
    ==> Stored results as Step 2.2


Step 7 ================================================================================================================

Manual calibration of PID parameters, target function from step 9.a

Step 7.1:   Ku =  3.0, Tu = 0.2 --> Kp = 1.35, Ki = 18.0
Step 7.2:   Ku = 20.0, Tu = 0.2 --> Kp = 9.0, Ki = 120.5


Step 8 ================================================================================================================

Stable oscillation was not observed for Kd 1.0 ... 200.0, therefore step 8 could not be performed.
Satisfied results achieved with results from Step 7.2 however


Step 9 ================================================================================================================


Efficiency ============================================================================================================

Unable to perform measurement