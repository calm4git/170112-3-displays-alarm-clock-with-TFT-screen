#include <TFTShape.h>
#include "../../../Free_Fonts.h" // Include the header file attached to this sketch
#include "layout3.h"


  ClockLayout3::ClockLayout3( TFT_eSPI& lcd ){
    
    dozenH_Prev=-1;
    unitH_Prev=-1;
    dozenM_Prev=-1;
    unitM_Prev=-1;

    Year_Prev=-1;
    Month_Prev=-1;
    Day_Prev=-1;
    Dow_Prev=-1;

    RedrawAlarm=true;
    Alarm_Enable=false;
    AlarmDow=0;
    AlarmHour=0;
    AlarmMinute=0;
    BargraphDrawn=false;
    _lcd=&lcd;

  }

 void ClockLayout3::UpdateTime( uint16_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t minute, uint8_t second ){


   

  // ============= display each digit in a "for" loop
  uint16_t Numdigit;
  uint8_t  digit;                                        // value of each digit to display
  uint8_t  i; 
                                                         // digit counter, 1 (= left, dozenH) to 4 (= right, unitM)

                                                         
uint8_t Red=0;
uint8_t Green=0;
uint8_t Blue=0;

  // Numdigit is the X position of the 7 segments

  //                 [chiffre, Numdigit, Red, Green, Blue] [..] [..] [..]
  uint8_t Values[][5] = {{ (uint8_t)(hour / 10) ,    8,  51, 255, 153},
                        {  (uint8_t)(hour % 10),   80, 255,   0,  51},
                        {  (uint8_t)(minute  / 10),  180, 255, 204,   0},
                        {  (uint8_t)(minute  % 10),  248, 127, 127, 255}
  };

  for ( i = 0; i < 4; i++){
  
    digit = Values[i][0]; 
    if( 1 == 1 ){
             
              Numdigit = Values[i][1]; Red = Values[i][2]; Green = Values[i][3]; Blue = Values[i][4];
              
              
              switch (digit)  // read and display the numerical value (1) for each digit position (i)
              {
                case 0: {
                    /*SegmentG();                            // Erease the previous segments
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    SegmentA(); SegmentB(); SegmentC(); SegmentD();
                    SegmentE(); SegmentF(); */
                    Segment('G',Numdigit,TFT_BLACK);
                   
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('E',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('F',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                  } break;
          
                case 1: {
                  /*
                    SegmentA(); SegmentD();
                    SegmentE(); SegmentF(); SegmentG();    // Erease the previous segments
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    SegmentB(); SegmentC();
                    */
          
                    Segment('A',Numdigit,TFT_BLACK);
                    Segment('D',Numdigit,TFT_BLACK);
                    Segment('E',Numdigit,TFT_BLACK);
                    Segment('F',Numdigit,TFT_BLACK);
                    Segment('G',Numdigit,TFT_BLACK);
                   
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                    
                  } break;
          
                case 2: {
                    Segment('C',Numdigit,TFT_BLACK);
                    Segment('F',Numdigit,TFT_BLACK);
                   
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('E',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                    
                  } break;
          
                case 3: {
                    Segment('E',Numdigit,TFT_BLACK);
                    Segment('F',Numdigit,TFT_BLACK);
                    
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                  } break;
          
                case 4: {
               
                    Segment('A',Numdigit,TFT_BLACK);
                    Segment('D',Numdigit,TFT_BLACK);
                    Segment('E',Numdigit,TFT_BLACK);
                   
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('F',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                  } break;
          
                case 5: {
               
                    Segment('B',Numdigit,TFT_BLACK);
                    Segment('E',Numdigit,TFT_BLACK);
                    
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('F',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                  } break;
          
                case 6: {
                  
          
                    Segment('B',Numdigit,TFT_BLACK);
                    
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('E',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('F',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                  } break;
          
                case 7: {
                 
                    Segment('D',Numdigit,TFT_BLACK);
                    Segment('E',Numdigit,TFT_BLACK);
                    Segment('F',Numdigit,TFT_BLACK);
                    Segment('G',Numdigit,TFT_BLACK);
                    
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    
                  } break;
          
                case 8: {
                   
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('E',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('F',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                  } break;
          
                case 9: {
                  
                    Segment('A',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('B',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('C',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('D',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('F',Numdigit,_lcd->color565(Red, Green, Blue));
                    Segment('G',Numdigit,_lcd->color565(Red, Green, Blue));
                  
                  } break;
              }                // end of switch
             } // end of if
  }                  // end of for_i loop
  Colon(second);
  SecondsBargraph(second);

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
 void ClockLayout3::UpdateAlarmInfo(bool Enabled, uint8_t dow, uint8_t hour, uint8_t minute){
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
 void ClockLayout3::ForceScreenRefresh( void ){
    dozenH_Prev=-1;
    unitH_Prev=-1;
    dozenM_Prev=-1;
    unitM_Prev=-1;

    Year_Prev=-1;
    Month_Prev=-1;
    Day_Prev=-1;
    Dow_Prev=-1;
    BargraphDrawn=false;
    RedrawAlarm=true;
    
 }


// ============= deux points
void ClockLayout3::Colon( uint8_t seconds)
{
  const int colonXpos = 160;
  const int colonYpos = 80;
  const int colonYOffset = 40;
  
    if( (seconds%2) == 0){
        _lcd->fillCircle(colonXpos, colonYpos, 7, _lcd->color565(0, 255, 255));
        _lcd->fillCircle(colonXpos, colonYpos + colonYOffset, 7, _lcd->color565(255,   0,   0));
    } else {
        _lcd->fillCircle(colonXpos, colonYpos, 7, _lcd->color565(255,   0,   0));
        _lcd->fillCircle(colonXpos, colonYpos+ colonYOffset, 7, _lcd->color565(0, 255, 255));
    }
    
}

// ============================= end of SevenSegments
//       -a-
//    f |    |  b
//        -g-
//    e |    |  c
//       -d-     dp
//
void ClockLayout3::Segment( uint8_t idx, uint16_t offset, uint16_t color){

  Serial.printf("Print Segemtn %c , Offset %u, Color %u\n\r", idx,offset,color);
  uint16_t x0 = 0;
  uint16_t y0 = 0;
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = 0;
  uint16_t x3 = 0;
  uint16_t y3 = 0;


  VEC2 vectorpoints[4];

  switch(idx){
    case 'A':{
      x0 = Xpos +  4 + offset;
      y0 = Ypos +  0;
      x1 = Xpos + 56 + offset;
      y1 = Ypos +  0;
      x2 = Xpos + 45 + offset;
      y2 = Ypos + 11;
      x3 = Xpos + 14 + offset;
      y3 = Ypos + 11;
      
    } break;

    case 'B':{
         x0 = Xpos + 60 + offset;
         y0 = Ypos + 1;
         x1 = Xpos + 60 + offset;
         y1 = Ypos + 59;
         x2 = Xpos + 50 + offset;
         y2 = Ypos + 49;
         x3 = Xpos + 50 + offset;
         y3 = Ypos + 11;
      
    } break;

    case 'C':{

       x0 = Xpos + 60 + offset;
       y0 = Ypos + 61;
       x1 = Xpos + 60 + offset;
       y1 = Ypos + 119;
       x2 = Xpos + 50 + offset;
       y2 = Ypos + 109;
       x3 = Xpos + 50 + offset;
       y3 = Ypos + 71;
      
    } break;

    case 'D':{

       x0 = Xpos + 56 + offset;
       y0 = Ypos + 120;
       x1 = Xpos +  3 + offset;
       y1 = Ypos + 120;
       x2 = Xpos + 12 + offset;
       y2 = Ypos + 111;
       x3 = Xpos + 47 + offset;
       y3 = Ypos + 111;
  
    } break;

    case 'E':{

       x0 = Xpos +  0 + offset;
       y0 = Ypos + 61;
       x1 = Xpos + 10 + offset;
       y1 = Ypos + 71;
       x2 = Xpos + 10 + offset;
       y2 = Ypos + 109;
       x3 = Xpos +  0 + offset;
       y3 = Ypos + 119;
  
    } break;

    case 'F':{
    
       x0 = Xpos +  0 + offset;
       y0 = Ypos +  1;
       x1 = Xpos + 10 + offset;
       y1 = Ypos + 11;
       x2 = Xpos + 10 + offset;
       y2 = Ypos + 49;
       x3 = Xpos +  0 + offset;
       y3 = Ypos + 59;
          
    } break;

    case 'G':{
    

   
     x0 = Xpos + 10 + offset;
     y0 = Ypos + 55;
     x1 = Xpos + 49 + offset;
     y1 = Ypos + 55;
     x2 = Xpos + 55 + offset;
     y2 = Ypos + 60;
     x3 = Xpos +  5 + offset;
     y3 = Ypos + 60;

   
    vectorpoints[0].x=x0;
    vectorpoints[0].y=y0;
  
    vectorpoints[1].x=x1;
    vectorpoints[1].y=y1;
  

    vectorpoints[2].x=x2;
    vectorpoints[2].y=y2;
  
    vectorpoints[3].x=x3;
    vectorpoints[3].y=y3;
    TFTShape poly(vectorpoints,4 );
    poly.fill(_lcd,0,0,color);
    
      x0 = Xpos +  5 + offset;
      y0 = Ypos + 60;
      x1 = Xpos + 55 + offset;
      y1 = Ypos + 60;
      x2 = Xpos + 50 + offset;
      y2 = Ypos + 65;
      x3 = Xpos + 10 + offset;
      y3 = Ypos + 65;
      
    } break;
  
  }
    vectorpoints[0].x=x0;
    vectorpoints[0].y=y0;
  
    vectorpoints[1].x=x1;
    vectorpoints[1].y=y1;
  

    vectorpoints[2].x=x2;
    vectorpoints[2].y=y2;
  
    vectorpoints[3].x=x3;
    vectorpoints[3].y=y3;
    TFTShape poly(vectorpoints,4 );
    poly.fill(_lcd,0,0,color);
  
}


void ClockLayout3::SecondsBargraph( uint8_t seconds)
{
  
  if( (false == BargraphDrawn) || ( 0== seconds ) ) {
    _lcd->fillRect(11, 174, 302, 11, _lcd->color565(0 , 0, 0));
    
    _lcd->drawRect(10, 175, 301, 11,_lcd->color565(255, 255, 255));

    for (uint16_t i = 1; i <= 60; i++) {
      _lcd->drawFastVLine( (5 * i) + 5, 176, 9,_lcd->color565(255, 255, 255));  // secondes separator
    }
    for(uint8_t i=0;i<seconds;i++){
      _lcd->fillRect( (i * 5) + 11, 176, 4, 9, _lcd->color565(255 , 255, 255));
    }
    BargraphDrawn=true;
    
  } 
  
  _lcd->fillRect( (seconds * 5) + 11, 176, 4, 9, _lcd->color565(255 , 255, 255));
  


}

void ClockLayout3::DrawAlarmONOFF( bool ON ){
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

void ClockLayout3::DrawAlarmNEXT( uint8_t hour, uint8_t minute, uint8_t dow, bool clear){
  
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

void ClockLayout3::DrawDate( uint16_t year, uint8_t month, uint8_t day, uint8_t dow ){
   _lcd->fillRect(5, 190, 320, 220, TFT_BLACK);
  _lcd->setCursor(5, 220, 2);
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
  _lcd->setFreeFont(FSB12); 
  _lcd->print(" ");
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
 
  _lcd->print(". ");
  _lcd->print(year);
  
}


