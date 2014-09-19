#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QColor>

class ColorHandler
{
public:
    ColorHandler()
    {
        // todo correct colors
        m_colors.push_back(QColor(176,196,222));// background
        m_colors.push_back(QColor("green"));
        m_colors.push_back(QColor("darkred"));
        m_colors.push_back(QColor("darkgreen"));
        m_colors.push_back(QColor("gray"));
        m_colors.push_back(QColor("darkgray"));
    }

    const QColor& operator[](quint16 index) const
    {
        while (index >= m_colors.size())
        {
            index -= m_colors.size();

            if (!index)
                ++index;
        }

        return m_colors[index];
    }

private:
    QList<QColor> m_colors;
};

class Node : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 value READ value WRITE setValue)
public:
    explicit Node(quint16 value, QObject *parent = 0);

    quint16 value      () const;
    void    setValue   (const quint16& value);

    const QColor& color() const;

signals:

public slots:

private:
    qreal log2(qreal val);

    static const ColorHandler m_colors;

    quint16 m_value;
    QColor  m_color;
};

#endif // NODE_H
