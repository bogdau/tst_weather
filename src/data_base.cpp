#include "include/data_base.h"
#include <sqlite3.h>
#include <sstream>
data_base::data_base()
{
    //    open_table_temp_press();
    //    open_table_magnetometr();
}

void data_base::open_table_temp_press()
{
    int rc = sqlite3_open(table_temp_press.c_str(), &db);
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;

        char *errMsg = 0;
        const char *createTableSQL = "CREATE TABLE IF NOT EXISTS TEMPERATURE_PRESSURE_DATA("
                                     "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "TEMPERATURE REAL NOT NULL,"
                                     "PRESSURE REAL NOT NULL,"
                                     "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP);";

        int rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

        if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
        else
        {
            std::cout << "Table created successfully" << std::endl;
        }
    }
}

void data_base::open_table_magnetometr()
{
    int rc = sqlite3_open(table_magnetometr.c_str(), &db);
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(0);
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;

        char *errMsg = 0;
        const char *createTableSQL = "CREATE TABLE IF NOT EXISTS MAGNETOMETR_DATA("
                                     "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "x REAL NOT NULL,"
                                     "y REAL NOT NULL,"
                                     "z REAL NOT NULL,"
                                     "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP);";

        int rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);

        if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
        else
        {
            std::cout << "Table created successfully" << std::endl;
        }
    }
}

void data_base::insert_table_temp_press(int temp, int pressure)
{
    char *errMsg = 0;
    char insertDataSQL[100];
    snprintf(insertDataSQL, sizeof(insertDataSQL),
             "INSERT INTO TEMPERATURE_PRESSURE_DATA (TEMPERATURE, PRESSURE) VALUES (%d, %d);",
             temp, pressure);

    int rc = sqlite3_exec(db, insertDataSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void data_base::insert_table_magnetometr(QVector<int> x)
{
    char *errMsg = 0;
    char insertDataSQL[100];

    snprintf(insertDataSQL, sizeof(insertDataSQL),
             "INSERT INTO MAGNETOMETR_DATA (X, Y, Z) VALUES (%d, %d, %d);",
             x[0], x[1], x[2]);

    int rc = sqlite3_exec(db, insertDataSQL, 0, 0, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

std::string data_base::read_table_temp_press()
{
    const char *query = "SELECT * FROM TEMPERATURE_PRESSURE_DATA";
    std::stringstream temp_pressure;
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            double temperature = sqlite3_column_double(stmt, 1);
            double pressure = sqlite3_column_double(stmt, 2);
            const char *timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

            std::cout << "ID: " << id << ", Timestamp: " << timestamp << ", Temperature: " << temperature << ", Pressure: " << pressure << std::endl;
            temp_pressure << "ID: " << id << ", Timestamp: " << timestamp << ", Temperature: " << temperature << ", Pressure: " << pressure << "\n";
        }
    }
    else
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }
    return temp_pressure.str();
}

std::string data_base::read_table_magnetometr()
{
    const char *query = "SELECT * FROM MAGNETOMETR_DATA";
    std::stringstream magnetometr;
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            double x = sqlite3_column_double(stmt, 1);
            double y = sqlite3_column_double(stmt, 2);
            double z = sqlite3_column_double(stmt, 3);
            const char *timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));

            std::cout << "ID: " << id << ", Timestamp: " << timestamp << ", x: " << x << ", y: " << y << ", z: " << z << std::endl;
            magnetometr << "ID: " << id << ", Timestamp: " << timestamp << ", x: " << x << ", y: " << y << ", z: " << z << "\n";
        }
        sqlite3_finalize(stmt); // Clean up the prepared statement
    }
    else
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }
    return magnetometr.str();
}

void data_base::clear_table_temp_press()
{
    char *errorMessage = nullptr;
    int rc = sqlite3_open("temperature_pressure.db", &db);
    const char *sql = "DELETE FROM TEMPERATURE_PRESSURE_DATA;";

    rc = sqlite3_exec(db, sql, 0, 0, &errorMessage);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else
    {
        const char *sql_update = "UPDATE TEMPERATURE_PRESSURE_DATA SET id = 0;";
        rc = sqlite3_exec(db, sql_update, 0, 0, 0);
        std::cout << "All tables cleared successfully" << std::endl;
    }
}

void data_base::clear_table_temp_press(int from_index, int to_index)
{
    char *errorMessage = nullptr;
    int rc = sqlite3_open("temperature_pressure.db", &db);
    const char *sql = "DELETE FROM TEMPERATURE_PRESSURE_DATA WHERE id BETWEEN ? AND ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, from_index);
    sqlite3_bind_int(stmt, 2, to_index);
    rc = sqlite3_exec(db, sql, 0, 0, &errorMessage);
    rc = sqlite3_step(stmt);
    std::cout << "Tables are clear from " << from_index << " to " << to_index << " ID" << std::endl;
    
    sqlite3_finalize(stmt);
}

void data_base::clear_table_magnetometr()
{
    char *errorMessage = nullptr;
    int rc = sqlite3_open("magnetometr.db", &db);
    const char *sql = "DELETE FROM MAGNETOMETR_DATA;";

    rc = sqlite3_exec(db, sql, 0, 0, &errorMessage);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else
    {
        std::cout << "All tables cleared successfully" << std::endl;
    }
}

void data_base::clear_table_magnetometr(int from_index, int to_index)
{
    char *errorMessage = nullptr;
    int rc = sqlite3_open("magnetometr.db", &db);
    const char *sql = "DELETE FROM MAGNETOMETR_DATA WHERE id BETWEEN ? AND ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, from_index);
    sqlite3_bind_int(stmt, 2, to_index);
    rc = sqlite3_step(stmt);
    std::cout << "Tables are clear from " << from_index << " to " << to_index << " ID" << std::endl;
    
    sqlite3_finalize(stmt);
}
