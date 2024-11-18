import os, sys
import matplotlib.pyplot as plt
import numpy as np

pwd=os.path.dirname(__file__)
sys.path.append(os.path.join(pwd, '..'))

from Arduino import arduino


PWM_PIN="03"
RD=268.5               # LED pre-resistor [Ohm]
msgs=[]

#arduino=arduino('/dev/ttyACM0')
arduino=arduino('COM3')
arduino.connect()

if arduino.connection:

  while False:
    num=input("Enter a number: ")
    value=arduino.write_read(num)
    print(value)
    
  plt.figure(figsize=(10,10))
  x=0
  VD_list = list()
  ID_list = list()
  while True:
    value=arduino.write_read("%s%d"%(PWM_PIN, x))
    msg="%d,%s"%(x, value)
    msgs.append(msg.split(","))
    print(msg)
    x+=2
    if x>=256:
      break

    A0=float(msgs[-1][2])   # LED voltage (-)
    A1=float(msgs[-1][1])   # LED voltage (+)
    VD=A1-A0
    ID=A0/RD                # LED pre-resistor current
    VD_list.append(VD)
    ID_list.append(ID)
    print("UD = %9.6f, ID = %9.6f "%(VD, ID))

    plt.scatter(VD, ID, marker='.', color='b')
    plt.xlim([0, 2])
    plt.ylim([0, 0.02])
    plt.xlabel("UD")
    plt.ylabel("ID")
    plt.grid(True)
    plt.savefig("myfig.png")

  x=0
  value=arduino.write_read("%s%d"%(PWM_PIN, x))

  plt.close()
  data = np.array([VD_list, ID_list]).T
  np.savetxt("./data.csv",data, delimiter=",", header="UD, ID")
