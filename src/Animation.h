#pragma once
#include "FrameBuffer.h"

class Animation : public FrameBuffer{
public:
    virtual void setup(){}
    virtual void draw() = 0;
    void configureMatrix(const PixelMatrix& matrix){
        size = matrix.size;
        pixels = new Pixel[size];
        for(uint32_t i = 0; i < size; i++) pixels[i].pos = matrix.positions[i];
    }
};