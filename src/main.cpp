#include "main.h"


#include <WiFi.h>
#include "wifiConfig.h"


OV2640 cam;


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
  Serial.println(WiFi.localIP());

  Serial.print("Stream Link: rtsp://");
	Serial.print(WiFi.localIP());
	Serial.println(":8554/mjpeg/1\n");

  

  startRTSPserver = true;

}

long begTime = clock();

void loop() {
  if (startRTSPserver == true){
    initRTSPServer();
    startRTSPserver = false;
  }
  // else if((clock() - begTime) > 60000){
  //   Serial.println("RTSP Server off");
  //   stopRTSP();
  // }
}