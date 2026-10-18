
#include <Arduino.h>

class LedState
{
public:
    LedState(int index);

  void ConfigurePin();
  void Toggle();

private:
  int _index = -1;
  int _state = LOW;
};