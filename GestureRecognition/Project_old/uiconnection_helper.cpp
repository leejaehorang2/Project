#include "uiconnection_helper.hpp"

UIConnectionHelper::UIConnectionHelper(QObject *parent) : QObject(parent)
{

}

void UIConnectionHelper::requestSetChartData(vector<sensorData> data)
{
    emit setChartData(data);
}
