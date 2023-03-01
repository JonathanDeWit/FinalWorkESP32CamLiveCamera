#ifndef ApiRequest_h
#define ApiRequest_h

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <StatusObject.h>

class ApiRequest {
private:
    static const String apiUrl;
    static const String cameraPath;
    static const String userName;
    static const String apiPassword;

public:
    static String helloWorld();
    static String login();
    static StatusObject getSystemState(String jwtToken);
    static boolean updateTransmitVideoStream(bool transmitVideoStream);
    static boolean updateLocalIp();
};

#endif