float thresholdVoltage;
int threshold;
bool carRunning;
bool restartCar = true;  //For debugging threshold
int count;
int lightSensor;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);  // Pin A0
  carRunning = true;
  thresholdVoltage = 2.5;
  threshold = thresholdVoltage * 1023 / 5;  // Turning a threshold voltage into a value understood by Arduino
  count = 0;
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  lightSensor = analogRead(A0);
  if (carRunning) {
    digitalWrite(13, HIGH);
    delay(200);
    Serial.print("Light ");
    Serial.print(float(lightSensor) * 5 / 1023);
    Serial.println("V");
  } else
    digitalWrite(13, LOW);

  if (lightSensor < threshold) {
    count++; 
    if (count > 4 ) {
    carRunning = false;
    Serial.println("Car stopped");
    }
    Serial.print("Dark ");
    Serial.print(float(lightSensor) * 5 / 1023);
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
