#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define DEBUG true
#define IS_DASHBOARD true

const char *ssid = "DashboardPCB";
const char *pass = "AlwaysDangerousCRT";
String receivedMsg="";
WiFiServer server(80);

#ifdef IS_DASHBOARD
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
#else
	void loop() {
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, LOW);
		Serial.begin(115200);

		WiFi.begin(ssid,pass);
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			#ifdef DEBUG

			#endif
		}
	}

	void setup() {

	}

#endif