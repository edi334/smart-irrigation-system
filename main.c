#include <LiquidCrystal.h>;

#define SM_PIN A5
#define TEMP_PIN A1
#define RAIN_PIN A0
#define BLUE_LED 9
#define WHITE_LED 10
#define EN_A 13
#define IN1 6
#define IN2 7

#define SOIL_THRESHOLD 100
#define RAIN_THRESHOLD 100

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// daca temp < 10 grade nu pornim
// daca soil is dry si nu ploua, pornim pompa
// daca soil is wet, oprim pompa

//blue led -> pompa pornita
//white_led -> pompa oprita

void setup() {
  lcd.begin(16, 2);
  lcd.print("Smart Irrigation");
  lcd.setCursor(4, 1);
  lcd.print("System!");
  pinMode(BLUE_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  Serial.begin(9600);
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_A,255);

  int temp_value = analogRead(TEMP_PIN);

  int sm_value = analogRead(SM_PIN);

  int rain_value = analogRead(RAIN_PIN);
  int rain_output = readSensor(rain_value);

  int tempCelsius = getTempInCelsius(temp_value);

  if (tempCelsius > 10.0) {
    if (sm_value < SOIL_THRESHOLD && rain_output < RAIN_THRESHOLD) {
      startPump(tempCelsius);
    } else {
      stopPump(tempCelsius);
    }
  }
  else {
    stopPump(tempCelsius);
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

void startPump(int tempCelsius) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Pompa pornita");
  lcd.setCursor(0, 1);
  lcd.print(tempCelsius);
  lcd.print(" grade Celsius");
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(WHITE_LED, LOW);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void stopPump(int tempCelsius) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Pompa oprita");
  lcd.setCursor(0, 1);
  lcd.print(tempCelsius); 
  lcd.print(" grade Celsius");
  digitalWrite(WHITE_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}
