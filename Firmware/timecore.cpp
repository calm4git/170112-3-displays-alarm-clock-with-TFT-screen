#include "timecore.h"
#include "timezones.h"

 Timecore::Timecore(){};
 
 Timecore::~Timecore(){
 
 }
    

uint32_t Timecore::GetUTC( void ){
    return local_softrtc_timestamp ;
}

datum_t Timecore::ConvertToDatum( uint32_t timestamp ){
 datum_t d;  
 tmElements_t time_element;
 breakTime(timestamp,time_element);

 d.year   = time_element.Year+1970;
 d.month  = time_element.Month;
 d.day    = time_element.Day;
 d.dow    = time_element.Wday;
 d.hour   = time_element.Hour;
 d.minute = time_element.Minute;
 d.second = time_element.Second;
  
 return d;
}

void Timecore::SetConfig(timecoreconf_t conf){
  Serial.println("Copy Conf to MEM");
  memcpy(&local_config,&conf,sizeof(timecoreconf_t));
}

timecoreconf_t Timecore::GetConfig( void ){
  return local_config;
}

timecoreconf_t Timecore::GetDefaultConfig( void) {
  timecoreconf_t default_cfg;
   default_cfg.TimeZone = Europe_London;
   default_cfg.DLTS_OffsetIDX = DLST_OFFSET_0;
   default_cfg.ManualDLSEna = false;
   default_cfg.AutomaticDLTS_Ena = true;
   default_cfg.TimeZoneOverride = false;
   default_cfg.GMTOffset = 0;
   return default_cfg; 
 
}

/* we do a degrading every 30 minutes */
void Timecore::SetUTC( uint32_t time, source_t source ){
    
    if(  source >= CurrentMasterSource ){
      CurrentMasterSource = source;
      /* The priority is higher or equal we sync now */
      noInterrupts();
      local_softrtc_timestamp = time;
      interrupts();
      for(uint32_t i=0;i<  RTC_SRC_CNT  ;i++){
        if( (TimeSources[i].type!=NO_RTC) && (TimeSources[i].type<source) ){
          TimeSources[i].WriteTime(time);
        }
      }   
    } else {
      Serial.printf("TS: %i from %i, lower prio as %i",time,source,CurrentMasterSource);
    }
}
void Timecore::SetUTC( datum_t time, source_t source  ){
    /*This is tricky as we now need to buld a utc timestamp */
    uint32_t timestamp =0;
    timestamp = TimeStructToTimeStamp( time );
    if(  source >= CurrentMasterSource ){
      CurrentMasterSource = source;
      /* The priority is higher or equal we sync now */
     
      noInterrupts();
      local_softrtc_timestamp = timestamp;
      interrupts();
      
    } else {
      Serial.printf("TS: %i from %i, lower prio as %i",timestamp,source,CurrentMasterSource);
    }
    
}

void Timecore::RegisterTimeSource(rtc_source_t source)  {

  for(uint32_t i=0;i<RTC_SRC_CNT;i++){
    if(TimeSources[i].type==NO_RTC){
       TimeSources[i]=source;
    }
  }
    
} 
void Timecore::RTC_Tick( void ){ /* Needs to be called once a second */
    local_softrtc_timestamp++;
    if(DegradeTimer_Src>0){
     DegradeTimer_Src--;
     
    } else {
      if(CurrentMasterSource>NO_RTC){
        CurrentMasterSource= (source_t)((int)CurrentMasterSource-1);
      }
      DegradeTimer_Src=900;
    }
}    

/* This are the rtc events fired */
void Timecore::RegisterCB( rtc_cb_t Event, void* CB ){
  
    rtc_event_callback[Event] = CB;

}

void Timecore::SetDLS_Offset(DLTS_OFFSET_t offset ){
    
if(offset<DLST_OFFSET_CNT){
    local_config.DLTS_OffsetIDX = offset;
  }
  
}

DLTS_OFFSET_t Timecore::GetDLS_Offset( void  ){
  return local_config.DLTS_OffsetIDX;
}

void Timecore::SetGMT_Offset(int32_t offset ){
    local_config.GMTOffset = offset;
  
}

int32_t Timecore::GetGMT_Offset( void ){
  return local_config.GMTOffset;
}
void Timecore::SetAutomaticDLS( bool ena){
    local_config.AutomaticDLTS_Ena=ena;
}

bool Timecore::GetAutomacitDLS( void ){
    return local_config.AutomaticDLTS_Ena;
}

bool Timecore::GetZimeZoneManual( void ){
  return local_config.TimeZoneOverride;
}

bool Timecore::GetManualDLSEna( void ){
  return local_config.ManualDLSEna;
}
 
void Timecore::SetManualDLSEna( bool ena){
  local_config.ManualDLSEna=ena;
}

bool Timecore::GetDLSstatus( void ){
   bool result=false;
  uint32_t now = GetUTC();
   if(local_config.AutomaticDLTS_Ena==true){
     bool northTZ = (dstEnd>dstStart)?1:0; // Northern or Southern hemisphere TZ?
     if( (northTZ && (now >= dstStart && now < dstEnd) )|| ( !northTZ && (now < dstEnd || now >= dstStart) ) ) {
      result = true;
     }
   } else {
      result = local_config.ManualDLSEna;
    
   }
   return result;
}

void Timecore::SetTimeZoneManual( bool ena){
  local_config.TimeZoneOverride=ena;
}

void Timecore::SetTimeZone(TIMEZONES_NAMES_t Zone ) {
/* we need to load the basic parameter to the core */
  local_config.TimeZone = Zone;
  dstYear = 0;
}



void Timecore::SetLocalTime( datum_t d){

  if(d.year>=2000){
    d.year-=2000;
  }
  uint32_t localtimestamp = TimeStructToTimeStamp( d );
  bool northTZ = (dstEnd>dstStart)?1:0; // Northern or Southern hemisphere TZ?
  /* we need to fix the offset */
 Serial.printf("TS:%i \n\r",localtimestamp);
 if(local_config.TimeZoneOverride==true){
   localtimestamp = localtimestamp-(local_config.GMTOffset*60);
 } else {
  localtimestamp = localtimestamp - ZoneTable[local_config.TimeZone].Offset;
  /* next is to check if we may have dlst */
 }
 
  if(dstYear!=d.year+30)
     {
      Serial.printf("Year: %i", d.year);
      dstYear=d.year+30;
      dstStart = calcTime(&ZoneTable[local_config.TimeZone].StartRule);
      dstEnd = calcTime(&ZoneTable[local_config.TimeZone].EndRule);
   
      Serial.println("\nDST Rules Updated:");
      Serial.print("DST Start: ");
      Serial.print(ctime(&dstStart));
      Serial.print("DST End:   ");
      Serial.println(ctime(&dstEnd));
  }

  if(local_config.AutomaticDLTS_Ena==true){
    if ((northTZ && (localtimestamp >= dstStart && localtimestamp < dstEnd)) || (!northTZ && (localtimestamp < dstEnd || localtimestamp >= dstStart))){
      localtimestamp -= ZoneTable[local_config.TimeZone].StartRule.offset;
      Serial.printf(" Removed DLS Offset  ");
    } else {
      
    }
  } else {
    if(local_config.ManualDLSEna==true){
      if(local_config.ManualDLSEna==true){
      switch(local_config.DLTS_OffsetIDX){
        case DLST_OFFSET_MINUS_60:{ 
          localtimestamp+=(60*60);          
        } break;
        
        case DLST_OFFSET_MINUS_30:{ 
          localtimestamp+=(30*60);          
        } break;

        case DLST_OFFSET_0:{ 
                    
        } break;
        
        case DLST_OFFSET_PLUS_30:{ 
          localtimestamp-=(30*60);          
        } break;
        
       case DLST_OFFSET_PLUS_60:{ 
          localtimestamp-=(60*60);         
        } break;

        default:{
          
        } break;
      }
      
    }
    }
  }
  Serial.printf("TS_UTC:%i \n\r",localtimestamp);
  SetUTC(localtimestamp, USER_DEFINED);
 
}

datum_t Timecore::GetLocalTimeDate( void ){
  uint32_t t = GetLocalTime();
  return ConvertToDatum(t);
}

time_t Timecore::GetLocalTime( void )
{
 bool northTZ = (dstEnd>dstStart)?1:0; // Northern or Southern hemisphere TZ?
 time_t now = GetUTC();  // Call the original time() function
// Serial.printf("UTC Time: %i ->",now);
  if(local_config.TimeZoneOverride==false){
    if(now>ZoneTable[local_config.TimeZone].Offset){
      now = now + ZoneTable[local_config.TimeZone].Offset;
   }  
  } else {
    Serial.println("MANOFFSET");
    now+=local_config.GMTOffset*60;
  }

 
  if(local_config.AutomaticDLTS_Ena==false){
    Serial.println("NOAUTODLS");
    if(local_config.ManualDLSEna==true){
       Serial.println("MANDLSENA");
      switch(local_config.DLTS_OffsetIDX){
        case DLST_OFFSET_MINUS_60:{ 
          now-=(60*60);          
        } break;
        
        case DLST_OFFSET_MINUS_30:{ 
          now-=(30*60);          
        } break;

        case DLST_OFFSET_0:{ 
          now=now;          
        } break;
        
        case DLST_OFFSET_PLUS_30:{ 
          now=now+(30*60);          
        } break;
        
       case DLST_OFFSET_PLUS_60:{ 
          now=now+(60*60);          
        } break;

        default:{
          now=now;
        } break;
      }
      
    }
  } else {
 
    
   
   if(ZoneTable[local_config.TimeZone].has_dls == false ){
    /* we are done here */  
   } else {
     
   uint8_t year = calcYear(now);
  
  
   // Init DST variables if necessary
    if(dstYear!=year)
     {
      dstYear=year;
      dstStart = calcTime(&ZoneTable[local_config.TimeZone].StartRule);
      dstEnd = calcTime(&ZoneTable[local_config.TimeZone].EndRule);
   
      Serial.println("\nDST Rules Updated:");
      Serial.print("DST Start: ");
      Serial.print(ctime(&dstStart));
      Serial.print("DST End:   ");
      Serial.println(ctime(&dstEnd));
  }
   
    if( ( northTZ && ( now >= dstStart && now < dstEnd) ) || ( !northTZ && (now < dstEnd || now >= dstStart)))
     {
      //Serial.printf("DLS active with %i offset",ZoneTable[CurrentZone].StartRule.offset);
      //Serial.println(ctime(&now));
      now += ZoneTable[local_config.TimeZone].StartRule.offset;
      /*    if(abbrev!=NULL)
             *abbrev = dstStartRule.abbrev; */
             
           } 
    else
     {
        /*    if(abbrev!=NULL)
           *abbrev = dstEndRule.abbrev; */
     }
  
   }
  }
 // Serial.printf("Corrected Time is %i \n\r",now);
  return(now);
}

// Private
uint8_t Timecore::calcYear(time_t time)
{
 uint8_t year=0;
 unsigned long days=0;
 
  time /= SECS_PER_DAY; // now it is days

  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  
  return(year);
}

/*----------------------------------------------------------------------*
 * Convert the given DST change rule to a time_t value                  *
 * for the given year.                                                  *
 * Based on code from Arduino Timezone Library by Jack Christensen      *
 *----------------------------------------------------------------------*/
time_t Timecore::calcTime(struct dstRule * tr)
{
 struct tm tm2;
 time_t t;
 uint8_t m, w;            //temp copies
 
    m = tr->month;
    w = tr->week;
    if (w == 0) {            //Last week = 0
        if (++m > 11) {      //for "Last", go to the next month
            m = 0;
            // yr++;
        }
        w = 1;               //and treat as first week of next month, subtract 7 days later
    }

    tm2.tm_hour = tr->hour;
    tm2.tm_min = tr->minute;
    tm2.tm_sec = 0;
    tm2.tm_mday = 1;
    tm2.tm_mon = m;
    tm2.tm_year = dstYear;

    // t = ::mktime(&tm2);        // mktime() seems to be broken, below is replacement
    t = my_mktime(&tm2);        //first day of the month, or first day of next month for "Last" rules

    t += (7 * (w - 1) + (tr->dow - weekday(t) + 7) % 7) * SECS_PER_DAY;
    if (tr->week == 0) t -= 7 * SECS_PER_DAY;    //back up a week if this is a "Last" rule
    
    return t;
}

/*----------------------------------------------------------------------*
 * Replacement for broken mktime()                                      *
 * Based on code from Paul Stoffregen's Arduino Time Library for        *
 *----------------------------------------------------------------------*/
time_t Timecore::my_mktime(struct tm *tmptr)
{   
  int i;
  time_t seconds;
  static int8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

  // seconds from 1970 till the given year
  seconds= tmptr->tm_year*(SECS_PER_DAY * 365);
  for (i = 0; i < tmptr->tm_year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year
  for (i = 0; i < tmptr->tm_mon; i++) {
    if ( (i == 1) && LEAP_YEAR(tmptr->tm_year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i];
    }
  }
  seconds+= (tmptr->tm_mday-1) * SECS_PER_DAY;
  seconds+= tmptr->tm_hour * SECS_PER_HOUR;
  seconds+= tmptr->tm_min * SECS_PER_MIN;
  seconds+= tmptr->tm_sec;

  return (time_t)seconds; 
}

uint32_t Timecore::TimeStructToTimeStamp(datum_t d ){
// assemble time elements into time_t 
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
static int8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};
uint8_t year =  d.year;
uint8_t month = d.month;
uint8_t day = d.day;
uint8_t hour = d.hour;
uint8_t minute = d.minute;
uint8_t second = d.second;
uint32_t UnixTamestamp;

if(month>12){
  month=1;
}

if(day>31){
  day=1;
}

if(hour>23){
  hour=0;
}

if(minute>59){
  minute=0;
}

if(second>59){
  second=0;
}

  int i;
  uint32_t seconds;
  year=year+30;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= year*(SECS_PER_DAY * 365);
  for (i = 0; i < year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < month; i++) {
    if ( (i == 2) && LEAP_YEAR(year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (day-1) * SECS_PER_DAY;
  seconds+= hour * SECS_PER_HOUR;
  seconds+= minute * SECS_PER_MIN;
  seconds+= second;
  UnixTamestamp = seconds;

return  UnixTamestamp; 
}

TIMEZONES_NAMES_t Timecore::GetTimeZone( ){
  return local_config.TimeZone;
}

const char* Timecore::GetTimeZoneName(TIMEZONES_NAMES_t Zone){
  return TimeZoneNames[Zone];
}

