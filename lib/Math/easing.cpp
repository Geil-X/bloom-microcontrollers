#include <math.h>

#include "easing.h"

#define PI 3.1415926545
#define PI_OVER_TWO 1.57079632679
#define E 2.71828182845f
#define EASING_BOUNCE 1.70158

float linearWindow(float t, float window_percentage) {
    if (t <= window_percentage) {
        return t / window_percentage;
    } else if (t >= 1 - window_percentage) {
        return 1 - ((t - 1 + window_percentage) / window_percentage);
    } else {
        return 1;
    }
}

float easeInSine(float t) {
    return (float) sin(PI_OVER_TWO * t);
}

float easeOutSine(float t) {
    return (float) (1 + sin(PI_OVER_TWO * (--t)));
}

float easeInOutSine(float t) {
    return (float) (0.5 * (1 + sin(PI * (t - 0.5))));
}

float easeInQuad(float t) {
    return t * t;
}

float easeOutQuad(float t) {
    return t * (2 - t);
}

float easeInOutQuad(float t) {
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

float easeInCubic(float t) {
    return t * t * t;
}

float easeOutCubic(float t) {
    return 1 + (--t) * t * t;
}

float easeInOutCubic(float t) {
    return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
}

float easeInQuart(float t) {
    t *= t;
    return t * t;
}

float easeOutQuart(float t) {
    t = (--t) * t;
    return 1 - t * t;
}

float easeInOutQuart(float t) {
    if (t < 0.5) {
        t *= t;
        return 8 * t * t;
    } else {
        t = (--t) * t;
        return 1 - 8 * t * t;
    }
}

float easeInQuint(float t) {
    float t2 = t * t;
    return t * t2 * t2;
}

float easeOutQuint(float t) {
    float t2 = (--t) * t;
    return 1 + t * t2 * t2;
}

float easeInOutQuint(float t) {
    float t2;
    if (t < 0.5) {
        t2 = t * t;
        return 16 * t * t2 * t2;
    } else {
        t2 = (--t) * t;
        return 1 + 16 * t * t2 * t2;
    }
}

float easeInExpo(float t) {
    return (float) ((pow(2, 8 * t) - 1) / 255);
}

float easeOutExpo(float t) {
    return (float) (1 - pow(2, -8 * t));
}

float easeInOutExpo(float t) {
    if (t < 0.5) {
        return (float) ((pow(2, 16 * t) - 1) / 510);
    } else {
        return (float) (1 - 0.5 * pow(2, -16 * (t - 0.5)));
    }
}

float easeInCirc(float t) {
    return (float) (1 - sqrt(1 - t));
}

float easeOutCirc(float t) {
    return (float) sqrt(t);
}

float easeInOutCirc(float t) {
    if (t < 0.5) {
        return (float) ((1 - sqrt(1 - 2 * t)) * 0.5);
    } else {
        return (float) ((1 + sqrt(2 * t - 1)) * 0.5);
    }
}

float easeInBack(float t) {
    return (float) (t * t * (E * t - EASING_BOUNCE));
}

float easeOutBack(float t) {
    return (float) (1 + (--t) * t * (E * t + EASING_BOUNCE));
}

float easeInOutBack(float t) {
    if (t < 0.5) {
        return (float) (t * t * (7 * t - 2.5) * 2);
    } else {
        return (float) (1 + (--t) * t * 2 * (7 * t + 2.5));
    }
}

float easeInElastic(float t) {
    float t2 = t * t;
    return (float) (t2 * t2 * sin(t * PI * 4.5));
}

float easeOutElastic(float t) {
    float t2 = (t - 1) * (t - 1);
    return (float) (1 - t2 * t2 * cos(t * PI * 4.5));
}

float easeInOutElastic(float t) {
    float t2;
    if (t < 0.45) {
        t2 = t * t;
        return (float) (8 * t2 * t2 * sin(t * PI * 9));
    } else if (t < 0.55) {
        return (float) ((0.5 + 0.75 * sin(t * PI * 4)));
    } else {
        t2 = (t - 1) * (t - 1);
        return (float) (1 - 8 * t2 * t2 * sin(t * PI * 9));
    }
}

float easeInBounce(float t) {
    return (float) (pow(2, 6 * (t - 1)) * fabs(sin(t * PI * 3.5)));
}

float easeOutBounce(float t) {
    return (float) (1 - pow(2, -6 * t) * fabs(cos(t * PI * 3.5)));
}

float easeInOutBounce(float t) {
    if (t < 0.5) {
        return (float) (8 * pow(2, 8 * (t - 1)) * fabs(sin(t * PI * 7)));
    } else {
        return (float) (1 - 8 * pow(2, -8 * t) * fabs(sin(t * PI * 7)));
    }
}