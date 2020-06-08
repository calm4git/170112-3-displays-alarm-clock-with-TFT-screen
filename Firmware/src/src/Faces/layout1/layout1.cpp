#include <TFTShape.h>
#include "../../../Free_Fonts.h" // Include the header file attached to this sketch
#include "layout1.h"




ClockLayout1::ClockLayout1( TFT_eSPI& lcd ){
    _lcd = &lcd;
    second_pev=-1;
    year_prev=-1; 
    month_prev=-1; 
    day_prev=-1; 
    dow_prev=-1; 
    hour_prev=-1; 
    minute_prev=-1; 

    
}

void ClockLayout1::UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second ){

  int16_t SinTable[12]={-110,-95,-55,  0,55, 95,110, 95, 55,   0,-55,-95};
  int16_t CosTable[12]={   0, 55, 95,110,95, 55,  0,-55,-95,-110,-95,-55};
  // display white/blue points each 5 second
  int16_t x1M = 160;                                               // centerpoint for the dots
  int16_t y1M = 120;
  //We draw all dots if this needs to have a refresh
  if(0 == (second%5) || ( second_pev < 0) ){
    //We calculate the startpoint we need to draw
    if(second_pev<0){
        for(uint8_t idx=0;idx<12;idx++){
            
            if( ( (minute%2)!=0 )  ){ 
                if(second < ( idx * 5 ) ){
                    _lcd->fillCircle(CosTable[idx]+x1M, SinTable[idx]+y1M, 7,_lcd->color565(31, 31, 255));
                } else {
                    _lcd->fillCircle(CosTable[idx]+x1M, SinTable[idx]+y1M, 7,_lcd->color565(255, 255, 255));
                }
            } else  {
                if(second < ( idx * 5 ) ){
                    _lcd->fillCircle(CosTable[idx]+x1M, SinTable[idx]+y1M, 7,_lcd->color565(255, 255, 255));
                } else {
                    _lcd->fillCircle(CosTable[idx]+x1M, SinTable[idx]+y1M, 7,_lcd->color565(31, 31, 255));    
                }
            }    
        }
    } else {
         uint8_t idx = ( second / 5 );
         if(idx < 12 ){
            if( ( (minute%2)==0 )  ){ 
                  _lcd->fillCircle(CosTable[idx]+x1M, SinTable[idx]+y1M, 7,_lcd->color565(31, 31, 255));
            } else {
                  _lcd->fillCircle(CosTable[idx]+x1M, SinTable[idx]+y1M, 7,_lcd->color565(255, 255, 255));
            }
         }
    }
    second_pev=second;
  }

  if( (minute != minute_prev) || (hour != hour_prev) ) {
  //Remove the hour handel by writing it balack 
  // set to black, invisible the previous hour hand
    //hour-hand
  //hour-hand
  TFTShape hand=TFTShapeBuilder::buildNgon(5,60);
  //we have 30 degree per hour assuming 24 hour clock
  uint16_t old_hour_andgle = (30 * hour_prev );
  //Next is the minute offset....(0,5 degree per minute )
  uint16_t old_minute_offset = minute_prev / 2;
  old_hour_andgle +=old_minute_offset;
  
  if(old_hour_andgle>=360){
   
      old_hour_andgle-=360;
  }

  uint16_t old_minute_angle = 6*minute_prev;

  hand.setScale(.08,.8);
  hand.setOffset(0,-30); 
  hand.setRotation(old_hour_andgle);
  hand.fill(_lcd,160,120,TFT_BLACK);
  
  
  //minutes-hand
  hand.setScale(.08,1);
  hand.setRotation(old_minute_angle);
  hand.fill(_lcd,160,120,TFT_BLACK);

  //Now for the new angles

  uint16_t hour_andgle = (30 * hour );
  //Next is the minute offset....(0,5 degree per minute )
  uint16_t minute_offset = minute / 2;
  hour_andgle +=minute_offset;
  
  if(hour_andgle>=360){
   
      hour_andgle-=360;
  }

  uint16_t minute_angle = 6*minute;

  hand.setScale(.08,.8);
  hand.setOffset(0,-30); 
  hand.setRotation(hour_andgle);
  hand.fill(_lcd,160,120,TFT_RED);
  
  
  //minutes-hand
  hand.setScale(.08,1);
  hand.setRotation(minute_angle);
  hand.fill(_lcd,160,120,TFT_GREEN);
  
  minute_prev=minute;
  hour_prev= hour ;

  }


  if( (year != year_prev) || ( month != month_prev) || ( day != day_prev ) || (dow != dow_prev)  ){
      year_prev = year;
      month_prev = month;
      day_prev = day;
      dow_prev = dow;
      DrawDate(  year,  month,  day,  dow );
  }

  if(true==RedrawAlarm){
      DrawAlarmONOFF(Alarm_Enable);
      DrawAlarmNEXT(AlarmHour,AlarmMinute,AlarmDow,!Alarm_Enable);
      RedrawAlarm=false;
  }


}
  




void ClockLayout1::UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute){
   if( (Alarm_Enable != Enabled ) || (AlarmDow != dow )  || (AlarmHour != hour ) || (AlarmMinute != minute) )  {
    RedrawAlarm=true;
    Alarm_Enable=Enabled;
    AlarmDow=dow;
    AlarmHour=hour;
    AlarmMinute=minute;
 
   } else {
    //We do nothing 
   }
}

void ClockLayout1::ForceScreenRefresh( void ){
    second_pev=-1;
    year_prev=-1; 
    month_prev=-1; 
    day_prev=-1; 
    dow_prev=-1; 
    hour_prev=-1; 
    minute_prev=-1; 
    RedrawAlarm=true;
    _lcd->fillScreen(TFT_BLACK);
}

void ClockLayout1::DrawAlarmONOFF( bool ON ){
  _lcd->setCursor(0, 15, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  _lcd->fillRect(0, 0, 90, 18, TFT_BLACK);  
  _lcd->setTextColor(TFT_WHITE,TFT_BLACK);
  _lcd->setFreeFont(FSB9); 
  _lcd->print("Alarm: ");
  if(true==ON){
      _lcd->println("On");
  } else {
      _lcd->println("Off");
  }
}
void ClockLayout1::DrawAlarmNEXT( uint8_t hour, uint8_t minute, uint8_t dow, bool clear ){
   if(false == clear){
    _lcd->fillRect(230, 0, 320, 40, TFT_BLACK);  
    _lcd->setCursor(230, 15, 2);
    _lcd->setTextColor(TFT_GREENYELLOW ,TFT_BLACK);
    _lcd->setFreeFont(FSB9); 
    switch(dow){
      //Our week starts on Monday 
      case 0:{
        _lcd->print("Monday");
      }break;
      case 1:{
        _lcd->print("Thuseday");
      }break;
      case 2:{
        _lcd->print("Wednesday");
      }break;
      case 3:{
        _lcd->print("Thursday");
      }break;
      case 4:{
        _lcd->print("Friday");
      }break;
      case 5:{
        _lcd->print("Saturday");
      }break;
      case 6:{
        _lcd->print("Sunday");
      }break;

      default:{
        _lcd->print("Towel Day");
      }break;
  }
  _lcd->setCursor(240, 32, 2);
  _lcd->printf("%02d",hour);
  _lcd->print(":");
  _lcd->printf("%02d",minute);

  } else {
      //Clear the area 
    _lcd->fillRect(230, 0, 320, 40, TFT_BLACK);
  }

}
void ClockLayout1::DrawDate( uint16_t year, uint8_t month, uint8_t day, uint8_t dow ){

   _lcd->fillRect(5, 210, 92, 20, TFT_BLACK);
  _lcd->setCursor(5, 225, 2);
  _lcd->setTextColor(TFT_CYAN,TFT_BLACK);
  _lcd->setFreeFont(FSB9); 
  //We need to draw the DOW
  switch(dow){
      //Our week starts on Monday 
      case 0:{
        _lcd->print("Monday");
      }break;
      case 1:{
        _lcd->print("Thuseday");
      }break;
      case 2:{
        _lcd->print("Wednesday");
      }break;
      case 3:{
        _lcd->print("Thursday");
      }break;
      case 4:{
        _lcd->print("Friday");
      }break;
      case 5:{
        _lcd->print("Saturday");
      }break;
      case 6:{
        _lcd->print("Sunday");
      }break;

      default:{
        _lcd->print("Towel Day");
      }break;
  }

  _lcd->fillRect(250, 200, 95, 20, TFT_BLACK);
  _lcd->setCursor(250, 215, 2);
  _lcd->setTextColor(TFT_CYAN,TFT_BLACK);
  _lcd->setFreeFont(FSB9); 
  
  _lcd->print(day);
  _lcd->print(". ");
  switch(month){
      case 0:{
        _lcd->print("Jan");
      }break;

      case 1:{
        _lcd->print("Feb");
      }break;

      case 2:{
        _lcd->print("Mar");
      }break;

      case 3:{
        _lcd->print("Apr");   
      }break;

      case 4:{
          _lcd->print("May");
      }break;

      case 5:{
          _lcd->print("Jun");
      }break;

      case 6:{
        _lcd->print("Jul");
      }break;

      case 7:{
        _lcd->print("Aug");
      }break;

      case 8:{
        _lcd->print("Sep");
      }break;

      case 9:{
        _lcd->print("Oct");
      }break;

      case 10:{
        _lcd->print("Nov");
      }break;

      case 11:{
        _lcd->print("Dec");
      }break;

      default:{
        _lcd->print("???");
      }break;
  }
  _lcd->print(".");
  _lcd->fillRect(250, 220, 95, 20, TFT_BLACK);
  _lcd->setCursor(250, 235, 2);
  _lcd->setTextColor(TFT_CYAN,TFT_BLACK);
  _lcd->setFreeFont(FSB9); 
  
  _lcd->print(" ");
  _lcd->print(year);
}