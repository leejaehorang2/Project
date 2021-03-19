#include "help_dialog.hpp"
#include "ui_help_dialog.h"

#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>

#include <math.h>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    QDesktopWidget desktopWidget;

    int desktopWidth = desktopWidget.width();
    int desktopHeight = desktopWidget.height() - 64;    //1016

    desktopHeight -= (int)ceil((desktopHeight * 0.1351));  //120

    const int dialogWidth = (int)(ceil(desktopWidth * 0.3438)); //660
    const int dialogHeight = (int)(ceil(desktopHeight * 0.7403));   //650

    const int titleLabelHeight = (int)(ceil(desktopHeight * 0.0683));   //60

    const int explainationWidth = (int)(ceil(desktopWidth * 0.2031)) + 30;   //390
    const int explainationHeight = (int)(ceil(desktopHeight * 0.0227));   //20
    const int explainationStrideX = (int)(ceil(desktopWidth * 0.0156));   //30
    const int explainationStrideY = titleLabelHeight + (int)(ceil(desktopHeight * 0.0113));   //10;

    const int motionIconCellStrideX = (int)(ceil(desktopWidth * 0.0156));   //30
    const int motionIconCellStrideY = explainationStrideY + explainationHeight + (int)(ceil(desktopHeight * 0.0113));   //10;
    const int motionIconCellWidth = (int)(ceil(desktopWidth * 0.3125));   //600
    const int motionIconCellHeight = (int)(ceil(desktopHeight * 0.5125));   //450

    const int closeButtonStrideX = (int)(ceil(desktopWidth * 0.1411));   //271
    const int closeButtonStrideY = motionIconCellStrideY + motionIconCellHeight + (int)(ceil(desktopHeight * 0.0398));   //35
    const int closeButtonWidth = (int)(ceil(desktopWidth * 0.0625));   //120
    const int closeButtonHeight = (int)(ceil(desktopHeight * 0.0455));   //40

    iconImageList[MOTION_IMAGE_01].load("../Icon/motion-01.png");
    iconImageList[MOTION_IMAGE_02].load("../Icon/motion-02.png");
    iconImageList[MOTION_IMAGE_03].load("../Icon/motion-03.png");
    iconImageList[MOTION_IMAGE_04].load("../Icon/motion-04.png");
    iconImageList[MOTION_IMAGE_05].load("../Icon/motion-05.png");
    iconImageList[MOTION_IMAGE_06].load("../Icon/motion-06.png");
    iconImageList[MOTION_IMAGE_07].load("../Icon/motion-07.png");
    iconImageList[MOTION_IMAGE_08].load("../Icon/motion-08.png");
    iconImageList[MOTION_IMAGE_09].load("../Icon/motion-09.png");
    iconImageList[MOTION_IMAGE_10].load("../Icon/motion-10.png");
    iconImageList[MOTION_IMAGE_11].load("../Icon/motion-11.png");
    iconImageList[MOTION_IMAGE_12].load("../Icon/motion-12.png");

    QString fontName = "Helvetica";

    QFont titleFont(fontName,20);
    QFont explainationFont(fontName, 11);
    QFont closeFont(fontName,13);

    titleFont.setBold(true);

    resize(dialogWidth, dialogHeight);
    setStyleSheet("background-color: rgba(57,57,57,255);");
//    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    ui->titleLabel->move(1, 1);
    ui->titleLabel->resize(dialogWidth-1, titleLabelHeight);
    ui->titleLabel->setStyleSheet("padding-left:20px; border:1px solid #979797; background-color:rgba(38,38,38,255); color:rgba(255,255,255,255);");
    ui->titleLabel->setText("Guide");
    ui->titleLabel->setFont(titleFont);

    ui->explainationLabel->move(explainationStrideX, explainationStrideY);
    ui->explainationLabel->resize(explainationWidth, explainationHeight);
    ui->explainationLabel->setStyleSheet("background-color:rgba(57,57,57,255); color:rgba(222,222,222,255);");
    ui->explainationLabel->setText("You can learn and recognize each of the following actions.");
    ui->explainationLabel->setFont(explainationFont);

    ui->motionIconWidget->move(motionIconCellStrideX, motionIconCellStrideY);
    ui->motionIconWidget->resize(motionIconCellWidth, motionIconCellHeight);


    ui->closeButton->move(closeButtonStrideX, closeButtonStrideY);
    ui->closeButton->resize(closeButtonWidth, closeButtonHeight);
    ui->closeButton->setText("Close");
    ui->closeButton->setFont(closeFont);
    ui->closeButton->setStyleSheet("QPushButton{background-color:rgba(91,154,248,255);color:rgba(255,255,255,255);border-radius:8px;}");

    QGridLayout *layout = new QGridLayout;

    for(int j = 0; j < 3; ++j){
        for(int i = 0; i < 4; ++i){
            QLabel *label = new QLabel;
            label->setPixmap(iconImageList[i + (j * 4)]);

            layout->addWidget(label, j, i);
        }
    }

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    ui->motionIconWidget->setLayout(layout);

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(clickedCloseButton()));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::clickedCloseButton()
{
    this->close();
}
