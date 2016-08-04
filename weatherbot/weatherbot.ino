/*
 *  Weatherbot
 *
 *  ESP8266 sketch for gathering temperature/humidity data
 *  from a DHT-22 sensor and delivering it over WiFi to a 
 *  remote server API
 *
 *  Created by Taylan Pince, 2016.
 */
 
#include <stdlib.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN  2

DHT dht(DHTPIN, DHTTYPE, 11);

float humidity, temperature;
const long interval = 15000; // interval at which to read sensor

const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi Password";
const char* host = "<Your Server API (i.e. 202.702.99.89)>";
const char* endpoint = "<Your Server API Endpoint (i.e. /weather)>";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  dht.begin(); // initialize temperature sensor
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  update_sensor_data();
  
  delay(interval);
}

void update_sensor_data() {
  humidity = dht.readHumidity(); // Read humidity (percent)
  temperature = dht.readTemperature(false); // Read temperature (celcius)

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.println("Temperature: ");
    Serial.println(temperature);
    Serial.println("Humidity: ");
    Serial.println(humidity);
    
    upload_sensor_data();
  }
}

void upload_sensor_data() {
  Serial.println("Uploading sensor data...");
  Serial.print("connecting to ");
  Serial.println(host);
  
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("requesting URL: ");
  Serial.println(endpoint);
  
  char activeValue[10];
  String payload = "{\"temperature\":";
  
  dtostrf(temperature, 4, 3, activeValue);
  payload += activeValue;
  
  payload += ",\"humidity\":";
  
  dtostrf(humidity, 4, 3, activeValue);
  payload += activeValue;
  
  payload += "}";
  
  Serial.println("Payload:");
  Serial.println(payload);
  Serial.println("Length:");
  Serial.println(payload.length());
  
  String requestData = "POST " + endpoint + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "User-Agent: ESP8266\r\n" +
                 "Content-Length: " + payload.length() + "\r\n"
                 "Connection: close\r\n\r\n" + payload;

  Serial.println("Request:");
  Serial.println(requestData);

  client.print(requestData);

  Serial.println("Request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    
    if (line.startsWith("HTTP")) {
      if (line.startsWith("HTTP/1.0 200 OK")) {
        Serial.println("Request successful");
      } else {
        Serial.println("Request failed");
      }
      
      break;
    }
  }

  Serial.println("Closing connection");
}

