bool carRunning;
float voltage;
bool restartCar = true;
int count;
float voltages[3] = {0};
unsigned long times[3] = {0}; // use unsigned long for millis()
float derLight;

// Number of ADC samples per averaged reading
const int NUM_SAMPLES = 10;

void setup() {
  pinMode(6, OUTPUT);
  pinMode(18, INPUT);  // Pin A4
  carRunning = true;
  count = 0;
  Serial.begin(9600);
}

void loop() {
  // Read smoothed voltage
  voltage = readAveragedVoltage(18, NUM_SAMPLES);

  // Shift arrays (keep last 3 samples)
  for (int i = 0; i < 2; i++) {
    voltages[i] = voltages[i+1]; 
    times[i] = times[i+1];
  }

  // Store new sample + time
  voltages[2] = voltage;
  times[2] = millis();

  // Compute derivative
  derLight = changeinLight(times, voltages);

  // Print results
  Serial.print(times[2]/1000.0);
  Serial.print(", ");
  Serial.print(voltage, 3);
  Serial.print(", ");
  Serial.println(derLight, 6);

  delay(200); // control logging rate
}

// --- Moving average voltage reader ---
float readAveragedVoltage(int pin, int samples) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
  }
  float avgReading = float(total) / samples;
  return avgReading * 3.3 / 1023.0; // convert to volts
}

// --- Derivative calculator (average of last 2 slopes) ---
float changeinLight(unsigned long times[], float voltages[]) {
  float difVoltage1 = voltages[2] - voltages[1];
  unsigned long difTime1 = times[2] - times[1];
  float lightSlope1 = 0;

  float difVoltage2 = voltages[1] - voltages[0];
  unsigned long difTime2 = times[1] - times[0];
  float lightSlope2 = 0;

  if (difTime1 != 0) {
    lightSlope1 = difVoltage1 / float(difTime1);
  }
  if (difTime2 != 0) {
    lightSlope2 = difVoltage2 / float(difTime2);
  }

  float lightSlope = (lightSlope1 + lightSlope2) / 2.0;
  return lightSlope;
}
