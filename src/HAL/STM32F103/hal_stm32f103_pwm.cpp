#include "PWMPort.h"
#include "stm32f10x_conf.h"

/**
  * @brief  
  * @param  
  * @retval  
  */
void PWMPort::Init(){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 719;
  TIM_TimeBaseInitStruct.TIM_Period = 0;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputNState_Enable;
  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct.TIM_Pulse = 0;
  TIM_OC1Init(TIM3, &TIM_OCInitStruct);

  TIM_CtrlPWMOutputs(TIM3, ENABLE);

  TIM_Cmd(TIM3,ENABLE);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void PWMPort::SetPwmValue(uint16_t Value) {
  TIM3->CCR1 = Value;
}