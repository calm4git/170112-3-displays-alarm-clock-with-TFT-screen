#include <TimeLib.h>
 #include <strings.h>
#include "alarm.h"

        
    Alarm::Alarm( void ){
            bzero((void*)(&AlarmTime), sizeof(AlarmTime) );
    }

    Alarm::~Alarm( void ){

    }

    void Alarm::SetAlarmTime( Alarmtime_t Time){
        AlarmTime=Time;
    }

    Alarm::Alarmtime_t Alarm::GetAlarmTime( void ){
        return AlarmTime;
    }

    bool Alarm::ActiveAlarm( void ){
        return AlarmTime.IsRinging;
    }

    void Alarm::QuitAlarm( void ){
        AlarmTime.IsRinging = false;
        AlarmTime.SnoozeCount=0;
    }

    void Alarm::ResetSnooze( void ){
        AlarmTime.SnoozeCount=0;

    }
    bool Alarm::SnoozeAlarm( void ){ //If we have reached the snoozelimit ( 15 * 10 minutes we will ignore snooze )
        if( AlarmTime.SnoozeCount < 15 ){
            AlarmTime.SnoozeCount++;
            //We postprone the Alert for 10 minutes , again ....
            return true;
        } else {
            return false; //Keep it rinning soozetime is over
        }

    }

    void Alarm::AlarmSetEnableDow( uint8_t dow, bool ena){
        switch(dow){

            case 0:{
                if(false == ena ){
                    AlarmTime.Monday=0;
                } else {
                    AlarmTime.Monday=1;
                }
            } break;

            case 1:{
                if(false == ena ){
                    AlarmTime.Thuseday=0;
                } else {

                }

            } break;

            case 2:{
                if(false == ena ){
                    AlarmTime.Wednesday=0;
                } else {
                    AlarmTime.Wednesday=0;
                }
            } break;

            case 3:{
                if(false == ena ){
                    AlarmTime.Tursday=0;
                } else {
                    AlarmTime.Tursday=0;
                }
            } break;

            case 4:{
                if(false == ena ){
                    AlarmTime.Friday=0;
                } else {
                    AlarmTime.Friday=1;
                }
            } break;

            case 5:{
                if(false == ena ){
                    AlarmTime.Saturday=0;
                } else {
                    AlarmTime.Saturday=1;
                }
            } break;

            case 6:{
                if(false == ena ){
                    AlarmTime.Sunday=0;
                } else {
                    AlarmTime.Sunday=1;
                }

            } break;

            default:{
                
            }break;
        }
    }

    bool Alarm::AlarmOnDOW(uint8_t dow){
        bool alarmonday=false;
        switch(dow){

            case 0:{
                if(AlarmTime.Monday>0){
                    alarmonday=true;
                }
            } break;

            case 1:{
                if(AlarmTime.Thuseday>0){
                    alarmonday=true;
                }
            } break;

            case 2:{
                if(AlarmTime.Wednesday>0){
                    alarmonday=true;
                }
            } break;

            case 3:{
                if(AlarmTime.Tursday>0){
                    alarmonday=true;
                }
            } break;

            case 4:{
               if(AlarmTime.Friday>0){
                    alarmonday=true;
                }
            } break;

            case 5:{
                if(AlarmTime.Saturday>0){
                    alarmonday=true;
                }
            } break;

            case 6:{
                if(AlarmTime.Sunday>0){
                    alarmonday=true;
                }

            } break;

            default:{
                alarmonday=false;
            }break;
        }

        return alarmonday;

    }

    

    bool Alarm::CheckAlarmTime( uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second ){
    
        tmElements_t timeelement;

        timeelement.Second = second;
        timeelement.Minute = minute;
        timeelement.Hour = hour;
        timeelement.Day = day;
        timeelement.Month = month;
        if(year>=1970){
            timeelement.Year = year-1970;
        } else {
            timeelement.Year=0;
        }
        time_t utc_timenow = makeTime(timeelement );
        timeelement.Wday = weekday( utc_timenow);
        return CheckAlarmTime(utc_timenow, timeelement);


    }

    bool Alarm::CheckAlarmTime( time_t utc_timenow  ){
    
        tmElements_t timeelement;
        breakTime( utc_timenow, timeelement);
        return CheckAlarmTime(utc_timenow, timeelement);

    }



     bool Alarm::CheckAlarmTime( time_t utc_timenow, tmElements_t timeelement ){
        bool ring = false;
        //We havent had a previus timespan so we need to hard check if the alarm needs to ring......
        tmElements_t alarmelement;
        tmElements_t _lastCalledElement;
        time_t utc_alarm;

     


        if(AlarmTime.Enabled>0){

            
            if( (AlarmTime.UseDate>0) ){ 
                //If we use a defined date and / or need to take care for snoozetime
                //Also every alarm that uses a given date is oneshot by default!
                //We need to check for a given timestamp 
                alarmelement.Hour = AlarmTime.Hour;
                alarmelement.Minute = AlarmTime.Minute;
                alarmelement.Second = AlarmTime.Second;
                alarmelement.Day = AlarmTime.Day;
                alarmelement.Month = AlarmTime.Day;
                if(AlarmTime.Year>1970){
                    alarmelement.Year = AlarmTime.Year-1970;
                } else {
                    alarmelement.Year = 0;
                } 

                utc_alarm = makeTime(  alarmelement);
                if(AlarmTime.SnoozeCount>0){
                    utc_alarm += 10*60*AlarmTime.SnoozeCount;
                }

                //Next will be a comperison....
                if( _lastCalled <=0 ){
                    //We don't have a timespan just plain compare
                    if( utc_alarm == utc_timenow ){
                        //We have a match....
                        ring = true;
                    } else {
                        //We don't ring.......
                        //But we check if the time is in the past so we will stop the alarm 
                        if( utc_alarm< utc_timenow ){
                            AlarmTime.Enabled=0;
                        }
                    }
                } else {
                    if( utc_alarm == utc_timenow ){
                        //We have a match....
                        ring = true;
                    } else {
                        //We don't ring.......
                        //But we check if the time is in the past so we will stop the alarm 
                        if( ( utc_alarm< utc_timenow ) && ( utc_alarm >= _lastCalled ) ){
                            ring = true;                    
                        } else {
                            AlarmTime.Enabled=0;
                        }
                    }                    
                }

            } else {
                //We calculate the alarm on a weekbased approch ...
                //We know the last time this function was called and also 
                breakTime( _lastCalled, _lastCalledElement);
                //We calculate the timestamp for each day that may be enabled....
                time_t weekstart=previousSunday(utc_timenow)+SECS_PER_DAY;//Monday 00:00:00
                for(uint8_t i=0;i<7;i++){
                    if( true == AlarmOnDOW(i) ){
                        
                        //We calculate the UTC time where it should ring....and we need to add the snozzetime....
                        uint32_t alarmseconds = ( AlarmTime.Second ) + ( AlarmTime.Minute*60 ) + (AlarmTime.Hour*3600)+(AlarmTime.SnoozeCount*10*60);
                        uint32_t alarmdayts = weekstart + alarmseconds + ( 60 * 60 * 24 * i );
                        if( ( alarmdayts< utc_timenow ) && ( alarmdayts >= _lastCalled ) ){
                            ring = true;  
                            if(true == AlarmTime.OneShot){
                                AlarmSetEnableDow(i,false);
                            }
                        } else {
                            //Not this day....
                        }

                    } else {
                        //We can skip the day
                    }
                
                }

                //Last step is to check if any actives alarms are left
                if(true == AlarmTime.OneShot){
                    bool alarm_active = false;
                    for(uint8_t i=0;i<7;i++){
                        alarm_active = alarm_active | AlarmOnDOW(i);
                    }
                    if(false == alarm_active){
                        AlarmTime.Enabled = false;
                    }
                }


            }

        } else {
            //We don't ring ....
        }

        _lastCalled = utc_timenow;
        if(true == ring ){
            AlarmTime.IsRinging = true; //We note that this has been triggered
            //If later an other alarm gets active we will put this one to sleep
        }

        return ring;
    }
    
     
time_t Alarm::GetNextAlarmTime( time_t utc_now ){

    tmElements_t alarmelement;

    time_t utc_alarm;
    time_t nextalarm_time=0;


    if(AlarmTime.Enabled>0){

            
            if( (AlarmTime.UseDate>0) ){ 
                //If we use a defined date and / or need to take care for snoozetime
                //Also every alarm that uses a given date is oneshot by default!
                //We need to check for a given timestamp 
                alarmelement.Hour = AlarmTime.Hour;
                alarmelement.Minute = AlarmTime.Minute;
                alarmelement.Second = AlarmTime.Second;
                alarmelement.Day = AlarmTime.Day;
                alarmelement.Month = AlarmTime.Day;
                if(AlarmTime.Year>1970){
                    alarmelement.Year = AlarmTime.Year-1970;
                } else {
                    alarmelement.Year = 0;
                } 

                utc_alarm = makeTime(  alarmelement);
                if(AlarmTime.SnoozeCount>0){
                    utc_alarm += 10*60*AlarmTime.SnoozeCount;
                }

                if(utc_now < utc_alarm){
                    //The alarm is in the future so we can return a timespan
                    nextalarm_time=utc_alarm;
                } else {
                    //Alarm is in the past for what ever reason
                    nextalarm_time=0;
                }

            } else {
               
                time_t daystart=previousMidnight(utc_now);//Currentday 00:00:00
                //This is a bit tricky.... sort of...
                for(uint8_t i=0;i<7;i++){
                    uint8_t DOW = ( dayOfWeek(daystart + ( SECS_PER_DAY * i) ) ) ;
                    //DOW starts with 1 for sunday we start with 0 on Monday
                    if(1==DOW){
                        DOW=6;
                    } else {
                        DOW=DOW-2;
                    }

                    if( true == AlarmOnDOW(DOW) ){
                        
                        //We calculate the UTC time where it should ring....and we need to add the snozzetime....
                        uint32_t alarmseconds = ( AlarmTime.Second ) + ( AlarmTime.Minute*60 ) + (AlarmTime.Hour*3600)+(AlarmTime.SnoozeCount*10*60);
                        uint32_t alarmtimeofday = daystart + ( SECS_PER_DAY * i)+alarmseconds;

                        if(alarmtimeofday>utc_now){
                            //Okay it's on this day
                            nextalarm_time=alarmtimeofday;
                            break;
                        } else {
                            //Alert is active and seems to be on this day but passed...
                            uint32_t seconds_passed = ( utc_now - alarmtimeofday );
                            nextalarm_time = SECS_PER_WEEK - seconds_passed;
                        }
                        
                    }
                
                }
            }
        } else {
             nextalarm_time=0;
        }
        return nextalarm_time; 
    }