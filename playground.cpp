
#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>
#include <stdlib.h>

#include "playground.h"

const QColor Playground::m_backroundColor = QColor(176,196,222);

Playground::Playground(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus      (Qt::ActiveWindowFocusReason);

    qsrand(QDateTime::currentDateTime().toTime_t());

    connect(this,SIGNAL(needToRepaint()),this,SLOT(repaint()));
    initGrid();

    generateNewNode();
}

QSize Playground::sizeHint() const
{
    return QSize(m_rectSize,m_rectSize);
}

void Playground::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    // background
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

//    painter.translate(-100,10);
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            QRect currentRect(x*(m_rectSize + m_rectMargin),y*(m_rectSize + m_rectMargin),
                                 m_rectSize,m_rectSize);
            const Node& node = m_grid[x][y];
            if(node.value())
            {
                painter.setPen  (node.color());
                painter.setBrush(node.color());
                painter.drawRoundedRect(currentRect,m_rectMargin,m_rectMargin);
                painter.setPen  (m_backroundColor);
                painter.drawText(x*(m_rectSize + m_rectMargin)+20,
                                 y*(m_rectSize + m_rectMargin)+20,
                                 QVariant(node.value()).toString());
            }
            else
            {
                painter.setBrush(m_backroundColor);
                painter.setPen  (m_backroundColor);
                painter.drawRoundedRect(currentRect,m_rectMargin,m_rectMargin);
            }
        }
    }
}

void Playground::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        moveRoutineLeft();
        isGameOver();
        generateNewNode();
        break;
    case Qt::Key_Right:
        moveRoutineRight();
        isGameOver();
        generateNewNode();
        break;
    case Qt::Key_Down:
        moveRoutineDown();
        isGameOver();
        generateNewNode();
        break;
    case Qt::Key_Up:
        moveRoutineUp();
        isGameOver();
        generateNewNode();
        break;
    default:
        break;
    }
}

void Playground::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size   ();
    int   minimum = newSize.height() < newSize.width() ? newSize.height()
                                                       : newSize.width ();

    setRectSize(minimum*5/(m_fieldSize*6));

    QWidget::resizeEvent(event);
}

void Playground::initGrid()
{
    m_grid = (Node**) malloc (m_fieldSize*sizeof(Node));
    for(int x = 0; x < m_fieldSize; ++x)
    {
        m_grid[x] = (Node*) malloc (m_fieldSize*sizeof(Node));
        for(int y = 0; y < m_fieldSize; ++y)
            m_grid[x][y].setValue(0);
    }
}

void Playground::generateNewNode()
{
    QVector<QPoint> vacantPlaces;
    vacantPlaces.reserve(m_fieldSize*m_fieldSize);

    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            if(!m_grid[x][y].value())
                vacantPlaces.append(QPoint(x,y));
        }
    }
    const QPoint& point = vacantPlaces.at(vacantPlaces.size()*rnd01());

    m_grid[point.x()][point.y()].setValue(2*int(1+rnd0or1()));
    emit needToRepaint();
}

bool Playground::isGameOver()
{
    // todo
    return false;
}

void Playground::mergeNodes(int xFrom, int yFrom, int xTo, int yTo)
{
    m_grid[xTo  ][yTo  ].setValue(m_grid[xFrom][yFrom].value());
    m_grid[xFrom][yFrom].setValue(0);
}

void Playground::mergeNodesInv(int xFrom, int yFrom, int xTo, int yTo)
{
    mergeNodes(yFrom, xFrom, yTo, xTo);
}

bool Playground::moveRoutineUp()
{
    bool result = false;

    // todo: optimize
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize-1; ++y)
        {
            moveRects(x,Direction::Up);

            quint16 curValue = m_grid[x][y].value();
            if(!curValue)
                break;

            if(curValue == m_grid[x][y+1].value())
            {
                m_grid[x][y  ].setValue(curValue*2);
                m_grid[x][y+1].setValue(0);
            }
        }
    }

    return result;
}

bool Playground::moveRoutineDown()
{
    bool result = false;

    // todo: optimize
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = m_fieldSize-1; y > 0; --y)
        {
            moveRects(x,Direction::Down);

            quint16 curValue = m_grid[x][y].value();
            if(!curValue)
                break;

            if(curValue == m_grid[x][y-1].value())
            {
                m_grid[x][y  ].setValue(curValue*2);
                m_grid[x][y-1].setValue(0);
            }
        }
    }

    return result;
}

bool Playground::moveRoutineRight()
{
    bool result = false;

    // todo: optimize
    for(int y = 0; y < m_fieldSize; ++y)
    {
        for(int x = m_fieldSize-1; x > 0; --x)
        {
            moveRects(y,Direction::Right);

            quint16 curValue = m_grid[x][y].value();
            if(!curValue)
                break;

            if(curValue == m_grid[x-1][y].value())
            {
                m_grid[x  ][y].setValue(curValue*2);
                m_grid[x-1][y].setValue(0);
            }
        }
    }

    return result;
}

bool Playground::moveRoutineLeft()
{
    bool result = false;

    // todo: optimize
    for(int y = 0; y < m_fieldSize; ++y)
    {
        for(int x = 0; x < m_fieldSize-1; ++x)
        {
            moveRects(y,Direction::Left);

            quint16 curValue = m_grid[x][y].value();
            if(!curValue)
                break;

            if(curValue == m_grid[x+1][y].value())
            {
                m_grid[x  ][y].setValue(curValue*2);
                m_grid[x+1][y].setValue(0);
            }
        }
    }

    return result;
}

void Playground::moveRects(quint8 indexConst, Playground::Direction direct)
{
    Movement   move;
    Arithmetic arithmOper;
    Comparison compare;
    NodeAccess access;
    NodeMerge  merge;

    int indexTo, indexFirstLimit;

    // todo optimize
    switch (direct)
    {
    case Direction::Left:
        move            = &incr;
        arithmOper      = &summ;
        compare         = &lsth;
        access          = &Playground::getNodeRowConst;
        merge           = &Playground::mergeNodes;
        indexTo         = 0;
        indexFirstLimit = m_fieldSize-1;
        break;
    case Direction::Right:
        move            = &decr;
        arithmOper      = &diff;
        compare         = &grtn;
        access          = &Playground::getNodeRowConst;
        merge           = &Playground::mergeNodes;
        indexTo         = m_fieldSize-1;
        indexFirstLimit = 0;
        break;
    case Direction::Up:
        move            = &incr;
        arithmOper      = &summ;
        compare         = &lsth;
        access          = &Playground::getNodeColumnConst;
        merge           = &Playground::mergeNodesInv;
        indexTo         = 0;
        indexFirstLimit = m_fieldSize-1;
        break;
    case Direction::Down:
        move            = &decr;
        arithmOper      = &diff;
        compare         = &grtn;
        access          = &Playground::getNodeColumnConst;
        merge           = &Playground::mergeNodesInv;
        indexTo         = m_fieldSize-1;
        indexFirstLimit = 0;
        break;
    default:
        return;
        break;
    }

    for(; compare(indexTo, indexFirstLimit); move(indexTo))
    {
        if(!(this->*access)(indexTo,indexConst).value())
        {
            int indexFromLimit = arithmOper(indexFirstLimit, 1);

            int indexFrom;
            for(indexFrom = arithmOper(indexTo,1); compare(indexFrom, indexFromLimit); move(indexFrom))
            {
                if((this->*access)(indexFrom,indexConst).value())
                {
                    (this->*merge)(indexFrom,indexConst,indexTo,indexConst);
                    break;
                }
            }
            if(indexFrom == indexFromLimit)
                break;
        }
    }
}

void Playground::setRectSize(int rectSize)
{
    m_rectSize   = rectSize;
    m_rectMargin = rectSize/5;
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

Node &Playground::getNodeColumnConst(int index1, int index2)
{
    return m_grid[index2][index1];
}

Node &Playground::getNodeRowConst(int index1, int index2)
{
    return m_grid[index1][index2];
}

float Playground::rnd01()
{
    return qrand()/float(RAND_MAX);  // from <stdlib.h>
}

unsigned short Playground::rnd0or1()
{
    float rndValue = rnd01();
    if(rndValue < 0.5)
        return 0;
    else
        return 1;
}
