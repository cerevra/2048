
#include <QLayout>
#include <QKeyEvent>
#include <QAction>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow       (parent               )
    , ui                (new Ui::MainWindow   )
    , m_topMargin       (100                  )
    , m_lftMargin       (100                  )
    , m_botMargin       (100                  )
    , m_rgtMargin       (100                  )
    , m_bestScore       (0                    )
    , m_settings        (nullptr              )
    , m_stsBestScore    ("bestScore"          )
    , m_stsFieldSize    ("fieldSize"          )
    , m_stsStyle        ("style"              )
{
    ui->setupUi(this);

    // Exit
    connect(ui->actionExit    , SIGNAL(triggered        ()),
            this              , SLOT  (close            ()));

    // Playground
    m_playground = new Playground     (this);
    m_playground->installEventFilter  (this);

    connect(m_playground      , SIGNAL(gameOver         ()),
            this              , SLOT  (gameOver         ()));
    connect(m_playground      , SIGNAL(maximumNode      (int)),
            this              , SLOT  (setMaximumNode   (int)));
    connect(m_playground      , SIGNAL(totalScore       (int)),
            this              , SLOT  (setTotalScore    (int)));

    // About dialog
    m_dialogAbout = new About         (this);
    connect(ui->actionAbout   , SIGNAL(triggered        ()),
            m_dialogAbout     , SLOT  (show             ()));

    // Settings dialog
    m_dialogSettings = new Settings   (this);
    connect(ui->actionSettings, SIGNAL(triggered        ()),
            m_dialogSettings  , SLOT  (show             ()));
    connect(m_dialogSettings  , SIGNAL(fieldSize        (quint8)),
            m_playground      , SLOT  (setFieldSize     (quint8)));
    connect(m_dialogSettings  , SIGNAL(style            (Style)),
            m_playground      , SLOT  (setRectStyle     (Style)));
    connect(m_dialogSettings  , SIGNAL(changed          ()),
            this              , SLOT  (saveCurrSession  ()));

    // New game
    connect(ui->actionNew     , SIGNAL(triggered()),
            m_playground      , SLOT  (resetGrid()));

    // other
    readPrevSession();

    setMaximumNode (m_playground->getMaxNode ());
    setTotalScore  (m_playground->getTotalScr());
}

MainWindow::~MainWindow()
{
    saveCurrSession();

    delete m_playground;
    delete ui;
    delete m_settings;
    delete m_dialogAbout;
    delete m_dialogSettings;
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

    m_playground->setGeometry(
        QRect(m_lftMargin,
              m_topMargin,
              size.width () - (m_lftMargin + m_rgtMargin),
              size.height() - (m_topMargin + m_botMargin)));

    QMainWindow::resizeEvent(event);
}

void MainWindow::gameOver()
{
    QString msg;

    if (int(ui->lcdNumber_maxNode->value()) >= 2048)
        msg = tr("You are the winner!");
    else
        msg = tr("Next time, bro..."  );

    QMessageBox::information(this, tr("Game over!"), msg);

    m_playground->resetGrid();
}

void MainWindow::setMaximumNode(int max)
{
    ui->lcdNumber_maxNode->display(max);

    if (max == 2048)
        QMessageBox::information(this,
                                 tr("You win!"),
                                 tr("You got a node with 2048!\n"
                                    "Here should be girls and beer but I'm not sure about your age.\n"
                                    "So... Congratulations!"));
}

void MainWindow::setTotalScore(int total)
{
    ui->lcdNumber_totalScore->display(total);

    if (total > m_bestScore)
    {
        m_bestScore = total;
        ui->lcdNumber_highestScore->display(total);
    }
}

void MainWindow::saveCurrSession()
{
    m_settings->setValue(m_stsBestScore, m_bestScore);
    m_settings->setValue(m_stsFieldSize, m_playground->fieldSize());
    m_settings->setValue(m_stsStyle    , Styles::resolve(m_playground->style()));

    m_settings->sync();
}

void MainWindow::readPrevSession()
{
    m_settings = new QSettings(QSettings::NativeFormat,
                               QSettings::UserScope,
                               qApp->organizationName(),
                               qApp->applicationName (),
                               this);

    m_bestScore = m_settings->value(m_stsBestScore).toInt();
    if (m_bestScore)
        ui->lcdNumber_highestScore->display(m_bestScore);

    int size = m_settings->value(m_stsFieldSize).toInt();
    if (size)
    {
        m_dialogSettings->setFieldSize(size);
        m_playground    ->setFieldSize(size);
    }
    else
        m_dialogSettings->setFieldSize(m_playground->fieldSize());

    QString style = m_settings->value (m_stsStyle).toString();
    if (!style.isEmpty())
    {
        Style st = Styles::resolve    (style);
        m_dialogSettings->setRectStyle(st);
        m_playground    ->setRectStyle(st);
    }
}
