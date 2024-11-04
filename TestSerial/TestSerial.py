
import os, sys

pwd=os.path.dirname(__file__)
sys.path.append(os.path.join(pwd, '..'))

from Arduino import arduino


#arduino=arduino('/dev/ttyACM0')
arduino=arduino('COM3')
arduino.connect()

if arduino.connection:
    
  while False:
    num=input("Enter a number: ")
    value=arduino.write_read(num)
    print(value)

  x=110
  while True:
    msg=arduino.write_read('%d\n'%x)
    y=int(msg)
    print('tx=%d msg=%d'%(x, y))
    if y-x!=1:
      print('!!!')
    x=x+1

