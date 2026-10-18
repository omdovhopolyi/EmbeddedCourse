#include <LedManager.hpp>

LedManager::LedManager(int delay)
    : _delay(delay)
{}

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
    _currentLedIndex = 0;
    ToggleCurrentLed();
}

void LedManager::Update()
{
    delay(_delay);

    if (_leds.empty()) {
        return;
    }

    ToggleCurrentLed();
    IncLedIndex();
    ToggleCurrentLed();
}

LedState* LedManager::GetLed(int index)
{
    if (index >= 0 && index < static_cast<int>(_leds.size())) {
        return &_leds[index];
    }
    return nullptr;
}

LedState* LedManager::GetCurrentLed()
{
    return GetLed(_currentLedIndex);
}

void LedManager::ConfigurePins()
{
    for (auto& led : _leds) {
        led.ConfigurePin();
    }
}

void LedManager::ToggleCurrentLed()
{
    auto currentLed = GetCurrentLed();
    if (!currentLed) {
        return;
    }
    currentLed->Toggle();
}

void LedManager::IncLedIndex()
{
    _currentLedIndex++;
    _currentLedIndex %= static_cast<int>(_leds.size());
}