
#include <Arduino.h>
#include <CmdMessenger.h>

#include "SevenSegDisplay.h"

void OnUnknownCommand();
void OnSetCount();
void OnSetElement();
void OnClear();

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

uint8_t g_digit[DIGIT_COUNT] = {0};

//GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
uint8_t g_segmentClock = 6;
uint8_t g_segmentLatch = 5;
uint8_t g_segmentData = 7;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger g_cmdMessenger = CmdMessenger(Serial);

enum
{
  kAcknowledge, // 0
  kError,       // 1
  kSetCount,    // 2
  kSetElement,  // 3
  kClear,       // 4
  kDebug        // 5
};

// Called when a received command has no attached function
void OnUnknownCommand()
{
  g_cmdMessenger.sendCmd(kError, "Unknown Command");
}

void OnSetCount()
{
  uint16_t number = g_cmdMessenger.readInt16Arg();
  uint8_t size = numDigits(number);

  char string[DIGIT_COUNT + 1] = {0};
  snprintf(string, DIGIT_COUNT + 1, "%d", number);

  clear();

  g_cmdMessenger.sendCmd(kDebug, string);

  for (uint8_t d = 0; d < DIGIT_COUNT; d++)
  {
    char c = (d < size) ? string[min(size, DIGIT_COUNT) - d - 1] : '\0';
    g_digit[d] = setNumber(g_digit[d], c);
    g_cmdMessenger.sendCmd(kDebug, c);
  }

  g_cmdMessenger.sendCmd(kAcknowledge, "SetCount Done");
}

void OnSetElement()
{
  uint16_t digit = g_cmdMessenger.readInt16Arg();
  uint16_t segment = g_cmdMessenger.readInt16Arg();
  bool on = g_cmdMessenger.readBoolArg();

  if ((digit < DIGIT_COUNT) && (segment < SEGMENT_COUNT))
  {
    on ? g_digit[digit] = setSegment(g_digit[digit], (SEGMENT)segment)
       : g_digit[digit] = clearSegment(g_digit[digit], (SEGMENT)segment);
  }

  g_cmdMessenger.sendCmd(kAcknowledge, "SetElement Done");
}

void OnClear()
{
  clear();

  g_cmdMessenger.sendCmd(kAcknowledge, "Clear Done");
}

void setup()
{
  Serial.begin(9600);

  // Attach callback methods
  g_cmdMessenger.attach(OnUnknownCommand);
  g_cmdMessenger.attach(kSetCount, OnSetCount);
  g_cmdMessenger.attach(kSetElement, OnSetElement);
  g_cmdMessenger.attach(kClear, OnClear);

  pinMode(g_segmentClock, OUTPUT);
  pinMode(g_segmentData, OUTPUT);
  pinMode(g_segmentLatch, OUTPUT);

  digitalWrite(g_segmentClock, LOW);
  digitalWrite(g_segmentData, LOW);
  digitalWrite(g_segmentLatch, LOW);

  test();

  g_cmdMessenger.sendCmd(kAcknowledge, "Sign Started!");
}

void loop()
{
  // Process incoming serial data, and perform callbacks
  g_cmdMessenger.feedinSerialData();

  // write data to the display
  updateDisplay();
}

void clear()
{
  for (uint8_t d = 0; d < DIGIT_COUNT; d++)
  {
    g_digit[d] = 0;
  }
}

void test()
{
  clear();
  updateDisplay();

  SEGMENT segment[] = {DP, F, A, B, C, D, E, G};

  // cycle through each segment
  for (uint16_t x = 0; x < DIGIT_COUNT; x++)
  {
    for (uint16_t s = 0; s < SEGMENT_COUNT; s++)
    {
      clear();
      g_digit[x] = setSegment(g_digit[x], segment[s]);
      updateDisplay();
      delay(100);
    }
  }

  clear();
  updateDisplay();
}

void updateDisplay()
{
  for (uint8_t d = 0; d < DIGIT_COUNT; d++)
  {
    // Clock these bits out to the drivers
    for (uint8_t s = 0; s < 8; s++)
    {
      digitalWrite(g_segmentClock, LOW);
      digitalWrite(g_segmentData, g_digit[d] & 1 << (7 - s));
      digitalWrite(g_segmentClock, HIGH); // Data transfers to the register on the rising edge of SRCK
    }
  }

  // Latch the current segment data
  digitalWrite(g_segmentLatch, LOW);
  digitalWrite(g_segmentLatch, HIGH); // Register moves storage register on the rising edge of RCK
}
