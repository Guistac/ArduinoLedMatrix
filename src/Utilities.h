#pragma once
#include <Arduino.h>

namespace Time{

    double getSeconds(){ return float(millis()) / 1000.0; }

    class Timer{
    public:
        Timer(double durationSeconds = 0.0){
            if(durationSeconds > 0.0) durationMillis = durationSeconds * 1000;
            else durationMillis = 0;
        }

        void start(double durationSeconds = 0.0){
            startMillis = millis();
            if(durationSeconds > 0.0) durationMillis = durationSeconds * 1000;
        }

        bool isExpired(){
            uint32_t nowMillis = millis();
            if(nowMillis - startMillis > durationMillis) return true;
            return false;
        }
    private:
        uint32_t startMillis = 0;
        uint32_t durationMillis = 0;
    };

    class IntervalTimer{
    public:
        IntervalTimer(float frequency){
            intervalMillis = 1000.0 / frequency;
        }

        bool isTriggered(){
            uint32_t nowMillis = millis();
            if(nowMillis - lastUpdateMillis < intervalMillis) return false;
            lastUpdateMillis = nowMillis;
            return true;
        }

    private:
        uint32_t lastUpdateMillis = 0;
        uint32_t intervalMillis = 0;
    };

}


float triangle(float in){
    if(in < 0.0) return 0.0;
    else if(in > 1.0) return 0.0;
    else if(in < 0.5) return in * 2.0;
    else return 2.0 - in * 2.0;
}


float sinNorm(float in){
    return map(sin(in), -1.0, 1.0, 0.0, 1.0);
}

float randomF(float min, float max){
    return float(random(min * 1000, max * 1000)) * 0.001;
}

float randomF(float max){
    return float(random(max * 1000)) * 0.001;
}

float lerp(float a, float b, float mix){
    if(mix > 1.0) mix = 1.0;
    else if(mix < 0.0) mix = 0.0;
    return a * (1.0 - mix) + b * mix;
} 

float iirFilter(float val, float target, float filter){
    if(filter < 0.0) filter = 0.0;
    else if(filter > 1.0) filter = 1.0;
    return val * (1.0 - filter) + target * filter;
}


void ledTest(){
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    while(true){
        digitalWrite(14, HIGH);
        //digitalWrite(15, HIGH);
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(14, LOW);
        //digitalWrite(15, LOW);
        digitalWrite(13, LOW);
        delay(500);
    }
}

float smoothstep(float x){
    return x * x * (3.0f - 2.0f * x);
}