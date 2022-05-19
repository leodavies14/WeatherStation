/*
 * time.c
 *
 * This file is to set a time to the frequency at which the
 * timer goes at.
 */

#include "time.h"
#include "fsl_clock.h"

unsigned int busFrequency;
volatile unsigned int secondsSinceStart = 0;
unsigned int cval = 0;
double time = 0.0;

// return the current value in microseconds
unsigned int micros()
{
	// set the Current Timer Value Register
	cval = PIT->CHANNEL[0].CVAL;
	time = 1.0 - (double)cval / (double)busFrequency;
	return secondsSinceStart * 1000000 + time * 1000000;
}


// to set the bus frequency to another number
void setBusFrequency(unsigned int value)
{
	busFrequency = value;
}

// To get the bus frequency
unsigned int getBusFrequency()
{
	return busFrequency;
}

// to increase the time at which it started
void incTime ()
{
	secondsSinceStart++;
}

void delayN(int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < 1720; j++)
		{
			//delay
		}
	}

}
