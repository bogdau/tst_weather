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
#include "form.h"

DataManager::DataManager(Settings &set):m_reader(nullptr),settings(set){
    timer = new QTimer (this);
}

void DataManager::class_manager(DataReading *dtrd){
    m_reader = std::unique_ptr<DataReading>(dtrd);
}

int DataManager::data_manager(){
    settings.loadSettings();

    timer->setInterval(settings.getSensorPollIntervalMs());
    connect(timer, &QTimer::timeout, this, &DataManager::colect_data);
    timer->start();
    return 0;
}


void DataManager::colect_data(){
    DataOutput dtot;

    static int i = 0;
    if(i == 3){
        settings.loadSettings();
        i = 0;
    }
    i++;
    timer->setInterval(settings.getSensorPollIntervalMs());

    emit tempChange(m_reader->readTemp());
    emit pressChange(m_reader->readPressure());

    mqtt_publish(m_reader->readTemp(),(m_reader->readTemp() * 1.8) + 32,m_reader->readPressure());
    dtot.print_info(m_reader->readTemp(),(m_reader->readTemp() * 1.8) + 32,m_reader->readPressure());
}

void DataManager::mqtt_publish(double temp_cel, double temp_far, double pressure){
    class mqtt_client *iot_client;
    int rc;  

    std::stringstream mqtt_string_builder;
    mqtt_string_builder  << "Temperature: " <<  std::fixed << std::setprecision(2) << temp_cel<<" ℃, "<< temp_far  <<   " ℉ "  << " Pressure: " <<  int(pressure) << " Pa" ;;

    std::stringstream mqtt_string_raw_data;
    mqtt_string_raw_data << std::fixed << std::setprecision(2) << temp_cel << " " << temp_far  << " " << int(pressure);

    std::string client_id = "user";
    std::string  host  = "localhost";
    const int port = 1883;
    std::string MQTT_TOPIC = "Data Output";

    int mid_val {4};
	int *mid {&mid_val};
    std::string topic_val = MQTT_TOPIC;
	std::string topic = topic_val;
    std::string topic_raw = "Raw";
	std::string payload = mqtt_string_builder.str();
    std::string payload_raw_data = mqtt_string_raw_data.str();

	int qos {1};
	bool retain {true};

    mosqpp::lib_init();

    iot_client = new mqtt_client(client_id.c_str(), host.c_str(), port);    


    rc = iot_client->loop();
    if (rc)
    {
        iot_client->reconnect();
    }
    else{
        iot_client->subscribe(NULL, MQTT_TOPIC.c_str());
        int publish_raw_data = iot_client->publish(mid, topic_raw.c_str(), payload_raw_data.size(), payload_raw_data.c_str(), qos, retain);
        int publish_full_data_status = iot_client->publish(mid, topic.c_str(), payload.size(), payload.c_str(), qos, retain);
        if(publish_full_data_status != MOSQ_ERR_SUCCESS or publish_raw_data != MOSQ_ERR_SUCCESS){
            std::cout << "Error: unable to publish message to Cumulocity MQTT broker.";
        }
    }

    mosqpp::lib_cleanup();
}
