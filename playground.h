#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QColor>

#include "node.h"

class Playground : public QWidget
{
    Q_OBJECT
public:
    explicit Playground(QWidget *parent = 0);
    QSize    sizeHint  () const;

signals:
    void needToRepaint ();

public slots:


protected:
    void paintEvent   (QPaintEvent  *event);
    void keyPressEvent(QKeyEvent    *event);
    void resizeEvent  (QResizeEvent *event);

private:
    void initGrid        ();
    void generateNewNode ();
    bool isGameOver      ();

    void swapNodes       (int xFrom, int yFrom, int xTo, int yTo);

    bool moveRoutineUp   ();
    bool moveRoutineDown ();
    bool moveRoutineRight();
    bool moveRoutineLeft ();

    void moveRectsInColumnUp   (quint8 column);
    void moveRectsInColumnDown (quint8 column);
    void moveRectsInRowRight   (quint8 row);
    void moveRectsInRowLeft    (quint8 row);

    void setRectSize     (int rectSize);

    float rnd01           ();
    unsigned short rnd0or1();

    static const quint8 m_fieldSize = 4;
    static const QColor m_backroundColor;

    int             m_rectSize;
    qreal           m_rectMargin;

    Node**          m_grid;
};

#endif // PLAYGROUND_H
