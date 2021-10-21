# I2C Command Data Packets

I2C Communication packet is 5 bytes long. There are 3 different types of commands that are sent. Each of which use different lengths of communications.

- Generic Command `1 Byte`
- Integer Command `3 Bytes`
- Float Command `5 Bytes`

The first byte is always used by the command ID but the following are used as the data bits for the integer which takes up 2 bytes or for the floating point numer which takes up 4 bytes.


| Type            | Byte 1     | Byte 2 | Byte 3 | Byte 4 | Byte 5 |
|-----------------|------------|--------|--------|--------|--------|
| Generic Command | Command ID |
| Integer Command | Command ID | Integer Byte 1 | Integer Byte 2
| Float Command   | Command ID | Float Byte 1 | Float Byte 2 | Float Byte 3 | Float Byte 4

## Command Breakdown

| Command         | Type    | Id                  | Value
|-----------------|---------| --------------------|---
| No Command      | Generic | NO_COMMAND = 0      |
| Setup           | Generic | SETUP = 1           |
| Home            | Generic | HOME = 2            |
| Open            | Generic | OPEN = 3            |
| Close           | Generic | CLOSE = 4           |
| Open To         | Float   | OPEN_TO  = 5        | Float Percentage out of 100
| Speed           | Integer | SPEED = 6           | New Speed (microsteps/s)
| Acceleration    | Integer | ACCELERATION = 7    | New Acceleration (microsteps/s^2)
| Ping            | Integer | PING = 8            |
| Invalid Command | Generic | INVALID_COMMAND = 9 |

# I2C Reserved Addresses

I2C by default uses 7-bit encoding, giving a total of 128 possible addresses in this mode. Not all of those addresses are available to use on the I2C bus. 16 of those addresses, `0x00` - `0x07` and `0x78` - `0x7F` are reserved I2C addresses. They are used for the following purposes.

* `0x00` - Reserved - General Call Address
* `0x01` - Reserved for CBUS Compatibility
* `0x02` - Reserved for I2C-compatible Bus Variants
* `0x03` - Reserved for Future Use
* `0x04`, `0x05`, `0x06`, `0x07` - Reserved for Hs-mode Master