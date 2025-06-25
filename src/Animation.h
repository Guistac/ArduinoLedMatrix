#pragma once
#include "FrameBuffer.h"

class Animation : public FrameBuffer{
public:
    virtual void setup(){}
    virtual void draw(){
        for(uint32_t i = 0; i < size; i++){
            fragmentShader(i, pixels[i].pos);
        }
    }
    virtual void fragmentShader(uint32_t i, Vec2f glFragCoord){}
    virtual void configureMatrix(const PixelMatrix& matrix){
        size = matrix.pixelCount;
        pixels = new Pixel[size];
        for(uint32_t i = 0; i < size; i++) pixels[i].pos = matrix.positions[i];
    }
};