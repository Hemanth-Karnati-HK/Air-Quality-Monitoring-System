#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int measurePin = A2;
const int ledPower = 3;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ledPower, OUTPUT);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int mq7Value = analogRead(A0);
  int mq135Value = analogRead(A1);

  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  float dustValue = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);

  Serial.print(mq7Value);
  Serial.print(",");
  Serial.print(mq135Value);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.println(dustValue);

  delay(60000); 
}
