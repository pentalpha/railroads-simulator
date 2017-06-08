#include "trainschedule.h"

TrainSchedule::TrainSchedule(string name)
    : trainName(name)
{

}

vector<TrainSchedule> TrainSchedule::getDefault()
{
    vector<TrainSchedule> schedules;
    TrainSchedule alpha("alpha");
    alpha.path = {"-A","C","D","F","E","-B"};
    schedules.push_back(alpha);
    TrainSchedule alpha2("alpha2");
    alpha2.path = {"A2","-B2","-E2","-F2","-D2","C2"};
    schedules.push_back(alpha2);
    TrainSchedule beta("beta");
    beta.path = {"G","Q","-O","-J","-D"};
    schedules.push_back(beta);
    TrainSchedule beta2("beta2");
    beta2.path = {"G2","D2","-J2","-O2","-Q"};
    schedules.push_back(beta2);
    TrainSchedule gamma("gamma");
    gamma.path = {"I","M","P","-H","-E"};
    schedules.push_back(gamma);
    TrainSchedule gamma2("gamma2");
    gamma2.path = {"M2","I2","E2","-H2","-P"};
    schedules.push_back(gamma2);
    TrainSchedule delta("delta");
    delta.path = {"O2", "N2", "-M2", "-M", "-N", "O"};
    schedules.push_back(delta);

    return schedules;
}
