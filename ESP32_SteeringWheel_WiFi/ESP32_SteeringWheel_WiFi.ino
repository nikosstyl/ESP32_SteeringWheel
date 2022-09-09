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

		WiFi.softAP(ssid, pass);
		#ifdef DEBUG
			Serial.println("Created WiFi hotspot");
		#endif
		server.begin();
	}

	void loop() {
		WiFiClient client = server.available();

		/* while (client.connected()) {
			#ifdef DEBUG
				Serial.println("New client connected");
			#endif

			digitalWrite(LED_BUILTIN, HIGH);
			if (client.available()) {
				char c = client.read();
				receivedMsg += c;
				if (c == '\0') {
					Serial.println(receivedMsg);
					receivedMsg = "";
				}
			}
		} */
		if (client) {
			printIfDebug("A new client has been connected!\n");
			digitalWrite(LED_BUILTIN, HIGH);
			while(client.connected()) {
				delay(500);
			}
			
		}
	}
#else
	void setup() {
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, LOW);
		Serial.begin(115200);

		WiFi.begin(ssid,pass);
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			printIfDebug(".");
		}
		digitalWrite(LED_BUILTIN, HIGH);

		#ifdef DEBUG
			printIfDebug("\nWifi Connected!\n");
		#endif
	}

	void loop() {
		delay(1000);
	}

#endif

void printIfDebug (String Serialoutput) {
	#ifdef DEBUG
		Serial.print(Serialoutput)
	#endif
	return;
}

