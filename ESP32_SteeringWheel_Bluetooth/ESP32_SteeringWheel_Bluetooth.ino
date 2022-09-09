#include "BluetoothSerial.h"

// #undef IS_DASHBOARD
#define IS_DASHBOARD true // Ena define poy orizei thn 8esh ths plaketas alla kai ta pragmata poy prepei na kanei. (Vgainei me undef)
						//   To steeringwheel einai o slave kai to dashboardPCB einai o master.
#define DEBUG true	// If debug == true, it spits out to serial a bunch of debugging information.

BluetoothSerial SerialBT;

String master_name = "DashboardPCB";
String slave_name = "SteeringWheelPCB";

#ifdef IS_DASHBOARD
String msg;
char *msgChar=NULL;
int receivedByte=0, launch=-1, up_shift=-1, down_shift=-1;

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
			if (receivedByte == '\n') {	// Molis lavei to ending byte, ksekinane oi ypoloipes doyleis (aka CAN Send).
				msgChar = (char*)calloc(msg.length()+1, sizeof(char));	// Allocate ena meros mnhmhs gia na parw tis times pou stelnei to allo ESP32.
				msg.toCharArray(msgChar, msg.length());	// Metatroph apo String se char gia na to dexetai h sscanf.
				sscanf(msgChar, "%d,%d,%d\n", &launch, &up_shift, &down_shift);	// Extracting values sent via Bluetooth
				free(msgChar);	// Apodesmevoume tis times gia na mhn kanei mpoum to ESP32.
				
				/* char output[128];
				sprintf(output, "Launch:%d\tUPShift:%d\tDownshift:%d", launch, up_shift, down_shift);
				Serial.println(output); */
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
char buffer[3*sizeof(int) + 3*sizeof(char)]={'\0'};
uint8_t launch=0, up_shift=0, down_shift=0;

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);
	SerialBT.begin(slave_name, false);

	printIfDebug("Bluetooth daemon running!\n");

	digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {	
	launch = rand();
	up_shift = rand();
	down_shift = rand();
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
