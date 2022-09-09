#define IS_DASHBOARD true // Ena define poy orizei thn 8esh ths plaketas alla kai ta pragmata poy prepei na kanei. (Vgainei me undef)
									// To steeringwheel einai o slave kai to dashboardPCB einai o master.
#define DEBUG true	// If debug == true, it spits out to serial a bunch of debugging information.

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String master_name = "DashboardPCB";
String slave_name = "SteeringWheelPCB";
String msg;
int receivedByte=0;

#ifdef IS_DASHBOARD
void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	Serial.begin(115200);
	SerialBT.begin(master_name, true);

	printIfDebug("Bluetooth daemon running!\n");

	while (!SerialBT.connect(slave_name)) {
		printIfDebug("Device not available, retrying...\n");
	}
	digitalWrite(LED_BUILTIN, HIGH);
	printIfDebug("Connected to " + slave_name + "!\n");
}

void loop() {
	if (!SerialBT.isClosed() && SerialBT.connected()) {
		if (SerialBT.available()) {
			receivedByte = SerialBT.read();
			msg += (char) receivedByte;
			if (receivedByte == '\n') {
				Serial.print(msg);
				msg="";
			}
		}
	}
	else {
		// The device tries to reconnect if the signal gets lost
		digitalWrite(LED_BUILTIN, LOW);
		printIfDebug("Device disconected!\n");

		while (!SerialBT.connect(slave_name)) {
			delay(200);
			printIfDebug("Device not available, retrying...\n");
		}
		digitalWrite(LED_BUILTIN, HIGH);
		
		printIfDebug("Device connected again!\n");
	}
}

#else // Kodikas gia to SteeringWheelPCB

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);
	SerialBT.begin(slave_name, false);

	printIfDebug("Bluetooth daemon running!\n");

	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {	
	if (Serial.available()) {
		SerialBT.write(Serial.read());
	}
	if (SerialBT.available()) {
		Serial.write(SerialBT.read());
	}
}

#endif

void printIfDebug(String output) {
	#ifdef DEBUG
		Serial.print(output);
	#endif
	return;
}
