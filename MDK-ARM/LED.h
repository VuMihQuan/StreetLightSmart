#include "main.h"

void analogWrite(TIM_HandleTypeDef *htim, uint32_t Channel, int level);
void turnOnLed_tb(TIM_HandleTypeDef *htim);
void turnOffLed(TIM_HandleTypeDef *htim);
void turnOnLed_MAX_1(TIM_HandleTypeDef *htim);
void turnOnLed_MAX_2(TIM_HandleTypeDef *htim);
void turnOnLed_MAX_1_2(TIM_HandleTypeDef *htim);
void turnOnLed(TIM_HandleTypeDef *htim);
