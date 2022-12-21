void setup() {
  Serial.begin(115200);
  pinMode(A0, OUTPUT);
}

void loop() {
  int x = analogRead(A0);
  Serial.println(x);
  delay(1000);
}
