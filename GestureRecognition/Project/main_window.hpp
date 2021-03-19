#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include "gesture_sensor.hpp"
#include "help_dialog.hpp"

#include <QValueAxis>
#include <QChartView>

using namespace std;

class QToolButton;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum _toolbar_list{
        TOOLBAR_DEVICE,
        TOOLBAR_MOTION,
        TOOLBAR_LEARNING,
        TOOLBAR_RECOGNITION,
        TOOLBAR_SAVE,
        TOOLBAR_LOAD,
        TOOLBAR_RESET,
        TOOLBAR_COUNT,
    };

    enum _icon_image_list{
        ICON_IMAGE_NORMAL_DEVICE_CONNECT,
        ICON_IMAGE_ACTIVE_DEVICE_CONNECT,
        ICON_IMAGE_NORMAL_DEVICE_DISCONNECT,
        ICON_IMAGE_ACTIVE_DEVICE_DISCONNECT,
        ICON_IMAGE_NORMAL_START_MOTION,
        ICON_IMAGE_ACTIVE_START_MOTION,
        ICON_IMAGE_NORMAL_STOP_MOTION,
        ICON_IMAGE_ACTIVE_STOP_MOTION,
        ICON_IMAGE_NORMAL_LEARNING,
        ICON_IMAGE_ACTIVE_LEARNING,
        ICON_IMAGE_NORMAL_RECOGNITION,
        ICON_IMAGE_ACTIVE_RECOGNITION,
        ICON_IMAGE_NORMAL_SAVE,
        ICON_IMAGE_ACTIVE_SAVE,
        ICON_IMAGE_NORMAL_LOAD,
        ICON_IMAGE_ACTIVE_LOAD,
        ICON_IMAGE_NORMAL_RESET,
        ICON_IMAGE_ACTIVE_RESET,
        ICON_IMAGE_COUNT,
    };

    enum _action_list{
        ACTION_DEVICE_CONNECT,
        ACTION_DEVICE_DISCONNECT,
        ACTION_START_MOTION,
        ACTION_STOP_MOTION,
        ACTION_LEARNING,
        ACTION_RECOGNITION,
        ACTION_SAVE,
        ACTION_LOAD,
        ACTION_RESET,
        ACTION_COUNT,
    };

    struct chartAxis{
        QValueAxis *axisX;
        QValueAxis *axisY;
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initializeWidgets();
    void initialize();
    void showHelpDialog();

    void setGestureSensorObject(GestureSensor *sensor)   {   sensorObj = sensor;    }

    void setMinIFValue(uint16_t minIF);
    void setMaxIFValue(uint16_t maxIF);
    void setCategoryValue(uint16_t category);

    void addLog(string log);

private:
    uint16_t getCategoryNumber(string category);
    void addCategoryListItem(uint16_t categoryNumber, string category);

    void setResultImage(uint16_t category);

    void createHelpDialog();

private slots:
    void setChartData(preprocessingDataSet data);
    void addLearningResult(QString result);
    void clearLearningResult();

    void clickedDeviceConnect();
    void clickedDeviceDisconnect();
    void clickedMotion();
    void clickedTrainingStart();
    void clickedTrainingStop();
    void clickedRecognition();
    void clickedLearning();
    void clickedSaveModel();
    void clickedLoadModel();
    void clickedResetModel();

private:
    const char* GetChartName(_data_list data);

private:
    Ui::MainWindow *ui;

    GestureSensor *sensorObj;
    HelpDialog *helpDialog;

    QAction *actions[ACTION_COUNT];
    QToolButton *toolButtons[TOOLBAR_COUNT];

    QPixmap iconImageList[ICON_IMAGE_COUNT];
    QIcon toolbarIconList[ACTION_COUNT];

    QPixmap motionImageList[MOTION_IMAGE_COUNT];

    QChartView *chartViews[DATA_TOTAL];
    chartAxis axisXY[DATA_TOTAL];

    vector<string> categoryNameVector;

    bool isMotionStartClicked;
};


#endif // MAIN_WINDOW_H
