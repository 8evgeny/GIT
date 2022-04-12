#include "main.h"

void setOutputs(void *args)
{
    (void)args;

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(200));

        while (!isBtnIMPEDANSE) //При нажатии  кн Импеданс - моргает
        {
            setErrorRele(true);
            vTaskDelay(pdMS_TO_TICKS(500));
            setErrorRele(false);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        while (!isBtnCALIBROVKA) //При нажатии  кн - моргает
        {
            setErrorRele(true);
            vTaskDelay(pdMS_TO_TICKS(200));
            setErrorRele(false);
            vTaskDelay(pdMS_TO_TICKS(200));
        }








//        setLedOverheatOut(1);
//        setLedOvercutOut(1);
//        setLedShortOut(1);
//        setLedBreakOut(1);
//        setErrorRele(1);

//        setReleLine1(1);
//        setRele24V(1);
//        setReleLine1(1);
//        setReleLine2(1);
//        setReleLine3(1);
//        setReleLine4(1);
//        setReleTr1(1);
//        setReleTr2(1);
//        setReleTr3(1);
//        setReleTr4(1);
//        setReadyLed(1);
//        setFan(1);

//        vTaskDelay(pdMS_TO_TICKS(2000));
//        setLedOverheatOut(0);
//        setLedOvercutOut(0);
//        setLedShortOut(0);
//        setLedBreakOut(0);
//        setErrorRele(0);

//        setReleLine1(0);
//        setRele24V(0);
//        setReleLine2(0);
//        setReleLine3(0);
//        setReleLine4(0);
//        setReleTr1(0);
//        setReleTr2(0);
//        setReleTr3(0);
//        setReleTr4(0);
//        setReadyLed(0);
//        setFan(0);
//        vTaskDelay(pdMS_TO_TICKS(2000));


    }

}
