/*
 * time.h
 *
 *  Header file for time
 */

#ifndef TIME_H_
#define TIME_H_

unsigned int micros();

void setBusFrequency(unsigned int value);

unsigned int getBusFrequency();

void incTime();

void delayN( int n );

#endif /* TIME_H_ */

