#ifndef LOGGING_H
#define LOGGING_H
#include <iostream>
#include <string>
#include <mutex>

using namespace std;

void log(std::string origin, std::string message);
void error(std::string origin, std::string message);

#endif // LOGGING_H
