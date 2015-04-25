#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QButtonGroup>

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
    void refreshStyle ();

protected slots:
    void okPressed    ();
    void cancelPressed();

signals:
    void fieldSize    (quint8 size);
    void style        ();

    void changed      ();

private:
    void  init        ();
    void  choiseStyle  ();
    StyleId checkedStyle();

    bool  trySetNewFieldSize();
    bool  trySetNewStyle    ();


    Ui::Settings* ui;
    QButtonGroup* m_buttonGrp;

    quint8 m_size;
};

#endif // SETTINGS_H
