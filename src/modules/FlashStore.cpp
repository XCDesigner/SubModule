#include "FlashStore.h"
#include "LocalEEPROM.h"
#include "Temperature.h"

/**
 * EEPROM partition manager is implemented in Eeprom_partition.c
 * This file supplies the interface to the applications.Version control, store,restore.
 */
FLashStore store_manager;

strSettings FLashStore::settings;

/**
  * @brief  
  * @retval None
  */
void FLashStore::RestoreParameter(){
  if(hw_internal_eeprom.RestoreSetting((uint8_t*)&settings, sizeof(settings)) == false) {
    LoadDefaultSettings();
    StoreParameter();
  }
}

/**
  * @brief  
  * @retval None
  */
void FLashStore::StoreParameter() {
  hw_internal_eeprom.StoreSetting((uint8_t*)&settings, sizeof(settings));
}

/**
  * @brief  
  * @retval None
  */
uint32_t FLashStore::GetModuleType() {
  return hw_internal_eeprom.GetModuleType();
}

/**
  * @brief  
  * @retval None
  */
void FLashStore::LoadDefaultSettings() {
  settings.Kd = 0;
  settings.Kp = 22;
  settings.Ki = 1;

  RefreshVariable();
}

/**
  * @brief  
  * @retval None
  */
void FLashStore::RefreshVariable() {
  temperature.SetPID(settings.Kp, settings.Ki, settings.Kd);
}