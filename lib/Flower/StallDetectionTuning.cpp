#include "StallDetectionTuning.h"

const int STALL_TABLE[NUM_STALL_VALUES][3] = {
// Speed | Stall Guard Threshold | Maximum Stall Detection Threshold
        {500, 15, 950},
        {750, 16, 950},
        {800, 16, 950},
        {1000, 17, 950},
        {1100, 18, 950},
        {1750, 19, 950},
        {1850, 20, 950},
        {2000, 21, 950},
        {2500, 23, 950},
        {3000, 25, 950},
        {3500, 27, 800},
        {4000, 28, 800},
        {5000, 34, 800},
        {6000, 41, 800},
        {7000, 46, 800},
        {8000, 56, 800},
        {9000, 62, 800},
        {10000, 63, 600},
        {11000, 63, 500},
        {12000, 63, 400},
        {13000, 63, 300},
        {15000, 63, 200}
};
