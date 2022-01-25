# Setup

----

* Install the [Platform IO](https://platformio.org/install/cli) command line interface
* If using the CLion development IDE you can install
  the [Platform IO Plugin](https://plugins.jetbrains.com/plugin/13922-platformio-for-clion).
    * You can learn more about CLion and Platform IO through
      this [tutorial](https://blog.jetbrains.com/clion/2020/08/arduino-from-hobby-to-prof-p1/)
    * Set up the CLion configurations to compile the code using this [tutorial](https://docs.platformio.org/enable/latest/integration/ide/clion.html)
* If you are using windows, install the C++ compiler [MinGW Gcc/G++](https://sourceforge.net/projects/mingw/files/latest/download)
    * You can follow this [guide](https://www.ics.uci.edu/~pattis/common/handouts/mingweclipse/mingw.html) to complete
      this installation
    * Be sure to check __mingw32-base__, __mingw32-gcc-g++__, and __msys-base__
    * Add these to your system `PATH` variable:
        * `C:\MinGW\bin`
        * `C:\MinGW\msys\1.0\bin`
* If you are using teensy boards, you need to install the [teensy loader application](https://www.pjrc.com/teensy/loader.html)

# Development

----

## Platformio Configuration `platformio.ini`

* Information about the `platform.ini` file can be found at the [project configuration file page](https://docs.platformio.org/en/latest/projectconf/index.html).
* For the full listing of all the development board configuration for platformio you can check the [boards listing page](https://docs.platformio.org/en/latest/boards/).

### [Arduino Uno](https://docs.platformio.org/en/latest/boards/atmelavr/uno.html)

```pio
[env:uno]
platform = atmelavr
board = uno
```

### [ATmega328P](https://docs.platformio.org/en/latest/boards/atmelavr/ATmega328P.html)

#### With 16 MHz External Clock

```pio
platform = atmelavr
framework = arduino
board = ATmega328P
board_build.mcu = atmega328p
board_build.f_cpu = 16000000L
upload_speed = 115200
monitor_speed = 115200
```

#### With 8 Mhz Internal Clock

```pio
platform = atmelavr
framework = arduino
board = 328p8m
board_build.mcu = atmega328p
board_build.f_cpu 	= 8000000L
upload_speed 		= 57600
monitor_speed 		= 57600
```


### [Arduino Atmega 2560](https://docs.platformio.org/en/latest/boards/atmelavr/megaatmega2560.html)

```pio
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
```

### [Teensy 3.1 / 3.2](https://docs.platformio.org/en/latest/boards/teensy/teensy31.html)

```pio
[env:teensy31]
platform = teensy
board = teensy31
```

### [Teensy 3.6](https://docs.platformio.org/en/latest/boards/teensy/teensy36.html)

```pio
platform = teensy
board = teensy36
```

### [Node MCU](https://docs.platformio.org/en/latest/boards/espressif8266/nodemcuv2.html)

```pio
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
```