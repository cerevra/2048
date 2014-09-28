
#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>
#include <stdlib.h>
#include <QPropertyAnimation>

#include "playground.h"

const QColor Playground::m_backroundColor = QColor(176,196,222);

Playground::Playground(QWidget *parent)
    : QWidget      (parent)
    , m_fieldSize  (4     )
    , m_xOffset    (0     )
    , m_yOffset    (0     )
    , m_maximumNode(0     )
    , m_totalScore (0     )
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus      (Qt::ActiveWindowFocusReason);

    qsrand(QDateTime::currentDateTime().toTime_t());

    connect(this,SIGNAL(needToRepaint()),this,SLOT(repaint()));
    initGrid();
}

Playground::~Playground()
{
    disconnect(this,SIGNAL(needToRepaint()),this,SLOT(repaint()));

    clearGrid();
}

QSize Playground::sizeHint() const
{
    return QSize(m_rectSize,m_rectSize);
}

void Playground::paintEvent(QPaintEvent *)
{
//    Q_UNUSED(event)

//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);

//    int rectInterval  = m_rectSize + m_rectMargin;

//    QFont font       = this->font();
//    font.setPointSizeF(m_rectSize/3);
//    int   fontHiegth = font.pointSize();
//    int   digitY     = (m_rectSize + fontHiegth)*0.5;

//    QFontMetrics fontMetrics(font);

//    for(int x = 0; x < m_fieldSize; ++x)
//    {
//        for(int y = 0; y < m_fieldSize; ++y)
//        {

//            QRect currentRect(m_xOffset+x*rectInterval, m_yOffset+y*rectInterval,
//                                 m_rectSize ,m_rectSize);
//            const Node* node = m_grid[x][y];

//            if (!node)
//            {
//                painter.setPen  (m_backroundColor);
//                painter.setBrush(m_backroundColor);
//                painter.drawRoundedRect(currentRect, m_rectMargin, m_rectMargin);

//                continue;
//            }

//            painter.setPen  (node->color());
//            painter.setBrush(node->color());
//            painter.drawRoundedRect(currentRect, m_rectMargin, m_rectMargin);

//            if (node->value())
//            {
//                QString valueStr  = QVariant(node->value()).toString();
//                painter.setPen  (m_backroundColor);
//                painter.setFont (font);
//                painter.drawText(x*rectInterval + (m_rectSize - fontMetrics.width(valueStr))*0.5 + m_xOffset,
//                                 y*rectInterval + digitY + m_yOffset,
//                                 valueStr);
//            }
//        }
//    }
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

    QWidget::resizeEvent(event);
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

    generateNewNode();
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

void Playground::setFieldSize(quint8 size)
{
    if (size == m_fieldSize)
        return;

    clearGrid();

    m_fieldSize   = size;

    m_maximumNode = 0;
    m_totalScore  = 0;

    initGrid();
    resizeEvent(new QResizeEvent(QSize(this->width(),this->height()),QSize()));
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

    Node* node = m_grid[point.x()][point.y()] = new Node(value, this);
    node->show();
    addAnimation(node, &point);

    m_totalScore += value;
    emit totalScore(m_totalScore);

    if (value > m_maximumNode)
    {
        m_maximumNode = value;
        emit maximumNode(value);
    }

//    emit needToRepaint();
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
    Movement   moveIndex;
    Arithmetic arithmOper;
    Comparison compare;
    NodeAccess access;

    int indexInit, indexLimit;

    // todo optimize
    switch (direction)
    {
    case Direction::Left:
        moveIndex  = &incr;
        arithmOper = &summ;
        compare    = &lsth;
        access     = &Playground::getNodeRowConst;
        indexInit  = 0;
        indexLimit = m_fieldSize;
        break;
    case Direction::Right:
        moveIndex  = &decr;
        arithmOper = &diff;
        compare    = &grtn;
        access     = &Playground::getNodeRowConst;
        indexInit  = m_fieldSize-1;
        indexLimit = -1;
        break;
    case Direction::Up:
        moveIndex  = &incr;
        arithmOper = &summ;
        compare    = &lsth;
        access     = &Playground::getNodeColumnConst;
        indexInit  = 0;
        indexLimit = m_fieldSize;
        break;
    case Direction::Down:
        moveIndex  = &decr;
        arithmOper = &diff;
        compare    = &grtn;
        access     = &Playground::getNodeColumnConst;
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
                        *(this->*access)(posToNear, indexTop) = *nodeFrom;
                        *nodeFrom = nullptr;
                        result = true;
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

void Playground::addAnimation(Node *node, const QPoint *to, const QPoint *from)
{
    QRect               curGeometry = node->geometry();
    QPropertyAnimation* anim        = new QPropertyAnimation(node, "geometry", this);
    int rectInterval  = m_rectSize + m_rectMargin;

    if (from)
    {}
    else
    {
        anim->setStartValue(QRect(m_xOffset + to->x()*rectInterval + m_rectSize/2,
                                  m_yOffset + to->y()*rectInterval + m_rectSize/2,
                                  0, 0));
        anim->setEndValue  (QRect(m_xOffset + to->x()*rectInterval,
                                  m_yOffset + to->y()*rectInterval,
                                  m_rectSize, m_rectSize));
        anim->setEasingCurve(QEasingCurve::InOutQuad);
        anim->setDuration(300);
    }
    anim->start(QPropertyAnimation::DeleteWhenStopped);
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

Node **Playground::getNodeColumnConst(int index1, int index2)
{
    return &m_grid[index2][index1];
}

Node **Playground::getNodeRowConst(int index1, int index2)
{
    return &m_grid[index1][index2];
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
