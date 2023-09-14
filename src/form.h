#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    void takeTemp(double temp);
     void takePressure(double pressure);

private:
    Ui::Form *ui;
};

#endif // FORM_H
