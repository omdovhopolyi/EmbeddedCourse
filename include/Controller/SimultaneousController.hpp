#ifndef SIMULTANEOUS_CONTROLLER
#define SIMULTANEOUS_CONTROLLER

#include <Controller/LedController.hpp>

#define SIMULTANEOUS_DELAY 200

class SimultaneousController final
    : public LedController
{
public:
    void Init(LedManager* manager) override;
    void Update(unsigned long dt) override;

private:
    void SwitchOffLeds();
    void ToggleLeds();

private:
    unsigned long _delay = 0L;
};

#endif