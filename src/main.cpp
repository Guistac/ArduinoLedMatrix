#include "SK6812_RGBW_Strip.h"
#include "Animations/Camille.h"
#include "Sequencer.h"
#include <vector>
#include "Animations/CoulCaf.h"
#include "SimplexNoise.h"

//int width = 16;
//int height = 13;
//uint32_t ledCount = width * height;
SK6812RGBW_Strip ledStrip(14, 389);
//OneWire_RGB_Strip ledStrip(0, 200);

std::vector<PixelSegment> segments = {
    PixelSegment(44, Vec2f(-896.0, 0.0), Vec2f(-212.0, -2580.0)),
    PixelSegment(43, Vec2f(212.0, -2580.0), Vec2f(896.0, 0.0)),

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

    PixelSegment(10, Vec2f(727,-55), Vec2f(567,-941)),
    PixelSegment(10, Vec2f(470,-914), Vec2f(631,-29)),
    PixelSegment(10, Vec2f(534,-2), Vec2f(374,-888)),

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

    PixelSegment(10, Vec2f(-534,-2), Vec2f(-374,-888)),
    PixelSegment(10, Vec2f(-470,-914), Vec2f(-631,-29)),
    PixelSegment(10, Vec2f(-727,-55), Vec2f(-567,-941)),

    PixelSegment(7, Vec2f(-450,-1029), Vec2f(-342,-1619)),
    PixelSegment(17, Vec2f(-67,-2577), Vec2f(-353,-1003)),

    PixelSegment(2, Vec2f(-300,-1000), Vec2f(-300,-1100)),
    PixelSegment(7, Vec2f(-200,-1600), Vec2f(-200,-1000)),
    PixelSegment(13, Vec2f(-100,-1000), Vec2f(-100,-2200)),
    PixelSegment(16, Vec2f(0,-2500), Vec2f(0,-1000)),
    PixelSegment(13, Vec2f(100,-1000), Vec2f(100,-2200)),
    PixelSegment(7, Vec2f(200,-1600), Vec2f(200,-1000)),
    PixelSegment(2, Vec2f(300,-1000), Vec2f(300,-1100)),

    PixelSegment(17, Vec2f(67,-2577), Vec2f(353,-1003)),
    PixelSegment(7, Vec2f(450,-1029), Vec2f(342,-1619)),
};

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


ArrowAnimation arrow_anim;
LinesAnimation lines_anim;
Chladni chladni_anim;
Pond pond_anim;
Animation *anim;

uint8_t relays[8] = {0,1,2,3,4,5,6,7};

CoulCaf coulCaf;

void setup() {
    pinMode(13, OUTPUT);
    randomSeed(analogRead(A0));
    SimplexNoise::init(random(100000));

    if(false){
        pinMode(14, OUTPUT);
        pinMode(15, OUTPUT);
        while(true){
            digitalWrite(14, HIGH);
            //digitalWrite(15, HIGH);
            digitalWrite(13, HIGH);
            delay(500);
            digitalWrite(14, LOW);
            //digitalWrite(15, LOW);
            digitalWrite(13, LOW);
            delay(500);
        }
    }

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

    PixelMatrix ledMatrix(segments);
    ledMatrix.scale(0.01);
    ledMatrix.flipY();

    ledStrip.setup();
    ledStrip.setPixelMatrix(ledMatrix);


    coulCaf.init(ledMatrix);


    for(auto animation : animations) animation->configureMatrix(ledMatrix);
    anim = &pond_anim;
    anim->configureMatrix(ledMatrix);
    sequencer.configureMatrix(ledMatrix);

    for(int i = 0; i < 8; i++) pinMode(relays[i], OUTPUT);
}

void loop() {

    float wow = sin(float(millis()) * 0.0005);
    for(int i = 0; i < 8; i++){
        bool state = SimplexNoise::noise(float(millis()) * 0.001, i + wow * 5) > 0; 
        digitalWrite(relays[i], state);
    }


    if(frameTimer.isTriggered()){
        //sequencer.draw();
        //ledStrip.addFrameBuffer(&sequencer);



        if(true){
            float radius = map(sin(float(millis()) * 0.01), -1.0, 1.0, 0, 17);
            ledStrip.clear(Color(0.0, 0.0, 0.0));
            ledStrip.drawCircle(Vec2f(0,10), radius, 5, Color(0.0, 1.0, 0.0, 0.0));
            ledStrip.drawCircle(Vec2f(0,10), radius * 0.8, 5, Color(0.0, 0.0, 0.0, 1.0));
            ledStrip.drawCircle(Vec2f(0,10), radius * 0.6, 5, Color(1.0, 0.0, 0.0, 0.0));
            ledStrip.drawCircle(Vec2f(0,10), radius * 0.4, 5, Color(1.0, 0.0, 1.0, 0.0));
            /*
            for(int i = 0; i < ledStrip.size; i++){
                if(i < 44) ledStrip.setPixel(i, Color(1.0, 0.0, 0.0));
                else if(i < 87) ledStrip.setPixel(i, Color(0.0, 0.0, 1.0));
                else if(i < 135) ledStrip.setPixel(i, Color(0.0, 1.0, 0.0));
                else if(i < 165) ledStrip.setPixel(i, Color(0.0, 0.0, 0.0, 1.0));
                else if(i < 251) ledStrip.setPixel(i, Color(1.0, 0.0, 1.0, 0.0));
                else if(i < 281) ledStrip.setPixel(i, Color(0.0, 1.0, 1.0, 0.0));
                else if(i < 305) ledStrip.setPixel(i, Color(0.0, 1.0, 0.0, 1.0));
                else if(i < 365) ledStrip.setPixel(i, Color(1.0, 1.0, 0.0, 1.0));
                else if(i < 389) ledStrip.setPixel(i, Color(0.0, 0.0, 1.0, 1.0));
                else ledStrip.setPixel(i, Color(0.0, 0.0, 0.0));
            }
                */
        }

        //arrow_anim.draw(); ledStrip.addFrameBuffer(&arrow_anim);
        //anim->draw();
        //ledStrip.addFrameBuffer(anim);


/*
        ledStrip.clear(Color(0.0, 1.0, 0.0, 0.0));
        for(int i = 0; i < ledStrip.size; i++){
        float h = (sin(float(i) * 0.3 - float(millis()) * 0.003) + 1.0) / 2.0;
        ledStrip.setPixel(i, ColorHSV(h, 1.0, 1.0));
        ledStrip.setPixel(i, ColorHSV(h, 1.0, 1.0));
        ledStrip.setPixel(i, Color(0.0, 0.0, 0.0, h));
        }
        //x w g b
*/
 

        coulCaf.draw();
        //ledStrip.addFrameBuffer(coulCaf.getFrameBuffer());
        ledStrip.addFrameBuffer(&coulCaf.sequencer);

        ledStrip.display();

        static bool led = true;
        led = !led;
        digitalWrite(13, led);
    }
}