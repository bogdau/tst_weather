#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QString>
#include "include/data_manager.h"
#include "settings_pop_up.h"
#include "include/settings.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(Settings &settings,QWidget *parent = nullptr);
    ~Form();

    void takeTemp(double temp);
    void takePressure(double pressure);
public slots:
    void show();
    void handleTempUnitChange(QString unit);
    void handlePressUnitChange(QString unit);
signals:
    void setting_request();
private:
    Ui::Form *ui;
    Settings &set;
    std::string currentTempUnit;
    std::string currentPressUnit;
};

#endif // FORM_H
