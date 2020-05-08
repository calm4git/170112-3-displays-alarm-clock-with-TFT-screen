#ifndef TIMECORE_H_
 #define TIMECORE_H_
 
 /* 
    This is a generic Time core that can be feeded by 
    differnt timesources in polled or event driven modes
    The time is internally handeld as UTC time and converted
    to the appropriate locale time
 */
 
 
#include "Arduino.h"
#include <TimeLib.h>
#include "timezone_enums.h"


typedef struct{
  TIMEZONES_NAMES_t TimeZone;
  DLTS_OFFSET_t DLTS_OffsetIDX;
  bool ManualDLSEna;
  bool AutomaticDLTS_Ena;
  bool TimeZoneOverride;
  int32_t GMTOffset; 
}timecoreconf_t;

/* the higher the id the higher the priority */
typedef enum {
    NO_RTC = 0,
    RTC_CLOCK,
    NTP_CLOCK,
    GPS_CLOCK,
    USER_DEFINED,
    RTC_SRC_CNT
} source_t  ;  

/* The RTX Soruce must provice a unix timestamp in GMT*/
typedef struct {
   source_t type;
   void (*SecondTick)(void);
   void (*WriteTime)(uint32_t);
   uint32_t (*ReadTime)(bool* delayed_result);   
} rtc_source_t;


typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t dow;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} datum_t;

typedef enum {
    OnSecondChanged=0,
    OnMinuteChanged,
    OnHourChanged,
    OnDayOfWeekChaned,
    OnDayChanged,
    OnMonthChanged,
    OnYearChanged,
    RTC_EVENT_CNT
} rtc_cb_t;

typedef enum {
 no_zone=0
} TimeZone_t;

class Timecore {
    
    public:
    
    Timecore();
    ~Timecore();

   
   
    void SetUTC( uint32_t time, source_t source );
    void SetUTC( datum_t time, source_t source  );

    uint32_t GetUTC( void );
    time_t GetLocalTime( void );
    datum_t GetLocalTimeDate( void );
    void SetLocalTime( datum_t d);
    
    datum_t ConvertToDatum( uint32_t timestamp);
    
    
    void SetTimeZone(TIMEZONES_NAMES_t Zone );
    TIMEZONES_NAMES_t GetTimeZone( void );
    
    void SetDLS_Offset(DLTS_OFFSET_t offset );
    DLTS_OFFSET_t GetDLS_Offset( void );
    
    void SetGMT_Offset(int32_t offset );
    int32_t GetGMT_Offset( void );
    
    void SetAutomaticDLS( bool ena);
    bool GetAutomacitDLS( void );

    bool GetDLSstatus( void );

    bool GetManualDLSEna( void );
    void SetManualDLSEna( bool ena);
    void SetTimeZoneManual( bool ena);
    bool GetZimeZoneManual( void );

    void SetConfig(timecoreconf_t conf);
    timecoreconf_t GetConfig( void );
    
    static timecoreconf_t GetDefaultConfig( void );
    /* This are the rtc events fired */
    void RegisterCB( rtc_cb_t Event, void* CB );
    void RegisterTimeSource(rtc_source_t source);  

    void RTC_Tick( void ); /* Needs to be called once a second */
    void RTC_Task( void ); /* In this context will the callbacks been executed */

    const char* GetTimeZoneName(TIMEZONES_NAMES_t Zone); 
    private:
        timecoreconf_t local_config={.TimeZone=Africa_Abidjan , .DLTS_OffsetIDX=DLST_OFFSET_0, .ManualDLSEna=false,.AutomaticDLTS_Ena=true, .TimeZoneOverride=false ,.GMTOffset=0}; 

       
        uint8_t dstYear;
        time_t dstStart;  // Start of DST in specific Year (seconds since 1970)
        time_t dstEnd;    // End of DST in listed Year (seconds since 1970)
        
        
        uint32_t local_softrtc_timestamp=0;
        source_t CurrentMasterSource=NO_RTC; /* If this is set to none we run from the internal rtc */
        
        void SetTimeZone(struct dstRule startRule, struct dstRule endRule );
        uint32_t TimeStructToTimeStamp(datum_t time);
       
               
        rtc_source_t TimeSources [RTC_SRC_CNT]; 
        void* rtc_event_callback[RTC_EVENT_CNT]; /* Holds the callbacks for the RTC events */
        
        
        uint8_t calcYear(time_t time);
        time_t calcTime(struct dstRule * tr);
        time_t my_mktime(struct tm *tmptr);
        
        uint16_t DegradeTimer_Src=3600;
        
        
        
};
 
 
 
 
 
#endif
