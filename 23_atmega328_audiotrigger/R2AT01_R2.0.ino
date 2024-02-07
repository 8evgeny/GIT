/*
  R2AT01M
  Двухканальный модуль аудиотриггера 
  Сравнивает уровень сигнала на входе с регулируемым опорным напряжением
  При обнаружении настроенного уровня включает оптрон
  Также с помощью 16-ти позиционных переключателей регулируются задержи включения и отключения оптронов
  Микроконтроллер ATMEGA328P
  
  

  Created 11.2024  by VegerukOV R1.0
  Edited  06.02.2024  by BelovVV R2.0
 
  
*/






//Channel 1


#define D0 8              // channel 1 входы от переключателя
#define D1 9
#define D2 10
#define D3 11
#define Ch1SignalPin A0  //сигнал
#define Ch1TrashPin A1   //Опорный уровень с потенциометра
#define IndCh1 A4        //лампочки
#define Ch1StrtOn 12     //сканирование переключателя задержки включения = 0
#define Ch1StpOn 13      //сканирование переключателя задержки отключения = 0



byte Ch1DelStrt;          //задержка включения аудиотриггера  относительно времени появления аудиосигнала, превышающего TRASH
byte Ch1DelStp;           //задержка выключения аудиотриггера  относительно времени  пропадания аудиосигнала, меньше, чем TRASH
byte Ch1Del;              //по каналу 1: задержка выключения-выключения аудиотриггера YYYYXXXX. хххх - задержка включения YYYY - задержка выключения
boolean IsSignal1;        //присутствие аудиосигнала , больше чем TRASH
boolean Start1,  Stop1;   //аудиотриггер запущен

//Channel 2

#define D01 4  // channel 2 входы от переключателя
#define D11 5
#define D21 6
#define D31 7
#define Ch2SignalPin A2
#define Ch2TrashPin A3
#define IndCh2 A5
#define Ch2StrtOn 2        //сканирование переключателя задержки включения = 0
#define Ch2StpOn 3         //сканирование переключателя задержки отключения = 0
uint32_t Ch2Time1, Ch2Time2, Ch2Time3, Ch2Time4, Ch2Delta, Ch1Time1, Ch1Time2,
Ch1Delta, Ch1Time3, Ch1Time4;                                                      //для измерения времени
byte Ch2DelStrt;                             //задержка включения аудиотриггера  относительно времени появления аудиосигнала, превышающего TRASH
byte Ch2DelStp;                              //задержка выключения аудиотриггера  относительно времени  пропадания аудиосигнала, меньше, чем TRASH
byte Ch2Del;                                 //по каналу 2:
boolean IsSignal2;                           //присутствие аудиосигнала , больше чем TRASH
boolean Start2, Stop2;                       //аудиотриггер запущен



int n;    // ????


 // Setup

void setup() {

  //Serial.begin(9600);

  pinMode(IndCh1, OUTPUT);
  pinMode(IndCh2, OUTPUT);

  pinMode(D0, INPUT_PULLUP);  //подтянуто к 1
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);

  pinMode(D01, INPUT_PULLUP);  //подтянуто к 1
  pinMode(D11, INPUT_PULLUP);
  pinMode(D21, INPUT_PULLUP);
  pinMode(D31, INPUT_PULLUP);




  //Индикация при включении попеременно моргаем индикаторами

  digitalWrite(IndCh1, HIGH);  
  delay(500);                  
  digitalWrite(IndCh1, LOW);  
  delay(500);                  
  digitalWrite(IndCh2, HIGH);  
  delay(500);                  
  digitalWrite(IndCh2, LOW);   
  delay(500);                  
}

// Loop

void loop() {


  //----------------------------------
  //СЧИТЫВАНИЕ ТЕТРАДЫ  Ch1DelStrt;


  pinMode(Ch1StpOn, INPUT);
  pinMode(Ch1StrtOn, OUTPUT);
  digitalWrite(Ch1StrtOn, LOW);
  bitWrite(Ch1Del, 3, digitalRead(D3));
  bitWrite(Ch1Del, 2, digitalRead(D2));
  bitWrite(Ch1Del, 1, digitalRead(D1));
  bitWrite(Ch1Del, 0, digitalRead(D0));
  digitalWrite(Ch1StrtOn, HIGH);

  //СЧИТЫВАНИЕ ТЕТРАДЫ  Ch1DelStp;

  pinMode(Ch1StrtOn, INPUT);
  pinMode(Ch1StpOn, OUTPUT);
  digitalWrite(Ch1StpOn, LOW);
  bitWrite(Ch1Del, 7, digitalRead(D3));
  bitWrite(Ch1Del, 6, digitalRead(D2));
  bitWrite(Ch1Del, 5, digitalRead(D1));
  bitWrite(Ch1Del, 4, digitalRead(D0));
  digitalWrite(Ch1StpOn, HIGH);



  pinMode(Ch1StrtOn, INPUT);  //ОТКЛЮЧАЕМ СКАНИРОВАНИЕ
  pinMode(Ch1StpOn, INPUT);

  Ch1Del = ~Ch1Del;
  Ch1DelStrt = Ch1Del & 0x0F;
  Ch1DelStp = (Ch1Del & 0xF0) >> 4;  //старший полубайт




  //----------------------------------
  //СЧИТЫВАНИЕ ТЕТРАДЫ  Ch1DelStrt;

  pinMode(Ch2StpOn, INPUT);
  pinMode(Ch2StrtOn, OUTPUT);
  digitalWrite(Ch2StrtOn, LOW);
  bitWrite(Ch2Del, 3, digitalRead(D31));
  bitWrite(Ch2Del, 2, digitalRead(D21));
  bitWrite(Ch2Del, 1, digitalRead(D11));
  bitWrite(Ch2Del, 0, digitalRead(D01));


  //СЧИТЫВАНИЕ ТЕТРАДЫ  Ch1DelStp;
  pinMode(Ch2StrtOn, INPUT);
  pinMode(Ch2StpOn, OUTPUT);
  digitalWrite(Ch2StpOn, LOW);
  bitWrite(Ch2Del, 7, digitalRead(D31));
  bitWrite(Ch2Del, 6, digitalRead(D21));
  bitWrite(Ch2Del, 5, digitalRead(D11));
  bitWrite(Ch2Del, 4, digitalRead(D01));

  pinMode(Ch2StrtOn, INPUT);  //ОТКЛЮЧАЕМ СКАНИРОВАНИЕ
  pinMode(Ch2StpOn, INPUT);

  Ch2Del = ~Ch2Del;
  Ch2DelStrt = Ch2Del & 0x0F;
  Ch2DelStp = (Ch2Del & 0xF0) >> 4;  //старший полубайт





  if (analogRead(Ch1SignalPin) >= analogRead(Ch1TrashPin))  //проверяем 1 раз - если сигнал есть проверим еще DelOnOff раз
  {
    Stop1 = false;
    if (Start1 == false)                                     //если аудиотриггер еще не запущен
    {
    Ch1Time1 = millis();
    Ch1Time2 = 0;
      Start1 = true;
    } 
    else  //если аудиотриггер уже работает
    {
      Ch1Time2 = millis();
      Ch1Delta = Ch1Time2 - Ch1Time1;                       //разница времени
      switch (Ch1DelStrt)                                   // запуск задержки в соответствии с переключателем
      {
        case 0:
          Serial.println(" 0 ms");
          digitalWrite(IndCh1, HIGH);
          break;
        case 1:
          Serial.println(" 50 ms");
          if (Ch1Delta >= 50) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 2:
          Serial.println(" 100 ms");
          if (Ch1Delta >= 100) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 3:
          Serial.println(" 200 ms");
          if (Ch1Delta >= 200) {
            digitalWrite(IndCh1, HIGH);
          } else {
            digitalWrite(IndCh1, LOW);
          }
          break;
        case 4:
          Serial.println(" 400 ms");
          if (Ch1Delta >= 400) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 5:
          Serial.println(" 800 ms");
          if (Ch1Delta >= 800) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 6:
          Serial.println(" 1.5 s");
          if (Ch1Delta >= 1500) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 7:
          Serial.println(" 2.2 s");
          if (Ch1Delta >= 2200) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 8:
          Serial.println(" 3 s");
          if (Ch1Delta >= 3000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 9:
          Serial.println(" 4 s");
          if (Ch1Delta >= 4000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 10:
          Serial.println(" 5 s");
          if (Ch1Delta >= 5000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 11:
          Serial.println(" 8 s");
          if (Ch1Delta >= 8000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 12:
          Serial.println(" 15 s");
          if (Ch1Delta >= 15000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;

        case 13:
          Serial.println(" 25 s");
          if (Ch1Delta >= 25000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 14:
          Serial.println(" 40 s");
          if (Ch1Delta >= 40000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;
        case 15:
          Serial.println(" 60 s");
          if (Ch1Delta >= 60000) {
            digitalWrite(IndCh1, HIGH);
          } else digitalWrite(IndCh1, LOW);
          break;


        default:
          Serial.println("x");
          // код для выполнения
      }
    }

  } else  //сигнал меньше заданного уровня
  {

    if (digitalRead(IndCh1) == HIGH)  //если сигнала нет но аудиотриггер установлен в 1 делаем задержку выключения аудиотриггера в ссответствии с переключателем
    {
      Serial.println(" IndCh1=HIGH");

      if (Stop1 == false)  //если аудиотриггер еще не запущен
      {
        //              Serial.println(" Stop1=false");

        Ch1Time3 = millis();
        Ch1Time4 = 0;
        Stop1 = true;
      } else  //если аудиотриггер уже работает
      {
        //              Serial.println(" Stop1=true");
        Ch1Time4 = millis();
        Ch1Delta = Ch1Time4 - Ch1Time3;  //разница времени


        //      Serial.print(" Ch1DelStop1=");
        //      Serial.print(Ch1DelStp );
        //      Serial.print(" Ch1Time4=");
        //      Serial.print(Ch1Time4 );
        //      Serial.print(" Ch1Time3=");
        //      Serial.print(Ch1Time3 );
        //      Serial.print(" Ch1Delta=");
        //      Serial.print(Ch1Delta );





        switch (Ch1DelStp)  // запуск задержки выключения в соответствии с переключателем
        {
          case 0:
            Serial.println(" 0 ms");
            digitalWrite(IndCh1, LOW);
            break;
          case 1:
            Serial.println(" 50 ms");
            if (Ch1Delta >= 50) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 2:
            Serial.println(" 100 ms");
            if (Ch1Delta >= 100) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 3:
            Serial.println(" 200 ms");
            if (Ch1Delta >= 200) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 4:
            Serial.println(" 400 ms");
            if (Ch1Delta >= 400) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 5:
            Serial.println(" 800 ms");
            if (Ch1Delta >= 800) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 6:
            Serial.println(" 1.5 s");
            if (Ch1Delta >= 1500) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 7:
            Serial.println(" 2.2 s");
            if (Ch1Delta >= 2200) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 8:
            Serial.println(" 3 s");
            if (Ch1Delta >= 3000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 9:
            Serial.println(" 4 s");
            if (Ch1Delta >= 4000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 10:
            Serial.println(" 5 s");
            if (Ch1Delta >= 5000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 11:
            Serial.println(" 8 s");
            if (Ch1Delta >= 8000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 12:
            Serial.println(" 15 s");
            if (Ch1Delta >= 15000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;

          case 13:
            Serial.println(" 25 s");
            if (Ch1Delta >= 25000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 14:
            Serial.println(" 40 s");
            if (Ch1Delta >= 40000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;
          case 15:
            Serial.println(" 60 s");
            if (Ch1Delta >= 60000) {
              digitalWrite(IndCh1, LOW);
            } else digitalWrite(IndCh1, HIGH);
            break;


          default:
            Serial.println("x");
            // код для выполнения
        }
      }


    } else {
      //     Serial.print(" No signal on ch1 ");
      Serial.print(" IndCh1=0 ");
      Serial.print(" / ");
      digitalWrite(IndCh1, LOW);
      Ch1Time1 = 0;
      Ch1Time2 = 0;
      Ch1Time3 = 0;
      Ch1Time4 = 0;
      Start1 = false;
      Stop1 = false;
    }
  }  // the end of test ch1


  //test ch1
  Serial.print(" Signal 2= ");
  Serial.print(analogRead(Ch2SignalPin));
  Serial.print("/ Trash2= ");
  Serial.print(analogRead(Ch2TrashPin));
  Serial.print(", ch2=");
  Serial.print(Ch2DelStp, HEX);
  Serial.print(Ch2DelStrt, HEX);
  Serial.println();  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


  if (analogRead(Ch2SignalPin) >= analogRead(Ch2TrashPin))  //проверяем 1 раз - если сигнал есть проверим еще DelOnOff раз
  {
    // Serial.println("");
    Stop2 = false;
    if (Start2 == false)  //если аудиотриггер еще не запущен
    {
      Ch2Time1 = millis();
      Ch2Time2 = 0;
      Start2 = true;
      //Serial.println("");
    } else  //если аудиотриггер уже работает
    {
      //      Serial.println("");
      Ch2Time2 = millis();
      Ch2Delta = Ch2Time2 - Ch2Time1;  //разница времени

      switch (Ch2DelStrt)  // запуск задержки в соответствии с переключателем
      {
        case 0:
          Serial.println(" 0 ms");
          digitalWrite(IndCh2, HIGH);
          break;
        case 1:
          Serial.println(" 50 ms");
          if (Ch2Delta >= 50) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 2:
          Serial.println(" 100 ms");
          if (Ch2Delta >= 100) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 3:
          Serial.println(" 200 ms");
          if (Ch2Delta >= 200) {
            digitalWrite(IndCh2, HIGH);
          } else {
            digitalWrite(IndCh2, LOW);
          }
          break;
        case 4:
          Serial.println(" 400 ms");
          if (Ch2Delta >= 400) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 5:
          Serial.println(" 800 ms");
          if (Ch2Delta >= 800) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 6:
          Serial.println(" 1.5 s");
          if (Ch2Delta >= 1500) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 7:
          Serial.println(" 2.2 s");
          if (Ch2Delta >= 2200) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 8:
          Serial.println(" 3 s");
          if (Ch2Delta >= 3000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 9:
          Serial.println(" 4 s");
          if (Ch2Delta >= 4000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 10:
          Serial.println(" 5 s");
          if (Ch2Delta >= 5000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 11:
          Serial.println(" 8 s");
          if (Ch2Delta >= 8000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 12:
          Serial.println(" 15 s");
          if (Ch2Delta >= 15000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;

        case 13:
          Serial.println(" 25 s");
          if (Ch2Delta >= 25000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 14:
          Serial.println(" 40 s");
          if (Ch2Delta >= 40000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;
        case 15:
          Serial.println(" 60 s");
          if (Ch2Delta >= 60000) {
            digitalWrite(IndCh2, HIGH);
          } else digitalWrite(IndCh2, LOW);
          break;


        default:
          Serial.println("x");
          // код для выполнения
          //         Serial.println("");
      }
    }

  } else  //сигнал меньше заданного уровня
  {
    //Serial.println("");
    if (digitalRead(IndCh2) == HIGH)  //если сигнала нет но аудиотриггер установлен в 1 делаем задержку выключения аудиотриггера в ссответствии с переключателем
    {
      //      Serial.println(" IndCh2=HIGH");
      //Serial.println("");
      if (Stop2 == false)  //если аудиотриггер еще не запущен
      {
        //             Serial.println(" Stop2=false");
        //  Serial.println("");
        Ch2Time3 = millis();
        Ch2Time4 = 0;
        Stop2 = true;
      } else  //если аудиотриггер уже работает
      {
        //        Serial.println("");
        //              Serial.println(" Stop2=true");
        Ch2Time4 = millis();
        Ch2Delta = Ch2Time4 - Ch2Time3;  //разница времени


        //      Serial.print(" Ch2DelStop2=");
        //      Serial.print(Ch2DelStp );
        //      Serial.print(" Ch2Time4=");
        //      Serial.print(Ch2Time4 );
        //      Serial.print(" Ch2Time3=");
        //      Serial.print(Ch2Time3 );
        //      Serial.print(" Ch2Delta=");
        //      Serial.print(Ch2Delta );





        switch (Ch2DelStp)  // запуск задержки выключения в соответствии с переключателем
        {
          case 0:
            Serial.println(" 0 ms");
            digitalWrite(IndCh2, LOW);
            break;
          case 1:
            Serial.println(" 50 ms");
            if (Ch2Delta >= 50) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 2:
            Serial.println(" 100 ms");
            if (Ch2Delta >= 100) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 3:
            Serial.println(" 200 ms");
            if (Ch2Delta >= 200) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 4:
            Serial.println(" 400 ms");
            if (Ch2Delta >= 400) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 5:
            Serial.println(" 800 ms");
            if (Ch2Delta >= 800) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 6:
            Serial.println(" 1.5 s");
            if (Ch2Delta >= 1500) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 7:
            Serial.println(" 2.2 s");
            if (Ch2Delta >= 2200) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 8:
            Serial.println(" 3 s");
            if (Ch2Delta >= 3000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 9:
            Serial.println(" 4 s");
            if (Ch2Delta >= 4000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 10:
            Serial.println(" 5 s");
            if (Ch2Delta >= 5000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 11:
            Serial.println(" 8 s");
            if (Ch2Delta >= 8000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 12:
            Serial.println(" 15 s");
            if (Ch2Delta >= 15000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;

          case 13:
            Serial.println(" 25 s");
            if (Ch2Delta >= 25000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 14:
            Serial.println(" 40 s");
            if (Ch2Delta >= 40000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;
          case 15:
            Serial.println(" 60 s");
            if (Ch2Delta >= 60000) {
              digitalWrite(IndCh2, LOW);
            } else digitalWrite(IndCh2, HIGH);
            break;


          default:
            Serial.println("x");
            // код для выполнения
            //  Serial.println("");
        }


        //    Serial.println("");
      }


    } else {
      //     Serial.print(" No signal on ch2 ");
      Serial.print(" IndCh2=0 ");
      Serial.println(" / ");
      digitalWrite(IndCh2, LOW);
      Ch2Time1 = 0;
      Ch2Time2 = 0;
      Ch2Time3 = 0;
      Ch2Time4 = 0;
      Start2 = false;
      Stop2 = false;
    }
    //  Serial.println("");
  }  // the end of test ch2
}
