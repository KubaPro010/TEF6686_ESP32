# PC Control Interface

The PC Control Interface of the firmware allows the PC to do useful things with the radio directly.

## Invoking

Any time, ever, the PC ("Serial device") can send a "~/" on the data line, once the radio is ready for command (or already is), a response will get received: [0x01 0xFF].

That means, we can send commands to the radio now.

## Packet

Every single packet starts with its length (MSB of length is reserved for optional CRC8 at end of packet, see [CRC](#crc)), example: [0x02 0xAA 0x55] [0x03 0x00 0x01 0x02]

The byte after the lenght is expected to be the command, which will be defined below

### Commands

#### 0 - Set clock

This sets the I2C clock.

4 bytes after command should represent the clock to set as commanded.

##### Example

Set clock to 400 kHz

[0x05] - Length

[0x00] - Command

[0x00] - MSB Byte of clock value

[0x06]

[0x1A]

[0x80] - LSB Byte of the clock value

##### Response

[0x01] - Length

[0x00] - Response to command 0

#### 1 - Send I2C data

This command sends out the provided data to a provided address over I2C

First byte after the command should be the address, and the rest until the end of the command shall be the data itself

##### Structure

[0x03] - Length

[0x01] - Command

[X] - Device address

[X] - Address to set on the device (data)

[X] - Address contents on the device (data)

##### Response

[0x02] - Length

[0x01] - Response to command 0

[X] - Arbitrary status value which matches the value of Arduino's endTransmission function

#### 2 - Send + Receive I2C data

This command sends out the provided data to a provided address over I2C, but then sends a repeated start and requests a number of bytes which is provided by the command

First byte after the command should be the address, the following byte should be the lenght of the data to send before reading, the message that is to be send should be sent now, and the last byte should be the number of bytes to receive from the device

##### Example

[0x03] - Length

[0x02] - Command

[X] - Device address

[X] - Length of data to send

[X] - Actual data

[n] - Request two bytes from the device

##### Response

[0x02+n] - Length

[0x02] - Response to command 2

[X] - Arbitrary status value which matches the value of Arduino's endTransmission function

n*[X] - Data received from the device

#### 3 - Quit

This commands quits the control interface.

It will also reset the baud rate to 115200

##### Example

[0x01] - Length

[0x03] - Command

##### Response

[0x01] - Length

[0x03] - Response to command 3

#### 4 - Protocol version

Returns the current protocol version

##### Example

[0x01] - Length

[0x04] - Command

##### Response

[0x02] - Length

[0x04] - Response to command 4

[0x03] - Current protocol version

#### 5 - Reboot

This command will reset the ESP32

##### Example

[0x01] - Length

[0x05] - Command

##### Response

[0x01] - Length

[0x05] - Response to command 5

#### 6 - Change baud rate

This will change the baud rate. Response of this command is still in the prev baud rate.

##### Example

Set baud rate to 921600

[0x05] - Length

[0x06] - Command

[0x00] - MSB byte of baud rate

[0x0E]

[0x10]

[0x00] - LSB byte of baud rate

##### Response

[0x01] - Length

[0x06] - Response to command 6

#### 7 - Write to EEPROM

This command will write to the NVS of the ESP32

This command is only available on versions 2+

##### Example

Store 0xff at address 0x10

[0x04] - Length

[0x07] - Command

[0x00] - MSB byte of address

[0x10] - LSB byte of address

[0xff]

Store 0xffaa at address 0x5555

[0x04] - Length

[0x07] - Command

[0x55] - MSB byte of address

[0x55] - LSB byte of address

[0xff]

[0xaa]

##### Response

[0x01] - Length

[0x07] - Response to command 7

#### 8 - Read from EEPROM

This command will read from the NVS of the ESP32

This command is only available on versions 2+

##### Structure

[0x04] - Length

[0x08] - Command

[X] - MSB byte of address

[X] - LSB byte of address

[n] - Length to read

##### Response

[0x01 + n] - Length

[0x08] - Response to command 8

n*[X] - Data read from EEPROM

#### 253 - Get user data for control mode

This command is only available on versions 2+

This commands gets the length and address of the user data for control mode

##### Structure

[0x01] - Length

[0xFD] - Command

##### Response

[0x05] - Length

[0xFD] - Response to command 253

[X] - MSB byte of address

[X] - LSB byte of address

[X] - MSB byte of size

[X] - LSB byte of size

#### 254 - Get persistence address

This command is only available on versions 2+

If an non zero was written to the persistance address in the EEPROM, after every reboot the radio would boot up into the control mode without the full init sequence (Holding mode while booting will override this)

##### Structure

[0x01] - Length

[0xFE] - Command

##### Response

[0x03] - Length

[0xFE] - Response to command 254

[X] - MSB byte of persistence address

[X] - LSB byte of persistence address

#### 255 - Ping / Wake

This command is only available on versions 2+

##### Structure

[0x01] - Length

[0xFF] - Command

##### Response

[0x01] - Length

[0xFF] - Response to command 255

### CRC

First bit of the length from version 3 control whether there is an crc byte at the end of the command, it is not included in the length. If the request was sent with CRC, response also will be too. In case of a CRC mismatch the radio will send back [0x02 0xFF 0x01]. CRC is calculated with the lenght included