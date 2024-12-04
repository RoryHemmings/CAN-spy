#include <Arduino.h>
#include <SPI.h>
#include <WebSocketsServer.h>
#include <WiFiNINA.h>

#include "can.h"
#include "defines.h"

// SSID and password for access point
const char *ssid = AP_SSID;
const char *pass = AP_PASS;

// Device ip address on access point
const IPAddress ip_address(10, 0, 0, 1);

const int led = LED_BUILTIN;

WiFiServer server(80);
int status = WL_IDLE_STATUS;

void printWiFiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print where to go in a browser:
    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip);
}

void init_wifi()
{
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");

        while (true)
            ;
    }

    WiFi.config(ip_address);

    Serial.print("Creating access point named: ");
    Serial.println(ssid);

    status = WiFi.beginAP(ssid, pass);
    if (status != WL_AP_LISTENING) {
        Serial.println("Creating access point failed");

        while (true)
            ;
    }

    // you're connected now, so print out the status
    printWiFiStatus();
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
}

void loop()
{
    // compare the previous status to the current status
    if (status != WiFi.status()) {
        // it has changed update the variable
        status = WiFi.status();

        if (status == WL_AP_CONNECTED) {
            // a device has connected to the AP
            Serial.println("Device connected to AP");
        } else {
            // a device has disconnected from the AP, and we are back in listening mode
            Serial.println("Device disconnected from AP");
        }
    }
}