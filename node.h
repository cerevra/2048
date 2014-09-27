#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QColor>
#include <QRectF>
#include <QWidget>

class Node : public QWidget
{
public:
    explicit Node(quint16 value, QWidget *parent = 0);

    quint16 value      () const;
    void    setValue   (const quint16& value);

    const QColor& color() const;

protected:
    void paintEvent    (QPaintEvent  *);

signals:

public slots:

private:
    qreal log2(qreal val);

    class ColorHandler
    {
    public:
        ColorHandler()
        {
            m_colors.push_back(QColor(176, 196, 222));// background
            m_colors.push_back(QColor(74 , 120, 27 ));
            m_colors.push_back(QColor(188, 137, 2  ));
            m_colors.push_back(QColor(175, 102, 42 ));
            m_colors.push_back(QColor(175, 60 , 42 ));
            m_colors.push_back(QColor(149, 36 , 84 ));
            m_colors.push_back(QColor(111, 28 , 114));
            m_colors.push_back(QColor(84 , 33 , 117));
            m_colors.push_back(QColor(46 , 43 , 122));
            m_colors.push_back(QColor(31 , 82 , 112));
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

    static const ColorHandler m_colors;

    quint16 m_value;
    QColor  m_color;
};

#endif // NODE_H
