#ifndef FLOWER_VOLTAGESENSOR_H
#define FLOWER_VOLTAGESENSOR_H

#include <Arduino.h>
#include <RunningMedian.h>

class VoltageSensor {
public:
    /**
     * Create a voltage sensor. The voltage sensor is simply a voltage divider
     * created by two resistors. The maximum voltage that is safe for the
     * arduino to read is 5V * (r1 + r2) / r1.
     *
     * @note Make sure that the voltage source you are reading shares a common
     *       ground with the arduino, otherwise you could cause a short and
     *       damage the arduino circuit.
     *
     * @param pin The analog read pin used for reading in the voltage.
     * @param r1 The value of the resistor in kilohms between the voltage source and the
     *           analog read pin. This resistor value should be significantly
     *           larger than the r2 resistor.
     * @param r2 The value of the resistor in kilohms between the analog read
     *           pin and the ground.
     */
    VoltageSensor(uint8_t pin, int r1, int r2);

    /**
     * Read the voltage connected to the voltage sensor. The precision of the
     * voltage reading is determined by the reduction of the voltage divider.
     * In other words, it is determined by the maximum voltage and the 10 bit
     * precision of the analog input. The precision is then Vmax / 2^10. The
     * value of the reading is averaged out over several readings to give a
     * more consistent reading of the sensor. This will affect the readings if
     * this function is not sampled fast enough.
     *
     * @note This value may not be completely accurate if not calibrated. This
     *       could be due to several factors, with the biggest being the
     *       precision of the resistors. Common resistors have a tolerance of
     *       5%.
     *
     * @return The input voltage.
     */
    float read();

private:
    uint8_t pin;
    float max_voltage;
    RunningMedian voltage = RunningMedian(19);
};

#endif //FLOWER_VOLTAGESENSOR_H