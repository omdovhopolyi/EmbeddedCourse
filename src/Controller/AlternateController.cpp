#include <Controller/AlternateController.hpp>
#include <LedManager.hpp>

void AlternateController::Init(LedManager* manager)
{
    LedController::Init(manager);

    _ledIndex = 0;
    SwitchOffLeds();
    ToggleCurrentLed();
    _delay = ALTERNATE_DELAY;
}

void AlternateController::Update(unsigned long dt)
{
    if (_delay > 0L) {
        _delay -= dt;
        return;
    }

    _delay = ALTERNATE_DELAY;

    ToggleCurrentLed();
    IncLedIndex();
    ToggleCurrentLed();
}

void AlternateController::SwitchOffLeds()
{
    auto& leds = _manager->GetLeds();
    for (auto& led : leds) {
        led.SetState(LOW);
    }
}

void AlternateController::IncLedIndex()
{
    _ledIndex++;
    _ledIndex %= _manager->GetLedsAmount();
}

void AlternateController::ToggleCurrentLed()
{
    if (auto led = _manager->GetLed(_ledIndex)) {
        led->Toggle();
    }
}
