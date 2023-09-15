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
    QObject::connect(ui->button_ok, &QPushButton::clicked, this, &settings_pop_up::onButtonClicked);
}

settings_pop_up::~settings_pop_up()
{
    delete ui;
}

void settings_pop_up::onButtonClicked()
{
    emit button_ok_pressed();
    this->QWidget::close();
}
