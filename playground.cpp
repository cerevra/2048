
#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>
#include <stdlib.h>
#include <QPropertyAnimation>

#include "playground.h"

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
    , m_style       (Styles::defaultStyle)
    , m_animCreate  (nullptr             )
    , m_animMove    (nullptr             )
    , m_firstDisplay(true                )
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus      (Qt::ActiveWindowFocusReason);

    qsrand(QDateTime::currentDateTime().toTime_t());

    connect(this, SIGNAL(needToRepaint()), this, SLOT(repaint()));

    initGrid();
}

Playground::~Playground()
{
    disconnect(this,SIGNAL(needToRepaint()),this,SLOT(repaint()));

    clearGrid();
}

void Playground::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int rectInterval  = m_rectSize + m_rectMargin;

    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            QRect rect(m_xOffset+x*rectInterval, m_yOffset+y*rectInterval,
                                 m_rectSize ,m_rectSize);

            painter.setPen  (m_backroundColor);
            painter.setBrush(m_backroundColor);

            if (m_style == Style::Classic)
                painter.drawRoundedRect(rect, m_rectMargin, m_rectMargin);
            else if (m_style == Style::Metro)
                painter.drawRect(rect);
        }
    }
}

void Playground::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        keyPress(Direction::Left);
        break;
    case Qt::Key_Right:
        keyPress(Direction::Right);
        break;
    case Qt::Key_Down:
        keyPress(Direction::Down);
        break;
    case Qt::Key_Up:
        keyPress(Direction::Up);
        break;
    default:
        break;
    }
}

void Playground::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int newHeigth = newSize.height();
    int newWidth  = newSize.width ();

    int minimum;
    if (newHeigth < newWidth)
    {
        minimum   =  newHeigth;
        m_xOffset = (newWidth  - minimum)/2;
    }
    else
    {
        minimum   =  newWidth;
        m_yOffset = (newHeigth - minimum)/2;
    }

    setRectSize(minimum*5/(m_fieldSize*6));

    int rectInterval = m_rectMargin + m_rectSize;
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            Node* node = m_grid[x][y];
            if (node)
            {
                node->setGeometry(QRect(m_xOffset + x*rectInterval,
                                        m_yOffset + y*rectInterval,
                                        m_rectSize, m_rectSize));
            }
        }
    }

    QWidget::resizeEvent(event);

    if (m_firstDisplay && minimum > 100)
    {
        m_firstDisplay = false;

        addAnimation(m_node, nullptr, &m_firstPoint);
        m_node->show();
        m_animCreate->start();
    }
}

void Playground::initGrid()
{
    m_grid = (Node***) malloc (m_fieldSize*sizeof(Node***));
    for(int x = 0; x < m_fieldSize; ++x)
    {
        m_grid[x] = (Node**) malloc (m_fieldSize*sizeof(Node**));
        for(int y = 0; y < m_fieldSize; ++y)
            m_grid[x][y] = nullptr;
    }

    initAnimation(true);
    generateNewNode();
}

void Playground::clearGrid()
{
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
            delete m_grid[x][y];

        free(m_grid[x]);
    }
    free(m_grid);
}

void Playground::initAnimation(bool firstStart)
{
    m_animCreate = new QParallelAnimationGroup();
    connect(m_animCreate, SIGNAL(finished   ()),
            m_animCreate, SLOT  (deleteLater()));

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

void Playground::nodeShow()
{
    m_node->show();
}

void Playground::resetGrid()
{
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            delete m_grid[x][y];
            m_grid[x][y] = nullptr;
        }
    }

    m_maximumNode = 0;
    m_totalScore  = 0;
    m_firstDisplay = true;

    initAnimation(true);
    generateNewNode();
    resizeEvent(new QResizeEvent(QSize(this->width(),this->height()),QSize()));
}

quint8 Playground::fieldSize() const
{
    return m_fieldSize;
}

quint16 Playground::getMaxNode() const
{
    return m_maximumNode;
}

quint16 Playground::getTotalScr() const
{
    return m_totalScore;
}

Style Playground::style() const
{
    return m_style;
}

void Playground::setFieldSize(quint8 size)
{
    if (size == m_fieldSize)
        return;

    clearGrid();

    m_fieldSize   = size;

    m_maximumNode = 0;
    m_totalScore  = 0;
    m_firstDisplay = true;

    initAnimation(true);
    initGrid();
    resizeEvent(new QResizeEvent(QSize(this->width(),this->height()),QSize()));
}

void Playground::setRectStyle(Style style)
{
    m_style = style;

    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            Node* node = m_grid[x][y];
            if (node)
                node->setRectStyle(style);
        }
    }

    emit needToRepaint();
}

void Playground::keyPress(Playground::Direction direction)
{
    if (moveRoutine(direction))
        if (!generateNewNode())
            checkForGameOver();
}

bool Playground::generateNewNode()
{
    QVector<QPoint> vacantPlaces;
    int maxCount = m_fieldSize*m_fieldSize;
    vacantPlaces.reserve(maxCount);

    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            if (!m_grid[x][y])
                vacantPlaces.append(QPoint(x,y));
        }
    }
    const QPoint& point = vacantPlaces.at(vacantPlaces.size()*rnd01());

    quint16 value = 2*int(1+rnd0or1());

    Node* node = m_grid[point.x()][point.y()] = new Node(value, this, m_style);

    m_node  = node;
    if (m_firstDisplay)
        m_firstPoint = point;
    else
        addAnimation(node, nullptr, &point);

    m_totalScore += value;
    emit totalScore(m_totalScore);

    if (value > m_maximumNode)
    {
        m_maximumNode = value;
        emit maximumNode(value);
    }

    if (!m_firstDisplay)
        m_animMove->start();

    return vacantPlaces.size() - 1;
}

void Playground::checkForGameOver()
{
    for (int x = 0; x < m_fieldSize; ++x)
    {
        for (int y = 0; y < m_fieldSize - 1; ++y)
        {
            if (m_grid[x][y]->value() == m_grid[x][y+1]->value())
                return;
        }
    }

    for (int y = 0; y < m_fieldSize; ++y)
    {
        for (int x = 0; x < m_fieldSize - 1; ++x)
        {
            if (m_grid[x][y]->value() == m_grid[x+1][y]->value())
                return;
        }
    }

    emit gameOver();
}

bool Playground::moveRoutine(Direction direction)
{
    initAnimation();

    Movement    moveIndex;
    Arithmetic  arithmOper;
    Comparison  compare;
    NodeAccess  access;
    Coordinates coords;
    int indexInit, indexLimit;

    // todo optimize
    switch (direction)
    {
    case Direction::Left:
        moveIndex  = &incr;
        arithmOper = &summ;
        compare    = &lsth;
        access     = &Playground::getNodeRowConst;
        coords     = &Playground::coordinates;
        indexInit  = 0;
        indexLimit = m_fieldSize;
        break;
    case Direction::Right:
        moveIndex  = &decr;
        arithmOper = &diff;
        compare    = &grtn;
        access     = &Playground::getNodeRowConst;
        coords     = &Playground::coordinates;
        indexInit  = m_fieldSize-1;
        indexLimit = -1;
        break;
    case Direction::Up:
        moveIndex  = &incr;
        arithmOper = &summ;
        compare    = &lsth;
        access     = &Playground::getNodeColumnConst;
        coords     = &Playground::coordinatesInv;
        indexInit  = 0;
        indexLimit = m_fieldSize;
        break;
    case Direction::Down:
        moveIndex  = &decr;
        arithmOper = &diff;
        compare    = &grtn;
        access     = &Playground::getNodeColumnConst;
        coords     = &Playground::coordinatesInv;
        indexInit  = m_fieldSize-1;
        indexLimit = -1;
        break;
    default:
        break;
    }

    bool result = false;

    for (int indexTop = 0; indexTop < m_fieldSize; ++indexTop)
    {
        for (int indexTo = indexInit; compare(indexTo, indexLimit); moveIndex(indexTo))
        {
            Node** nodeTo = (this->*access)(indexTo, indexTop);

            int indexFrom;
            for (indexFrom = arithmOper(indexTo, 1); compare(indexFrom, indexLimit); moveIndex(indexFrom))
            {
                Node** nodeFrom = (this->*access)(indexFrom, indexTop);
                if (! *nodeFrom)
                    continue;

                if (! *nodeTo)
                {
                    *nodeTo   = *nodeFrom;
                    *nodeFrom = nullptr;
                    result    = true;
                    addAnimation(*nodeTo,
                                 coords(indexFrom ,indexTop),
                                 coords(indexTo   ,indexTop));
                    continue;
                }

                quint16 valueTo = (*nodeTo)->value();
                if ((*nodeFrom)->value() == valueTo)
                {
                    quint16 newValue = valueTo*2;

                    delete *nodeTo;
                    *nodeTo   = *nodeFrom;
                    *nodeFrom = nullptr;

                    result    = true;
                    (*nodeTo)->setValue(newValue);

                    addAnimation(*nodeTo,
                                 coords(indexFrom ,indexTop),
                                 coords(indexTo   ,indexTop));

                    if (newValue > m_maximumNode)
                    {
                        m_maximumNode = newValue;
                        emit maximumNode(m_maximumNode);
                    }
                }
                else
                {
                    int posToNear = arithmOper(indexTo, 1);
                    if (indexFrom != posToNear)
                    {
                        Node** nodeNear = (this->*access)(posToNear, indexTop);
                        *nodeNear = *nodeFrom;
                        *nodeFrom = nullptr;
                        result = true;
                        addAnimation(*nodeNear,
                                     coords(indexFrom ,indexTop),
                                     coords(posToNear ,indexTop));
                    }
                }

                break;
            }

            if (indexFrom == indexLimit)
                break;
        }
    }

    emit needToRepaint();

    return result;
}

void Playground::setRectSize(int rectSize)
{
    m_rectSize   = rectSize;
    m_rectMargin = rectSize/5;
}

void Playground::addAnimation(Node *node, const QPoint *from, const QPoint *to)
{
    QPropertyAnimation* anim         = new QPropertyAnimation(node, "geometry", this);
    int                 rectInterval = m_rectSize + m_rectMargin;

    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->setDuration(100);

    if (from)
    {
        anim->setStartValue(QRect(m_xOffset + from->x()*rectInterval,
                                  m_yOffset + from->y()*rectInterval,
                                  m_rectSize, m_rectSize));
        anim->setEndValue  (QRect(m_xOffset + to  ->x()*rectInterval,
                                  m_yOffset + to  ->y()*rectInterval,
                                  m_rectSize, m_rectSize));

        m_animMove->addAnimation(anim);
    }
    else
    {
        anim->setStartValue(QRect(m_xOffset + to->x()*rectInterval + m_rectSize/2,
                                  m_yOffset + to->y()*rectInterval + m_rectSize/2,
                                  3, 3));
        anim->setEndValue  (QRect(m_xOffset + to->x()*rectInterval,
                                  m_yOffset + to->y()*rectInterval,
                                  m_rectSize, m_rectSize));

        m_animCreate->addAnimation(anim);
    }
}

int &Playground::incr(int &arg)
{
    return ++arg;
}

int &Playground::decr(int &arg)
{
    return --arg;
}

int Playground::summ(int x1, int x2)
{
    return x1+x2;
}

int Playground::diff(int x1, int x2)
{
    return x1-x2;
}

bool Playground::grtn(int x1, int x2)
{
    return x1>x2;
}

bool Playground::lsth(int x1, int x2)
{
    return x1<x2;
}

QPoint *Playground::coordinates(int x, int y)
{
    return new QPoint(x, y);
}

QPoint *Playground::coordinatesInv(int y, int x)
{
    return new QPoint(x, y);
}

Node **Playground::getNodeColumnConst(int y, int x)
{
    return &m_grid[x][y];
}

Node **Playground::getNodeRowConst(int x, int y)
{
    return &m_grid[x][y];
}

float Playground::rnd01()
{
    return qrand()/float(RAND_MAX);  // from <stdlib.h>
}

unsigned short Playground::rnd0or1()
{
    float rndValue = rnd01();
    if (rndValue < 0.5)
        return 0;
    else
        return 1;
}
