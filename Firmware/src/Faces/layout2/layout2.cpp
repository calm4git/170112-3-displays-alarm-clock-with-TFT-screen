#include "../../../Free_Fonts.h" // Include the header file attached to this sketch
#include "layout2.h"

 
ClockLayout2::ClockLayout2( TFT_eSPI& lcd ){
   _lcd = &lcd;
   dozenH_Prev=-1;
   unitH_Prev=-1;
   dozenM_Prev=-1;
   unitM_Prev=-1;
   PointDrawn=-1; 

   Year_Prev=-1;
   Month_Prev=-1;
   Day_Prev=-1;
   Dow_Prev=-1;

   RedrawAlarm=true;
   Alarm_Enable=false;
   AlarmDow=0;
   AlarmHour=0;
   AlarmMinute=0;
  
}

void ClockLayout2::ForceScreenRefresh( void ){
   
   dozenH_Prev=-1;
   unitH_Prev=-1;
   dozenM_Prev=-1;
   unitM_Prev=-1;
   PointDrawn=-1; 

   Year_Prev=-1;
   Month_Prev=-1;
   Day_Prev=-1;
   Dow_Prev=-1;

   RedrawAlarm=true; 
   _lcd->fillScreen(TFT_BLACK);
  
}

 void ClockLayout2::UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute){
   
   
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
        

//We need an override with a compleate unix timestamp, works better for the esp32 
 void ClockLayout2::UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second ){
 
    uint8_t dozenH = hour / 10;
    uint16_t  Numdigit = 8;

    if(dozenH_Prev!=dozenH){
        PointsHelper(dozenH,Numdigit,_lcd->color565(255,255,255));
        dozenH_Prev=dozenH;
    }

    uint8_t unitH = hour % 10;
    Numdigit = 80;
    if(unitH_Prev != unitH ){
        PointsHelper(unitH,Numdigit,_lcd->color565(253,0,0));
        unitH_Prev = unitH;
    }

    if(PointDrawn<1){
        Numdigit = 153;
        PointsColumn();
    PointDrawn=1;
    }

    uint8_t  dozenM = minute / 10;
    Numdigit = 180;


    if(dozenM_Prev != dozenM){
        PointsHelper(dozenM,Numdigit,_lcd->color565(0,255,0));
        dozenM_Prev = dozenM;
    }


    uint8_t unitM = minute % 10;
    Numdigit = 253;
    if(unitM_Prev != unitM ){
        PointsHelper(unitM,Numdigit,_lcd->color565(127,127,255) );
        unitM_Prev = unitM;
    }
  
    switch( (minute%4) ){
        case 0:{
                DrawSeconds(second, _lcd->color565(253,0,0)); //RED
        } break;

        case 1:{
                DrawSeconds(second,  _lcd->color565(0,255,0)); //GREEN              
        } break;

        case 2:{
            DrawSeconds(second, _lcd->color565(63,63,255)); //BLUE
        } break;

        case 3:{
            DrawSeconds(second, 0x7BEF); //GREY
        } break;

        default:{
            DrawSeconds(second, 0x7BEF); //GREY
        } break;

    }

  if(true == RedrawAlarm){
    DrawAlarmONOFF(Alarm_Enable);
    DrawAlarmNEXT(AlarmHour,AlarmMinute,AlarmDow,!Alarm_Enable);
    RedrawAlarm=false;
  }


  if( (Year_Prev != year ) || (Month_Prev != month ) || (Day_Prev != day) || (Dow_Prev != dow ) ){
      Year_Prev = year;
      Month_Prev = month;
      Day_Prev = day;
      Dow_Prev = dow;
      DrawDate(  year,  month,  day,  dow );
  }
  
}

void ClockLayout2::PointsHelper( uint8_t Val, int16_t offset, uint16_t Color){

 switch (Val) {
      case 0: Points0(offset,Color); break;
      case 1: Points1(offset,Color); break;
      case 2: Points2(offset,Color); break;
      case 3: Points3(offset,Color); break;
      case 4: Points4(offset,Color); break;
      case 5: Points5(offset,Color); break;
      case 6: Points6(offset,Color); break;
      case 7: Points7(offset,Color); break;
      case 8: Points8(offset,Color); break;
      case 9: Points9(offset,Color); break; 
      }
}


void ClockLayout2::Points0(uint16_t Numdigit, uint16_t Color)
// display of 0 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);
 
  _lcd->fillRect(Numdigit +  0, Ypos +  0, 14, 84, Color);
  _lcd->fillRect(Numdigit + 14, Ypos +  0, 42, 14, Color);
  _lcd->fillRect(Numdigit + 42, Ypos +  0, 14, 84, Color);
  _lcd->fillRect(Numdigit + 14, Ypos + 70, 42, 14, Color);
 
}

void ClockLayout2::Points1(uint16_t Numdigit, uint16_t Color)
// display of 1  ==================================
{
  _lcd->fillRect (0 + Numdigit, 0 + Ypos, 56 , 84, TFT_BLACK);

  _lcd->fillRect(Numdigit + 14, Ypos + 70, 42, 14, Color);  
  _lcd->fillRect(Numdigit + 28, Ypos +  0, 14, 70, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos + 14, 14, 14, Color);  
}

void ClockLayout2::Points2(uint16_t Numdigit, uint16_t Color)
// display of 2 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos + 14, 14, 14, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 28, 56, 14, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 28, 14, 42, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 70, 56, 14, Color);  
}

void ClockLayout2::Points3(uint16_t Numdigit, uint16_t Color)
// display of 3 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos +  0, 14, 84, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos + 28, 42, 14, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 70, 56, 14, Color);  
}

void ClockLayout2::Points4(uint16_t Numdigit, uint16_t Color)
// display of 4 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 14, 42, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 42, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 28, Ypos + 28, 14, 56, Color);  
}

void ClockLayout2::Points5(uint16_t Numdigit, uint16_t Color)
// display of 5 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 56, 14, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 14, 14, 28, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 28, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos + 42, 14, 28, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 70, 56, 14, Color);  
}

void ClockLayout2::Points6(uint16_t Numdigit, uint16_t Color)
// display of 6 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56 , 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 56, 14, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 14, 14, 70, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 28, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos + 42, 14, 28, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 70, 56, 14, Color);  
}

void ClockLayout2::Points7(uint16_t Numdigit, uint16_t Color)
// display of 7 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos + 14, 14, 14, Color);  
  _lcd->fillRect(Numdigit + 28, Ypos + 28, 14, 28, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos + 56, 14, 28, Color);  
}

void ClockLayout2::Points8(uint16_t Numdigit, uint16_t Color)
// display of 8 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 14, 84, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos +  0, 42, 14, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos + 28, 42, 14, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos + 70, 42, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos +  0, 14, 84, Color);  
}

void ClockLayout2::Points9(uint16_t Numdigit, uint16_t Color)
// display of 9 ==================================
{
  _lcd->fillRect(0 + Numdigit, 0 + Ypos, 56, 84, TFT_BLACK);

  _lcd->fillRect(Numdigit +  0, Ypos +  0, 14, 42, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos +  0, 42, 14, Color);  
  _lcd->fillRect(Numdigit + 14, Ypos + 28, 42, 14, Color);  
  _lcd->fillRect(Numdigit +  0, Ypos + 70, 56, 14, Color);  
  _lcd->fillRect(Numdigit + 42, Ypos +  0, 14, 84, Color);  
}

void ClockLayout2::PointsColumn()
// display of : ==================================
{

  uint16_t Color = _lcd->color565(255,255,255);
  int DP = 150;                                   // horizontale position of ":"

  _lcd->fillRect(DP, 0 + Ypos, 25, 84, TFT_BLACK);
  _lcd->fillRect(DP + 0, Ypos + 28, 14, 14, Color);         
  _lcd->fillRect(DP + 0, Ypos + 56, 14, 14, Color);         
}



void ClockLayout2::fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

  byte seg = 6; // Segments are 3 degrees wide = 60 segments for 360 degrees
  byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {
    
    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    _lcd->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    _lcd->fillTriangle(x1, y1, x2, y2, x3, y3, colour);
  
    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
   
  }
}

void ClockLayout2::DrawSeconds(uint8_t seconds, uint16_t color){
  seconds++; //We will run from 1 to 60;
  if(seconds>60){
    seconds=60;
  }
  fillArc(58,188,0, seconds ,45,45,8,color);
}

void ClockLayout2::DrawAlarmONOFF( bool ON ){
  _lcd->setCursor(0, 15, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  _lcd->fillRect(0, 0, 100, 40, TFT_BLACK);  
  _lcd->setTextColor(TFT_WHITE,TFT_BLACK);
  _lcd->setFreeFont(FSB9); 
  _lcd->print("Alarm: ");
  if(true==ON){
      _lcd->println("On");
  } else {
      _lcd->println("Off");
  }
  
}

void ClockLayout2::DrawAlarmNEXT( uint8_t hour, uint8_t minute, uint8_t dow, bool clear){
  
  if(false == clear){
    _lcd->fillRect(180, 0, 320, 40, TFT_BLACK);  
    _lcd->setCursor(180, 15, 2);
    _lcd->setTextColor(TFT_WHITE,TFT_BLACK);
    _lcd->setFreeFont(FSB9); 
    _lcd->print("Next: ");
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
  _lcd->setCursor(230, 32, 2);
  _lcd->printf("%02d",hour);
  _lcd->print(":");
  _lcd->printf("%02d",minute);

  } else {
      //Clear the area 
    _lcd->fillRect(180, 0, 320, 40, TFT_BLACK);
  }
  
}

void ClockLayout2::DrawDate( uint16_t year, uint8_t month, uint8_t day, uint8_t dow ){
   _lcd->fillRect(130, 140, 320, 220, TFT_BLACK);
  _lcd->setCursor(130, 180, 2);
  _lcd->setTextColor(TFT_WHITE,TFT_BLACK);
  _lcd->setFreeFont(FSB18); 
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


  _lcd->setCursor(130, 205, 2);
  _lcd->setFreeFont(FSB9); 
  _lcd->print(day);
  _lcd->print(". ");
  switch(month){
      case 0:{
        _lcd->print("January");
      }break;

      case 1:{
        _lcd->print("February");
      }break;

      case 2:{
        _lcd->print("March");
      }break;

      case 3:{
        _lcd->print("April");   
      }break;

      case 4:{
          _lcd->print("May");
      }break;

      case 5:{
          _lcd->print("June");
      }break;

      case 6:{
        _lcd->print("July");
      }break;

      case 7:{
        _lcd->print("August");
      }break;

      case 8:{
        _lcd->print("September");
      }break;

      case 9:{
        _lcd->print("October");
      }break;

      case 10:{
        _lcd->print("November");
      }break;

      case 11:{
        _lcd->print("December");
      }break;

      default:{
        _lcd->print("Grommec");
      }break;

  }
 
  _lcd->print(" ");
  _lcd->print(year);
  
}
