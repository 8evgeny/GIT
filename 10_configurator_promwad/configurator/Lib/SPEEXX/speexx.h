#ifndef _SPEEX_AUDIO_FILTER_HH
#define _SPEEX_AUDIO_FILTER_HH

#ifdef HAVE_CONFIG_H
#include "config.h"

#endif
#include <speex/speex.h>
#include <cstdint>

#define FRAME_SIZE 1400 //*0.0625ms = 20ms
#define ENCODED_FRAME_SIZE 70 //8
#define MAX_REC_FRAMES 90 // = MAX_REC_FRAMES*0,02сек

class Speex
{
public:
    /*!
     \brief Base destructor.

     \fn ~Speex
    */
    ~Speex();

    /*!
     \brief Initialization for Speex.

     \fn initialize
    */
    void initializeEncoder();
    void initializeDecoder();
    /*!
     \brief Voice encoding.

     \fn encodingVoice
    */
    int encodingVoice(const spx_int16_t *input, char *output, unsigned int size);
    int decodingVoice(const char *input, spx_int16_t *output, unsigned int size);
    /*!
     \brief Set Quality.
     \param val Quality
     \fn setQuality
    */
    void setQuality(int val);

    /*!
     \brief Set Complexity.
     \param val Complexity
     \fn setComplexity
    */
    void setComplexity(int val);

    /*!
     \brief Set Vbr.
     \param val Vbr
     \fn setVbr
    */
    void setVbr(int val);
private:
    uint8_t Start_Encoding;
    uint8_t *Rec_Data_ptr;
    void *enc_state;
    void *dec_state;/* Holds the states of the encoder & the decoder */
    SpeexBits bits; /* Holds bits so they can be read and written by the Speex routines */
    int quality;
    int complexity;
    int vbr;
    int enh;
    uint8_t Index_Encoding;
    uint32_t Encoded_Frames ;
};
#endif
