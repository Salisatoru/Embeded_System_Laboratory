#include <WiFi.h>
#include <MQTTClient.h>

// Wi-Fi credentials
const char ssid[] = "realmeC55";
const char pass[] = "12345678";

// MQTT broker details
const char mqtt_broker[] = "test.mosquitto.org";
const char mqtt_topic[] = "group1.15/command";
const char mqtt_client_id[] = "arduino_group_1.15";
int MQTT_PORT = 8081;

// Pins for motor and LEDs
#define LED1         30  // LED1 pin (Clockwise indicator)
#define LED2         29  // LED2 pin (Counterclockwise indicator)

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;
int counter = 0;

// Function to handle incoming MQTT messages
void messageReceived(String &topic, String &payload) {
  Serial.println("Incoming: " + topic + " - " + payload);

  // Example: Control motor based on MQTT payload
  if (payload == "LED1-On") {
    // Rotate motor clockwise
    digitalWrite(LED1, HIGH);          // Turn on LED1
    digitalWrite(LED2, LOW);           // Turn off LED2
  } else if (payload == "LED2-On") {
    // Rotate motor counterclockwise
    digitalWrite(LED1, LOW);           // Turn off LED1
    digitalWrite(LED2, HIGH);          // Turn on LED2
  } else if (payload == "stop") {
    // Stop the motor
    digitalWrite(LED1, LOW);           // Turn off LED1
    digitalWrite(LED2, LOW);           // Turn off LED2
  }
}

// Function to connect to Wi-Fi and MQTT
void connect() {
  // Connect to Wi-Fi
  Serial.print("Checking Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWi-Fi connected!");

  // Connect to MQTT broker
  Serial.print("Connecting to MQTT broker...");
  while (!client.connect(mqtt_client_id)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT connected!");

  // Subscribe to the MQTT topic
  client.subscribe(mqtt_topic);
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize Wi-Fi and MQTT
  WiFi.begin(ssid, pass);
  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);

  // Connect to Wi-Fi and MQTT
  connect();

  // Set motor and LED pins as output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  // Handle MQTT messages
  client.loop();
  delay(10);

  // Reconnect if the MQTT client is disconnected
  if (!client.connected()) {
    connect();
  }

  // Example periodic publishing
  if (millis() - lastMillis > 2000) {
    lastMillis = millis();
    client.publish(mqtt_topic, "Counter = " + String(counter++));
  }

  // Motor control logic can also be triggered by external MQTT commands
}

