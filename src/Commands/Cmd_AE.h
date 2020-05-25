#ifndef _CMD_AE_H_
#define _CMD_AE_H_

#include "stdint.h"
#include "Cmd_Base.h"

class CMD_AE : CMD_BASE {
public:
  int Process();
  int Reack();
private:
  void Sub01();
  void Sub02();
};

extern CMD_AE cmd_AE;

#endif