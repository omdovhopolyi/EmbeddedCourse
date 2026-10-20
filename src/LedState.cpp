#include <LedState.hpp>

LedState::LedState(int index)
    : _index(index) 
    {}

void LedState::ConfigurePin()
{
    pinMode(_index, OUTPUT);
}

void LedState::Toggle()
{
    _state = (_state == LOW ? HIGH : LOW);
    digitalWrite(_index, _state);
}

void LedState::SetState(int state)
{
    _state = state;
    digitalWrite(_index, _state);
}