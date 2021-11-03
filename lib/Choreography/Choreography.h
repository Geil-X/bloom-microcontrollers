#ifndef FLOWER_CHOREOGRAPHY_H
#define FLOWER_CHOREOGRAPHY_H

#include <Arduino.h>
#include <MathExtra.h>


struct Sequence {
public:
    Sequence() {
        this->run = zero;
        this->start_time = 0;
        this->duration = 0;
    }

    /**
     *
     * @param startTime
     * @param duration
     * @param run  function (float time, float duration, float x, float y) returns float position [0 -> 1]
     */
    Sequence(float startTime, float duration, float (*run)(float, int, float, float, float)) {
        this->run = run;
        this->start_time = startTime;
        this->duration = duration;
    }

    /**
     * Input range 0 -> 1 and output range 0 -> 1.
     * @param t The time in seconds
     * @param iteration The current iteration of the sequence
     * @param duration The duration of the sequence
     * @param x The x-position of the device
     * @param y The y-position of the device
     * @return
     */
    float (*run)(float, int, float, float, float);

    /** The time that this particular sequence starts running in seconds. */
    float start_time;

    /** The duration of the sequence in seconds. */
    float duration;


private:
    static float zero(float t, int i, float d, float x, float y) { return 0; }
};

template<size_t MAX_SIZE>
class Choreography {
public:
    Choreography();

    /**
     * Add a sequence to the choreography. They are added in sequential order.
     * If you try to add more sequences than specified by the type declaration,
     * the sequence won't be added.
     *
     * @example This function can be chained together for increased readability.
     * @code
     *    Choreography<2> choreography = Choreography()
     *        .addSequence(5, identity)
     *        .addSequence(5, inverse);
     *
     * @param duration The duration of the sequence that is going to be performed.
     * @param sequence
     *     The function returning the sequence segment.
     *     The function should have a domain and range both from 0 -> 1 inclusive.
     * @param sequenceType The type of modification
     */
    Choreography addSequence(float duration, float (*sequence)(float, int, float, float, float));

    /**
     * Perform the choreography and get the position of the choreography at
     * the current time step. The choreography loops so that if you give it a
     * time that is longer than the duration, then it will start back at the
     * beginning again.
     *
     * @param milliseconds The current running time of the choreography or device.
     * @return The location of the sequence in the range 0 -> 1.
     */
    float perform(unsigned long milliseconds, float x = 0, float y = 0);

private:
    int seed;
    int iteration;
    unsigned int sequenceCount = 0;
    float duration = 0;
    Sequence sequences[MAX_SIZE] = {};
};

template<size_t MAX_SIZE>
Choreography<MAX_SIZE>::Choreography()  {
    iteration = -1;
    seed = 0;
}

template<size_t MAX_SIZE>
Choreography<MAX_SIZE>
Choreography<MAX_SIZE>::addSequence(float sequenceDuration, float (*sequence)(float, int, float, float, float)) {
    // Cannot add more sequences than specified
    if (sequenceCount >= MAX_SIZE) return (*this);

    sequences[sequenceCount] = Sequence(duration, sequenceDuration, sequence);
    sequenceCount++;
    duration += sequenceDuration;

    return (*this);
}

/**
 * Look for the current sequence that is running then perform that sequence after
 * adjusting it into the local window of that sequence.
 *
 *
 * Global Choreography Time:
 *
 *     Sequence Start    Current Time
 *                  |    |
 *     0            v    v                  Choreography End
 *     +------+-----+------------------+----+
 *     |      |     | Current Sequence |    |
 *     +------+-----+------------------+----+
 *
 *
 * Local Sequence Time:
 *
 *           Current Time
 *           |
 *     0     v            Sequence End
 *     +------------------+
 *     | Current Sequence |
 *     +------------------+
 */
template<size_t MAX_SIZE>
float Choreography<MAX_SIZE>::perform(unsigned long milliseconds, float x, float y) {
    // Seconds is constrained to the max duration
    int current_iteration = milliseconds / (unsigned long) (duration * 1000);
    float seconds = (milliseconds % ((unsigned long) (duration * 1000))) / 1000.f;

    if (iteration != current_iteration) {
        seed = (int) random(0, 10000);
        iteration = current_iteration;
    }

    for (int sequenceIndex = sequenceCount - 1; sequenceIndex >= 0; sequenceIndex--) {
        Sequence sequence = sequences[sequenceIndex];
        if (seconds > sequence.start_time) {
            float sequenceTime = (seconds - sequence.start_time) / sequence.duration;
            float position = sequences[sequenceIndex].run(sequenceTime, seed, sequence.duration, x, y);
            return constrain(position, 0, 1);
        }
    }

    return 0;
}

#endif //FLOWER_CHOREOGRAPHY_H
