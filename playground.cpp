
#include "playground.h"

#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>
#include <stdlib.h>
#include <functional>
#include <utility>

const QColor Playground::m_backroundColor = QColor(176, 196, 222);

Playground::Playground(QWidget *parent   )
    : QWidget       (parent              )
    , m_fieldSize   (4                   )
    , m_rectSize    (0                   )
    , m_rectMargin  (0                   )
    , m_xOffset     (0                   )
    , m_yOffset     (0                   )
    , m_maximumNode (0                   )
    , m_totalScore  (0                   )
    , m_firstDisplay(true                )
    , m_node        (nullptr             )
    , m_animCreate  (nullptr             )
    , m_animMove    (nullptr             )
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus      (Qt::ActiveWindowFocusReason);

    qsrand (QDateTime::currentDateTime().toTime_t());

    connect(this, SIGNAL(needToRepaint()), this, SLOT(repaint()));

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
    emit needToRepaint   ();
}

void Playground::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen       (m_backroundColor);
    painter.setBrush     (m_backroundColor);

    int rectInterval  = m_rectSize + m_rectMargin;

    for(int x = 0; x < m_fieldSize; ++x) {
        for(int y = 0; y < m_fieldSize; ++y) {
            QRect rect(m_xOffset + x*rectInterval,
                       m_yOffset + y*rectInterval,
                       m_rectSize,
                       m_rectSize);

            Style::getInstance().drawRect(painter, rect, m_rectMargin);
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

        addCreateAnimation(m_node, &m_firstPoint);
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

    int rectInterval = m_rectMargin + m_rectSize;
    for(int x = 0; x < m_fieldSize; ++x) {
        for(int y = 0; y < m_fieldSize; ++y) {
            SpNode node = m_grid[x][y];
            if (node) {
                node->setGeometry(QRect(m_xOffset + x*rectInterval,
                                        m_yOffset + y*rectInterval,
                                        m_rectSize, m_rectSize));
            }
        }
    }

    QWidget::resizeEvent(event);
}

void Playground::nodeShow()
{
    m_node->show();

    for(int i = 0; i < m_toDelete.size(); ++i)
        m_toDelete[i].reset();
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

    m_grid = new SpNode*[m_fieldSize];
    for(int x = 0; x < m_fieldSize; ++x) {
        m_grid[x] = new SpNode[m_fieldSize];
    }

    initAnimation  (true);
    generateNewNode();
}

void Playground::clearGrid()
{
    for(int x = 0; x < m_fieldSize; ++x)
        delete[] m_grid[x];

    delete[] m_grid;
}

void Playground::initAnimation(bool firstStart)
{
    m_animCreate = new QParallelAnimationGroup();
    connect(m_animCreate, SIGNAL(finished       ()),
            this        , SLOT  (endOfAnimation ()));

    if (!firstStart)
    {
        m_animMove = new QParallelAnimationGroup();
        connect(m_animMove  , SIGNAL(finished   ()),
                m_animCreate, SLOT  (start      ()));
        connect(m_animMove  , SIGNAL(finished   ()),
                this        , SLOT  (nodeShow   ()));
        connect(m_animMove  , SIGNAL(finished   ()),
                m_animMove  , SLOT  (deleteLater()));
    }
}

void Playground::keyPress(const Move& move)
{
    if (m_isAnimationRunning)
        return;

    if (moveRoutine(move))
        if (!generateNewNode())
            checkForGameOver();
}

bool Playground::generateNewNode()
{
    auto newPoint = generateNewPoint();

    SpNode node(new Node(generateNewValue(), this),
                [](Node* n) {n->deleteLater();});
    m_node = m_grid[newPoint.first.x()][newPoint.first.y()] = node;

    connect(this        , SIGNAL(needToRefreshPix()),
            m_node.get(), SLOT  (refreshPix      ()));

    if (m_firstDisplay)
        m_firstPoint = newPoint.first;
    else {
        addCreateAnimation(m_node, &newPoint.first);
        m_animMove->start();
        m_isAnimationRunning = true;
    }

    return newPoint.second;
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

bool Playground::moveRoutine(const Move& move)
{
    initAnimation();

    bool result = false;
    for (int indexTop = 0; indexTop < m_fieldSize; ++indexTop) {
        for (int indexTo = move.indexInit();
             move.compare  (indexTo, move.indexLimit());
             move.moveIndex(indexTo))
        {
            auto res = moveToPoint(move, indexTo, indexTop);
            result |= res.first;
            bool isEndRiched = res.second;

            if (isEndRiched)
                break;
        }
    }

    emit needToRepaint();

    return result;
}

std::pair<bool, bool> Playground::moveToPoint(const Move& move, int indexTo, int indexTop)
{
    SpNode& nodeTo = move.access(indexTo, indexTop);

    bool result = false;
    int indexFrom;
    for (indexFrom = move.arithmOper(indexTo, 1);
         move.compare  (indexFrom, move.indexLimit());
         move.moveIndex(indexFrom))
    {
        SpNode& nodeFrom = move.access(indexFrom, indexTop);
        if (! nodeFrom)
            continue;

        if (! nodeTo) {
            result = true;
            movePoint(nodeFrom, nodeTo, move, indexFrom, indexTo, indexTop);
            continue;
        }

        quint16 valueTo = nodeTo->value();
        if (nodeFrom->value() == valueTo) {
            quint16 newValue = valueTo*2;
            checkMaxNode(newValue);
            nodeFrom->setValue(newValue);

            m_toDelete.append(nodeTo);
            result = true;
            movePoint(nodeFrom, nodeTo, move, indexFrom, indexTo, indexTop);
        }
        else {
            int posToNear = move.arithmOper(indexTo, 1);
            if (indexFrom != posToNear) {
                result = true;
                SpNode& nodeNear = move.access(posToNear, indexTop);
                movePoint(nodeFrom, nodeNear, move, indexFrom, posToNear, indexTop);
            }
        }

        break;
    }

    return std::make_pair(result, indexFrom == move.indexLimit());
}

void Playground::movePoint(SpNode& nodeFrom, SpNode& nodeTo, const Move& move, int indexFrom, int indexTo, int indexTop)
{
    nodeTo = nodeFrom;
    nodeFrom.reset();
    addMoveAnimation(nodeTo,
                 move.coords(indexFrom ,indexTop),
                 move.coords(indexTo   ,indexTop));
}

void Playground::setRectSize(int rectSize)
{
    m_rectSize   = rectSize;
    m_rectMargin = rectSize/5;
}

void Playground::addMoveAnimation(SpNode node, const QPoint* from, const QPoint* to)
{
    QPropertyAnimation* anim = animationFactory(node);

    int                 rectInterval = m_rectSize + m_rectMargin;
    anim->setStartValue(QRect(m_xOffset + from->x()*rectInterval,
                              m_yOffset + from->y()*rectInterval,
                              m_rectSize, m_rectSize));
    anim->setEndValue  (QRect(m_xOffset + to  ->x()*rectInterval,
                              m_yOffset + to  ->y()*rectInterval,
                              m_rectSize, m_rectSize));

    m_animMove->addAnimation(anim);
}

void Playground::addCreateAnimation(SpNode node, const QPoint* point)
{
    QPropertyAnimation* anim = animationFactory(node);

    int                 rectInterval = m_rectSize + m_rectMargin;
    anim->setStartValue(QRect(m_xOffset + point->x()*rectInterval + m_rectSize/2,
                              m_yOffset + point->y()*rectInterval + m_rectSize/2,
                              3, 3));
    anim->setEndValue  (QRect(m_xOffset + point->x()*rectInterval,
                              m_yOffset + point->y()*rectInterval,
                              m_rectSize, m_rectSize));

    m_animCreate->addAnimation(anim);
}

QPropertyAnimation* Playground::animationFactory(SpNode node)
{
    QPropertyAnimation* anim = new QPropertyAnimation(node.get(), "geometry", this);

    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->setDuration(100);

    return anim;
}

void Playground::checkMaxNode(int val)
{
    if (val > m_maximumNode) {
        m_maximumNode = val;
        emit maximumNode(m_maximumNode);
    }
}

std::pair<QPoint, bool> Playground::generateNewPoint()
{
    QVector<QPoint> vacantPlaces;
    vacantPlaces.reserve(m_fieldSize*m_fieldSize);

    for(int x = 0; x < m_fieldSize; ++x) {
        for(int y = 0; y < m_fieldSize; ++y) {
            if (!m_grid[x][y])
                vacantPlaces.append(QPoint(x,y));
        }
    }

    bool gridHasEmptyNode = vacantPlaces.size() - 1;
    return std::make_pair(vacantPlaces.at(vacantPlaces.size()*rnd01()),
                          gridHasEmptyNode);
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
