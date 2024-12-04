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
            DEBUG_PRINT("0");
        }
        DEBUG_PRINT_FMT(mac[i], HEX);
        if (i > 0) {
            DEBUG_PRINT(":");
        }
    }
    DEBUG_PRINTLN();
}

static void printWifiData()
{
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    DEBUG_PRINT("IP Address: ");
    DEBUG_PRINTLN(ip);
    DEBUG_PRINTLN(ip);

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    DEBUG_PRINT("MAC address: ");
    printMacAddress(mac);
}

static void printCurrentNet()
{
    // print the SSID of the network you're attached to:
    DEBUG_PRINT("SSID: ");
    DEBUG_PRINTLN(WiFi.SSID());

    // print the MAC address of the router you're attached to:
    byte bssid[6];
    WiFi.BSSID(bssid);
    DEBUG_PRINT("BSSID: ");
    printMacAddress(bssid);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    DEBUG_PRINT("signal strength (RSSI):");
    DEBUG_PRINTLN(rssi);

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    DEBUG_PRINT("Encryption Type:");
    DEBUG_PRINTLN_FMT(encryption, HEX);
    DEBUG_PRINTLN();
}

void init_wifi()
{
    if (WiFi.status() == WL_NO_MODULE) {
        DEBUG_PRINTLN("Communication with WiFi module failed!");

        while (true)
            ;
    }

    // WiFi.config(ip_address);

    while (status != WL_CONNECTED) {
        DEBUG_PRINT("Attempting to connect to WPA SSID: ");
        DEBUG_PRINTLN(ssid);

        status = WiFi.begin(ssid, pass);

        delay(5000);
        DEBUG_PRINT(".");
    }

    DEBUG_PRINTLN("Connection Successful");

    printCurrentNet();
    printWifiData();
}