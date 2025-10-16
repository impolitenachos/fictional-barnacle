float thresholdVoltage;
int threshold;
bool carRunning;
bool restartCar = true;  // For debugging threshold
int count;
int lightSensor;
bool debugMotor = true;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);  // Pin A0
  carRunning = true;
  thresholdVoltage = .92;
  threshold = thresholdVoltage * 1023 / 3.3;  // Convert threshold voltage to ADC value
  count = 0;
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  lightSensor = analogRead(A4);
  float voltage = float(lightSensor) * 3.3 / 1023;
  float timeSec = millis() / 1000.0;

  if (carRunning && lightSensor > threshold) {
    digitalWrite(13, HIGH);
    delay(200);
  } else {
    digitalWrite(13, LOW);
  }

  if (lightSensor < threshold) {
    Serial.print("Dark ");
    if (!debugMotor) count++;
    if (count > 4) {
      carRunning = false;
      Serial.print("Car stopped");
    }
    delay(200);
  } else {
    Serial.print("Light ");
    if (restartCar) {
      carRunning = true;
      count = 0;
    }
  }

  // ✅ Print in format usable by Serial Plotter
  if (carRunning) {
    Serial.print("Voltage:");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print("Time:");
    Serial.println(timeSec);
  }
}
