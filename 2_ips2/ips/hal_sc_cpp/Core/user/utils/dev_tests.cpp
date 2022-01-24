/*
 * dev_tests.cpp
 *
 *  Created on: 5 сент. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#include <dev_tests.hpp>

#include <ipcThread.h>
#include <ips_globals.hpp>
#include <new>
#include <ipssystem.hpp>

#include <audio.hpp>
#include <audio_generators.hpp>

#include <ipsUdp.h>

extern testUDPAudio *test_udp_audio_ptr;
//static uint8_t recv_buffer[2*1024];
//static testUDPAudio reciever("192.168.0.172",1488,&recv_buffer[0]);
//test_udp_audio_ptr=&reciever;

void test_sine_audio()
    {
	audioDevice& a_output_i=audioDevice::getInstance();
	//audioDevice* audio_ptr=a_output_i.getPtr();
	sineAudioGenerator *generator=(sineAudioGenerator*)pvPortMalloc(sizeof(sineAudioGenerator));
        generator=(sineAudioGenerator*)new (generator)sineAudioGenerator();
        //generator->sineGenerator();
        //sineAudioGenerator* generator=hinited;
        //sineAudioGenerator* generator=&generator_i;
        ipsSystem::debug->printfl("Create generator\n");
        generator->setPipe(a_output_i.ADCPointer());
        a_output_i.Play();
        for (int i=100;i<3500;i++){
    	generator->setFrequency(i);
    	osDelay(4);
        }
        for (int i=3500; i>49;i--){
    	generator->setFrequency(i);
    	osDelay(4);
        }
        generator->setFrequency(440);
        osDelay(1000);
        a_output_i.Stop();
        generator->~sineAudioGenerator();
    }

void test_udp_player(){
    audioDevice& a_output_i=audioDevice::getInstance();
//    uint8_t* recv_buffer=(uint8_t*)system_cpp.getBuffer();
//    testUDPAudio *reciever=(testUDPAudio*)pvPortMalloc(sizeof(testUDPAudio));
//    reciever=(testUDPAudio*)new(reciever)testUDPAudio("192.168.0.172",1488,recv_buffer);

    static uint8_t recv_buffer[2*1024];
    static testUDPAudio reciever("172.17.10.71",1983,&recv_buffer[0]);
    test_udp_audio_ptr=&reciever;
    test_udp_audio_ptr->setPipe(a_output_i.ADCPointer());
    a_output_i.Play();
    while(1)osDelay(10);
}
