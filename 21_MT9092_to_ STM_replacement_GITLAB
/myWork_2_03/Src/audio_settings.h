/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   audio_settings.h
 * Author: Роман Ганаза
 *
 * Created on 13 августа 2020 г., 10:12
 */

#ifndef AUDIO_SETTINGS_H
#define AUDIO_SETTINGS_H

#define TALK_MODE_VOLUME	100   // громкость громкой связи (режим "Talk") - теперь в процентах

#define HEADSET_BEEP_ATT    5 // раз, ослабление сигналов вызова и отзвука кнопок при подключенной гарнитуре

#define BEEP_VOL_ATT        0.5f // 0..1 - коэффициент ослабления для отзвука настроек праметров

//-------------- все настройки ниже данной черты трогать нельзя-----------------// 

// частота сэмплирования кодека 48 кГц, в линию направляется 24 кГц
// 16 бит в 32-битном кадре (см. инит I2S)
#define SAMPLERATE  24000

 // размер массива с 16-битными элементами: 2 канала (левый/правый), 1 мс
#define AUDIO_IN_OUT_DMA_BUFFER_SIZE 2


// для дополнительного PDM-микрофона
// частота дискретизации входного звука с PDM микрофона - 24 кГц
// кадровая частота I2S для PDM в 2 раза выше для коэф. децимации 64
// битовая частота I2S = 24 кГц * 64 = 1536000 Гц = 1,5 МГц

// выходные PCM сэмплы (после фильтрации/децимации)
#define PCM_AUDIO_IN_BUFFER_SIZE                24// количество сэмплов в 1 мс @ 24 кГц
// в половину этого PDM буфера должна влезать 1 мс коэф. децимации 64
// коэф. децимации - во сколько раз битовая частота PDM выше частоты дискретизации PCM
#define PDM_AUDIO_IN_DMA_BUFFER_SIZE            192 //=PCM_AUDIO_IN_BUFFER_SIZE*8 - слов по 16 бит, после децимации на 64 из 4-х слов получится 1 сэмпл, умножаем на 2, т.к. DMA прерывается по полбуфера
    
#endif /* AUDIO_SETTINGS_H */

