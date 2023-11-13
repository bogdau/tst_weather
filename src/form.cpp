#include "include/form.h"
#include "ui/ui_form.h"
#include <QString>
#include <iostream>

Form::Form(Settings &settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    set(settings)
{
    ui->setupUi(this);
    settings.loadSettings();
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, [this](bool){emit setting_request();});
    ui->current_temp_unit->setText(set.temperature_unit.c_str());
    ui->current_pressure_unit->setText(set.pressure_unit.c_str());
}

Form::~Form()
{
    delete ui;
}

void Form::show(){
    this->QWidget::show();
}

void Form::takeTemp(double temp){
    set.loadSettings();
    currentTempUnit = set.temperature_unit;
    ui->current_temp_unit->setText(QString::fromStdString(currentTempUnit));
    if(currentTempUnit == "°C"){
        ui->Temp->setText(QString::number(temp));
    }
    else if(currentTempUnit == "°F"){
        ui->Temp->setText(QString::number((temp * 1.8)+32));
    }
    else{
        ui->Temp->setText(QString::number(temp));
    }
}

void Form::takePressure(double press){
    set.loadSettings();
    currentPressUnit = set.pressure_unit;
    ui->current_pressure_unit->setText(QString::fromStdString(currentPressUnit));
    if (currentPressUnit == "Pa"){
        ui->Pressure->setText(QString::number(press));
    }
    else if(currentPressUnit == "ATM"){
        ui->Pressure->setText(QString::number(press / 101325));
    }
    else{
         ui->Pressure->setText(QString::number(press));
    }
}

void Form::handleTempUnitChange(QString unit){
    currentTempUnit = unit.toStdString();
    set.saveTempSettings(currentTempUnit);
    ui->current_temp_unit->setText(unit);
}

void Form::handlePressUnitChange(QString unit){
    currentPressUnit = unit.toStdString();
    set.savePressSettings(currentPressUnit);
    ui->current_pressure_unit->setText(unit);
}

void Form::getMagneticField(QVector<int> magneticMg){
    ui->magnetic_x->setText(QString::number(magneticMg[0]));
    ui->magnetic_y->setText(QString::number(magneticMg[1]));
    ui->magnetic_z->setText(QString::number(magneticMg[2]));
}

