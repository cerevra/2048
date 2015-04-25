#include "move.h"

#include "playground.h"

SpNode& Move::getNodeColumnConst(int y, int x) const
{
    return m_parent->m_grid[x][y];
}

SpNode& Move::getNodeRowConst(int x, int y) const
{
    return m_parent->m_grid[x][y];
}

quint8 Move::fieldSize() const
{
    return m_parent->m_fieldSize;
}

MoveLeft::MoveLeft(Playground* parent)
{
    m_parent = parent;

    m_moveIndex  = incr;
    m_arithmOper = std::plus<int>();
    m_compare    = std::less<int>();

    m_coords = coordinates;
    m_access = getNodeRowConst;

    m_indexInit = 0;
    m_indexLimit = fieldSize();
}

MoveRight::MoveRight(Playground* parent)
{
    m_parent = parent;

    m_moveIndex  = decr;
    m_arithmOper = std::minus  <int>();
    m_compare    = std::greater<int>();

    m_coords = coordinates;
    m_access = getNodeRowConst;

    m_indexInit = fieldSize() - 1;
    m_indexLimit = -1;
}

MoveUp::MoveUp(Playground* parent)
{
    m_parent = parent;

    m_moveIndex  = incr;
    m_arithmOper = std::plus<int>();
    m_compare    = std::less<int>();

    m_coords = coordinatesInv;
    m_access = getNodeColumnConst;

    m_indexInit = 0;
    m_indexLimit = fieldSize();
}

MoveDown::MoveDown(Playground* parent)
{
    m_parent = parent;

    m_moveIndex  = decr;
    m_arithmOper = std::minus  <int>();
    m_compare    = std::greater<int>();

    m_coords = coordinatesInv;
    m_access = getNodeColumnConst;

    m_indexInit = fieldSize() - 1;
    m_indexLimit = -1;
}
