#include "SerialPort.h"
#include "stm32f10x_conf.h"

uint8_t SerialPort::ring_recv_buffer[1024];
uint16_t SerialPort::read_tail;
uint16_t SerialPort::read_head;
uint8_t SerialPort::ring_send_buffer[1024];
uint16_t SerialPort::send_tail;
uint16_t SerialPort::send_head;

/**
  * @brief  
  * @param  
  * @retval  
  */
void SerialPort::Init(int Baudary){
  USART_InitTypeDef USART_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = Baudary;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;

  USART_Init(USART1, &USART_InitStruct);
  USART_Cmd(USART1, ENABLE);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
int SerialPort::SendBytes(uint8_t *Datas, uint16_t Len) {
  uint16_t next_send_head;
  int byte_sent = 0;
  for(int i=0;i<Len;i++) {
    next_send_head = (send_head + 1) % sizeof(ring_send_buffer);
    if(next_send_head == send_tail)
      break;
    ring_send_buffer[send_head] = Datas[i];
    byte_sent++;
    send_head = next_send_head;
  }
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  return byte_sent;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
bool SerialPort::ReadOneByte(uint8_t *Data) {
  if(read_tail != read_head){
    *Data = ring_recv_buffer[read_tail];
    read_tail = (read_tail + 1) % sizeof(ring_recv_buffer);
    return true;
  }
  else {
    return false;
  }
}

/**
  * @brief  
  * @param  
  * @retval  
  */
int SerialPort::ReadBytes(uint8_t *pData, uint16_t Len) {
  int i;
  for(i=0;i<Len;i++) {
    if(read_tail == read_head)
      break;
    pData[i] = ring_recv_buffer[read_tail];
    read_tail = (read_tail + 1) % sizeof(ring_recv_buffer);
  }
  return i;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
bool SerialPort::ISR_SendOneByte() {
  if(send_tail == send_head) {
    //USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    USART_ClearITPendingBit(USART1, (uint16_t)USART_IT_TXE);
    USART1->CR1 &= ~USART_CR1_TXEIE;
    return false;
  } 
  else {
    USART1->DR = ring_send_buffer[send_tail];
    send_tail = (send_tail + 1) % sizeof(ring_send_buffer);
    return true;
  }
 }

/**
  * @brief  
  * @param  
  * @retval  
  */
void SerialPort::ISR_StoreOneByte(uint8_t Data) {
  uint8_t next_head;
  next_head = (read_head + 1) % sizeof(ring_recv_buffer);
  if(next_head != read_tail) {
    ring_recv_buffer[read_head] = Data;
    read_head =  next_head;
  }
}

extern "C" {
void __irq_usart1(void) {
  uint8_t recv_data;
  if(USART1->SR & USART_SR_RXNE) {
    recv_data = USART1->DR;
    hw_uart_port.ISR_StoreOneByte(recv_data);
  }
  else if(USART1->SR & USART_SR_TXE) {
    hw_uart_port.ISR_SendOneByte();
  }
}
}