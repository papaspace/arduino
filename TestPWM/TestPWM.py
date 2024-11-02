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

          # Wait for the handshake message
          tik=time.time()
          while time.time()-tik<1000.0:
            msg=self.connection.readline()
            if msg==b'':  
              time.sleep(0.1)
            else:
              print('Connection established')
              print('Handshake message: %s'%msg)
              return self.connection

          print('Connection established')
          print('No handshake message received')
          return self.connection

        except Exception as ex:
          print('Unable to establish connection: %s'%ex)
          return self.connection

    raise ValueError('Unable to find port '+self.port_name)

  def write_read(self, x, sleep=0.01):
    self.connection.write(bytes(x, 'utf-8'))
    
    while True:
      msg=self.connection.readline()
      if msg==b'':
        time.sleep(sleep)
      else:
        return msg

#arduino=arduino('/dev/ttyACM0')
arduino=arduino('COM3')
arduino.connect()

if arduino.connection:

  while True:
    num=input("Enter a number: ")
    value=arduino.write_read(num)
    print(value)


