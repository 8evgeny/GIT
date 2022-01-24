#include "speexx.h"


Speex::~Speex()
{
    /*Destroy the decoder state*/
    if (enc_state)
        speex_encoder_destroy(enc_state);
    if (dec_state)
        speex_decoder_destroy(dec_state);
    /*Destroy the bit-stream truct*/
    speex_bits_destroy(&bits);
}

void Speex::initializeEncoder(void)
{
    SpeexMode *mode = nullptr;
    int modeID = -1;
    spx_int32_t rate = 44100;
    int quality = 4, complexity = 1, vbr = 0, enh = 1;
    /* Speex encoding initializations */
    speex_bits_init(&bits);
    enc_state = speex_encoder_init(&speex_wb_mode);
    speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &vbr);
    speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &quality);
    speex_encoder_ctl(enc_state, SPEEX_SET_COMPLEXITY, &complexity);
    speex_encoder_ctl(enc_state, SPEEX_SET_SAMPLING_RATE, &rate);
    int tmp = 8;
    speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &tmp);
    Rec_Data_ptr = nullptr;
    quality = 4;
    complexity = 1;
    vbr = 0;
    enh = 1; /* SPEEX PARAMETERS, MUST REMAINED UNCHANGED */
    Index_Encoding = 0;
    Encoded_Frames = 0;
}

void Speex::initializeDecoder(void)
{
    spx_int32_t rate = 44100;
    int quality = 4, complexity = 1, vbr = 0, enh = 1;
    /* Speex encoding initializations */
    dec_state = speex_decoder_init(&speex_wb_mode);
    speex_encoder_ctl(dec_state, SPEEX_SET_VBR, &vbr);
    speex_encoder_ctl(dec_state, SPEEX_SET_QUALITY, &quality);
    speex_encoder_ctl(dec_state, SPEEX_SET_COMPLEXITY, &complexity);
    speex_encoder_ctl(dec_state, SPEEX_SET_SAMPLING_RATE, &rate);
    int tmp = 1;
    speex_decoder_ctl(dec_state, SPEEX_SET_ENH, &tmp);
    quality = 4;
    complexity = 1;
    vbr = 0;
    enh = 1; /* SPEEX PARAMETERS, MUST REMAINED UNCHANGED */
    Index_Encoding = 0;
    Encoded_Frames = 0;
}

int Speex::encodingVoice(const spx_int16_t *input, char *output, unsigned int size)
{
    uint8_t i, nbBytes;

    //for (i = 0; i < FRAME_SIZE; i++) inpuit[] ^= 0x16000;
    /* Flush all the bits in the struct so we can encode a new frame */
    speex_bits_insert_terminator(&bits);
    /* Encode the frame */
    speex_encode_int(enc_state, (spx_int16_t *)input, &bits);
    /* Copy the bits to an array of char that can be decoded */
    nbBytes = speex_bits_write(&bits, (char *)output, size);
    speex_bits_reset(&bits);
    return nbBytes;
}


int Speex::decodingVoice(const  char *input, spx_int16_t *output, unsigned int size)
{
    uint8_t i, nbBytes;

    //for (i = 0; i < FRAME_SIZE; i++) inpuit[] ^= 0x16000;
    /* Flush all the bits in the struct so we can encode a new frame */
//    speex_bits_insert_terminator(&bits);
    /* Encode the frame */
    speex_bits_read_from(&bits, (char *)input, size);
    nbBytes = speex_decode_int(dec_state, &bits, (spx_int16_t *)output);
    return nbBytes;
}

void Speex::setQuality(int val)
{
    quality = val;
    speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &quality);
}
void Speex::setComplexity(int val)
{
    complexity = val;
    speex_encoder_ctl(enc_state, SPEEX_SET_COMPLEXITY, &complexity);
}
void Speex::setVbr(int val)
{
    vbr = val;
    speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &vbr);
}


