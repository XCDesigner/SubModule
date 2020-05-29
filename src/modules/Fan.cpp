#include "FAN.h"
#include "GPIOS.h"

FAN fan_extboard;
FAN fan_end;

/**
  * @brief  Initialize
  * @retval None
  */
FAN::FAN() {
  Valid = false;
}

/**
  * @brief  Initialize
  * @retval None
  */
void FAN::Init(GPIO_PIN Pin, GPIO_LEVEL Level) {
  ControlPin = Pin;
  ActiveLevel = Level;
  if(ActiveLevel == L_HIGH)
    InactiveLevel = L_LOW;
  else
    InactiveLevel = L_HIGH;
  hw_gpio.OutputWrite(ControlPin, InactiveLevel);
  Valid = true;
}

/**
  * @brief  
  * @retval None
  */
void FAN::On() {
  if(Valid == true)
    hw_gpio.Write(ControlPin, ActiveLevel);
}

/**
  * @brief  
  * @retval None
  */
void FAN::Off(){
  if(Valid == true)
    hw_gpio.Write(ControlPin, InactiveLevel);
}