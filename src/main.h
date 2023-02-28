#include <Arduino.h>

#include <ArduinoJson.h>
#include "StatusObject.h"
#include "ApiRequest.h"
#include "CameraUser.h"

//Camera includes
#include "OV2640.h"
#include "OV2640Streamer.h"
#include "CRtspSession.h"



// Select which of the servers are active
// Select only one or the streaming will be very slow!
// #define ENABLE_WEBSERVER
#define ENABLE_RTSPSERVER

//Camera class
extern OV2640 cam;




//RTSP functions
void initRTSPServer(void);
void stopRTSPServer(void);


//boolean startRTSPserver;
extern boolean stopRTSPserver;
extern boolean startRTSPserver;
extern boolean runningRTSPserver;