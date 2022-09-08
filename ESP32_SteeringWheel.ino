#define IS_STEERING_WHEEL_PCB false // Ena define poy orizei thn 8esh ths plaketas alla kai ta pragmata poy prepei na kanei.
									// To steeringwheel einai o slave kai to dashboardPCB einai o master.
#define DEBUG true	// If debug == true, it spits out to serial a bunch of debugging information.
#define MAX_BUFFER_LEN 256

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String master_name = "DashboardPCB";
String slave_name = "SteeringWheelPCB";
String output="";
byte incomingInfo;

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	Serial.begin(115200);
	SerialBT.begin(master_name, !IS_STEERING_WHEEL_PCB);
	
	if (DEBUG) {
		Serial.println("Bluetooth daemon running!");
	}

	if (!IS_STEERING_WHEEL_PCB) {
		while (!SerialBT.connect(slave_name)) {
			Serial.println("Device not available, retrying...");
		}
		digitalWrite(LED_BUILTIN, HIGH);
		if (DEBUG) {
			output = "Connected to " + slave_name + "!";
			Serial.println(output);
		}
	}
}

void loop() {
	if (!SerialBT.isClosed() && SerialBT.connected()) {
		if (SerialBT.available()) {
			incomingInfo = (byte)SerialBT.read();
		}
	}
	else {
		// The device tries to reconnect if the signal gets lost
		digitalWrite(LED_BUILTIN, LOW);
		if (DEBUG) {
			Serial.println("Device disconected!");
		}
		while (!SerialBT.connect(slave_name)) {
			Serial.println("Device not available, retrying...");
		}
		digitalWrite(LED_BUILTIN, HIGH);
		Serial.println("Device connected again!");
	}
}
