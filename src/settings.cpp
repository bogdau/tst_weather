#include "include/settings.h"
#include <iostream>
#include <filesystem>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sqlite3.h>
#include "include/data_base.h"

Settings::Settings()
{
    struct stat sb;
    if (stat(dir.c_str(), &sb) != 0)
    {
        mkdir(dir.c_str(), 0777);
    }

    int rc = sqlite3_open(config_file.c_str(), &db);
    if (rc)
    {
        std::cerr << "Cannot open SQLite database: " << sqlite3_errmsg(db) << std::endl;
    }

    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS settings (id INTEGER PRIMARY KEY AUTOINCREMENT, data_output_time INTEGER, temperature TEXT, pressure TEXT);";
    rc = sqlite3_exec(db, createTableSQL, 0, 0, 0);

    if (rc)
    {
        std::cerr << "Cannot create settings table: " << sqlite3_errmsg(db) << std::endl;
    }
    loadSettings();
}

Settings::~Settings()
{
    sqlite3_close(db);
}

void Settings::loadSettings()
{
    const char *query = "SELECT * FROM settings;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
        // Handle the error as needed
    }

    // If no records are found, insert default values
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        const char *insertDefaultValuesSQL = "INSERT INTO settings (data_output_time, temperature, pressure) VALUES (1000, \"C\", \"Pa\");";
        rc = sqlite3_exec(db, insertDefaultValuesSQL, 0, 0, 0);
        if (rc)
        {
            std::cerr << "Failed to insert default values: " << sqlite3_errmsg(db) << std::endl;
        }
    }
    else
    {
        // Load values from the database
        time = sqlite3_column_int(stmt, 1);

        // Check if the column value is NULL before using it
        const char *temperature_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        temperature_unit = (temperature_text) ? std::string(temperature_text) : "C";

        // Check if the column value is NULL before using it
        const char *pressure_text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        pressure_unit = (pressure_text) ? std::string(pressure_text) : "Pa";
    }
}

void Settings::saveTempSettings(std::string temp_unit)
{
    const char *query = "UPDATE settings SET temperature = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }

    rc = sqlite3_bind_text(stmt, 1, temp_unit.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

void Settings::savePressSettings(std::string press_unit)
{
    const char *query = "UPDATE settings SET pressure = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
        // Handle the error as needed
    }

    rc = sqlite3_bind_text(stmt, 1, press_unit.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

void Settings::saveTimeSettings(int time_update)
{
    const char *query = "UPDATE settings SET data_output_time = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    }

    rc = sqlite3_bind_int(stmt, 1, time_update);

    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

int Settings::getSensorPollIntervalMs()
{
    return int(time);
}
