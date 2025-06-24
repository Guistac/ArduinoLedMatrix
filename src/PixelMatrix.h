#pragma once
#include "Color.h"
#include <vector>

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
Vec2f operator-(const Vec2f& a, const Vec2f& b){return Vec2f(a.x - b.x, a.y - b.y);}
Vec2f operator+(const Vec2f& a, const Vec2f& b){return Vec2f(a.x + b.x, a.y + b.y);}
Vec2f operator*(const Vec2f& a, const Vec2f& b){return Vec2f(a.x * b.x, a.y * b.y);}
Vec2f operator/(const Vec2f& a, const Vec2f& b){return Vec2f(a.x / b.x, a.y / b.y);}
bool operator==(const Vec2f& a, const Vec2f& b){return a.x == b.x && a.y == b.y;}
Vec2f operator*(const Vec2f& a, const float b){ return Vec2f(a.x * b, a.y * b);}
Vec2f operator*(const float b, const Vec2f& a){ return Vec2f(a.x * b, a.y * b);}
Vec2f operator/(const Vec2f& a, const float b){ return Vec2f(a.x / b, a.y / b);}
Vec2f operator/(const float b, const Vec2f& a){ return Vec2f(a.x / b, a.y / b);}

struct Pixel{
    Color col;
    Vec2f pos;
};


class PixelSegment{
public:
    PixelSegment(uint16_t ledCount, Vec2f startCoord, Vec2f endCoord){
        coordinates.resize(ledCount);
        if(ledCount == 1) {
            coordinates[0] = startCoord;
            return;
        }
        for(int i = 0; i < ledCount; i++){
            float offset = float(i) / float(ledCount - 1);
            float x = startCoord.x + (endCoord.x - startCoord.x) * offset;
            float y = startCoord.y + (endCoord.y - startCoord.y) * offset;
            coordinates[i] = Vec2f(x,y);
        }
    }
    std::vector<Vec2f> coordinates = {};    
};


class PixelMatrix{
public:
    PixelMatrix(size_t pixelCount_) : pixelCount(pixelCount_){
        positions = new Vec2f[pixelCount_];
    }
    PixelMatrix(std::vector<PixelSegment>& segments){
        pixelCount = 0;
        for(auto& segment : segments) pixelCount += segment.coordinates.size();
        positions = new Vec2f[pixelCount];
        int pixel = 0;
        for(auto& segment : segments){
            for(int i = 0; i < segment.coordinates.size(); i++){
                positions[pixel] = segment.coordinates[i];
                pixel++;
            }
        }
    }
    void scale(float factor){
        for(int i = 0; i < pixelCount; i++){
            positions[i].x *= factor;
            positions[i].y *= factor;
        }
    }
    Vec2f getPos(uint32_t i){
        if(i >= pixelCount) return Vec2f(0.0, 0.0);
        return positions[i];
    }
    void flipX(){
        for(int i = 0; i < pixelCount; i++){
            positions[i].x = -positions->x;
        }
    }
    void flipY(){
        for(int i = 0; i < pixelCount; i++){
            positions[i].y *= -1.f;
        }
    }
public:
    uint32_t pixelCount;
    Vec2f* positions;
    //std::vector<Vec2f> positions;
};