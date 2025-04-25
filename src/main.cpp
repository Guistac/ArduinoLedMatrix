#include "SK6812_RGBW_Strip.h"
#include "Animations/Camille.h"
#include "Sequencer.h"
#include <vector>

int width = 16;
int height = 13;
uint32_t ledCount = width * height;
SK6812RGBW_Strip ledStrip(14, ledCount);

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

Vec2f middle(7.5, 6);
FrameBuffer circleAnimation;
FrameBuffer radiusAnimation;
FrameBuffer pointAnimation;
FrameBuffer randomCirclesAnimation;
FrameBuffer boxAnimation;

void setup() {
    Serial.begin(9600);

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
    ledStrip.setup();
    ledStrip.setPixelMatrix(ledMatrix);
    for(auto animation : animations) animation->configureMatrix(ledMatrix);
    sequencer.configureMatrix(ledMatrix);

    circleAnimation = ledStrip.getEmptyFramebuffer();
    radiusAnimation = ledStrip.getEmptyFramebuffer();
    pointAnimation = ledStrip.getEmptyFramebuffer();
    randomCirclesAnimation = ledStrip.getEmptyFramebuffer();
    boxAnimation = ledStrip.getEmptyFramebuffer();
}

float framesPerSecond = 100.0;
uint32_t frameInterval_micros = 1000000.0 / framesPerSecond;
uint32_t lastFrameTime_micros = 0;

void loop() {

    if(frameTimer.isTriggered()){
        sequencer.draw();
        ledStrip.addFrameBuffer(&sequencer);
        //pond.draw();
        //ledStrip.addFrameBuffer(&pond);
        ledStrip.display();
    }
    return;


    uint32_t now_micros = micros();
    if(now_micros - lastFrameTime_micros < frameInterval_micros) return;
    lastFrameTime_micros = now_micros;
    double now_seconds = float(now_micros) / 1000000.0;


    //concentric circles
    for(uint32_t i = 0; i < ledCount; i++){
        {
            float rad = sin(now_seconds * 0.5) * 5.0;
            Vec2f center(middle.x + sin(now_seconds) * rad, middle.y + cos(now_seconds) * rad);
            Vec2f pos = circleAnimation.pixels[i].pos;
            float dist = pos.dist(center);
            float step = 10.0;
            float b = map(fmod(-dist + now_seconds * 5 + sin(now_seconds) * 4.0, step), 0, step, 0.0, 1.0);
            b = map(triangle(b), 0.5, 1.0, 0.0, 1.0);
            //float noise = float(random(950, 1000)) / 1000.0;
            //bool b_blue = random(1000) > 998;
            float blue = map(b, 0.0, 0.2, 0.02, 0.0);
            blue = max(blue, 0.02);
            Color c(0.0, 0.0, blue, b);
            circleAnimation.setPixel(i,c);
        }
    }

    //radar radius
    for(uint32_t i = 0; i < ledCount; i++){
        Vec2f pos = radiusAnimation.pixels[i].pos;
        double t_rot = 10.0 + now_seconds * .5;
        Vec2f movPos(pos.x + sin(t_rot) * .5, pos.y + cos(t_rot) * .5);
        double ang = atan2(movPos.x - middle.x, movPos.y - middle.y);
        ang += t_rot;
        ang = fmod(ang, 2 * M_PI) - M_PI;
        double angNorm = map(ang, -M_PI, M_PI, 0.0, 1.0);
        double br = triangle(map(angNorm, .9, 1.0, 0.0, 1.0));
        double bll = map(br, 0.5, 1.0, 0.0, 1.0);
        Color c(0.0, 0.0, bll, br);
        radiusAnimation.setPixel(i, c);
    }
    radiusAnimation.drawCircle(middle, 2.0, 0.0, Color(0.0));

    //moving circle
    pointAnimation.clear(Color(0.0, 0.02));
    static Time::Timer circleTimer;
    static Vec2f circleTarget = middle;
    static Vec2f circleActual = middle;
    static float circleFilter = 0.025;
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
        Serial.printf("Time: %.1fs  Filter:%.3f\n", newTime, circleFilter);
        circleTimer.start(newTime);
    }
    circleActual.x = iirFilter(circleActual.x, circleTarget.x, circleFilter);
    circleActual.y = iirFilter(circleActual.y, circleTarget.y, circleFilter);
    for(int i = 0; i < 10; i++){
        if(random(100) > 90){
            float hue = fmod(float(random(50, 60)) * 0.01, 1.0);
            float brightness = random(80) * 0.01;
            pointAnimation.setPixel(random(ledCount), ColorHSV(hue, 0.8, brightness), ADD);
        }
    }
    //Vec2f pointPos(middle.x + sin(now_seconds*5) * 4, middle.y + cos(now_seconds*5) * 4);
    Vec2f pointPos = circleActual;
    pointAnimation.drawCircle(pointPos, 2.0, 3, Color(0.5, 0.5, 0.0, 1.0), ADD);
    //pointAnimation.drawCircle(pointPos, 1.0, 2.0, ColorHSV(0.7, 0.5, 1.0, 1.0), ADD);


    //Random Circles
    //randomCirclesAnimation.clear(Color(0.0, 0.05));
    randomCirclesAnimation.clear(Color(0.0, 0.1));
    if(randomF(1.0) > 0.97){
        float w = 10;
        float h = 7;
        Vec2f min(middle.x - w, middle.y - h);
        Vec2f max(middle.x + w, middle.y + h);
        Vec2f pos(randomF(min.x, max.x), randomF(min.y, max.y));
        float rad = randomF(2.0, 7.0);
        randomCirclesAnimation.drawCircle(pos, rad, rad * 0.5, ColorHSV(randomF(1.0), 0.2, 1.0, 1.0));
    }
    for(int i = 0; i < 208; i++){
        if(randomF(1000) > 970){
            randomCirclesAnimation.setPixel(i, ColorHSV(0.6, 1.0, .3));
        }
    }

    //nuit orage / 
    //jour cadran / DVD


    //DVD box
    boxAnimation.clear(Color(0.0, 0.1));
    static Vec2f boxPosition = middle;
    static Vec2f boxSize(4.5, 4.5);
    static Vec2f boxVel(0.1, 0.04);

    static Vec2f box2Pos(middle.x - 4.0, middle.y - 4.0);
    static Vec2f box2Vel = boxVel;
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
    boxAnimation.drawBox(boxPosition, boxSize, 2.0, boxColor);
    //boxAnimation.drawBox(box2Pos, boxSize, 2.0, Color(0.5, 0.0, 0.0, 0.25));
    boxAnimation.drawBox(box2Pos, boxSize, 2.0, Color(0.0, 0.0, 1.0));
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




    //ledStrip.addFrameBuffer(pointAnimation);
    //ledStrip.addFrameBuffer(circleAnimation);
    //ledStrip.addFrameBuffer(radiusAnimation);
    //ledStrip.addFrameBuffer(randomCirclesAnimation);
    ledStrip.addFrameBuffer(&boxAnimation);
    ledStrip.display();
}