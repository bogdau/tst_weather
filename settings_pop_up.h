#ifndef SETTINGS_POP_UP_H
#define SETTINGS_POP_UP_H

#include <QObject>
#include <QDialog>
#include <QComboBox>
#include <QString>
#include "form.h"

namespace Ui {
class settings_pop_up;
}

class settings_pop_up : public QDialog
{
    Q_OBJECT

public:
    explicit settings_pop_up(QWidget *parent = nullptr);
    ~settings_pop_up();
signals:
    void on_temp_units_currentIndexChanged(QString unit);
    void on_pressure_units_currentIndexChanged(QString unit);
    void button_ok_pressed();
public slots:
    void onButtonClicked();

private:
    Ui::settings_pop_up *ui;
};

#endif // SETTINGS_POP_UP_H
