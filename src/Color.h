#pragma once
#include <FastLED.h>
#include "Utilities.h"

float test = lerp(0.0, 1.0, 0.5);

struct Color{
    Color(){}
    Color(float white, float a_ = 1.0){
        r = 0.0;
        g = 0.0;
        b = 0.0;
        w = white;
        a = a_;
        normalize();
    }
    Color(float r_, float g_, float b_, float w_ = 0.0, float a_ = 1.0){
        r = r_;
        g = g_;
        b = b_;
        w = w_;
        a = a_;
        normalize();
    }
    void normalize(){
        r = min(max(r, 0.0), 1.0);
        g = min(max(g, 0.0), 1.0);
        b = min(max(b, 0.0), 1.0);
        w = min(max(w, 0.0), 1.0);
        a = min(max(a, 0.0), 1.0);
    }
    void setAlpha(float a_){
        a = min(max(a_, 0.0), 1.0);
    }
    void set(const Color& in){
        r = in.r;
        g = in.g;
        b = in.b;
        w = in.w;
        a = in.a;
        normalize();
    }
    void mixNormal(const Color& in){
        r = r * (1.0 - in.a) + in.r * in.a;
        g = g * (1.0 - in.a) + in.g * in.a;
        b = b * (1.0 - in.a) + in.b * in.a;
        w = w * (1.0 - in.a) + in.w * in.a;
        normalize();
    }
    void mixAdd(const Color& in){
        r = r + in.r * in.a;
        g = g + in.g * in.a;
        b = b + in.b * in.a;
        w = w + in.w * in.a;
        normalize();
    }
    void mixSubtract(const Color& in){
        r = r - in.r * in.a;
        g = g - in.g * in.a;
        b = b - in.b * in.a;
        w = w - in.w * in.a;
        normalize();
    }
    void mixMultiply(const Color& in){
        r = r * (1.0 - in.a) + r * in.r * in.a;
        g = g * (1.0 - in.a) + g * in.g * in.a;
        b = b * (1.0 - in.a) + b * in.b * in.a;
        w = w * (1.0 - in.a) + w * in.w * in.a;
        normalize();
    }
    void mixDivide(const Color& in){
        r = r * (1.0 - in.a) + r / in.r * in.a;
        g = g * (1.0 - in.a) + g / in.g * in.a;
        b = b * (1.0 - in.a) + b / in.b * in.a;
        w = w * (1.0 - in.a) + w / in.w * in.a;
        normalize();
    }
    Color lerp(const Color& in, float mix){
        if(mix > 1.0) mix = 1.0;
        if(mix < 0.0) mix = 0.0;
        float xim = 1.0 - mix;
        return Color(
            r * xim + in.r * mix,
            g * xim + in.g * mix,
            b * xim + in.b * mix,
            w * xim + in.w * mix,
            a * xim + in.a * mix
        );
    }
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
    float w = 0.0;
    float a = 1.0;
};

struct ColorHSV : public Color{
    ColorHSV(float h, float s, float v, float w = 0.0, float a = 1.0){
        uint8_t h8 = min(max(h, 0.0), 1.0) * 255;
        uint8_t s8 = min(max(s, 0.0), 1.0) * 255;
        uint8_t v8 = min(max(v, 0.0), 1.0) * 255;
        w = min(max(w, 0.0), 1.0);
        a = min(max(a, 0.0), 1.0);
        CRGB rgb8;
        hsv2rgb_rainbow(CHSV(h8,s8,v8), rgb8);
        r = float(rgb8.r) / 255.0;
        g = float(rgb8.g) / 255.0;
        b = float(rgb8.b) / 255.0;
    }
};

enum ColorMixMode{
    NORMAL,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};