#include "main.h"

void setImpedanceRele(bool set) { if(set) gpio_set(GPIOA, GPIO8);  else gpio_clear(GPIOA, GPIO8);}
void setReleTr1(bool set) { if(set) gpio_set(GPIOA, GPIO9);  else gpio_clear(GPIOA, GPIO9);}
void setReleTr2(bool set) { if(set) gpio_set(GPIOA, GPIO10);  else gpio_clear(GPIOA, GPIO10);}
void setReleTr3(bool set) { if(set) gpio_set(GPIOA, GPIO11);  else gpio_clear(GPIOA, GPIO11);}
void setReleTr4(bool set) { if(set) gpio_set(GPIOA, GPIO12);  else gpio_clear(GPIOA, GPIO12);}

void setReadyLed(bool set) { if(set) gpio_set(GPIOB, GPIO8);  else gpio_clear(GPIOB, GPIO8);}
void setUpr1(bool set) { if(set) gpio_set(GPIOB, GPIO1);  else gpio_clear(GPIOB, GPIO1);}
void setUpr2(bool set) { if(set) gpio_set(GPIOB, GPIO2);  else gpio_clear(GPIOB, GPIO2);}

void setLedShortOut(bool set) { if(set) gpio_set(GPIOB, GPIO10);  else gpio_clear(GPIOB, GPIO10);}
void setLedBreakOut(bool set) { if(set) gpio_set(GPIOB, GPIO11);  else gpio_clear(GPIOB, GPIO11);}
void setLedOvercutOut(bool set) { if(set) gpio_set(GPIOB, GPIO12);  else gpio_clear(GPIOB, GPIO12);}
void setLedOverheatOut(bool set) { if(set) gpio_set(GPIOB, GPIO13);  else gpio_clear(GPIOB, GPIO13);}
void setErrorRele(bool set) { if(set) gpio_set(GPIOB, GPIO14);  else gpio_clear(GPIOB, GPIO14);}
void setReleLine1(bool set) { if(set) gpio_set(GPIOB, GPIO15);  else gpio_clear(GPIOB, GPIO15);}

void setRele24V(bool set) { if(set) gpio_set(GPIOC, GPIO9);  else gpio_clear(GPIOC, GPIO9);}

void setFan(bool set) { if(set) gpio_set(GPIOB, GPIO9);  else gpio_clear(GPIOB, GPIO9);}


void setPowerOut(bool set)
{
    //Передача команды через UART

}

//Свободные пины
void setTestLed1(bool set) { if(set) gpio_set(GPIOC, GPIO15);  else gpio_clear(GPIOC, GPIO15);}
void setTestLed2(bool set) { if(set) gpio_set(GPIOC, GPIO14);  else gpio_clear(GPIOC, GPIO14);}
void setTestLed3(bool set) { if(set) gpio_set(GPIOC, GPIO4);  else gpio_clear(GPIOC, GPIO4);}
void setTestLed4(bool set) { if(set) gpio_set(GPIOC, GPIO5);  else gpio_clear(GPIOC, GPIO5);}
void setTestLed5(bool set) { if(set) gpio_set(GPIOC, GPIO12);  else gpio_clear(GPIOC, GPIO12);}
