// CircleLabel.h

#pragma once
#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

namespace Ui {
class CircleLabel;
}

class CircleLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int diameter READ diameter WRITE setDiameter NOTIFY diameterChanged)

private:
    int m_diameter;
    QString m_text;
    QColor m_bgColor;
    QColor m_textColor;

	// 追加: 文字の枠（アウトライン）と影（シャドウ）の設定
	QColor m_outlineColor = Qt::black;
	int   m_outlineWidth = 4;
	bool  m_shadowEnabled = false;
	QColor m_shadowColor = QColor(0,0,0,150);
	QPoint m_shadowOffset = QPoint(3,3);

public:
    explicit CircleLabel(QWidget* parent = nullptr);
    ~CircleLabel();

    int diameter() const;
    void setDiameter(int d);
    void setText(const QString&);
    void setBgColor(const QColor&);
    void setTextColor(const QColor&);
    void setTextOutlineColor(const QColor& c);
    void setTextOutlineWidth(int w);
    void enableTextShadow(bool enable);
    void setTextShadowColor(const QColor& c);
    void setTextShadowOffset(const QPoint& off);

signals:
    void diameterChanged(int);

protected:
    void paintEvent(QPaintEvent*);
};
