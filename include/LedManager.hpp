#include <Arduino.h>
#include <LedState.hpp>
#include <vector>

class LedManager
{
public:
    LedManager(int delay = 1000);

    void AddLed(int pinIndex);
    void Setup();
    void Update();

    LedState* GetLed(int index);
    LedState* GetCurrentLed();

private:
    void ConfigurePins();
    void ToggleCurrentLed();
    void IncLedIndex();

private:
    int _delay = -1;
    int _currentLedIndex = -1;
    std::vector<LedState> _leds;     
};