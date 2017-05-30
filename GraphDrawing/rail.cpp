#include "rail.h"

Rail::Rail(std::string name, int length, int xStart, int yStart, int xEnd, int yEnd)
{
    this->name = name;
    this->length = length;
    this->xStart = xStart;
    this->xEnd = xEnd;
    this->yStart = yStart;
    this->yEnd = yEnd;
}
