#include "CircleLabel.h"

CircleLabel::CircleLabel(QWidget *parent)
    : QWidget(parent), m_diameter(100), m_bgColor(Qt::black), m_textColor(Qt::white)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground, true);
}

CircleLabel::~CircleLabel()
{
}

int CircleLabel::diameter() const { return m_diameter; }

void CircleLabel::setDiameter(int d)
{
    if (m_diameter == d)
        return;
    m_diameter = d;
    // ウィジェット矩形を中央に合わせて更新
    if (parentWidget())
    {
        QRect frect = parentWidget()->rect();
        int x = frect.center().x() - m_diameter / 2;
        int y = frect.center().y() - m_diameter / 2;
        setGeometry(x, y, m_diameter, m_diameter);
    }
    else
    {
        resize(m_diameter, m_diameter);
    }
    update();
    emit diameterChanged(m_diameter);
}

void CircleLabel::setText(const QString &t)
{
    m_text = t;
    update();
}

void CircleLabel::setBgColor(const QColor &c)
{
    m_bgColor = c;
    update();
}

void CircleLabel::setTextColor(const QColor &c)
{
    m_textColor = c;
    update();
}

void CircleLabel::setTextOutlineColor(const QColor& c){ m_outlineColor = c; update(); }
void CircleLabel::setTextOutlineWidth(int w){ m_outlineWidth = w; update(); }
void CircleLabel::enableTextShadow(bool enable){ m_shadowEnabled = enable; update(); }
void CircleLabel::setTextShadowColor(const QColor& c){ m_shadowColor = c; update(); }
void CircleLabel::setTextShadowOffset(const QPoint& off){ m_shadowOffset = off; update(); }

// ヘルパー: 指定領域に収まる最大のピクセルフォントサイズを返す
static int computeMaxPixelFontSize(const QFont &baseFont, const QString &text, int maxWidth, int maxHeight, int maxStart)
{
	QFont f = baseFont;
	// 上限を maxStart に制限
	int start = qMax(4, maxStart);
	for (int sz = start; sz >= 6; --sz) {
		f.setPixelSize(sz);
		QFontMetricsF fm(f);
		QRectF br = fm.boundingRect(text);
		// boundingRect はフォントメトリクスにより多少上下するので余裕を持たせる
		if (br.width() <= maxWidth && br.height() <= maxHeight) {
			return sz;
		}
	}
	return 6;
}

void CircleLabel::paintEvent(QPaintEvent* /*e*/)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 背景円（既存の描画と同等）
    QRect frect = QRect((width() - m_diameter)/2, (height() - m_diameter)/2, m_diameter, m_diameter);
    p.setPen(Qt::NoPen);
    p.setBrush(m_bgColor);
    p.drawEllipse(frect);

    if (m_text.isEmpty()) return;

    // アウトライン幅やシャドウオフセットを考慮した余白を計算
    int outlinePad = m_outlineWidth + 2; // 若干の余裕
    int shadowPadX = qAbs(m_shadowOffset.x());
    int shadowPadY = qAbs(m_shadowOffset.y());
    int padW = outlinePad + shadowPadX;
    int padH = outlinePad + shadowPadY;

    // 文字が収まる最大領域（円内に収めるために少し縮める）
    int availW = qMax(1, frect.width() - padW * 2);
    int availH = qMax(1, frect.height() - padH * 2);

    // 基本フォントを親のフォントから取得し、ピクセルサイズを探索
    QFont baseFont = this->font();
    // 最大開始サイズは円の高さの半分程度を目安に
    int maxStart = qMax(12, frect.height() / 2);
    int pixelSize = computeMaxPixelFontSize(baseFont, m_text, availW, availH, maxStart);
    baseFont.setPixelSize(pixelSize);

    // フォント適用と文字位置計算
    p.setFont(baseFont);
    QFontMetricsF fm(baseFont);
    QRectF textBounds = fm.boundingRect(m_text);
    QPointF center = frect.center();
    QPointF textPos(center.x() - textBounds.width()/2.0, center.y() + textBounds.height()/2.0 - fm.descent());

    // テキストパスを作成
    QPainterPath textPath;
    textPath.addText(textPos, baseFont, m_text);

    // 1) シャドウ（有効なら先に描画、オフセットでずらす）
    if (m_shadowEnabled) {
        QPainterPath shadowPath = textPath;
        QTransform tr;
        tr.translate(m_shadowOffset.x(), m_shadowOffset.y());
        shadowPath = tr.map(shadowPath);
        p.setPen(Qt::NoPen);
        p.setBrush(m_shadowColor);
        p.drawPath(shadowPath);
    }

    // 2) アウトライン（幅が 0 より大きければ描く）
    if (m_outlineWidth > 0) {
        QPen pen(m_outlineColor, m_outlineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        p.drawPath(textPath);
    }

    // 3) 塗り（テキスト本体）
    p.setPen(Qt::NoPen);
    p.setBrush(m_textColor);
    p.drawPath(textPath);
}
