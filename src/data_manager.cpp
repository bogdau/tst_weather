#include <iostream>
#include <iomanip>
#include <mosquittopp.h>

#include "data_manager.h"
#include "data_output.h"
#include "data_reading.h"
#include "settings.h"
#include "mqtt.h"

int DataManager::data_manager(){ 
    BMP280 bmp;
    
    uint8_t ret = bmp.init(BMP280::CSBState::High, BMP280_FILTER_COEFF_16, BMP280_OS_16X, BMP280_OS_2X, BMP280_ODR_62_5_MS);
    if (ret != 0)
    {
        return -1;
    }

    DataReading dtrd(bmp);
    DataOutput dtot;
    Settings settings;
    settings.loadSettings("../config/config.json");

    int i = 0;
    while(true){
        if(i == 19){
            settings.loadSettings("../config/config.json");
            i = 0;
        }

        if(bmp.read() != 0){
            std::cerr << "can't read data" << std::endl;
            bmp.delay_ms(120);
        }
        dtrd.dataRefresh();
        mqtt_publish(dtrd.getTempCels(),dtrd.getTempFahr(),dtrd.getPressure() );
        dtot.print_info(dtrd.getTempCels(),dtrd.getTempFahr(),dtrd.getPressure() );

        i++;
        bmp.delay_ms(settings.getSensorPollIntervalMs());
    }

    return 0;
}


void DataManager::mqtt_publish(double temp_cel, double temp_far, double pressure){
    class mqtt_client *iot_client;
    int rc;  

    std::stringstream mqtt_string_builder;
    mqtt_string_builder  << "Temperature: " <<  std::fixed << std::setprecision(2) << temp_cel<<" ℃, "<< temp_far  <<   " ℉ "  << " Pressure: " <<  int(pressure) << " Pa" ;;

    std::string client_id = "user";
    std::string  host  = "localhost";
    int port = 1883;
    std::string MQTT_TOPIC = "Test";

    int mid_val {4};
	int *mid {&mid_val};
	std::string  topic_val = "Test";
	std::string topic = topic_val;
	std::string payload = mqtt_string_builder.str();

	int qos {1};
	bool retain {true};

    mosqpp::lib_init();

    iot_client = new mqtt_client(client_id.c_str(), host.c_str(), port);    


    rc = iot_client->loop();
    if (rc)
    {
        iot_client->reconnect();
    }
    else
        iot_client->subscribe(NULL, MQTT_TOPIC.c_str());

        int publish_status = iot_client->publish(mid, topic.c_str(), payload.size(), payload.c_str(), qos, retain);
        if(publish_status != MOSQ_ERR_SUCCESS){
            std::cout << "Error: unable to publish message to Cumulocity MQTT broker.";
    }

    mosqpp::lib_cleanup();
}