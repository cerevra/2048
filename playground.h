#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QColor>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <memory>

#include "node.h"
#include "styles.h"
#include "move.h"

class Playground : public QWidget
{
    Q_OBJECT
public:
    explicit Playground      (QWidget *parent = 0);
    ~Playground              ();

    inline quint8  fieldSize        () const;
    inline quint16 getMaxNode       () const;
    inline quint16 getTotalScr      () const;

signals:
    void    needToRepaint    ();
    void    needToRefreshPix ();
    void    gameOver         ();

    void    maximumNode      (int max);
    void    totalScore       (int total);

public slots:
    void    resetGrid        ();
    void    setFieldSize     (quint8 size);
    void    refreshStyle     ();

protected:
    void    paintEvent       (QPaintEvent  *);
    void    keyPressEvent    (QKeyEvent    *event);
    void    resizeEvent      (QResizeEvent *event);
    void    offsetOnResize   (QResizeEvent *event);
    void    drawRectsOnResize(QResizeEvent *event);

private slots:
    void    nodeShow         ();
    void    endOfAnimation   ();

private:
    void    initGrid         ();
    void    clearGrid        ();

    void    initAnimation    (bool firstStart = false);

    void    keyPress         (const Move& move);
    bool    generateNewNode  ();
    void    checkForGameOver ();

    bool    moveRoutine      (const Move& move);
    std::pair<bool, bool>
    moveToPoint              (const Move& move, int indexTo, int indexTop);
    void    movePoint        (SpNode& nodeFrom, SpNode& nodeTo, const Move& move,
                              int indexFrom, int indexTo, int indexTop);

    void    setRectSize      (int rectSize);

    void    addMoveAnimation  (SpNode node, const QPoint* from, const QPoint* to);
    void    addCreateAnimation(SpNode node, const QPoint* point);
    QPropertyAnimation* animationFactory(SpNode node);

    void    checkMaxNode     (int val);

    std::pair<QPoint, bool> generateNewPoint();
    quint16                 generateNewValue();

    float   rnd01            ();
    quint16 rnd0or1          ();

    static const QColor m_backroundColor;

    quint8       m_fieldSize;
    int          m_rectSize;
    qreal        m_rectMargin;

    int          m_xOffset;
    int          m_yOffset;

    SpNode**     m_grid;

    int          m_maximumNode;
    int          m_totalScore;

    bool         m_firstDisplay;

    SpNode       m_node;
    QPoint       m_firstPoint;

    QList<SpNode> m_toDelete;

    bool         m_isAnimationRunning = false;

    QParallelAnimationGroup* m_animCreate;
    QParallelAnimationGroup* m_animMove;

    friend class Move;
};

inline quint8 Playground::fieldSize() const
{
    return m_fieldSize;
}

inline quint16 Playground::getMaxNode() const
{
    return m_maximumNode;
}

inline quint16 Playground::getTotalScr() const
{
    return m_totalScore;
}

#endif // PLAYGROUND_H
