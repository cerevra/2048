
#include "playground.h"

#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>
#include <stdlib.h>
#include <functional>
#include <utility>

const QColor Playground::m_backroundColor = QColor(176, 196, 222);

Playground::Playground(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus      (Qt::ActiveWindowFocusReason);

    qsrand (QDateTime::currentDateTime().toTime_t());

    initGrid();
}

Playground::~Playground()
{
    clearGrid();
}

void Playground::resetGrid()
{
    clearGrid  ();
    initGrid   ();
    resizeEvent(new QResizeEvent(QSize(this->width(),this->height()),QSize()));
}

void Playground::setFieldSize(quint8 size)
{
    if (size == m_fieldSize)
        return;

    clearGrid  ();
    m_fieldSize = size;
    initGrid   ();
    resizeEvent(new QResizeEvent(QSize(this->width(),this->height()),QSize()));
}

void Playground::refreshStyle()
{
    emit needToRefreshPix();
}

void Playground::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen       (m_backroundColor);
    painter.setBrush     (m_backroundColor);

    Style& style = Style::getInstance();
    for(int x = 0; x < m_fieldSize; ++x) {
        for(int y = 0; y < m_fieldSize; ++y) {
            QRect rect(m_xOffset + x*m_rectInterval,
                       m_yOffset + y*m_rectInterval,
                       m_rectSize,
                       m_rectSize);

            style.drawRect(painter, rect, m_rectMargin);
        }
    }
}

void Playground::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left : keyPress(MoveLeft (this)); break;
    case Qt::Key_Right: keyPress(MoveRight(this)); break;
    case Qt::Key_Down : keyPress(MoveDown (this)); break;
    case Qt::Key_Up   : keyPress(MoveUp   (this)); break;
    default: break;
    }
}

void Playground::resizeEvent(QResizeEvent *event)
{
    offsetOnResize(event);
    drawRectsOnResize(event);

    if (m_firstDisplay &&
            std::min(event->size().width(), event->size().height()) > 100)
    {
        m_firstDisplay = false;

        addCreateAnimation(m_node, m_firstPoint);
        m_node      ->show ();
        m_animCreate->start();
    }
}

void Playground::offsetOnResize(QResizeEvent* event)
{
    QSize newSize = event->size();

    int smallOffset = 0;
    int bigOffset   = (newSize.width() - newSize.height())/2;

    if (newSize.height() < newSize.width()) {
        m_xOffset = bigOffset;
        m_yOffset = smallOffset;
    }
    else {
        m_xOffset = smallOffset;
        m_yOffset = -bigOffset;
    }
}

void Playground::drawRectsOnResize(QResizeEvent* event)
{
    int minimumSize = std::min(event->size().width(), event->size().height());
    setRectSize(minimumSize*5/(m_fieldSize*6));

    for(int x = 0; x < m_fieldSize; ++x) {
        for(int y = 0; y < m_fieldSize; ++y) {
            SpNode node = m_grid[x][y];
            if (node) {
                node->setGeometry(QRect(m_xOffset + x*m_rectInterval,
                                        m_yOffset + y*m_rectInterval,
                                        m_rectSize, m_rectSize));
            }
        }
    }

    QWidget::resizeEvent(event);
}

void Playground::nodeShow()
{
    m_node->show();
    m_toDelete.clear();
}

void Playground::endOfAnimation()
{
    m_animCreate->deleteLater();
    m_isAnimationRunning = false;
}

void Playground::initGrid()
{
    m_maximumNode  = 0;
    m_totalScore   = 0;
    m_firstDisplay = true;

    m_grid.resize(m_fieldSize);
    for(int x = 0; x < m_fieldSize; ++x) {
        m_grid[x].resize(m_fieldSize);
    }

    initFirstAnimation();
    generateNewNode();
}

void Playground::clearGrid()
{
    m_grid.clear();
}

void Playground::initAnimation()
{
    initFirstAnimation();

    m_animMove = new QParallelAnimationGroup();
    connect(m_animMove  , SIGNAL(finished   ()),
            m_animCreate, SLOT  (start      ()));
    connect(m_animMove  , SIGNAL(finished   ()),
            this        , SLOT  (nodeShow   ()));
    connect(m_animMove  , SIGNAL(finished   ()),
            m_animMove  , SLOT  (deleteLater()));
}

void Playground::initFirstAnimation()
{
    m_animCreate = new QParallelAnimationGroup();
    connect(m_animCreate, SIGNAL(finished      ()),
            this        , SLOT  (endOfAnimation()));
}

void Playground::keyPress(const Move& move)
{
    if (m_isAnimationRunning)
        return;

    if (move.moveRoutine()) {
        generateNewNode();
        if (!m_gridHasEmptyNode)
            checkForGameOver();
    }
}

void Playground::generateNewNode()
{
    QPoint newPoint = generateNewPoint();

    SpNode node(new Node(generateNewValue(), this),
                [](Node* n) {n->deleteLater();});
    m_node = m_grid[newPoint.x()][newPoint.y()] = node;

    connect(this        , SIGNAL(needToRefreshPix()),
            m_node.get(), SLOT  (refreshPix      ()));

    if (m_firstDisplay)
        m_firstPoint = newPoint;
    else {
        addCreateAnimation(m_node, newPoint);
        m_animMove->start();
        m_isAnimationRunning = true;
    }
}

void Playground::checkForGameOver()
{
    for (int x = 0; x < m_fieldSize; ++x) {
        for (int y = 0; y < m_fieldSize - 1; ++y) {
            if (m_grid[x][y]->value() == m_grid[x][y+1]->value())
                return;
        }
    }

    for (int y = 0; y < m_fieldSize; ++y) {
        for (int x = 0; x < m_fieldSize - 1; ++x) {
            if (m_grid[x][y]->value() == m_grid[x+1][y]->value())
                return;
        }
    }

    emit gameOver();
}

void Playground::setRectSize(int rectSize)
{
    m_rectSize     = rectSize;
    m_rectMargin   = rectSize/5;
    m_rectInterval = m_rectSize + m_rectMargin;
}

void Playground::addMoveAnimation(SpNode node, const QPoint& from, const QPoint& to)
{
    QPropertyAnimation* anim = animationFactory(node);

    setStartAnimPoint(anim, from, m_rectSize);
    setEndAnimPoint  (anim, to);

    m_animMove->addAnimation(anim);
}

void Playground::addCreateAnimation(SpNode node, const QPoint& point)
{
    QPropertyAnimation* anim = animationFactory(node);

    setStartAnimPoint(anim, point, 3, m_rectSize/2);
    setEndAnimPoint  (anim, point);

    m_animCreate->addAnimation(anim);
}

QPropertyAnimation* Playground::animationFactory(SpNode node)
{
    QPropertyAnimation* anim = new QPropertyAnimation(node.get(), "geometry", this);

    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->setDuration(100);

    return anim;
}

void Playground::setStartAnimPoint(QPropertyAnimation* anim,
                                   const QPoint& point,
                                   int size,
                                   int summ)
{
    anim->setStartValue(QRect(m_xOffset + point.x()*m_rectInterval + summ,
                              m_yOffset + point.y()*m_rectInterval + summ,
                              size, size));
}

void Playground::setEndAnimPoint(QPropertyAnimation* anim, const QPoint& point)
{
    anim->setEndValue  (QRect(m_xOffset + point.x()*m_rectInterval,
                              m_yOffset + point.y()*m_rectInterval,
                              m_rectSize, m_rectSize));
}

void Playground::checkMaxNode(int val)
{
    if (val > m_maximumNode) {
        m_maximumNode = val;
        emit maximumNode(m_maximumNode);
    }
}

QPoint Playground::generateNewPoint()
{
    QVector<QPoint> vacantPlaces;
    vacantPlaces.reserve(m_fieldSize*m_fieldSize);

    for(int x = 0; x < m_fieldSize; ++x) {
        for(int y = 0; y < m_fieldSize; ++y) {
            if (!m_grid[x][y])
                vacantPlaces.append(QPoint(x,y));
        }
    }

    m_gridHasEmptyNode = vacantPlaces.size() - 1;
    return vacantPlaces.at(vacantPlaces.size()*rnd01());
}

quint16 Playground::generateNewValue()
{
    quint16 value = 2*int(1 + rnd0or1());
    m_totalScore += value;
    emit totalScore(m_totalScore);
    checkMaxNode(value);

    return value;
}

float Playground::rnd01()
{
    return qrand()/float(RAND_MAX);  // RAND_MAX from <stdlib.h>
}

quint16 Playground::rnd0or1()
{
    float rndValue = rnd01();
    if (rndValue < 0.5)
        return 0;
    else
        return 1;
}
