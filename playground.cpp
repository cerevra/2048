
#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>
#include <stdlib.h>

#include "playground.h"

const QColor Playground::m_backroundColor = QColor(176,196,222);

Playground::Playground(QWidget *parent)
    : QWidget    (parent)
    , m_fieldSize(4     )
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

void Playground::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            QRect currentRect(x*(m_rectSize + m_rectMargin),y*(m_rectSize + m_rectMargin),
                                 m_rectSize,m_rectSize);
            const Node& node = m_grid[x][y];

            painter.setPen  (node.color());
            painter.setBrush(node.color());
            painter.drawRoundedRect(currentRect,m_rectMargin,m_rectMargin);

            if (node.value())
            {
                painter.setPen  (m_backroundColor);
                painter.drawText(x*(m_rectSize + m_rectMargin)+20,
                                 y*(m_rectSize + m_rectMargin)+20,
                                 QVariant(node.value()).toString());
            }
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

    generateNewNode();
}

void Playground::clearGrid()
{
    for(int x = 0; x < m_fieldSize; ++x)
    {
        free(m_grid[x]);
    }
    free(m_grid);
}

void Playground::resetGrid()
{
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
            m_grid[x][y].setValue(0);
    }

    generateNewNode();
}

quint8 Playground::fieldSize() const
{
    return m_fieldSize;
}

void Playground::setFieldSize(quint8 size)
{
    if (size == m_fieldSize)
        return;

    clearGrid();

    m_fieldSize = size;

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
    vacantPlaces.reserve(m_fieldSize*m_fieldSize);

    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize; ++y)
        {
            if (!m_grid[x][y].value())
                vacantPlaces.append(QPoint(x,y));
        }
    }
    const QPoint& point = vacantPlaces.at(vacantPlaces.size()*rnd01());

    m_grid[point.x()][point.y()].setValue(2*int(1+rnd0or1()));
    emit needToRepaint();

    return vacantPlaces.size() - 1;
}

void Playground::checkForGameOver()
{
    for (int x = 0; x < m_fieldSize; ++x)
    {
        for (int y = 0; y < m_fieldSize - 1; ++y)
        {
            if (m_grid[x][y].value() == m_grid[x][y+1].value())
                return;
        }
    }

    for (int y = 0; y < m_fieldSize; ++y)
    {
        for (int x = 0; x < m_fieldSize - 1; ++x)
        {
            if (m_grid[x][y].value() == m_grid[x+1][y].value())
                return;
        }
    }

    emit gameOver();
}

void Playground::moveNode(int xFrom, int yFrom, int xTo, int yTo)
{
    m_grid[xTo  ][yTo  ].setValue(m_grid[xFrom][yFrom].value());
    m_grid[xFrom][yFrom].setValue(0);
}

void Playground::moveNodeInv(int xFrom, int yFrom, int xTo, int yTo)
{
    moveNode(yFrom, xFrom, yTo, xTo);
}

bool Playground::moveRoutine(Playground::Direction direction)
{
    bool result = false;

    Movement   move;
    Arithmetic arithmOper;
    Comparison compare;
    NodeAccess access;

    int indexInit, indexLimit;

    // todo optimize
    switch (direction)
    {
    case Direction::Left:
        move       = &incr;
        arithmOper = &summ;
        compare    = &lsth;
        access     = &Playground::getNodeRowConst;
        indexInit  = 0;
        indexLimit = m_fieldSize-1;
        break;
    case Direction::Right:
        move       = &decr;
        arithmOper = &diff;
        compare    = &grtn;
        access     = &Playground::getNodeRowConst;
        indexInit  = m_fieldSize-1;
        indexLimit = 0;
        break;
    case Direction::Up:
        move       = &incr;
        arithmOper = &summ;
        compare    = &lsth;
        access     = &Playground::getNodeColumnConst;
        indexInit  = 0;
        indexLimit = m_fieldSize-1;
        break;
    case Direction::Down:
        move       = &decr;
        arithmOper = &diff;
        compare    = &grtn;
        access     = &Playground::getNodeColumnConst;
        indexInit  = m_fieldSize-1;
        indexLimit = 0;
        break;
    default:
        break;
    }

    // todo: optimize
    for(int indexTop = 0; indexTop < m_fieldSize; ++indexTop)
    {
        for(int index = indexInit; compare(index, indexLimit); move(index))
        {
            bool moveResult = moveRects(indexTop,direction);
            if (!result)
                result = moveResult;

            quint16 curValue = (this->*access)(index, indexTop).value();
            if (!curValue)
                break;

            if (curValue == (this->*access)(arithmOper(index, 1), indexTop).value())
            {
                (this->*access)(index               , indexTop).setValue(curValue*2);
                (this->*access)(arithmOper(index, 1), indexTop).setValue(0);
                result = true;
            }
        }
    }

    return result;
}

bool Playground::moveRects(quint8 indexConst, Playground::Direction direction)
{
    bool result = false;

    Movement   move;
    Arithmetic arithmOper;
    Comparison compare;
    NodeAccess access;
    NodeMerge  merge;

    int indexTo, indexToLimit;

    // todo optimize
    switch (direction)
    {
    case Direction::Left:
        move         = &incr;
        arithmOper   = &summ;
        compare      = &lsth;
        access       = &Playground::getNodeRowConst;
        merge        = &Playground::moveNode;
        indexTo      = 0;
        indexToLimit = m_fieldSize-1;
        break;
    case Direction::Right:
        move         = &decr;
        arithmOper   = &diff;
        compare      = &grtn;
        access       = &Playground::getNodeRowConst;
        merge        = &Playground::moveNode;
        indexTo      = m_fieldSize-1;
        indexToLimit = 0;
        break;
    case Direction::Up:
        move         = &incr;
        arithmOper   = &summ;
        compare      = &lsth;
        access       = &Playground::getNodeColumnConst;
        merge        = &Playground::moveNodeInv;
        indexTo      = 0;
        indexToLimit = m_fieldSize-1;
        break;
    case Direction::Down:
        move         = &decr;
        arithmOper   = &diff;
        compare      = &grtn;
        access       = &Playground::getNodeColumnConst;
        merge        = &Playground::moveNodeInv;
        indexTo      = m_fieldSize-1;
        indexToLimit = 0;
        break;
    default:
        return false;
        break;
    }

    for(; compare(indexTo, indexToLimit); move(indexTo))
    {
        if (!(this->*access)(indexTo,indexConst).value())
        {
            int indexFromLimit = arithmOper(indexToLimit, 1);

            int indexFrom;
            for(indexFrom = arithmOper(indexTo,1); compare(indexFrom, indexFromLimit); move(indexFrom))
            {
                if ((this->*access)(indexFrom,indexConst).value())
                {
                    (this->*merge)(indexFrom,indexConst,indexTo,indexConst);
                    result = true;
                    break;
                }
            }
            if (indexFrom == indexFromLimit)
                break;
        }
    }

    return result;
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
    if (rndValue < 0.5)
        return 0;
    else
        return 1;
}
