#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "LedStateEnum.h"

class Led {
public:
    void Init(uint8_t pin);
    bool SetState(LedState state);
    void Toggle();

private:
    uint8_t _pin = LED_BUILTIN;
    LedState _state = LedState::Off;
};

#endif
