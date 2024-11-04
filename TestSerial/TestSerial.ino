int x;
String inputStr="";

void setup() { 
  Serial.begin(115200); 
  Serial.setTimeout(1);
  delay(100);
  Serial.write("Hello, I am ready");
}

void loop() {

  if (Serial.available())
  {
    //delay(10);                            // We still need a small delay until we can read data from the serial buffer

    inputStr += Serial.readString();
    if (inputStr[inputStr.length()-1]=='\n')
    {
      x = inputStr.toInt(); 
      Serial.print(x+1);      
      inputStr="";
    }
  }

}
