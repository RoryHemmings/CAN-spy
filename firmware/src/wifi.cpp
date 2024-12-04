#include <WiFiNINA.h>

#include "defines.h"
#include "wifi.h"

// SSID and password for access point
const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;

// Device ip address on access point
// IPAddress ip_address(192, 168, 1, 10);
int status = WL_IDLE_STATUS;

static void printMacAddress(byte mac[])
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

static void printWifiData()
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

static void printCurrentNet()
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