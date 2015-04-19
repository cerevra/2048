#ifndef MOVE_P_H
#define MOVE_P_H

#include <QPoint>
#include <functional>
#include <memory>

#include "node.h"

class Playground;

class Move
{
public:
    virtual ~Move() {}

    bool moveRoutine() const;
    void moveToPoint() const;
    void movePoint(SpNode& nodeFrom, SpNode& nodeTo, int indexTo) const;

    virtual int& moveIndex(int& idx) const = 0;
    virtual QPoint* coords(int x1, int x2) const = 0;

protected:
    virtual void init() = 0;

    SpNode& getNodeColumnConst(int y, int x) const;
    SpNode& getNodeRowConst(int x, int y) const;
    quint8 fieldSize() const;

    mutable Playground* m_parent;
    std::function<int    (int,int)>              m_arithmOper;
    std::function<bool   (int,int)>              m_compare;
    std::function<SpNode&(const Move*, int,int)> m_access;
    int m_indexInit;
    int m_indexLimit;

    mutable int m_indexTop;
    mutable int m_indexTo;
    mutable int m_indexFrom;
    mutable bool m_result = false;
};

typedef std::shared_ptr<Move> SpMove;

#endif // MOVE_P_H
