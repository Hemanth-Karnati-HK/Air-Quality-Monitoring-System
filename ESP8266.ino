#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "Adafruit_CCS811.h"

const char* ssid = "HS40";
const char* password = "7013505113";
unsigned long myChannelNumber = 2492563;
const char* myWriteAPIKey = "SP0UU42BWDK5SLRM";

WiFiClient client;
Adafruit_CCS811 ccs;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  int attempts = 0;
  while (!ccs.begin() && attempts < 5) {
    Serial.println("Failed to start CCS811 sensor! Retrying...");
    delay(1000);
    attempts++;
  }
  if (attempts == 5) {
    Serial.println("Failed to initialize CCS811 after multiple attempts. Check the sensor.");
  }

  ThingSpeak.begin(client);
  Serial.println("ThingSpeak Started");
}

void loop() {
  if (ccs.available() && !ccs.readData()) {
    float co2 = ccs.geteCO2();
    float tvoc = ccs.getTVOC();

    if (Serial.available()) {
      String data = Serial.readStringUntil('\n');
      Serial.println("Received data: " + data);

      int firstCommaIndex = data.indexOf(',');
      int secondCommaIndex = data.indexOf(',', firstCommaIndex + 1);
      int thirdCommaIndex = data.indexOf(',', secondCommaIndex + 1);
      int fourthCommaIndex = data.indexOf(',', thirdCommaIndex + 1);
      int fifthCommaIndex = data.indexOf(',', fourthCommaIndex + 1);

      int mq7Value = data.substring(0, firstCommaIndex).toInt();
      int mq135Value = data.substring(firstCommaIndex + 1, secondCommaIndex).toInt();
      float temperature = data.substring(secondCommaIndex + 1, thirdCommaIndex).toFloat();
      float humidity = data.substring(thirdCommaIndex + 1, fourthCommaIndex).toFloat();
      float dustDensity = data.substring(fourthCommaIndex + 1, fifthCommaIndex).toFloat();

      Serial.println("Parsed Values: ");
      Serial.println("MQ7: " + String(mq7Value) + ", MQ135: " + String(mq135Value) + ", Temp: " + String(temperature) + ", Humidity: " + String(humidity) + ", Dust: " + String(dustDensity) + ", CO2: " + String(co2) + ", TVOC: " + String(tvoc));

      ThingSpeak.setField(1, mq7Value);
      ThingSpeak.setField(2, mq135Value);
      ThingSpeak.setField(3, temperature);
      ThingSpeak.setField(4, humidity);
      ThingSpeak.setField(5, co2);
      ThingSpeak.setField(6, tvoc);
      ThingSpeak.setField(7, dustDensity);

      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    }
  } else if (!ccs.available()) {
    Serial.println("CCS811 not available or waiting for new data.");
  }

  delay(40000); 
}
