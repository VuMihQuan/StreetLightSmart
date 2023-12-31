#include "SIM.h"

void senSMS(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_UART_Transmit(&huart,(uint8_t *) "AT+,)
}