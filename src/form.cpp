#include "form.h"
#include "ui_form.h"
#include <QString>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}


void Form::takeTemp(double temp){
    ui->Temp->setText(QString::number(temp));
}

void Form::takePressure(double press){
    ui->Pressure->setText(QString::number(press));
}
