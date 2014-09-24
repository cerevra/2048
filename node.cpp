
#include <QtMath>

#include "node.h"

const Node::ColorHandler Node::m_colors = ColorHandler();

Node::Node(quint16 value, QObject *parent)
    : QObject(parent)
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

qreal Node::log2(qreal val)
{
    return qLn(val)/qLn(2);
}
