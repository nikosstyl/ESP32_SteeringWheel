#include "BluetoothSerial.h"
#include <CAN.h>

// #undef IS_DASHBOARD
#define IS_DASHBOARD true // Ena define poy orizei thn 8esh ths plaketas alla kai ta pragmata poy prepei na kanei. (Vgainei me undef)
						//   To steeringwheel einai o slave kai to dashboardPCB einai o master.
// #define DEBUG true	// If debug == true, it spits out to serial a bunch of debugging information.
#undef DEBUG

#define BUFFER_LENGTH 128

// The button assigned values are not correct. Please, fix them while testing the actual system!
#define BTN_LAUNCH 2
#define BTN_UPSHIFT 3
#define BTN_DOWNSHIFT 4
#define BTN_PTT 5

#define CAN_ID 120

BluetoothSerial SerialBT;

String master_name = "DashboardPCB";
String slave_name = "SteeringWheelPCB";

#ifdef IS_DASHBOARD
char msg[BUFFER_LENGTH]={'\0'}; // Pinakas pou apothikevetai to Bluetooth.
int receivedByte=0, launch=-1, up_shift=-1, down_shift=-1;
unsigned int currentChar=0;	// Enas arithmos pou dixnei thn 8esh toy pinaka poy 8a mpei to neo stoixeio. 

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	/* CAN implementation is expiremental! They may not work!*/
	while(!CAN.begin(1000E3));
	
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
			msg[currentChar++] = (char) receivedByte;
			if (receivedByte == '\n') {	// Molis lavei to ending byte, ksekinane oi ypoloipes doyleis (aka CAN Send).
				sscanf(msg, "%d,%d,%d\n", &launch, &up_shift, &down_shift);	// Extracting values sent via Bluetooth
				
				CAN.beginPacket(CAN_ID);
				CAN.write(launch);
				CAN.write(up_shift);
				CAN.write(down_shift);
				CAN.endPacket();

				#ifdef DEBUG
				char output[128];
				sprintf(output, "Launch:%d\tUPShift:%d\tDownshift:%d", launch, up_shift, down_shift);
				Serial.println(output);
				#endif
				
				strncpy(msg, "\n", currentChar);
				currentChar = 0;
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
char buffer[BUFFER_LENGTH]={'\0'};
uint8_t launch=0, up_shift=0, down_shift=0;

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);
	SerialBT.begin(slave_name, false);

	printIfDebug("Bluetooth daemon running!\n");

	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {	
	launch = digitalRead(BTN_LAUNCH);
	up_shift = digitalRead(BTN_UPSHIFT);
	down_shift = digitalRead(BTN_DOWNSHIFT);
	sprintf(buffer, "%d,%d,%d\n", launch, up_shift, down_shift);
	SerialBT.print(buffer);
	printIfDebug(buffer);
	delay(10);
}
#endif

void printIfDebug(String output) {
	#ifdef DEBUG
		Serial.print(output);
	#endif
	return;
}
