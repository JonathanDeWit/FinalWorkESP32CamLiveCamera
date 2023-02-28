#ifndef CameraUser_h
#define CameraUser_h

#include <Arduino.h>

class CameraUser {
private:

public:
    static String JwtToken;
    static String LocalIp;
    static bool TransmitVideoStream;
    static bool HubReceiveVideoStream;
    static String DeviceName;
    static String DeviceLocation;
    static long JwtTime;
    static String helloWorld();
};

#endif