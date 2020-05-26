#include "SPI.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
#include "src/Faces/Faces.h"
#include "src/Faces/layout1/layout1.h"
#include "src/Faces/layout2/layout2.h"
#include "src/Faces/layout3/layout3.h"
#include "src/AlarmMgr/AlarmMgr.h"
#include <TimeLib.h>

TFT_eSPI lcd = TFT_eSPI();       // Invoke custom library

ClockLayout3 L3 = ClockLayout3(lcd);
ClockLayout2 L2 = ClockLayout2(lcd);
ClockLayout1 L1 = ClockLayout1(lcd);

AlarmMgr Alarms = AlarmMgr();

Faces* Face[] = {&L1, &L2, &L3 };

static uint8_t F=0;
tmElements_t tm;
time_t utc_now;

void setup_clock( void ){

  F=2;
  Face[F]->ForceScreenRefresh();
  tm.Hour=23;
  tm.Minute=54;
  tm.Second=48;
  tm.Year=(2020-1970);
  utc_now = makeTime(tm);
  Face[F]->UpdateTime(utc_now);
  
  Alarm::Alarmtime_t At;
  Alarms.EnableAlarms();
  
  At.Day=0;
  At.IsRinging=0;
  
  At.Hour=8;
  At.Minute=22;
  At.Second=5;
  
  At.OneShot=0;
  At.Monday=1;
  
  At.UseDate=0;
  At.Year=2019;
  At.Enabled = 1;

  Alarms.SetAlarm(0,At);
  //We will set the alarm 

  At.Day=0;
  At.IsRinging=0;
  
  At.Hour=9;
  At.Minute=11;
  At.Second=2;
  
  At.OneShot=0;
  At.Monday=0;
  At.Wednesday=1;
  At.Friday=0;

  At.UseDate=0;
  At.Year=2019;
  At.Enabled = 1;

  Alarms.SetAlarm(1,At);
  

  At.Day=0;
  At.IsRinging=0;
  
  At.Hour=10;
  At.Minute=01;
  At.Second=01;
  
  At.OneShot=0;
  At.Monday=0;
  At.Wednesday=1;
  At.Friday=1;

  At.UseDate=0;
  At.Year=2019;
  At.Enabled = 1;

  Alarms.SetAlarm(2,At);
}

void RenderAlarmMenuEntry( uint32_t offset, bool selected,  Alarm::Alarmtime_t At  ){
  uint8_t active_days=0;
  
  if(selected == true ){
    lcd.fillRect(0,40+offset,300,40,lcd.color565( 92,96,102 ) );
  } else {
     lcd.fillRect(0,40+offset,300,40,TFT_WHITE );
  }
  lcd.drawRect(0,40+offset,300,40,lcd.color565( 112,116,122 ));
  lcd.setTextColor(TFT_BLACK,TFT_BLACK);
  lcd.setFreeFont(FSB12); 
  lcd.setCursor(0,65+offset);
  
  if(selected == true ){
    lcd.print(" >");
  }
  
  lcd.setCursor(26,70+offset);
  if(false == At.Enabled){
    lcd.print("Alarm disabled");
  } else {
    if(true == At.UseDate){
        if(At.Hour<10){
          lcd.print("0");
        } 
        lcd.print(At.Hour);
        lcd.print(":");
        if(At.Minute<10){
          lcd.print("0");
        }
        lcd.print(At.Minute);
        lcd.setFreeFont(FSB9); 
        lcd.setCursor(120,47+offset);
        lcd.print(At.Day);
        lcd.print(".");
        lcd.print(At.Month);
        lcd.print(".");
        lcd.print(At.Year);

    } else {
        lcd.setCursor(30,67+offset);
        if(At.Hour<10){
          lcd.print("0");
        } 
        lcd.print(At.Hour);
        lcd.print(":");
        if(At.Minute<10){
          lcd.print("0");
        }
        lcd.print(At.Minute);
        lcd.setFreeFont(FSB9); 
        lcd.setCursor(120,57+offset);
        //We generate the Dow entry...
        if(At.Monday>0){
          lcd.print("Mo");
          active_days++;
          
          if(active_days==5){
             lcd.setCursor(120,74+offset);
          }
        }

        if(At.Tursday>0){
          if(active_days>0){
            lcd.print(", ");
          }
          
          lcd.print("Tu");
          active_days++;
          
          if(active_days==5){
             lcd.setCursor(120,74+offset);
          }

        }
        
        if(At.Wednesday>0){
          if(active_days>0){
            lcd.print(", ");
          }
          
          lcd.print("We");
          active_days++;

          if(active_days==5){
             lcd.setCursor(120,74+offset);
          }
        }

        if(At.Thuseday>0){
          if(active_days>0){
            lcd.print(", ");
          }
          
          lcd.print("Th");
          active_days++;

          if(active_days==5){
              lcd.setCursor(120,74+offset);
          }
        }

        if(At.Friday>0){
          if(active_days>0){
            lcd.print(", ");
          }
          
          lcd.print("Fr");
          active_days++;

          if(active_days==5){
              lcd.setCursor(120,74+offset);
          }
        }

       
        
        if(At.Saturday > 0){
          if(active_days>0){
            lcd.print(", ");
          }
          
          lcd.print("Sa");
          active_days++;

          if(active_days==5){
              lcd.setCursor(120,74+offset);
          }
        }
        if(At.Sunday > 0){
          if(active_days>0){
            lcd.print(", ");
          }
          
          lcd.print("Su");
          active_days++;

          if(active_days==5){
              lcd.setCursor(120,74+offset);
          }
        }
        
        lcd.setCursor(230,74+offset);
        
        if(true == At.OneShot){
          lcd.print("Once");
        }
    }
  }
}

void TestRenderMenuAlarms( uint8_t selected_idx){
  Alarm::Alarmtime_t At ;
  /* Draw Headline */
  lcd.fillRect(0,0,320,40,lcd.color565( 45,47,50 ) );
  
  lcd.setTextColor(lcd.color565( 112,116,122 ),TFT_BLACK);
  lcd.setFreeFont(FSB18); 
  lcd.setCursor(160- ( lcd.textWidth("Alarms") / 2) ,30);
  lcd.print("Alarms");
  //Draw scrolling element
  lcd.fillRect(302,43,15,196,TFT_WHITE);
  lcd.drawRect(300,40,20,200,lcd.color565( 112,116,122 ));
  lcd.drawRect(301,41,18,198,lcd.color565( 112,116,122 ));
  lcd.drawRect(302,42,16,196,lcd.color565( 112,116,122 ));
  //Next is to draw the elements as far as possibel....

  
  //We calcualte the range we need to draw or like to draw
  //We can display 5 Elements 
  uint8_t startidx=0;
  if(selected_idx>4){
    startidx = selected_idx -4 ;
  }

  //We set the scrollbar accordingly
  lcd.fillRect(305,45 +(196/8)*selected_idx ,10,(196/8)-2 , TFT_DARKGREY);

  for(uint8_t i=startidx;i<8;i++){
    if(true == Alarms.GetAlarm(i,&At) ){
      RenderAlarmMenuEntry( (40*(i-startidx))  ,( i==selected_idx ), At );
    } 
  }
 


}




void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);
  
  setup_clock();

  
  
}


void UpdateClock( time_t utc_now ){
  Alarms.CheckAlarms(utc_now);
  if( 0 == ( utc_now % 60 )  ){
      F++;
      if(F >=   3   ){
        F=0;
      }
      Face[F]->ForceScreenRefresh();
  }
  Face[F]->UpdateTime(utc_now);
  Alarm* Al_Ptr=NULL;
  if(true == Alarms.GetNextAlarm(&Al_Ptr,utc_now)){
    time_t altime = Al_Ptr->GetNextAlarmTime(utc_now);
    Face[F]->UpdateAlarmInfo(true,altime);
  } else {
    Face[F]->UpdateAlarmInfo(false,0);
  }
  
  if(true==Alarms.CheckAlarms(utc_now)){
    //We need to ring 
    Serial.printf("Ring Ring @ %u \n\r",utc_now );
    //Signal that we do a kind of ring or play a mp3....

  }
}




//The Clock will use utc timesamps 
void loop() {
  //We run only once a second to update the alarms and display if the second updated....
  //increase time and let the display draw
  for(uint8_t i=0;i<7;i++){
    TestRenderMenuAlarms( i );
    delay(500);
  }

  for(uint8_t i=0;i<7;i++){
    TestRenderMenuAlarms( 7-i );
    delay(500);
  }
 

  
  


}
