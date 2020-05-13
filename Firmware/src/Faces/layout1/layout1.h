 #ifndef __LAYOUT1_H__
 #define __LAYOUT1_H__

#include "../faces.h"
#include "TFT_eSPI.h"


 class ClockLayout1: public Faces{

    public:
        
        ClockLayout1( TFT_eSPI& lcd );

        void UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second );
        void UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute);
        void ForceScreenRefresh( void );
        uint32_t GetScreenHight( void ) { return 240; }
        uint32_t GetScreenWidth( void ) { return 320; }

    private:
        
        void DrawAlarmONOFF( bool ON );
        void DrawAlarmNEXT( uint8_t hour, uint8_t minute, uint8_t dow, bool clear );
        void DrawDate( uint16_t year, uint8_t month, uint8_t day, uint8_t dow );


        int8_t second_pev;
        int16_t year_prev; 
        int8_t month_prev; 
        int8_t day_prev; 
        int8_t dow_prev; 
        int8_t hour_prev; 
        int8_t minute_prev; 

        bool RedrawAlarm;
        bool Alarm_Enable;
        uint8_t AlarmDow;
        uint8_t AlarmHour;
        uint8_t AlarmMinute;
            
        TFT_eSPI* _lcd;

 };
 #endif