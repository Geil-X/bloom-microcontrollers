#ifndef FLOWER_SEQUENCE_H
#define FLOWER_SEQUENCE_H


enum SequenceType {
    SEQUENCE_DEFAULT,
    SEQUENCE_LINEAR,
    SEQUENCE_LINEAR_DECREASE
};


class Sequence {
public:
    /**
     *
     * @param sequence
     * @param duration
     */
    Sequence(float startTime, float duration, float (*sequence)(float), SequenceType sequenceType = SEQUENCE_DEFAULT);

    /**
     *
     * @param seconds
     * @return
     */
    float perform(float seconds);

    float getStartTime() { return startTime; }

    float getDuration() { return duration; }

private:
    /**
     * Input range 0 -> 1 and output range 0 -> 1
     * @return
     */
    float (*sequence)(float);

    float startTime;

    /** Duration of the sequence in seconds */
    float duration;

    /**
     *
     */
    SequenceType sequenceType;
};


#endif //FLOWER_SEQUENCE_H
