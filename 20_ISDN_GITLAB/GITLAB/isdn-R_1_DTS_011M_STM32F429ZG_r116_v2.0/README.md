# **ISDN**



## В данном проекте собрана информация по расшифровке телеграмм

В директории [/Doc](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/tree/main/Doc?ref_type=heads) в наличии следующие файлы:

    Изначальные ( проработанные ещё Лисуновым)

        - Кодировка Trace-телеграмм системы Intron-D 23.09.08.doc
        - Дополнения к Кодировка Trace-телеграмм системы Intron-D от 09.06.2011 .doc

    Дополнения от инженера Краснова (Роман Ганаза)

        - Дополнение к Кодировке Trace-телеграмм системы Intron-D от 26.07.2023.docx

***В качестве итога разработана прошивка для контроллера платы R 1 DTS 011***

    Версии прошивок разложены по веткам.
    Каждой версии прошивки соотвектсвует отдельная веткка от main
    Начиная с версии 2.03 есть исходники
    Исходники лежат в архиве SRC

## Версии прошивок:

### **Первая версия**

[R_1_DTS_011M_STM32F429ZG_r116_v2.0.hex](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/tree/R_1_DTS_011M_STM32F429ZG_r116_v2.0?ref_type=heads) 

### **2.01**
            
[R_1_DTS_011M_STM32F429ZG_r119_v2.01.hex](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/tree/R_1_DTS_011M_STM32F429ZG_r119_v2.01?ref_type=heads)

### **2.03 c исходниками**

[R_1_DTS_011M_STM32F429ZG_r122_v2.03.hex](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/tree/R_1_DTS_011M_STM32F429ZG_r122_v2.03?ref_type=heads)

### **2.04 последняя версия**

[R_1_DTS_011M_STM32F429ZG_r123_v2.04.hex](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/tree/R_1_DTS_011M_STM32F429ZG_r123_v2.04?ref_type=heads)


## Сборка из исходников

    Инструкция в файле 
[!!!_Сборка_ПО_LME_MK.docx](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/blob/main/____%D0%A1%D0%B1%D0%BE%D1%80%D0%BA%D0%B0_%D0%9F%D0%9E_LME_MK.docx?ref_type=heads)
    Делалась для LME, для DTS процедура не отличается

## Прошивка

    Инструкция в файле
[Прошивка_DTS011_Custom_UART_R01.docx](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/blob/main/%D0%9F%D1%80%D0%BE%D1%88%D0%B8%D0%B2%D0%BA%D0%B0_DTS011_Custom_UART_R01.docx?ref_type=heads)

    Также возможна прошивка через STM32 JTAG  

     Инструкция в файле 
   [Прошивка_DTS011_Custom_JTAG_R01.docx](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/blob/main/%D0%9F%D1%80%D0%BE%D1%88%D0%B8%D0%B2%D0%BA%D0%B0_DTS011_Custom_JTAG_R01.docx?ref_type=heads)

## Результаты тестирования последней версии прошивки в файле [ПИ DTS от Краснова.docx](http://gitlab.git-holding.ru:9071/git-comm/isdn/-/blob/main/%D0%9F%D0%98_DTS_%D0%BE%D1%82_%D0%9A%D1%80%D0%B0%D1%81%D0%BD%D0%BE%D0%B2%D0%B0.docx?ref_type=heads)
       
