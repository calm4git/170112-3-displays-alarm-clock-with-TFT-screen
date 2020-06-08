#include <Ticker.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
#include "src/Faces/Faces.h"
#include "src/Faces/layout1/layout1.h"
#include "src/Faces/layout2/layout2.h"
#include "src/Faces/layout3/layout3.h"
#include "src/AlarmMgr/AlarmMgr.h"
#include "src/Menus/menus.h"
#include <TimeLib.h>
#include "src/HAL/Clock_HAL.h"

Ticker InternalRTC;


TFT_eSPI lcd = TFT_eSPI();      

ClockLayout3 L3 = ClockLayout3(lcd);
ClockLayout2 L2 = ClockLayout2(lcd);
ClockLayout1 L1 = ClockLayout1(lcd);

AlarmMgr Alarms = AlarmMgr();

Menus Menu = Menus(lcd,Alarms);

Faces* Face[] = {&L1, &L2, &L3 };

static uint8_t F=0;
tmElements_t tm;
volatile time_t utc_now;

void inc_utc_now() {
  utc_now++;
}

void setup_clock( void ){

  F=0;
  Face[F]->ForceScreenRefresh();
  tm.Hour=23;
  tm.Minute=54;
  tm.Second=56;
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
  InternalRTC.attach(0.1, inc_utc_now);
}

/*  This if a Renderingtest for Time and Date Setting / Setup 
    We have : Hour Minute ( Second ) , but for this clock we will set seconds to 0 after the edit ?!
    Also    : Year Month Day , the DOW will calculated automaticaly

    Also this will be used to set the LDR and calibrate it to something usefull


*/

//We need to set if we have already drawn stuff to reduce flicker 
//as good as possible





time_t GetTime( void ){
  return utc_now ;
}

void SetTime( time_t tm ){
  utc_now = tm;
}

void UpdateClock( time_t utc_now , bool forcerefresh ){
  static time_t last_call = 0;
  if( (last_call == utc_now) && (false == forcerefresh) ) {
    //We do nothing...
  } else {
    last_call = utc_now;
    Alarms.CheckAlarms(utc_now);
    if( 0 == ( utc_now % SECS_PER_HOUR )  ){
        F++;
        if(F >=   3   ){
          F=0;
        }
        forcerefresh=true;
    }
    
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

    if( true == forcerefresh ){
      Face[F]->ForceScreenRefresh();
    }
    Face[F]->UpdateTime(utc_now);
  }
}

void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.init();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);
  
  setup_clock();
  Menu.SetCallBackForGetTime( GetTime );
  Menu.SetCallBackForSetTime( SetTime );
  
 
}


//The Clock will use utc timesamps 
void loop() {
 
  //We run only once a second to update the alarms and display if the second updated....
  //increase time and let the display draw
  static bool NoMenuRenderd=true;
  char key=0;
  Menus::KeyAction_t Key=Menus::NothingPressed;
  //We use the the serial interface 
  while(Serial.available()){
    key=Serial.read();

    if(key=='a'){
        Key=Menus::KeyA_Pressed;
    }

    if(key=='b'){
        Key=Menus::KeyB_Pressed;
    }

    if(key=='c'){
        Key=Menus::KeyC_Pressed;
    }

    if(key=='d'){
        Key=Menus::KeyD_Pressed;
    }
    Serial.print(key);

  }

  if( true == Menu.RenderMenu(Key, utc_now) ){
    //We can render the clockfaces
    UpdateClock(utc_now,  ( false == NoMenuRenderd ) );
    NoMenuRenderd = true;

  } else {
    //Menu has been drawn...
    NoMenuRenderd = false;

  }
  

}
