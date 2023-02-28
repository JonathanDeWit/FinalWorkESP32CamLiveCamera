#include "ApiRequest.h"
#include "CameraUser.h"

const String ApiRequest::apiUrl = "https://finalworkapi.azurewebsites.net";
const String ApiRequest::cameraPath = "/api/camerauser";
const String ApiRequest::userName = "CameraA";
const String ApiRequest::apiPassword = "Jonathan01853";


String ApiRequest::helloWorld() {
    String url = apiUrl + "/api/user/helloworld";

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    String result = "";

    if (httpCode == HTTP_CODE_OK) {
        result = http.getString();
    } else {
        result = "Error: HTTP status code " + String(httpCode) + "\n";
    }
    http.end();

    return result;
}

String ApiRequest::login() {
    Serial.println(CameraUser::DeviceName);

    //Making login json
    DynamicJsonDocument jsonBody(200);
    jsonBody["Email"] = userName;
    jsonBody["Password"] = apiPassword;

    //convert jsonBody to a string
    String requestBody;
    serializeJson(jsonBody, requestBody);

    //making the http request
    HTTPClient http;
    http.begin(apiUrl+cameraPath+"/login");
    http.addHeader("Content-Type", "application/json");
    //send post request
    int httpResponseCode = http.POST(requestBody);

    String result = "";
    //Check if the request was sucsesfull or not
    if (httpResponseCode == HTTP_CODE_OK) {
        //Parse the response JSON and save the jwtToken
        DynamicJsonDocument jsonDoc(600);
        DeserializationError err = deserializeJson(jsonDoc, http.getString());
        if (!err) {
        const char* jwtToken = jsonDoc["jwtToken"];
        
        result = String(jwtToken);
        CameraUser::JwtToken = String(jwtToken);
        CameraUser::JwtTime = clock();
        }
    } else {
        Serial.println("Login: " + String(httpResponseCode));
    }

    //Free resources
    http.end();

    return result;
}

boolean ApiRequest::updateTransmitVideoStream(bool transmitVideoStream) {

    if((clock() - CameraUser::JwtTime) > 3600000){
        login();
    }


    if(CameraUser::JwtToken.compareTo("")){
        String transmitStatusString = "false";

        if(transmitVideoStream){
            transmitStatusString = "true";
        }
        Serial.println("Update TransmitVideoStream");
        //Making json for the login request with username and password.
        HTTPClient http;
        http.begin(apiUrl+cameraPath+"/update/TransmitVideoStream?transmitVideoStream="+transmitStatusString);
        http.addHeader("Authorization", "Bearer " + CameraUser::JwtToken);
        int httpCode = http.POST("");

        if (httpCode == HTTP_CODE_OK) {
            String statusStringRequest = http.getString();
            Serial.println("Debug: "+statusStringRequest);
            http.end();
            return true;
        }
        else {
            Serial.println("Error on HTTP request");
            http.end();
        }
    }
    
    return false;
}

StatusObject ApiRequest::getSystemState(String jwtToken) {
    StatusObject result(false, false, false);

    if((clock() - CameraUser::JwtTime) > 3600000){
        login();
    }

    //Check if the jwt token is not empty
    if(jwtToken.compareTo("")){
        //Making json for the login request with username and password.
        HTTPClient http;
        http.begin(apiUrl+cameraPath+"/getSystemState");
        http.addHeader("Authorization", "Bearer " + jwtToken);
        int httpCode = http.GET();

        
        if (httpCode == HTTP_CODE_OK) {
            String statusStringRequest = http.getString();
            Serial.println("Get system state: "+statusStringRequest);
            DynamicJsonDocument jsonDoc(550);
            DeserializationError err = deserializeJson(jsonDoc,statusStringRequest);
            if (!err) {
                const bool sysState = jsonDoc["SysState"];
                const bool diviceHasUser = jsonDoc["DiviceHasUser"];
                result.sysState = sysState;
                result.diviceHasUser = diviceHasUser;
                result.requestSucseed = true;
                
            }
            else{
                Serial.println("Error parsing JSON: " + String(err.c_str()));
            }
        }
        else {
            Serial.println("Error on get status request");
        }
        http.end();
    }
    else{
        Serial.println("Token is not Valid");
    }
    
    return result;
}