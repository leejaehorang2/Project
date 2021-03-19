#ifndef HELP_DIALOG_HPP
#define HELP_DIALOG_HPP

#include <QDialog>
#include <QIcon>

#include "definition.hpp"

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private slots:
    void clickedCloseButton();

private:
    Ui::HelpDialog *ui;

    QPixmap iconImageList[MOTION_IMAGE_COUNT];
};

#endif // HELP_DIALOG_HPP
