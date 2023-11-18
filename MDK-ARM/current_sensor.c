#include "current_sensor.h"

void ConvertVoltage(uint32_t *voltage_read, float *rawVoltage)
{
    rawVoltage[0] =(float) voltage_read[0] * 1.5 * 3.3 / 4095.0;
    rawVoltage[1] =(float) voltage_read[1] * 1.5 * 3.3 / 4095.0;
    rawVoltage[2] =(float) voltage_read[2] * 1.5 * 3.3 / 4095.0;
}