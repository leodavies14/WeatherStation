/*
 * wind_vane.c
 *
 *
 */

#include "Wind_Vane.h"
#include "math.h"
#include "MKL46Z4.h"


double voltages[] = {2.53, 1.31, 1.49, 0.27, 0.30, 0.21, 0.59, 0.41, 0.92, 0.79, 2.03, 1.93, 3.05, 2.67, 2.86, 2.26};
int times = -1;
double volt;
double distance;

void windVaneInit() // Initialize for the ADC analog digital converter.
{
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[16] = PORT_PCR_MUX(0);

	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
	ADC0->CFG1 = ADC_CFG1_ADIV(2) | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(1) | ADC_CFG1_ADICLK(0);
}

double getVoltage()
{
	ADC0->SC1[0] = ADC_SC1_ADCH(1);

	while ( !(ADC0->SC1[0] & ADC_SC1_COCO_MASK) );

	int result = ADC0->R[0];
	return 3.3 * result / 4095;
}


double windVaneGetAngle()
{
	volt = getVoltage();
	int mini = 0;
	double minf = 5.0;

	for ( int i = 0; i < 16; i++ )
	{
		distance = (voltages[i] - volt);
		if ( distance < minf )
		{
			minf = distance;
			mini = i;
		}
	}
	times++;
	return 22.5f * mini;
}


