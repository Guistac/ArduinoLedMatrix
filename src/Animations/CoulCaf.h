#pragma once
#include "../Animation.h"
#include "../Sequencer.h"
#include "../PixelMatrix.h"
#include "../SK6812_RGBW_Strip.h"
#include "SimplexNoise.h"
#include "Sequencer.h"
#include <vector>

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


class Checker : public Animation{
public:
    virtual void setup() override{}
    virtual void fragmentShader(uint32_t i, Vec2f glFragCoord) override {

        glFragCoord = glFragCoord + Vec2f(0.0, -10.0);

        glFragCoord.y += map(sin(Time::getSeconds() * 1.24), -1.0, 1.0, -8.0, 10.0);

        //glFragCoord.x = abs(glFragCoord.x);     

        glFragCoord.rotate(Time::getSeconds() * 1.46);        
        float scale = map(sin(Time::getSeconds() * 3.33), -1.0, 1.0, 0.1, 0.5);
        glFragCoord = glFragCoord * scale;

        float dist = glFragCoord.mag();
        float distort = map(sin(dist + Time::getSeconds() * 9.73), -1.0, 1.0, 0.9, 1.1);
        //glFragCoord = glFragCoord * distort;

        float xoff = cos(glFragCoord.x * PI);
        float yoff = cos(glFragCoord.y * PI + M_PI);
        float mag = 0.5 + (xoff + yoff) / 2.0;
        float highlight = map(mag, .5, 1.0, 0.0, 1.0);
        setPixel(i, Color(mag, highlight, 0.0, 0.0));

        /*
        float scale = map(sin(Time::getSeconds()), -1.0, 1.0, 0.01, 2.0);
        int xmod = int(glFragCoord.x * scale) % 2;
        int ymod = (int(glFragCoord.y * scale) + xmod) % 2;
        setPixel(i, Color(ymod * 100));
        */
    }
};

class CharCoulCaf{
public:

    uint8_t blinkerRelays[8] = {0,1,2,3,4,5,6,7}; //only 6 are actually used
    bool statusLed = false; //blinks on each frame

    //389x WS2811 Leds, data out on pin 14, 10cm per pixel
    SK6812RGBW_Strip ledStrip = SK6812RGBW_Strip(14, 389);
    std::vector<PixelSegment> segments = {
        //Lys
        PixelSegment(44, Vec2f(-896.0, 0.0), Vec2f(-212.0, -2580.0)),
        PixelSegment(43, Vec2f(212.0, -2580.0), Vec2f(896.0, 0.0)),
        //Triangle Bas
        PixelSegment(1, Vec2f(500.0, 0.0), Vec2f(500.0, 0.0)),          
        PixelSegment(3, Vec2f(400.0, 200.0), Vec2f(400.0, 0.0)), //2
        PixelSegment(5, Vec2f(300.0, 0.0), Vec2f(300.0, 400.0)), //4
        PixelSegment(6, Vec2f(200.0, 500.0), Vec2f(200.0, 0.0)), //5
        PixelSegment(6, Vec2f(100.0, 0.0), Vec2f(100.0, 500.0)), //5
        PixelSegment(6, Vec2f(0.0, 500.0), Vec2f(0.0, 0.0)),      //5
        PixelSegment(6, Vec2f(-100.0, 0.0), Vec2f(-100.0, 500.0)),//5
        PixelSegment(6, Vec2f(-200.0, 500.0), Vec2f(-200.0, 0.0)),//5
        PixelSegment(5, Vec2f(-300.0, 0.0), Vec2f(-300.0, 400.0)),//4
        PixelSegment(3, Vec2f(-400.0, 200.0), Vec2f(-400.0, 0.0)),//2
        PixelSegment(1, Vec2f(-500.0, 0.0), Vec2f(-500.0, 0.0)),
        //Base Coté 1
        PixelSegment(10, Vec2f(727,-55), Vec2f(567,-941)),
        PixelSegment(10, Vec2f(470,-914), Vec2f(631,-29)),
        PixelSegment(10, Vec2f(534,-2), Vec2f(374,-888)),
        //Base Fase
        PixelSegment(2, Vec2f(500,-100), Vec2f(500,0)),
        PixelSegment(6, Vec2f(400,0), Vec2f(400,-500)),
        PixelSegment(10, Vec2f(300,-900), Vec2f(300,0)),
        PixelSegment(10, Vec2f(200,0), Vec2f(200,-900)),
        PixelSegment(10, Vec2f(100,-900), Vec2f(100,0)),
        PixelSegment(10, Vec2f(0,0), Vec2f(0,-900)),
        PixelSegment(10, Vec2f(-100,-900), Vec2f(-100,0)),
        PixelSegment(10, Vec2f(-200,0), Vec2f(-200,-900)),
        PixelSegment(10, Vec2f(-300,-900), Vec2f(-300,0)),
        PixelSegment(6, Vec2f(-400,0), Vec2f(-400,-500)),
        PixelSegment(2, Vec2f(-500,-100), Vec2f(-500,0)),
        //Base Coté 2
        PixelSegment(10, Vec2f(-534,-2), Vec2f(-374,-888)),
        PixelSegment(10, Vec2f(-470,-914), Vec2f(-631,-29)),
        PixelSegment(10, Vec2f(-727,-55), Vec2f(-567,-941)),
        //Pointe Coté 2
        PixelSegment(7, Vec2f(-450,-1029), Vec2f(-342,-1619)),
        PixelSegment(17, Vec2f(-67,-2577), Vec2f(-353,-1003)),
        //Pointe Face
        PixelSegment(2, Vec2f(-300,-1000), Vec2f(-300,-1100)),
        PixelSegment(7, Vec2f(-200,-1600), Vec2f(-200,-1000)),
        PixelSegment(13, Vec2f(-100,-1000), Vec2f(-100,-2200)),
        PixelSegment(16, Vec2f(0,-2500), Vec2f(0,-1000)),
        PixelSegment(13, Vec2f(100,-1000), Vec2f(100,-2200)),
        PixelSegment(7, Vec2f(200,-1600), Vec2f(200,-1000)),
        PixelSegment(2, Vec2f(300,-1000), Vec2f(300,-1100)),
        //Pointe Coté 1
        PixelSegment(17, Vec2f(67,-2577), Vec2f(353,-1003)),
        PixelSegment(7, Vec2f(450,-1029), Vec2f(342,-1619)),
    };

    //LinesAnimation lines_anim;
    //Chladni chladni_anim;
    Pond pond_anim;
    //Pond2 pond2_anim;
    ArrowAnimation arrow_anim;
    CircleAnimation circle_anim;
    Checker cherckerAnim;

    Sequencer sequencer = Sequencer({
        Sequence(&cherckerAnim,    60.0, 1.0),
        Sequence(&pond_anim,    60.0, 1.0),
        Sequence(&arrow_anim,   60.0, 1.0),
        Sequence(&circle_anim,  60.0, 1.0)
    });
    Time::IntervalTimer frameTimer = Time::IntervalTimer(100.0);


    void init(){
        pinMode(13, OUTPUT);
        for(int i = 0; i < 8; i++) pinMode(blinkerRelays[i], OUTPUT);
        randomSeed(analogRead(A0));
        SimplexNoise::init(random(100000));

        PixelMatrix matrix(segments);
        matrix.scale(0.01);
        matrix.flipY();
        ledStrip.setup();
        ledStrip.setPixelMatrix(matrix);
        sequencer.configureMatrix(matrix);

    }

    void update(){
        updateBlinkerRelays();
        if(frameTimer.isTriggered()){
            sequencer.draw();
            ledStrip.addFrameBuffer(&sequencer);
            ledStrip.display();
            statusLed = !statusLed;
            digitalWrite(13, statusLed);
        }

    }

    void updateBlinkerRelays(){
        float wow = sin(float(millis()) * 0.0005);
        for(int i = 0; i < 8; i++){
            bool state = SimplexNoise::noise(float(millis()) * 0.001, i + wow * 5) > 0; 
            digitalWrite(blinkerRelays[i], state);
        }
    }

};