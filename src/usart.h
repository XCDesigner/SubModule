#ifndef _USART_H_
#define _USART_H_

#include "stdint.h"

class Usart {
public:
  void Init(void);
  void Loop(void);
private:
  uint8_t PushRingBuff(uint8_t *p_src, uint8_t len);
  uint8_t recv_buffer[2048];
  uint8_t send_buffer[128];
  uint8_t parsed_buff[1024];
  uint8_t read_tail, read_head;
};

extern Usart uart;

#endif