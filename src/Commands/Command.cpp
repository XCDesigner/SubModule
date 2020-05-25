#include "Command.h"

Command command_processor;

/**
 * brief  Handle the command,1st fetch command from uart buffer. 2nd call the command function
 * retval None
 */
void Command::Handle(uint8_t *pData) {
  CMD_BASE *cmd_to_process;
  SplitData(pData, cmd.pData);
  switch(cmd.pri_code) {
    case 0x01:
      cmd_to_process = (CMD_BASE*)(&cmd_01);
    break;

    case 0xA0:
      cmd_to_process = (CMD_BASE*)(&cmd_A0);
    break;

    case 0xAE:
      cmd_to_process = (CMD_BASE*)(&cmd_AE);
    break;
      
    default:
      cmd_to_process = (CMD_BASE*)(&cmd_base);
    break;
  }
  cmd_to_process->Process();
}

/**
 * brief  Split data
 * retval None
 */
int Command::SplitData(uint8_t *pData, uint8_t *pContent) {
  cmd.pri_code = pData[10];
  cmd.sub_code = pData[11];
  cmd.result = pData[12];
  cmd.pData = &pData[13];
  return 0;
}