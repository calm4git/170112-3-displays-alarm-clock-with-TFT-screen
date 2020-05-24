
#include "AlarmMgr.h"

AlarmMgr::AlarmMgr( void ){
    //We need to initilize the class
    //Alarms are disabled by thire constructor
    ringduration= 60 * 15 ; //15 minutes max ring duration per alarm...
    
    AlarmsEnabled=false;
    ring_start=0;
    is_ringing=false;
    snooze=false;
    snooze_until=0;
    AlarmsEnabled=false;
    
    

}

void AlarmMgr::SetRingDuration(uint32_t duration_m){
    if(duration_m<=0){
        duration_m=1;
    }
    ringduration = duration_m*60;
}

uint32_t AlarmMgr::GetRingDuration( void ){
    return ringduration;
}

void AlarmMgr::QuitAlarms( void ){   
    
    for(uint8_t i=0;i<ALARMCOUNT;i++){
                UserAlarms[i].QuitAlarm(); //We stop any ringing...
    }
    ring_start=0;
    snooze_until=0;
    is_ringing=false;

}

bool AlarmMgr::SetAlarm( uint8_t idx, Alarm* Al ){
    return false;

}

bool AlarmMgr::GetAlarm( uint8_t idx, Alarm** Al ){
    return false;
}

//This will return the next Alarm that is active 
bool AlarmMgr::GetNextAlarm(Alarm** Al, time_t utc_now ){
    uint32_t min_distance = UINT32_MAX ;
    uint32_t getnextalarmtime=0;
    bool foundalarm=false;
    
    for(uint8_t i=0;i<ALARMCOUNT;i++){
        getnextalarmtime=UserAlarms[i].GetNextAlarmTime( utc_now );
        if(min_distance>0){
            if(min_distance > (getnextalarmtime - utc_now) ){
            
                min_distance = (getnextalarmtime - utc_now);
                *Al = &UserAlarms[i];
                foundalarm=true;
            }
        }
    }

    return foundalarm;

}
//If something is ringing this will return the entry....
bool AlarmMgr::GetCurrentAlarm(Alarm** al ){
  bool found = false;
  for(uint8_t i=0;i<ALARMCOUNT;i++){
      if(true == UserAlarms[i].ActiveAlarm()){
            *al=&UserAlarms[i];
            found = true;
      }
  }
  return found;
} 

//This will snooze the active alarm
void AlarmMgr::SnoozeAlarm( time_t utc_now ){
    //We will snooze globaly, to do so we need the current time
    if(false == snooze){

        if(true == is_ringing){
            //We will only snooze if we have an active alarm....
            snooze_until = utc_now + ( ringduration * 60 ) ;
            snooze=true;
        }

    } else {



    }


} 

bool AlarmMgr::isSnoozedAlarm( void ){
    return snooze;
    
}

//Will disable global alarms
void AlarmMgr::DisableAlarms( void ){
    AlarmsEnabled=false;
    ring_start=0;
    is_ringing=false;
    snooze=false;
    snooze_until=0;
    AlarmsEnabled=false;
    for(uint8_t i=0;i<ALARMCOUNT;i++){
                UserAlarms[i].QuitAlarm(); //We stop any ringing...
    }
}

//This will enable global alarms
void AlarmMgr::EnableAlarms( void ){
    AlarmsEnabled=true;
    ring_start=0;
    is_ringing=false;
    snooze=false;
    snooze_until=0;
    AlarmsEnabled=false;
    for(uint8_t i=0;i<ALARMCOUNT;i++){
                UserAlarms[i].QuitAlarm(); //We stop any ringing...
    }

} 

//This will check all alarms and also handle the timeout 
bool AlarmMgr::CheckAlarms( time_t utc_now ){
    bool ringing = false;
    bool ring = false;
    for(uint8_t i=0;i<ALARMCOUNT;i++){
       if(true == UserAlarms[i].CheckAlarmTime( utc_now ) ){
         ring = true;
       }
    }

    if(true == ring ){
        ring_start = utc_now;
        is_ringing = true;
        snooze_until=0;
        snooze = false;
        ringing=true;
    } else {
        //We can check if we still need to ring
        if(true == snooze ){
            if(snooze_until < utc_now){
                //snooze is over 
                ring_start = utc_now;
                ringing=true;
            } else {
                ringing=false;
            }
        } else {
            if(ring_start + ( ringduration *60 ) < utc_now){
                //We will stop ringing! Active alarms will be disabled
                for(uint8_t i=0;i<ALARMCOUNT;i++){
                    UserAlarms[i].QuitAlarm(); //We stop any ringing...
                }
                is_ringing = false;
                snooze_until=0;
                snooze = false;
                ringing=false;
            } else {
                ringing=true;
            } 
        }
    }           
    return ringing;
} 