#include "main.h"

void setImpedanceRele()  { gpio_set(GPIOA, GPIO8);}
void resetImpedanceRele(){ gpio_clear(GPIOA, GPIO8);}

void setReleTr1()   { gpio_set(GPIOA, GPIO9);}
void resetReleTr1() { gpio_clear(GPIOA, GPIO9);}
void setReleTr2()   { gpio_set(GPIOA, GPIO10);}
void resetReleTr2() { gpio_clear(GPIOA, GPIO10);}
void setReleTr3()   { gpio_set(GPIOA, GPIO11);}
void resetReleTr3() { gpio_clear(GPIOA, GPIO11);}
void setReleTr4()   { gpio_set(GPIOA, GPIO12);}
void resetReleTr4() { gpio_clear(GPIOA, GPIO12);}

void setReadyLed()  { gpio_set(GPIOB, GPIO0);}
void resetReadyLed(){ gpio_clear(GPIOB, GPIO0);}
void setUpr1()      { gpio_set(GPIOB, GPIO1);}
void resetUpr1()    { gpio_clear(GPIOB, GPIO1);}
void setUpr2()      { gpio_set(GPIOB, GPIO2);}
void resetUpr2()    { gpio_clear(GPIOB, GPIO2);}

void setLedShortOut()     { gpio_set(GPIOB, GPIO10);}
void resetLedShortOut()   { gpio_clear(GPIOB, GPIO10);}
void setLedBreakOut()     { gpio_set(GPIOB, GPIO11);}
void resetLedBreakOut()   { gpio_clear(GPIOB, GPIO11);}
void setLedOvercutOut()   { gpio_set(GPIOB, GPIO12);}
void resetLedOvercutOut() { gpio_clear(GPIOB, GPIO12);}
void setLedOverheatOut()  { gpio_set(GPIOB, GPIO13);}
void resetLedOverheatOut(){ gpio_clear(GPIOB, GPIO13);}
