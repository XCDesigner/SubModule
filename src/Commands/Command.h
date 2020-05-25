#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "stdint.h"
#include "Cmd_Base.h"
#include "Cmd_01.h"
#include "Cmd_A0.h"
#include "Cmd_AE.h"

class Command {
public:
  void Handle(uint8_t *pData);
  int SplitData(uint8_t *pData, uint8_t *pContent);
private:
  CMD_BASE cmd;
};

extern Command command_processor;

#endif