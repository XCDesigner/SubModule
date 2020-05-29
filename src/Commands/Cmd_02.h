#ifndef _CMD_02_H_
#define _CMD_02_H_

#include "stdint.h"
#include "Cmd_Base.h"

class CMD_02 : CMD_BASE {
public:
  CMD_02();
  int Process(uint8_t *pBufferForReack);
};

extern CMD_02 cmd_02;

#endif