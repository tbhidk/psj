// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  int lum = analogRead(A0);
  Serial.println(lum);
}
