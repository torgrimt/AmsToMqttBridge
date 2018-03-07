/*
* Simple sketch to simulate reading data from a Kamstrup
* AMS Meter.
*
* Created 24. October 2017 by Roar Fredriksen
*/

#include <HanReader.h>
#include <Kaifa.h>

// The HAN Port reader
HanReader hanReader;

byte samples[] =
{
	// List #1
	0x7E, 0xA0, 0x27, 0x01, 0x02, 0x01, 0x10, 0x5A, 0x87, 0xE6, 0xE7, 0x00, 0x0F, 0x40, 0x00, 0x00, 0x00, 0x09, 0x0C, 0x07, 0xE1, 0x09, 0x0E, 0x04, 0x13, 0x1F, 0x02, 0xFF, 0x80, 0x00, 0x00, 0x02, 0x01, 0x06, 0x00, 0x00, 0x03, 0x98, 0xAB, 0xAD, 0x7E,

	// List#2
	0x7E, 0xA0, 0x79, 0x01, 0x02, 0x01, 0x10, 0x80, 0x93, 0xE6, 0xE7, 0x00, 0x0F, 0x40, 0x00, 0x00, 0x00, 0x09, 0x0C, 0x07, 0xE1, 0x09, 0x0E, 0x04, 0x13, 0x1F, 0x0A, 0xFF, 0x80, 0x00, 0x00, 0x02, 0x0D, 0x09, 0x07, 0x4B, 0x46, 0x4D, 0x5F, 0x30, 0x30, 0x31, 0x09, 0x10, 0x36, 0x39, 0x37, 0x30, 0x36, 0x33, 0x31, 0x34, 0x30, 0x31, 0x37, 0x35, 0x33, 0x39, 0x38, 0x35, 0x09, 0x08, 0x4D, 0x41, 0x33, 0x30, 0x34, 0x48, 0x33, 0x45, 0x06, 0x00, 0x00, 0x03, 0x96, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x06, 0x00, 0x00, 0x05, 0x64, 0x06, 0x00, 0x00, 0x0C, 0x92, 0x06, 0x00, 0x00, 0x0C, 0x49, 0x06, 0x00, 0x00, 0x09, 0x46, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x09, 0x4E, 0x1F, 0x85, 0x7E,

	// List#3
	0x7E, 0xA0, 0x9B, 0x01, 0x02, 0x01, 0x10, 0xEE, 0xAE, 0xE6, 0xE7, 0x00, 0x0F, 0x40, 0x00, 0x00, 0x00, 0x09, 0x0C, 0x07, 0xE1, 0x09, 0x0E, 0x04, 0x14, 0x00, 0x0A, 0xFF, 0x80, 0x00, 0x00, 0x02, 0x12, 0x09, 0x07, 0x4B, 0x46, 0x4D, 0x5F, 0x30, 0x30, 0x31, 0x09, 0x10, 0x36, 0x39, 0x37, 0x30, 0x36, 0x33, 0x31, 0x34, 0x30, 0x31, 0x37, 0x35, 0x33, 0x39, 0x38, 0x35, 0x09, 0x08, 0x4D, 0x41, 0x33, 0x30, 0x34, 0x48, 0x33, 0x45, 0x06, 0x00, 0x00, 0x03, 0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x40, 0x06, 0x00, 0x00, 0x07, 0x91, 0x06, 0x00, 0x00, 0x0C, 0x9D, 0x06, 0x00, 0x00, 0x0D, 0x66, 0x06, 0x00, 0x00, 0x09, 0x41, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x09, 0x4C, 0x09, 0x0C, 0x07, 0xE1, 0x09, 0x0E, 0x04, 0x14, 0x00, 0x0A, 0xFF, 0x80, 0x00, 0x00, 0x06, 0x00, 0x02, 0xBF, 0x69, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xF7, 0x06, 0x00, 0x00, 0x3F, 0xFC, 0x71, 0x71, 0x7E
};
int sampleIndex = 0;

void setup() {
	setupDebugPort();

	// initialize the HanReader
	// (passing no han port, as we are feeding data manually, but provide Serial for debugging)
	hanReader.setup(NULL, &Serial);
	hanReader.compensateFor09HeaderBug = true;
}

void setupDebugPort()
{
	// Initialize the Serial port for debugging
	Serial.begin(115200);
	while (!Serial) {}
	Serial.setDebugOutput(true);
	Serial.println("Serial1");
	Serial.println("Serial debugging port initialized");
}

void loop() {
	// Read one byte from the "port", and see if we got a full package
	if (hanReader.read(samples[sampleIndex++]))
	{
		// Get the list identifier
		int listSize = hanReader.getListSize();

		Serial.println("");
		Serial.print("List size: ");
		Serial.print(listSize);
		Serial.print(": ");

		// Only care for the ACtive Power Imported, which is found in the first list
		if (listSize == (int)Kaifa::List1 || listSize == (int)Kaifa::List2 || listSize == (int)Kaifa::List3)
		{
			if (listSize == (int)Kaifa::List1)
			{
				Serial.println(" (list #1 has no ID)");
			}
			else
			{
				String id = hanReader.getString((int)Kaifa_List2::ListVersionIdentifier);
				Serial.println(id);
			}

			time_t time = hanReader.getPackageTime();
			Serial.print("Time of the package is: ");
			Serial.println(time);

			if (listSize == (int)Kaifa::List1)
			{
				int power = hanReader.getInt((int)Kaifa_List1::ActivePowerImported);
				Serial.print("Power consumtion is right now: ");
				Serial.print(power);
				Serial.println(" W");
			}
			else
			{
				float current[3];
				current[0] = (float)hanReader.getInt((int)Kaifa_List2::CurrentL1) / 1000.0;
				current[1] = (float)hanReader.getInt((int)Kaifa_List2::CurrentL2) / 1000.0;
				current[2] = (float)hanReader.getInt((int)Kaifa_List2::CurrentL3) / 1000.0;

				int voltage[3];
				voltage[0] = hanReader.getInt((int)Kaifa_List2::VoltageL1);
				voltage[1] = hanReader.getInt((int)Kaifa_List2::VoltageL2);
				voltage[2] = hanReader.getInt((int)Kaifa_List2::VoltageL3);

				for (int i = 0; i < 3; i++)
				{
					Serial.print("L");
					Serial.print(i + 1);
					Serial.print(" is ");
					Serial.print(voltage[i]);
					Serial.print(" V (");
					Serial.print(current[i]);
					Serial.println(" A)");
				}

				if (listSize == (int)Kaifa::List3)
				{
					time_t clock = hanReader.getTime((int)Kaifa_List3::MeterClock);
					Serial.print("Clock is: ");
					Serial.println(clock);
				}
			}
		}
	}

	delay(10);
	if (sampleIndex >= sizeof(samples))
	{
		delay(2000);
		sampleIndex = 0;
	}
}