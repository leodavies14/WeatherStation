/*
 * anemometer.c
 *
 *  For the anemometer
 *  will calculate the speed of the wind
 */


#include "time.h"
#include "anemometer.h"


unsigned int startSampleTime;
unsigned int currentWindCount;
double currentWindSpeed1;
double windFactor = 2.4;



void anemometerInit()
{
	startSampleTime = micros();
	currentWindCount = 0;
	currentWindSpeed1 = 0.0;
}

double getCurrentWindSpeed()
{
	const double sampleTime = 2; //2 seconds
	unsigned int compareValue = sampleTime * 1000000;

	if ( micros() - startSampleTime >= compareValue )
	{
		double timeSpan;
		timeSpan = (micros() - startSampleTime);
		currentWindSpeed1 = ((double)currentWindCount/(timeSpan)) * windFactor * 1000000;
		currentWindCount = 0;
		startSampleTime = micros();
	}

		return currentWindSpeed1 / 1.6;
}

void incCurrentWindCount()
{
	currentWindCount++;
}


