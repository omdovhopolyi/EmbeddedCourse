#include <Arduino.h>
#include <iostream>

#define LED 17
#define LDR 2
#define N   4

const u16_t LDR_THRESHOLD = 2000;
u32_t adcPeriod = 100;
u32_t lastTimeChange = 0;
u16_t readings[N] = {};
u16_t idx = 0;
u32_t sum = 0;

float refMillivolts = 3100.f;
u16_t adcMax = 4095;

u16_t movingAvg(u16_t rawData) {
    sum -= readings[idx];
    sum += rawData;
    readings[idx] = rawData;
    ++idx;

    if(idx >= N) {
        idx = 0;
    }

    return sum / N;
}

void filterInit(u16_t initData) {
    for(u8_t i  = 0; i < N; ++i) {
        readings[i] = initData;
        sum += initData;
    }
}

void setup() {
    pinMode(LED, OUTPUT); 
    pinMode(LDR, INPUT);  
    Serial.begin(115200);
    Serial.println("setuped\n");
    filterInit(analogRead(LDR));
}

void loop() { 
    if(millis() - lastTimeChange > adcPeriod) {
        lastTimeChange = millis(); 
        const u32_t adcValue = analogRead(LDR);
        const u32_t readMillivolts = analogReadMilliVolts(LDR);
        const float calculatedMillivolts = static_cast<float>(adcValue) / adcMax * refMillivolts;  
        const float errorPercent = std::abs(calculatedMillivolts - readMillivolts) / calculatedMillivolts * 100.f;

        printf("raw: %d;\tcalculated: %.2fmV;\tread: %dmV;\terror: %.2f %\n", adcValue, calculatedMillivolts, readMillivolts, errorPercent);

        const u16_t filteredAdcValue = movingAvg(adcValue);
        //std::cout << "adc raw value: " << adcValue << " | adc filtered: " << filteredAdcValue << std::endl;

        digitalWrite(LED, filteredAdcValue < LDR_THRESHOLD ? HIGH : LOW);
    }
}