import os, sys
import matplotlib.pyplot as plt
import numpy as np
import time

pwd=os.path.dirname(__file__)
sys.path.append(os.path.join(pwd, '..'))

from Arduino import arduino

msgs=[]

arduino=arduino('COM3')
arduino.connect()

if arduino.connection:

  TIK_list = list()
  DC_list = list()
  UA_list = list()
  U1_list = list()
  IL_list = list()
  Iset_list = list()
  TIME_list = list()
  ERROR_list = list()
  
  t0=time.time()

  while True:
    try:

      t=time.time()

      # Read input data from serial port
      line=arduino.read(sleep=0.1)
      print(line)
      
      line.removesuffix('\r\n')
      tokens=line.split(",")

      [TIK, DC, UA, U1, IL, Iset, ERROR]=[float(token) for token in tokens]

      # Store data in lists
      TIME_list.append(t-t0)
      TIK_list.append(TIK)
      DC_list.append(DC)
      UA_list.append(UA)
      U1_list.append(U1)
      IL_list.append(IL)
      Iset_list.append(Iset)
      ERROR_list.append(ERROR)

      # Plot IL as function of duty cycle DC
      plt.figure(figsize=(10,10))
      plt.scatter(DC_list, IL_list, marker='.', color='b', label="IL")
      plt.legend()
      plt.xlim([0,255])
      plt.xlabel("DC")
      plt.ylabel("IL")
      plt.grid(True)
      plt.savefig("IL_DC.png")
      plt.close()

      # Plot results with PID
      plt.figure(figsize=(10,10))
      plt.plot(TIME_list, UA_list, marker='', color='b', label="UA")
      plt.plot(TIME_list, ERROR_list, marker='', color='k', label="ERROR")
      plt.plot(TIME_list, IL_list, marker='', color='r', label="IL")
      plt.plot(TIME_list, Iset_list, marker='', color='g', label="I_set")
      plt.ylim([-2,8])
      plt.legend()
      plt.xlabel("Time / s")
      plt.ylabel("")
      plt.grid(True)
      plt.savefig("myfig.png")
      plt.close()

      data = np.array([TIME_list, TIK_list, DC_list, UA_list, U1_list, IL_list, Iset_list, ERROR_list]).T
      np.savetxt("./data.csv",data, delimiter=",", header="TIME, TIK, DC, UA, U1, IL, Iset, ERROR")

    except KeyboardInterrupt:
      exit()

    except Exception as e:
      print(e)
      pass
