#include "data_base.h"
#include <sqlite3.h>
data_base::data_base()
{
//    open_table_temp_press();
//    open_table_magnetometr();
}

void data_base::open_table_temp_press(){
    int rc = sqlite3_open(table_temp_press.c_str(), &db);
       if (rc) {
           std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
           create_table_temp_press();
           exit(0);
       } else {
           std::cout << "Opened database successfully" << std::endl;
       }
           create_table_temp_press();
}

void data_base::open_table_magnetometr(){
    int rc = sqlite3_open(table_magnetometr.c_str(), &db);
       if (rc) {
           std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
           create_table_magnetometr();
           exit(0);
       } else {
           std::cout << "Opened database successfully" << std::endl;
       }
           create_table_magnetometr();
}

void data_base::create_table_temp_press(){
    char* errMsg = 0;
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS TEMPERATURE_PRESSURE_DATA("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "TEMPERATURE REAL NOT NULL,"
                                 "PRESSURE REAL NOT NULL,"
                                 "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP);";

    int rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }
}

void data_base::create_table_magnetometr(){
    char* errMsg = 0;
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS MAGNETOMETR_DATA("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "x REAL NOT NULL,"
                                 "y REAL NOT NULL,"
                                 "z REAL NOT NULL,"
                                 "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP);";

    int rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }
}

void data_base::insert_table_temp_press(int temp, int pressure){
    char* errMsg = 0;
    char insertDataSQL[100];
    snprintf(insertDataSQL, sizeof(insertDataSQL),
             "INSERT INTO TEMPERATURE_PRESSURE_DATA (TEMPERATURE, PRESSURE) VALUES (%d, %d);",
             temp,pressure);

    int rc = sqlite3_exec(db, insertDataSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Record inserted successfully" << std::endl;
    }
}

void data_base::insert_table_magnetometr(QVector<int> x){
    char* errMsg = 0;
    char insertDataSQL[100];
    snprintf(insertDataSQL, sizeof(insertDataSQL),
             "INSERT INTO MAGNETOMETR_DATA (X, Y, Z) VALUES (%d, %d, %d);",
             x[0],x[1],x[2]);

    int rc = sqlite3_exec(db, insertDataSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Record inserted successfully" << std::endl;
    }
}

void data_base::read_table_temp_press(){
    const char* query = "SELECT * FROM TEMPERATURE_PRESSURE_DATA";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
                 int id = sqlite3_column_int(stmt, 0);
                 double temperature = sqlite3_column_double(stmt, 1);
                 double pressure = sqlite3_column_double(stmt, 2);
                 const char* timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

                 std::cout << "ID: " << id << ", Temperature: " << temperature << ", Pressure: " << pressure << ", Timestamp: " << timestamp << std::endl;
             }
    } else {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }

}

void data_base::read_table_magnetometr(){
    const char* query = "SELECT * FROM MAGNETOMETR_DATA";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
       while (sqlite3_step(stmt) == SQLITE_ROW) {
           int id = sqlite3_column_int(stmt, 0);
           double x = sqlite3_column_double(stmt, 1);
           double y = sqlite3_column_double(stmt, 2);
           double z = sqlite3_column_double(stmt, 3);
           const char* timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

           std::cout << "ID: " << id << ", x: " << x << ", y: " << y << ", z: " << z << ", Timestamp: " << timestamp << std::endl;
       }
       sqlite3_finalize(stmt); // Clean up the prepared statement
    } else {
       std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }
}
