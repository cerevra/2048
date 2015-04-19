
#include "move.h"

int& MvForward::moveIndex(int& arg) const
{
    return ++arg;
}

void MvForward::init()
{
    m_indexInit  = 0;
    m_indexLimit = fieldSize();
}

int& MvBackward::moveIndex(int& arg) const
{
    return --arg;
}

void MvBackward::init()
{
    m_indexInit  = fieldSize() - 1;
    m_indexLimit = -1;
}

QPoint* MvInRow::coords(int x1, int x2) const
{
    return new QPoint(x1, x2);
}

QPoint* MvInColumn::coords(int x2, int x1) const
{
    return new QPoint(x1, x2);
}

MvForward::MvForward() {
    m_arithmOper = std::plus<int>();
    m_compare    = std::less<int>();
}

MvBackward::MvBackward() {
    m_arithmOper = std::minus  <int>();
    m_compare    = std::greater<int>();
}

MvInRow::MvInRow() {
    m_access = std::mem_fn(&MvInRow::getNodeRowConst);
}

MvInColumn::MvInColumn() {
    m_access = std::mem_fn(&MvInColumn::getNodeColumnConst);
}

MoveLeft::MoveLeft(Playground* parent)
{
    m_parent = parent;
    init();
}

MoveRight::MoveRight(Playground* parent)
{
    m_parent = parent;
    init();
}

MoveUp::MoveUp(Playground* parent)
{
    m_parent = parent;
    init();
}

MoveDown::MoveDown(Playground* parent)
{
    m_parent = parent;
    init();
}
