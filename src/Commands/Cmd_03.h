#ifndef _CMD_03_H_
#define _CMD_03_H_

#include "stdint.h"
#include "Cmd_Base.h"

class CMD_03 : CMD_BASE {
public:
  CMD_03();
  int Process(uint8_t *pBufferForReack);
private:
  void Sub00();
  void Sub01();
  void Sub02();
};

extern CMD_03 cmd_03;

#endif