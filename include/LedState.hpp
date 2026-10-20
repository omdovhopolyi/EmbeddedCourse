#ifndef LED_STATE
#define LED_STATE

#include <Arduino.h>

class LedState
{
public:
    LedState(int index);

  void ConfigurePin();
  void Toggle();
  void SetState(int state);

private:
  int _index = -1;
  int _state = LOW;
};

#endif