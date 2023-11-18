#include "main.h"
#include "LED.h"
#include "LDR.h"
//extern uint32_t *timekeep;
int detectObject(void);
void train(int time, uint32_t *timekeep, TIM_HandleTypeDef *htim, int task);
