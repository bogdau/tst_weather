#include "settings_pop_up.h"
#include "ui_settings_pop_up.h"
#include <QPushButton>
#include <iostream>

settings_pop_up::settings_pop_up(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings_pop_up)
{
    ui->setupUi(this);
    this->QWidget::setWindowTitle("Settings");
    QObject::connect(ui->button_ok, &QPushButton::clicked, this, &settings_pop_up::on_button_ok_clicked);
}

settings_pop_up::~settings_pop_up()
{
    delete ui;
}

void settings_pop_up::on_button_ok_clicked()
{
    emit temp_units_currentIndexChanged(ui->temp_units->currentText());
    emit pressure_units_currentIndexChanged(ui->pressure_units->currentText());
    emit button_ok_pressed();
    this->QWidget::close();
}
