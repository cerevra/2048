
#include <QtMath>
#include <QPainter>
#include <QVariant>
#include <QKeyEvent>

#include "node.h"

const Node::ColorHandler Node::m_colors = ColorHandler();

Node::Node(quint16 value, QWidget *parent)
    : QWidget (parent)
    , m_size  (0     )
    , m_digitY(0     )
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

    QRect currentRect(0, 0, m_size, m_size);
    painter.setPen  (m_color);
    painter.setBrush(m_color);
    painter.drawRoundedRect(currentRect, 0,0);//size/5, size/5);

    QFontMetrics fontMetrics(m_font);
    QString valueStr  = QVariant(m_value).toString();
    painter.setPen  (m_colors[0]);
    painter.setFont (m_font);
    painter.drawText((m_size - fontMetrics.width(valueStr))*0.5,
                      m_digitY,
                      valueStr);
}

void Node::resizeEvent(QResizeEvent *event)
{
    m_size   = event->size().height();

    m_font   = this->font();
    m_font.setPointSizeF(m_size/3);
    m_digitY = (m_size + m_font.pointSize())*0.5;
}

qreal Node::log2(qreal val)
{
    return qLn(val)/qLn(2);
}
