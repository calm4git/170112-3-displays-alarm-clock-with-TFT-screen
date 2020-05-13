#ifndef __LAYOUT3_H__
 #define __LAYOUT3_H__

#include "../faces.h"
#include "TFT_eSPI.h"

 class ClockLayout3: public Faces{

    public:
        
        ClockLayout3( TFT_eSPI& lcd );

        void UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second );
        void UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute);
        void ForceScreenRefresh( void );
        uint32_t GetScreenHight( void ) { return 240; }
        uint32_t GetScreenWidth( void ) { return 320; }

    private:

    int8_t dozenH_Prev;
    int8_t unitH_Prev;
    int8_t dozenM_Prev;
    int8_t unitM_Prev;

    int16_t Year_Prev;
    int8_t Month_Prev;
    int8_t Day_Prev;
    int8_t Dow_Prev;

    bool RedrawAlarm;
    bool Alarm_Enable;
    uint8_t AlarmDow;
    uint8_t AlarmHour;
    uint8_t AlarmMinute;

    TFT_eSPI* _lcd;
    bool BargraphDrawn;

    const uint8_t Ypos = 42 ;
    const uint8_t Xpos = 0 ;

    void DrawAlarmONOFF( bool ON );
    void DrawAlarmNEXT( uint8_t hour, uint8_t minute, uint8_t dow, bool clear );
    void DrawDate( uint16_t year, uint8_t month, uint8_t day, uint8_t dow );


    void Segment( uint8_t idx, uint16_t offset, uint16_t color);
    void SecondsBargraph( uint8_t seconds);
    void Colon( uint8_t second );
 };

 #endif