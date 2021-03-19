/********************************************************************************
** Form generated from reading UI file 'help_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_DIALOG_H
#define UI_HELP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelpDialog
{
public:
    QLabel *titleLabel;
    QLabel *explainationLabel;
    QWidget *motionIconWidget;
    QPushButton *closeButton;

    void setupUi(QDialog *HelpDialog)
    {
        if (HelpDialog->objectName().isEmpty())
            HelpDialog->setObjectName(QStringLiteral("HelpDialog"));
        HelpDialog->resize(717, 719);
        titleLabel = new QLabel(HelpDialog);
        titleLabel->setObjectName(QStringLiteral("titleLabel"));
        titleLabel->setGeometry(QRect(60, 30, 531, 31));
        explainationLabel = new QLabel(HelpDialog);
        explainationLabel->setObjectName(QStringLiteral("explainationLabel"));
        explainationLabel->setGeometry(QRect(60, 90, 531, 31));
        motionIconWidget = new QWidget(HelpDialog);
        motionIconWidget->setObjectName(QStringLiteral("motionIconWidget"));
        motionIconWidget->setGeometry(QRect(80, 150, 561, 451));
        closeButton = new QPushButton(HelpDialog);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setGeometry(QRect(290, 630, 131, 61));

        retranslateUi(HelpDialog);

        QMetaObject::connectSlotsByName(HelpDialog);
    } // setupUi

    void retranslateUi(QDialog *HelpDialog)
    {
        HelpDialog->setWindowTitle(QApplication::translate("HelpDialog", "Dialog", nullptr));
        titleLabel->setText(QApplication::translate("HelpDialog", "TextLabel", nullptr));
        explainationLabel->setText(QApplication::translate("HelpDialog", "TextLabel", nullptr));
        closeButton->setText(QApplication::translate("HelpDialog", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelpDialog: public Ui_HelpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_DIALOG_H
