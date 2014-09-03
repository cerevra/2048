#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <QVector>
#include <QColor>

#include "node.h"

class Playground : public QWidget
{
    Q_OBJECT
public:
    explicit Playground(QWidget *parent = 0);
    QSize    sizeHint  () const;

signals:
    void needToRepaint ();

public slots:


protected:
    void paintEvent   (QPaintEvent  *event);
    void keyPressEvent(QKeyEvent    *event);
    void resizeEvent  (QResizeEvent *event);

private:
    void  initGrid        ();
    void  generateNewNode ();
    bool  isGameOver      ();

    void setRectSize      (int rectSize);

    float rnd01           ();
    unsigned short rnd0or1();

    static const quint16 m_fieldSize = 4;
    static const QColor  m_backroundColor;

    int             m_rectSize;
    qreal           m_rectMargin;

    Node**          m_grid;
};

#endif // PLAYGROUND_H
