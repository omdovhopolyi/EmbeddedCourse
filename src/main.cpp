#include <Arduino.h>

struct Config {
    static constexpr uint8_t ButtonPin = 0;
    static constexpr uint8_t LedPin = LED_BUILTIN;

    static constexpr unsigned long LedOnTimeMs = 1000;
    static constexpr unsigned long LedBlinkMs = 500;
    static constexpr unsigned long DebounceMs = 40;
    static constexpr unsigned long SerialBaud = 115200;
    static constexpr unsigned int TimeLogDelayIterations = 100000;
};

enum class LedState {
    Off,
    On
};

enum class LedControllerType : uint8_t {
  Off,
  On,
  Blink,
  Count
};

// Єдина глобальна змінна для обміну між ISR та loop().
volatile bool buttonEvent = false;

void onButtonChange() {
    // ISR лише повідомляє про зміну сигналу.
    buttonEvent = true;
}

void SwitchLedControllerType(LedControllerType& type) {
  uint8_t typeNum = static_cast<uint8_t>(type);
  typeNum++;
  typeNum %= static_cast<uint8_t>(LedControllerType::Count);
  type = static_cast<LedControllerType>(typeNum);

  Serial.print("Current type: ");
  Serial.println(typeNum);
}

void setLed(LedState state) {
    digitalWrite(Config::LedPin, state == LedState::On ? HIGH : LOW);
}

void setup() {
    Serial.begin(Config::SerialBaud);

    pinMode(Config::LedPin, OUTPUT);
    setLed(LedState::Off);

    pinMode(Config::ButtonPin, INPUT_PULLUP);

    // Реагуємо і на натискання, і на відпускання.
    attachInterrupt(digitalPinToInterrupt(Config::ButtonPin), onButtonChange, CHANGE);

    Serial.println("Ready. Press the button.");
}

void loop() {
    // Поточний стан світлодіода: увімкнений (On) або вимкнений (Off).
    static LedState ledState = LedState::Off;

    static LedControllerType ledControllerType = LedControllerType::Off;

    // Час останнього ввімкнення LED або перезапуску його таймера.
    // Потрібний, щоб вимкнути LED через 1000 мс.
    static unsigned long ledStartedAt = 0;

    // Час обробки останньої події від кнопки.
    // Від нього відраховуємо 40 мс для перевірки антидребезгу.
    static unsigned long lastChangeAt = 0;

    // Останній прочитаний рівень кнопки, ще не підтверджений антидребезгом.
    // HIGH — відпущена, LOW — натиснута.
    static int rawButtonState = HIGH;

    // Підтверджений стан кнопки після перевірки антидребезгу.
    // Порівнюємо з rawButtonState, щоб виявити нове натискання або відпускання.
    static int stableButtonState = HIGH;

    static int timeLogCounter = 0;

    // захищаємо читання та скидання прапорця.
    // До цієї ділянки переривання ввімкнені.
    noInterrupts();
    const bool eventDetected = buttonEvent;
    buttonEvent = false;
    interrupts();

    const unsigned long now = millis();
    const unsigned long microSecNow = micros();

    // Отримали подію — читаємо кнопку й починаємо
    // відлік часу для підтвердження її стану.
    if (eventDetected) {
        rawButtonState = digitalRead(Config::ButtonPin);
        lastChangeAt = now;
    }

    // Приймаємо новий стан після 40 мс без нових подій.
    if (rawButtonState != stableButtonState && now - lastChangeAt >= Config::DebounceMs) {

        stableButtonState = rawButtonState;

        // LOW означає підтверджене натискання.
        if (stableButtonState == LOW) {
          SwitchLedControllerType(ledControllerType);
          ledStartedAt = now;
        }
    }

    switch (ledControllerType) {
      case LedControllerType::Off: {
        if (ledState != LedState::Off) {
          ledState = LedState::Off;
          setLed(LedState::Off);
          Serial.println("Button pressed: LED OFF");
        }
        break;
      }
      case LedControllerType::On: {
        if (ledState != LedState::On) {
          ledState = LedState::On;
          setLed(LedState::On);
          Serial.println("Button pressed: LED ON");
        }
        break;
      }
      case LedControllerType::Blink: {
        if (now - ledStartedAt >= Config::LedBlinkMs) {
          ledStartedAt = now;
          ledState = (ledState == LedState::On ? LedState::Off : LedState::On);
          setLed(ledState);
          Serial.println("Button pressed: LED BLINK");
        }
        break;
      }
      default: {
        Serial.println("Unknown led controller type");
      }
    }

    if (timeLogCounter <= 0) {
      timeLogCounter = Config::TimeLogDelayIterations;
      const int timeElapsed = micros() - microSecNow;
      Serial.print("Time elapsed: ");
      Serial.print(timeElapsed);
      Serial.println(" microseconds");
    }

    timeLogCounter--;
}