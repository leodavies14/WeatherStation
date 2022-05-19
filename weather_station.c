/*
 * weather_station.c
 *
 * Left button: PTC12
 * Right Button: PTC13
 * Anemometer: PTC 13
 * Rain Gauge: PTC 16
 *
 */

#include "MKL46Z4.h"
#include "time.h"
#include "anemometer.h"
#include "rain.h"
#include "wind_vane.h"
#include "lcd.h"
#include "fsl_clock.h"

volatile int hour = 0, min = 0, sec = 0;
volatile int state = 0;
volatile int idleTime = 0;
volatile int lastWindTime = 0;
volatile int lastRainTime = 0;
volatile int currentTime = 0;
volatile double currentWindSpeed = 0.0;
volatile double totalRain = 0.0;
volatile double windAngle = 0.0;
volatile double currentWindDirection = 0.0;
volatile double windVoltage;
volatile int time_1 = 0;
volatile int time_2 = 0;
volatile int time_3 = 0;
volatile int time_4 = 0;
volatile int speed_1 = 0;
volatile int speed_2 = 0;
volatile int speed_3 = 0;
volatile int speed_4 = 0;
volatile int rain_1 = 0;
volatile int rain_2 = 0;
volatile int rain_3 = 0;
volatile int rain_4 = 0;
//int temp;
//int decimal;



// Initialize both buttons, the anemometer and rain gauge
void Switch_Anemo_Rain_Init()

{
	// enable interrupt
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);

	//enable clock
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	// initialize GPIO
	PORTC->PCR[3] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTC->PCR[12] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTC->PCR[13] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTC->PCR[16] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	//Set as input
	PTC->PDDR &= ~0x13008;
}

void displayTime(int hour, int min)
{
	clearDisplay();
	time_1 = hour/10;
	time_2 = hour%10;
	time_3 = min/10;
	time_4 = min%10;
	displayDigit(1, time_1);
	displayDigit(2, time_2);
	displayDigit(3, time_3);
	displayDigit(4, time_4);
	displayColon();
}

void PORTC_PORTD_IRQHandler(void)
{
	__disable_irq();

	//Left button: PTC12 (will control the hours and go back to change time)
	if ( PORTC->ISFR & 0x1000 )
	{
		if ( state == 0 ) // Add time to the hours
		{

			if(hour >= 23)
				hour = 0;
			else
				hour ++;

			displayTime(hour, min);

		}
		else // Displays time
		{
			state = 0;
			displayTime(hour, min);
		}

		idleTime = 0;
	}

	if ( PORTC->ISFR & 0x08 )
	{	//Right Button: PTC13 (controls to go back to different measurements
		// as well as changes the minutes)

		if ( state == 0 ) // If changing time then add to minutes
		{
			if(min >= 60)
			{
				min = 0;
				hour++;
			}
			else
				min++;

			displayTime(hour, min);

		}
		else
		{
			if(state == 4)
				state = 1;
			else
				state ++;

			if ( state == 1 )
			{
				displayTime(hour, min);
			}
		}

		idleTime = 0;
	}

	if ( PORTC->ISFR & 0x2000 )
	{	// Anemometer: PTC 13 (Will calculate wind speed)
		currentTime= micros()-lastWindTime;
		lastWindTime = micros();
		incCurrentWindCount();
	}

	if ( PORTC->ISFR & 0x10000 )
	{	// Rain Gauge: PTC 16 (Will calculate how much water was gotten.)
		lastRainTime = micros();
		incCurrentRainCount();
	}

	PORTC->ISFR = 0x13008;
	__enable_irq();
}

void PIT_Initialize()
{
	NVIC_EnableIRQ(PIT_IRQn);

	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; // Enable clock to PIT module
	PIT->MCR = PIT_MCR_MDIS(0);

	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[0].LDVAL = getBusFrequency() - 1;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
}

void PIT_IRQHandler(void)
{
	__disable_irq();
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

	incTime();
	sec++;

	if (sec > 59) {  //example: 21:58:59 then it becomes 21:59:00
			min ++;
			sec = 0;
		}

	if ((min > 59)) {
		if (hour >= 23) { //if its at 23:59:59 make it 0:00:00
			hour = 0;
			min = 0;
			sec = 0;
		}
		else { // example: 22:59:59 becomes 23:00:00
			hour ++;
			min = 0;
			sec = 0;
		}

	}



	if (state == 0)
	{
		idleTime++;
		if (idleTime == 30)
			state = 1;
	}

	if((state == 0) || (state == 1)){
		displayTime(hour, min);
	}
	__enable_irq();
}






int main(void) {
	setBusFrequency(CLOCK_GetFreq(kCLOCK_BusClk));

	init_lcd();
	__enable_irq();
	PIT_Initialize();
	Switch_Anemo_Rain_Init();
	windVaneInit();
	anemometerInit();
	rainInit();

	displayTime(hour, min);

	while(1)
	{
		switch(state)
		{
		case 1: // Just time
			break;

		case 2: // Current wind speed
			currentWindSpeed = getCurrentWindSpeed();
			clearColon();
			int temp;
			int decimal;
			temp = currentWindSpeed;
			speed_1 = temp/10;
			speed_2 = temp%10;
			decimal = 100*(currentWindSpeed - temp);
			speed_3 = decimal/10;
			speed_4 = decimal%10;
			displayDigit(1, speed_1);
			displayDigit(2, speed_2);
			displayDigit(3, speed_3);
			displayDigit(4, speed_4);
			displayDecimalPoint(2);

			break;

		case 3: //Wind direction
			windAngle = windVaneGetAngle();
			currentWindDirection = windAngle * 10;
			windVoltage = getVoltage();
			clearDecimalPoint(2);

			//North
			if((windVoltage >= 2.51 && windVoltage <=2.55) || (windVoltage >= 1.29 && windVoltage <=1.33)){
				clearDisplay();
				turnOnSegment(4, 'A');
				turnOnSegment(4, 'B');
				turnOnSegment(4, 'C');
				turnOnSegment(4, 'E');
				turnOnSegment(4, 'F');
			}
						//North east
						else if((windVoltage >= 1.47 && windVoltage <=1.51) || (windVoltage >= 0.25 && windVoltage < 0.29)){
							clearDisplay();
							turnOnSegment(3, 'A');
							turnOnSegment(3, 'B');
							turnOnSegment(3, 'C');
							turnOnSegment(3, 'E');
							turnOnSegment(3, 'F');
							turnOnSegment(4, 'A');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'E');
							turnOnSegment(4, 'F');
							turnOnSegment(4, 'G');
						}
						//East
						else if((windVoltage >= 0.29 && windVoltage <= 0.32) || (windVoltage >= 0.19 && windVoltage <= 0.23)){
							clearDisplay();
							turnOnSegment(4, 'A');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'E');
							turnOnSegment(4, 'F');
							turnOnSegment(4, 'G');
						}
						//South East
						else if((windVoltage >= 0.57 && windVoltage <= 0.61) || (windVoltage >= 0.39 && windVoltage <= 0.43)){
							clearDisplay();
							turnOnSegment(3, 'A');
							turnOnSegment(3, 'C');
							turnOnSegment(3, 'D');
							turnOnSegment(3, 'F');
							turnOnSegment(3, 'G');
							turnOnSegment(4, 'A');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'E');
							turnOnSegment(4, 'F');
							turnOnSegment(4, 'G');
						}
						//South
						else if((windVoltage >= 0.90 && windVoltage <= 0.94) || (windVoltage >= 0.77 && windVoltage <= 0.81)){
							clearDisplay();
							turnOnSegment(4, 'A');
							turnOnSegment(4, 'C');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'F');
							turnOnSegment(4, 'G');
						}
						//South West
						else if((windVoltage >= 2.01 && windVoltage <= 2.05) || (windVoltage >= 1.91 && windVoltage <= 1.95)){
							clearDisplay();
							turnOnSegment(2, 'A');
							turnOnSegment(2, 'C');
							turnOnSegment(2, 'D');
							turnOnSegment(2, 'F');
							turnOnSegment(2, 'G');
							turnOnSegment(3, 'C');
							turnOnSegment(3, 'D');
							turnOnSegment(3, 'E');
							turnOnSegment(3, 'F');
							turnOnSegment(4, 'B');
							turnOnSegment(4, 'C');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'E');
						}
						//West
						else if((windVoltage >= 3.03 && windVoltage <= 3.07) || (windVoltage >= 2.65 && windVoltage <= 2.69)){
							clearDisplay();
							turnOnSegment(3, 'C');
							turnOnSegment(3, 'D');
							turnOnSegment(3, 'E');
							turnOnSegment(3, 'F');
							turnOnSegment(4, 'B');
							turnOnSegment(4, 'C');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'E');
						}
						//North West
						else{
							clearDisplay();
							turnOnSegment(2, 'A');
							turnOnSegment(2, 'B');
							turnOnSegment(2, 'C');
							turnOnSegment(2, 'E');
							turnOnSegment(2, 'F');
							turnOnSegment(3, 'C');
							turnOnSegment(3, 'D');
							turnOnSegment(3, 'E');
							turnOnSegment(3, 'F');
							turnOnSegment(4, 'B');
							turnOnSegment(4, 'C');
							turnOnSegment(4, 'D');
							turnOnSegment(4, 'E');
						}

			break;

		case 4: // total rain
			totalRain += getCurrentRainTotal() * 100;

			//clearDisplay();
			int tmp;
			int dcml;
			tmp = totalRain;
			rain_1 = tmp/10;
			rain_2 = tmp%10;
			dcml = 100*(totalRain - tmp);
			rain_3 = dcml/10;
			rain_4 = dcml%10;
			displayDigit(1, rain_1);
			displayDigit(2, rain_2);
			displayDigit(3, rain_3);
			displayDigit(4, rain_4);
			displayDecimalPoint(2);

			break;


		}
		delayN(10);
	}

    return 0 ;
}







