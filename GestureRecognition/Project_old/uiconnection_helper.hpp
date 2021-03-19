#ifndef UICONNECTION_HELPER_H
#define UICONNECTION_HELPER_H

#include <QObject>
#include <vector>
#include "definition.hpp"

using namespace std;

class UIConnectionHelper : public QObject
{
    Q_OBJECT
public:
    explicit UIConnectionHelper(QObject *parent = nullptr);

signals:
    void setChartData(vector<sensorData> data);

public slots:
    void requestSetChartData(vector<sensorData> data);
};

#endif // UICONNECTION_HELPER_H
