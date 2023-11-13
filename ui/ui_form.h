/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLabel *Temp_text;
    QLabel *Pressure_text;
    QLabel *Temp;
    QLabel *Pressure;
    QPushButton *pushButton;
    QLabel *current_temp_unit;
    QLabel *current_pressure_unit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *magnetic_z;
    QLabel *magnetic_x;
    QLabel *magnetic_y;
    QLabel *magnetic;
    QLabel *label_4;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(417, 317);
        Temp_text = new QLabel(Form);
        Temp_text->setObjectName(QString::fromUtf8("Temp_text"));
        Temp_text->setGeometry(QRect(20, 50, 82, 22));
        Pressure_text = new QLabel(Form);
        Pressure_text->setObjectName(QString::fromUtf8("Pressure_text"));
        Pressure_text->setGeometry(QRect(20, 110, 63, 22));
        Temp = new QLabel(Form);
        Temp->setObjectName(QString::fromUtf8("Temp"));
        Temp->setGeometry(QRect(190, 50, 68, 22));
        Pressure = new QLabel(Form);
        Pressure->setObjectName(QString::fromUtf8("Pressure"));
        Pressure->setGeometry(QRect(190, 110, 68, 22));
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(250, 230, 91, 30));
        current_temp_unit = new QLabel(Form);
        current_temp_unit->setObjectName(QString::fromUtf8("current_temp_unit"));
        current_temp_unit->setGeometry(QRect(270, 50, 68, 22));
        current_pressure_unit = new QLabel(Form);
        current_pressure_unit->setObjectName(QString::fromUtf8("current_pressure_unit"));
        current_pressure_unit->setGeometry(QRect(270, 110, 68, 22));
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(200, 170, 16, 22));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(260, 170, 16, 22));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(320, 170, 16, 22));
        magnetic_z = new QLabel(Form);
        magnetic_z->setObjectName(QString::fromUtf8("magnetic_z"));
        magnetic_z->setGeometry(QRect(290, 170, 31, 22));
        magnetic_x = new QLabel(Form);
        magnetic_x->setObjectName(QString::fromUtf8("magnetic_x"));
        magnetic_x->setGeometry(QRect(170, 170, 31, 22));
        magnetic_y = new QLabel(Form);
        magnetic_y->setObjectName(QString::fromUtf8("magnetic_y"));
        magnetic_y->setGeometry(QRect(230, 170, 31, 22));
        magnetic = new QLabel(Form);
        magnetic->setObjectName(QString::fromUtf8("magnetic"));
        magnetic->setGeometry(QRect(20, 170, 111, 20));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(360, 170, 68, 22));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        Temp_text->setText(QCoreApplication::translate("Form", "Temerature", nullptr));
        Pressure_text->setText(QCoreApplication::translate("Form", "Pressure", nullptr));
        Temp->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        Pressure->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "Settings", nullptr));
        current_temp_unit->setText(QCoreApplication::translate("Form", "\302\260C", nullptr));
        current_pressure_unit->setText(QCoreApplication::translate("Form", "Pa", nullptr));
        label->setText(QCoreApplication::translate("Form", "X", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "Y", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "Z", nullptr));
        magnetic_z->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        magnetic_x->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        magnetic_y->setText(QCoreApplication::translate("Form", "TextLabel", nullptr));
        magnetic->setText(QCoreApplication::translate("Form", " Magnetic field", nullptr));
        label_4->setText(QCoreApplication::translate("Form", "Gauss", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
