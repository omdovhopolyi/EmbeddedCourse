#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include <Arduino.h>
#include <functional>
#include <vector>

#define DEBOUNCE_DELAY 100

struct ButtonState
{
    int pin = -1;
    int currentState = LOW;
    bool isReading = false;
    unsigned long debounceDelay = 0; 
    std::function<void()> callback;
};

class InputManager
{
public:
    void AddSubscription(int pinIndex, const std::function<void()>& callback);
    void ConfigureButtons();
    void Update(int dt);

private:
    std::vector<ButtonState> _buttons;
};

#endif