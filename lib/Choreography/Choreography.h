#ifndef FLOWER_CHOREOGRAPHY_H
#define FLOWER_CHOREOGRAPHY_H

#include <Arduino.h>
#include <Sequence.h>

template<size_t MAX_SIZE>
class Choreography {
public:
    /**
     * Add a sequence to the choreography. They are added in sequential order.
     *
     * @param duration The duration of the sequence that is going to be performed.
     * @param sequence
     *     The function returning the sequence segment.
     *     The function should have a domain and range both from 0 -> 1 inclusive.
     * @param sequenceType
     *     The type of modification
     */
    void addSequence(float duration, float (*sequence)(float), SequenceType sequenceType = SEQUENCE_DEFAULT);

    /**
     * Perform the choreography and get the position of the choreography at
     * the current time step. The choreography loops so that if you give it a
     * time that is longer than the duration, then it will start back at the
     * beginning again.
     *
     * @param seconds The current running time of the choreography or device.
     * @return The location of the sequence in the range 0 -> 1.
     */
    float perform(float seconds);

private:
    int sequenceCount = 0;
    float duration = 0;
    Sequence sequences[MAX_SIZE];
};

#endif //FLOWER_CHOREOGRAPHY_H
