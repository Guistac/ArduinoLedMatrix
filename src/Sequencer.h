#pragma once
#include "Animation.h"
#include <vector>

class Sequence{
public:

    Sequence(Animation* anim, float durationSeconds, float fadeInSeconds){
        animation = anim;
        durationMillis = durationSeconds * 1000;
        fadeInMillis = fadeInSeconds * 1000;
    }

    Animation* animation = nullptr;
    uint32_t startTimeMillis = 0;
    uint32_t fadeInMillis = 1.0;
    uint32_t durationMillis = 10.0;
};



class Sequencer : public Animation{
public:

    Sequencer(std::vector<Sequence> seqs){
        sequences = seqs;
        sequenceIndex = 0;
    }

    virtual void draw() override{
        uint32_t nowMillis = millis();

        Sequence* currentSequence = getCurrentSequence();
        Sequence* nextSequence = getNextSequence();
        if(nowMillis - currentSequence->startTimeMillis >= currentSequence->durationMillis - nextSequence->fadeInMillis){
            sequenceIndex++;
            if(sequenceIndex >= sequences.size()) sequenceIndex = 0;
            Serial.printf("NEXT SEQUENCE %i\n", sequenceIndex);
            currentSequence = getCurrentSequence();
            currentSequence->startTimeMillis = nowMillis;
            nextSequence = getNextSequence();
        }

        if(nowMillis - currentSequence->startTimeMillis < currentSequence->fadeInMillis){
            float fade = float(nowMillis - currentSequence->startTimeMillis) / float(currentSequence->fadeInMillis);
            //Serial.printf("Fading in %i (%.1f%%)\n", sequenceIndex, fade * 100.0);
            Sequence* previousSequence = getPreviousSequence();
            previousSequence->animation->draw();
            currentSequence->animation->draw();
            clear();
            addFrameBuffer(previousSequence->animation);
            addFrameBuffer(currentSequence->animation, fade);
        }
        else if(nowMillis - currentSequence->startTimeMillis < currentSequence->durationMillis - nextSequence->fadeInMillis){
            //Serial.printf("Full animation %i\n", sequenceIndex);
            currentSequence->animation->draw();
            addFrameBuffer(currentSequence->animation);
        }
    }

    Sequence* getPreviousSequence(){
        if(sequenceIndex == 0) return &sequences[sequences.size() - 1];
        return &sequences[sequenceIndex - 1];
    }
    Sequence* getCurrentSequence(){ return &sequences[sequenceIndex]; }
    Sequence* getNextSequence(){
        if(sequenceIndex == sequences.size() - 1) return &sequences[0];
        return &sequences[sequenceIndex + 1];
    }

    std::vector<Sequence> sequences = {};
    uint32_t sequenceIndex;

};