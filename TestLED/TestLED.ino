int led1=7;
int led2=13;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}


void loop() {
  // Hier schalten wir die LEDs ein
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);

  // Warten
  delay(100);

  // Hier schalten wir die LEDs aus
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);

  // Warten
  delay(100);
}
