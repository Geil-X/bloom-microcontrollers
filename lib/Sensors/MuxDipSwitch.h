#ifndef FLOWER_MUXDIPSWITCH_H
#define FLOWER_MUXDIPSWITCH_H

#include <Arduino.h>

/**
 * Class for reading a 7 bit dit switch though a mux that has 3 select pins.
 */
class MuxDipSwitch {
public:

    /**
     * @param output Output pin from the mux used for reading the dip switch value
     * @param select1 First bit/pin of the mux selection
     * @param select2 Second bit/pin of the mux selection
     * @param select3 Third bit/pin of the mux selection
     */
    MuxDipSwitch(uint8_t output, uint8_t select1, uint8_t select2, uint8_t select3);

    uint8_t read();

private:
    uint8_t output;
    uint8_t select1;
    uint8_t select2;
    uint8_t select3;
};


#endif //FLOWER_MUXDIPSWITCH_H
