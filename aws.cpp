#include "aws.h"
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include "include/settings.h"
aws::aws()
{
}

void aws::connect(){
    std::ifstream root_ca_file("credentials/root-CA.crt", std::ios::binary);
    std::stringstream root_ca;
    root_ca << root_ca_file.rdbuf();
      /************************ Setup ****************************/

    // Do the global initialization for the API.


    /**
     * cmdData is the arguments/input from the command line placed into a single struct for
     * use in this sample. This handles all of the command line parsing, validating, etc.
     * See the Utils/CommandLineUtils for more information.
     */

    // Create the MQTT builder and populate it with data from cmdData.
    auto clientConfigBuilder = Aws::Iot::MqttClientConnectionConfigBuilder("credentials/tst-weather.cert.pem", "credentials/tst-weather.private.key");
    clientConfigBuilder.WithEndpoint("a2c27i8vurj003-ats.iot.eu-north-1.amazonaws.com");
    clientConfigBuilder.WithCertificateAuthority("credentials/root-CA.crt");

    // Create the MQTT connection from the MQTT builder
    auto clientConfig = clientConfigBuilder.Build();
    if (!clientConfig)
    {
        fprintf(
            stderr,
            "Client Configuration initialization failed with error %s\n",
            Aws::Crt::ErrorDebugString(clientConfig.LastError()));
        exit(-1);
    }
    Aws::Iot::MqttClient client = Aws::Iot::MqttClient();
    connection = client.NewConnection(clientConfig);
    if (!*connection)
    {
        fprintf(
            stderr,
            "MQTT Connection Creation failed with error %s\n",
            Aws::Crt::ErrorDebugString(connection->LastError()));
        exit(-1);
    }
    
    connection->OnConnectionCompleted = 
        std::bind(&aws::onConnectionCompleted, (this), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    //connection->OnDisconnect = std::bind(&aws::onDisconnect,std::placeholders::_1);
    connection->OnConnectionInterrupted = std::bind(&aws::onInterrupted,this,std::placeholders::_1,std::placeholders::_2);
    // connection->OnConnectionResumed = std::bind(&aws::onResumed,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
      if (!connection->Connect("basicPubSub", false /*cleanSession*/, 1000 /*keepAliveTimeSecs*/))
    {
        fprintf(stderr, "MQTT Connection failed with error %s\n", ErrorDebugString(connection->LastError()));
        exit(-1);
    }
}

void aws::onConnectionCompleted(Aws::Crt::Mqtt::MqttConnection &, int errorCode, Aws::Crt::Mqtt::ReturnCode returnCode, bool){
     if (errorCode)
            {
                fprintf(stdout, "Connection failed with error %s\n", Aws::Crt::ErrorDebugString(errorCode));
                connectionCompletedPromise.set_value(false);
            }
            else
            {
                fprintf(stdout, "Connection completed with return code %d\n", returnCode);
                connectionCompletedPromise.set_value(true);
            }
}

void onResumed(Aws::Crt::Mqtt::MqttConnection &, Aws::Crt::Mqtt::ReturnCode, bool){
    fprintf(stdout, "Connection resumed\n");
 }

void aws::onInterrupted(Aws::Crt::Mqtt::MqttConnection &, int error){
    fprintf(stdout, "Connection interrupted with error %s\n", Aws::Crt::ErrorDebugString(error));
}

void aws::onDisconnect(Aws::Crt::Mqtt::MqttConnection &){
    fprintf(stdout, "Disconnect completed\n");
    connectionClosedPromise.set_value();
}


void aws::subscribe(std::string topic,std::function<void(std::string)> callBack){
    if (connectionCompletedPromise.get_future().get())
    {
        std::stringstream input_command;
        // std::mutex receiveMutex;
        uint32_t receivedCount = 0;
        // This is invoked upon the receipt of a Publish on a subscribed topic.
        auto onMessage = [&](Mqtt::MqttConnection &,
                             const String &topic,
                             const ByteBuf &byteBuf,
                             bool /*dup*/,
                             Mqtt::QOS /*qos*/,
                             bool /*retain*/) {
            {
                // // std::lock_guard<std::mutex> lock(receiveMutex);
                // ++receivedCount;
                fprintf(stdout, "received from topic %s\n", topic.c_str());
                fprintf(stdout, "Message: ");
                fwrite(byteBuf.buffer, 1, byteBuf.len, stdout);
                fprintf(stdout, "\n");
                // std::string temp_input_data(byteBuf.buffer,byteBuf.len);
                std::cout << "TRACE _---_" << std::endl;
//                input_command << byteBuf.buffer;
                std::cout << "TRACE ^^^" << std::endl;
                // command_selector();
                input_command.str("");
                callBack(std::string((const char*)byteBuf.buffer));
            }
        };
        auto onSubAck =
            [&](Mqtt::MqttConnection &, uint16_t packetId, const String &topic, Mqtt::QOS QoS, int errorCode) {
                if (errorCode)
                {
                    fprintf(stderr, "Subscribe failed with error %s\n", aws_error_debug_str(errorCode));
                    exit(-1);
                }
                else
                {
                    if (!packetId || QoS == AWS_MQTT_QOS_FAILURE)
                    {
                        fprintf(stderr, "Subscribe rejected by the broker.");
                        exit(-1);
                    }
                    else
                    {
                        fprintf(stdout, "Subscribe on topic %s on packetId %d Succeeded\n", topic.c_str(), packetId);
                    }
                }
            };
        
        connection->Subscribe(topic.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, onMessage, onSubAck);
        // Subscribe for incoming publish messages on topic.
        // std::promise<void> subscribeFinishedPromise;
    }
}

//void aws::command_selector(){
//    Settings s;
//    if(input_command.str() == "temperature:C"){
//        std::cout << "TEMPERATURE C SET" << std::endl;
//        s.saveTempSettings("°C");
//    }
//    else if(input_command.str() == "temperature:F"){
//        s.saveTempSettings("°F");
//    }
//    else if(input_command.str() == "pressure:Pa"){
//        s.savePressSettings("Pa");
//    }
//    else if(input_command.str() == "pressure:ATM"){
//        s.savePressSettings("ATM");
//    }
//    else{
//        std::cout << "unrecognize" << std::endl;
//    }
//}
