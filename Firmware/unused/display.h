#ifndef _DISPLAY_H_
#define _DISPLAY_H_

// Copyright (C) 2016 Olivier CROISET. All rights reserved
// Copyright (C) 2017,2018 Elektor Labs (edited). All rights reserverd
/***************************************************************/
/*                     Multidisplay alarm-clock                */
/*                     Olivier CROISET  2016(C)                */
/*                  edited by Elektor Labs 2017(C)             */
/***************************************************************/

// https://github.com/olikraus/lcdlib/wiki/reference
// http://arduiniana.org/libraries/streaming/ for strings concatenation
// ILI 9341     :  25mA
// Screen LEDs  :  5 mA
// ESP8266      : TBDmA

#include <math.h>
#include <Streaming.h>
#include "Ucglib.h"
#include "timecore.h"

unsigned long lastUploadTime = 0;
unsigned long lastMeasureTime = 0;
unsigned long lastAPConnection = 0;
unsigned long lastAlarmSound = 0;

/* IF we need activ printing */
//#define DEBUGPRINT( X ) Serial.print( X ) 
#define DEBUGPRINT( X )  do{ }while( 1 == 0) 
    
#define MAX_BEEP_TIME_MIN  ( 15 )

// Init the TFT screen               dc  cs  reset
//ESP8266
//Ucglib_ILI9341_18x240x320_HWSPI lcd(/*cd=*/ D3, /*cs=*/ D4 /*reset=*/ /*D0*/);

//ESP32
Ucglib_ILI9341_18x240x320_HWSPI lcd(/*cd=*/ 10, /*cs=*/ 5 /*reset=*/ /*D0*/);
typedef enum {
  released=0,
  pressed,
  repeated_pressed,

} buttonstate_t;

typedef enum {
  KEY_A=0,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_COUNT
} button_t;

buttonstate_t ButtonStateArray[KEY_COUNT][2];

//======================================= Variables for the DS1302
int16_t myYear;
int8_t myMonth, myDate, myHour, mySec , myMin, myDow;

int8_t currentHour;              // for the display changes
int8_t previousDate = 0, previousHour, previousMin, previousSec;

#define Ypos ( 30 )
#define Xpos ( 0 )

typedef struct {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
} Color_t;

typedef struct{
  uint16_t LDR_BRIGHT;
  uint16_t INV_LDR_BRIGHT;
  uint16_t LDR_DARK;
  uint16_t INV_LDR_DARK;
}LDR_CAL_t;

typedef struct {
uint8_t hour;
uint8_t min;
uint8_t sec;
uint8_t dow;
uint16_t year;
uint8_t mon;
uint8_t date;
} timeval_t;

/* Why the hell float ? */
float angleSec = 0;           // second hand angle
float angleMin = 0;           // minute hand angle
float angleHour = 0;          // hour hand angle
float angleMinForHour = 0;
float angleSecPrec;           // second hand angle in triangle shape for SecondeEphemere

// Variables for the CadranAiguilles
// tetragones points of the minutes hand, and erease
int16_t Mhdx, Mhdy, Mhgx, Mhgy, Mbgx, Mbgy, Mbdx, Mbdy;
int16_t Mhdxprec, Mhdyprec, Mhgxprec, Mhgyprec, Mbgxprec, Mbgyprec, Mbdxprec, Mbdyprec;

// tetragones points of the hour hand, and erease
int16_t Hhdx, Hhdy, Hhgx, Hhgy, Hbgx, Hbgy, Hbdx, Hbdy;
int16_t Hhdxprec, Hhdyprec, Hhgxprec, Hhgyprec, Hbgxprec, Hbgyprec, Hbdxprec, Hbdyprec;

float angletick, x1Trait, y1Trait, x2Trait, y2Trait ;

//======================================= Interrupteur de validation de l'alarme

typedef struct{
  uint8_t BeepOn:1;
  uint8_t SnoozeOn:1;
  uint8_t AlertArmed:1;
  uint8_t Reserved:5;
  uint8_t SnoozeHour;
  uint8_t SnoozeMinute;
} AlertBits_t;

volatile uint16_t BeepRunCnt;
volatile AlertBits_t AlertBits;

// Alarm time variables
typedef struct {
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Day;
} Alarm_t;

/* We need 9 to set and one to work with */
#define USERALARMCNT ( 10 )
volatile Alarm_t UserAlarm[USERALARMCNT];


int8_t AlarmIndex = 0;
int8_t Alarmx = 0;
int16_t BlackNext = 0;
int16_t AlarmNext = 0;
boolean NextAlarm;
int16_t AlarmDistance;

boolean SwitchAlarm;
boolean ToggeleBeper = true;
boolean refreshScreen = true;


timeval_t t_temp;

volatile LDR_CAL_t LDR_Calibration; /* LDR Calibration Values for the Devices */

//char Month[nombre de chaines][length of strings, add 1 for the final \0 ]

// This is bad as it consumes RAM and Flash at the same time !




// English
 char Month[][10]    = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
 char Day[][13]    =    {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday", "Mon-Fri", "Never"};
 char OFF_ON[][5]    = {"Auto","Max","Med","Low"};


// variables for the LED dimming
//int16_t light;   // little variable, but great comfort for people who need complete darkness !
volatile bool SecondTick;
volatile bool MinuteTick;
volatile int8_t last_screen=-1;
volatile uint8_t auto_dimmer=0;

void  Backlight( void );
void AiguilleSecondes( void ) ;
void CadranBlanc( );
void CadranRond();
void AfficherDate();
void Cadran2( bool );
void SecondeSecteur(Color_t Color);
void Secondes( void );
void Colon( void );
void CadranSecondes ( void );
void SevenSegments(bool);    
bool GetNextAlert(bool refresh);
void AffAlarm( bool ) ;
void PointsHelper( uint8_t Val, int16_t offset, Color_t Color);
void PointsColumn( void );
void Points0(uint16_t Numdigit, Color_t Color);
void Points1(uint16_t Numdigit, Color_t Color);
void Points2(uint16_t Numdigit, Color_t Color);
void Points3(uint16_t Numdigit, Color_t Color);
void Points4(uint16_t Numdigit, Color_t Color);
void Points5(uint16_t Numdigit, Color_t Color);
void Points6(uint16_t Numdigit, Color_t Color);
void Points7(uint16_t Numdigit, Color_t Color);
void Points8(uint16_t Numdigit, Color_t Color);
void Points9(uint16_t Numdigit, Color_t Color);

int16_t SettingHelper1(int16_t Aspect) ;
int8_t SettingHelper2(int8_t Aspect, int8_t timeAspect1, int8_t timeAspect2) ;

void Segment( uint8_t idx, uint16_t offset);

void InitDisplay(){
  // Init of the screen ILI9341
  lcd.begin(UCG_FONT_MODE_SOLID);
  lcd.clearScreen();
  lcd.setRotate90();                             // Rotate in landscape format, connector on the right side
  lcd.setFont  (ucg_font_9x15_tr);               // Font select
  /* Show connect to WiFi Screen */
  lcd.setPrintPos( 0, 10);
  lcd.setColor(155, 155, 155);  // dimmed if alarm OFF
  lcd.setPrintPos( 0, 24);
  lcd.clearScreen();

  
}

uint8_t Touches ( ){
  return 0;
}

void displayrefesh(){
  refreshScreen=true;
}

extern Timecore timec;

void RTC_ReadTime(){

 /* Adapt this to yout RTC if needed */
 tmElements_t time_element;
 breakTime(timec.GetLocalTime(),time_element);
 
  myYear  = time_element.Year+1970;
  myMonth = time_element.Month;
  myDate  = time_element.Day;
  if(time_element.Wday>0){
    time_element.Wday--;
  } else {
    time_element.Wday=6;
  }
  myDow   = time_element.Wday;
  myHour  = time_element.Hour;
  myMin   = time_element.Minute;
  mySec   = time_element.Second;

}


void ButtonScanTask( ){
  /* This will read the Button and prodice differnt states and events */

  for(uint8_t i=0;i<KEY_COUNT;i++){
    
  }
 
}

buttonstate_t GetButtonState ( button_t Button ) {
  
  buttonstate_t Result = released;
  switch(Button){
    case KEY_A:{
      
    } break;

    case KEY_B:{
      
    } break;

    case KEY_C:{
      
    } break;

    case KEY_D:{
      
    } break;

    default:{
      
    }break;
    
  }
  
  
  return Result;
}


void SetupDisplay(){
  UserAlarm[0].Hour=0;
  UserAlarm[0].Minute=0;
  UserAlarm[0].Day=8;

  RTC_ReadTime(); /* This syncs the time to the display */
  
  refreshScreen = true;
  AlertBits.BeepOn=LOW;
  /* Load next alert */
  GetNextAlert(true);
  

}

/*==================================================*/
/*                 MAIN LOOP                        */
/*==================================================*/
void DisplayTask()
{
  
  if(previousSec != mySec){
    SecondTick=true;
    Backlight();
    //Serial.println(".");
      
   } 

     
  if(previousMin !=  myMin){
    MinuteTick=true;
    
  }
  previousSec  = mySec;
  previousMin  = myMin;
  previousHour = myHour;
  previousDate = myDate;

 RTC_ReadTime();

  // ========================= TURNING DISPLAY =========================
  if (myHour != previousHour) {
  
    lcd.clearScreen();  // Erase the screen before each clock display change
  }
  // ======================= CadranAiguilles : selected at 0, 3, 6, 9, 12, 15, 18, 21, 24 hour
  if ((myHour % 3) == 0) {
    if(last_screen != 0){
      refreshScreen=true;
      last_screen=0;
    }
    if (mySec  != previousSec || refreshScreen == true)  {
      AiguilleSecondes();
    }
    if (myMin  != previousMin || refreshScreen == true)  {
      CadranRond();
    }
    if (myHour != previousHour || refreshScreen == true) {
      CadranBlanc();
      CadranRond();
    }
    AfficherDate();                              // AfficherDate in the CadranAiguilles tab
  }

  // ======================= Cadran2 : selected at 1, 4, 7, 10, 13, 16, 19, 22 hour
  if ((myHour % 3) == 1) {
    Color_t Color;
    if(last_screen != 1){
      refreshScreen=true;
      last_screen=1;
    }
    
    if (myMin != previousMin || refreshScreen == true) {
      Cadran2(refreshScreen);
    }
    
    if (myMin != (myMin / 2) * 2) {
      Color.Red = 127;
      Color.Green = 127;
      Color.Blue = 255;

    }
    else {
      Color.Red = 255;
      Color.Green = 255;
      Color.Blue = 51;
    }
    if(  SecondTick==true ){
      SecondeSecteur(Color);                             // Display of the 5 seconds   
    }

   
    /*--------------------------------------------------*/


    /*--------------------------------------------------*/
    lcd.setColor(255 , 255, 255);
    lcd.setPrintPos( 160, 165);
    lcd << (Day[myDow - 1]) << " " << (myDate) ; // endl;
    lcd.setPrintPos( 160, 185);
    lcd << (Month[myMonth - 1]) << " " << (myYear); // endl;
  }

  // ======================= SevenSegments : selected at 2, 5, 8, 11, 14, 17, 20, 23 hour
  // affiche le cadre des secondes
  if ((myHour % 3) == 2)
  {
    if(last_screen != 2){
      refreshScreen=true;
      last_screen=2;
    }

    if(refreshScreen == true){
        lcd.setColor(255 , 255, 255);
        lcd.setPrintPos(  30, 180);
        lcd << (Day[myDow - 1]) << " " << (myDate) << " " << (Month[myMonth - 1]) << " " << (myYear); //endl;
    }

    if(true==SecondTick){
      Colon();
      Secondes();
      SecondTick=false;
    }
    if (previousMin != myMin || refreshScreen == true) {
      CadranSecondes();  // affiche les points des secondes
      SevenSegments(refreshScreen);    
      
    } 
    
  }
  
 
  if(MinuteTick == true){
    MinuteTick=false;
  }

    if(true==GetNextAlert( (MinuteTick | refreshScreen ) ) ){
    AffAlarm(true);
  } else {
    AffAlarm(refreshScreen);
  }
  if(MinuteTick == true){
    MinuteTick=false;
  }
  
  refreshScreen = false;



  //=========================== DIMMING OF THE TFT SCREEN LEDs ============================
  SecondTick=false;
  
  // ================= END OF MAIN LOOP ======================
}


void Backlight(){

  /* need a backlight control */
}





void Buzzer( ) {
   /* Need a buzzer to be implemented in the watch */
}

bool GetNextAlert(bool refresh){
/* 
 *  This will copy the next Alert into UserAlarm[0];
 */
static uint8_t Last_Alert=0;
uint8_t NextAlert  = 0;
uint16_t Distance[10] = {0xFFFF,} ;
/* We use 0xFFFF to mark the alert as off */
/* Helper vars */
int16_t DistDays=0;
int16_t DistHours=0;
int16_t DistMinutes=0;
uint8_t Dow_ZeroStart = myDow-1;
bool alert=false;


if( ( AlertBits.SnoozeOn != LOW ) || ( AlertBits.BeepOn != LOW ) ){
  return false;
}

if(refresh==false){
  return false;
} else {
   
}
 
     for(uint8_t i=1;i<USERALARMCNT;i++){
       
       //Compiler warning, as this is always true due to limited range of datattype
        if( (UserAlarm[i].Day>=0) && ( UserAlarm[i].Day<8) ){
            //Serial.println("Active");
            if( (UserAlarm[i].Day==7) && (Dow_ZeroStart < 5) ){ /* Mo-Fr */
              DistDays=0;             
            } else {
              if(UserAlarm[i].Day==7){
                /* We set the alert to monday */
                DistDays= 7-Dow_ZeroStart;
              } else {
                if(UserAlarm[i].Day>=Dow_ZeroStart){
                         
                    DistDays=UserAlarm[i].Day-Dow_ZeroStart;
                } else {
                    DistDays= 7-Dow_ZeroStart + UserAlarm[i].Day ;
                }  
              }
              
              
           } /* Days distance done */

           if( myHour <= UserAlarm[i].Hour ){
            /* Sameay alert */
              DistHours = UserAlarm[i].Hour - myHour;
           } else {
             DistHours = 24-myHour+UserAlarm[i].Hour;
             if(DistDays>0){
                  DistDays--;
              } else {
                  /* if we have a sameday alert we need to check if this will be active tomorrow */
                  if( (UserAlarm[i].Day==7) && (Dow_ZeroStart < 4) ){ /* Mo-Do */
                           /* We can ring tomorrow */
                           DistDays=0;
                   }  else if ( (UserAlarm[i].Day==7) && (Dow_ZeroStart == 4) ){
                      /* We Set the day to monday for distance calculation and must be aware to get one more substarcted */
                      DistDays= 7-Dow_ZeroStart; /* Set Day to monday */ 
                      if(DistDays > 0){
                        DistDays--;
                      }
                   }else {
                       DistDays=6;
                   }
              }
           }
           

           if( myMin > UserAlarm[i].Minute){
             /* Not same hour */
               DistMinutes = 60 - myMin + UserAlarm[i].Minute;
               
               if(DistHours>0){
                DistHours--;
               } else {
             
                DistHours=23;
                  /* Need to adjust the day */
                  if(DistDays>0){
                    DistDays--;
                  } else {

                   if( (UserAlarm[i].Day==7) && (Dow_ZeroStart < 4) ){ /* Mo-Do */
                           /* We can ring tomorrow */
                           DistDays=0;
                   }  else if ( (UserAlarm[i].Day==7) && (Dow_ZeroStart == 4) ){
                      
                      if(DistDays>0){
                        DistDays--;
                      } else {
                        /* tjis gets tricky here as we may have to some stuff again */   
                        DistDays= 7-Dow_ZeroStart; /* Set Day to monday */ 
                        //Serial.println(F("Adjust days @ Minutes"));
                      }
                   
                   } else { 
                          DistDays=6;
                  }
                
               }
            } 

               
           } else {
            /* same hour */
               DistMinutes =  UserAlarm[i].Minute - myMin;        
               
           }
          
           Distance[i] = ( DistDays * 1440 ) + ( DistHours * 60 ) + ( DistMinutes );  
        } else {
          Distance[i]=0xFFFF;
        }

     }
      NextAlert=0;
      for(uint8_t i=1;i<USERALARMCNT;i++){
        if( Distance[i] < Distance[0] ){
            NextAlert=i;
            Distance[0]= Distance[i];
        }
      }
      if(NextAlert>0){
        UserAlarm[0].Hour    = UserAlarm[NextAlert].Hour;
        UserAlarm[0].Minute  = UserAlarm[NextAlert].Minute;
        UserAlarm[0].Day     = UserAlarm[NextAlert].Day;
        AlarmDistance = Distance[0];
      } else {
        UserAlarm[0].Hour    = 99;
        UserAlarm[0].Minute  = 99;
        UserAlarm[0].Day     = 8;
       
      }
  if(Last_Alert != NextAlert){
    Last_Alert= NextAlert;
    return true;
  } else {
    return false;
  }
  
}



void AlarmSync() {
  
}



/* -------------------- end main code --------------------*/

void AffAlarm(bool refresh) /* Used to turn Alram on and off */
{
  static uint8_t Button_PrevValue=0;
  static uint8_t Prev_Snooze=LOW;

  if(Prev_Snooze != AlertBits.SnoozeOn){
    refresh=true;
  }
  Prev_Snooze = AlertBits.SnoozeOn;
  uint8_t ButtonVlaue = HIGH;;
  if( ( ( ButtonVlaue == LOW ) && ( Button_PrevValue == HIGH)  ) || (refresh == true) )
  {

    if( ( ButtonVlaue == LOW ) && ( Button_PrevValue == HIGH) ){
      if(SwitchAlarm== HIGH ){
        SwitchAlarm = LOW;
      } else {
        SwitchAlarm = HIGH;
      }
     // EEPROM.write( EEPALERTONOFF_ADDR, SwitchAlarm );
     
      
    }
    /* Process snooze here */
    lcd.setColor(0, 0, 0);
    lcd.drawBox(255, 0, (320-255), 25);
    lcd.drawBox(50, 0, 30, 10);
    
    lcd.setPrintPos( 0, 10);
    
    if (SwitchAlarm == HIGH) {
      lcd.setColor(155, 155, 155);  // dimmed if alarm OFF
        lcd.print(F("Alarm OFF"));
        AlertBits.BeepOn=LOW;
        AlertBits.SnoozeOn=LOW;
        AlertBits.Reserved=0;
        AlertBits.SnoozeHour=0;
        AlertBits.SnoozeMinute=0;
    }
    else
    {
      lcd.setColor(255, 255, 255);
      lcd.print(F("Alarm ON"));
    }

    if(AlertBits.SnoozeOn==HIGH){
      lcd.setPrintPos( 235, 10);
      lcd << "Snooze     " << endl;
      
      lcd.setPrintPos( 235, 25);
      lcd << ((AlertBits.SnoozeHour < 10) ? "0" : "") << AlertBits.SnoozeHour  << ":" << ((AlertBits.SnoozeMinute < 10) ? "0" : "") << AlertBits.SnoozeMinute << endl;
      
    } else {
      if(UserAlarm[0].Day >= 8){

        
      } else {
        lcd.setPrintPos( 235, 10);
        lcd << Day[UserAlarm[0].Day] << endl;
        
        lcd.setPrintPos( 235, 25);
        lcd << ((UserAlarm[0].Hour < 10) ? "0" : "") << UserAlarm[0].Hour << ":" << ((UserAlarm[0].Minute < 10) ? "0" : "") << UserAlarm[0].Minute << endl;
      }
    }
       
  }
  
  Button_PrevValue = ButtonVlaue;
}


void Cadran2( bool refresh ){
  // Numdigit = display horizontal position
  // Flash ROM = 2504 bytes
  // ============= display dozen hours
  static int8_t dozenH_Prev=-1;
  static int8_t unitH_Prev=-1;
  static int8_t dozenM_Prev=-1;
  static int8_t unitM_Prev=-1;
  static int8_t PointDrawn=-1;  

 if(false != refresh ){
  dozenH_Prev=-1;
  unitH_Prev=-1;
  dozenM_Prev=-1;
  unitM_Prev=-1;
  PointDrawn=-1;    
 }
  
  uint8_t dozenH = myHour / 10;
  uint16_t  Numdigit = 8;
  Color_t Color;
  Color.Red   = 255;
  Color.Green = 255;
  Color.Blue  = 255;

  if(dozenH_Prev!=dozenH){
    PointsHelper(dozenH,Numdigit,Color);
    dozenH_Prev=dozenH;
  }

  // ============= display units hours
  uint8_t unitH = myHour % 10;
  Numdigit = 80;
  Color.Red   = 253;
  Color.Green = 0;
  Color.Blue  = 0;

  if(unitH_Prev != unitH ){
     PointsHelper(unitH,Numdigit,Color);
      unitH_Prev = unitH;
  }
  // ============= deux points
  Numdigit = 153;
  PointsColumn();
  /*
    previousMin  = myMin;
    previousHour = myHour;
  */
  // ============= display dozen minutes
  uint8_t  dozenM = myMin / 10;
  Numdigit = 180;
  Color.Red   = 0;
  Color.Green = 255;
  Color.Blue  =  0;

  if(dozenM_Prev != dozenM){
    PointsHelper(dozenM,Numdigit,Color);
    dozenM_Prev = dozenM;
  }


  // ============= display units minutes
  uint8_t unitM = myMin % 10;
  Numdigit = 253;
  Color.Red   = 127;
  Color.Green = 127;
  Color.Blue  = 255;

  if(unitM_Prev != unitM ){
      PointsHelper(unitM,Numdigit,Color);
      unitM_Prev = unitM;
  }

}

void PointsHelper( uint8_t Val, int16_t offset, Color_t Color){

 switch (Val) {
      case 0: Points0(offset,Color); break;
      case 1: Points1(offset,Color); break;
      case 2: Points2(offset,Color); break;
      case 3: Points3(offset,Color); break;
      case 4: Points4(offset,Color); break;
      case 5: Points5(offset,Color); break;
      case 6: Points6(offset,Color); break;
      case 7: Points7(offset,Color); break;
      case 8: Points8(offset,Color); break;
      case 9: Points9(offset,Color); break; 
      }
}


void Points0(uint16_t Numdigit, Color_t Color)
// display of 0 ==================================
{
  lcd.setColor(0, 0, 0);             // Set to black
  lcd.drawBox (0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 14, 84);  // left du 0
  lcd.drawBox(Numdigit + 14, Ypos +  0, 42, 14);  // haut du 0
  lcd.drawBox(Numdigit + 42, Ypos +  0, 14, 84);  // right du 0
  lcd.drawBox(Numdigit + 14, Ypos + 70, 42, 14);  // bas du 0
}

void Points1(uint16_t Numdigit, Color_t Color)
// display of 1  ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox (0 + Numdigit, 0 + Ypos, 56 , 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit + 14, Ypos + 70, 42, 14);  // pied du 1
  lcd.drawBox(Numdigit + 28, Ypos +  0, 14, 70);  // tige du 1
  lcd.drawBox(Numdigit + 14, Ypos + 14, 14, 14);  // bec du 1
}

void Points2(uint16_t Numdigit, Color_t Color)
// display of 2 ==================================
{
  lcd.setColor(0, 0, 0);             // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 56, 14);  // up of 2
  lcd.drawBox(Numdigit + 42, Ypos + 14, 14, 14);  // right du 2
  lcd.drawBox(Numdigit +  0, Ypos + 28, 56, 14);  // middle of 2
  lcd.drawBox(Numdigit +  0, Ypos + 28, 14, 42);  // left du 2
  lcd.drawBox(Numdigit +  0, Ypos + 70, 56, 14);  // low of 2
}

void Points3(uint16_t Numdigit, Color_t Color)
// display of 3 ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 56, 14);  // haut du 3
  lcd.drawBox(Numdigit + 42, Ypos +  0, 14, 84);  // tige du 3
  lcd.drawBox(Numdigit + 14, Ypos + 28, 42, 14);  // milieu du 3
  lcd.drawBox(Numdigit +  0, Ypos + 70, 56, 14);  // bas du 3
}

void Points4(uint16_t Numdigit, Color_t Color)
// display of 4 ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 14, 42);  // left du 4
  lcd.drawBox(Numdigit +  0, Ypos + 42, 56, 14);  // milieu du 4
  lcd.drawBox(Numdigit + 28, Ypos + 28, 14, 56);  // pied du 4
}

void Points5(uint16_t Numdigit, Color_t Color)
// display of 5 ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 56, 14);  // haut du 5
  lcd.drawBox(Numdigit +  0, Ypos + 14, 14, 28);  // left du 5
  lcd.drawBox(Numdigit +  0, Ypos + 28, 56, 14);  // milieu du 5
  lcd.drawBox(Numdigit + 42, Ypos + 42, 14, 28);  // right du 5
  lcd.drawBox(Numdigit +  0, Ypos + 70, 56, 14);  // bas du 5
}

void Points6(uint16_t Numdigit, Color_t Color)
// display of 6 ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56 , 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 56, 14);  // haut du 6
  lcd.drawBox(Numdigit +  0, Ypos + 14, 14, 70);  // left du 6
  lcd.drawBox(Numdigit +  0, Ypos + 28, 56, 14);  // milieu du 6
  lcd.drawBox(Numdigit + 42, Ypos + 42, 14, 28);  // right du 6
  lcd.drawBox(Numdigit +  0, Ypos + 70, 56, 14);  // bas du 6
}

void Points7(uint16_t Numdigit, Color_t Color)
// display of 7 ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 56, 14);  // haut du 7
  lcd.drawBox(Numdigit + 42, Ypos + 14, 14, 14);  // right du 7
  lcd.drawBox(Numdigit + 28, Ypos + 28, 14, 28);  // milieu du 7
  lcd.drawBox(Numdigit + 14, Ypos + 56, 14, 28);  // bas du 7
}

void Points8(uint16_t Numdigit, Color_t Color)
// display of 8 ==================================
{
  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 14, 84);  // left du 8
  lcd.drawBox(Numdigit + 14, Ypos +  0, 42, 14);  // haut du 8
  lcd.drawBox(Numdigit + 14, Ypos + 28, 42, 14);  // milieu du 8
  lcd.drawBox(Numdigit + 14, Ypos + 70, 42, 14);  // bas du 8
  lcd.drawBox(Numdigit + 42, Ypos +  0, 14, 84);  // right du 8
}

void Points9(uint16_t Numdigit, Color_t Color)
// display of 9 ==================================
{
  lcd.setColor(0, 0, 0);                         // Set to black
  lcd.drawBox(0 + Numdigit, 0 + Ypos, 56, 84);

  lcd.setColor(Color.Red , Color.Green, Color.Blue);                // Set the color
  lcd.drawBox(Numdigit +  0, Ypos +  0, 14, 42);  // left du 9
  lcd.drawBox(Numdigit + 14, Ypos +  0, 42, 14);  // haut du 9
  lcd.drawBox(Numdigit + 14, Ypos + 28, 42, 14);  // milieu du 9
  lcd.drawBox(Numdigit +  0, Ypos + 70, 56, 14);  // bas du 9
  lcd.drawBox(Numdigit + 42, Ypos +  0, 14, 84);  // right du 9
}

void PointsColumn()
// display of : ==================================
{
 uint8_t Red = 255;
 uint8_t Green  = 255;
 uint8_t Blue  = 255;
  int DP = 150;                                   // horizontale position of ":"

  lcd.setColor(0, 0, 0);                          // Set to black
  lcd.drawBox(DP, 0 + Ypos, 25, 84);
  lcd.setColor(Red , Green, Blue);                // Set the color
  lcd.drawBox(DP + 0, Ypos + 28, 14, 14);         // point du haut de ":"
  lcd.drawBox(DP + 0, Ypos + 56, 14, 14);         // point du bas de ":"
}


int16_t ext_radius_cos_lut[12]={  0 , 25 , 43 , 50 , 43, 25,  0, -25, -43, -50, -43, -25 };
int16_t ext_radius_sin_lut[12]={ -50, -43, -25,   0, 25, 43, 50,  43,  25,  0,  -25, -43 };

int16_t int_radius_cos_lut[12]={   0,  20,  35, 40,  35, 20,  0, -20, -35, -40, -35, -20 };
int16_t int_radius_sin_lut[12]={ -40, -35, -20,  0,  20, 35, 40,  35,  20,  0,  -20, -35 };

int16_t prec_ext_radius_cos_lut[12]={ -22,   4, 28,   45, 50, 41, 22,  -4, -28, -45, -50, -41 };
int16_t prec_ext_radius_sin_lut[12]={ -45, -50, -41, -22,  4, 28, 45,  50,  41,  22,  -4, -28 };

int16_t prec_int_radius_cos_lut[12]={ -17,   3,  23, 36,  40, 33, 17,  -3, -23, -36, -40, -33 };
int16_t prec_int_radius_sin_lut[12]={ -36, -40, -33,-17,   3, 23, 36,  40,  33,  17,  -3, -23 };

void SecondeSecteur(Color_t Color)
{
  // display of the seconds in a circle made of sectors
  uint8_t idx=0;
  int ext_radius = 50;
  int int_radius = 40;
  int xcenter = 70;
  int ycenter = 190;
  int x_ext_Sec, y_ext_Sec, xSecPrec, ySecPrec, x_int_Sec, y_int_Sec, x_ext_SecPrec, y_ext_SecPrec, x_int_SecPrec, y_int_SecPrec;             // coordinates of the seconds sectors

  if (mySec % 5 == 0)
  {
    idx= (mySec / 5) +1 ;
    if(idx>11){
      idx=0;
    }
    //    angleSec = ((mySec * PI) / 30) - (PI / 2);
    //    angleSecPrec = ((mySec * PI) / 30) - (PI / 2) + (PI / 7); //   PI/7 rather than PI/6 will separate the 5 secondes sectors

    x_ext_Sec = ext_radius_cos_lut[idx]; x_ext_Sec = x_ext_Sec + xcenter ;
    y_ext_Sec = ext_radius_sin_lut[idx]; y_ext_Sec = y_ext_Sec + ycenter ;
    x_int_Sec = int_radius_cos_lut[idx]; x_int_Sec = x_int_Sec + xcenter ;
    y_int_Sec = int_radius_sin_lut[idx]; y_int_Sec = y_int_Sec + ycenter ;

    x_ext_SecPrec = prec_ext_radius_cos_lut[idx]; x_ext_SecPrec = x_ext_SecPrec + xcenter ;
    y_ext_SecPrec = prec_ext_radius_sin_lut[idx]; y_ext_SecPrec = y_ext_SecPrec + ycenter ;
    x_int_SecPrec = prec_int_radius_cos_lut[idx]; x_int_SecPrec = x_int_SecPrec + xcenter ;
    y_int_SecPrec = prec_int_radius_sin_lut[idx]; y_int_SecPrec = y_int_SecPrec + ycenter ;


    lcd.setColor(Color.Red, Color.Green, Color.Blue);
    lcd.drawTetragon(x_ext_Sec, y_ext_Sec, x_int_Sec, y_int_Sec, x_int_SecPrec, y_int_SecPrec, x_ext_SecPrec, y_ext_SecPrec);

  }
}

void CadranRond()
{
    // set to black, invisible the previous hour hand
  int16_t x1M = 160;  //120                                            // centre du cadran x et y
  int16_t y1M = 120;
  
  // save the previous values of minutes
  Mhdxprec = Mhdx;
  Mhdyprec = Mhdy;
  Mhgxprec = Mhgx;
  Mhgyprec = Mhgy;
  Mbgxprec = Mbgx;
  Mbgyprec = Mbgy;
  Mbdxprec = Mbdx;
  Mbdyprec = Mbdy;

 
// save the previous values for the hour hand
  Hhdxprec = Hhdx;
  Hhdyprec = Hhdy;
  Hhgxprec = Hhgx;
  Hhgyprec = Hhgy;
  Hbgxprec = Hbgx;
  Hbgyprec = Hbgy;
  Hbdxprec = Hbdx;
  Hbdyprec = Hbdy;

 
  angleMinForHour = ((myMin * PI) / 30);
  angleMin = ((myMin * PI) / 30) - (PI / 2);
  angleHour = ((myHour * PI) / 6) - (PI / 2) + (angleMinForHour / 12);

  Hhgx = -17 * cos(angleHour - 0.67); Hhgx = Hhgx + x1M;           // point hg ligne haute
  Hhgy = -17 * sin(angleHour - 0.67); Hhgy = Hhgy + y1M;
  Hhdx =  70 * cos(angleHour + 0.10); Hhdx = Hhdx + x1M;           // point hd ligne haute
  Hhdy =  70 * sin(angleHour + 0.10); Hhdy = Hhdy + y1M;

  Hbgx = -17 * cos(angleHour + 0.67); Hbgx = Hbgx + x1M;           // point bg ligne basse
  Hbgy = -17 * sin(angleHour + 0.67); Hbgy = Hbgy + y1M;
  Hbdx =  70 * cos(angleHour - 0.10); Hbdx = Hbdx + x1M;           // point bd ligne basse
  Hbdy =  70 * sin(angleHour - 0.10); Hbdy = Hbdy + y1M;

  
  Mhgx = -15 * cos(angleMin - 0.55); Mhgx = Mhgx + x1M;            // point g ligne haute
  Mhgy = -15 * sin(angleMin - 0.55); Mhgy = Mhgy + y1M;
  Mhdx =  95 * cos(angleMin + 0.05); Mhdx = Mhdx + x1M;            // point d ligne haute
  Mhdy =  95 * sin(angleMin + 0.05); Mhdy = Mhdy + y1M;

  Mbgx = -15 * cos(angleMin + 0.55); Mbgx = Mbgx + x1M;            // point g ligne basse
  Mbgy = -15 * sin(angleMin + 0.55); Mbgy = Mbgy + y1M;
  Mbdx =  95 * cos(angleMin - 0.05); Mbdx = Mbdx + x1M;            // point d ligne basse
  Mbdy =  95 * sin(angleMin - 0.05); Mbdy = Mbdy + y1M;

  lcd.setColor( 0, 0, 0);                                 // erase the minute hand in black
  lcd.drawTetragon( Mhgxprec, Mhgyprec, Mhdxprec, Mhdyprec, Mbdxprec, Mbdyprec, Mbgxprec, Mbgyprec);     // minute hand
  lcd.drawTetragon( Hhgxprec, Hhgyprec, Hhdxprec, Hhdyprec, Hbdxprec, Hbdyprec, Hbgxprec, Hbgyprec);     // hour hand
 
  lcd.setColor( 255, 0, 0);                                        // green
  lcd.drawTetragon( Hhgx, Hhgy, Hhdx, Hhdy, Hbdx, Hbdy, Hbgx, Hbgy);    // new hand

  lcd.setColor(  0, 255, 0);
  lcd.drawTetragon( Mhgx, Mhgy, Mhdx, Mhdy, Mbdx, Mbdy, Mbgx, Mbgy);    // new hand
}

void CouleurPoints()    // color of the 5 seconds dots
{
  if (myMin == (myMin / 2) * 2)
  {
    lcd.setColor(100, 100, 255); // light blue
  }
  else  {
    lcd.setColor(255, 255, 255);  // white
  }
}

int16_t SinTable[12]={-110,-95,-55,  0,55, 95,110, 95, 55,   0,-55,-95};
int16_t CosTable[12]={   0, 55, 95,110,95, 55,  0,-55,-95,-110,-95,-55};

void AiguilleSecondes()
{
  // display white/blue points each 5 second
  int x2Trait, y2Trait;

  int16_t x1M = 160;                                               // centre du cadran x et y
  int16_t y1M = 120;
  // trace des secondes
  if(mySec % 5 == 0 )
  {
    uint8_t idx = ( mySec / 5 );
    if(idx < 12 ){
      CouleurPoints();                                       // pour  les points en Red
      lcd.drawDisc( CosTable[idx]+x1M, SinTable[idx]+y1M, 7, UCG_DRAW_ALL);       // dessin des points de 5 minutes sur le cadran
    }
  }
}

void CadranBlanc()
{
 int16_t x1M = 160;                
 int16_t y1M = 120;                
               
 for(uint8_t i=0;i<12;i++){

  if(i*5 < mySec){
    if (myMin == (myMin / 2) * 2)
    {
      lcd.setColor(100, 100, 255); // light blue
    }
    else  {
      lcd.setColor(255, 255, 255);  // white
    }

  } else {
    if (myMin != (myMin / 2) * 2)
    {
      lcd.setColor(100, 100, 255); // light blue
    }
    else  {
      lcd.setColor(255, 255, 255);  // white
    }
   
  }
   lcd.drawDisc( CosTable[i]+x1M, SinTable[i]+y1M, 7, UCG_DRAW_ALL);       // dessin des points de 5 minutes sur le cadran
 }
  // end of CadranBlanc
}

void AfficherDate()
{
  lcd.setColor(255 , 255, 255);
  lcd.setPrintPos(  10, 215); lcd.print(myDate);
  lcd.setPrintPos(  10, 237); lcd.print(Day[myDow - 1]);
  lcd.setPrintPos( 280, 215); lcd.print(myYear);
  lcd.setPrintPos( 235, 237); lcd.print(Month[myMonth - 1]);
}



void HeadlineHelper(){
  lcd.clearScreen();
  lcd.setColor ( 255, 255, 255);                        // White
  lcd.drawFrame(  0,  0,  320, 220);                    // Frame for the settings

  lcd.setColor( 255, 255, 255);                         // White

  lcd.setPrintPos(  10,  20); lcd.print(F("v      -       +      ok"));
}

void AlertHelper(){
  lcd.print(F("   Hour:   Minute:   Day:"));
}

void PrintCopyright(){
    lcd.setPrintPos( 5, 236);    
    lcd.print(F(" Author: O.CROISET 2017 Rev:2.6.1"));
}

void SettingAlert()
{
  uint8_t PrevA=88;
  AlertBits.BeepOn = LOW;                                         // stop the beeper if ringing
  uint8_t Button = 0;                                                    // RaZ de Button

  /*ALARM DISPLAY*/

  NextAlarm = true;
  Alarmx = 1;
  AlarmNext = 70;
  BlackNext = 55;

  HeadlineHelper();
  PrintCopyright();
  lcd.setPrintPos(  10,  40); lcd.print(F("       SETTINGS"));
  lcd.setPrintPos(  10,  55); lcd.print(F("ALARM"));

  for (int i = 1 ; i <= 9 ; i++) {
    lcd.setPrintPos( 10, (55 + (i * 15))); lcd.print(i);
    lcd.setPrintPos( 10, (55 + (i * 15))); AlertHelper();
  }

  lcd.setColor  ( 0,   0,    0,   0);      // Black box
  lcd.drawFrame ( 0, 230,  319, 239);      // Draw box for the author
  lcd.setColor  ( 255,  255,  255);
  lcd.setPrintPos( 5, 236);    


 /* Only nine  */
  for (uint8_t i = 1 ; i <= (USERALARMCNT-1) ; i++) {
    lcd.setPrintPos( 83,  (55 + (i * 15))); lcd.print(UserAlarm[i].Hour);
    lcd.setPrintPos( 172,  (55 + (i * 15))); lcd.print(UserAlarm[i].Minute);
    lcd.setPrintPos( 237,  (55 + (i * 15))); lcd.print(Day[UserAlarm[i].Day]);
  }

  while (Alarmx != USERALARMCNT)
  {
    Button = Touches();
    lcd.setColor ( 0, 0, 255);                          // Blue
    lcd.setPrintPos( 10, AlarmNext); lcd.print(Alarmx);
    lcd.setPrintPos( 10, AlarmNext); AlertHelper();
    lcd.setColor ( 0,  0,  0);
    lcd.drawBox (80, BlackNext, 30, 20);
    do {
      UserAlarm[Alarmx].Hour = SettingHelper1(UserAlarm[Alarmx].Hour);
      UserAlarm[Alarmx].Hour = SettingHelper2(UserAlarm[Alarmx].Hour, 23, 0);
      if (UserAlarm[Alarmx].Hour != PrevA && NextAlarm == true)
      {
        delay(50);
        lcd.setColor ( 0,  0,  0);
        lcd.drawBox (80, BlackNext, 30, 20);
        lcd.setColor ( 0, 255, 0);
        lcd.setPrintPos( 83,  AlarmNext); lcd.print(UserAlarm[Alarmx].Hour); // refresh the display in red
        PrevA = UserAlarm[Alarmx].Hour;
      }
    }
    while (Button != 3 && Button != 4 && NextAlarm != false);            // while OK not pressed, else continue
    if (Button == 4) {
      NextAlarm = false;
    }
    else {
      delay(100);
    }
    Button = 0;
//    C = 0;
//    D = 0;
    lcd.setColor ( 255, 255, 255);
    lcd.setPrintPos( 83,  AlarmNext); lcd.print(UserAlarm[Alarmx].Hour);
    PrevA = 88;
    do {
      UserAlarm[Alarmx].Minute = SettingHelper1(UserAlarm[Alarmx].Minute);
      UserAlarm[Alarmx].Minute = SettingHelper2(UserAlarm[Alarmx].Minute, 59, 0);
      if (UserAlarm[Alarmx].Minute != PrevA && NextAlarm == true)
      {
        delay(50);
        lcd.setColor ( 0,  0,  0);
        lcd.drawBox (170, BlackNext, 30, 20);
        lcd.setColor ( 0, 255, 0);
        lcd.setPrintPos( 172,  AlarmNext); lcd.print(UserAlarm[Alarmx].Minute); // refresh the display in red
        PrevA = UserAlarm[Alarmx].Minute;
      }
    }
    while (Button != 3 && Button != 4 && NextAlarm != false);                                 // while OK not pressed, else continue
    if (Button == 4) {
      NextAlarm = false;
    }
    else {
      delay(100);
    }
//    Button = 0; C = 0; D = 0;                                   // reset the C counter
    lcd.setColor ( 255, 255, 255);
    lcd.setPrintPos( 172,  AlarmNext); lcd.print(UserAlarm[Alarmx].Minute);
    PrevA = 88;
    do {
      UserAlarm[Alarmx].Day = SettingHelper1(UserAlarm[Alarmx].Day);
      UserAlarm[Alarmx].Day = SettingHelper2(UserAlarm[Alarmx].Day, 8, 0);
      if (UserAlarm[Alarmx].Day != PrevA && NextAlarm == true)
      {
        delay(50);
        lcd.setColor ( 0,  0,  0);
        lcd.drawBox (233, BlackNext, 84, 20);
        lcd.setColor ( 0, 255, 0);
        lcd.setPrintPos( 237,  AlarmNext); lcd.print(Day[UserAlarm[Alarmx].Day]);
        PrevA = UserAlarm[Alarmx].Day;
      }
    }
    while (Button != 3 && Button != 4 && NextAlarm != false);                                 // while OK not pressed, else continue
    NextAlarm = true;
//    Button = 0; C = 0; D = 0;
    lcd.setColor ( 255, 255, 255);
    lcd.setPrintPos( 10, AlarmNext); lcd.print(Alarmx);
    lcd.setPrintPos( 10, AlarmNext); AlertHelper();
    lcd.setPrintPos( 83,  AlarmNext); lcd.print(UserAlarm[Alarmx].Hour);
    lcd.setPrintPos( 172,  AlarmNext); lcd.print(UserAlarm[Alarmx].Minute);
    lcd.setPrintPos( 237,  AlarmNext); lcd.print(Day[UserAlarm[Alarmx].Day]);
    PrevA = 88;
    Alarmx++;
    AlarmNext = AlarmNext + 15;
    BlackNext = BlackNext + 15;
  }
//------------------------------------------- end of alerts --------------------------------------------//
  lcd.clearScreen();
  refreshScreen = true;
  AlarmSync();
  GetNextAlert(true);
}


void Backlight_Cal_Sync(){
  uint8_t* u8Ptr;
  u8Ptr=(uint8_t*)&LDR_Calibration;
  for(uint8_t i=0;i<sizeof(LDR_CAL_t);i++){
      //EEPROM.write(LDR_CAL_OFFSET+i, *(u8Ptr));
      u8Ptr++;
  }
}



/* 
 *  A Pointer would be too much ... :-(  
 *  and a bool in return if something has pressed 
 *  
*/
int16_t SettingHelper1(int16_t Aspect) {
  //Button = Touches(); ??
/*
  if(A > (DureeAppui)) {
    Aspect--;  // left
    A = 0;
  }
  if (B > (DureeAppui)) {
    Aspect++;  // right
    B = 0;
  }
 */
  return Aspect;
}
int8_t SettingHelper2(int8_t Aspect, int8_t timeAspect1, int8_t timeAspect2) {
  if (Aspect > timeAspect1) {
    Aspect = timeAspect2 ;
  }
  if (Aspect < timeAspect2 ) {
    Aspect = timeAspect1;
  }
  return Aspect;
}

void SevenSegments(bool refresh)
{
  static uint8_t SegemenValue[4]={255,255,255,255};
 
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
  uint8_t Values[][5] = {{ (uint8_t)(myHour / 10) ,    8,  51, 255, 153},
                        {  (uint8_t)(myHour % 10),   80, 255,   0,  51},
                        {  (uint8_t)(myMin  / 10),  180, 255, 204,   0},
                        {  (uint8_t)(myMin  % 10),  248, 127, 127, 255}
  };

  for ( i = 0; i < 4; i++){
  
    digit = Values[i][0]; 
    if( ( refresh != false ) || (digit != SegemenValue[i]) ){
     SegemenValue[i]=digit;
              Numdigit = Values[i][1]; Red = Values[i][2]; Green = Values[i][3]; Blue = Values[i][4];
              lcd.setColor(0, 0, 0);
              switch (digit)  // read and display the numerical value (1) for each digit position (i)
              {
                case 0: {
                    /*SegmentG();                            // Erease the previous segments
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    SegmentA(); SegmentB(); SegmentC(); SegmentD();
                    SegmentE(); SegmentF(); */
                    Segment('G',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    Segment('F',Numdigit);
                    
                  } break;
          
                case 1: {
                  /*
                    SegmentA(); SegmentD();
                    SegmentE(); SegmentF(); SegmentG();    // Erease the previous segments
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    SegmentB(); SegmentC();
                    */
          
                    Segment('A',Numdigit);
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    
                    
                  } break;
          
                case 2: {
                    Segment('C',Numdigit);
                    Segment('F',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('B',Numdigit);
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    Segment('G',Numdigit);
                    
                    
                  } break;
          
                case 3: {
                    Segment('E',Numdigit);
                    Segment('F',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    Segment('D',Numdigit);
                    Segment('G',Numdigit);
                    
                  } break;
          
                case 4: {
               
                    Segment('A',Numdigit);
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                  } break;
          
                case 5: {
               
                    Segment('B',Numdigit);
                    Segment('E',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('C',Numdigit);
                    Segment('D',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                    
                  } break;
          
                case 6: {
                  
          
                    Segment('B',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('C',Numdigit);
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                    
                  } break;
          
                case 7: {
                 
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    
                  } break;
          
                case 8: {
                   
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    Segment('D',Numdigit);
                    Segment('E',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                  } break;
          
                case 9: {
                  
                    Segment('E',Numdigit);
                    lcd.setColor(Red, Green, Blue);        // Set the color
                    Segment('A',Numdigit);
                    Segment('B',Numdigit);
                    Segment('C',Numdigit);
                    Segment('D',Numdigit);
                    Segment('F',Numdigit);
                    Segment('G',Numdigit);
                  
                  } break;
              }                // end of switch
             } // end of if
  }                  // end of for_i loop
}                    // end of SevenSegments

// ============= deux points
void Colon()
{
  int c1, c23 ;                                // dots color definition
  int colonXpos;
  int colonYpos;

  colonXpos = 160;
  colonYpos = 70;
  c1 = 255; c23 = 255;
    if( (mySec%2) == 0){
        lcd.setColor(0, c23, c23);
    } else {
        lcd.setColor(c1,   0,   0);
    }
    lcd.drawDisc(colonXpos, colonYpos, 7, UCG_DRAW_ALL );        // upper dot
    colonYpos = 110;  
    if( (mySec%2) == 0){
        lcd.setColor(c1,   0,   0);
        
    } else {
        lcd.setColor(0, c23, c23);
    }
    lcd.drawDisc(colonXpos, colonYpos, 7, UCG_DRAW_ALL);         // lower dot
}

// ============================= end of SevenSegments
//       -a-
//    f |    |  b
//        -g-
//    e |    |  c
//       -d-     dp
//
void Segment( uint8_t idx, uint16_t offset){

  uint16_t x0 = 0;
  uint16_t y0 = 0;
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = 0;
  uint16_t x3 = 0;
  uint16_t y3 = 0;
  

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
        
     lcd.drawTetragon(x0, y0, x1, y1, x2, y2, x3, y3);  // segment g upper part
    
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
  lcd.drawTetragon(x0, y0, x1, y1, x2, y2, x3, y3);  
}


void Secondes()
{
  // affiche les points des secondes
  int xposSec;
  xposSec = (mySec * 5);
  lcd.setColor(255 , 255, 255);                // Set to white
  lcd.drawBox(xposSec + 6, 201, 4, 9);
}

void CadranSecondes()
{
  int i;
  lcd.setColor(0 , 0, 0);
  lcd.drawBox (  11, 199, 302, 11);             // inside of the secondes frame
  lcd.setColor( 255, 255, 255);                 // set to white
  lcd.drawFrame( 10, 200, 302, 11);             // secondes frame
  for (i = 1; i <= 60; i++) {
    lcd.drawVLine( (5 * i) + 5, 201, 9);  // secondes separator
  }
}

#endif
