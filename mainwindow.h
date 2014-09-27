#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSettings>

#include "about.h"
#include "settings.h"

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
    void gameIsOver       ();

    void setMaximumNode   (int max);
    void setTotalScore    (int total);

protected slots:
    void transmitFieldSize();

private:
    void readPrevSession  ();
    void saveCurrSession  ();

    Ui::MainWindow *ui;

    int          m_topMargin;
    int          m_lftMargin;
    int          m_botMargin;
    int          m_rgtMargin;
    QGridLayout* m_gridLayout;
    Playground * m_playground;
    QAction    * m_actionClose;

    About      * m_dialogAbout;
    Settings   * m_dialogSettings;

    int          m_highestScore;

    QSettings  * m_settings;
    QString      m_stsHighOption;
    QString      m_stsFieldSize;
};

#endif // MAINWINDOW_H
