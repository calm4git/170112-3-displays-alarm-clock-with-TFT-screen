#include "menus.h"

#define IS_LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )
static const uint8_t MonthDay[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0

uint8_t Menus::DaysInMonth( uint8_t Month, uint16_t year){
  uint8_t days = 0;
  if(Month>0){
    Month--;
  }
  if(Month<12){
    days = MonthDay[Month];
  }
  if(year>1970){
    year-=1970;
  }


  if(1==Month){
    if(false!=IS_LEAP_YEAR(year)){
      days++;
    }
  }
  return days;
}


 Menus::Menus( TFT_eSPI& lcd, AlarmMgr& Alarm){
     _lcd = &lcd;
     _Alarms = &Alarm;
     CurretnMenuState = NoMenu;
     SelectedAlarmIndex=0;
     SelectedAlarmOptionIndex=0;
     SelectedSettingsOptions=0;
     
    SelectedTimeIndex=0;
    SelectedDateIndex=0;
     _GetTime=nullptr;
     _SetTime=nullptr;

 }

void Menus::SetCallBackForGetTime( time_t (*GetTime)( void ) ){
    _GetTime=GetTime;
}
void Menus::SetCallBackForSetTime( void (*SetTime)(time_t utc_now) ){
    _SetTime=SetTime;
}

//Returns false if in NoMenu....
bool Menus::RenderMenu( KeyAction_t key, time_t utc_now ){ 
    
    //We will render into the AlarmList
    switch(key){
        case KeyA_Pressed:{
            Serial.println("Key A");
        }break;

        case KeyB_Pressed:{
            Serial.println("Key B");
        } break;

        case KeyC_Pressed:{
            Serial.println("Key C");
        }break;

        case KeyD_Pressed:{
            Serial.println("Key D");
        }break;

        default:{
           
        }break;


    }
    switch( CurretnMenuState ){
        
        case NoMenu:{
            //If A is pressed we will show the AlarmList
            if(key==KeyA_Pressed){
                CurretnMenuState = MenuAlarmList;
                ShowMenuAlarmList(SelectedAlarmIndex, true);
            }

            if(key==KeyD_Pressed){ //We would enter the timesettings here
                CurretnMenuState = SettingsList;
                ShowMenuSettingsList(SelectedSettingsOptions,true);
            }
        } break;

        case MenuAlarmList:{
            if( (key==KeyB_Pressed) || (key==KeyC_Pressed) ){
                if(key==KeyB_Pressed){
                    if( ( SelectedAlarmIndex+1 ) < _Alarms->GetMaxAlarms()  ){
                        SelectedAlarmIndex++;
                    }
                }

                if(key==KeyC_Pressed){
                    if(SelectedAlarmIndex>0){
                        SelectedAlarmIndex--;
                    }
                }

                ShowMenuAlarmList(SelectedAlarmIndex,false);
            }
            if(key==KeyA_Pressed){
                CurretnMenuState = MenuAlarmSetting;
                SelectedAlarmOptionIndex=0;
                Alarm::Alarmtime_t At;
                if(true == _Alarms->GetAlarm(SelectedAlarmIndex,&At)){
                  ShowAlarmSetup( At,SelectedAlarmOptionIndex, true );
                }
            }

            if(key==KeyD_Pressed){
                CurretnMenuState = NoMenu;
                SelectedAlarmIndex=0;
            }

        } break;

        case MenuAlarmSetting:{
            //The currenlty shown alarm is now here displayed
            //We need to move our settingscursor 
            if( (key==KeyB_Pressed) || (key==KeyC_Pressed) ){
                if(key==KeyB_Pressed){
                    if( ( SelectedAlarmOptionIndex) < 10  ){
                        SelectedAlarmOptionIndex++;
                    }
                }

                if(key==KeyC_Pressed){
                    if(SelectedAlarmOptionIndex>0){
                        SelectedAlarmOptionIndex--;
                    }
                }
                Alarm::Alarmtime_t At;
                if(true == _Alarms->GetAlarm(SelectedAlarmIndex,&At)){
                  if(false == At.Enabled){
                    SelectedAlarmOptionIndex=0;
                  }
                  ShowAlarmSetup( At,SelectedAlarmOptionIndex,false );
                }
               
            }

            if(key==KeyA_Pressed){
                Alarm::Alarmtime_t At;
                if(true == _Alarms->GetAlarm(SelectedAlarmIndex,&At)){
                  AlarmSettings(At, SelectedAlarmOptionIndex);
                  _Alarms->SetAlarm(SelectedAlarmIndex, At);                 
                }
                if(true == _Alarms->GetAlarm(SelectedAlarmIndex,&At)){
                  ShowAlarmSetup( At,SelectedAlarmOptionIndex,false );
                }
            
            }

            if(key==KeyD_Pressed){
                CurretnMenuState = MenuAlarmList;
                ShowMenuAlarmList(SelectedAlarmIndex,true);
            }

        } break;

        case SettingsList:{
             if(key==KeyD_Pressed){
              //Back to no menu
              CurretnMenuState = NoMenu;
             }

            if(key==KeyB_Pressed){
                if(SelectedSettingsOptions<4){
                  SelectedSettingsOptions++;
                  ShowMenuSettingsList(SelectedSettingsOptions,true);
                }
             }


             if(key==KeyC_Pressed){
                if(SelectedSettingsOptions>0){
                  SelectedSettingsOptions--;
                  ShowMenuSettingsList(SelectedSettingsOptions,true);
                }
             }

             
             if(key==KeyA_Pressed){
               //Enter submenu...
               switch( SelectedSettingsOptions ){
                 case 0:{
                   CurretnMenuState=MenuTimeSettings;
                   tmElements_t tm;
                   breakTime(utc_now,tm);
                   DrawTimeSettings(tm.Second,tm.Minute,tm.Hour,0,true);
                 }break;

                 case 1:{
                  tmElements_t tm;
                  breakTime(utc_now,tm);
                  CurretnMenuState=MenuDateSettings;
                  DrawDateSettings(tm.Day,tm.Month,tm.Year,0,true);
                 }break;

                 case 2:{

                 }break;

                 case 3:{

                 }break;

                 case 4:{

                 }break;

                 default:{
                   //We are out of range....
                   SelectedSettingsOptions=4;
                 }
               }
             }

        }break;

        case MenuTimeSettings:{
             static time_t last_call=0;
             tmElements_t tm;
             breakTime(utc_now,tm);
             
             if(key==KeyA_Pressed){
               switch(SelectedTimeIndex){
                 case 0:{
                   //Hour++ , if we are at 23 hours we will "jump" to zero by substracting 23
                  if(tm.Hour<23){
                    tm.Hour++;
                  } else {
                    tm.Hour=0;
                  }
                  if(_SetTime!=nullptr){
                    _SetTime(makeTime(tm));
                  }
                 }break;

                 case 1:{
                   //Minuets++, if we at 59 we "jump" to zero by sunstarcting 59
                   if(tm.Minute<59){
                     tm.Minute++;
                   } else {
                     tm.Minute=0;
                   }
                   if(_SetTime!=nullptr){
                    _SetTime(makeTime(tm));
                  }
                 }break;

                 case 2:{
                   //This works different as we only set seconds to zero
                   tm.Second=0;
                   if(_SetTime!=nullptr){
                    _SetTime(makeTime(tm));
                  }
                 }break;

                 default:{
                  //Do nothing...
                 }
               }
               //We need to set the time again 
             }

            if(key==KeyB_Pressed){
               SelectedTimeIndex++;
               if(SelectedTimeIndex>2){
                 SelectedTimeIndex=2;
               }
               DrawTimeSettings(tm.Second,tm.Minute,tm.Hour,SelectedTimeIndex,false);
             }

            if(key==KeyC_Pressed){
               if(SelectedTimeIndex>0){
                 SelectedTimeIndex--;
               }
               DrawTimeSettings(tm.Second,tm.Minute,tm.Hour,SelectedTimeIndex,false);
             }

  	        if(key==KeyD_Pressed){
               CurretnMenuState = SettingsList;
               ShowMenuSettingsList(SelectedSettingsOptions,true);

             }
             

             if( (key==NothingPressed) && (last_call != utc_now) ){
               last_call=utc_now;
               DrawTimeSettings(tm.Second,tm.Minute,tm.Hour,SelectedTimeIndex,false);
             }
        }break;

        case MenuDateSettings:{
             static time_t last_call=0;
             tmElements_t tm;
             breakTime(utc_now,tm);
             
             if(key==KeyA_Pressed){
              SelectedDateIndex++;
              if(SelectedDateIndex>2){
                SelectedDateIndex=0;
              }
             }

             if(key==KeyB_Pressed){
              switch(SelectedDateIndex){
                case 0:{
                  tm.Day++;
                  if(tm.Day>DaysInMonth(tm.Month,tm.Year+1970)){
                    tm.Day=1;
                  }
                }break;

                case 1:{
                    if(tm.Month<11){
                      tm.Month++;
                    } else {
                      tm.Month=1;
                    }
                }break;

                case 2:{
                  if(tm.Year<68){
                      tm.Year++;
                  } else {
                    tm.Year = 68;
                  }
                }break;

                default:{

                }break;
              }
              if(_SetTime!=nullptr){
                    _SetTime(makeTime(tm));
              }
              DrawDateSettings(tm.Day,tm.Month,tm.Year,SelectedDateIndex,false);
             
             }

             if(key==KeyC_Pressed){
                switch(SelectedDateIndex){
                  case 0:{
                  if(tm.Day>1){
                  tm.Day--;
                  }else{
                   tm.Day=DaysInMonth(tm.Month,tm.Year+1970);                  
                  }
                  }break;

                  case 1:{
                    if(tm.Month>1){
                      tm.Month--;
                    } else {
                      tm.Month=11;
                    }

                  }break;

                  case 2:{
                    if(tm.Year>0){
                      tm.Year--;
                    }

                  }break;

                  default:{

                  }break;
                }
                if(_SetTime!=nullptr){
                    _SetTime(makeTime(tm));
                }
                DrawDateSettings(tm.Day,tm.Month,tm.Year,SelectedDateIndex,false);
             }
             
             if(key==KeyD_Pressed){
               CurretnMenuState = SettingsList;
               ShowMenuSettingsList(SelectedDateIndex,true);
             }
             
             
             
             if( (key==NothingPressed) && (last_call != utc_now) ){
              tmElements_t tm;
              last_call=utc_now;
              breakTime(utc_now,tm);
              DrawDateSettings(tm.Day,tm.Month,tm.Year,SelectedDateIndex,false);
             }
        }break;

        default:{
          //If we end here we will leave the menu 
          CurretnMenuState = NoMenu;
        } break;
    }

    return ( NoMenu == CurretnMenuState );
    
}

//----------------- Alarm Settings ----------------------------------- //
void Menus::DrawStatusWeekday(uint16_t offset_x, uint16_t offset_y,uint8_t Dow, bool Enabled , bool selected){
  _lcd->setCursor(10+offset_x,55+offset_y);
  _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
  _lcd->setFreeFont(FSB9); 
  switch(Dow){
    case 0:{
      _lcd->print("Mo");
    } break;   

    case 1:{
      _lcd->print("Tu");
    } break;   

    case 2:{
      _lcd->print("We");
    } break;   

    case 3:{
      _lcd->print("Th");
    } break;   

    case 4:{
      _lcd->print("Fr");
    } break;   

    case 5:{
      _lcd->print("Sa");
    } break;   

    case 6:{
      _lcd->print("Su");
    } break;   

  }
  if(false == selected){
    for(uint8_t i=0;i<3;i++){
      _lcd->drawRect(10+offset_x-i,60+offset_y-i,23+(i*2),23+(i*2),_lcd->color565( 112,116,122 ));
    }
    _lcd->drawRect(10+offset_x,60+offset_y,25,25,TFT_BLACK);
    

    
  } 
  else {
    for(uint8_t i=0;i<3;i++){
      _lcd->drawRect(10+offset_x-i,60+offset_y-i,23+(i*2),23+(i*2),TFT_BLUE);
    }

  }
  //This will build the checked image in green 
  if(true == Enabled){
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(14+i+offset_x,64+offset_y,31-i+offset_x,81+offset_y, _lcd->color565( 112,116,122 ));
      _lcd->drawLine(14+i+offset_x,81+offset_y,31-i+offset_x,64+offset_y, _lcd->color565( 112,116,122 ));
    }

    for(uint8_t i=0;i<6;i++) {
      _lcd->drawLine(12+i+offset_x,71+offset_y,20+i+offset_x,80+offset_y,TFT_GREEN);
    }
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(25-i+offset_x,80+offset_y,32-(i/3)+offset_x,63+offset_y, TFT_GREEN);
    }

  } else {
    
     for(uint8_t i=0;i<6;i++) {
      _lcd->drawLine(12+i+offset_x,71+offset_y,20+i+offset_x,80+offset_y, _lcd->color565( 112,116,122 ));
    }
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(25-i+offset_x,80+offset_y,32-(i/3)+offset_x,63+offset_y, _lcd->color565( 112,116,122 ));
    }

    
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(14+i+offset_x,64+offset_y,31-i+offset_x,81+offset_y, TFT_RED);
      _lcd->drawLine(14+i+offset_x,81+offset_y,31-i+offset_x,64+offset_y, TFT_RED);
    }

  }

}

void Menus::DrawAlarmEnabled(uint16_t offset_x,uint16_t offset_y ,bool Ena, bool selected){

if(false == selected){
  
  for(uint8_t i=1;i<4;i++){
    _lcd->drawRect(10+offset_x-i,50+offset_y-i,25+(i*2),25+(i*2),_lcd->color565( 112,116,122 ));
  }
  _lcd->drawRect(10+offset_x,50+offset_y,25,25,TFT_BLACK);
} else {
  for(uint8_t i=0;i<4;i++){
    _lcd->drawRect(10+offset_x-i,50+offset_y-i,25+(i*2),25+(i*2),TFT_BLUE);
  }
}
  //This will build the checked image in green 
  if(true == Ena){
    for(uint8_t i=0;i<6;i++) {
      _lcd->drawLine(12+i+offset_x,61+offset_y,20+i+offset_x,70+offset_y,TFT_GREEN);
    }
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(25-i+offset_x,70+offset_y,32-(i/3)+offset_x,53+offset_y, TFT_GREEN);
    }
  } else {
    for(uint8_t i=0;i<6;i++) {
      _lcd->drawLine(12+i+offset_x,61+offset_y,20+i+offset_x,70+offset_y,_lcd->color565( 112,116,122 ));
    }
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(25-i+offset_x,70+offset_y,32-(i/3)+offset_x,53+offset_y, _lcd->color565( 112,116,122 ));
    }
  }

  _lcd->setCursor(46+offset_x,70+offset_y);
  _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
  _lcd->setFreeFont(FSB12); 
  _lcd->print("Enabled");
}

void Menus::DrawAlarmTimeHHMM(uint16_t offset_x,uint16_t offset_y, uint8_t hours, uint8_t minutes, bool hour_selected, bool minitues_selected ){

  _lcd->setCursor(10+offset_x,80+offset_y);
  _lcd->setFreeFont(FSB24); 
  
  _lcd->fillRect(10+offset_x,45+offset_y,110,40, _lcd->color565( 112,116,122 ) );

  if(false == hour_selected){
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
  } else {
    _lcd->setTextColor(TFT_BLUE ,TFT_BLUE);
  }
  if(hours<10){
    _lcd->print("0");
  }
  _lcd->print(hours);
  _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
  _lcd->print(":");
  
   if(false == minitues_selected){
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
  } else {
    _lcd->setTextColor(TFT_BLUE ,TFT_BLUE);
  }
  if(minutes<10){
    _lcd->print("0");
  }
  _lcd->print(minutes);

}

void Menus::DrawOneShot(uint16_t offset_x,uint16_t offset_y ,bool Ena, bool selected){


  if(false == selected){
    
    for(uint8_t i=1;i<4;i++){
      _lcd->drawRect(10+offset_x-i,50+offset_y-i,25+(i*2),25+(i*2),_lcd->color565( 112,116,122 ));
    }
    _lcd->drawRect(10+offset_x,50+offset_y,25,25,TFT_BLACK);
  } else {
    for(uint8_t i=0;i<4;i++){
      _lcd->drawRect(10+offset_x-i,50+offset_y-i,25+(i*2),25+(i*2),TFT_BLUE);
    }
  }

  //This will build the checked image in green 
  if(true == Ena){
    for(uint8_t i=0;i<6;i++) {
      _lcd->drawLine(12+i+offset_x,61+offset_y,20+i+offset_x,70+offset_y,TFT_GREEN);
    }
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(25-i+offset_x,70+offset_y,32-(i/3)+offset_x,53+offset_y, TFT_GREEN);
    }
  } else {
     for(uint8_t i=0;i<6;i++) {
      _lcd->drawLine(12+i+offset_x,61+offset_y,20+i+offset_x,70+offset_y,_lcd->color565( 112,116,122 ));
    }
    for(uint8_t i=0;i<6;i++){
      _lcd->drawLine(25-i+offset_x,70+offset_y,32-(i/3)+offset_x,53+offset_y, _lcd->color565( 112,116,122 ));
    }
  }

  _lcd->setCursor(46+offset_x,70+offset_y);
  _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
  _lcd->setFreeFont(FSB12); 
  _lcd->print("Only Once");
}

void Menus::ShowAlarmSetup(  Alarm::Alarmtime_t At, uint8_t selected_option ,bool refresh){
  /* Draw Headline */
  if(true == refresh ){
    _lcd->fillRect(0,0,320,40,_lcd->color565( 45,47,50 ) );
    _lcd->setTextColor(_lcd->color565( 112,116,122 ),TFT_BLACK);
    _lcd->setFreeFont(FSB18); 
    _lcd->setCursor(160- ( _lcd->textWidth("Alarm settings") / 2) ,30);
    _lcd->print("Alarm settings");
    _lcd->fillRect(0,40,320,200,_lcd->color565( 112,116,122 ));
  }
  /* This will be for one day */
  DrawAlarmEnabled(0,0,At.Enabled, (0 == selected_option));
    if(true==At.Enabled){
      for(uint8_t i=0;i<7;i++){
        DrawStatusWeekday(45+i*35,40,i,Alarm::AlarmGetEnableDow(i,At) , ( (i+1) == selected_option));
      }
      DrawOneShot(0,85,At.OneShot, 8 == selected_option);
      DrawAlarmTimeHHMM(40,130,At.Hour ,At.Minute, 9 == selected_option, 10 == selected_option);
    } else {
      _lcd->fillRect(0,80,320,160,_lcd->color565( 112,116,122 ) );
    }
  
 

}


// ------------- This is the List of Alarms in the Systems --------------------------

void Menus::DrawAlarmMenuEntry( uint32_t offset, bool selected,  Alarm::Alarmtime_t At  ){
  uint8_t active_days=0;
  
  if(selected == true ){
    _lcd->fillRect(0,40+offset,300,40,_lcd->color565( 92,96,102 ) );
  } else {
    _lcd->fillRect(0,40+offset,300,40,TFT_WHITE );
  }
  _lcd->drawRect(0,40+offset,300,40,_lcd->color565( 112,116,122 ));
  _lcd->setTextColor(TFT_BLACK,TFT_BLACK);
  _lcd->setFreeFont(FSB12); 
  _lcd->setCursor(0,65+offset);
  
  if(selected == true ){
    _lcd->print(" >");
  }
  
  _lcd->setCursor(26,70+offset);
  if(false == At.Enabled){
    _lcd->print("Alarm disabled");
  } else {
    if(true == At.UseDate){
        if(At.Hour<10){
          _lcd->print("0");
        } 
        _lcd->print(At.Hour);
        _lcd->print(":");
        if(At.Minute<10){
          _lcd->print("0");
        }
        _lcd->print(At.Minute);
        _lcd->setFreeFont(FSB9); 
        _lcd->setCursor(120,47+offset);
        _lcd->print(At.Day);
        _lcd->print(".");
        _lcd->print(At.Month);
        _lcd->print(".");
        _lcd->print(At.Year);

    } else {
        _lcd->setCursor(30,67+offset);
        if(At.Hour<10){
          _lcd->print("0");
        } 
        _lcd->print(At.Hour);
        _lcd->print(":");
        if(At.Minute<10){
          _lcd->print("0");
        }
        _lcd->print(At.Minute);
        _lcd->setFreeFont(FSB9); 
        _lcd->setCursor(120,57+offset);
        //We generate the Dow entry...
        if(At.Monday>0){
          _lcd->print("Mo");
          active_days++;
          
          if(active_days==5){
             _lcd->setCursor(120,74+offset);
          }
        }

        if(At.Tursday>0){
          if(active_days>0){
            _lcd->print(", ");
          }
          
          _lcd->print("Tu");
          active_days++;
          
          if(active_days==5){
             _lcd->setCursor(120,74+offset);
          }

        }
        
        if(At.Wednesday>0){
          if(active_days>0){
            _lcd->print(", ");
          }
          
          _lcd->print("We");
          active_days++;

          if(active_days==5){
             _lcd->setCursor(120,74+offset);
          }
        }

        if(At.Thuseday>0){
          if(active_days>0){
            _lcd->print(", ");
          }
          
          _lcd->print("Th");
          active_days++;

          if(active_days==5){
              _lcd->setCursor(120,74+offset);
          }
        }

        if(At.Friday>0){
          if(active_days>0){
            _lcd->print(", ");
          }
          
          _lcd->print("Fr");
          active_days++;

          if(active_days==5){
              _lcd->setCursor(120,74+offset);
          }
        }

       
        
        if(At.Saturday > 0){
          if(active_days>0){
            _lcd->print(", ");
          }
          
          _lcd->print("Sa");
          active_days++;

          if(active_days==5){
              _lcd->setCursor(120,74+offset);
          }
        }
        if(At.Sunday > 0){
          if(active_days>0){
            _lcd->print(", ");
          }
          
          _lcd->print("Su");
          active_days++;

          if(active_days==5){
              _lcd->setCursor(120,74+offset);
          }
        }
        
        _lcd->setCursor(230,74+offset);
        
        if(true == At.OneShot){
          _lcd->print("Once");
        }
    }
  }
}

//We need to set if we have already drawn stuff to reduce flicker 
//as good as possible
void Menus::ShowMenuAlarmList( uint8_t selected_idx, bool refresh ){
  Alarm::Alarmtime_t At ;
  /* Draw Headline */
  if(true == refresh){
    _lcd->fillRect(0,0,320,40,_lcd->color565( 45,47,50 ) );
    
    _lcd->setTextColor(_lcd->color565( 112,116,122 ),TFT_BLACK);
    _lcd->setFreeFont(FSB18); 
    _lcd->setCursor(160- ( _lcd->textWidth("Alarms") / 2) ,30);
    _lcd->print("Alarms");
    
  } 

  //Draw scrolling element
  //_lcd->fillRect(302,43,15,196,TFT_WHITE);
  _lcd->fillRect(303,43,15,197,TFT_WHITE);
  _lcd->drawRect(300,40,20,200,_lcd->color565( 112,116,122 ));
  _lcd->drawRect(301,41,18,198,_lcd->color565( 112,116,122 ));
  _lcd->drawRect(302,42,16,196,_lcd->color565( 112,116,122 ));
  //Next is to draw the elements as far as possibel....

  
  //We calcualte the range we need to draw or like to draw
  //We can display 5 Elements 
  uint8_t startidx=0;
  if(selected_idx>4){
    startidx = selected_idx -4 ;
  }

  //We set the scrollbar accordingly
  _lcd->fillRect(305,45 +(196/8)*selected_idx ,10,(196/8)-2 , TFT_DARKGREY);

  for(uint8_t i=startidx;i<_Alarms->GetMaxAlarms();i++){
    if(true == _Alarms->GetAlarm(i,&At) ){
      DrawAlarmMenuEntry( (40*(i-startidx))  ,( i==selected_idx ), At );
    } 
  }
 


}

void  Menus::AlarmSettings( Alarm::Alarmtime_t &At, uint8_t option_idx ){
    //Checkboxes are easy....
    //other things are more complicated...
    switch (option_idx){
      case 0:{
        At.Enabled = !At.Enabled;
      }break;

      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      {
          Alarm::AlarmToggleEnabledDow(option_idx-1,At);
      } break;

      case 8:{
        At.OneShot = !At.OneShot;
      } break;

      case 9:{
        if(At.Hour<23){
          At.Hour++; 
        } else {
          At.Hour=0;
        }
      }break;

      case 10:{
        if(At.Minute<59){
        
          At.Minute++; 
        
        } else {

          At.Minute=0; 
        
        }

      } break;

      default:{

      }break;



    }

}


void Menus::DrawTimeSettings(  uint8_t Second, uint8_t Minute, uint16_t Hour, uint8_t selected_option, bool refresh ){
    static uint8_t last_Second=0;
    static uint8_t last_Minute=0;
    static uint8_t last_Hour=0;
    static uint8_t last_selectedidx=0;

    if( (true == refresh ) || (last_selectedidx!=selected_option)){
          last_selectedidx =selected_option;
          last_Second=255;
          last_Minute=255;
          last_Hour=255;
    }

    /* Draw Headline */
    if(true == refresh){
      _lcd->fillRect(0,0,320,40,_lcd->color565( 45,47,50 ) );
      _lcd->setTextColor(_lcd->color565( 112,116,122 ),TFT_BLACK);
      _lcd->setFreeFont(FSB12); 
      _lcd->setCursor(160- ( _lcd->textWidth("Time settings") / 2) ,30);
      _lcd->print("Time settings");
      _lcd->fillRect(0,40,320,200,_lcd->color565( 112,116,122 ));
    }

    if(true == refresh){
      _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
      _lcd->setCursor(10,80);
      _lcd->print("Hour:");
      
    }
    _lcd->setCursor(100,80);
    
    if(0 == selected_option ){
      _lcd->setTextColor(TFT_RED ,TFT_BLACK);
    }


    
    if(last_Hour != Hour){
      last_Hour = Hour;
      _lcd->fillRect(100,61,30,20, _lcd->color565( 112,116,122 ));
      if(Hour<10){
        _lcd->print("0");
      }
      _lcd->print(Hour);
    }
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
    
    if(true == refresh){
      _lcd->setCursor(10,120);
      _lcd->print("Minute:");
    }

   
    _lcd->setCursor(100,120);
    if(1 == selected_option ){
      _lcd->setTextColor(TFT_RED ,TFT_BLACK);
    }

    if(last_Minute != Minute){
      last_Minute = Minute;
      _lcd->fillRect(100,101,30,20, _lcd->color565( 112,116,122 ));
      if(Minute<10){
        _lcd->print("0");
      }
      _lcd->print(Minute);
    }
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
    
     
    if(true == refresh){
      _lcd->setCursor(10,160);
      _lcd->print("Second:");
    }
   
    _lcd->setCursor(100,160);
    if(2 == selected_option ){
      _lcd->setTextColor(TFT_RED ,TFT_BLACK);
    }
    
    if(last_Second != Second){
      last_Second = Second;
      _lcd->fillRect(100,141,30,20, _lcd->color565( 112,116,122 ));
      if(Second<10){
        _lcd->print("0");
      }
      _lcd->print(Second);
    }
    
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
    

}

void Menus::DrawDateSettings(uint8_t Day, uint8_t Month, uint16_t Year, uint8_t selected_option, bool refresh){
    static uint8_t _Day=255;
    static uint8_t _Month=255;
    static uint16_t _Year=255;
    static uint8_t last_selected_idx=0;

    if( (true == refresh ) || ( last_selected_idx != selected_option) ){
      _Day=255;
      _Month=255;
      _Year=255;
      last_selected_idx=selected_option;
    }


    /* Draw Headline */
    if(true == refresh ){
      _lcd->fillRect(0,0,320,40,_lcd->color565( 45,47,50 ) );
      _lcd->setTextColor(_lcd->color565( 112,116,122 ),TFT_BLACK);
      _lcd->setFreeFont(FSB12); 
      _lcd->setCursor(160- ( _lcd->textWidth("Date settings") / 2) ,30);
      _lcd->print("Date settings");
      _lcd->fillRect(0,40,320,200,_lcd->color565( 112,116,122 ));
    }
  
    if(true == refresh ){
      _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
      _lcd->setCursor(10,80);
      _lcd->print("Day:");
    }
    if(0 == selected_option ){
      _lcd->setTextColor(TFT_RED ,TFT_BLACK);
    } 
    if( _Day != Day ){
      _Day = Day;
      _lcd->fillRect(100,58,40,25, _lcd->color565( 112,116,122 ) );
      _lcd->setCursor(100,80);
      if(Day<10){
        _lcd->print("0");
      }
      _lcd->print(Day);
    }
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
    
    if(true == refresh){
      _lcd->setCursor(10,120);
      _lcd->print("Month:");
      
    }

    if(1 == selected_option ){
      _lcd->setTextColor(TFT_RED ,TFT_BLACK);
    }
    if(_Month != Month ){
      _Month = Month;
      _lcd->setCursor(100,120);
      _lcd->fillRect(100,95,160,40, _lcd->color565( 112,116,122 ) );
      switch(Month){
        case 1:{
          _lcd->print("January");
        }break;
        case 2:{
          _lcd->print("February");
        }break;
        case 3:{
          _lcd->print("March");
        }break;
        case 4:{
          _lcd->print("April");
        }break;
        case 5:{
          _lcd->print("May");
        }break;
        case 6:{
          _lcd->print("June");
        }break;
        case 7:{
          _lcd->print("July");
        }break;
        case 8:{
          _lcd->print("August");
        }break;
        case 9:{
          _lcd->print("September");
        }break;
        case 10:{
          _lcd->print("October");
        }break;
        case 11:{
          _lcd->print("November");
        }break;
        case 12:{
          _lcd->print("December");
        }break;
        default:{
          _lcd->print("???");
        }break;

      }
    }
    _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
    
    if(true == refresh ){
    _lcd->setCursor(10,160);
    _lcd->print("Year:");
  
    }
    if(2 == selected_option ){
      _lcd->setTextColor(TFT_RED ,TFT_BLACK);
    } 
    
    if( _Year != Year){
      _Year = Year;
      _lcd->setCursor(100,160);
       _lcd->fillRect(100,138,60,25, _lcd->color565( 112,116,122 ) );
      if((Year+1970) < 100){
        _lcd->print("0");
      }

      if( (Year+1970) < 1000){
        _lcd->print("0");
      }
      _lcd->print((Year+1970));
    
    }
     _lcd->setTextColor(TFT_BLACK ,TFT_BLACK);
    

}


void Menus::DrawSettingsMenuEntry( uint32_t offset, char* Name,  bool selected ){

  
  if(selected == true ){
    _lcd->fillRect(0,40+offset,300,40,_lcd->color565( 92,96,102 ) );
  } else {
    _lcd->fillRect(0,40+offset,300,40,TFT_WHITE );
  }
  _lcd->drawRect(0,40+offset,300,40,_lcd->color565( 112,116,122 ));
  _lcd->setTextColor(TFT_BLACK,TFT_BLACK);
  _lcd->setFreeFont(FSB12); 
  _lcd->setCursor(0,65+offset);
  
  if(selected == true ){
    _lcd->print(" >");
  }
  
  _lcd->setCursor(26,70+offset);
    _lcd->print(Name);


}

void Menus::ShowMenuSettingsList( uint8_t selected_idx, bool refresh ){


  

  /* Draw Headline */
  if(true == refresh){
    _lcd->fillRect(0,0,320,40,_lcd->color565( 45,47,50 ) );
    
    _lcd->setTextColor(_lcd->color565( 112,116,122 ),TFT_BLACK);
    _lcd->setFreeFont(FSB18); 
    _lcd->setCursor(160- ( _lcd->textWidth("Settings") / 2) ,30);
    _lcd->print("Settings");
    
  } 

  //Draw scrolling element
  //_lcd->fillRect(302,43,15,196,TFT_WHITE);
  _lcd->fillRect(303,43,15,197,TFT_WHITE);
  _lcd->drawRect(300,40,20,200,_lcd->color565( 112,116,122 ));
  _lcd->drawRect(301,41,18,198,_lcd->color565( 112,116,122 ));
  _lcd->drawRect(302,42,16,196,_lcd->color565( 112,116,122 ));
  //Next is to draw the elements as far as possibel....

  
  //We calcualte the range we need to draw or like to draw
  //We can display 5 Elements 
  if(selected_idx>4){
    selected_idx = 4 ;
  }

  //We set the scrollbar accordingly
  _lcd->fillRect(305,45 ,10,(196)-7 , TFT_DARKGREY);

  DrawSettingsMenuEntry(40*0,"Time",( 0 == selected_idx ));
  DrawSettingsMenuEntry(40*1,"Date",( 1 == selected_idx ));
  DrawSettingsMenuEntry(40*2,"Display",( 2 == selected_idx ));
  DrawSettingsMenuEntry(40*3,"Ringtone",( 3 == selected_idx ));
  DrawSettingsMenuEntry(40*4,"About",( 4 == selected_idx ));
 


}

