#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QColor>

#include "node.h"

class Playground : public QWidget
{
    typedef int& (*Movement  )             (int&);
    typedef int  (*Arithmetic)             (int,int);
    typedef bool (*Comparison)             (int,int);
    typedef Node**(Playground::* NodeAccess)(int,int);

    enum class Direction {Up, Down, Right, Left};

    Q_OBJECT
public:
    explicit Playground(QWidget *parent = 0);
    ~Playground();
    QSize    sizeHint  () const;

    void   resetGrid   ();
    quint8 fieldSize   () const;

    quint16 getMaxNode () const;
    quint16 getTotalScr() const;

signals:
    void needToRepaint ();
    void gameOver      ();

    void maximumNode   (int max);
    void totalScore    (int total);

public slots:
    void setFieldSize  (quint8 size);

protected:
    void paintEvent    (QPaintEvent  *);
    void keyPressEvent (QKeyEvent    *event);
    void resizeEvent   (QResizeEvent *event);

private:
    void initGrid        ();
    void clearGrid       ();

    void keyPress        (Direction direction);
    bool generateNewNode ();
    void checkForGameOver();

    bool moveRoutine         (Direction direction);

    void setRectSize     (int rectSize);

    void addAnimation    (Node* node, const QPoint* to, const QPoint* from = nullptr);

    static int& incr(int& arg);
    static int& decr(int& arg);
    static int  summ(int x1, int x2);
    static int  diff(int x1, int x2);
    static bool grtn(int x1, int x2);
    static bool lsth(int x1, int x2);
    Node**      getNodeColumnConst(int index1, int index2);
    Node**      getNodeRowConst   (int index1, int index2);

    float rnd01           ();
    unsigned short rnd0or1();


    static const QColor m_backroundColor;

    quint8  m_fieldSize;
    int     m_rectSize;
    qreal   m_rectMargin;

    int     m_xOffset;
    int     m_yOffset;

    Node*** m_grid;

    // statictics
    int     m_maximumNode;
    int     m_totalScore;
};

#endif // PLAYGROUND_H
