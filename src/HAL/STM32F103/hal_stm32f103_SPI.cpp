#include "SPIPort.h"
#include "stm32f10x_conf.h"

/**
  * @brief  
  * @param  
  * @retval  
  */
void SPIPort::Init(){
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_Init(SPI1, &SPI_InitStruct);
  SPI_Cmd(SPI1, ENABLE);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint8_t SPIPort::ReadWriteOneByte(uint8_t DataToSend) {
  uint8_t ret_value = 0xff;
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = DataToSend;
  while(!(SPI1->SR & SPI_SR_RXNE));
  ret_value = SPI1->DR;
  return ret_value;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void SPIPort::SendOneByte(uint8_t Data) {
  ReadWriteOneByte(Data);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void SPIPort::SendBytes(uint8_t *Datas, uint16_t Len) {
  for(int i=0;i<Len;i++)
    ReadWriteOneByte(Datas[i]);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint8_t SPIPort::ReadOneByte(void) {
  return ReadWriteOneByte(0xff);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void SPIPort::ReadBytes(uint8_t *pData, uint16_t Len) {
  for(int i=0;i<Len;i++)
    *pData++ = ReadWriteOneByte(0xff);
}

extern "C" {

}