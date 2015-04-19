
#include <QtMath>
#include <QPainter>
#include <QVariant>
#include <QKeyEvent>

#include "node.h"

const Node::ColorHandler Node::m_colors = ColorHandler();

Node::Node(quint16 value, QWidget *parent)
    : QWidget (parent)
{
    setValue(value);
}

void Node::setValue(const quint16 &value)
{
    m_value = value;
    m_color = m_colors[qRound(log2(value))];

    refreshPix();
}

const QColor& Node::color() const
{
    return m_color;
}

void Node::paintEvent(QPaintEvent *)
{
    QPainter painter     (this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap   (0, 0, m_pixmap);
}

void Node::resizeEvent(QResizeEvent *event)
{
    m_size   = event->size().height();

    m_font   = this ->font();
    m_font.setPointSizeF(m_size/3);
    m_digitY = (m_size + m_font.pointSize())*0.5;

    refreshPix();
}

void Node::refreshPix()
{
    m_pixmap = QPixmap(size());
    m_pixmap.fill(Qt::transparent);

    QPainter painter(&m_pixmap);
    painter.initFrom(this);
    painter.setPen  (m_color);
    painter.setBrush(m_color);

    QRect rect(0, 0, m_size, m_size);
    Style::getInstance().drawRect(painter, rect, m_size/5);

    QFontMetrics fontMetrics   (m_font);
    QString valueStr = QVariant(m_value).toString();
    painter.setPen  ( m_colors[0]);
    painter.setFont ( m_font);
    painter.drawText((m_size - fontMetrics.width(valueStr))*0.5,
                      m_digitY,
                      valueStr);
}

qreal Node::log2(qreal val)
{
    return qLn(val)/qLn(2);
}
