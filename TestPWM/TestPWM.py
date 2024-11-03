import os, sys

pwd=os.path.dirname(__file__)
sys.path.append(os.path.join(pwd, '..'))

from Arduino import arduino

#arduino=arduino('/dev/ttyACM0')
arduino=arduino('COM3')
arduino.connect()

if arduino.connection:

  while True:
    num=input("Enter a number: ")
    value=arduino.write_read(num)
    print(value)


