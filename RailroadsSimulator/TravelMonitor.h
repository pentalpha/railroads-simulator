#ifndef TRAVELMONITOR_H
#define TRAVELMONITOR_H

#include <QDialog>
#include <QMutex>
#include <QMutexLocker>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <deque>

namespace Ui {
class TravelMonitor;
}

class TravelMonitor : public QDialog
{
    Q_OBJECT

public:
    explicit TravelMonitor(QWidget *parent = 0);
    ~TravelMonitor();

public slots:
    void putTravelTime(std::string trainName, float duration);

signals:
    void newDataAvailableFor();

private:
    void registerTrain(std::string trainName);
    void updateData();
    QString getTextDescription();


    QMutex addDataMutex;
    Ui::TravelMonitor *ui;
    std::set<std::string> registeredTrains;
    std::map<std::string, std::deque<float> > travelDurations;
    std::map<std::string, float> avgTravelDuration;
    std::map<std::string, float> standardDeviation;
};

#endif // TRAVELMONITOR_H
