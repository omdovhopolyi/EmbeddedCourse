#ifndef LED_CONTROLLER
#define LED_CONTROLLER

class LedManager;

class LedController
{
public:
    virtual ~LedController();
    virtual void Init(LedManager* manager);
    virtual void Update(unsigned long dt) = 0;

protected:
    LedManager* _manager = nullptr;
};

#endif