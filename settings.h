#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

#include "styles.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings (QWidget *parent = 0);
    ~Settings         ();

    void setFieldSize (quint8 size);
    void setRectStyle (Style  style);

protected slots:
    void okPressed    ();
    void cancelPressed();

signals:
    void fieldSize    (quint8 size);
    void style        (Style  style);

    void changed      ();

private:
    Ui::Settings *ui;

    quint8 m_size;
    Style  m_style;
};

#endif // SETTINGS_H
