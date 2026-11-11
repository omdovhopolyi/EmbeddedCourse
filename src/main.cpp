#include <Arduino.h>
#include <iostream>

struct Config {
  static constexpr uint8_t outPin = 17;
  static constexpr uint8_t inPin = 15;
  static constexpr uint8_t ldrPin = 2; 
  static constexpr u16_t thresholdDark = 1900;
  static constexpr u16_t thresholdLight = 2500;
  static constexpr u32_t adcPeriod = 100;
  static constexpr u16_t inputThreshold = 40;
  static constexpr u8_t avgValuesCount = 10;
};

volatile u32_t lastTimeChange = 0;
u32_t lastTimeUpdate = 0;
bool debouncing = false;
u32_t relaySignalSentTimestamp = 0;
u32_t relaySignalReceivedTimestamp = 0;
u32_t values[Config::avgValuesCount];
u8_t avgIndex = 0;
bool avgInited = false;
int sum = 0;

void initAvg(u16_t value) {
  for (u8_t i = 0; i < Config::avgValuesCount; i++) {
    values[i] = value;
    sum += value;
  }
}

u32_t updateAvg(u16_t newValue) {
  const auto value = values[avgIndex];
  sum -= value;
  sum += newValue;
  values[avgIndex] = newValue;
  avgIndex++;
  avgIndex %= Config::avgValuesCount;

  return sum / Config::avgValuesCount;
}

void onInput() {
  lastTimeChange = millis();
}

void setup() {
  pinMode(Config::outPin, OUTPUT); 
  pinMode(Config::ldrPin, INPUT);
  pinMode(Config::inPin, INPUT_PULLUP);  
  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(Config::inPin), onInput, CHANGE);
}

void loop() { 
  if(millis() - lastTimeUpdate > Config::adcPeriod) {
    lastTimeUpdate = millis(); 
    u16_t adcValue = analogRead(Config::ldrPin);
    //Serial.print("acd: ");
    //Serial.println(adcValue);
    if (adcValue > Config::thresholdLight) {
      //Serial.println("ON");
      digitalWrite(Config::outPin, LOW);
    }
    else if (adcValue < Config::thresholdDark) {
      //Serial.println("OFF");
      digitalWrite(Config::outPin, HIGH);
      relaySignalSentTimestamp = micros();
    }
  }

  if (lastTimeChange > 0 && millis() - lastTimeChange > Config::inputThreshold) {
    lastTimeChange = 0;
    const int inPinState = digitalRead(Config::inPin);
    if (inPinState == LOW) {
      relaySignalReceivedTimestamp = micros();
      const unsigned long relayDelay = (relaySignalReceivedTimestamp - relaySignalSentTimestamp);

      if (!avgInited) {
        avgInited = true;
        initAvg(relayDelay);
      }
      const auto average = updateAvg(relayDelay);

      Serial.print("Relay switch delay: ");
      Serial.print(relayDelay);
      Serial.print(" microseconds; ");
      Serial.print("Average: ");
      Serial.println(average);
    }
  }
}