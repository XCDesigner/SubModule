#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "stdint.h"
#include "Cmd_Base.h"
#include "Cmd_01.h"
#include "Cmd_A0.h"
#include "Cmd_AE.h"

class Command {
public:
  void GetReackData(uint8_t *pBuff);
  int Handle(uint8_t *pData);
  int SplitData(uint8_t *pData, uint8_t *pContent);
  void AddCmdList(CMD_BASE *Command);
private:
  CMD_BASE *pList;
  CMD_BASE cmd;
  uint8_t command_count;
  CMD_BASE *cmd_list[20];
  uint8_t reack_buff[128];
  uint8_t reack_len;
};


#endif