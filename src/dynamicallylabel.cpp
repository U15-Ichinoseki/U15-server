#include "dynamicallylabel.h"

DynamicallyLabel::DynamicallyLabel(QWidget *parent) :
    QLabel(parent)
{
}

DynamicallyLabel::~DynamicallyLabel()
{
}


void DynamicallyLabel::resizeEvent(QResizeEvent *event) 
{
    // 親クラスの resizeEvent() を呼び出す
    QLabel::resizeEvent(event);
    
    // int textLength = text().toUtf8().size();
    int hsize = qMax(height()/2, 1);
    // int wsize = qMax((width()*2/textLength) , 1);
    // int fsize = qMin(wsize, hsize);

    QFont font = this->font();
    font.setPointSize(hsize);
    this->setFont(font);    
}
