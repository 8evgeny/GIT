#include "main.h"

void setMute(bool set) { if(set) gpio_clear(GPIOA, GPIO7);  else gpio_set(GPIOA, GPIO7);}       //Выключить звук
void setImpedanceRele(bool set) { if(set) gpio_clear(GPIOA, GPIO8);  else gpio_set(GPIOA, GPIO8);}
#ifndef useUSART1
void setReleTr1(bool set) { if(set) gpio_clear(GPIOA, GPIO9);  else gpio_set(GPIOA, GPIO9);}    //LED Трансляция1
void setReleTr2(bool set) { if(set) gpio_clear(GPIOA, GPIO10);  else gpio_set(GPIOA, GPIO10);}  //LED Трансляция2
#endif
void setReleTr3(bool set) { if(set) gpio_clear(GPIOA, GPIO11);  else gpio_set(GPIOA, GPIO11);}  //LED Трансляция3
void setReleTr4(bool set) { if(set) gpio_clear(GPIOA, GPIO12);  else gpio_set(GPIOA, GPIO12);}  //LED Трансляция4

void setReadyLed(bool set) { if(set) gpio_set(GPIOB, GPIO0);  else gpio_clear(GPIOB, GPIO0);}   //LED Готовность
void setUpr1(bool set) { if(set) gpio_clear(GPIOB, GPIO1);  else gpio_set(GPIOB, GPIO1);}       //UPR 078 микширование не задействованы
void setUpr2(bool set) { if(set) gpio_clear(GPIOB, GPIO2);  else gpio_set(GPIOB, GPIO2);}       //UPR ST  микширование не задействованы
void setGAIN(bool set) { if(set) gpio_set(GPIOB, GPIO5);  else gpio_clear(GPIOB, GPIO5);}       //Поднять усиление
void setRESET(bool set) { if(set) gpio_clear(GPIOB, GPIO5);  else gpio_set(GPIOB, GPIO5);}      //Сброс усилителя
void setFan(bool set) { if(set) gpio_clear(GPIOB, GPIO9);  else gpio_set(GPIOB, GPIO9);}        //Вентилятор
//#if (defined useI2C1 && defined  useDisplay)//i2c2 диагностический экранчик на i2c1
void setLedShortOut(bool set) { if(set) gpio_clear(GPIOB, GPIO10);  else gpio_set(GPIOB, GPIO10);}    //КЗ ->
void setLedBreakOut(bool set) { if(set) gpio_clear(GPIOB, GPIO11);  else gpio_set(GPIOB, GPIO11);}    //Обрыв ->
//#endif

void setLedOvercutOut(bool set) { if(set) gpio_clear(GPIOB, GPIO12);  else gpio_set(GPIOB, GPIO12);}  //Перегрузка ->
void setLedOverheatOut(bool set) { if(set) gpio_clear(GPIOB, GPIO13);  else gpio_set(GPIOB, GPIO13);} //Перегрев ->
void setErrorRele(bool set) { if(set) gpio_clear(GPIOB, GPIO14);  else gpio_set(GPIOB, GPIO14);}      //LED Авария + Авария ->
void setReleLine1(bool set) { if(set) gpio_clear(GPIOB, GPIO15);  else gpio_set(GPIOB, GPIO15);}      //LED Линия1 + Вых линии 1 ->
void setReleLine2(bool set) { if(set) gpio_clear(GPIOC, GPIO6);  else gpio_set(GPIOC, GPIO6);}        //LED Линия2
void setReleLine3(bool set) { if(set) gpio_clear(GPIOC, GPIO7);  else gpio_set(GPIOC, GPIO7);}        //LED Линия3
void setReleLine4(bool set) { if(set) gpio_clear(GPIOC, GPIO8);  else gpio_set(GPIOC, GPIO8);}        //LED Линия4

void setRele24V(bool set) { if(set) gpio_clear(GPIOC, GPIO9);  else gpio_set(GPIOC, GPIO9);}          //LED Реле + Выход Реле ->

//Тестовые задачи
void setTestLed1(bool set) { if(set) gpio_clear(GPIOC, GPIO15);  else gpio_set(GPIOC, GPIO15);}
void setTestLed2(bool set) { if(set) gpio_clear(GPIOC, GPIO14);  else gpio_set(GPIOC, GPIO14);}
void setTestLed3(bool set) { if(set) gpio_clear(GPIOC, GPIO5);  else gpio_set(GPIOC, GPIO5);}

