
//#include <ESP8266WebServer.h>
//#include <ESP8266WebServerSecure.h>
#include <Arduino.h>
#include <WebServer.h>

#include <ArduinoJson.h>
#include "timecore.h"
#include "NTP_Client.h"
#include "datastore.h"

#include "webfunctions.h"

extern Timecore timec;
extern NTP_Client NTPC;
extern void displayrefesh( void );
extern void sendData(String data);
//extern ESP8266WebServer * server;
extern WebServer * server;

void response_settings(){
StaticJsonBuffer<350> jsonBuffer;
char strbuffer[129];
String response="";  
  
  JsonObject& root = jsonBuffer.createObject();

  
  memset(strbuffer,0,129);
  datum_t d = timec.GetLocalTimeDate();
  snprintf(strbuffer,64,"%02d:%02d:%02d",d.hour,d.minute,d.second);
  //strbuffer;

  root["time"] = strbuffer;
 
  memset(strbuffer,0,129);
  snprintf(strbuffer,64,"%04d-%02d-%02d",d.year,d.month,d.day);
  root["date"] = strbuffer;

  memset(strbuffer,0,129);
  snprintf(strbuffer,129,"%s",NTPC.GetServerName());
  root["ntpname"] = strbuffer;
  root["tzidx"] = (int32_t)timec.GetTimeZone();
  root["ntpena"] = NTPC.GetNTPSyncEna();
  root["ntp_update_span"]=NTPC.GetSyncInterval();
  root["zoneoverride"]=timec.GetZimeZoneManual();;
  root["gmtoffset"]=timec.GetGMT_Offset();;
  root["dlsdis"]=!timec.GetAutomacitDLS();
  root["dlsmanena"]=timec.GetManualDLSEna();
  uint32_t idx = timec.GetDLS_Offset();
  root["dlsmanidx"]=idx;
  root.printTo(response);
  sendData(response);
}

 void settime_update( ){ /* needs to process date and time */
  Serial.println("POST ");
  datum_t d;
  d.year=2000;
  d.month=1;
  d.day=1;
  d.hour=0;
  d.minute=0;
  d.second=0;

  bool time_found=false;
  bool date_found=false;
  
  if( ! server->hasArg("date") || server->arg("date") == NULL ) { // If the POST request doesn't have username and password data
    /* we are missong something here */
  } else {
   
    Serial.printf("found date: %s\n\r",server->arg("date").c_str());
    uint8_t d_len = server->arg("date").length();
    Serial.printf("datelen: %i\n\r",d_len);
    if(server->arg("date").length()!=10){
      Serial.println("date len failed");
    } else {   
      String year=server->arg("date").substring(0,4);
      String month=server->arg("date").substring(5,7);
      String day=server->arg("date").substring(8,10);
      d.year = year.toInt();
      d.month = month.toInt();
      d.day = day.toInt();
      date_found=true;
    }   
  }

  if( ! server->hasArg("time") || server->arg("time") == NULL ) { // If the POST request doesn't have username and password data
    
  } else {
    if(server->arg("time").length()!=8){
      Serial.println("time len failed");
    } else {
    
      String hour=server->arg("time").substring(0,2);
      String minute=server->arg("time").substring(3,5);
      String second=server->arg("time").substring(6,8);
      d.hour = hour.toInt();
      d.minute = minute.toInt();
      d.second = second.toInt();     
      time_found=true;
    }
     
  } 
  if( (time_found==true) && ( date_found==true) ){
    Serial.printf("Date: %i, %i, %i ", d.year , d.month, d.day );
    Serial.printf("Time: %i, %i, %i ", d.hour , d.minute, d.second );
    timec.SetLocalTime(d);
  }
  
  server->send(200);   
  displayrefesh(); 
 }
 
 void ntp_settings_update( ){ /* needs to process NTP_ON, NTPServerName and NTP_UPDTAE_SPAN */



  if( ! server->hasArg("NTP_ON") || server->arg("NTP_ON") == NULL ) { // If the POST request doesn't have username and password data
    NTPC.SetNTPSyncEna(false);  
  } else {
    NTPC.SetNTPSyncEna(true);  
  }

  if( ! server->hasArg("NTPServerName") || server->arg("NTPServerName") == NULL ) { // If the POST request doesn't have username and password data
      
  } else {
    strncpy( NTPC.GetServerName(), server->arg("NTPServerName").c_str(),129);
  }

  if( ! server->hasArg("ntp_update_delta") || server->arg("ntp_update_delta") == NULL ) { // If the POST request doesn't have username and password data
     
  } else {
    NTPC.SetSyncInterval( server->arg("ntp_update_delta").toInt() );
  }
  NTPC.SaveSettings();
  server->send(200);   
  
 }
 
 void timezone_update( ){ /*needs to handel timezoneid */
  if( ! server->hasArg("timezoneid") || server->arg("timezoneid") == NULL ) { // If the POST request doesn't have username and password data
    /* we are missong something here */
  } else {
   
    Serial.printf("New TimeZoneID: %s\n\r",server->arg("timezoneid").c_str());
    uint32_t timezoneid = server->arg("timezoneid").toInt();
    timec.SetTimeZone( (TIMEZONES_NAMES_t)timezoneid );   
  }
  server->send(200);    
  displayrefesh();
 }
 void timezone_overrides_update( ){ /* needs to handle DLSOverrid,  ManualDLS, dls_offset, ZONE_OVERRRIDE and GMT_OFFSET */

  bool DLSOverrid=false;
  bool ManualDLS = false;
  bool ZONE_OVERRRIDE = false;
  int32_t gmt_offset = 0;
  DLTS_OFFSET_t dls_offsetidx = DLST_OFFSET_0;
  if( ! server->hasArg("dlsdis") || server->arg("dlsdis") == NULL ) { // If the POST request doesn't have username and password data
      /* we are missing something here */
  } else {
    DLSOverrid=true;  
  }

  if( ! server->hasArg("dlsmanena") || server->arg("dlsmanena") == NULL ) { // If the POST request doesn't have username and password data
      /* we are missing something here */
  } else {
    ManualDLS=true;  
  }

  if( ! server->hasArg("ZONE_OVERRRIDE") || server->arg("ZONE_OVERRRIDE") == NULL ) { // If the POST request doesn't have username and password data
      /* we are missing something here */
  } else {
    ZONE_OVERRRIDE=true;  
  }

  if( ! server->hasArg("gmtoffset") || server->arg("gmtoffset") == NULL ) { // If the POST request doesn't have username and password data
      /* we are missing something here */
  } else {
    gmt_offset = server->arg("gmtoffset").toInt();
  }

  if( ! server->hasArg("dlsmanidx") || server->arg("dlsmanidx") == NULL ) { // If the POST request doesn't have username and password data
      /* we are missing something here */
  } else {
    dls_offsetidx = (DLTS_OFFSET_t) server->arg("dlsmanidx").toInt();
  }
  timec.SetGMT_Offset(gmt_offset);
  timec.SetDLS_Offset( (DLTS_OFFSET_t)(dls_offsetidx) );
  timec.SetAutomaticDLS(!DLSOverrid);
  timec.SetManualDLSEna(ManualDLS);
  timec.SetTimeZoneManual(ZONE_OVERRRIDE);

  write_timecoreconf(timec.GetConfig());
  
  server->send(200);    
  displayrefesh();
  
 }

