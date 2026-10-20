#include <LedManager.hpp>
#include <InputManager.hpp>
#include <Controller/AlternateController.hpp>
#include <Controller/SimultaneousController.hpp>
#include <memory>

#define LED_PIN_1 15
#define LED_PIN_2 16
#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 21

unsigned long lastTime = 0L;

LedManager ledManager;
InputManager inputManager;

void setup() {
  Serial.begin(115200);

  ledManager.AddLed(LED_PIN_1);
  ledManager.AddLed(LED_PIN_2);
  
  inputManager.AddSubscription(BUTTON_PIN_1, []() {
    auto controller = std::unique_ptr<AlternateController>(new AlternateController());
    ledManager.SetController(std::move(controller));
  });

  inputManager.AddSubscription(BUTTON_PIN_2, []() {
    auto controller = std::unique_ptr<SimultaneousController>(new SimultaneousController());
    ledManager.SetController(std::move(controller));
  });
  
  ledManager.Setup();
  inputManager.ConfigureButtons();
}

void loop() {
  const auto dt = millis() - lastTime;
  lastTime = millis();
  
  inputManager.Update(dt);
  ledManager.Update(dt);
}