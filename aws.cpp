#include "aws.h"
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
aws::aws()
{
}

// int aws::aws_start(int argc, char *argv[])
// {
//     /************************ Setup ****************************/

//     // Do the global initialization for the API.
//     ApiHandle apiHandle;

//     /**
//      * cmdData is the arguments/input from the command line placed into a single struct for
//      * use in this sample. This handles all of the command line parsing, validating, etc.
//      * See the Utils/CommandLineUtils for more information.
//      */
//     cmdData = Utils::parseSampleInputPubSub(argc, argv, &apiHandle, "basic-pubsub");
//     String messagePayload = "\" work \"";
//     // Create the MQTT builder and populate it with data from cmdData.
   
//     // Create the MQTT connection from the MQTT builder

//     /**
//      * In a real world application you probably don't want to enforce synchronous behavior
//      * but this is a sample console application, so we'll just do that with a condition variable.
//      */

//     auto clientConfigBuilder =
//         Aws::Iot::MqttClientConnectionConfigBuilder(cmdData.input_cert.c_str(), cmdData.input_key.c_str());
//     auto clientConfig = clientConfigBuilder.Build();
//      Aws::Iot::MqttClient client = Aws::Iot::MqttClient();
//     auto connection = client.NewConnection(clientConfig);

//     // Invoked when a MQTT connect has completed or failed
//     auto onConnectionCompleted =
//         [&](Aws::Crt::Mqtt::MqttConnection &, int errorCode, Aws::Crt::Mqtt::ReturnCode returnCode, bool)
//     {
//         if (errorCode)
//         {
//             fprintf(stdout, "Connection failed with error %s\n", Aws::Crt::ErrorDebugString(errorCode));
//             connectionCompletedPromise.set_value(false);
//         }
//         else
//         {
//             fprintf(stdout, "Connection completed with return code %d\n", returnCode);
//             connectionCompletedPromise.set_value(true);
//         }
//     };

//     // Invoked when a MQTT connection was interrupted/lost
//     auto onInterrupted = [&](Aws::Crt::Mqtt::MqttConnection &, int error)
//     {
//         fprintf(stdout, "Connection interrupted with error %s\n", Aws::Crt::ErrorDebugString(error));
//     };

//     // Invoked when a MQTT connection was interrupted/lost, but then reconnected successfully
//     auto onResumed = [&](Aws::Crt::Mqtt::MqttConnection &, Aws::Crt::Mqtt::ReturnCode, bool)
//     {
//         fprintf(stdout, "Connection resumed\n");
//     };

//     // Invoked when a disconnect message has completed.
//     auto onDisconnect = [&](Aws::Crt::Mqtt::MqttConnection &)
//     {
//         fprintf(stdout, "Disconnect completed\n");
//         connectionClosedPromise.set_value();
//     };
//     // Assign callbacks
//     connection->OnConnectionCompleted = std::move(onConnectionCompleted);
//     connection->OnDisconnect = std::move(onDisconnect);
//     connection->OnConnectionInterrupted = std::move(onInterrupted);
//     connection->OnConnectionResumed = std::move(onResumed);

//     /************************ Run the sample ****************************/

//     // Connect
//     fprintf(stdout, "Connecting...\n");
//     if (!connection->Connect(cmdData.input_clientId.c_str(), false /*cleanSession*/, 1000 /*keepAliveTimeSecs*/))
//     {
//         fprintf(stderr, "MQTT Connection failed with error %s\n", ErrorDebugString(connection->LastError()));
//         exit(-1);
//     }

//     if (connectionCompletedPromise.get_future().get())
//     {
//         uint32_t receivedCount = 0;

//         // This is invoked upon the receipt of a Publish on a subscribed topic.
//         auto onMessage = [&](Mqtt::MqttConnection &,
//                              const String &topic,
//                              const ByteBuf &byteBuf,
//                              bool /*dup*/,
//                              Mqtt::QOS /*qos*/,
//                              bool /*retain*/)
//         {
//             {
//                 ++receivedCount;
//                 fprintf(stdout, "Publish #%d received on topic %s\n", receivedCount, topic.c_str());
//                 fprintf(stdout, "Message: ");
//                 fwrite(byteBuf.buffer, 1, byteBuf.len, stdout);

//                 // std::fstream json_file;
//                 // json_file.open("/home/pi/build-tst_weather_ui-Desktop-Debug/config/config.json", std::ofstream::out | std::ofstream::trunc);
//                 // json_file << byteBuf.buffer;
//                 // json_file.close();
//                 fprintf(stdout, "\n");
//             }
//         };

//         // Subscribe for incoming publish messages on topic.
//         std::promise<void> subscribeFinishedPromise;
//         auto onSubAck =
//             [&](Mqtt::MqttConnection &, uint16_t packetId, const String &topic, Mqtt::QOS QoS, int errorCode)
//         {
//             if (errorCode)
//             {
//                 fprintf(stderr, "Subscribe failed with error %s\n", aws_error_debug_str(errorCode));
//                 exit(-1);
//             }
//             else
//             {
//                 if (!packetId || QoS == AWS_MQTT_QOS_FAILURE)
//                 {
//                     fprintf(stderr, "Subscribe rejected by the broker.");
//                     exit(-1);
//                 }
//                 else
//                 {
//                     fprintf(stdout, "Subscribe on topic %s on packetId %d Succeeded\n", topic.c_str(), packetId);
//                 }
//             }
//             subscribeFinishedPromise.set_value();
//         };

//         connection->Subscribe(cmdData.input_topic.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, onMessage, onSubAck);
//         subscribeFinishedPromise.get_future().wait();

//         // uint32_t publishedCount = 0;
//         // while (publishedCount < cmdData.input_count)
//         // {
//         //     ByteBuf payload = ByteBufFromArray((const uint8_t *)messagePayload.data(), messagePayload.length());

//         //     auto onPublishComplete = [](Mqtt::MqttConnection &, uint16_t, int) {};
//         //     connection->Publish(
//         //         cmdData.input_topic.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, false, payload, onPublishComplete);
//         //     ++publishedCount;

//         //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//         // }

//         // Unsubscribe from the topic.
//         // std::promise<void> unsubscribeFinishedPromise;
//         // connection->Unsubscribe(cmdData.input_topic.c_str(), [&](Mqtt::MqttConnection &, uint16_t, int) {
//         //     unsubscribeFinishedPromise.set_value();
//         // });
//         // unsubscribeFinishedPromise.get_future().wait();

//         // // Disconnect
//         // if (connection->Disconnect())
//         // {
//         //     connectionClosedPromise.get_future().wait();
//         // }
//     }
//     else
//     {
//         exit(-1);
//     }
//     return 0;
// }

// void aws::mqtt_builder(){
//     auto clientConfigBuilder =
//         Aws::Iot::MqttClientConnectionConfigBuilder(cmdData.input_cert.c_str(), cmdData.input_key.c_str());
//     clientConfigBuilder.WithEndpoint(cmdData.input_endpoint);
//     if (cmdData.input_ca != "")
//     {
//         clientConfigBuilder.WithCertificateAuthority(cmdData.input_ca.c_str());
//     }
//     if (cmdData.input_proxyHost != "")
//     {
//         Aws::Crt::Http::HttpClientConnectionProxyOptions proxyOptions;
//         proxyOptions.HostName = cmdData.input_proxyHost;
//         proxyOptions.Port = static_cast<uint16_t>(cmdData.input_proxyPort);
//         proxyOptions.AuthType = Aws::Crt::Http::AwsHttpProxyAuthenticationType::None;
//         clientConfigBuilder.WithHttpProxyOptions(proxyOptions);
//     }
//     if (cmdData.input_port != 0)
//     {
//         clientConfigBuilder.WithPortOverride(static_cast<uint16_t>(cmdData.input_port));
//     }
// }

// void aws::mqtt_connect(){
//      auto clientConfigBuilder =
//         Aws::Iot::MqttClientConnectionConfigBuilder(cmdData.input_cert.c_str(), cmdData.input_key.c_str());
//     auto clientConfig = clientConfigBuilder.Build();
//     if (!clientConfig)
//     {
//         fprintf(
//             stderr,
//             "Client Configuration initialization failed with error %s\n",
//             Aws::Crt::ErrorDebugString(clientConfig.LastError()));
//         exit(-1);
//     }
//     Aws::Iot::MqttClient client = Aws::Iot::MqttClient();
//     auto connection = client.NewConnection(clientConfig);
//     if (!*connection)
//     {
//         fprintf(
//             stderr,
//             "MQTT Connection Creation failed with error %s\n",
//             Aws::Crt::ErrorDebugString(connection->LastError()));
//         exit(-1);
//     }
// }


void aws::connect(int argc, char *argv[]){
    std::ifstream root_ca_file("credentials/root-CA.crt", std::ios::binary);
    std::stringstream root_ca;
    root_ca << root_ca_file.rdbuf();
      /************************ Setup ****************************/

    // Do the global initialization for the API.
    ApiHandle apiHandle;

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
    auto connection = client.NewConnection(clientConfig);
    if (!*connection)
    {
        fprintf(
            stderr,
            "MQTT Connection Creation failed with error %s\n",
            Aws::Crt::ErrorDebugString(connection->LastError()));
        exit(-1);
    }

    /**
     * In a real world application you probably don't want to enforce synchronous behavior
     * but this is a sample console application, so we'll just do that with a condition variable.
     */
    std::promise<bool> connectionCompletedPromise;
    std::promise<void> connectionClosedPromise;

    // Invoked when a MQTT connect has completed or failed
    // auto onConnectionCompleted =
    //     [&](Aws::Crt::Mqtt::MqttConnection &, int errorCode, Aws::Crt::Mqtt::ReturnCode returnCode, bool) {
    //         if (errorCode)
    //         {
    //             fprintf(stdout, "Connection failed with error %s\n", Aws::Crt::ErrorDebugString(errorCode));
    //             connectionCompletedPromise.set_value(false);
    //         }
    //         else
    //         {
    //             fprintf(stdout, "Connection completed with return code %d\n", returnCode);
    //             connectionCompletedPromise.set_value(true);
    //         }
    //     };

    // Invoked when a MQTT connection was interrupted/lost
    auto onInterrupted = [&](Aws::Crt::Mqtt::MqttConnection &, int error) {
        fprintf(stdout, "Connection interrupted with error %s\n", Aws::Crt::ErrorDebugString(error));
    };

    // Invoked when a MQTT connection was interrupted/lost, but then reconnected successfully
    auto onResumed = [&](Aws::Crt::Mqtt::MqttConnection &, Aws::Crt::Mqtt::ReturnCode, bool) {
        fprintf(stdout, "Connection resumed\n");
    };

    // Invoked when a disconnect message has completed.
    auto onDisconnect = [&](Aws::Crt::Mqtt::MqttConnection &) {
        fprintf(stdout, "Disconnect completed\n");
        connectionClosedPromise.set_value();
    };

    // Assign callbacks

    connection->OnConnectionCompleted = std::bind(&aws::onConnectionCompleted, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
    connection->OnDisconnect = std::move(onDisconnect);
    connection->OnConnectionInterrupted = std::move(onInterrupted);
    connection->OnConnectionResumed = std::move(onResumed);

}

void aws::onConnectionCompleted(Aws::Crt::Mqtt::MqttConnection &, int errorCode, Aws::Crt::Mqtt::ReturnCode returnCode, bool){
     if (errorCode)
            {
                fprintf(stdout, "Connection failed with error %s\n", Aws::Crt::ErrorDebugString(errorCode));

            }
            else
            {
                fprintf(stdout, "Connection completed with return code %d\n", returnCode);
            
            }
}
