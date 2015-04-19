#ifndef MOVE_H
#define MOVE_H

#include "move_p.h"

class MvForward : public virtual Move {
public:
    MvForward();

    int& moveIndex(int &arg) const;

protected:
    void init();
};

class MvBackward : public virtual Move {
public:
    MvBackward();

    int& moveIndex(int &arg) const;

protected:
    void init();
};

class MvInRow : public virtual Move {
public:
    MvInRow();

    QPoint* coords(int x1, int x2) const;
};

class MvInColumn : public virtual Move {
public:
    MvInColumn();

    QPoint* coords(int x2, int x1) const;
};

class MoveLeft : public MvForward, public MvInRow {
public:
    MoveLeft(Playground* parent);
};

class MoveRight: public MvBackward, public MvInRow {
public:
    MoveRight(Playground* parent);
};

class MoveUp : public MvForward, public MvInColumn {
public:
    MoveUp(Playground* parent);
};

class MoveDown: public MvBackward, public MvInColumn {
public:
    MoveDown(Playground* parent);
};

#endif // MOVE_P_H
