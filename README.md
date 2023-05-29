# Final Work ESP32-Cam Live Camera
This project is part of my Final Work project for my Bachelor degree in Applied Computer Science at the Erasmus Brussel University of Applied Sciences and Arts.
My project mainly consisted of building a video surveillance system by using microcontrollers.
If you want to know more about the project, I invite you to consult the following links:

[Final Work Paper](https://github.com/JonathanDeWit/FinalWorkESP32CamLiveCamera/blob/master/FinalWorkPaper.pdf) (NL)

Other related repositories:
* [Android App](https://github.com/JonathanDeWit/FinalWorkAndroidApp)
* [ASP.NET API](https://github.com/JonathanDeWit/FinalWorkApi)
* [Raspberry PI automated video convertor](https://github.com/JonathanDeWit/FinalWorkRaspberryPiConvertor)
* [SRT Media server automation](https://github.com/JonathanDeWit/FinalWorkSrtServer)

 ## Purpose
This GitHub repo contains an PlatformIO project meant for an ESP32-CAM board.
This project's primary goal is to send a live video stream using the RTSP protocol. This video stream is only active if the user has activated the system using his app. if the user turn the system off the video stream will automatically be stopped.

Every API call you will find in this project was directed to the ASP.Net API which you can find [here](https://github.com/JonathanDeWit/FinalWorkApi)


 ## Primary features
 - Making an RTSP server and crate a live stream.
   - Wen the microcontroller starts to stream the stream wil be availble at: "\<Ip>:8554/mjpeg/1"
 - Make API calls.
   - authenticate and store the JWT token.
     - Replace the JWT token when it is about to expire.
   - Retreave the system status.
     - this mainly includes seeing if the microcontroller is linked to a user and seeing if the security system should be activated or not.
 - Updating the microcontroller status.
     - Updating the microcontroller transmission status.
     - Updating his local Ip.
 - Using RTOS to execute various actions of my code on separate threads concurrently.

 
 ## When does the live stream starts?
The microcontroller will check almost every second with the API to see if the system should be activated or not. If so it will start the RTSP server on a new thread using RTOS. 
When the API returns that the live stream needs to stop the microcontroller will stop the RTSP server and continue checking util the system status change again.

 
