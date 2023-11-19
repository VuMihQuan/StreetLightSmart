#include "current_sensor.h"
float R1 = 1;
float R2 = 2;
void ConvertVoltage(uint16_t *voltage_read, float *rawVoltage)
{
	for(int i =0 ;i <3 ; i++)
	{
		rawVoltage[i] = (float) voltage_read[i] * 1.5 * 3.3 / 4095.0*(R1 +R2)/R2;
	}
}