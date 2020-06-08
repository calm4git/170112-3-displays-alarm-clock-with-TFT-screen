#ifndef __CLOCK_HAL_H__
 #define __CLOCK_HAL_H__
    //We assume an LDR or simmilar
    //LED Backlight controlled with someting 
    //And four Buttons 
    void HAL_init( void );
    void GetLDRValue( void );
    //We need a RTC interface....
    void GetRTC_UTC( void );
    void SetRTC_UTC( void );




#endif