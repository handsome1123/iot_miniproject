// [com 22]
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

const int motorA1 = 4; // Connect L9110 motor driver module IN1 to pin 4
const int motorA2 = 2; // Connect L9110 motor driver module IN2 to pin 2

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
    // Run the fan in one direction
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
  }
  else
  {
    // Stop the fan
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
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
    if (client.connect("ESP32_Fan", mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      // Once connected, resubscribe
      client.subscribe("fan");
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

  // Set motor control pins as output
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
