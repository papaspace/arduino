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

  TIK = list()
  DC = list()
  UA = list()
  IL = list()
  Iset = list()
  TIME = list()
  
  t0=time.time()

  while True:
    try:

      line=arduino.read(sleep=0.1)
      print(line)
      
      line.removesuffix('\r\n')
      tokens=line.split(",")

      values=[float(tokens[0]), float(tokens[1]), float(tokens[2]), float(tokens[3]), float(tokens[4])]

      t=time.time()

      TIK.append(values[0])
      DC.append(values[1])
      UA.append(values[2])
      IL.append(values[3])
      Iset.append(values[4])
      TIME.append(t-t0)

      

      plt.figure(figsize=(10,10))
      plt.plot(TIME, UA, marker='.', color='b', label="UA")
      plt.plot(TIME, IL, marker='.', color='r', label="IL")
      plt.plot(TIME, Iset, marker='.', color='g', label="I_set")
      plt.legend()
      plt.xlabel("Time / s")
      plt.ylabel("")
      plt.grid(True)
      plt.savefig("myfig.png")
      plt.close()

      data = np.array([TIK, DC, UA, IL, Iset, TIME]).T
      np.savetxt("./data.csv",data, delimiter=",", header="TIK, DC, UA, IL, Iset, TIME")

    except KeyboardInterrupt:
      exit()

    except Exception as e:
      print(e)
      pass
