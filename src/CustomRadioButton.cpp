#include "CustomRadioButton.h"
#include "ui_CustomRadioButton.h"

CustomRadioButton::CustomRadioButton(QWidget *parent)
    : QRadioButton(parent)
    // , ui(new Ui::CustomRadioButton)
{
    // ui->setupUi(this);
}

CustomRadioButton::~CustomRadioButton()
{
    // delete ui;
}

void CustomRadioButton::keyPressEvent(QKeyEvent *event)  {
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        event->ignore();
    } else {
        QRadioButton::keyPressEvent(event);
    }
}
