/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#include "EEPROM.h"
 
#define Ch1SignalPin A0
#define Ch1TrashPin A1
#define Ch2SignalPin A2
#define Ch2TrashPin A3

#define IndCh1 A4
#define IndCh2 A5

#define Ch1StrtOn 12  //сканирование переключателя задержеи старта = 0
#define Ch1StpOn 13 //сканирование переключателя задержеи стопа = 0

#define SaveFr 6//сохранить значение частоты
#define Fr 7// частота

#define D0 8
#define D1 9
#define D2 10
#define D3 11

byte Ch1DelStrt;//задержка включения аудиотриггера  относительно времени появления аудиосигнала, превышающего TRASH
byte Ch1DelStp;//задержка выключения аудиотриггера  относительно времени  пропадания аудиосигнала, меньше, чем TRASH
byte Ch2DelStrt;
byte Ch2DelStp;
byte Ch1Del;//по каалу 1: задержка выключения-выключения аудиотриггера YYYYXXXX. хххх - задержка включения YYYY - задержка выключения
byte Ch2Del;//по каалу 2: 

unsigned long Ch1Time1,Ch1Time2,Ch1Time3,Ch1Time4, Ch1Delta ;//для измерения времени

boolean IsSignal;//присутствие аудиосигнала , больше чем TRASH
boolean Start, Stop;//аудиотриггер запущен

int n;//

int Htime;       // целочисленная переменная для хранения времени высокого логического уровня
int Ltime;       // целочисленная переменная для хранения времени низкого логического уровня
float Ttime;     // переменная для хранения общей длительности периода
float frequency; // переменная для хранения частоты
int freqCounter = 0;
long preMillis = 0;
int SfreqCounter;


void writeIntIntoEEPROM(int address, int number)
{
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}



void isr() //interrupt service routine
{
  freqCounter++;
}

void DelOn()
{     //Serial.print(" Ch1Delta ");
      //Serial.println(Ch1Delta);
      switch (Ch1DelStrt) // запуск задержки в соответствии с переключателем
      {
       case  0:
          //Serial.println(" 0 ms");
          digitalWrite(IndCh1, HIGH);
          break;
       case 1:
          //Serial.println(" 50 ms");
          if (Ch1Delta>=50) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 2:
          //Serial.println(" 100 ms");
          if (Ch1Delta>=100)
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case  3:
          //Serial.println(" 200 ms");
          if (Ch1Delta>=200) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else 
          {
            digitalWrite(IndCh1, LOW);
          }
          break;
       case 4:
          //Serial.println(" 400 ms");
          if (Ch1Delta>=400)
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 5:
          //Serial.println(" 800 ms");
          if (Ch1Delta>=800) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case  6:
          //Serial.println(" 1.5 s");
          if (Ch1Delta>=1500)
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 7:
          //Serial.println(" 2.2 s");
          if (Ch1Delta>=2200) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          //else digitalWrite(IndCh1, LOW);
          break;
       case 8:
          //Serial.println(" 3 s");
          if (Ch1Delta>=3000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case  9:
          //Serial.println(" 4 s");
          if (Ch1Delta>=4000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 10:
          //Serial.println(" 5 s");
          if (Ch1Delta>=5000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 11:
          //Serial.println(" 8 s");
          if (Ch1Delta>=8000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 12:
          //Serial.println(" 15 s");
          if (Ch1Delta>=15000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
          
       case  13:
          //Serial.println(" 25 s");
          if (Ch1Delta>=25000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 14:
          //Serial.println(" 40 s");
          if (Ch1Delta>=40000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
       case 15:
          //Serial.println(" 60 s");
          if (Ch1Delta>=60000) 
          {
            digitalWrite(IndCh1, HIGH);
          }
          else digitalWrite(IndCh1, LOW);
          break;
 
          
       default:
       Serial.println("x");
          // код для выполнения
      }
  
}

void DelOff()
{     //Serial.print(" Ch1Delta ");
      //Serial.println(Ch1Delta);
    switch (Ch1DelStp) // запуск задержки выключения в соответствии с переключателем
      {
       case  0:
          //Serial.println(" 0 ms");
         if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          break;
       case 1:
          //Serial.println(" 50 ms");
          if (Ch1Delta>=50) 
          {
            if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 2:
          //Serial.println(" 100 ms");
          if (Ch1Delta>=100)
          {
            if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case  3:
          //Serial.println(" 200 ms");
          if (Ch1Delta>=200) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 4:
          //Serial.println(" 400 ms");
          if (Ch1Delta>=400)
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 5:
          //Serial.println(" 800 ms");
          if (Ch1Delta>=800) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case  6:
          //Serial.println(" 1.5 s");
          if (Ch1Delta>=1500)
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 7:
          //Serial.println(" 2.2 s");
          if (Ch1Delta>=2200) 
          {
           //if   (digitalRead(Fr)==HIGH) 
           digitalWrite(IndCh1, LOW);
          }
          //else digitalWrite(IndCh1, HIGH);
          break;
       case 8:
          //Serial.println(" 3 s");
          if (Ch1Delta>=3000) 
          {
           //if   (digitalRead(Fr)==HIGH) 
           digitalWrite(IndCh1, LOW);
          }
          //else digitalWrite(IndCh1, HIGH);
          break;
       case  9:
          //Serial.println(" 4 s");
          if (Ch1Delta>=4000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 10:
          //Serial.println(" 5 s");
          if (Ch1Delta>=5000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 11:
          //Serial.println(" 8 s");
          if (Ch1Delta>=8000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 12:
          //Serial.println(" 15 s");
          if (Ch1Delta>=15000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
          
       case  13:
          //Serial.println(" 25 s");
          if (Ch1Delta>=25000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 14:
          //Serial.println(" 40 s");
          if (Ch1Delta>=40000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
       case 15:
          //Serial.println(" 60 s");
          if (Ch1Delta>=60000) 
          {
           if   (digitalRead(Fr)==HIGH) digitalWrite(IndCh1, LOW);
          }
          else digitalWrite(IndCh1, HIGH);
          break;
 
          
       default:
       Serial.println("x");
          // код для выполнения
      }
}

  
// the setup function runs once when you press reset or power the board
void setup() {
attachInterrupt(0,isr,RISING);  //attaching the interrupt
    //Serial.begin(9600);

pinMode(IndCh1, OUTPUT);  
pinMode(IndCh2, OUTPUT); 

pinMode(D0, INPUT_PULLUP);//подтянуто к 1
pinMode(D1, INPUT_PULLUP);
pinMode(D2, INPUT_PULLUP);
pinMode(D3, INPUT_PULLUP);

pinMode(Fr, INPUT_PULLUP);
pinMode(SaveFr, INPUT_PULLUP);



  //поморгаем для начала  
  digitalWrite(IndCh1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(IndCh1, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
  digitalWrite(IndCh2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(IndCh2, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second

  SfreqCounter = readIntFromEEPROM(0);
  
  //Serial.print(" saved freqensy=");
 //Serial.println(SfreqCounter); 
}

// the loop function runs over and over again forever
void loop() {

  

  
//----------------------------------  
//СЧИТЫВАНИЕ ТЕТРАДЫ  Ch1DelStrt;  
//digitalWrite(IndCh1, HIGH); 

pinMode(Ch1StpOn, INPUT);
pinMode(Ch1StrtOn, OUTPUT);
digitalWrite(Ch1StrtOn, LOW);
bitWrite(Ch1Del, 3, digitalRead(D3));
bitWrite(Ch1Del, 2, digitalRead(D2));
bitWrite(Ch1Del, 1, digitalRead(D1));
bitWrite(Ch1Del, 0, digitalRead(D0));


//СЧИТЫВАНИЕ ТЕТРАДЫ  Ch1DelStp;  
pinMode(Ch1StrtOn, INPUT);
pinMode(Ch1StpOn, OUTPUT);
digitalWrite(Ch1StpOn, LOW);
bitWrite(Ch1Del, 7, digitalRead(D3));
bitWrite(Ch1Del, 6, digitalRead(D2));
bitWrite(Ch1Del, 5, digitalRead(D1));
bitWrite(Ch1Del, 4, digitalRead(D0));

pinMode(Ch1StrtOn, INPUT);//ОТКЛЮЧАЕМ СКАНИРОВАНИЕ 
pinMode(Ch1StpOn, INPUT);

Ch1Del= ~Ch1Del;



Ch1DelStrt=Ch1Del & 0x0F;
Ch1DelStp=(Ch1Del & 0xF0)>>4;//старший полубайт





if (analogRead(Ch1SignalPin)>=analogRead(Ch1TrashPin) ) // or  ( frequency<=SfreqCounter+155 and frequency>=SfreqCounter-155)    )//проверяем 1 раз - если сигнал есть проверим еще DelOnOff раз
  {
  //Serial.println("     Ch1SignalPin > trash or  frequency = saved freq");   
    Stop=false;
    if (Start==false)   //если аудиотриггер еще не запущен
    {
      Ch1Time1=millis();
      Ch1Time2=0;
      Start=true;
    }
    else                  //если аудиотриггер уже работает
    {
      
      Ch1Time2=millis();
      Ch1Delta=Ch1Time2-Ch1Time1;//разница времени 
      
      //сравниваем состояниек переключателя с задержой
      ////Serial.print (" задержка включения аудиотриггера"); 


      DelOn();

     
    }
  
  } 
  else //сигнал меньше заданного уровня
  {
    //Serial.println("     Ch1SignalPin < trash or  frequency != saved freq");  
    if (digitalRead( IndCh1 ) == HIGH)//если сигнала нет но аудиотриггер установлен в 1 делаем задержку выключения аудиотриггера в ссответствии с переключателем
    {
      ////Serial.println(" IndCh1=HIGH");
      
          if (Stop==false)   //если аудиотриггер еще не запущен
            {
              ////Serial.println(" Stop=false");
              
              Ch1Time3=millis();
              Ch1Time4=0;
              Stop=true;
            }
             else                  //если аудиотриггер уже работает
             {
              ////Serial.println(" Stop=true");
              Ch1Time4=millis();
              Ch1Delta=Ch1Time4-Ch1Time3;//разница времени 




      //Serial.print (" задержка выключения аудиотриггера");  

         
      DelOff();   
 


      
             }
      //Serial.println();

    }
    else
    {
     //Serial.print(" No signal ");
     //Serial.print(" IndCh1=0 ");
     //Serial.println(" / ");
     digitalWrite(IndCh1, LOW);
     Ch1Time1=0;
     Ch1Time2=0;
     Ch1Time3=0;
     Ch1Time4=0;     
     Start=false;
     Stop=false;
 
     }
  }



  
}
