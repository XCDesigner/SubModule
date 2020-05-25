#include "CANPort.h"
#include "stm32f10x_conf.h"

uint8_t CANPort::ring_recv_buffer[1024];
uint16_t CANPort::read_tail;
uint16_t CANPort::read_head;

/**
  * @brief  
  * @param  
  * @retval  
  */
void CANPort::Init(){
  GPIO_InitTypeDef GPIO_InitStruct;
  CAN_InitTypeDef CAN_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  CAN_InitStruct.CAN_ABOM = DISABLE;
  CAN_InitStruct.CAN_AWUM = DISABLE;
  CAN_InitStruct.CAN_NART = ENABLE;
  CAN_InitStruct.CAN_TTCM = DISABLE;
  CAN_InitStruct.CAN_TXFP = DISABLE;
  CAN_InitStruct.CAN_BS1 = CAN_BS1_14tq;
  CAN_InitStruct.CAN_BS2 = CAN_BS1_5tq;
  CAN_InitStruct.CAN_SJW = 1;
  CAN_InitStruct.CAN_Prescaler = 6;
  CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;
  CAN_Init(CAN1, &CAN_InitStruct);
  FilterInit();
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void CANPort::FilterInit() {
  CAN_FilterInitTypeDef CAN_FilterInitStruct;
  CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
  CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStruct.CAN_FilterIdHigh = 0xffff;
  CAN_FilterInitStruct.CAN_FilterIdLow = 0xffff;
  CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xffff;
  CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0xffff;
  CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStruct.CAN_FilterNumber = 0;
  CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInit(&CAN_FilterInitStruct);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void CANPort::ISRInit() {
  NVIC_InitTypeDef NVIC_InitStruct;
  
  NVIC_InitStruct.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
int CANPort::SendBytes(uint8_t *Datas, uint16_t Len) {
  CanTxMsg TxMessage;
  uint8_t i;
  int byte_to_send = 0;
  
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.StdId = 0xffff; 
  while(Len > 0) {
    for(i=0;(i<8) && (Len--);i++)
      TxMessage.Data[i] = *Datas++;
    TxMessage.DLC = i;
    byte_to_send = byte_to_send + i;
    CAN_Transmit(CAN1, &TxMessage);
  }
  return byte_to_send;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
bool CANPort::ReadOneByte(uint8_t *Data) {
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
int CANPort::ReadBytes(uint8_t *pData, uint16_t Len) {
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
void CANPort::PushData(uint8_t *pData, uint8_t Len) {
  uint8_t next_head;
  for(int i=0;i<Len;i++) {
    next_head = (read_head + 1) % sizeof(ring_recv_buffer);
    if(next_head == read_tail)
      return;
    ring_recv_buffer[read_head] = pData[i];
    read_head = next_head;
  }
}

extern "C" {
void __irq_usb_lp_can_rx0(void) {
  CanRxMsg msg;
  if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)) {
    CAN_Receive(CAN1, CAN_FIFO0, &msg);
    hw_can_port.PushData(msg.Data, msg.DLC);
  }
}

void __irq_can_rx1(void) {
  CanRxMsg msg;
  if(CAN_GetITStatus(CAN1, CAN_IT_FMP1)) {
    CAN_Receive(CAN1, CAN_FIFO1, &msg);
  }
}
}