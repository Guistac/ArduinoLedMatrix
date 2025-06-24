#pragma once
#include "FrameBuffer.h"

class SK6812RGBW_Strip : public FrameBuffer{
public:

    SK6812RGBW_Strip(uint8_t dataPin_, size_t ledCount_) : FrameBuffer(ledCount_){
        dataPin = dataPin_;
        ledCount = ledCount_;
        colors = new uint32_t[ledCount];
        for(size_t p = 0; p < ledCount; p++) colors[p] = 0x0;
    }

    void setup(){
        pinMode(dataPin, OUTPUT);
    }

    void display(){
        uint32_t pixelCount = min(size, ledCount);
        for(uint32_t i = 0; i < pixelCount; i++){
            uint8_t r = max(min(pixels[i].col.r * 255, 255), 0);
            uint8_t g = max(min(pixels[i].col.g * 255, 255), 0);
            uint8_t b = max(min(pixels[i].col.b * 255, 255), 0);
            uint8_t w = max(min(pixels[i].col.w * 255, 255), 0);
            colors[i] = (g << 24) | (r << 16) | (b << 8) | w;
        }
        noInterrupts();
        for(uint32_t p = 0; p < ledCount; p++){
            for(uint8_t b = 0; b < 32; b++){
                if((colors[p] >> (31 - b)) & 0x1){
                    digitalWriteFast(dataPin, HIGH);
                    delayNanoseconds(one_highDelay_nanos);
                    digitalWriteFast(dataPin, LOW);
                    delayNanoseconds(one_lowDelay_nanos);
                }
                else{
                    digitalWriteFast(dataPin, HIGH);
                    delayNanoseconds(zero_highDelay_nanos);
                    digitalWriteFast(dataPin, LOW);
                    delayNanoseconds(zero_lowDelay_nanos);
                }
            }
        }
        interrupts();
        delayMicroseconds(90);
    }

private:
    uint32_t* colors; //color format: 0xRRGGBBWW
    uint32_t ledCount;
    uint8_t dataPin;
    uint32_t one_highDelay_nanos = 600;
    uint32_t one_lowDelay_nanos = 600;
    uint32_t zero_highDelay_nanos = 300;
    uint32_t zero_lowDelay_nanos = 900;
};


class OneWire_RGB_Strip : public FrameBuffer{
public:

    OneWire_RGB_Strip(uint8_t dataPin_, size_t ledCount_) : FrameBuffer(ledCount_){
        dataPin = dataPin_;
        ledCount = ledCount_;
        colors = new uint32_t[ledCount];
        for(size_t p = 0; p < ledCount; p++) colors[p] = 0x0;
    }

    void setup(){
        pinMode(dataPin, OUTPUT);
    }

    void display(){
        uint32_t pixelCount = min(size, ledCount);
        for(uint32_t i = 0; i < pixelCount; i++){
            uint8_t r = max(min(pixels[i].col.r * 255, 255), 0);
            uint8_t g = max(min(pixels[i].col.g * 255, 255), 0);
            uint8_t b = max(min(pixels[i].col.b * 255, 255), 0);
            colors[i] = (g << 16) | (r << 8) | b;
        }
        noInterrupts();
        for(uint32_t p = 0; p < ledCount; p++){
            for(uint8_t b = 0; b < 24; b++){
                if((colors[p] >> (23 - b)) & 0x1){
                    digitalWriteFast(dataPin, HIGH);
                    delayNanoseconds(one_highDelay_nanos);
                    digitalWriteFast(dataPin, LOW);
                    delayNanoseconds(one_lowDelay_nanos);
                }
                else{
                    digitalWriteFast(dataPin, HIGH);
                    delayNanoseconds(zero_highDelay_nanos);
                    digitalWriteFast(dataPin, LOW);
                    delayNanoseconds(zero_lowDelay_nanos);
                }
            }
        }
        interrupts();
        delayMicroseconds(90);
    }

private:
    uint32_t* colors; //color format: 0xRRGGBBWW
    uint32_t ledCount;
    uint8_t dataPin;
    uint32_t one_highDelay_nanos = 600;
    uint32_t one_lowDelay_nanos = 600;
    uint32_t zero_highDelay_nanos = 300;
    uint32_t zero_lowDelay_nanos = 900;
};
