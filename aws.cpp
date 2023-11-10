#include "aws.h"
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include "include/settings.h"
aws::aws()
{
}

void aws::connect()
{
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
    if (!connection->Connect("basicPubSub", false , 1000 ))
    {
        fprintf(stderr, "MQTT Connection failed with error %s\n", ErrorDebugString(connection->LastError()));
        exit(-1);
    }
}

void aws::onConnectionCompleted(Aws::Crt::Mqtt::MqttConnection &, int errorCode, Aws::Crt::Mqtt::ReturnCode returnCode, bool)
{
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

void aws::subscribe(std::string topic, std::function<void(std::string)> callBack)
{
    if (connectionCompletedPromise.get_future().get())
    {
        auto onMessage = [&](Mqtt::MqttConnection &,
                             const String &topic,
                             const ByteBuf &byteBuf,
                             bool,
                             Mqtt::QOS,
                             bool)
        {
            fprintf(stdout, "Message: ");
            fwrite(byteBuf.buffer, 1, byteBuf.len, stdout);
            fprintf(stdout, "\n");
            callBack(std::string((const char *)byteBuf.buffer));
        };
        auto onSubAck =
            [&](Mqtt::MqttConnection &, uint16_t packetId, const String &topic, Mqtt::QOS QoS, int errorCode)
        {
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
    }
}

void aws::publish(std::string topic, std::string messagePayload)
{
    ByteBuf payload = ByteBufFromArray((const uint8_t *)messagePayload.data(), messagePayload.length());

    auto onPublishComplete = [](Mqtt::MqttConnection &, uint16_t, int) {};
    connection->Publish(topic.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, false, payload, onPublishComplete);
}
