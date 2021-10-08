# I2C Reserved Addresses

I2C by default uses 7-bit encoding, giving a total of 128 possible addresses in this mode. Not all of those addresses are available to use on the I2C bus. 16 of those addresses, `0x00` - `0x07` and `0x78` - `0x7F` are reserved I2C addresses. They are used for the following purposes.

* `0x00` - Reserved - General Call Address
* `0x01` - Reserved for CBUS Compatibility
* `0x02` - Reserved for I2C-compatible Bus Variants
* `0x03` - Reserved for Future Use
* `0x04`, `0x05`, `0x06`, `0x07` - Reserved for Hs-mode Master