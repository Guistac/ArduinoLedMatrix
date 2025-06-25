#pragma once
#include "../Animation.h"

class PondAnimation : public Animation{
public:
    Vec2f middle = Vec2f(7.5, 6);
    virtual void draw() override{
        Serial.println("Pond");
        float now_seconds = Time::getSeconds();
        for(uint32_t i = 0; i < size; i++){
            {
                float rad = sin(now_seconds * 0.5) * 5.0;
                Vec2f center(middle.x + sin(now_seconds) * rad, middle.y + cos(now_seconds) * rad);
                Vec2f pos = pixels[i].pos;
                float dist = pos.dist(center);
                float step = 10.0;
                float b = map(fmod(-dist + now_seconds * 5 + sin(now_seconds) * 4.0, step), 0, step, 0.0, 1.0);
                b = map(triangle(b), 0.5, 1.0, 0.0, 1.0);
                //float noise = float(random(950, 1000)) / 1000.0;
                //bool b_blue = random(1000) > 998;
                float blue = map(b, 0.0, 0.2, 0.02, 0.0);
                blue = max(blue, 0.02);
                Color c(0.0, 0.0, blue, b);
                setPixel(i,c);
            }
        }
    }
};

class RadarAnimation : public Animation{
public:
    Vec2f middle = Vec2f(7.5, 6);
    virtual void setup() override{}
    virtual void draw() override{
        Serial.println("Radar");
        float now_seconds = Time::getSeconds();
        for(uint32_t i = 0; i < size; i++){
            Vec2f pos = pixels[i].pos;
            double t_rot = 10.0 + now_seconds * .5;
            Vec2f movPos(pos.x + sin(t_rot) * .5, pos.y + cos(t_rot) * .5);
            double ang = atan2(movPos.x - middle.x, movPos.y - middle.y);
            ang += t_rot;
            ang = fmod(ang, 2 * M_PI) - M_PI;
            double angNorm = map(ang, -M_PI, M_PI, 0.0, 1.0);
            double br = triangle(map(angNorm, .9, 1.0, 0.0, 1.0));
            double bll = map(br, 0.5, 1.0, 0.0, 1.0);
            Color c(0.0, 0.0, bll, br);
            setPixel(i, c);
        }
        //mask middle to avoid blinking of pixels
        drawCircle(middle, 2.0, 0.0, Color(0.0));
    }
};

class MovingDotAnimation : public Animation{
public:
    Vec2f middle = Vec2f(7.5, 6);
    Time::Timer circleTimer;
    Vec2f circleTarget = middle;
    Vec2f circleActual = middle;
    float circleFilter = 0.025;

    virtual void draw() override{
        Serial.println("MovingDot");
        clear(Color(0.0, 0.02));
        if(circleTimer.isExpired()){
            float w = 7;
            float h = 5;
            Vec2f min(middle.x - w, middle.y - h);
            Vec2f max(middle.x + w, middle.y + h);

            Vec2f newTarget;
            float moveDistance = 0.0;
            while(moveDistance < 8.0){
                newTarget = Vec2f(randomF(min.x, max.x), randomF(min.y, max.y));
                moveDistance = circleTarget.dist(newTarget);
            }
            circleTarget = newTarget;

            float newTime = randomF(2.0, 5.0);
            circleFilter = randomF(0.001, 0.04);
            //Serial.printf("Time: %.1fs  Filter:%.3f\n", newTime, circleFilter);
            circleTimer.start(newTime);
        }
        circleActual.x = iirFilter(circleActual.x, circleTarget.x, circleFilter);
        circleActual.y = iirFilter(circleActual.y, circleTarget.y, circleFilter);
        for(int i = 0; i < 10; i++){
            if(random(100) > 90){
                float hue = fmod(float(random(50, 60)) * 0.01, 1.0);
                float brightness = random(80) * 0.01;
                setPixel(random(size), ColorHSV(hue, 0.8, brightness), ADD);
            }
        }
        //Vec2f pointPos(middle.x + sin(now_seconds*5) * 4, middle.y + cos(now_seconds*5) * 4);
        Vec2f pointPos = circleActual;
        drawCircle(pointPos, 2.0, 3, Color(0.5, 0.5, 0.0, 1.0), ADD);
        //pointAnimation.drawCircle(pointPos, 1.0, 2.0, ColorHSV(0.7, 0.5, 1.0, 1.0), ADD);
    }
};

class LightningAnimation : public Animation{
public:
    Vec2f middle = Vec2f(7.5, 6);
    virtual void draw() override{
        Serial.println("Lightning");
        //Random Circles
        //randomCirclesAnimation.clear(Color(0.0, 0.05));
        clear(Color(0.0, 0.1));
        if(randomF(1.0) > 0.97){
            float w = 10;
            float h = 7;
            Vec2f min(middle.x - w, middle.y - h);
            Vec2f max(middle.x + w, middle.y + h);
            Vec2f pos(randomF(min.x, max.x), randomF(min.y, max.y));
            float rad = randomF(2.0, 7.0);
            drawCircle(pos, rad, rad * 0.5, ColorHSV(randomF(1.0), 0.2, 1.0, 1.0));
        }
        for(int i = 0; i < 208; i++){
            if(randomF(1000) > 970){
                setPixel(i, ColorHSV(0.6, 1.0, .3));
            }
        }
    }
};

class DvdAnimation : public Animation{
public:
    Vec2f middle = Vec2f(7.5, 6);
    Vec2f boxPosition = middle;
    Vec2f boxSize = Vec2f(4.5, 4.5);
    Vec2f boxVel = Vec2f(0.1, 0.04);
    Vec2f box2Pos = Vec2f(middle.x - 4.0, middle.y - 4.0);
    Vec2f box2Vel = boxVel;
    int width = 16;
    int height = 13;

    virtual void setup() override{}
    virtual void draw() override{
        Serial.println("DVD");
        //DVD box
        clear(Color(0.0, 0.1));

        Color boxColor;
        bool b_top = false;
        bool b_bottom = false;
        bool b_left = false;
        bool b_right = false;

        bool b_bounce = false;
        if(boxPosition.x < boxSize.x * 0.5){
            b_bounce = true;
            b_left = true;
        }
        else if(boxPosition.x > width - boxSize.x * 0.5){
            b_bounce = true;
            b_right = true;
        }

        if(boxPosition.y < boxSize.y * 0.5){
            b_bounce = true;
            b_top = true;
        }
        else if(boxPosition.y > height - boxSize.y * 0.5){
            b_bounce = true;
            b_bottom = true;
        }

        
        //if(b_bounce) boxColor = Color(1.0, 1.0, 1.0, 1.0);
        /*else */boxColor = Color(1.0);
        drawBox(boxPosition, boxSize, 2.0, boxColor);
        //boxAnimation.drawBox(box2Pos, boxSize, 2.0, Color(0.5, 0.0, 0.0, 0.25));
        drawBox(box2Pos, boxSize, 2.0, Color(0.0, 0.0, 1.0));
        /*
        if(b_top) for(int i = 0; i < 16; i++) boxAnimation.setPixel(i, Color(1.0, 1.0, 0.0));
        if(b_bottom) for(int i = 192; i < 208; i++) boxAnimation.setPixel(i, Color(1.0, 1.0, 0.0));
        if(b_left){
            for(int i = 0; i < 7; i++) boxAnimation.setPixel(i * 32, Color(1.0, 0.5, 1.0));
            for(int i = 0; i < 6; i++) boxAnimation.setPixel(31 + i * 32, Color(1.0, 0.5, 1.0));
        }
        if(b_right){
            for(int i = 0; i < 7; i++) boxAnimation.setPixel(15 + i * 32, Color(1.0, 0.5, 0.0));
            for(int i = 0; i < 6; i++) boxAnimation.setPixel(16 + i * 32, Color(1.0, 0.5, 0.0));
        }
        */
        boxPosition.x += boxVel.x;
        boxPosition.y += boxVel.y;
        float bounceMargin = 1.0;
        if(boxPosition.x < boxSize.x * 0.5 - bounceMargin || boxPosition.x > width - boxSize.x * 0.5 + bounceMargin) boxVel.x *= -1.0;
        if(boxPosition.y < boxSize.y * 0.5 - bounceMargin || boxPosition.y > height - boxSize.y * 0.5 + bounceMargin) boxVel.y *= -1.0;

        box2Pos.x += box2Vel.x;
        box2Pos.y += box2Vel.y;
        if(box2Pos.x < boxSize.x * 0.5 - bounceMargin || box2Pos.x > width - boxSize.x * 0.5 + bounceMargin) box2Vel.x *= -1.0;
        if(box2Pos.y < boxSize.y * 0.5 - bounceMargin || box2Pos.y > height - boxSize.y * 0.5 + bounceMargin) box2Vel.y *= -1.0;

    }
};


/*

/*
#include "SK6812_RGBW_Strip.h"
int width = 16;
int height = 13;
uint32_t ledCount = width * height;
OneWire_RGB_Strip ledStrip(0, 200);

PondAnimation pond;
RadarAnimation radar;
MovingDotAnimation movingDot;
LightningAnimation lighting;
DvdAnimation dvd;
std::vector<Animation*> animations = {
    &pond,
    &radar,
    &movingDot,
    &lighting,
    &dvd
};
Sequencer sequencer({
    Sequence(&pond,         180.0, 30.0),
    Sequence(&radar,        180.0, 30.0),
    Sequence(&movingDot,    180.0, 30.0),
    Sequence(&lighting,     180.0, 30.0),
    Sequence(&dvd,          180.0, 30.0)
});
Time::IntervalTimer frameTimer(100.0);

    /*
    //CAMILLE
    PixelMatrix ledMatrix(ledCount);
    int c = 0;
    for(int y = 0; y < height; y++){
        bool b_heightEven = y % 2 == 0;
        for(int x = 0; x < width; x++){
            ledMatrix.positions[c].y = y;
            ledMatrix.positions[c].x = b_heightEven ? x : width - x - 1;
            c++;
        }
    }
    */
    //for(auto animation : animations) animation->configureMatrix(ledMatrix);
    //anim->configureMatrix(ledMatrix);
    //sequencer.configureMatrix(ledMatrix);
*/
*/