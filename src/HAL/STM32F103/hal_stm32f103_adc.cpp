#include "ADCPort.h"
#include "Temperature.h"
#include "stm32f10x_conf.h"

uint16_t ADCPort::DMA_buffer[TOTAL_CHANNELS * SAMPLE_DEEP];
uint16_t ADCPort::RawData[TOTAL_CHANNELS];

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::Init(){
  TimerInit();
  DMAInit();
  ISRInit();
  ADCInit();
}

/**
  * @brief  
  * @param  ADC
  * @retval  
  */
void ADCPort::ADCInit() {
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_InitTypeDef ADC_InitStruct;

  RCC_ADCCLKConfig(RCC_PCLK2_Div4);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  ADC_DeInit(ADC1);
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStruct.ADC_NbrOfChannel = TOTAL_CHANNELS;
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;
  ADC_Init(ADC1, &ADC_InitStruct);  

  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_71Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_71Cycles5);

  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1) == SET);
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::ISRInit() {
  NVIC_InitTypeDef NVIC_InitStruct;

  NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
  NVIC_Init(&NVIC_InitStruct);

  // NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
  // NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  // NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
  // NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
  // NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::DMAInit() {
  DMA_InitTypeDef DMA_InitStruct;

  DMA_DeInit(DMA1_Channel1);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_InitStruct.DMA_BufferSize = sizeof(DMA_buffer) / sizeof(DMA_buffer[0]);
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)DMA_buffer;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
  DMA_Init(DMA1_Channel1, &DMA_InitStruct);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::TimerInit() {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  TIM_DeInit(TIM1);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;
  TIM_TimeBaseInitStruct.TIM_Period = 100;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
  
  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStruct.TIM_DeadTime = 1;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);

  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 50;
  TIM_OC1Init(TIM1, &TIM_OCInitStruct);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  // TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::Enable() {
  TIM_Cmd(TIM1,ENABLE);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::Disable() {
  TIM_Cmd(TIM1,DISABLE);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void ADCPort::Filter() {
  uint32_t sum[TOTAL_CHANNELS];
  uint32_t max[TOTAL_CHANNELS];
  uint32_t min[TOTAL_CHANNELS];
  int i, j;

  for(i=0;i<TOTAL_CHANNELS;i++) {
    max[i] = min[i] = DMA_buffer[i];
    sum[i] = 0;
  }

  for(i=0;i<SAMPLE_DEEP;i++) {
    for(j=0;j<TOTAL_CHANNELS;j++) {
      sum[j] += DMA_buffer[i * TOTAL_CHANNELS + j];
      if(DMA_buffer[i * TOTAL_CHANNELS + j] > max[j])
        max[j] = DMA_buffer[i * TOTAL_CHANNELS + j];
      else if(DMA_buffer[i * TOTAL_CHANNELS + j] < min[j])
        min[j] = DMA_buffer[i * TOTAL_CHANNELS + j];
    }
  }

  for(i=0;i<TOTAL_CHANNELS;i++) {
    sum[i] = sum[i] - max[i] - min[i];
  }

  for(i=0;i<TOTAL_CHANNELS;i++) {
    RawData[i] = sum[i] / (SAMPLE_DEEP - 2);
  }
}

extern "C" {
void __irq_dma1_channel1(void) {
  if(DMA_GetITStatus(DMA1_IT_TC1) == SET) {
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    hw_adc_port.Filter();
    temperature.IsrProcess();
  }
}

void __irq_tim1_up(void) {
  uint32_t sr;
  sr = TIM_GetITStatus(TIM1, TIM_IT_Update);
  if(sr == SET) {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }
}
}