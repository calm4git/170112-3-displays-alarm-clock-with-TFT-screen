#ifndef __ALARM_H__
 #define __ALARM_H__

    class Alarm{

        public:
            typedef struct{
                uint8_t Hour;
                uint8_t Minute;
                uint8_t Second;
                uint8_t Day;
                uint8_t Month;
                uint16_t Year;
                uint16_t Enabled:1;
                uint16_t OneShot:1;
                uint16_t UseDate:1;
                uint16_t Reserved:2;
                uint16_t IsRinging:1;
                uint16_t SnoozeCount:3; //We allow 3Bit snoozecount = 7 times ! , snooze is fixed to 10 minutes!
                uint16_t Monday:1;
                uint16_t Thuseday:1;
                uint16_t Wednesday:1;
                uint16_t Tursday:1;
                uint16_t Friday:1;
                uint16_t Saturday:1;
                uint16_t Sunday:1;
            } Alarmtime_t;
            
            Alarm( void );
            ~Alarm( void );

            void SetAlarmTime( Alarmtime_t Time);
            Alarmtime_t GetAlarmTime( void );
            void ResetSnooze( void );
            bool SnoozeAlarm( void ); //If we have reached the snoozelimit ( 15 * 10 minutes we will ignore snooze )
            bool CheckAlarmTime( uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second ); 
            bool CheckAlarmTime( time_t utc_timenow ); 

        private:

            Alarmtime_t AlarmTime;
            time_t _lastCalled = 0;
            
            bool CheckAlarmTime( time_t utc_timenow, tmElements_t timeelement ); 
            bool AlarmOnDOW(uint8_t dow);
            void AlarmSetEnableDow( uint8_t dow, bool ena);
        


    };

#endif