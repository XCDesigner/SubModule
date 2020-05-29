#include <Arduino.h>
#include "io.h"
#include "modules/FlashStore.h"
#include "FunctionDefiner.h"

void setup() {
  function_definer.Init();
}

void loop() {
  function_definer.Loop();
}
