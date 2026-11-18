#include "Led.h"

void Led::Init(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    SetState(_state);
}

bool Led::SetState(LedState state)
{   
    bool changed = false;
    if (_state != state) {
        _state = state;
        digitalWrite(_pin, _state == LedState::On ? HIGH : LOW);
        changed = true;
    }
    return changed;
}

void Led::Toggle()
{
    _state = (_state == LedState::On ? LedState::Off : LedState::On);
    SetState(_state);
}
