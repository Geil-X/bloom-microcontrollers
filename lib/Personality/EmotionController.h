#ifndef FLOWER_EMOTIONCONTROLLER_H
#define FLOWER_EMOTIONCONTROLLER_H

#include <Arduino.h>

#include <Personality.h>

class EmotionController {
public:
    explicit EmotionController(IEmotionState *initialEmotion) : currentEmotion(initialEmotion) {
        emotionStartTime = millis();
    }

    void initFlower(Flower &flower) {
        currentEmotion->initFlower(flower);
        emotionStartTime = millis();
    }

    void update(uint32_t millis, Flower &flower, CRGB leds[], uint8_t numLeds) {
        // There is no emotion to transition to
        if (!isTransitioning()) {
            currentEmotion->controlMotor(millis, flower);
            currentEmotion->controlLights(millis, flower, leds, numLeds);
        } // Emotions have fully transitioned
        else if (millis - transitionStartTime > transitionDuration && flower.isAtTarget()) {
            currentEmotion = nextEmotion;
            nextEmotion = nullptr;
            transitionStartTime = 0;
            transitionDuration = 0;
            emotionStartTime = millis;
            currentEmotion->initFlower(flower);
            currentEmotion->controlMotor(millis, flower);
            currentEmotion->controlLights(millis, flower, leds, numLeds);
        } // Emotions are currently in transition
        else {
            currentEmotion->controlMotor(millis, flower);
            currentEmotion->controlLights(millis, flower, leds, numLeds);
            nextEmotion->controlLights(millis, flower, transitionColors, numLeds);

            uint16_t transitionTime = constrain(millis - transitionStartTime, 0, transitionDuration);
            Serial.println("Transition Time: " + String(transitionTime));
            fract16 transitionPercentage = map16Max(transitionTime, transitionDuration, UINT16_MAX);
            Serial.println("Percentage 16 bit: " + String(transitionPercentage));
            fract8 transitionAmount = fract16ToFract8(transitionPercentage);
            Serial.println("Percentage 8 bit: " + String(transitionAmount));
            nblend(leds, transitionColors, numLeds, transitionAmount);
        }
    }

    void transitionTo(IEmotionState *emotion, uint32_t duration, uint32_t millis) {
        this->nextEmotion = emotion;
        transitionStartTime = millis;
        transitionDuration = duration;
    }

    [[nodiscard]] uint32_t getEmotionDuration(uint32_t millis) const {
        return millis - emotionStartTime;
    }

    [[nodiscard]] bool isTransitioning() const {
        return nextEmotion != nullptr;
    }

private:
    IEmotionState *currentEmotion = nullptr;
    IEmotionState *nextEmotion = nullptr;
    uint32_t emotionStartTime = 0;
    uint32_t transitionStartTime = 0;
    uint32_t transitionDuration = 0;

    // TODO: Make this not dependent on a magic number constant
    CRGB transitionColors[12]{};
};

#endif //FLOWER_EMOTIONCONTROLLER_H
