#include "IR_sensor.h"

void train(int time, uint32_t *timekeep, TIM_HandleTypeDef *htim, int task)
{
  if(HAL_GetTick() - *timekeep < time)
  {
			if(task == 1)
			{
				turnOnLed_MAX_1(htim);
			}
			else if(task == 2)
			{
				turnOnLed_MAX_2(htim);
			}
			else if(task == 3)
			{
				turnOnLed_MAX_1_2(htim);
			}
				
  }
	else{
		turnOnLed_tb(htim);
		*timekeep = HAL_GetTick()+time;
	}
		
}
int detectObject(void)
{
    GPIO_PinState a = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
    GPIO_PinState b = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
    if((a == GPIO_PIN_RESET) && (b == GPIO_PIN_SET))
    {
				return 1;
    }
		else if((b == GPIO_PIN_RESET) && (a == GPIO_PIN_SET))
    {
				return 2;
    }
		else if((a == GPIO_PIN_RESET) && (b == GPIO_PIN_RESET))
		{
			return 3;
		}
		else return 0;
}
