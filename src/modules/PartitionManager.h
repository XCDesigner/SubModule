#ifndef _PARTITION_MANAGE_H_
#define _PARTITION_MANAGE_H_

#include "stdint.h"

class FlashPartition{
public:
  static void RestoreParameter();
  static void StoreParameter();
  static uint32_t GetModuleType();
private:


};

extern FlashPartition hw_flash_partition;

#endif