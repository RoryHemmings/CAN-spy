#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WebSocketsClient.h>

#include "can.h"
#include "defines.h"

// SSID and password for access point
const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;

const int led = LED_BUILTIN;

// Device ip address on access point
IPAddress ip_address(192, 168, 1, 10);
int status = WL_IDLE_STATUS;

WiFiClient client;
WebSocketsClient websocket; 
const int WEBSOCKET_RECONNECT_INTERVAL = 5000;

void printMacAddress(byte mac[])
{
    for (int i = 5; i >= 0; i--) {
        if (mac[i] < 16) {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i > 0) {
            Serial.print(":");
        }
    }
    Serial.println();
}

void printWifiData()
{
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.println(ip);

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    printMacAddress(mac);
}

void printCurrentNet()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the MAC address of the router you're attached to:
    byte bssid[6];
    WiFi.BSSID(bssid);
    Serial.print("BSSID: ");
    printMacAddress(bssid);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption, HEX);
    Serial.println();
}

void init_wifi()
{
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");

        while (true)
            ;
    }

    // WiFi.config(ip_address);

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);

        status = WiFi.begin(ssid, pass);

        delay(5000);
        Serial.print(".");
    }

    Serial.println("Connection Successful");

    printCurrentNet();
    printWifiData();
}

void handle_websocket_event(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {
    case WStype_DISCONNECTED:
        Serial.println("[WSc] Disconnected!");
        break;
    case WStype_CONNECTED:
        Serial.println("[WSc] Connected!");

        // send message to server when Connected
        websocket.sendTXT("Connected");
        break;
    case WStype_TEXT:
        Serial.print("[WSc] get text:");
        Serial.println((char *)payload);

        // send message to server
        // websocket.sendTXT("message here");
        break;
    case WStype_BIN:
        // send data to server
        // websocket.sendBIN(payload, length);
        break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_PING:
    case WStype_PONG:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void setup()
{
    Serial.begin(9600);

    #ifdef DEBUG
    while (!Serial)
        ; /* Wait till serial is ready */
    #endif

    pinMode(led, OUTPUT);

    init_wifi();
    // init_can();

    websocket.begin(C2_ADDR, C2_PORT);
    websocket.onEvent(handle_websocket_event);
    websocket.setReconnectInterval(WEBSOCKET_RECONNECT_INTERVAL);

    Serial.println("C2 Server Target:");
    Serial.print("Address: ");
    Serial.println(C2_ADDR);
    Serial.print("Port: ");
    Serial.println(C2_PORT);
}

void loop()
{
    websocket.loop();
}