#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QColor>

class ColorHandler
{
public:
    ColorHandler()
    {
        m_colors.push_back(QColor(176, 196, 222));// background
        m_colors.push_back(QColor(162, 162, 109));
        m_colors.push_back(QColor(158, 144, 105));
        m_colors.push_back(QColor(139, 139, 94 ));
        m_colors.push_back(QColor(128, 155, 115));
        m_colors.push_back(QColor(180, 141, 169));
        m_colors.push_back(QColor(196, 129, 177));
        m_colors.push_back(QColor(191, 100, 166));
        m_colors.push_back(QColor(134, 56 , 112));
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
