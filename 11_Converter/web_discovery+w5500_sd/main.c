#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include <stdio.h>
#include <string.h>
#include "socket.h"	// Just include one header for WIZCHIP
#include "spi.h"
#include "ff/ff.h"
#include "mmc.h"
#include "spi.h"



////////////////////////////////////////////////
// Shared Buffer Definition for LOOPBACK TEST //
////////////////////////////////////////////////
#define DATA_BUF_SIZE   2048
uint8_t gDATABUF[DATA_BUF_SIZE];

///////////////////////////////////
// Default Network Configuration //
///////////////////////////////////
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {10, 189, 62, 123},
                            .sn = {255,255,255,0},
                            .gw = {10, 189, 62, 1},
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC };


// initialize the dependent host peripheral
void platform_init(void);
void reverse(char s[]);
void itoa(int n, char s[]);


//////////
// TODO //
//////////////////////////////////////////////////////////////////////////////////////////////
// Call back function for W5500 SPI - Theses used as parameter of reg_wizchip_xxx_cbfunc()  //
// Should be implemented by WIZCHIP users because host is dependent                         //
//////////////////////////////////////////////////////////////////////////////////////////////
void  wizchip_select(void);
void  wizchip_deselect(void);
void  wizchip_write(uint8_t wb);
uint8_t wizchip_read();
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
// For example of ioLibrary_BSD //
//////////////////////////////////
void network_init(void);								// Initialize Network information and display it
int32_t tcp_http_mt(uint8_t, uint8_t*, uint16_t);		// Multythread TCP server
void HTTP_reset(uint8_t sockn);
//////////////////////////////////


//states for multythread http
#define HTTP_IDLE 0
#define HTTP_SENDING 1

//variables for multythread http
uint32_t sentsize[_WIZCHIP_SOCK_NUM_];
uint8_t http_state[_WIZCHIP_SOCK_NUM_];
FIL fs[_WIZCHIP_SOCK_NUM_];


int main(void)
{
   uint8_t i;
   uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

   ///////////////////////////////////////////
   // Host dependent peripheral initialized //
   ///////////////////////////////////////////
   platform_init();

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP //
   ////////////////////////////////////////////////////////////////////////////////////////////////////

   /* Chip selection call back */

#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_select);  // CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif


     /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);

    ////////////////////////////////////////////////////////////////////////

    /* WIZCHIP SOCKET Buffer initialize */
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
       //init fail
       while(1);
    }


    /* Network initialization */
    network_init();

    while(!mmc_mount());

    //all connections inactive
    for(i=0;i<_WIZCHIP_SOCK_NUM_;i++)
    HTTP_reset(i);

	/*******************************/
	/* WIZnet W5500 Code Examples  */
	/* HTTP Test    */
	/*******************************/
    /* Main loop */
    while(1)
	{
    	/* Loopback Test */
    	// TCP server
    	for(i=0;i<_WIZCHIP_SOCK_NUM_;i++)
    	tcp_http_mt(i, gDATABUF, 80);
	}

    // end of Main loop
} // end of main()


//////////
// TODO //
/////////////////////////////////////////////////////////////////
// SPI Callback function for accessing WIZCHIP                 //
// WIZCHIP user should implement with your host spi peripheral //
/////////////////////////////////////////////////////////////////
void  wizchip_select(void)
{
	W5500_select();
}

void  wizchip_deselect(void)
{
	W5500_release();
}

void  wizchip_write(uint8_t wb)
{
	W5500_tx(wb);
}

uint8_t wizchip_read()
{
   return W5500_rx();
}
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// Intialize the network information to be used in WIZCHIP //
/////////////////////////////////////////////////////////////
void network_init(void)
{
   uint8_t tmpstr[6];

	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

	ctlwizchip(CW_GET_ID,(void*)tmpstr);
}
/////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////
// HTTP Multythread Example Code using ioLibrary_BSD         //
///////////////////////////////////////////////////////////////
#define len(some) (sizeof(some)/sizeof(some[0]))


const char http_404_full[] =
	"HTTP/1.0 404 Not Found\r\n"
	"Content-Type: text/html;"
	"Server: STM32+W5500\r\n"
	"\r\n"
	"<pre>Page not found\r\n\r\n";


const char  http_200[] = "HTTP/1.0 200 OK\r\n";
const char http_server[] = "Server: STM32+W5500\r\n";
const char http_connection_close[] = "Connection: close\r\n";
const char http_content_type[] = "Content-Type: ";
const char http_content_length[] = "Content-Length: ";
const char  http_linebreak[] = "\r\n";
const char  http_header_end[] = "\r\n\r\n";
const char http_not_found[] = "<h1>404 - Not Found</h1>";

const char http_text_plain[] = "text/plain";
const char http_text_html[] = "text/html";
const char http_text_js[] = "text/javascript";
const char  http_text_css[] = "text/css";
const char http_image_gif[] = "image/gif";
const char http_image_png[] = "image/png";
const char  http_image_jpeg[] = "image/jpeg";
const char  http_video_mp4[] = "video/mp4";
const char  http_video_avi[] = "video/avi";

char default_page[]="index.htm";


// get mime type from filename extension
const char *httpd_get_mime_type(char *url)
{
	const char *t_type;
	char *ext;

     t_type = http_text_plain;

	if((ext = strrchr(url, '.')))
	{
		ext++;
		strlwr(ext);

	    if(strcmp(ext, "txt")==0)
		t_type = http_text_plain;

	    else if(strcmp(ext, "htm")==0)
		t_type = http_text_html;

	    else if(strcmp(ext, "html")==0)
		t_type = http_text_html;

	    else if(strcmp(ext, "js")==0)
		t_type = http_text_js;

	    else if(strcmp(ext, "css")==0)
		t_type = http_text_css;

	    else if(strcmp(ext, "gif")==0)
		t_type = http_image_gif;

	    else if(strcmp(ext, "png")==0)
		t_type = http_image_png;

	    else if(strcmp(ext, "jpg")==0)
		t_type = http_image_jpeg;

	    else if(strcmp(ext, "jpeg")==0)
		t_type = http_image_jpeg;

	    else if(strcmp(ext, "mp4")==0)
		t_type = http_video_mp4;

	    else if(strcmp(ext, "avi")==0)
		t_type = http_video_avi;

	}

	return t_type;
}


//http server

void HTTP_reset(uint8_t sockn)
{
    sentsize[sockn]=0;
	http_state[sockn]=HTTP_IDLE;
}


int32_t tcp_http_mt(uint8_t sn, uint8_t* buf, uint16_t port)
{
   int32_t ret;
   uint32_t size = 0;
   char *url,*p,str[10];
   const char *mime;
   UINT blocklen;
   uint16_t header_sz=0;


   switch(getSn_SR(sn))
   {
      case SOCK_ESTABLISHED :

         if(getSn_IR(sn) & Sn_IR_CON)
         {
            setSn_IR(sn,Sn_IR_CON);
         }

         if((size = getSn_RX_RSR(sn)) > 0)
         {
            if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recv(sn,buf,size);

            HTTP_reset(sn);

            if(ret <= 0)
            return ret;

            url =(char*) buf + 4;

            if((http_state[sn]==HTTP_IDLE)&&(memcmp(buf, "GET ", 4)==0)&&((p = strchr(url, ' '))))// extract URL from request header
            {
              *(p++) = 0;//making zeroed url string

              sentsize[sn]=0;

              if(strcmp(url,"/")==0)
              url=default_page;

               //http data fill
               if(f_open(&fs[sn], url, FA_READ)==0)
        	  {

               mime=httpd_get_mime_type(url);
               strcpy((char*)buf,http_200);

               //from here possibly not mandatory?
               strcat((char*)buf, http_server);
   			   strcat((char*)buf,http_connection_close);

               strcat((char*)buf, http_content_length);
   			   itoa(fs[sn].fsize,str);
   			   strcat((char*)buf,str);
   			   strcat((char*)buf, http_linebreak);
   			  //till here possibly not mandatory?

               strcat((char*)buf, http_content_type);
               strcat((char*)buf,mime);
               strcat((char*)buf, http_header_end);

               header_sz=strlen((char*)buf);

               http_state[sn]=HTTP_SENDING;


        	  }
               else
               {
                //404 - should be less 2048
                strcpy((char*)buf,http_404_full);
                size=strlen((char*)buf);
                ret=send(sn,buf,size);
                if(ret < 0)
                 {
                  close(sn);
                  return ret;
                 }

                //ending
                HTTP_reset(sn);
                disconnect(sn);
               }
            }
         }

           if(http_state[sn]==HTTP_SENDING)
            {
                 //sending answer
                 if(fs[sn].fsize != sentsize[sn])
                {

   			   	  f_read(&fs[sn], &buf[header_sz], DATA_BUF_SIZE-header_sz, &blocklen);

            	   ret = send(sn,buf,blocklen+header_sz);

                   if(ret < 0)
                   {
                    f_close(&fs[sn]);
                    close(sn);
                    return ret;
                   }

                  sentsize[sn] += ret; // Don't care SOCKERR_BUSY, because it is zero.
                 }

                if(sentsize[sn]>=fs[sn].fsize)
               {
                 //ending
                HTTP_reset(sn);
                f_close(&fs[sn]);
                disconnect(sn);
               }
              }
         break;
      case SOCK_CLOSE_WAIT :

    	  HTTP_reset(sn);

         if((ret=disconnect(sn)) != SOCK_OK)
         return ret;

         break;
      case SOCK_INIT :

    	  HTTP_reset(sn);

         if( (ret = listen(sn)) != SOCK_OK) return ret;
         break;
      case SOCK_CLOSED:

    	  HTTP_reset(sn);

         if((ret=socket(sn,Sn_MR_TCP,port,0x00)) != sn)
         return ret;

         break;

      default:
    	  HTTP_reset(sn);
         break;
   }
   return 1;
}



//////////////////////////////////////////////////////////////////////////////
// Platform (STM32F103X) initialization for peripherals as GPIO, SPI, UARTs //
//////////////////////////////////////////////////////////////////////////////
void platform_init(void)
{
	spiW5500_init();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//****************************************************
// reverse:
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

//*******************************************************
// itoa
 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* sign */
         n = -n;          /* make positive n */
     i = 0;
     do {       /* generate reverse */
         s[i++] = n % 10 + '0';   /* next digit */
     } while ((n /= 10) > 0);     /* delete */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

 //*********************************************
