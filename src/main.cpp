#include <LedManager.hpp>

#define RED_LED_PIN 40
#define BLUE_LED_PIN 15
#define GREEN_LED_PIN 17
#define DELAY 1000

LedManager manager = { DELAY };

void setup() {
  manager.AddLed(RED_LED_PIN);
  manager.AddLed(GREEN_LED_PIN);
  manager.AddLed(BLUE_LED_PIN);
  manager.Setup();
}

void loop() {
  manager.Update();
}