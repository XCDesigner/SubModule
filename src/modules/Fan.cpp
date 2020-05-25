#include "FAN.h"
#include "GPIOS.h"

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
}

/**
  * @brief  
  * @retval None
  */
void FAN::On() {
  hw_gpio.Write(ControlPin, ActiveLevel);
}

/**
  * @brief  
  * @retval None
  */
void FAN::Off(){
  hw_gpio.Write(ControlPin, InactiveLevel);
}