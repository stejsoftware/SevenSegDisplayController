# Seven Segment Display Controller [![Build Status](https://travis-ci.org/stejsoftware/SevenSegDisplayController.svg?branch=master)](https://travis-ci.org/stejsoftware/SevenSegDisplayController)

A simple firmware to control any number of [7-Segment Displays](https://www.sparkfun.com/products/8530) with the [Large Digit Driver](https://www.sparkfun.com/products/13279).

## Config / Connection

The number of digits can be configured with a `#define` in the file [SevenSegDisplayController.cpp](src\SevenSegDisplayController.cpp) on line 42.

```C++
#define DIGIT_COUNT 4
```

Here's how to hook up the Arduino pins to the Large Digit Driver.

### Data

|  Arduino | 7 Seg Display |
| :------: | ------------- |
| 6        | CLK (Green on the 6-pin cable) |
| 5        | LAT (Blue)    |
| 7        | SER on the IN side (Yellow) |
| 5V       | 5V (Orange)   |
| GND      | GND (Black)   |

### Power

| Power | Arduino      | 7 Seg Display |
| :---: | ------------ | ------------- |
| 12v   | raw (barrel) | 12V (Red)     |

There are two connectors on the Large Digit Driver. 'IN' is the input side that should be connected to your microcontroller (the Arduino). 'OUT' is the output side that should be connected to the 'IN' of addtional digits.

Each display will use about 150mA with all segments and decimal point on.

## Build

The easiest way is to build with [PlatformIO](http://docs.platformio.org/en/latest/quickstart.html#process-project). PlatformIO will download all of the required libraries for this project and the target you select. 

This progject was written with [Visual Studio Code](https://code.visualstudio.com/download) and [PlatformIO](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

## Usage

The display is controlled via the serial conneciton on the Arduino at 9600 baud.

| Command    | #     | Args           | Example    | |
| --------   | :---: | -----          | -------    | ---- 
| SetCount   | 2     | number         | `2,1234;`  | Shows the give number on the display 
| SetElement | 3     | digit, seg, on | `3,0,A,1;` | Sets the state of a single stegment on a single digit (0=off; 1=on) 
| Clear      | 4     |                | `4;`       | Turns off all segments of the display 
