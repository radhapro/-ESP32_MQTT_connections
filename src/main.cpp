 #include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// --- YE 2 LINE AAPKO BADALNI HAIN ---
const char* ssid = "Robozz Lab";
const char* password = "Robotics@cloud";
// ------------------------------------
 // Dekho, yahan bhi humne wahi address likha hai jo MQTTBox me likha tha
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// Ye function dobara connect hone ki koshish karega
void reconnect() {
  // Jab tak connection na jud jaye, koshish karte raho
  while (!client.connected()) {
    Serial.print("MQTT Broker se judne ki koshish...");
    
    // Iska naam alag rakha hai, taaki computer se clash na ho
    if (client.connect("Mera_ESP32")) {
      Serial.println(" jud gaya!");
    } else {
      Serial.print("failed, state=");
      Serial.print(client.state());
      Serial.println(" 5 sec baad fir koshish");
      delay(5000); // 5 second ruk kar fir se try karega
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // WiFi se connect karo
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi jud gaya!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT Broker ka address aur port set karo
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // Agar connection toota hua hai, to reconnect function ko bulao
  if (!client.connected()) {
    reconnect();
  }
  
  // Ye line bohot zaroori hai. Ye connection ko zinda rakhti hai.
  client.loop();
  
    // --- YE 2 NAYI LINE ADD KI HAIN ---
  delay(5000); // 5 second ruko
  client.publish("esp32/status", "Hi, main connected hoon!");
  // ------------------------------------
} 