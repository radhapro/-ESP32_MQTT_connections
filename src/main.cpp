 #include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// --- YE 2 LINE AAPKO BADALNI HAIN ---
const char* ssid = "Robozz Lab";
const char* password = "Robotics@cloud";
// ------------------------------------

const char* mqtt_server = "broker.hivemq.com";

// ESP32 ki neeli LED pin 2 par hoti hai
const int ledPin = 2;

WiFiClient espClient;
PubSubClient client(espClient);

// YE FUNCTION TAB CHALEGA JAB KOI COMMAND AAYEGA
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Command aaya: ");
  Serial.println(message);

  // Check karo ki message kya hai
  if (message == "motor on") {
    Serial.println("LED ON kar raha hoon...");
    digitalWrite(ledPin, HIGH); // LED ON karo
    
    // Turant status wapas bhejo
    client.publish("esp32/motor/status", "motor on ho gi hai");

  } else if (message == "motor off") {
    Serial.println("LED OFF kar raha hoon...");
    digitalWrite(ledPin, LOW); // LED OFF karo

    // Turant status wapas bhejo
    client.publish("esp32/motor/status", "motor off ho gi hai");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT Broker se judne ki koshish...");
    if (client.connect("ESP32_MotorController")) {
      Serial.println(" jud gaya!");
      // Command waale topic ko sunna shuru karo
      client.subscribe("esp32/motor/command");
      Serial.println("Ab main 'esp32/motor/command' topic ko sun raha hoon...");
    } else {
      Serial.print("failed, state=");
      Serial.print(client.state());
      Serial.println(" 5 sec baad fir koshish");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // LED pin ko Output ke liye set karo
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" WiFi jud gaya!");
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}