#include "GPIOS.h"
#include "stm32f10x_conf.h"

uint32_t Ports[] = {
  (uint32_t)GPIOA,
  (uint32_t)GPIOB,
  (uint32_t)GPIOC,
  (uint32_t)GPIOD,
  (uint32_t)GPIOE
};

uint32_t RccTable[] = {
  RCC_APB2Periph_GPIOA,
  RCC_APB2Periph_GPIOB,
  RCC_APB2Periph_GPIOC,
  RCC_APB2Periph_GPIOD,
  RCC_APB2Periph_GPIOE
};

GPIOMode_TypeDef Modes[] = {
  GPIO_Mode_Out_PP,
  GPIO_Mode_Out_OD,
  GPIO_Mode_IPU,
  GPIO_Mode_IN_FLOATING
};

/**
  * @brief  
  * @param  
  * @retval  
  */
void GPIOS::SetMode(GPIO_PIN Pin, GPIO_MODE Mode) {
  uint8_t PortIndex;
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = (1UL << (Pin % 16));
  GPIO_InitStruct.GPIO_Mode = Modes[Mode];
  PortIndex = Pin / 16;

  RCC_APB2PeriphClockCmd(RccTable[PortIndex], ENABLE);
  GPIO_Init((GPIO_TypeDef*)Ports[PortIndex], &GPIO_InitStruct);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void GPIOS::OutputWrite(GPIO_PIN Pin, GPIO_LEVEL IOLevel) {
  uint8_t PortIndex;
  uint16_t PinBit;
  PortIndex = Pin / 16;
  PinBit = (1UL << (Pin % 16));

  SetMode(Pin, GPIO_MODE_OUT_PP);
  if(IOLevel == L_HIGH)
    GPIO_WriteBit((GPIO_TypeDef*)Ports[PortIndex], PinBit, Bit_SET);
  else
    GPIO_WriteBit((GPIO_TypeDef*)Ports[PortIndex], PinBit, Bit_RESET);   
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void GPIOS::Write(GPIO_PIN Pin, GPIO_LEVEL IOLevel) {
  uint8_t PortIndex;
  uint16_t PinBit;

  PortIndex = Pin / 16;
  PinBit = (1UL << (Pin % 16));

  if(IOLevel == L_HIGH)
    GPIO_WriteBit((GPIO_TypeDef*)Ports[PortIndex], PinBit, Bit_SET);
  else
    GPIO_WriteBit((GPIO_TypeDef*)Ports[PortIndex], PinBit, Bit_RESET);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
bool GPIOS::Read(GPIO_PIN Pin) {
  uint8_t PortIndex;
  uint16_t PinBit;

  PortIndex = Pin / 16;
  PinBit = (1UL << (Pin % 16));
  return (GPIO_ReadInputDataBit((GPIO_TypeDef*)Ports[PortIndex], PinBit) == SET);
}