#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "playground.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject * watched, QEvent * event);

protected:
    void resizeEvent(QResizeEvent *event              );

public slots:
    void gameIsOver ();

private:
    Ui::MainWindow *ui;

    int          m_topMargin;
    int          m_lftMargin;
    int          m_botMargin;
    int          m_rgtMargin;
    QGridLayout* m_gridLayout;
    Playground * m_playground;
    QAction    * m_actionClose;
};

#endif // MAINWINDOW_H
