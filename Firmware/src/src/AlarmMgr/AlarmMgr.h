#ifndef __ALARMMGR_H__
 #define __ALARMMGR_H__

    #include <Arduino.h>
    #include <TimeLib.h>
    #include "../Alarms/alarm.h"

    #define ALARMCOUNT 8
    class AlarmMgr{

        //We impliment a basic class just be able to handle 8 alarms
        public:

            AlarmMgr( void );
            void SetRingDuration(uint32_t duration_m);
            uint32_t GetRingDuration( void );
            
            bool SetAlarm( uint8_t idx, Alarm::Alarmtime_t Al );
            bool GetAlarm( uint8_t idx, Alarm::Alarmtime_t* Al );

            bool GetNextAlarm(Alarm** Al, time_t utc_now ); //This will return the next Alarm that is active 
            bool GetCurrentAlarm(Alarm** Al ); //If something is ringing this will return the entry....
            
            void SnoozeAlarm( time_t utc_now  ); //This will snooze the alarm on a global scope....
            bool isSnoozedAlarm( void ); //Returns if a snooze if active...

            void QuitAlarms( void );
            void DisableAlarms( void ); //Will disable global alarms
            void EnableAlarms( void ); //This will enable global alarms

            bool CheckAlarms( time_t utc_now ); //This will check all alarms and also handle the timeout, returns true if it needs to ring

            uint8_t GetMaxAlarms( void );

        private:
            //Internally stored user alarms 
            
            Alarm UserAlarms[ALARMCOUNT];
            time_t ring_start;
            bool is_ringing;
            uint32_t ringduration;
            bool snooze;
            uint32_t snooze_until;
            bool AlarmsEnabled;
            

    };


#endif