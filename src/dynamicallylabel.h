#ifndef DYNAMICALLYLABEL_H
#define DYNAMICALLYLABEL_H

#include <QLabel>
#include <QWidget>

namespace Ui {
class DynamicallyLabel;
}

class DynamicallyLabel : public QLabel
{
    Q_OBJECT
public:

    explicit DynamicallyLabel(QWidget *parent = 0);
    ~DynamicallyLabel();

protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // DYNAMICALLYLABEL_H
