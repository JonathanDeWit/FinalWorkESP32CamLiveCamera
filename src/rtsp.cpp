#include "main.h"

/*
Inspirerd by
https://github.com/geeksville/Micro-RTSP
https://github.com/circuitrocks/ESP32-RTSP
*/

#ifdef ENABLE_RTSPSERVER

void rtspTask(void *pvParameters);

//RTOS task handle of the all RTSP tasks
TaskHandle_t rtspTaskHandler;

//WiFi server for RTSP 
WiFiServer rtspServer(8554);

//Stream for the camera video
CStreamer *streamer = NULL;

//RTSP Session  communication
CRtspSession *session = NULL;

//Client to handle the RTSP connection
WiFiClient rtspClient;


boolean stopRTSPserver = false;
boolean startRTSPserver = false;
boolean runningRTSPserver = false;
//Start RTSP task
void initRTSPServer(void)
{
	//Create a task for the RTSP server
	xTaskCreate(rtspTask, "RTSP", 4096, NULL, 1, &rtspTaskHandler);

	//Is the RTSP task running?
	if (rtspTaskHandler == NULL)
	{
		Serial.println("Failing to create an RTSP task");
	}
	else
	{
		Serial.println("RTSP task created");
	}
}

//Stop the RTSP task
void stopRTSPServer(void)
{
	stopRTSPserver = true;
}

/**
 * The task that handles RTSP connections
 * Starts the RTSP server
 * Handles requests in an endless loop
 * starts
 */
void rtspTask(void *pvParameters)
{
	uint32_t msecPerFrame = 50;
	static uint32_t lastimage = millis();

	//rtspServer.setNoDelay(true);
	rtspServer.setTimeout(1);
	rtspServer.begin();
	runningRTSPserver = true;
	CameraUser::HubReceiveVideoStream = true;
	ApiRequest::updateTransmitVideoStream(CameraUser::HubReceiveVideoStream);
				
	while (1)
	{
		//Check if we have an active client connection
		if (session)
		{
			session->handleRequests(0); // we don't use a timeout here,
			// instead we send only if we have new enough frames

			uint32_t now = millis();
			if (now > lastimage + msecPerFrame || now < lastimage)
			{ // handle clock rollover
				session->broadcastCurrentFrame(now);
				lastimage = now;
			}

			// Handle disconnection from RTSP client
			if (session->m_stopped)
			{
				Serial.println("RTSP client closed connection");
				delete session;
				delete streamer;
				session = NULL;
				streamer = NULL;
				CameraUser::HubReceiveVideoStream = false;
				ApiRequest::updateTransmitVideoStream(CameraUser::HubReceiveVideoStream);
			}
		}
		//If we dont have a active client connaction it will accept the connection
		else
		{
			rtspClient = rtspServer.accept();
			// Handle connection request from RTSP client
			if (rtspClient)
			{
				Serial.println("RTSP client started connection");
				streamer = new OV2640Streamer(&rtspClient, cam); // our streamer for UDP/TCP based RTP transport

				session = new CRtspSession(&rtspClient, streamer); // our threads RTSP session and state
				delay(100);
			}
		}
		//Check if the RTSP sever need to shut down or not.
		if (stopRTSPserver)
		{
			//Check if there stil is an RTSP client.
			if (rtspClient)
			{
				//If this is the case we will notify the client that the server is stopping
				Serial.println("The ESP32-cam will close the RTSP server your RTSP connection will be closed!");
				delete session;
				delete streamer;
				session = NULL;
				streamer = NULL;
			}

			stopRTSPserver = false;
			runningRTSPserver = false;
			
			//Delete task
			vTaskDelete(NULL);
		}
		delay(10);
	}
}
#endif
