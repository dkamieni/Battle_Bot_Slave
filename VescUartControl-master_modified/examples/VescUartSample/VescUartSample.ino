/*
 Name:		VescUartSample.ino
 Created:	9/26/2015 10:12:38 PM
 Author:	AC
*/

// the setup function runs once when you press reset or power the board
// To use VescUartControl stand alone you need to define a config.h file, that should contain the Serial or you have to comment the line
// #include Config.h out in VescUart.h

//Include libraries copied from VESC
 #include "VescUart.h"
#include "datatypes.h"
#include "local_datatypes.h"

unsigned long count;
float throttle = 5;

void setup() {
	

	Serial.begin(115200);

}

struct bldcMeasure measuredValues;
	
// the loop function runs over and over again until power down or reset
void loop() {
	//int len=0;
	//len = ReceiveUartMessage(message);
	//if (len > 0)
	//{
	//	len = PackSendPayload(message, len);
	//	len = 0;
	//}
	
	if (VescUartGetValue(measuredValues)) {
		Serial.print("Loop: "); Serial.println(count++);
		SerialPrint(measuredValues);
    VescUartSetCurrent((throttle  / 100) * 40.0);
	}
	else
	{
		Serial.println("Failed to get data!");
	}
 
	
}





