void setup() {
  Serial.begin(115200);
  pinMode(A0, OUTPUT);
  pinMode(A7, INPUT);
  int x = analogRead(A7);
  int res = result(x);
  Serial.println(res);
  digitalWrite(A0, res);
}

void loop() {
    // Serial.println(digitalRead(A0));
}

int result(int dt){
	return dt * 1000000 / 1515152;
}