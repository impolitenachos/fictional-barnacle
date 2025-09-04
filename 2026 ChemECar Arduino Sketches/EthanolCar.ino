void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(6, HIGH);
  Serial.println("ON");
  delay(1000);
  digitalWrite(6, LOW);
  Serial.println("OFF");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
