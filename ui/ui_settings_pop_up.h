/********************************************************************************
** Form generated from reading UI file 'settings_pop_up.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_POP_UP_H
#define UI_SETTINGS_POP_UP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_settings_pop_up
{
public:
    QLabel *label_2;
    QLabel *label;
    QComboBox *temp_units;
    QComboBox *pressure_units;
    QPushButton *button_ok;
    QPushButton *button_cancel;

    void setupUi(QDialog *settings_pop_up)
    {
        if (settings_pop_up->objectName().isEmpty())
            settings_pop_up->setObjectName(QString::fromUtf8("settings_pop_up"));
        settings_pop_up->resize(400, 300);
        label_2 = new QLabel(settings_pop_up);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 130, 121, 31));
        label = new QLabel(settings_pop_up);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 60, 131, 31));
        temp_units = new QComboBox(settings_pop_up);
        temp_units->addItem(QString());
        temp_units->addItem(QString());
        temp_units->setObjectName(QString::fromUtf8("temp_units"));
        temp_units->setGeometry(QRect(200, 60, 86, 30));
        pressure_units = new QComboBox(settings_pop_up);
        pressure_units->addItem(QString());
        pressure_units->addItem(QString());
        pressure_units->setObjectName(QString::fromUtf8("pressure_units"));
        pressure_units->setGeometry(QRect(200, 130, 86, 30));
        button_ok = new QPushButton(settings_pop_up);
        button_ok->setObjectName(QString::fromUtf8("button_ok"));
        button_ok->setGeometry(QRect(240, 240, 61, 31));
        button_ok->setFocusPolicy(Qt::WheelFocus);
        button_cancel = new QPushButton(settings_pop_up);
        button_cancel->setObjectName(QString::fromUtf8("button_cancel"));
        button_cancel->setGeometry(QRect(310, 240, 61, 30));

        retranslateUi(settings_pop_up);

        temp_units->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(settings_pop_up);
    } // setupUi

    void retranslateUi(QDialog *settings_pop_up)
    {
        settings_pop_up->setWindowTitle(QCoreApplication::translate("settings_pop_up", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("settings_pop_up", "Pressure unit", nullptr));
        label->setText(QCoreApplication::translate("settings_pop_up", "Temperature unit", nullptr));
        temp_units->setItemText(0, QCoreApplication::translate("settings_pop_up", "\302\260F", nullptr));
        temp_units->setItemText(1, QCoreApplication::translate("settings_pop_up", "\302\260C", nullptr));

        pressure_units->setItemText(0, QCoreApplication::translate("settings_pop_up", "Pa", nullptr));
        pressure_units->setItemText(1, QCoreApplication::translate("settings_pop_up", "ATM", nullptr));

        button_ok->setText(QCoreApplication::translate("settings_pop_up", "Ok", nullptr));
        button_cancel->setText(QCoreApplication::translate("settings_pop_up", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class settings_pop_up: public Ui_settings_pop_up {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_POP_UP_H
