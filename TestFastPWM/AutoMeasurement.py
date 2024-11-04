import os, sys
import matplotlib.pyplot as plt
import numpy as np

pwd=os.path.dirname(__file__)
sys.path.append(os.path.join(pwd, '..'))

from Arduino import arduino


PWM_PIN="10"
RD=215.0        # LED pre-resistor [Ohm]
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
  
  while True:
    value=arduino.write_read("%s%d"%(PWM_PIN, x))
    msg="%d,%s"%(x, value)
    msgs.append(msg.split(","))
    print(msg)
    x+=1
    if x>=256:
      break

    #data=np.array(msgs).T

    A0=float(msgs[-1][1])   # LED voltage
    A1=float(msgs[-1][2])   # PWM voltage
    ID=(A1-A0)/RD           # LED pre-resistor current

    plt.scatter(A0, ID, marker='.', color='b')
    plt.xlim([0, 5])
    plt.ylim([0, 5.0/RD])
    plt.xlabel("UD")
    plt.ylabel("ID")
    plt.grid(True)
    plt.savefig("myfig.png")

  x=0
  value=arduino.write_read("%s%d"%(PWM_PIN, x))

  plt.close()

