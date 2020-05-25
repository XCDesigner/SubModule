#ifndef _FLASH_STORE_H_
#define _FLASH_STORE_H_

#include "stdint.h"

#define MODULE_TYPE_STEPPER 0x01
#define MODULE_TYPE_EXECUTER 0x02

typedef struct {
  float Kp;
  float Ki;
  float Kd;
}strSettings;

class FLashStore{
public:
  static void RestoreParameter();
  static void StoreParameter();
  static uint32_t GetModuleType();
  static void LoadDefaultSettings();
  static void RefreshVariable();
private:

public:
  static strSettings settings;  
private:

};

extern FLashStore store_manager;

#endif