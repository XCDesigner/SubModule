#include "Cmd_Base.h"

CMD_BASE cmd_base;

// uint8_t CMD_BASE::pri_code;
// uint8_t CMD_BASE::sub_code;
// uint8_t CMD_BASE::*pData;
// char CMD_BASE::result;

int CMD_BASE::Process() {
  result = -1;
  Reack();
  return 0;
}

int CMD_BASE::Reack() {
  return 0;
}