#ifndef MOVE_H
#define MOVE_H

#include <QPoint>
#include <functional>
#include <memory>

#include "node.h"

class Playground;
class Move;

class Move
{
public:
    virtual ~Move() {}

    int& moveIndex(int& idx) const {
        return m_moveIndex(idx);
    }

    int  arithmOper(int left, int right) const {
        return m_arithmOper(left, right);
    }

    bool compare(int left, int right) const {
        return m_compare(left, right);
    }

    SpNode& access(int x1, int x2) const {
        return m_access(this, x1, x2);
    }

    QPoint* coords(int x1, int x2) const {
        return m_coords(x1, x2);
    }

    int indexInit() const {
        return m_indexInit;
    }

    int indexLimit() const {
        return m_indexLimit;
    }

protected:
    static int & incr(int &arg)
    {
        return ++arg;
    }

    static int &decr(int &arg)
    {
        return --arg;
    }

    SpNode& getNodeColumnConst(int y, int x) const;
    SpNode& getNodeRowConst(int x, int y) const;

    static QPoint *coordinates(int x, int y)
    {
        return new QPoint(x, y);
    }

    static QPoint *coordinatesInv(int y, int x)
    {
        return new QPoint(x, y);
    }

    quint8 fieldSize() const;

    std::function<int&   (int&)>                 m_moveIndex;
    std::function<int    (int,int)>              m_arithmOper;
    std::function<bool   (int,int)>              m_compare;
    std::function<QPoint*(int, int)>             m_coords;
    std::function<SpNode&(const Move*, int,int)> m_access;
    int m_indexInit, m_indexLimit;

    mutable Playground* m_parent;
};

class MoveLeft : public Move {
public:
    MoveLeft(Playground* parent);
};

class MoveRight: public Move {
public:
    MoveRight(Playground* parent);
};

class MoveUp : public Move {
public:
    MoveUp(Playground* parent);
};

class MoveDown: public Move {
public:
    MoveDown(Playground* parent);
};


#endif // MOVE_H
