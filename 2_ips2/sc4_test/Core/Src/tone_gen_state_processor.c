/**
  ******************************************************************************
  * @file    tone_gen_state_processor.c
  * @author  dmib@@ETS
  * @brief   This file contains tone generator state processor implemenatation
  ******************************************************************************
  */
#include "string.h"
#include "main.h"
#include "tone_generator.h"
#include "tone_gen_state_processor.h"

#define time HAL_GetTick()

#define WAVE1_DATA			0
#define WAVE1_DATA_LEN		0
#define WAVE2_DATA			0
#define WAVE2_DATA_LEN		0
#define WAVE3_DATA			0
#define WAVE3_DATA_LEN		0

/**
  * @brief  saved signal processor state
  */
sTONE_gen_state old_st;
/**
  * @brief  current signal processor state
  */
sTONE_gen_state cur_st;

void tone_genProcessState(sTONE_gen_state *st);

/**
  * @brief  initialization of tone processor state
  * @param  [inout] st - tone processor state structure
  * @param  [in] sigtype - signal type
  * @param  [in] maxlen - length of signal in ms
  * @retval none
  */
void tone_genInitState(sTONE_gen_state *st, uint8_t sigtype, uint16_t maxlen)
{
	 if (st->prev_st) {
		if (sigtype == SIG_GEN_OFF) {
			if (st->prev_st->sig_state != SIG_GEN_OFF) { // restore old state
				memcpy(st, st->prev_st, sizeof(sTONE_gen_state));
				st->prev_st->sig_type = SIG_GEN_OFF;
				return;
			}
		} else
			memcpy(st->prev_st, st, sizeof(sTONE_gen_state)); // save state
	 }
	 st->sig_type = sigtype;
	 st->sig_state = 0;
	 st->maxlen = maxlen;
	 tone_genProcessState(st);
}

/**
  * @brief  update of tone processor state
  * @param  [inout] st - tone processor state structure
  * @retval none
  */
void tone_genProcessState(sTONE_gen_state *st)
{
  switch (st->sig_type) {
    case SIG_GEN_OFF: 
       break;			
    case SIG_GEN_TCH600:
			 switch (st->sig_state) {
				 case 0:
					st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K600][0];
					Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K600][1], st->param.tone_sig.sigX);
				    st->sig_pause=0;
				    st->sig_time = time;
					st->sig_state=1;
				    break;
				 case 1: 
					if ((time-st->sig_time)>st->maxlen) { 
						tone_genInitState(st, SIG_GEN_OFF, 0);
					}
				    break;
         default:;					 
       }
       break;			
    case SIG_GEN_TCH1000:
			 switch (st->sig_state) {
				 case 0:
					  st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K1000][0];
					  Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K1000][1], st->param.tone_sig.sigX);
				      st->sig_pause=0;
				      st->sig_time = time;
					  st->sig_state=1;
				    break;
				 case 1:
					 if ((time-st->sig_time)>st->maxlen) {
						 tone_genInitState(st, SIG_GEN_OFF, 0);
					 }
					break;
		         default:;
		       }
		       break;
    case SIG_GEN_TCH2100:
			 switch (st->sig_state) {
				 case 0:
					st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K2100][0];
					Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K2100][1], st->param.tone_sig.sigX);
				    st->sig_pause=0;
				    st->sig_time = time;
					st->sig_state=1;
				    break;
				 case 1: 
					if ((time-st->sig_time)>st->maxlen) { 
						 tone_genInitState(st, SIG_GEN_OFF, 0);
					}
				    break;
         default:;					 
       }
       break;			
    case SIG_GEN_STATION:  
			 switch (st->sig_state) {
				 case 0:
					  st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K425][0];
				    Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K425][1] / 8, st->param.tone_sig.sigX);
				    st->sig_time = time;
				    st->sig_pause=0;
					  st->sig_state=1;
				    break;
				 case 1:if ((time-st->sig_time)>st->maxlen) {  // max 30sec
						 tone_genInitState(st, SIG_GEN_OFF, 0);
                    };
         default:;					 
       }
       break;			
	case SIG_GEN_CALL:
		 switch (st->sig_state) {
			 case 0:
				st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K425][0];
			    Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K425][1] / 8, st->param.tone_sig.sigX);
			    st->sig_pause=0;
			    st->sig_time = time;
				st->sig_state=1;
			    break;
			 case 1:
				if ((time-st->sig_time)>100) {
			     st->sig_pause=1;
			     st->sig_time = time;
				 st->sig_state=2;
				}
			    break;
			 case 2:
				if ((time-st->sig_time)>100) {
			     st->sig_pause=0;
			     st->sig_time = time;
				 st->sig_state=3;
				}
			 case 3:
				if ((time-st->sig_time)>100) {
			     st->sig_pause=1;
			     st->sig_time = time;
				 st->sig_state=4;
				}
			    break;
			 case 4:
				if ((time-st->sig_time)>100) {
			     st->sig_pause=0;
			     st->sig_time = time;
				 st->sig_state=5;
				}
			 break;
			 case 5:
				if ((time-st->sig_time)>100) {
			     st->sig_pause=1;
			     st->sig_time = time;
				 st->sig_state=6;
				}
			    break;
			 case 6:
				if ((time-st->sig_time)>3000) {
			     st->sig_pause=0;
			     st->sig_time = time;
				 st->sig_state=0;
				}
			   break;
		 }
		 break;
    case SIG_GEN_SENDCTRL: 
			 switch (st->sig_state) {
				 case 0:
					  st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K425][0];
				    Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K425][1] / 8, st->param.tone_sig.sigX);
				    st->sig_pause=0;
				    st->sig_time = time;
					  st->sig_state=1;
				    break;
				 case 1: 
	          if ((time-st->sig_time)>1000) { 
				     st->sig_pause=1;
				     st->sig_time = time;
					   st->sig_state=2;
            }
				    break;
				 case 2: 
	          if ((time-st->sig_time)>4000) { 
				     st->sig_pause=0;
				     st->sig_time = time;
					   st->sig_state=0;
            }
				    break;
         default:;					 
       }
       break;			
    case SIG_GEN_BUSY:     
			 switch (st->sig_state) {
				 case 0:
					st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K425][0];
				    Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K425][1] / 8, st->param.tone_sig.sigX);
				    st->sig_pause = 0;
				    st->sig_time = time;
					st->sig_state = 1;
				    break;
				 case 1: 
					if ((time-st->sig_time)>300) { 
						st->sig_pause = 1;
						st->sig_time = time;
						st->sig_state = 2;
					}
				    break;
				 case 2: 
					if ((time-st->sig_time)>300) { 
						if (st->maxlen<600) {
							tone_genInitState(st, SIG_GEN_OFF, 0);
						} else {
							st->maxlen -= 600;
							st->sig_pause = 0;
							st->sig_time = time;
							st->sig_state = 1;
						}
					}
				    break;
         default:;					 
       }
       break;			
    case SIG_GEN_UNAVAIL:
			 switch (st->sig_state) {
				 case 0:
					  st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K425][0];
				    Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K425][1] / 8, st->param.tone_sig.sigX);
				    st->sig_pause=0;
				    st->sig_time = time;
					  st->sig_state=1;
				    break;
				 case 1: 
					if ((time-st->sig_time)>330) { 
						st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K1000][0];
						Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K1000][1] / 8, st->param.tone_sig.sigX);
						st->sig_time = time;
						st->sig_state=2;
					}
				    break;
				 case 2: 
					if ((time-st->sig_time)>330) { 
						st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K1200][0];
						Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K1200][1] / 8, st->param.tone_sig.sigX);
						st->sig_time = time;
						st->sig_state=3;
					}
				 case 3: 
					if ((time-st->sig_time)>330) { 
						tone_genInitState(st, SIG_GEN_OFF, 0);
					}
				    break;
         default:;					 
       }
       break;			
    case SIG_GEN_OPVACK: 
			 switch (st->sig_state) {
				 case 0:
					st->param.tone_sig.sigK=gen_freq_coef[FREQ_GEN_K425][0];
					Freq_GenInit(st->param.tone_sig.sigK, gen_freq_coef[FREQ_GEN_K425][1] / 8, st->param.tone_sig.sigX);
					st->sig_pause=0;
				    st->sig_time = time;
					st->sig_state=1;
				    break;
				 case 1: 
					if ((time-st->sig_time)>200) { 
						st->sig_pause=1;
						st->sig_time = time;
						st->sig_state=2;
					}
				    break;
				 case 2: 
					if ((time-st->sig_time)>5000) { 
						st->sig_pause=0;
						st->sig_time = time;
						st->sig_state=0;
					}
				    break;
				default:;					 
			}
			 break;
	case SIG_GEN_WAVE1:
			 switch (st->sig_state) {
				 case 0:
				      st->sig_pause=0;
				      st->sig_time = time;
					  st->param.wave_sig.wave_file = WAVE1_DATA;
					  st->param.wave_sig.wave_smp_num = WAVE1_DATA_LEN;
					  st->param.wave_sig.wave_idx = 0;
					  st->sig_state=1;
				    break;
				 case 1: 
	                if ((time-st->sig_time)>st->maxlen) {
						tone_genInitState(st, SIG_GEN_OFF, 0);
                    }
				    break;
			 }
			 break;
	case SIG_GEN_WAVE2:
			 switch (st->sig_state) {
				 case 0:
				      st->sig_pause=0;
				      st->sig_time = time;
					  st->param.wave_sig.wave_file = WAVE2_DATA;
					  st->param.wave_sig.wave_smp_num = WAVE2_DATA_LEN;
					  st->param.wave_sig.wave_idx = 0;
					  st->sig_state=1;
				    break;
				 case 1: 
	                if ((time-st->sig_time)>st->maxlen) {
						tone_genInitState(st, SIG_GEN_OFF, 0);
                    }
				    break;
			 }
			 break;
	case SIG_GEN_WAVE3:
			 switch (st->sig_state) {
				 case 0:
				      st->sig_pause=0;
				      st->sig_time = time;
					  st->param.wave_sig.wave_file = WAVE3_DATA;
					  st->param.wave_sig.wave_smp_num = WAVE3_DATA_LEN;
					  st->param.wave_sig.wave_idx = 0;
					  st->sig_state=1;
				    break;
				 case 1: 
	               if ((time-st->sig_time)>st->maxlen) {
						tone_genInitState(st, SIG_GEN_OFF, 0);
                   }
				    break;
			 }
			 break;
		default:; 
  }
}

/**
  * @brief  get signal samples
  * @param  [in] st - tone processor state structure
  * @param  [inout] buf - buffer for samples
  * @param  [in] len - number of samples to generate
  * @retval result - number of generated samples (0 - if signal ended)
  */
uint16_t tone_genGetSamples(sTONE_gen_state *st, int16_t *buf, uint16_t len)
{
  uint16_t i, wav_len;
  
  switch (st->sig_type) {
    case SIG_GEN_OFF: 
	   return 0;
    case SIG_GEN_TCH600:
    case SIG_GEN_TCH1000:
    case SIG_GEN_TCH2100:
    case SIG_GEN_STATION:  
    case SIG_GEN_SENDCTRL: 
    case SIG_GEN_BUSY:     
    case SIG_GEN_UNAVAIL:
    case SIG_GEN_OPVACK: 
    case SIG_GEN_CALL:
	    if (st->sig_pause) 
		 for (i=0;i<len;i++) buf[i] = 0;
		else
		 for (i=0;i<len;i++) buf[i] = Freq_GenSample(st->param.tone_sig.sigK, st->param.tone_sig.sigX);
		break;
	case SIG_GEN_WAVE1:
	case SIG_GEN_WAVE2:
	case SIG_GEN_WAVE3:
	    if (st->sig_pause) 
		 for (i=0;i<len;i++) buf[i] = 0;
		else {
		 wav_len = st->param.wave_sig.wave_smp_num - st->param.wave_sig.wave_idx;
		 if (wav_len < len)
		 {
		  for (i=0;i<wav_len;i++)
		   buf[i] = st->param.wave_sig.wave_file[st->param.wave_sig.wave_idx++];
		  for (i=wav_len;i<len;i++) buf[i] = 0;
		  st->param.wave_sig.wave_idx = 0;
		 } else {
		  for (i=0;i<len;i++) 
		   buf[i] = st->param.wave_sig.wave_file[st->param.wave_sig.wave_idx++];
		 }		  
		}
		break;	
	 default:;
  }
  
  return len;
}

void tone_genInit()
{
	memset(&old_st, 0, sizeof(sTONE_gen_state));
	memset(&cur_st, 0, sizeof(sTONE_gen_state));
	cur_st.prev_st = &old_st;
}

void tone_genPlayTone(uint8_t sigtype, uint16_t maxlen)
{
	 tone_genInitState(&cur_st, sigtype, maxlen);
}

uint8_t tone_genIsTone()
{
	return (cur_st.sig_type != SIG_GEN_OFF);
}

void tone_genStop()
{
	old_st.sig_type = SIG_GEN_OFF;
	cur_st.sig_type = SIG_GEN_OFF;
}

void tone_genProcess()
{
	tone_genProcessState(&cur_st);
}

uint16_t tone_genGetData(int16_t *buf, uint16_t len)
{
	return tone_genGetSamples(&cur_st, buf, len);
}
