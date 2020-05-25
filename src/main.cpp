#include <Arduino.h>
#include "io.h"
#include "usart.h"
#include "modules/Temperature.h"
#include "modules/Fan.h"
#include "CANPort.h"
#include "SerialPort.h"
#include "SPIPort.h"
#include "modules/FlashStore.h"

void setup() {
  // put your setup code here, to run once:
  temperature.Init();
  //uart.Init();
  FAN *extFan = new FAN();
  extFan->Init(PB_7, L_HIGH);
  extFan->On();
  extFan->Off();
  store_manager.settings.Kp = 22;
  
  //store_manager.StoreParameter();
  store_manager.RestoreParameter();
}

void loop() {
  // put your main code here, to run repeatedly
  uart.Loop();
  // temperature.SetTargetTemp(100);
}
