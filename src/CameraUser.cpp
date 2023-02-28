#include "CameraUser.h"

String CameraUser::JwtToken = "";
String CameraUser::LocalIp;
bool CameraUser::TransmitVideoStream = false;
bool CameraUser::HubReceiveVideoStream = false;
String CameraUser::DeviceName = "";
String CameraUser::DeviceLocation;
long CameraUser::JwtTime = 0;
