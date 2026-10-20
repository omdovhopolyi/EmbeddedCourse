#ifndef LED_MANAGER
#define LED_MANAGER

#include <LedState.hpp>
#include <vector>
#include <memory>

class LedController;

class LedManager
{
public:
    void SetController(std::unique_ptr<LedController> controller);

    void AddLed(int pinIndex);
    void Setup();
    void Update(unsigned long dt);

    std::vector<LedState>& GetLeds();
    LedState* GetLed(int index);
    int GetLedsAmount() const;

private:
    void ConfigurePins();

private:
    std::unique_ptr<LedController> _controller;
    std::vector<LedState> _leds;     
};

#endif