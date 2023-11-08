#ifndef AWS_H
#define AWS_H

#include <aws/crt/Api.h>
#include <aws/crt/StlAllocator.h>
#include <aws/crt/auth/Credentials.h>
#include <aws/crt/io/TlsOptions.h>

#include <aws/iot/MqttClient.h>

#include <algorithm>
#include <aws/crt/UUID.h>
#include <chrono>
#include <mutex>
#include <thread>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "utils/CommandLineUtils.h"

using namespace Aws::Crt;

class aws
{
private:
    void onConnectionCompleted(Aws::Crt::Mqtt::MqttConnection &, int errorCode, Aws::Crt::Mqtt::ReturnCode returnCode, bool);
    void onInterrupted(Aws::Crt::Mqtt::MqttConnection &, int error);
    void onResumed(Aws::Crt::Mqtt::MqttConnection &, Aws::Crt::Mqtt::ReturnCode, bool);
    void onDisconnect(Aws::Crt::Mqtt::MqttConnection &);
    // Utils::cmdData cmdData;
    // ApiHandle apiHandle;
    std::promise<bool> connectionCompletedPromise;
    std::promise<void> connectionClosedPromise;
    std::shared_ptr<Aws::Crt::Mqtt::MqttConnection> connection;
public:
    aws();
    // int aws_start(int, char*[]);
    // void mqtt_connect();
    // void mqtt_builder();
    // void ReceivedMessage();
    // void subscribeToTopic();
    void connect();
    void subscribe(std::string, std::string);
};

#endif // AWS_H
