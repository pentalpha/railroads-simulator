#include "TravelMonitor.h"
#include "ui_TravelMonitor.h"
#include <QtMath>
#include <vector>

TravelMonitor::TravelMonitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TravelMonitor)
{
    ui->setupUi(this);
    connect(this, &TravelMonitor::newDataAvailableFor, this, &TravelMonitor::updateData);
}

void TravelMonitor::registerTrain(std::string trainName){
    registeredTrains.insert(trainName);
    travelDurations[trainName] = std::deque<float>();
    standardDeviation[trainName] = 0.0;
    avgTravelDuration[trainName] = 0.0;
}

void TravelMonitor::putTravelTime(std::string trainName, float duration){
    QMutexLocker locker(&addDataMutex);
    if(registeredTrains.count(trainName) == 0){
        registerTrain(trainName);
    }
    travelDurations[trainName].push_back(duration);
    emit newDataAvailableFor();
}

void TravelMonitor::updateData(){
    QMutexLocker locker(&addDataMutex);
    for(std::string trainName : registeredTrains){
        std::deque<float> durations = travelDurations[trainName];
        float total = 0.0;
        float xSquareTotal = 0.0;
        float n = (float)(durations.size());
        for(float x : travelDurations[trainName]){
            total += x;
            xSquareTotal += x*x;
        }
        float avg = total/n;
        float avgSquare = avg*avg;
        float squareAvg = xSquareTotal/n;
        float deviation = qSqrt(squareAvg-avgSquare);
        avgTravelDuration[trainName] = avg;
        standardDeviation[trainName] = deviation;
    }
    ui->plainTextEdit->setPlainText(getTextDescription());
}

QString TravelMonitor::getTextDescription(){
    std::string content = "";
    for(std::string trainName : registeredTrains){
        content += trainName;
        content += ":\t";
        content += QString::number(avgTravelDuration[trainName], 'f', 2).toStdString();
        content += "s, ";
        content += QString::number(standardDeviation[trainName], 'f', 2).toStdString();
        content += "s, ";
        content += QString::number(travelDurations[trainName].back(), 'f', 2).toStdString();
        content += "s\n";
    }
    QString str(content.c_str());
    return str;
}

TravelMonitor::~TravelMonitor()
{
    delete ui;
}
