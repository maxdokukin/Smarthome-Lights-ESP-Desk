#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

// Wi-Fi network credentials
const char *ssid = "KGB Headquarters";
const char *password = "DecoratingLandsFace";

// Function to connect to Wi-Fi
void wifi_connect() {
    WiFi.persistent(false);             // Disable persistent Wi-Fi parameters storage
    WiFi.mode(WIFI_STA);                // Set Wi-Fi mode to Station
    WiFi.setAutoReconnect(true);        // Enable auto-reconnect
    WiFi.begin(ssid, password);         // Start Wi-Fi connection with SSID and password
    Serial.println("WiFi connecting...");

    while (!WiFi.isConnected()) {       // Wait for the connection to be established
        delay(100);
        Serial.print(".");
    }
    Serial.print("\n");
    Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str()); // Print the assigned IP address
}

#endif // WIFI_INFO_H_
