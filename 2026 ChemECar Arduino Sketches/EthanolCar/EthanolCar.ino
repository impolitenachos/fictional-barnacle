float thresholdVoltage;
int threshold;
bool carRunning;
bool restartCar = true;  //For debugging threshold
int count;

void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  pinMode(18, INPUT);  // Pin A4
  carRunning = true;
  thresholdVoltage = 2.5;
  threshold = thresholdVoltage * 1023 / 5;  // Turning a threshold voltage into a value understood by Arduino
  count = 0;
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (carRunning) {
    digitalWrite(6, HIGH);
    delay(200);
    Serial.print("Car running ");
    Serial.print(float(analogRead(18)) * 5 / 1023);
    Serial.println("V");
  } else
    digitalWrite(6, LOW);

  if (analogRead(18) < threshold) {
    count++; 
    if (count > 4 ) {
    carRunning = false;
    Serial.println("Car stopped");
    }
    Serial.print("Reaction below threshold ");
    Serial.print(float(analogRead(18)) * 5 / 1023);
    Serial.print("V. ");
    Serial.print(count);
    Serial.println(" times. ");
    delay(200);
  } else {
    if (restartCar) {
      carRunning = true;
      count = 0;
    }
  }
}
