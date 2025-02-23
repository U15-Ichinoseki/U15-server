#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include <QWidget>
#include <QKeyEvent>
#include <QRadioButton>

namespace Ui {
class CustomRadioButton;
}

class CustomRadioButton : public QRadioButton 
{
    Q_OBJECT

public:
    explicit CustomRadioButton(QWidget *parent = nullptr);
    ~CustomRadioButton();

private:
    Ui::CustomRadioButton *ui;

protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // CUSTOMRADIOBUTTON_H
