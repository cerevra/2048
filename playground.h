#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QColor>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <memory>
#include <vector>

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

    void    initAnimation    ();
    void    initFirstAnimation();

    void    keyPress         (const Move& move);
    void generateNewNode();
    void    checkForGameOver ();

    void    setRectSize      (int rectSize);

    void    addMoveAnimation  (SpNode node, const QPoint& from, const QPoint& to);
    void    addCreateAnimation(SpNode node, const QPoint& point);
    QPropertyAnimation* animationFactory(SpNode node);
    void    setStartAnimPoint (QPropertyAnimation* anim, const QPoint& point, int size, int summ = 0);
    void    setEndAnimPoint   (QPropertyAnimation* anim, const QPoint& point);
    void    checkMaxNode     (int val);

    QPoint generateNewPoint();
    quint16                 generateNewValue();

    float   rnd01            ();
    quint16 rnd0or1          ();

    static const QColor m_backroundColor;

    quint8       m_fieldSize  = 4;
    int          m_rectSize   = 0;
    qreal        m_rectMargin = 0;
    int          m_rectInterval = m_rectSize + m_rectMargin;

    int          m_xOffset    = 0;
    int          m_yOffset    = 0;

    int          m_maximumNode = 0;
    int          m_totalScore  = 0;

    bool         m_firstDisplay = true;

    SpNode       m_node;
    QPoint       m_firstPoint;

    typedef std::vector<SpNode    > NodeColumn;
    typedef std::vector<NodeColumn> Grid;
    Grid         m_grid;

    QList<SpNode> m_toDelete;

    bool         m_isAnimationRunning = false;

    bool         m_gridHasEmptyNode;

    QParallelAnimationGroup* m_animCreate = nullptr;
    QParallelAnimationGroup* m_animMove   = nullptr;

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
