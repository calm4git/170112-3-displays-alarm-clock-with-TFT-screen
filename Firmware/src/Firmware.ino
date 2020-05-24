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
  tm.Hour=8;
  tm.Minute=5;
  tm.Second=48;
  tm.Year=(2020-1970);
  utc_now = makeTime(tm);
  Face[F]->UpdateTime(utc_now);
  
}


//The Clock will use utc timesamps 
void loop() {
 
  static uint32_t ticks =0;
  static uint32_t ft=0;
   
  
  
 
  delay(10);
  utc_now++;
  ft++;
  Alarms.CheckAlarms(utc_now);
  if( ft> 15*60 ){
      F++;
      ft=0;
      if(F >=   3   ){
        F=0;
      }
      Face[F]->ForceScreenRefresh();
      Face[F]->UpdateTime(utc_now);
  }

  ticks++;


  if(ticks==50){
    utc_now+=SECS_PER_DAY;
  }
  
  if(ticks==190){
     utc_now+=SECS_PER_WEEK*4;
  }


  //increase time and let the display draw
  Face[F]->UpdateTime(utc_now);
  if(ticks<100){
    Face[F]->UpdateAlarmInfo(false,  2,  9,  6);
  } else {
    Face[F]->UpdateAlarmInfo(true,  2,  9,  6);
    if(ticks>200){
      ticks=0;
    }
  }

  


}
