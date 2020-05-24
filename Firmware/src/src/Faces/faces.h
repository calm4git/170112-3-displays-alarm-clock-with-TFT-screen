 #ifndef __FACES_H__
  #define __FACES_H__

#include <stdint.h>
#include <TimeLib.h>

class Faces{

    public:
        void UpdateTime( time_t utc_now );
        virtual void UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second );
       
        void UpdateAlarmInfo(bool Enabled, time_t utc_now);
        virtual void UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute);
        
        virtual void ForceScreenRefresh( void );

        /* This will tell for what resulution the face is build for */
        virtual uint32_t GetScreenHight( void );
        virtual uint32_t GetScreenWidth( void );



 };

 #endif