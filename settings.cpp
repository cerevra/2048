#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setFieldSize(quint8 size)
{
    ui->spinBox->setValue(size);
}

void Settings::okPressed()
{
    emit fieldSize(ui->spinBox->value());
}
