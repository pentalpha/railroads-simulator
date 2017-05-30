#ifndef RAIL_H
#define RAIL_H
#include <string>

class Rail
{
public:
    Rail(std::string name, int length, int xStart, int yStart, int xEnd, int yEnd);
    int length;
    int xStart, xEnd;
    int yStart, yEnd;
    std::string name;
};

#endif // RAIL_H
