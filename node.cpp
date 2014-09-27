
#include <QtMath>
#include <QPainter>

#include "node.h"

const Node::ColorHandler Node::m_colors = ColorHandler();

Node::Node(quint16 value, QWidget *parent)
    : QWidget(parent)
{
    setValue(value);
}

quint16 Node::value() const
{
    return m_value;
}

void Node::setValue(const quint16 &value)
{
    m_value = value;
    m_color = m_colors[qRound(log2(value))];
}

const QColor& Node::color() const
{
    return m_color;
}

void Node::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QFont font       = this->font();
    font.setPointSizeF(width()/3);
    int   fontHiegth = font.pointSize();
    int   digitY     = (height() + fontHiegth)*0.5;

    QFontMetrics fontMetrics(font);
    QRect currentRect(0, 0, width() ,height());
    painter.setPen  (m_color);
    painter.setBrush(m_color);
    painter.drawRoundedRect(currentRect, 10, 10);

    QString valueStr  = QVariant(m_value).toString();
    painter.setPen  (m_colors[0]);
    painter.setFont (font);
    painter.drawText((width() - fontMetrics.width(valueStr))*0.5,
                      digitY,
                      valueStr);
}

qreal Node::log2(qreal val)
{
    return qLn(val)/qLn(2);
}
