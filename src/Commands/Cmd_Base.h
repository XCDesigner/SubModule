#ifndef _CMD_BASE_H_
#define _CMD_BASE_H_

#include "stdint.h"
#include "protocal.h"

class CMD_BASE {
public:
  virtual int Process();
  virtual int Reack();
  uint8_t pri_code;
  uint8_t sub_code;
  uint8_t *pData;
  char result;
};

extern CMD_BASE cmd_base;

#endif