#include "LED.h"


void analogWrite(TIM_HandleTypeDef *htim, uint32_t Channel, int level)
{
    __HAL_TIM_SetCompare(htim, Channel, level);
}
void turnOnLed_tb(TIM_HandleTypeDef *htim)
{
		analogWrite(htim, TIM_CHANNEL_1 , 200);
    analogWrite(htim, TIM_CHANNEL_2 , 200);
    analogWrite(htim, TIM_CHANNEL_3 , 200);
    analogWrite(htim, TIM_CHANNEL_4 , 200);
}
void turnOffLed(TIM_HandleTypeDef *htim)
{
		analogWrite(htim, TIM_CHANNEL_1 , 0);
    analogWrite(htim, TIM_CHANNEL_2 , 0);
    analogWrite(htim, TIM_CHANNEL_3 , 0);
    analogWrite(htim, TIM_CHANNEL_4 , 0);
    
}
void turnOnLed_MAX_1(TIM_HandleTypeDef *htim)
{
	analogWrite(htim, TIM_CHANNEL_1 , 1000);
	analogWrite(htim, TIM_CHANNEL_2 , 1000);
  analogWrite(htim, TIM_CHANNEL_3 , 200);
	analogWrite(htim, TIM_CHANNEL_4 , 200);
	
}
void turnOnLed_MAX_2(TIM_HandleTypeDef *htim)
{
	analogWrite(htim, TIM_CHANNEL_1 , 200);
	analogWrite(htim, TIM_CHANNEL_2 , 200);
  analogWrite(htim, TIM_CHANNEL_3 , 1000);
  analogWrite(htim, TIM_CHANNEL_4 , 1000);
}
void turnOnLed_MAX_1_2(TIM_HandleTypeDef *htim)
{
	analogWrite(htim, TIM_CHANNEL_1 , 1000);
	analogWrite(htim, TIM_CHANNEL_2 , 1000);
  analogWrite(htim, TIM_CHANNEL_3 , 1000);
  analogWrite(htim, TIM_CHANNEL_4 , 1000);
}