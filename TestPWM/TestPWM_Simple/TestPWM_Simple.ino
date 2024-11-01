int LED = 3;
int val_A0;

int Fadeschritte = 5;  
int Helligkeit = 0;  

void setup()
{
  Serial.begin(115200); 
  Serial.setTimeout(1);
}

void loop() 
{

  analogWrite(LED, Helligkeit);
  Helligkeit = Helligkeit + Fadeschritte;    
  delay(25);
  if (Helligkeit == 0 || Helligkeit == 255) 
  {
    Fadeschritte = -Fadeschritte;  
  }

  val_A0=analogRead(A0);
  Serial.println(val_A0);

}
