#ifndef TRAINSCHEDULE_H
#define TRAINSCHEDULE_H

#include <string>
#include <vector>

using namespace std;

class TrainSchedule
{
public:
    TrainSchedule(string name);
    static vector<TrainSchedule> getDefault();

    string trainName;
    vector<string> path;
};

#endif // TRAINSCHEDULE_H
