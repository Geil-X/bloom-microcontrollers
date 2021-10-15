#include "Choreography.h"

template<size_t MAX_SIZE>
void Choreography<MAX_SIZE>::addSequence(
        float sequenceDuration, float (*sequence)(float), SequenceType sequenceType) {
    // Cannot add more sequences than specified
    if (sequenceCount >= MAX_SIZE) return;

    sequences[sequenceCount] =
            Sequence(duration, sequenceDuration, sequence, sequenceType);

    sequenceCount++;
    duration += sequenceDuration;
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
float Choreography<MAX_SIZE>::perform(float seconds) {
    float periodTime = fmod(seconds, duration);
    for (int sequenceIndex = sequenceCount - 1; sequenceIndex >= 0; sequenceIndex--) {
        float startTime = sequences[sequenceIndex].getStartTime();
        if (seconds > startTime) {
            float sequenceTime = periodTime - startTime;
            sequences[sequenceIndex].perform(sequenceTime);
        }
    }

    return 0;
}