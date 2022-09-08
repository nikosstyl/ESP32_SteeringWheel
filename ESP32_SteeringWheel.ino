#define IS_DASHBOARD true // Ena define poy orizei thn 8esh ths plaketas alla kai ta pragmata poy prepei na kanei. (Vgainei me undef)
									// To steeringwheel einai o slave kai to dashboardPCB einai o master.
#define DEBUG true	// If debug == true, it spits out to serial a bunch of debugging information.

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

	#ifdef IS_DASHBOARD
		SerialBT.begin(master_name, true);
	#else
		SerialBT.begin(slave_name, false);
	#endif


	#ifdef DEBUG
		Serial.println("Bluetooth daemon running!");
	#endif

	#ifdef IS_DASHBOARD
		while (!SerialBT.connect(slave_name)) {
			Serial.println("Device not available, retrying...");
		}
		digitalWrite(LED_BUILTIN, HIGH);
		#ifdef DEBUG
			output = "Connected to " + slave_name + "!";
			Serial.println(output);
		#endif
	#endif
}

void loop() {
	#ifdef IS_DASHBOARD
		DashboardPCB();
	#else
		SteeringWheelPCB();
	#endif
	
}

void DashboardPCB() {
	if (!SerialBT.isClosed() && SerialBT.connected()) {
		if (SerialBT.available()) {
			incomingInfo = (byte)SerialBT.read();
		}
	}
	else {
		// The device tries to reconnect if the signal gets lost
		digitalWrite(LED_BUILTIN, LOW);
		#ifdef DEBUG
			Serial.println("Device disconected!");
		#endif

		while (!SerialBT.connect(slave_name)) {
			#ifdef DEBUG
				Serial.println("Device not available, retrying...");
			#endif
		}
		digitalWrite(LED_BUILTIN, HIGH);
		
		#ifdef DEBUG
		Serial.println("Device connected again!");
		#endif
	}
}

void SteeringWheelPCB() {
	if (Serial.available()) {
		SerialBT.write(Serial.read());
	}
	if (SerialBT.available()) {
		Serial.write(SerialBT.read());
	}
}