#include <Controller/LedController.hpp>

LedController::~LedController() = default;

void LedController::Init(LedManager* manager)
{
    _manager = manager;
}