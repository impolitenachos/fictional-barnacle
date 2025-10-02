bool carRunning;
float voltage;
bool restartCar = true;
int count;
float voltages[5] = {0};
unsigned long times[5] = {0}; 
float derLight;
bool stableInitialVoltage;
float initialVoltage;
bool reactionStarted = false;
bool reactionStopped = false;

const int NUM_SAMPLES = 10;
const float DERIV_THRESHOLD = 0.0001;  // threshold for "flat"
const int STABLE_COUNT_REQUIRED = 5;   // number of stable readings to confirm

int stableCount = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(18, INPUT);  // Pin A4
  pinMode(6, OUTPUT);
  carRunning = true;
  count = 0;
  Serial.begin(9600);
  stableInitialVoltage = false;

  digitalWrite(13, HIGH); // keep ON initially
}

void loop() {
  digitalWrite(6, HIGH);
  voltage = readAveragedVoltage(18, NUM_SAMPLES);

  // Shift arrays
  for (int i = 0; i < 4; i++) {
    voltages[i] = voltages[i+1]; 
    times[i] = times[i+1];
  }

  // Store new sample
  voltages[4] = voltage;
  times[4] = millis();

  // Establish stable initial voltage
  if (!stableInitialVoltage) {
    bool allEqual = true;
    for (int i = 1; i < 5; i++) {
      if (fabs(voltages[i] - voltages[0]) > 0.01) {
        allEqual = false;
        break;
      }
    }
    if (allEqual) {
      stableInitialVoltage = true;
      initialVoltage = voltages[0];
    }
  }

  // Derivative
  derLight = changeinLight(times, voltages);

  // --- Detect reaction start ---
  if (!reactionStarted && fabs(derLight) > DERIV_THRESHOLD * 10) {  
    // “10x threshold” so we only start when slope is really moving
    reactionStarted = true;
    stableCount = 0; // reset
  }

  // --- Detect reaction stop (only after start) ---
  if (reactionStarted && !reactionStopped) {
    if (fabs(derLight) < DERIV_THRESHOLD) {
      stableCount++;
      if (stableCount >= STABLE_COUNT_REQUIRED) {
        // Check we ended at a different level than initial
        if (fabs(voltage - initialVoltage) > 0.05) { 
          reactionStopped = true;
          digitalWrite(13, LOW);   // turn OFF only when final state reached
        }
      }
    } else {
      stableCount = 0; // reset if slope not flat
    }
  }

  // Print
  Serial.print(times[4]/1000.0);
  Serial.print(", ");
  Serial.print(voltage, 3);
  Serial.print(", ");
  Serial.print(derLight, 6);
  Serial.print(", ");
  Serial.println("reactionStopped");
  Serial.println(reactionStopped);
  Serial.println("reactionStopped");
  Serial.println(reactionStopped);
  Serial.println("reactionStarted");
  Serial.println(reactionStarted);

  if (stableInitialVoltage && initialVoltage > 0 && voltage > 0) {
    Serial.println(log(voltage/initialVoltage), 9);
  } else {
    Serial.println("N/A");
  }

  delay(200);
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

// --- Derivative calculator ---
float changeinLight(unsigned long times[], float voltages[]) {
  float difVoltage1 = voltages[4] - voltages[3];
  unsigned long difTime1 = times[4] - times[3];
  float lightSlope1 = (difTime1 != 0) ? difVoltage1 / float(difTime1) : 0;

  float difVoltage2 = voltages[3] - voltages[2];
  unsigned long difTime2 = times[3] - times[2];
  float lightSlope2 = (difTime2 != 0) ? difVoltage2 / float(difTime2) : 0;

  float difVoltage3 = voltages[2] - voltages[1];
  unsigned long difTime3 = times[2] - times[1];
  float lightSlope3 = (difTime3 != 0) ? difVoltage3 / float(difTime3) : 0;

  float difVoltage4 = voltages[1] - voltages[0];
  unsigned long difTime4 = times[1] - times[0];
  float lightSlope4 = (difTime4 != 0) ? difVoltage4 / float(difTime4) : 0;

  return (lightSlope1 + lightSlope2 + lightSlope3 + lightSlope4) / 4.0;
}
