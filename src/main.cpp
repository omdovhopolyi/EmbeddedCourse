#include <Arduino.h>

struct Config {
  static constexpr uint8_t ButtonPin = 15;
  static constexpr unsigned long DebounceTime = 50L;
  static constexpr unsigned long PollPeriod = 10L;
};

volatile bool interruptCaught = false;

enum class DebounceType {
  None,
  TimeBased,
  StateBased,
  Polling
};

DebounceType type = DebounceType::Polling;
int counter = 0;
unsigned long lastTimestamp = 0L;
unsigned long pollTimestamp = 0L;
bool buttonPressed = false;
bool lastButtonState = false;

void OnButtonPressed() {
  interruptCaught = true;
}

void setup() {
  Serial.begin(115200);

  //pinMode(Config::ButtonPin, INPUT_PULLUP);
  pinMode(Config::ButtonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(Config::ButtonPin), OnButtonPressed, FALLING);
}

void loop() {
  const auto now = millis();

  if (type == DebounceType::None) {
    if (interruptCaught) {
      interruptCaught = false;
      counter++;
      Serial.print("None. Count: ");
      Serial.println(counter);
    }
  }
  else if (type == DebounceType::TimeBased) {
    if (lastTimestamp > 0L && interruptCaught) {
      interruptCaught = false;
      lastTimestamp = now;
    }
    if (lastTimestamp > 0L && now - lastTimestamp >= Config::DebounceTime) {
      lastTimestamp = 0L;
      counter++;
      Serial.print("Time based. Count: ");
      Serial.println(counter);
    }
  }
  else if (type == DebounceType::StateBased) {
    if (interruptCaught) {
      interruptCaught = false;
      lastTimestamp = now;
    }
    if (lastTimestamp > 0L && now - lastTimestamp >= Config::DebounceTime) {
      lastTimestamp = 0L;
      const auto pinState = digitalRead(Config::ButtonPin);
      if (pinState == LOW) {
        counter++;
        Serial.print("Time based. Count: ");
        Serial.println(counter);
      }
    }
  }
  else if (type == DebounceType::Polling) {
    if (now - pollTimestamp >= Config::PollPeriod) {
      pollTimestamp = now;
      const bool pinState = (digitalRead(Config::ButtonPin) == LOW);
      if (pinState != lastButtonState) {
        lastTimestamp = now;
        lastButtonState = pinState;
      }
      if (lastTimestamp > 0L && now - lastTimestamp >= Config::DebounceTime) {
        lastTimestamp = 0L;
        if (pinState != buttonPressed) {
          buttonPressed = pinState;
          if (buttonPressed) {
            counter++;
            Serial.print("Polling. Count: ");
            Serial.println(counter);
          }
        }
      }
    }
  }
}