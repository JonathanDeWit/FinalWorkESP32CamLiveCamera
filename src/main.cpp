#include "main.h"

#include <WiFi.h>
#include "wifiConfig.h"


OV2640 cam;

String jwt;
StatusObject status(false, false, false);

// void apiRequestTask(void * parameter) {
//   // Call the API request function and store the JWT token in a global variable
//   jwt = ApiRequest::login();
//   // Delete the task when it is done
//   vTaskDelete(NULL);
// }

void setup() {
  //Start serial connection
  Serial.begin(115200);

  Serial.println("****ESP32-cam starded****");

  //Initialize camera
	cam.init(esp32cam_aithinker_config);
	
  //Connect to Wifi
  WiFi.begin(ssid, password);
  Serial.println("Try to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to the WiFi network");
  Serial.println("IP address: ");
  CameraUser::LocalIp = WiFi.localIP().toString();
  Serial.println(CameraUser::LocalIp);
  ApiRequest::updateLocalIp();
  
  
  //Try to login and get JWT token
  Serial.println("Api request:");
  CameraUser::JwtToken = ApiRequest::login();
  if(jwt.compareTo(""))
  {
    Serial.println("Jwt token: "+CameraUser::JwtToken);
  }else{
    Serial.print("HTTP Error");
  }
  //xTaskCreate(apiRequestTask,"ApiRequestTask", 4096, NULL, 2, NULL);

  
  status = ApiRequest::getSystemState(CameraUser::JwtToken);

  if(status.requestSucseed){
    startRTSPserver = status.sysState;
  }else{
    Serial.println("RTSP server off");
  }
  if (status.sysState && status.requestSucseed ){
      initRTSPServer();
      Serial.print("Stream Link: rtsp://");
      Serial.print(WiFi.localIP());
      Serial.println(":8554/mjpeg/1\n");

      startRTSPserver = false;
  }

  CameraUser::HubReceiveVideoStream = true;
	ApiRequest::updateTransmitVideoStream(CameraUser::HubReceiveVideoStream);

  Serial.println("---Startup Complete---");
}

long begTime = clock();
int temp = 10000;
int delayTime = 10000;

void loop() {

  if((clock() - begTime) > temp){
    Serial.println(temp);
    temp += 10000;
    if (temp == 40000)
    {
       temp = 10000;
    }
    
  }


  if((clock() - begTime) > delayTime){
    status = ApiRequest::getSystemState(CameraUser::JwtToken);

    if(status.requestSucseed){
      startRTSPserver = status.sysState;
    }

    //Check if the camera needs to start the live stream
    if (status.sysState && status.requestSucseed && status.sysState != runningRTSPserver){
      initRTSPServer();
      delayTime = 30000;
      Serial.print("Stream Link: rtsp://");
      Serial.print(WiFi.localIP());
      Serial.println(":8554/mjpeg/1\n");
      startRTSPserver = false;
    }
    //Check if the camera needs to stop the live stream
    else if(!status.sysState && status.requestSucseed && status.sysState != runningRTSPserver){
      Serial.println("End Server");
      stopRTSPServer();
      delayTime = 10000;
    }
    //Print the current state
    else{
      if (runningRTSPserver)
      {
        Serial.println("Server is running");
        Serial.print("Stream Link: rtsp://");
        Serial.print(WiFi.localIP());
        Serial.println(":8554/mjpeg/1\n");
      }
      else{
        Serial.println("Server is not running");
      }

      if(CameraUser::LocalIp.compareTo(WiFi.localIP().toString())){
        CameraUser::LocalIp = WiFi.localIP().toString();
        ApiRequest::updateLocalIp();
      }
    }

    begTime = clock();
  }
}