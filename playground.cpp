
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

void Playground::swapNodes(int xFrom, int yFrom, int xTo, int yTo)
{
    m_grid[xTo  ][yTo  ].setValue(m_grid[xFrom][yFrom].value());
    m_grid[xFrom][yFrom].setValue(0);
}

bool Playground::moveRoutineUp()
{
    bool result = false;

    // todo: optimize
    for(int x = 0; x < m_fieldSize; ++x)
    {
        for(int y = 0; y < m_fieldSize-1; ++y)
        {
            moveRectsInColumnUp(x);

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
            moveRectsInColumnDown(x);

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
            moveRectsInRowRight(y);

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
            moveRectsInRowLeft(y);

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

// todo: optimize
// up only
void Playground::moveRectsInColumnUp(quint8 column)
{
    for(int y = 0; y < m_fieldSize-1; ++y)
    {
        if(!m_grid[column][y].value())
        {
            int yFwd;
            for(yFwd = y+1; yFwd < m_fieldSize; ++yFwd)
            {
                if(m_grid[column][yFwd].value())
                {
                    swapNodes(column,yFwd,column,y);
                    break;
                }
            }
            if(yFwd == m_fieldSize)
                break;
        }
    }
}

void Playground::moveRectsInColumnDown(quint8 column)
{
    for(int y = m_fieldSize-1; y > 0; --y)
    {
        if(!m_grid[column][y].value())
        {
            int yFwd;
            for(yFwd = y-1; yFwd >= 0; --yFwd)
            {
                if(m_grid[column][yFwd].value())
                {
                    swapNodes(column,yFwd,column,y);
                    break;
                }
            }
            if(yFwd == m_fieldSize)
                break;
        }
    }
}

void Playground::moveRectsInRowRight(quint8 row)
{
    for(int x = m_fieldSize-1; x > 0; --x)
    {
        if(!m_grid[x][row].value())
        {
            int xFwd;
            for(xFwd = x-1; xFwd >= 0; --xFwd)
            {
                if(m_grid[xFwd][row].value())
                {
                    swapNodes(xFwd,row,x,row);
                    break;
                }
            }
            if(xFwd == m_fieldSize)
                break;
        }
    }
}

void Playground::moveRectsInRowLeft(quint8 row)
{
    for(int x = 0; x < m_fieldSize-1; ++x)
    {
        if(!m_grid[x][row].value())
        {
            int xFwd;
            for(xFwd = x+1; xFwd < m_fieldSize; ++xFwd)
            {
                if(m_grid[xFwd][row].value())
                {
                    swapNodes(xFwd,row,x,row);
                    break;
                }
            }
            if(xFwd == m_fieldSize)
                break;
        }
    }
}

void Playground::setRectSize(int rectSize)
{
    m_rectSize   = rectSize;
    m_rectMargin = rectSize/5;
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
