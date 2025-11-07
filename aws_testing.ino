#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecureBearSSL.h>

// Include AWS IoT Certificates
#include "ca.h"    // Amazon Root CA
#include "cert.h"  // Device Certificate
#include "key.h"   // Private Key

// WiFi Credentials
const char* ssid = "Mr.lonely";
const char* password = "Antony....";

// AWS IoT Core Endpoint
const char* awsEndpoint = "a1p29l9k7yb1mr-ats.iot.eu-north-1.amazonaws.com";  // Example: a1b2c3d4e5f6-ats.iot.us-east-1.amazonaws.com
const int awsPort = 8883;

// MQTT Topic
const char* topic = "myDevice/sensorData";

// Secure Client
BearSSL::WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

// AWS IoT Connection Function
void connectAWS() {
    Serial.println("\n🔒 Configuring AWS IoT SSL...");
    wifiClient.setBufferSizes(512, 512);
    wifiClient.setTrustAnchors(new BearSSL::X509List(ca_der, ca_der_len));
    wifiClient.setClientRSACert(
        new BearSSL::X509List(cert_der, cert_der_len),
        new BearSSL::PrivateKey(key_der, key_der_len)
    );

    Serial.println("🔌 Connecting to AWS IoT...");
    client.setServer(awsEndpoint, awsPort);
    
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection... ");
        if (client.connect("NodeMCUClient")) { 
            Serial.println("✅ Connected to AWS IoT Core!");
        } else {
            Serial.print("❌ Failed (Error ");
            Serial.print(client.state());  // Print detailed MQTT error code
            Serial.println("). Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Setup Function
void setup() {
    Serial.begin(115200);
    Serial.println("\n🚀 Starting NodeMCU AWS IoT Connection...");

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("🔗 Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());

    // Connect to AWS IoT
    connectAWS();
}

// Loop Function
void loop() {
    if (!client.connected()) {
        connectAWS();  // Reconnect if disconnected
    }
    
    client.loop();
    
    // Publish test message
    String message = "{\"temperature\": 25.5}";
    Serial.print("📤 Publishing: ");
    Serial.println(message);
    client.publish(topic, message.c_str());

    delay(5000);
}
