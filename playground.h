#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QColor>
#include <QParallelAnimationGroup>

#include "node.h"
#include "styles.h"

class Playground : public QWidget
{
    typedef int&    (            *Movement   )(int&    );
    typedef int     (            *Arithmetic )(int, int);
    typedef bool    (            *Comparison )(int, int);
    typedef Node ** (Playground::*NodeAccess )(int, int);
    typedef QPoint* (            *Coordinates)(int, int);

    enum class Direction {Up,
                          Down,
                          Right,
                          Left};

    Q_OBJECT
public:
    explicit Playground      (QWidget *parent = 0);
    ~Playground              ();

    quint8  fieldSize        () const;
    Style   style            () const;
    quint16 getMaxNode       () const;
    quint16 getTotalScr      () const;

signals:
    void    needToRepaint    ();
    void    gameOver         ();

    void    maximumNode      (int max);
    void    totalScore       (int total);

public slots:
    void    resetGrid        ();
    void    setFieldSize     (quint8 size);
    void    setRectStyle     (Style style);

protected:
    void    paintEvent       (QPaintEvent  *);
    void    keyPressEvent    (QKeyEvent    *event);
    void    resizeEvent      (QResizeEvent *event);

private slots:
    void    nodeShow         ();
    void    endOfAnimation   ();

private:
    void    initGrid         ();
    void    clearGrid        ();

    void    initAnimation    (bool firstStart = false);

    void    keyPress         (Direction direction);
    bool    generateNewNode  ();
    void    checkForGameOver ();

    bool    moveRoutine      (Direction direction);

    void    setRectSize      (int rectSize);

    void    addAnimation     (Node* node, const QPoint* from, const QPoint* to);

    float   rnd01            ();
    quint16 rnd0or1          ();

    static int&    incr              (int& arg);
    static int&    decr              (int& arg);
    static int     summ              (int x1, int x2);
    static int     diff              (int x1, int x2);
    static bool    grtn              (int x1, int x2);
    static bool    lsth              (int x1, int x2);
    static QPoint* coordinates       (int x , int y );
    static QPoint* coordinatesInv    (int y , int x );
           Node**  getNodeColumnConst(int y , int x );
           Node**  getNodeRowConst   (int x , int y );


    static const QColor m_backroundColor;

    quint8       m_fieldSize;
    int          m_rectSize;
    qreal        m_rectMargin;

    int          m_xOffset;
    int          m_yOffset;

    Node***      m_grid;

    int          m_maximumNode;
    int          m_totalScore;

    Style        m_style;

    bool         m_firstDisplay;

    Node*        m_node;
    QPoint       m_firstPoint;

    QList<Node*> m_toDelete;

    bool         m_isAnimationRunning = false;

    QParallelAnimationGroup* m_animCreate;
    QParallelAnimationGroup* m_animMove;
};

#endif // PLAYGROUND_H
