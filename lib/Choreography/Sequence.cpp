#include "Sequence.h"

Sequence::Sequence(float startTime, float duration, float (*sequence)(float), SequenceType sequenceType) {
    this->sequence = sequence;
    this->startTime = startTime;
    this->duration = duration;
    this->sequenceType = sequenceType;
}

float Sequence::perform(float seconds) {
    return sequence((seconds - startTime) / duration);
}
