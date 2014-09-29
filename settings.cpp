#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui     (new Ui::Settings),
    m_size (0)
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
    // Field size
    quint8 newSize = ui->spinBox->value();
    if (newSize != m_size)
        emit fieldSize(m_size);

    // Style
    RectStyle newStyle;
    if (ui->radioButton_styleClassic->isChecked())
        newStyle = RectStyle::Classic;
    else if (ui->radioButton_styleMetro->isChecked())
        newStyle = RectStyle::Metro;

    if (newStyle != m_style)
        emit style    (m_style);
}

void Settings::cancelPressed()
{
    ui->spinBox->setValue(m_size);

    if (m_style == RectStyle::Classic)
        ui->radioButton_styleClassic->toggle();
    else if (m_style == RectStyle::Metro)
        ui->radioButton_styleMetro  ->toggle();
}
