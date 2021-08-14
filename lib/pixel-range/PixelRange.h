#pragma once
#include <Arduino.h>

class PixelRange
{
public:
  PixelRange() {}
  PixelRange(int rows, int startingIndex, int endingIndex);

  void setReverseIteration(bool reverse);
  uint8_t getNextIndex();
  uint8_t getCurrentIndex();

  int getRowCount();
  int getStart();
  int getEnd();

private:
  bool _iterateInReverse = false;
  uint8_t _iteratorIndex = 0;
  int _rowCount;
  int _start;
  int _end;

  uint8_t forwardIteration();
  uint8_t reverseIteration();
};