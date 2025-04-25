#pragma once
#include "Color.h"

struct Vec2f{
    Vec2f(){}
    Vec2f(float x_, float y_) : x(x_), y(y_){}
    float dist(Vec2f& other){ return sqrt(sq(x - other.x) + sq(y - other.y)); }
    Vec2f lerp(const Vec2f& in, float mix){
        if(mix < 0.0) mix = 0.0;
        else if(mix > 0.0) mix = 1.0;
        float xim = 1.0 - mix;
        return Vec2f(x * xim + in.x * mix, y * xim + in.y * mix);
    }
    float x = 0.0;
    float y = 0.0;
};

struct Pixel{
    Color col;
    Vec2f pos;
};

class PixelMatrix{
public:
    PixelMatrix(size_t pixelCount_) : size(pixelCount_){
        positions = new Vec2f[size];
    }
    Vec2f getPos(uint32_t i){
        if(i >= size) return Vec2f(0.0, 0.0);
        return positions[i];
    }
public:
    uint32_t size;
    Vec2f* positions;
};