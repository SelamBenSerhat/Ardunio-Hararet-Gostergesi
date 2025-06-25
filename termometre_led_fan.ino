const int sensorPin = A0;
const int fanRelayPin = 12;
const int ledPin = 5;

const float fixedResistor = 328.0;
const float beta = 3950.0;
const float T0 = 296.15;
const float R0 = 7500.0;

unsigned long previousMillis = 0;
bool ledState = false;

void setup() {
  Serial.begin(9600);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(fanRelayPin, LOW);
}

void loop() {
  int analogValue = analogRead(sensorPin);
  float voltage = analogValue * (5.0 / 1023.0);
  float Rt = voltage * fixedResistor / (5.0 - voltage);
  float tempKelvin = 1.0 / ((1.0 / beta) * log(Rt / R0) + (1.0 / T0));
  float tempCelsius = tempKelvin - 273.15;

  Serial.print("Gerilim: ");
  Serial.print(voltage);
  Serial.print(" V\t");

  Serial.print("Direnç: ");
  Serial.print(Rt);
  Serial.print(" ohm\t");

  Serial.print("Sıcaklık: ");
  Serial.print(tempCelsius);
  Serial.println(" °C");

  // Fan kontrolü
  if (tempCelsius >= 90.0) {
    digitalWrite(fanRelayPin, HIGH);
  } else if (tempCelsius <= 80.0) {
    digitalWrite(fanRelayPin, LOW);
  }

  // LED davranışı
  if (tempCelsius <= 60.0) {
    analogWrite(ledPin, 51);  // %20 sabit
  }
  else if (tempCelsius > 60.0 && tempCelsius <= 80.0) {
    analogWrite(ledPin, 128); // %50 sabit
  }
  else if (tempCelsius > 80.0 && tempCelsius <= 105.0) {
    blinkLed(128, 500);       // %50 yanıp sönme
  }
  else if (tempCelsius > 105.0) {
    blinkLed(255, 500);       // %100 yanıp sönme
  }

  delay(100);
}

void blinkLed(int brightness, unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    analogWrite(ledPin, ledState ? brightness : 0);
  }
}
