#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSettings>

#include "about.h"
#include "settings.h"

#include "playground.h"
#include "styles.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow   (QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter      (QObject * watched, QEvent * event);

protected:
    void resizeEvent      (QResizeEvent *event              );

public slots:
    void gameOver         ();

    void setMaximumNode   (int max);
    void setTotalScore    (int total);

    void saveCurrSession  ();

private:
    void readPrevSession  ();

    Ui::MainWindow *ui;

    int          m_topMargin;
    int          m_lftMargin;
    int          m_botMargin;
    int          m_rgtMargin;
    Playground * m_playground;

    About      * m_dialogAbout;
    Settings   * m_dialogSettings;

    int          m_bestScore;

    QSettings  * m_settings;
    QString      m_stsBestScore;
    QString      m_stsFieldSize;
    QString      m_stsStyle;
};

#endif // MAINWINDOW_H
