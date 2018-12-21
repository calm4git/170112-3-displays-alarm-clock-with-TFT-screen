#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Hash.h>
#include <ESPAsyncUDP.h>
#include <SPI.h>
#include <FS.h>


#include "timecore.h"
#include "datastore.h"

#include <Wire.h>
#include "RTClib.h"
#include <Ticker.h>

#include <TimeLib.h>
#include "NTP_Client.h"

#include "display.h"

RTC_DS3231 rtc_clock;

Timecore timec;
NTP_Client NTPC;

Ticker TimeKeeper;
/* 63 Char max and 17 missign for the mac */





uint32_t RTC_ReadUnixTimeStamp(bool* delayed_result){
 DateTime now = rtc_clock.now();
 *delayed_result=false;
 return now.unixtime();
}

void RTC_WriteUnixTimestamp( uint32_t ts){
  rtc_clock.adjust(DateTime( ts));
}



/*SETUP*/
void setup()
{
  uint8_t connect_cnt=0;
 // We start by connecting to a WiFi network
  Serial.begin (115200);
  datastoresetup();
  SPIFFS.begin();
  /* we need to wait for a second */
  Serial.println("Booting...");
  delay(1000);
  Serial.println("Init LCD");   
  InitDisplay();
  Serial.println("Init WiFi");   
  initWiFi();
  TimeKeeper.attach_ms(10, callback);
  Serial.println("Read Timecore Config");
  timecoreconf_t cfg = read_timecoreconf();
  timec.SetConfig(cfg);

  
  if (! rtc_clock.begin()) {
    Serial.println("Couldn't find RTC");
   
  } else {
    DateTime now = rtc_clock.now();
      
    rtc_source_t I2C_DS3231;

    I2C_DS3231.SecondTick=NULL;
    I2C_DS3231.type = RTC_CLOCK;
    I2C_DS3231.ReadTime=RTC_ReadUnixTimeStamp;
    I2C_DS3231.WriteTime=RTC_WriteUnixTimestamp;
    timec.RegisterTimeSource(I2C_DS3231);
    
      
    timec.SetUTC(now.unixtime()  , RTC_CLOCK );
  
  
  }

  Serial.println("Setup NTP now");
  NTPC.ReadSettings();
  NTPC.begin( &timec );
  NTPC.Sync();
  SetupDisplay();
}

void NTPC_Tick( ){
 
}

/* we need to presace it to 1hz for the button i guesst ....*/
/* We use 100Hz */

void callback()
{
  static uint8_t prescaler_1s=0;
  prescaler_1s++;
  if(prescaler_1s>=100){
    prescaler_1s=0;
    /*Increment time */ 
   
    //Serial.println("+");
    timec.RTC_Tick();
    NTPC.Tick();
     
  }
  
  /* this dose timekeeping */
}














/*==================================================*/
/*                 MAIN LOOP                        */
/*==================================================*/
void loop()
{
NetworkTask();
DisplayTask();
  
  // ================= END OF MAIN LOOP ======================
}












 


