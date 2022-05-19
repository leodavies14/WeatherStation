/*
 * rain.c
 *
 */

#include "rain.h"

unsigned int currentRainCount;
double rainAmount;

void rainInit()
{
	currentRainCount = 0;
}

double getCurrentRainTotal()
{
	rainAmount = 0.2794 * currentRainCount/2;  // mm of rain - we get two interrupts per bucket
	currentRainCount = 0;
	return rainAmount;
}

void incCurrentRainCount()
{
	currentRainCount++;
}

