//float thresholdVoltage;
//int threshold;
bool carRunning;
float voltage;
float prevVoltage;
bool restartCar = true;  //For debugging threshold
int count;
float voltages[3] = {0};
float times[3] = {0};
float derLight;
float difVoltage1;
float difTime1;
float lightSlope1;
float difVoltage2;
float difTime2;
float lightSlope2;
float lightSlope;

void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  pinMode(18, INPUT);  // Pin A4
  carRunning = true;
  // thresholdVoltage = 2.5;
  // threshold = thresholdVoltage * 1023 / 5;  // Turning a threshold voltage into a value understood by Arduino
  count = 0;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  voltage = float(analogRead(18)) * 3.3 / 1023;
  for (int i=0; i<2; i++)
  {
  voltages[i] = voltages[i+1]; 
  times[i] = times[i+1];
  }
  voltages[2] = voltage;
  times[2] = millis();
  derLight = changeinLight(voltages, times);
  Serial.println(derLight);
  delay(200);
}

float changeinLight(float times[], float voltages[]) {
//Compute new derivative 
difVoltage1 = voltages[2]-voltages[1];
difTime1 = times[2]-times[1];
lightSlope1 = 0;
difVoltage2 = voltages[1]-voltages[0];
difTime2 = times[1]-times[0];
lightSlope2 = 0;
lightSlope = 0;

if (difTime1 != 0)
{
  lightSlope1 = difVoltage1/difTime1; 
}

if (difTime2 != 0)
{
  lightSlope2 = difVoltage2/difTime2;
}

lightSlope = (lightSlope1+lightSlope2) /2;

return lightSlope;
}