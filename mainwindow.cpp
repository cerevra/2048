
#include <QLayout>
#include <QKeyEvent>
#include <QAction>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow     (parent               )
    , ui              (new Ui::MainWindow   )
    , m_topMargin     (100                  )
    , m_lftMargin     (100                  )
    , m_botMargin     (100                  )
    , m_rgtMargin     (100                  )
    , m_gridLayout    (new QGridLayout(this))
    , m_playground    (new Playground (this))
    , m_actionClose   (new QAction    (this))
    , m_dialogAbout   (new About      (this))
    , m_dialogSettings(new Settings   (this))
    , m_highestScore  (0                    )
{
    ui->setupUi(this);

    m_gridLayout->addWidget         (m_playground);
    m_playground->installEventFilter(this        );

    // adding hotkey for closing
    addAction(m_actionClose);
    m_actionClose->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    m_actionClose->activate   (QAction::Trigger);
    connect(m_actionClose     , SIGNAL(triggered()),
            this              , SLOT  (close    ()));

    // Playground communication
    connect(m_playground      , SIGNAL(gameOver      ()),
            this              , SLOT  (gameIsOver    ()));
    connect(m_playground      , SIGNAL(maximumNode   (int)),
            this              , SLOT  (setMaximumNode(int)));
    connect(m_playground      , SIGNAL(totalScore    (int)),
            this              , SLOT  (setTotalScore (int)));
    setMaximumNode(m_playground->getMaxNode ());
    setTotalScore (m_playground->getTotalScr());

    // About dialog
    connect(ui->actionAbout   , SIGNAL(triggered()),
            m_dialogAbout     , SLOT  (show     ()));

    // Settings dialog
    connect(ui->actionSettings, SIGNAL(triggered()),
            m_dialogSettings  , SLOT  (show     ()));
    connect(ui->actionSettings, SIGNAL(triggered        ()),
            this              , SLOT  (transmitFieldSize()));
    connect(m_dialogSettings  , SIGNAL(fieldSize   (quint8)),
            m_playground      , SLOT  (setFieldSize(quint8)));

    connect(ui->actionExit    , SIGNAL(triggered()),
            this              , SLOT  (close    ()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        switch (keyEvent->key())
        {
        case Qt::Key_Q:
            return close();
        default:
            break;
        }
    }
    return QMainWindow::eventFilter(watched,event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();

    m_gridLayout->setGeometry(
        QRect(m_lftMargin,
              m_topMargin,
              size.width ()-(m_lftMargin+m_rgtMargin),
              size.height()-(m_topMargin+m_botMargin)));

    QMainWindow::resizeEvent(event);
}

void MainWindow::gameIsOver()
{
    QMessageBox::information(this, QString("Game is over!"), QString());

    m_playground->resetGrid();
}

void MainWindow::setMaximumNode(int max)
{
    ui->lcdNumber_maxNode->display(max);
}

void MainWindow::setTotalScore(int total)
{
    ui->lcdNumber_totalScore->display(total);

    if (total > m_highestScore)
    {
        m_highestScore = total;
        ui->lcdNumber_highestScore->display(total);
    }
}

void MainWindow::transmitFieldSize()
{
    m_dialogSettings->setFieldSize(m_playground->fieldSize());
}
