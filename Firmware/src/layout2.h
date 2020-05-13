#ifndef __LAYOUT2_H__
 #define __LAYOUT2_H__

#include "TFT_eSPI.h"

 class ClockLayout2{

    public:
        
        ClockLayout2( TFT_eSPI& lcd );

        void UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second );
        void UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute);
        void ForceScreenRefresh( void );

    private:

    void PointsHelper( uint8_t Val, int16_t offset, uint16_t Color);    
    void Points0(uint16_t Numdigit, uint16_t Color);
    void Points1(uint16_t Numdigit, uint16_t Color);
    void Points2(uint16_t Numdigit, uint16_t Color);
    void Points3(uint16_t Numdigit, uint16_t Color);
    void Points4(uint16_t Numdigit, uint16_t Color);
    void Points5(uint16_t Numdigit, uint16_t Color);
    void Points6(uint16_t Numdigit, uint16_t Color);
    void Points7(uint16_t Numdigit, uint16_t Color);
    void Points8(uint16_t Numdigit, uint16_t Color);
    void Points9(uint16_t Numdigit, uint16_t Color);
    void PointsColumn( void );
    void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);
    void DrawSeconds(uint8_t seconds, uint16_t color);
    void DrawAlarmONOFF( bool ON );
    void DrawAlarmNEXT( uint8_t hour, uint8_t minute, uint8_t dow, bool clear );
    void DrawDate( uint16_t year, uint8_t month, uint8_t day, uint8_t dow );

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

    

    int8_t PointDrawn; 
    

    TFT_eSPI* _lcd;

    /* common data */
    const uint8_t Ypos = 42 ;
    const uint8_t Xpos = 0 ;
    const float  DEG2RAD = 0.0174532925;
    

 };


#endif
