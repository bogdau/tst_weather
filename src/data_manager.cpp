#include <iostream>
#include <iomanip>
#include <mosquittopp.h>
#include <QTimer>
#include <unistd.h>

#include "include/data_manager.h"
#include "include/data_output.h"
#include "include/data_reading.h"
#include "include/settings.h"
#include "include/mqtt.h"
#include "include/form.h"
#include "include/mki109v1.h"
#include "include/data_base.h"

DataManager::DataManager(Settings &set, mki109v1 &mki) : m_reader(nullptr), settings(set), mki(&mki)
{
    timer = new QTimer(this);
}

void DataManager::class_manager(DataReading *dtrd)
{
    m_reader = std::unique_ptr<DataReading>(dtrd);
}

int DataManager::data_manager()
{
    settings.loadSettings();

    db_temp.open_table_temp_press();
    db_mag.open_table_magnetometr();

    timer->setInterval(settings.getSensorPollIntervalMs());
    connect(timer, &QTimer::timeout, this, &DataManager::colect_data);
    timer->start();
    oled o;
    return 0;
}

void DataManager::colect_data()
{
    DataOutput dtot;
    o.init(m_reader->readTemp(), m_reader->readPressure());

    db_temp.insert_table_temp_press(m_reader->readTemp(), m_reader->readPressure());

    db_mag.insert_table_magnetometr(mki->lis3mdl_read_data_polling());

    static int i = 0;
    if (i == 3)
    {
        settings.loadSettings();
        i = 0;
    }
    i++;
    timer->setInterval(settings.getSensorPollIntervalMs());

    emit tempChange(m_reader->readTemp());
    emit pressChange(m_reader->readPressure());

    mqtt_publish(m_reader->readTemp(), (m_reader->readTemp() * 1.8) + 32, m_reader->readPressure());
    dtot.print_info(m_reader->readTemp(), (m_reader->readTemp() * 1.8) + 32, m_reader->readPressure());
}

void DataManager::command_selector(std::string &data, aws &a)
{
    auto index = std::find(data.begin(), data.end(), ':');
    if (index == data.end())
    {
        std::cout << "Unable to parse string" << data << std::endl;

        a.publish("sdk/tst_weather/pub",
                  "\t\t\t\t---help--- \n"
                  "temperature:C - set temperature measurement to Celsius \n"
                  "temperature:F - set temperature measurement to Fahrenheit \n"
                  "pressure:Pa - set pressure measurement to Pascal \n"
                  "pressure:ATM - set pressure measirement to ATM \n"
                  "time: *interval* - set time interval to custom in milliseconds by default 1000 \n"
                  "-------------------------------------------------------------- \n"
                  "database:clear - clear database column \n"
                  "database:1-10 - delete row by id(example from 1 to 10) \n"
                  "database:load - load database to aws \n");
        return;
    }
    std::string comand(data.begin(), index);
    std::string comand_data(index + 1, data.end());

    int from_number = 0;
    int to_number = 0;

    auto comand_index = std::find(comand_data.begin(), comand_data.end(), '-');
    if (comand_index != comand_data.end())
    {
        std::string from_number_str(comand_data.begin(), comand_index);
        std::string to_number_str(comand_index + 1, comand_data.end());
        from_number = std::stoi(from_number_str);
        to_number = std::stoi(to_number_str);
    }

    Command comand_token = string_to_Comand.find(comand)->second;

    switch (comand_token)
    {
    case Command::SET_TEMPERATURE_UNIT:
        if (comand_data == "C")
        {
            settings.saveTempSettings("°C");
            std::cout << "the temperature measurement is set to Celsius" << std::endl;
        }
        else if (comand_data == "F")
        {
            settings.saveTempSettings("°F");
            std::cout << "the temperature measurement is set to Fahrenheit" << std::endl;
        }
        else
        {
            std::cout << "UNRECOGNIZE" << std::endl;
        }
        break;
    case Command::SET_PRESSURE_UNIT:
        if (comand_data == "Pa")
        {
            settings.savePressSettings("Pa");
            std::cout << "the pressure measurement is set to Pascal" << std::endl;
        }
        else if (comand_data == "ATM")
        {
            settings.savePressSettings("ATM");
            std::cout << "the pressure measurement is set to ATM" << std::endl;
        }
        else
        {
            std::cout << "UNRECOGNIZE" << std::endl;
        }
        break;
    case Command::SET_TIME_INTERVAL:
        settings.saveTimeSettings(std::stoi(comand_data));
        std::cout << "the time interval is set " << std::stoi(comand_data) << std::endl;
        break;
    case Command::DATABASE:
        if (comand_data == "clear")
        {
            db_temp.clear_table_temp_press();
            db_mag.clear_table_magnetometr();
        }
        else if (from_number > 0 && to_number > 0)
        {
            db_temp.clear_table_temp_press(from_number, to_number);
            db_mag.clear_table_magnetometr(from_number, to_number);
        }
        else if (comand_data == "load")
        {
            a.publish("sdk/tst_weather/pub", "Tempreture and Pressure:\n" + db_temp.read_table_temp_press());
            a.publish("sdk/tst_weather/pub", "Magnetometr:\n" + db_mag.read_table_magnetometr());
        }
        else
        {
            std::cout << "UNRECOGNIZE" << std::endl;
        }

        break;
    default:
        a.publish("sdk/tst_weather/pub",
                  "\t\t\t\t---help--- \n"
                  "temperature:C - set temperature measurement to Celsius \n"
                  "temperature:F - set temperature measurement to Fahrenheit \n"
                  "pressure:Pa - set pressure measurement to Pascal \n"
                  "pressure:ATM - set pressure measirement to ATM \n"
                  "time: *interval* - set time interval to custom in milliseconds by default 1000 \n"
                  "-------------------------------------------------------------- \n"
                  "database:clear - clear database column \n"
                  "database:1-10 - delete row by id(example from 1 to 10) \n"
                  "database:load - load database to aws \n");
        break;
    }
}

void DataManager::mqtt_publish(double temp_cel, double temp_far, double pressure)
{
    class mqtt_client *iot_client;
    int rc;

    std::stringstream mqtt_string_builder;
    mqtt_string_builder << "Temperature: " << std::fixed << std::setprecision(2) << temp_cel << " °C, " << temp_far << " °F "
                        << " Pressure: " << int(pressure) << " Pa";
    ;

    std::stringstream mqtt_string_raw_data;
    mqtt_string_raw_data << std::fixed << std::setprecision(2) << temp_cel << " " << temp_far << " " << int(pressure);

    std::string client_id = "user";
    std::string host = "localhost";
    const int port = 1883;
    std::string MQTT_TOPIC = "Data Output";

    int mid_val{4};
    int *mid{&mid_val};
    std::string topic_val = MQTT_TOPIC;
    std::string topic = topic_val;
    std::string topic_raw = "Raw";
    std::string payload = mqtt_string_builder.str();
    std::string payload_raw_data = mqtt_string_raw_data.str();

    int qos{1};
    bool retain{true};

    mosqpp::lib_init();

    iot_client = new mqtt_client(client_id.c_str(), host.c_str(), port);

    rc = iot_client->loop();
    if (rc)
    {
        iot_client->reconnect();
    }
    else
    {
        iot_client->subscribe(NULL, MQTT_TOPIC.c_str());
        int publish_raw_data = iot_client->publish(mid, topic_raw.c_str(), payload_raw_data.size(), payload_raw_data.c_str(), qos, retain);
        int publish_full_data_status = iot_client->publish(mid, topic.c_str(), payload.size(), payload.c_str(), qos, retain);
        if (publish_full_data_status != MOSQ_ERR_SUCCESS or publish_raw_data != MOSQ_ERR_SUCCESS)
        {
            std::cout << "Error: unable to publish message to Cumulocity MQTT broker.";
        }
    }

    mosqpp::lib_cleanup();
}
