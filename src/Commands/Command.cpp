#include "Command.h"

// Command uart_command_processor;
// Command can_command_processor;

/**
 * brief  Handle the command,1st fetch command from uart buffer. 2nd call the command function
 * retval None
 */
int Command::Handle(uint8_t *pData) {
  // CMD_BASE *cmd_to_process;
  SplitData(pData, cmd.pData);
  for(int i=0;i<command_count;i++) {
    if(cmd.pri_code == cmd_list[i]->pri_code) {
      reack_len = cmd_list[i]->Process(reack_buff);
        return reack_len;
    }
  }
  return 0;
}

/**
  * brief  
  * retval None
  */
void Command::GetReackData(uint8_t *pBuff) {
  for(int i=0;i<reack_len;i++)
    pBuff[i] = reack_buff[i];
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

/**
 * brief  Split data
 * retval None
 */
void Command::AddCmdList(CMD_BASE *Command) {
  cmd_list[command_count] = Command;
  command_count++;
}