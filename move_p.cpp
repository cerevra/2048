#include "move_p.h"

#include "playground.h"

bool Move::moveRoutine() const
{
    m_parent->initAnimation();

    for (m_indexTop = 0; m_indexTop < m_parent->m_fieldSize; ++m_indexTop) {
        for (m_indexTo = m_indexInit;
             m_compare(m_indexTo, m_indexLimit);
             moveIndex(m_indexTo))
        {
            moveToPoint();
            if (m_indexFrom == m_indexLimit)
                break;
        }
    }

    return m_result;
}

void Move::moveToPoint() const
{
    SpNode& nodeTo = m_access(this, m_indexTo, m_indexTop);

    for (m_indexFrom = m_arithmOper(m_indexTo, 1);
         m_compare(m_indexFrom, m_indexLimit);
         moveIndex(m_indexFrom))
    {
        SpNode& nodeFrom = m_access(this, m_indexFrom, m_indexTop);
        if (! nodeFrom)
            continue;

        if (! nodeTo) {
            m_result = true;
            movePoint(nodeFrom, nodeTo, m_indexTo);
            continue;
        }

        quint16 valueTo = nodeTo->value();
        if (nodeFrom->value() == valueTo) {
            quint16 newValue = valueTo*2;
            m_parent->checkMaxNode(newValue);
            nodeFrom->setValue(newValue);

            m_parent->m_toDelete.append(nodeTo);
            m_result = true;
            movePoint(nodeFrom, nodeTo, m_indexTo);
        }
        else {
            int posToNear = m_arithmOper(m_indexTo, 1);
            if (m_indexFrom != posToNear) {
                m_result = true;
                SpNode& nodeNear = m_access(this, posToNear, m_indexTop);
                movePoint(nodeFrom, nodeNear, posToNear);
            }
        }

        break;
    }
}

void Move::movePoint(SpNode& nodeFrom, SpNode& nodeTo, int indexTo) const
{
    nodeTo = nodeFrom;
    nodeFrom.reset();
    m_parent->addMoveAnimation(nodeTo,
                 coords(m_indexFrom, m_indexTop),
                 coords(indexTo    , m_indexTop));
}

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
