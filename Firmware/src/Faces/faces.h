 #ifndef __FACES_H__
  #define __FACES_H__

#include <stdint.h>

class Faces{

    public:
        
        virtual void UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second );
        virtual void UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute);
        virtual void ForceScreenRefresh( void );

        /* This will tell for what resulution the face is build for */
        virtual uint32_t GetScreenHight( void );
        virtual uint32_t GetScreenWidth( void );



 };

 #endif