 #include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// --- YE 2 LINE AAPKO BADALNI HAIN ---
const char* ssid = "Robozz Lab";
const char* password = "Robotics@cloud";
 const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// YE FUNCTION TAB CHALEGA JAB KOI MESSAGE AAYEGA
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Ek naya message aaya hai! Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

// Ye function dobara connect hone ki koshish karega
void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT Broker se judne ki koshish...");
    if (client.connect("Mera_ESP32_Receiver")) {
      Serial.println(" jud gaya!");
      
      // --- SIRF YE EK LINE IMPORTANT HAI ---
      // Connection judte hi, is channel ko sunna shuru kar do
      client.subscribe("esp32/command");
      Serial.println("Ab main 'esp32/command' topic ko sun raha hoon...");

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
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" WiFi jud gaya!");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Message aane par kya karna hai, ye set kiya
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Hamesha check karte raho ki koi message aaya ya nahi
}