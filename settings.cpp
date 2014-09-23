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
    m_size = ui->spinBox->value();
    emit fieldSize(m_size);
}

void Settings::cancelPressed()
{
    ui->spinBox->setValue(m_size);
}
