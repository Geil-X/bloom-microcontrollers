# Commands

* Add a new stepper motor to the teensy controller
    ```
    NEWSTEPPER <dir_pin> <step_pin> <sensor_pin>
    ```

* Start the flower group setup
    ```
    SETUP
    ```

* Open up the flower to a particular value within the range 0 to 100
    ```
    OPEN <value> ALL
    OPEN <value> <index,>
    ```
    * `index`: motor index, or a list of comma separated index

* Open a flower of a particular index
    ```
    OPENIND <value,> <index,>
    ```
    * `value`: position value, or a list of comma separated values
    * `index`: motor index, or a list of comma separated index

* Get the flower stats
    ```
    STAT
    ```

* Put the controller into debug mode
    ```
    DEBUG
    ```

* Soft stop the motors
  ```
  SOFTSTOP
  ```

* Force stop the motors
    ```
    FORCESTOP
    ```

* Reset the motors
    ```
    RESET
    ```

* Set the motors threshold
    ```
    THRESHOLD <id> <stall_threshold> <boundary_offset> <stall_detection_move_block>
    ```
    * `id`: motor id
    * `stall_threshold`: stall sensitivity
        * 0 -> 100
    * `boundary_offset`: boundary inset from homing extremes
        * 0 -> 200
    * `stall_detection_move_block`:
        * 0 -> 100

* Set the speed of all the motors in steps per second
  ```
  SPEED <speed>
  ```

* Set the acceleration of all the motors in steps per second squared
  ```
  ACCELERATION <acceleration>
  ```
  
* Ping the microcontroller to check its status
  ```
  PING
  ```

* Restart the microcontroller
  ```
  RESTART
  ```