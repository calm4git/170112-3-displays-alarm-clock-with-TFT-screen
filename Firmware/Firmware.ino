#include "SPI.h"
#include "TFT_eSPI.h"
#include "src/layout1.h"
#include "src/layout2.h"
#include "src/layout3.h"


TFT_eSPI lcd = TFT_eSPI();       // Invoke custom library

ClockLayout3 L3 = ClockLayout3(lcd);
ClockLayout2 L2 = ClockLayout2(lcd);
ClockLayout1 L1 = ClockLayout1(lcd);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);
  /*
  L2.UpdateTime(2020,5,1,0,8,5,48);
  L2.ForceScreenRefresh();
  */
  L3.UpdateTime(2020,5,1,0,8,5,48);
  L3.ForceScreenRefresh();
}

void loop() {
  static uint32_t ticks =0;
  static uint8_t hour = 8;
  static uint8_t minute = 5;
  static uint8_t second = 45;
  static uint16_t Year = 2020;
  static uint8_t Month = 0;
  static uint8_t Day=1;
  static uint8_t DOW = 0;
  // put your main code here, to run repeatedly:
  delay(10);
  second++;
  if(second >=60){
    second =0;
    minute++;
    if(minute>=60){
      minute=0;
      hour++;
      if(hour>=24){
        hour=0;
      }
    }
  }
  ticks++;
  if(ticks==50){
    Day++;
    if(Day>31){
      Day=1;
    }
    DOW++;
    if(DOW>6){
      DOW=0;
    }

  }

  if(ticks==190){
     Month++;
     if(Month>11){
       Month=0;
     }
  }


  //increase time and let the display draw
  L3.UpdateTime(Year,Month,Day,DOW,hour,minute,second);
  if(ticks<100){
    L3.UpdateAlarmInfo(false,  2,  9,  6);
  } else {
    L3.UpdateAlarmInfo(true,  2,  9,  6);
    if(ticks>200){
      ticks=0;
    }
  }

}
