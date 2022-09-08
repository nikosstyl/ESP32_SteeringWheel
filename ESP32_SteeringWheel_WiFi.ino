#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define DEBUG true

const char *ssid = "DashboardPCB";
const char *pass = "AlwaysDangerousCRT";
String receivedMsg="";
WiFiServer server(80);

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	Serial.begin(115200);

	WiFI.softAP(ssid, pass);
	#ifdef DEBUG
		Serial.println("Created WiFi hotspot");
	#endif
}

void loop() {
	WiFiClient client = server.available();
	#ifdef DEBUG
		Serial.println("New client connected");
	#endif

	while (client.connected()) {
		digitalWrite(LED_BUILTIN, HIGH);
		if (client.available()) {
			char c = client.read();
			receivedMsg += c;
			if (c == '\0') {
				Serial.println(receivedMsg);
				receivedMsg = "";
			}
		}
	}
}