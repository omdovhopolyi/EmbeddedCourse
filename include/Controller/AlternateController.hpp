#ifndef ALTERNATE_CONTROLLER
#define ALTERNATE_CONTROLLER

#include <Controller/LedController.hpp>

#define ALTERNATE_DELAY 1000

class AlternateController final
    : public LedController
{
public:
    void Init(LedManager* manager);
    void Update(unsigned long dt) override;

private:
    void SwitchOffLeds();
    void IncLedIndex();
    void ToggleCurrentLed();

private:
    unsigned long _delay = 0L;
    int _ledIndex = 0;
};

#endif