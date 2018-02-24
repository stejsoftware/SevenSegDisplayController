
#include <Arduino.h>
#include <CmdMessenger.h>

#include "SevenSegDisplay.h"

void OnUnknownCommand();
void OnSetCount();
void OnSetElement();
void OnClear();
void OnTest();

void showNumber(int16_t value);
void clear();
void test();

void updateDisplay();

/*
 Controlling large 7-segment displays
 By: Nathan Seidle
 SparkFun Electronics
 Date: February 25th, 2015
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 The large 7 segment displays can be controlled easily with a TPIC6C594 IC. This code demonstrates how to control
 one display.

 Here's how to hook up the Arduino pins to the Large Digit Driver

 Arduino pin 6 -> CLK (Green on the 6-pin cable)
 5 -> LAT (Blue)
 7 -> SER on the IN side (Yellow)
 5V -> 5V (Orange)
 Power Arduino with 12V and connect to Vin -> 12V (Red)
 GND -> GND (Black)

 There are two connectors on the Large Digit Driver. 'IN' is the input side that should be connected to
 your microcontroller (the Arduino). 'OUT' is the output side that should be connected to the 'IN' of addtional
 digits.

 Each display will use about 150mA with all segments and decimal point on.
*/

#define DIGIT_COUNT 4
#define SEGMENT_COUNT 8

uint8_t digit[DIGIT_COUNT] = {0};

//GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
uint8_t segmentClock = 6;
uint8_t segmentLatch = 5;
uint8_t segmentData = 7;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
  kAcknowledge, // 0
  kError,       // 1
  kSetCount,    // 2
  kSetElement,  // 3
  kClear,       // 4
  kTest         // 5
};

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError, "Unknown Command");
}

void OnSetCount()
{
  showNumber(cmdMessenger.readInt32Arg());
  cmdMessenger.sendCmd(kAcknowledge, "SetCount Done");
}

void OnSetElement()
{
  uint16_t digit = cmdMessenger.readInt16Arg();
  uint16_t segment = cmdMessenger.readInt16Arg();
  bool on = cmdMessenger.readBoolArg();

  if ((digit < DIGIT_COUNT) && (segment < SEGMENT_COUNT))
  {
  }

  cmdMessenger.sendCmd(kAcknowledge, "SetElement Done");
}

void OnClear()
{
  clear();
  cmdMessenger.sendCmd(kAcknowledge, "Clear Done");
}

void OnTest()
{
  test();
  cmdMessenger.sendCmd(kAcknowledge, "Test Done");
}

void setup()
{
  Serial.begin(9600);

  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kSetCount, OnSetCount);
  cmdMessenger.attach(kSetElement, OnSetElement);
  cmdMessenger.attach(kClear, OnClear);
  cmdMessenger.attach(kTest, OnTest);

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  test();

  cmdMessenger.sendCmd(kAcknowledge, "Sign Started!");
}

void loop()
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();

  // write data to the display
  updateDisplay();
}

void showNumber(int16_t number)
{
  char string[DIGIT_COUNT + 2];
  sprintf(string, "%d", number);

  clear();

  for (uint8_t d = 0; d < DIGIT_COUNT; d++)
  {
    digit[d] = setNumber(digit[d], string[d]);
  }
}

void clear()
{
  for (uint8_t d = 0; d < DIGIT_COUNT; d++)
  {
    digit[d] = 0;
  }
}

void test()
{
}

void updateDisplay()
{
  for (uint8_t d = 0; d < DIGIT_COUNT; d++)
  {
    // Clock these bits out to the drivers
    for (uint8_t s = 0; s < 8; s++)
    {
      digitalWrite(segmentClock, LOW);
      digitalWrite(segmentData, digit[d] & 1 << (7 - s));
      digitalWrite(segmentClock, HIGH); // Data transfers to the register on the rising edge of SRCK
    }

    // Latch the current segment data
    digitalWrite(segmentLatch, LOW);
    digitalWrite(segmentLatch, HIGH); // Register moves storage register on the rising edge of RCK
  }
}
