#include "PixelRange.h"

PixelRange::PixelRange(int rows, int startingIndex, int endingIndex)
{
  _rowCount = rows;
  _start = startingIndex;
  _end = endingIndex;
};

int PixelRange::getrRowCount()
{
  return _rowCount;
}
int PixelRange::getStart()
{
  return _start;
}
int PixelRange::getEnd()
{
  return _end;
}

void PixelRange::setReverseIteration(bool reverse)
{
  _iterateInReverse = reverse;
}

uint8_t PixelRange::getCurrentIndex()
{
  return _iteratorIndex;
}

uint8_t PixelRange::getNextIndex()
{
  if (_iterateInReverse == false)
  {
    return forwardIteration();
  }
  else
  {
    return reverseIteration();
  }
}

uint8_t PixelRange::forwardIteration()
{
  if (_iteratorIndex < _end)
  {
    return ++_iteratorIndex;
  }
  return _iteratorIndex;
}

uint8_t PixelRange::reverseIteration()
{
  if (_iteratorIndex > _start)
  {
    return --_iteratorIndex;
  }
  return _iteratorIndex;
}