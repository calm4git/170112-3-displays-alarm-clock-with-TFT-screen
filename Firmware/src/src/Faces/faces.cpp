#include <Arduino.h>
#include "faces.h"

 void Faces::UpdateTime( time_t utc_now ){
    tmElements_t tm;
    uint8_t dow=0;
    breakTime(utc_now, tm);
    //We expect 0 to be Monday, this starts with 1 = Sunday....
    dow=tm.Wday;
    if(1==dow){
        dow=6;
    } else {
        if(dow == 0){
            
        } else {
            dow=dow-2;
        }
    }
    UpdateTime( 1970+tm.Year,tm.Month-1,tm.Day,dow,tm.Hour,tm.Minute,tm.Second );
 }

 void Faces::UpdateAlarmInfo( bool Enabled, time_t utc_now ){
    tmElements_t tm;
    uint8_t dow=0;
    breakTime(utc_now, tm);
    //We expect 0 to be Monday, this starts with 1 = Sunday....
    dow=tm.Wday;
    if(1==dow){
        dow=6;
    } else {
        dow=dow-2;
    }
    UpdateAlarmInfo(Enabled ,dow,tm.Hour,tm.Minute);
 }