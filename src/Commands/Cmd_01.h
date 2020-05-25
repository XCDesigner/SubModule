#ifndef _CMD_01_H_
#define _CMD_01_H_

#include "stdint.h"
#include "Cmd_Base.h"

class CMD_01 : CMD_BASE {
public:
  int Process();
};

extern CMD_01 cmd_01;

#endif