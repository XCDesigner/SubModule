#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "stdint.h"
#include "Commands/Command.h"

class Communication {
public:
  uint8_t PushRingBuff(uint8_t *p_src, uint8_t len);
  
protected:
  uint8_t recv_buffer[1024];
  uint8_t send_buffer[128];
  uint8_t parsed_buff[1024];
  uint8_t read_tail, read_head;
  Command commands;
};

#endif