# ESP8266 AWS IoT Core MQTT Publisher (BearSSL)

A straightforward Arduino project for connecting the ESP8266 to AWS IoT Core. It securely publishes sensor data (a JSON message) using MQTT, `PubSubClient`, and `BearSSL::WiFiClientSecure` with X.509 certificates.

This code is designed to be a clear, minimal example for getting your ESP8266 securely connected to the AWS IoT cloud.

---

## 🚀 Features

* Connects ESP8266 to a WiFi network.
* Establishes a secure TLS 1.2 connection to AWS IoT Core (Port 8883).
* Uses **BearSSL** (`WiFiClientSecureBearSSL`) for certificate handling.
* Includes all necessary certificate files (`ca.h`, `cert.h`, `key.h`) in DER format.
* Publishes a sample JSON message to a specified MQTT topic every 5 seconds.
* Includes robust reconnect logic for both WiFi and MQTT.

---

## 📋 Requirements

### Hardware
* An ESP8266-based board (NodeMCU, Wemos D1 Mini, etc.)

### Software
* [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/).
* ESP8266 Core for Arduino.
* Libraries:
    * `PubSubClient` by Nick O'Leary
    * (The `ESP8266WiFi` and `WiFiClientSecureBearSSL` libraries are included with the ESP8266 core)

### Services
* An active AWS Account.
* A configured "Thing" in AWS IoT Core with associated certificates and policies.

---

## ⚙️ Setup and Configuration

### 1. AWS IoT Core

Before you can compile, you MUST create a "Thing" in your AWS IoT Core console.

1.  Log in to your AWS Console and navigate to **IoT Core**.
2.  Go to **Manage** > **Things** > **Create things**.
3.  Create a single thing (e.g., `NodeMCU_Device`).
4.  Select **"Auto-generate a new certificate"**.
5.  Create a **Policy** that allows connecting and publishing. An (insecure) policy for testing is:
    ```json
    {
      "Version": "2012-10-17",
      "Statement": [
        {
          "Effect": "Allow",
          "Action": "iot:*",
          "Resource": "*"
        }
      ]
    }
    ```
    **Note:** For production, always restrict this policy to specific topics and actions.
6.  **DOWNLOAD ALL KEYS** when prompted. You will get:
    * `xxxxxx-certificate.pem.crt` (Device Certificate)
    * `xxxxxx-private.pem.key` (Private Key)
    * `AmazonRootCA1.pem` (Root CA)
7.  Find your AWS IoT Endpoint in the **Settings** menu. It looks like:
    `a1b2c3d4e5f6-ats.iot.us-east-1.amazonaws.com`

### 2. Convert Certificates to DER Format

This project uses certificate files (`ca.h`, `cert.h`, `key.h`) that are C++ byte arrays (DER format). You must convert the `.pem` files you downloaded.

You can use OpenSSL in your terminal:

```shell
# Convert Root CA
openssl x509 -in AmazonRootCA1.pem -outform DER -out ca.der

# Convert Device Certificate
openssl x509 -in xxx-certificate.pem.crt -outform DER -out cert.der
```

# Convert Private Key
openssl rsa -in xxx-private.pem.key -outform DER -out key.der
