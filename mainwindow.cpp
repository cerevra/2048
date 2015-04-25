
#include <QLayout>
#include <QKeyEvent>
#include <QAction>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::m_stsBestScore = "bestScore";
const QString MainWindow::m_stsFieldSize = "fieldSize";
const QString MainWindow::m_stsStyle     = "style"    ;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow       (parent            )
    , ui                (new Ui::MainWindow)
    , m_topMargin       (100               )
    , m_lftMargin       (100               )
    , m_botMargin       (100               )
    , m_rgtMargin       (100               )
{
    ui->setupUi(this);
    init();
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
    if (event->type() == QEvent::KeyPress &&
        static_cast<QKeyEvent*>(event)->key() == Qt::Key_Q)
            return close();

    return QMainWindow::eventFilter(watched, event);
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
    QString msg = int(ui->lcdNumber_maxNode->value()) >= 2048 ?
                       tr("You are the winner!")              :
                       tr("Next time, bro..."  );

    showMsgBox(tr("Game over!"), msg);
    m_playground->resetGrid();
}

void MainWindow::setMaximumNode(int max)
{
    ui->lcdNumber_maxNode->display(max);

    if (max == 2048)
        showMsgBox(tr("You win!"),
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
    m_settings->setValue(m_stsStyle    , Style::getInstance().idInt());

    m_settings->sync();
}

void MainWindow::init()
{
    initPlayground    ();
    initAboutDialog   ();
    initSettingsDialog();
    initActions       ();
    initSettings      ();

    readPrevSession   ();
    setMaximumNode(m_playground->getMaxNode ());
    setTotalScore (m_playground->getTotalScr());
}

void MainWindow::initPlayground()
{
    m_playground = new Playground     (this);
    m_playground->installEventFilter  (this);

    connect(m_playground      , SIGNAL(gameOver       ()),
            this              , SLOT  (gameOver       ()));
    connect(m_playground      , SIGNAL(maximumNode    (int)),
            this              , SLOT  (setMaximumNode (int)));
    connect(m_playground      , SIGNAL(totalScore     (int)),
            this              , SLOT  (setTotalScore  (int)));
}

void MainWindow::initAboutDialog()
{
    m_dialogAbout = new About         (this);
    connect(ui->actionAbout   , SIGNAL(triggered      ()),
            m_dialogAbout     , SLOT  (show           ()));
}

void MainWindow::initSettingsDialog()
{
    m_dialogSettings = new Settings   (this);
    connect(ui->actionSettings, SIGNAL(triggered      ()),
            m_dialogSettings  , SLOT  (show           ()));
    connect(m_dialogSettings  , SIGNAL(fieldSize      (quint8)),
            m_playground      , SLOT  (setFieldSize   (quint8)));
    connect(m_dialogSettings  , SIGNAL(style          ()),
            m_playground      , SLOT  (refreshStyle   ()));
    connect(m_dialogSettings  , SIGNAL(changed        ()),
            this              , SLOT  (saveCurrSession()));
}

void MainWindow::initActions()
{
    connect(ui->actionExit    , SIGNAL(triggered      ()),
            this              , SLOT  (close          ()));
    connect(ui->actionNew     , SIGNAL(triggered      ()),
            m_playground      , SLOT  (resetGrid      ()));
}

void MainWindow::initSettings()
{
    m_settings = new QSettings(QSettings::NativeFormat,
                               QSettings::UserScope,
                               qApp->organizationName(),
                               qApp->applicationName (),
                               this);
}

void MainWindow::readPrevSession()
{
    readPrevBestScore();
    readPrevFieldSize();
    readPrevStyle    ();
}

void MainWindow::readPrevBestScore()
{
    if (m_settings->contains(m_stsBestScore)) {
        m_bestScore = m_settings->value(m_stsBestScore).toInt();
        ui->lcdNumber_highestScore->display(m_bestScore);
    }
}

void MainWindow::readPrevFieldSize()
{
    int size = m_settings  ->contains(m_stsFieldSize)         ?
               m_settings  ->value   (m_stsFieldSize).toInt() :
               m_playground->fieldSize();

    m_playground    ->setFieldSize(size);
    m_dialogSettings->setFieldSize(size);
}

void MainWindow::readPrevStyle()
{
    if (m_settings->contains(m_stsStyle)) {
        Style::getInstance().setId(m_settings->value(m_stsStyle).toInt());
        m_dialogSettings->refreshStyle();
    }
}

void MainWindow::showMsgBox(const QString& header, const QString& body)
{
    QMessageBox::information(this, header, body);
}
