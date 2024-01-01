void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  int value = analogRead(A0);

  Serial.print("PotRaw: ");
  Serial.print(value);
  int voltage = (value)/(2^10 - 1) * 5;
  Serial.print(", Pot: ");
  Serial.println(voltage);
  delay(100);
  */
  
  /*
  testing your capture program function:
  float start = micros();
  for (int i = 0; i < 1000; i++) {
    int value = analogRead(A0);
  }
  Serial.println(micros() - start);
  */
}
