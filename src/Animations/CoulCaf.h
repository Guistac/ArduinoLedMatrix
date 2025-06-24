#pragma once
#include "../Animation.h"
#include "../Sequencer.h"
#include "../PixelMatrix.h"

class LinesAnimation : public Animation{
public:
    virtual void setup() override{}
    virtual void draw() override{
    #define rep(x,m) (fmodf(x,m)-(m)*0.5f)
    #define ab(x) (x<0.0f?-(x):x)
    #define fract(x) (fmodf(x,1.f))
        float w, t = Time::getSeconds()*1.f;
        float o = rnd(floorf(t*1.4f))*32.f;
        for (int i = 0; i < size; i++) {
            Vec2f pos = pixels[i].pos;
            float x=pos.x,y=pos.y;
            float h = rnd(x-o);
            y = -rep(ab(y-t*55.f*map(h,0.f,1.f,-4.f,4.f)),24.f);
            w = ab(y-10.f) - (map(h,0.f,1.f,4.f,18.f));
            float wi = 1.f-w;
            float r = h < 0.7f ? 0.8f : 0.025f;
            float g = h > 0.5f ? 0.9f : 0.012f;
            float b = max(pos.y / 25.f - h,0.f) * 0.025f;
            setPixel(i, Color(r,wi,g*w*0.85f,b));
        }
    }

    float rnd(float x) {
        return fract(sinf(x*1317.532)*721.43);
    }
    float h11(float x) {
        float t = fract(x);
        return 0.5f*(t*rnd(floorf(x)) + (1.f-t)*rnd(floorf(x)+1.f));
    }

};



class Chladni : public Animation{
public:
    virtual void draw() override{
        Vec2f iResolution(10, 26);
        for(int i = 0; i < size; i++){

            Vec2f p = (pixels[i].pos - Vec2f(0.0, 10.0));
            p = p * 50.0f;
            float t = Time::getSeconds() *0.01f;
            t = rep(t,.3f*PI);
            p = p + Vec2f(sinf(t*0.8f*p.y)*t*20.f, sinf(t*0.2f*p.x)*t*12.f);
            /*
            Color s1(1.0, 1.0, 7.0, 2.0);
            Color s2(-2.0, 1.0, 4.0, 4.6);
            
            float t = 0.5 * sinf(Time::getSeconds() * 10.0) + 0.5;
            
            float a = lerp(s1.r, s2.r, t);
            float b = lerp(s1.g, s2.g, t);
            float n = lerp(s1.b, s2.b, t);
            float m = lerp(s1.w, s2.w, t);
                
            float max_amp = ab(a) + ab(b);
            float amp = a * sinf(PI*n*p.x) * sinf(PI*m*p.y) + b * sinf(PI*m*p.x) * sinf(PI*n*p.y);

            ///float col = step(abs(amp), 0.1);
            */
            setPixel(i, Color(0.0, 0.25f*(sinf(p.x)+sinf(p.y))+0.5f, 0.0, 0.0));
            //fragColor = vec4(vec3(col),1.0);
        }
    }
};


class Pond : public Animation{
public:
    virtual void draw() override{
        Vec2f center(sinNorm(M_PI_4 + Time::getSeconds() * 2.0) * 5.0, 3.0 + sinNorm(Time::getSeconds() * 2.0) * 10.0);
        Vec2f center2(0.0, 20.0);
        for(int i = 0; i < size; i++){
            Vec2f pos = pixels[i].pos;
            float dist = pos.dist(center);
            float dist2 = pos.dist(center2);
            float ampR = sinNorm(dist * 2.0 + Time::getSeconds() * 10.0);
            float ampW = sinNorm(dist2 * 2.0 + Time::getSeconds() * 5.0);
            float ampX = 0.0;
            float crest = 1.4;
            if(ampR + ampW > crest) ampX = map(ampR + ampW, crest, 2.0, 0.0, 1.0);
            //float ampR = sinNorm((dist / 2) - Time::getSeconds() * 20.0);
            setPixel(i, Color(ampW, 0.0, 0.0, ampX));
        }
    }
};


class Pond2 : public Animation{
public:
    virtual void draw() override{
        Vec2f center(sinNorm(M_PI_4 + Time::getSeconds() * 2.0) * 5.0, 3.0 + sinNorm(Time::getSeconds() * 2.0) * 10.0);
        Vec2f center2(0.0, 20.0);
        for(int i = 0; i < size; i++){
            Vec2f pos = pixels[i].pos;
            float dist = pos.dist(center);
            float dist2 = pos.dist(center2);
            float ampR = sinNorm(dist * 2.0 + Time::getSeconds() * 10.0);
            float ampW = sinNorm(dist2 * 2.0 + Time::getSeconds() * 5.0);
            float ampX = 0.0;
            float crest = 1.4;
            if(ampR + ampW > crest) ampX = map(ampR + ampW, crest, 2.0, 0.0, 1.0);
            //float ampR = sinNorm((dist / 2) - Time::getSeconds() * 20.0);
            setPixel(i, Color(ampW, 0.0, ampR, ampX));
        }
    }
};


class ArrowAnimation : public Animation{
public:
    virtual void setup() override{}
    virtual void draw() override{
        float thick = 2.0f;
        float w, t = Time::getSeconds() * 30.0f;
        for (int i = 0; i < size; i++) {
            Vec2f pos = pixels[i].pos;
            float x=pos.x,y=pos.y;
            y = -rep(-(y-t),10.0f);
            //y *= 0.8f;
            w = ab(.0f-ab(x)-y)-thick;
            w = fmaxf(-w,0.f)/thick;
            setPixel(i, Color(1.f*(1.f-w),1.0f*w,0,1.0f*w));
        }
    }
};

class CircleAnimation : public Animation{
public:
    virtual void setup() override{}
    virtual void draw() override{
        float radius = map(sin(float(millis()) * 0.01), -1.0, 1.0, 0, 17);
        clear(Color(0.0, 0.0, 0.0));
        drawCircle(Vec2f(0,10), radius, 5, Color(0.0, 1.0, 0.0, 0.0));
        drawCircle(Vec2f(0,10), radius * 0.8, 5, Color(0.0, 0.0, 0.0, 1.0));
        drawCircle(Vec2f(0,10), radius * 0.6, 5, Color(1.0, 0.0, 0.0, 0.0));
        drawCircle(Vec2f(0,10), radius * 0.4, 5, Color(1.0, 0.0, 1.0, 0.0));
        Serial.println(millis());
    }
};

class CoulCaf{
public:

    LinesAnimation lines_anim;
    Chladni chladni_anim;
    Pond pond_anim;
    Pond2 pond2_anim;
    ArrowAnimation arrow_anim;
    CircleAnimation circle_anim;

    Sequencer sequencer = Sequencer({
        Sequence(&pond_anim,    60.0, 1.0),
        Sequence(&arrow_anim,   60.0, 1.0),
        Sequence(&circle_anim,  60.0, 1.0)
    });
    void init(PixelMatrix& matrix){
        sequencer.configureMatrix(matrix);
        for(int i = 0; i < sequencer.sequences.size(); i++){
            sequencer.sequences[i].animation->configureMatrix(matrix);
        }
    }
    void draw(){
        sequencer.draw();
    }
    FrameBuffer* getFrameBuffer(){
        return &sequencer;
    }

};