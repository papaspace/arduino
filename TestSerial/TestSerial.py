import serial
import serial.tools.list_ports
import time


class arduino:
  def __init__(self, port_name):
    self.port_name=port_name
    self.connection=None
    
  def connect(self, baudrate=115200, timeout=0.1):
    print('Connecting (port="'"%s>"'", baud rate=%d) ...'%(self.port_name, baudrate))
    ports=serial.tools.list_ports.comports()
    for port in ports:
      if port.device==self.port_name:
        try:
          self.connection=serial.Serial(port=port.device, baudrate=baudrate, timeout=timeout)
          print('Connection established')
          while True:
            msg=self.connection.readline()
            if msg==b'':  
              time.sleep(0.1)
            else:
              print('Handshake message: %s'%msg)
              break
          return self.connection
        except Exception as ex:
          print('Unable to establish connection: %s'%ex)
          return self.connection

    raise ValueError('Unable to find port '+self.port_name)

  def write_read(self, x):
    self.connection.write(bytes(x, 'utf-8'))
    time.sleep(0.2)
    data=self.connection.readline()
    return data

arduino=arduino('/dev/ttyACM0')
arduino.connect()

if arduino.connection:

  while False:
    num=input("Enter a number: ")
    value=arduino.write_read(num)
    print(value)

  x=110
  while True:
    msg=arduino.write_read('%d'%x)
    y=int(msg)
    print('tx=%d msg=%d'%(x, y))
    if y-x!=1:
      print('!!!')
    x=x+1

