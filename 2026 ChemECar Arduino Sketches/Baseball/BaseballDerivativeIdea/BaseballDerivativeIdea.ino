bool carRunning = false;
float voltage;
int HighNum = 0;          // counter for "low" derivative (stop)
int HighStartCount = 0;   // counter for "high" derivative (start)
int count = 0;
bool carOperating = true;

const float ThresholdChange = 0.005;  // baseline V/s threshold
// Optional hysteresis: slightly different start/stop thresholds
const float StartThreshold = 0.010;    // could be 0.006
const float StopThreshold  = ThresholdChange;    // could be 0.004

const int NUM_SAMPLES = 10;
const unsigned long SAMPLE_DELAY = 200;  // ms
const int CONFIRM_COUNT = 3;             // consecutive samples required

float voltages[3] = {0};
unsigned long times[3] = {0};
float derLight;

void setup() {
  pinMode(12, OUTPUT); // Motor control (MOSFET gate)
  pinMode(13, OUTPUT); // LED indicator
  pinMode(18, INPUT);  // Analog input (A4)
  Serial.begin(9600);
  delay(1000);
  Serial.println("Time(s), Voltage(V), dV/dt(V/s)");
}

void loop() {
  if (carOperating) {
  // --- Read and compute derivative ---
  voltage = readAveragedVoltage(18, NUM_SAMPLES);
  shiftData(voltages, times, voltage, millis());
  derLight = changeInLight(times, voltages);

  Serial.print(times[2] / 1000.0, 2);
  Serial.print(", ");
  Serial.print(voltage, 3);
  Serial.print(", ");
  Serial.println(derLight, 4);

  // --- START condition (noise protected) ---
  if (!carRunning) {
    if (fabs(derLight) > StartThreshold) {
      HighStartCount++;
    } else {
      HighStartCount = 0;
    }

    if (HighStartCount >= CONFIRM_COUNT) {
      carRunning = true;
      digitalWrite(12, HIGH); // Turn on MOSFET (start motor)
      digitalWrite(13, HIGH); // Turn on LED
      Serial.println("Derivative HIGH (confirmed) -> Starting car motor.");
    }
  }

  // --- STOP condition (noise protected) ---
  if (carRunning) {
    if (fabs(derLight) < StopThreshold) {
      HighNum++;
    } else {
      HighNum = 0;
    }

    if (HighNum >= CONFIRM_COUNT && count > 20) {
      carRunning = false;
      digitalWrite(12, LOW); // Turn off MOSFET (stop motor)
      digitalWrite(13, LOW); // Turn off LED
      Serial.println("Derivative LOW (confirmed) -> Stopping car motor.");
      carOperating = false;
    }

    count++;
  }

  delay(SAMPLE_DELAY);
  } 
}

// --- Helper: shift old data, insert new sample ---
void shiftData(float v[], unsigned long t[], float newV, unsigned long newT) {
  for (int i = 0; i < 2; i++) {
    v[i] = v[i + 1];
    t[i] = t[i + 1];
  }
  v[2] = newV;
  t[2] = newT;
}

// --- Helper: average multiple ADC samples ---
float readAveragedVoltage(int pin, int samples) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
  }
  float avgReading = float(total) / samples;
  return avgReading * 3.3 / 1023.0;
}

// --- Helper: calculate derivative (V/s) ---
float changeInLight(unsigned long times[], float voltages[]) {
  float dv1 = voltages[2] - voltages[1];
  float dt1 = (times[2] - times[1]);
  float dv2 = voltages[1] - voltages[0];
  float dt2 = (times[1] - times[0]);

  float slope1 = (dt1 > 0) ? (dv1 / dt1) * 1000.0 : 0;  // convert to V/s
  float slope2 = (dt2 > 0) ? (dv2 / dt2) * 1000.0 : 0;

  return (slope1 + slope2) / 2.0;
}
