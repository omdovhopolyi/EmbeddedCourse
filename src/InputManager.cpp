#include <InputManager.hpp>

void InputManager::AddSubscription(int pinIndex, const std::function<void()>& callback)
{
    _buttons.emplace_back();
    auto& button = _buttons.back();
    button.pin = pinIndex;
    button.callback = callback;
}

void InputManager::ConfigureButtons()
{
    for (auto& button : _buttons) {
        pinMode(button.pin, INPUT_PULLUP);
    }
}

void InputManager::Update(int dt)
{
    for (auto& button : _buttons) {
        const auto readState = digitalRead(button.pin);
        
        if (button.isReading) {
            if (button.debounceDelay > 0L) {
                button.debounceDelay -= dt;
                if (button.debounceDelay <= 0L) {
                    button.isReading = false;
                    if (readState != button.currentState) {
                        button.currentState = readState;
                        if (button.currentState == LOW) {
                            button.callback();
                        }
                    }
                }
            }
        }
        else if (readState != button.currentState) {
            button.isReading = true;
            button.debounceDelay = DEBOUNCE_DELAY;
        }
    }
}