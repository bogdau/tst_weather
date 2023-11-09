#ifndef __DATA_BASE_H__
#define __DATA_BASE_H__
#include <sqlite3.h>
#include <iostream>
#include <QVector>

class data_base
{
private:
    sqlite3 *db;
    const std::string table_temp_press = "temperature_pressure.db";
    const std::string table_magnetometr = "magnetometr.db";
public:
    data_base();
    void open_table_temp_press();
    void open_table_magnetometr();

    void create_table_temp_press();
    void create_table_magnetometr();

    std::string read_table_temp_press();
    std::string read_table_magnetometr();

    void insert_table_temp_press(int,int);
    void insert_table_magnetometr(QVector<int>);

    void clear_table_temp_press();
    void clear_table_magnetometr();
};

#endif // DATA_BASE_H
