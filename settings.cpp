#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent              )
    , ui     (new Ui::Settings    )
{
    ui->setupUi(this);
    init();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setFieldSize(quint8 size)
{
    m_size = size;
    ui->spinBox->setValue(m_size);
}

void Settings::refreshStyle()
{
    choiseStyle();
}

void Settings::okPressed()
{
    bool settingsChanged = false;

    settingsChanged |= trySetNewFieldSize();
    settingsChanged |= trySetNewStyle    ();

    if (settingsChanged)
        emit changed();
}

void Settings::cancelPressed()
{
    ui->spinBox->setValue(m_size);
    choiseStyle();
}

void Settings::init()
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed    ()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelPressed()));

    m_buttonGrp = new QButtonGroup(this);
    m_buttonGrp->addButton(ui->radioButton_styleClassic, resolveStyle(StyleId::Classic));
    m_buttonGrp->addButton(ui->radioButton_styleMetro  , resolveStyle(StyleId::Metro  ));
}

void Settings::choiseStyle()
{
    switch(Style::getInstance().id()) {
    case StyleId::Classic: ui->radioButton_styleClassic->toggle(); break;
    case StyleId::Metro  : ui->radioButton_styleMetro  ->toggle(); break;
    }
}

StyleId Settings::checkedStyle()
{
    return resolveStyle(m_buttonGrp->checkedId());
}

bool Settings::trySetNewFieldSize()
{
    quint8 newSize = ui->spinBox->value();
    if (newSize == m_size)
        return false;

    m_size = newSize;
    emit fieldSize(m_size);
    return true;
}

bool Settings::trySetNewStyle()
{
    StyleId newStyle = checkedStyle();
    if (newStyle == Style::getInstance().id())
        return false;

    Style::getInstance().setId(newStyle);
    emit style();
    return true;
}
