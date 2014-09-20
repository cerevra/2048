#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    void setFieldSize(quint8 size);

protected slots:
    void okPressed   ();

signals:
    void fieldSize   (quint8 size);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
