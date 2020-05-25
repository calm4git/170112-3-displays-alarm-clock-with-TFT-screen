#include "SPI.h"
#include "TFT_eSPI.h"
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
void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);
  F=2;
  Face[F]->ForceScreenRefresh();
  tm.Hour=23;
  tm.Minute=54;
  tm.Second=48;
  tm.Year=(2020-1970);
  utc_now = makeTime(tm);
  Face[F]->UpdateTime(utc_now);
  Alarms.EnableAlarms();
  Alarm::Alarmtime_t At;
  
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


//The Clock will use utc timesamps 
void loop() {
 
  static uint32_t ticks =0;
  static uint32_t ft=0;
   
  
  
  delay(2);
  //increase time and let the display draw
  utc_now+=20;
  ft++;
  Alarms.CheckAlarms(utc_now);
  if( ft> 340*60 ){
      F++;
      ft=0;
      if(F >=   3   ){
        F=0;
      }
      Face[F]->ForceScreenRefresh();
      Face[F]->UpdateTime(utc_now);
  }

  ticks++;
  
  if(true==Alarms.CheckAlarms(utc_now)){
    //We need to ring 
    Serial.printf("Ring Ring @ %u \n\r",utc_now );
  }
  Alarm* Al_Ptr=NULL;
  if(true == Alarms.GetNextAlarm(&Al_Ptr,utc_now)){
    time_t altime = Al_Ptr->GetNextAlarmTime(utc_now);
    Face[F]->UpdateAlarmInfo(true,altime);
  } else {
    Face[F]->UpdateAlarmInfo(false,0);
  }
  
  Face[F]->UpdateTime(utc_now);
  
  


}
