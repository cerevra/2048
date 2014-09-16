#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QColor>

#include "node.h"

class Playground : public QWidget
{
    typedef int& (*Movement  )(int&);
    typedef int  (*Arithmetic)(int,int);
    typedef bool (*Comparison)(int,int);
    typedef Node&(Playground::* NodeAccess)(int,int);
    typedef void (Playground::* NodeMerge )(int,int,int,int);

    enum class Direction {Up,Down,Right,Left};

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

    void moveNode        (int xFrom, int yFrom, int xTo, int yTo);
    void moveNodeInv     (int xFrom, int yFrom, int xTo, int yTo);

    bool moveRoutine     (Direction direction);

    void moveRects       (quint8 indexConst, Direction direction);

    void setRectSize     (int rectSize);

    // temporary solution (todo)
    static int& incr(int& arg);
    static int& decr(int& arg);
    static int  summ(int x1, int x2);
    static int  diff(int x1, int x2);
    static bool grtn(int x1, int x2);
    static bool lsth(int x1, int x2);
    Node&       getNodeColumnConst(int index1, int index2);
    Node&       getNodeRowConst   (int index1, int index2);

    float rnd01           ();
    unsigned short rnd0or1();

    static const quint8 m_fieldSize = 4;
    static const QColor m_backroundColor;

    int             m_rectSize;
    qreal           m_rectMargin;

    Node**          m_grid;
};

#endif // PLAYGROUND_H
