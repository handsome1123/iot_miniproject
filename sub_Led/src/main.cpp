// sub 01 red lab8
// running on port => com06
// pin Led 23 
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
// WiFi Connection details
const char *ssid = "god";
const char *password = "mike2003";
// HiveMQ Cloud Broker details
const char *mqtt_broker = "a2dfcf7d2fde4d719cbda072dd814b18.s1.eu.hivemq.cloud";
const char *mqtt_username = "Handsome";
const char *mqtt_password = "Handsome12345";
const int mqtt_port = 8883; // Usually 8883 for TLS
WiFiClientSecure espClient;
PubSubClient client(espClient);
void callback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);
  if (message == "true")
  { 
    digitalWrite(23, HIGH);
  }
  else
  {
    digitalWrite(23, LOW);
  }
}
void connectToWiFi()
{
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_LED", mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      // Once connected, resubscribe
      client.subscribe("led");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{
  Serial.begin(9600);
  connectToWiFi();
  // Bypass SSL/TLS certificate validation - use only for testing!
  espClient.setInsecure();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  pinMode(23, OUTPUT);
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}