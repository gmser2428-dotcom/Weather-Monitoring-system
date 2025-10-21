#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <ThingSpeak.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(100);
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("BMP180 sensor not found");
    while (1);
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  ThingSpeak.begin(client);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  if (isnan(humidity) || isnan(temperature) || isnan(pressure)) {
    delay(2000);
    return;
  }

  Serial.println("---------------");
  Serial.print("Temperature: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Pressure: "); Serial.println(pressure);

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, pressure);

  int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (statusCode == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Error sending data");
  }

  delay(15000);
}