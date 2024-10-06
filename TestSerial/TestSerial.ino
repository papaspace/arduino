int x;

void setup() { 
  Serial.begin(115200); 
  Serial.setTimeout(1);
  delay(100);
  Serial.write("Hello, I am ready");
}

void loop() {

  while (!Serial.available());          // Do nothing as long as the serial buffer is empty
  delay(10);                            // We still need a small delay until we can read data from the serial buffer
  x = Serial.readString().toInt(); 
  Serial.print(x + 1);

}
