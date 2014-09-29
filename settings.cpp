#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent              )
    , ui     (new Ui::Settings    )
    , m_size (0                   )
    , m_style(Styles::defaultStyle)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed    ()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelPressed()));
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

void Settings::okPressed()
{
    bool settingsChanged = false;

    // Field size
    quint8 newSize = ui->spinBox->value();
    if (newSize != m_size)
    {
        m_size = newSize;
        settingsChanged = true;
        emit fieldSize(m_size);
    }

    // Style
    Style newStyle;
    if (ui->radioButton_styleClassic->isChecked())
        newStyle = Style::Classic;
    else if (ui->radioButton_styleMetro->isChecked())
        newStyle = Style::Metro;

    if (newStyle != m_style)
    {
        m_style = newStyle;
        settingsChanged = true;
        emit style    (m_style);
    }

    if (settingsChanged)
        emit changed();
}

void Settings::cancelPressed()
{
    ui->spinBox->setValue(m_size);

    if (m_style == Style::Classic)
        ui->radioButton_styleClassic->toggle();
    else if (m_style == Style::Metro)
        ui->radioButton_styleMetro  ->toggle();
}
