#include <LedManager.hpp>
#include <Controller/LedController.hpp>

void LedManager::SetController(std::unique_ptr<LedController> controller)
{
    _controller = std::move(controller);
    _controller->Init(this);
}

void LedManager::AddLed(int pinIndex)
{
    _leds.emplace_back(pinIndex);
}

void LedManager::Setup()
{
    if (_leds.empty()) {
        return;
    }
    ConfigurePins();
}

void LedManager::Update(unsigned long dt)
{
    if (!_controller) {
        return;
    }
    _controller->Update(dt);
}

std::vector<LedState>& LedManager::GetLeds()
{
    return _leds;
}

LedState* LedManager::GetLed(int index)
{
    if (index >= 0 && index < static_cast<int>(_leds.size())) {
        return &_leds[index];
    }
    return nullptr;
}

int LedManager::GetLedsAmount() const
{
    return static_cast<int>(_leds.size());
}

void LedManager::ConfigurePins()
{
    for (auto& led : _leds) {
        led.ConfigurePin();
    }
}