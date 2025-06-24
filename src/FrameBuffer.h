#pragma once
#include "Color.h"
#include "PixelMatrix.h"

class FrameBuffer{
public:
    FrameBuffer(){}
    FrameBuffer(int size_){
        size = size_;
        pixels = new Pixel[size];
    }

    void setPixelMatrix(const PixelMatrix& matrix){
        uint32_t pixelCount = min(matrix.pixelCount, size);
        for(uint32_t i = 0; i < pixelCount; i++) pixels[i].pos = matrix.positions[i];
    }

    void clear(const Color& color){
        for(uint32_t i = 0; i < size; i++) setPixel(i, color, NORMAL);
    }

    void clear(){
        Color black(0.0);
        for(uint32_t i = 0; i < size; i++) setPixel(i, black, NORMAL);
    }

    void addFrameBuffer(FrameBuffer* input, float opacity = 1.0, ColorMixMode mode = NORMAL){
        int pixelCount = min(input->size, size);
        for(int i = 0; i < pixelCount; i++){
            Color newColor = input->pixels[i].col;
            newColor.a = opacity;
            setPixel(i, newColor, mode);
        }
    }

    void setPixel(uint32_t pixel, const Color& color, ColorMixMode mode = NORMAL){
        if(pixel >= size) return;
        Pixel& p = pixels[pixel];
        switch(mode){
            default:
            case NORMAL:
                p.col.mixNormal(color);
                break;
            case ADD:
                p.col.mixAdd(color);
                break;
            case SUBTRACT:
                p.col.mixSubtract(color);
                break;
            case MULTIPLY:
                p.col.mixMultiply(color);
                break;
            case DIVIDE:
                p.col.mixDivide(color);
                break;
        }
    }

    void drawCircle(Vec2f pos, float radius, float edgeGradient, const Color& color, ColorMixMode mode = NORMAL){
        float gradientRadiusStart = radius - edgeGradient;
        for(uint32_t i = 0; i < size; i++){
            float dist = pos.dist(pixels[i].pos);
            float gradientPosition = map(dist, gradientRadiusStart, radius, 1.0, 0.0);
            if(dist < gradientRadiusStart) setPixel(i, color, mode);
            else if(dist < radius) {
                Color fadedColor = color;
                fadedColor.setAlpha(color.a * gradientPosition);
                setPixel(i, fadedColor, mode);
            }
        }
    }

    void drawBox(Vec2f p, Vec2f s, float edgeGradient, const Color& color, ColorMixMode mode = NORMAL){
        Vec2f rad(s.x * 0.5, s.y * 0.5);
        for(uint32_t i = 0; i < size; i++){
            float distX = abs(pixels[i].pos.x - p.x);
            float distY = abs(pixels[i].pos.y - p.y);
            if(distX < rad.x && distY < rad.y) {
                bool b_gradX = distX > rad.x - edgeGradient;
                bool b_gradY = distY > rad.y - edgeGradient;
                if(b_gradX || b_gradY){
                    float gradPosX = map(distX, rad.x - edgeGradient, rad.x, 1.0, 0.0);
                    float gradPosY = map(distY, rad.y - edgeGradient, rad.y, 1.0, 0.0);
                    float gradPos;
                    if(b_gradX && b_gradY) gradPos = min(gradPosX, gradPosY);
                    else if(b_gradX) gradPos = gradPosX;
                    else gradPos = gradPosY;
                    Color faded = color;
                    faded.setAlpha(color.a * gradPos);
                    setPixel(i, faded, mode);
                }
                else setPixel(i, color, mode);
            }
        }
    }


    FrameBuffer getEmptyFramebuffer(){
        FrameBuffer output(size);
        for(uint32_t i = 0; i < size; i++) output.pixels[i].pos = pixels[i].pos;
        return output;
    }

    Pixel* pixels;
    uint32_t size;
};