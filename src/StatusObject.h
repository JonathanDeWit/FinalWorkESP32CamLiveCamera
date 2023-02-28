#ifndef StatusObject_h
#define StatusObject_h

#include <Arduino.h>

class StatusObject {
private:

public:
    bool sysState;
    bool diviceHasUser;
    bool requestSucseed;

    
    StatusObject(bool initialSysState, bool initialDiviceHasUser, bool initialRequestSucseed) {
        sysState = initialSysState;
        diviceHasUser = initialDiviceHasUser;
        requestSucseed = initialRequestSucseed;
    }


};


#endif