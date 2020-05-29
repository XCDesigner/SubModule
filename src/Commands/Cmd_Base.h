#ifndef _CMD_BASE_H_
#define _CMD_BASE_H_

#include "stdint.h"
#include "protocal.h"

class CMD_BASE {
public:
  virtual int Process(uint8_t *pBufferForReack);
  virtual int Reack();
  uint8_t pri_code;
  uint8_t sub_code;
  uint8_t *pData;
  char result;
  uint8_t *preack_buff;
  uint16_t reack_len;
};

extern CMD_BASE cmd_base;

#endif