#include "FunctionDefiner.h"
#include "Fan.h"
#include "CAN.h"
#include "SPIPort.h"
#include "Temperature.h"
#include "DS2431.h"
#include "Arduino.h"
#include "usart.h"
#include "FlashStore.h"

FunctionDefiner function_definer;

ADCPort *FunctionDefiner::adc;
ModuleType FunctionDefiner::type;
uint32_t FunctionDefiner::service_life;
uint32_t FunctionDefiner::time_counter;

/**
  * @brief  Initialize
  * @retval None
  */
void FunctionDefiner::Init() {
  uint32_t type_in_flash;
  uint32_t tmp_tick;
  uint16_t resistor_raw0, resistor_raw1;

  ds2431.Init();

  fan_extboard.Init(PB_7, L_HIGH);
  store_manager.RestoreParameter();

  // Flash store the main type, motor module and function module
  // FDM, Laser, or CNC is one type of funtion module.It depends on the resistor on the board.
  type_in_flash = store_manager.GetModuleType();
  adc = &hw_adc_port;
  if(1) {
  // if(type_in_flash == 0) {
    // adc->Init();
    // adc->Enable();
    // tmp_tick = millis() + 100;
    // while(tmp_tick > millis());
    // // Get the resistor on the board
    // do {
    //   resistor_raw1 = resistor_raw0;
    //   resistor_raw0 = adc->GetAdcValue(1);
    //   tmp_tick = millis() + 100;
    //   while(tmp_tick > millis());
    // } while(((resistor_raw1 - resistor_raw0) > 100) || (resistor_raw1 == 0));

    can.Init();
    resistor_raw0 = 4000;
    type = M_UNDEFINE;
    if(resistor_raw0 > 3000) {
      type = M_FDM;
      FDM_Init();
    }
    else if(resistor_raw0 > 2000) {
      type = M_LASER;
      Laser_Init();
    } 
    else if(resistor_raw0 > 1000) {
      type = M_CNC;
      CNC_Init();
    }
  }
  else {
    //uart.Init();
    Motor_Init();
  } 

  service_life = ds2431.ReadTime();
}

/**
  * @brief  
  * @retval None
  */
void FunctionDefiner::Loop() {
  switch(type) {
    case M_FDM:
      FDM_loop();
    break;

    case M_LASER:
      Laser_loop();
    break;

    case M_CNC:
      CNC_loop();
    break;

    case M_MOTOR:
      Motor_loop();
    break;

    default:
      while(1);
    break;
  }
}

/*
  * @brief  
  * @retval None
  */
void FunctionDefiner::FDM_loop(){
  while(1) {
    temperature.Loop();
    can.Loop();
  }
}

/*
  * @brief  
  * @retval None
  */
void FunctionDefiner::Laser_loop(){
  while(1) {
    can.Loop();
  }
}

/*
  * @brief  
  * @retval None
  */
void FunctionDefiner::CNC_loop(){ 
  while(1) {
    can.Loop();
  }
}

/*
  * @brief  
  * @retval None
  */
void FunctionDefiner::Motor_loop(){
  while(1) {
    uart.Loop();
  }
}

/*
  * @brief  
  * @retval None
  */
void FunctionDefiner::FDM_Init() {
  temperature.Init();
}

/**
  * @brief  
  * @retval None
  */
void FunctionDefiner::Laser_Init() {
  adc->Disable();
}

/**
  * @brief  
  * @retval None
  */
void FunctionDefiner::Motor_Init() {
}

/**
  * @brief  
  * @retval None
  */
void FunctionDefiner::CNC_Init() {
  adc->Disable();
}

/**
  * @brief  
  * @retval None
  */
void FunctionDefiner::StartJob() {
  time_counter = millis();
}

/**
  * @brief  
  * @retval None
  */
void FunctionDefiner::EndJob() {
  service_life = service_life + (millis() - time_counter);
  ds2431.WriteTime(service_life);
}

/**
  * @brief  
  * @retval None
  */
ModuleType FunctionDefiner::GetType() {
  return type;
}

/**
  * @brief  
  * @retval None
  */
uint32_t FunctionDefiner::GetServiceLift() {
  return service_life;
}