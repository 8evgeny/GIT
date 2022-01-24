/*
 * audio_generators.hpp
 *
 *  Created on: Sep 4, 2020
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_UTILS_AUDIO_GENERATORS_HPP_
#define USER_UTILS_AUDIO_GENERATORS_HPP_

#include <ipcThread.h>
#include <audio.hpp>

class sineAudioGenerator :public ipcThread {
public:
    sineAudioGenerator();
//    sineGenerator(basicPipe* pipe);
    int setPipe(basicPipe* pipe);
    void setFrequency(uint32_t Herz);
    void reset();



    void taskRunFcn();

protected:
    void generate();



    int16_t g_function();

    audio_frame_t *buffer=nullptr;
    basicPipe *o_pipe=nullptr;
    uint32_t frequency=1000;
    uint64_t timestamp=0;
    int num_frames=0;
    double period=1/1000;
    uint16_t gain=1000;
};



#endif /* USER_UTILS_AUDIO_GENERATORS_HPP_ */
