#include <iostream>
#include <memory>
#include <QWidget>
#include <QObject>
#include <QApplication>

#include "include/bmp280data.h"
#include "include/RawData.h"
#include "include/data_manager.h"
#include "form.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::unique_ptr<DataManager> dtrt = std::make_unique<DataManager>();
    Form main_window;

    BMP280Data *bmp_data = new BMP280Data;
    RawData *raw_data = new RawData;

    if(bmp_data->init() ==  true){
        dtrt->class_manager(raw_data);
    }
    else{
        dtrt->class_manager(bmp_data);
    }

    QObject::connect(dtrt.get(), &DataManager::tempChange, &main_window, &Form::takeTemp);
    QObject::connect(dtrt.get(), &DataManager::pressChange, &main_window, &Form::takePressure);
    main_window.show();
    dtrt->data_manager();

    return app.exec();

}
