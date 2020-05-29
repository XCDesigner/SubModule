#ifndef _CMD_A0_H_
#define _CMD_A0_H_

#include "stdint.h"
#include "Cmd_Base.h"

class CMD_A0 : CMD_BASE {
public:
  CMD_A0();
  int Process(uint8_t *pBufferForReack);
  int Reack();
private:
  void Sub01();
  void Sub02();
};

extern CMD_A0 cmd_A0;

#endif