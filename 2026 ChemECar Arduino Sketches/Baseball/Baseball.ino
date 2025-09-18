float thresholdVoltage;
int threshold;
bool carRunning;
bool restartCar = true;  //For debugging threshold
int count;
int lightSensor;
bool debugMotor = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);  // Pin A0
  carRunning = true;
  thresholdVoltage = 2.0;
  threshold = thresholdVoltage * 1023 / 3.3;  // Turning a threshold voltage into a value understood by Arduino
  count = 0;
  Serial.begin(9600);
  delay(1000);
  //TODO: 
  /* Add blinking LED to show it's ready
  * Maybe turn open valve?? 
  */
}

void loop() {
  // put your main code here, to run repeatedly 
  lightSensor = analogRead(A0);
  if (carRunning && lightSensor < threshold) {
    //TODO: add solid LED code 
    digitalWrite(13, HIGH);
    delay(200);
    Serial.print("Light ");
    Serial.print(float(lightSensor) * 3.3 / 1023);
    Serial.println("V");
    Serial.println(millis()/1000.0);
  } else
    digitalWrite(13, LOW);

  if (lightSensor > threshold) {
    if(!debugMotor)
    count++; 
    if (count > 4 ) {
    carRunning = false;
    Serial.println("Car stopped");
    // TODO: turn off LED
    }
    Serial.print("Dark ");
    Serial.print(float(lightSensor) * 3.3 / 1023);
    Serial.print("V. ");
    Serial.print(count);
    Serial.println(" times. ");
    Serial.println(millis()/1000.0);
    delay(200);
  } else {
    if (restartCar) {
      carRunning = true;
      count = 0;
    }
  }
}
