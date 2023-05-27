#define SM_PIN A5
#define TEMP_PIN A1
#define RAIN_PIN A0
#define BLUE_LED 11
#define WHITE_LED 12

#define SOIL_THRESHOLD 100
#define RAIN_THRESHOLD 100
// daca temp < 10 grade nu pornim
// daca soil is dry si nu ploua, pornim pompa
// daca soil is wet, oprim pompa

//blue led -> pompa pornita
//white_led -> pompa oprita

void setup() {
  pinMode(BLUE_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int temp_value = analogRead(TEMP_PIN);

  int sm_value = analogRead(SM_PIN);

  int rain_value = analogRead(RAIN_PIN);
  int rain_output = readSensor(rain_value);

  int tempCelsius = getTempInCelsius(temp_value);

  if (tempCelsius > 10.0) {
    if (sm_value < SOIL_THRESHOLD && rain_output < RAIN_THRESHOLD) {
      startPump();
    } else {
      stopPump();
    }
  }
  else {
    stopPump();
  }

  Serial.print("Soil value ");
  Serial.println(sm_value);

  Serial.println(readSensor(rain_value));

  Serial.print("Temperature: ");
  Serial.println(getTempInCelsius(temp_value));

  Serial.println("");

  delay(4000);
}

int readSensor(int rain_value) {
  int outputValue = map(rain_value, 0, 1023, 255, 0);
  return outputValue;
}

float getTempInCelsius(int temp_value) {
  float voltage = temp_value * (5.0 / 1024.0);
  float temperatureC = voltage * 100;
  return temperatureC;
}

void startPump() {
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(WHITE_LED, LOW);
}

void stopPump() {
  digitalWrite(WHITE_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
}
