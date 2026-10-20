#include <Controller/SimultaneousController.hpp>
#include <LedManager.hpp>

void SimultaneousController::Init(LedManager* manager)
{
    LedController::Init(manager);
    SwitchOffLeds();
    _delay = SIMULTANEOUS_DELAY;
}

void SimultaneousController::Update(unsigned long dt)
{
    if (_delay > 0L) {
        _delay -= dt;
        return;
    }

    _delay = SIMULTANEOUS_DELAY;

    ToggleLeds();
}

void SimultaneousController::SwitchOffLeds()
{
    auto& leds = _manager->GetLeds();
    for (auto& led : leds) {
        led.SetState(LOW);
    }
}

void SimultaneousController::ToggleLeds()
{
    auto& leds = _manager->GetLeds();
    for (auto& led : leds) {
        led.Toggle();
    }
}