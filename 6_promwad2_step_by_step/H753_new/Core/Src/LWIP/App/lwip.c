#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "rs232_printf.h"

/* USER CODE BEGIN 0 */

extern ETH_HandleTypeDef heth;
/* Semaphore to signal Ethernet Link state update */
osSemaphoreId Netif_LinkSemaphore = NULL;
/* Ethernet link thread Argument */
struct link_str link_arg;

/**
 * Convert human readable IPv4 address to UINT32
 * @param pDottedQuad   Input C string e.g. "192.168.0.1"
 * @param pIpAddr       Output IP address
 * return 1 on success, else 0
 */
static int ipStringToNumber(const char *pDottedQuad, uint8_t  *pIpAddr)
{
    unsigned int            byte3;
    unsigned int            byte2;
    unsigned int            byte1;
    unsigned int            byte0;
    char              dummyString[2];

    /* The dummy string with specifier %1s searches for a non-whitespace char
    * after the last number. If it is found, the result of sscanf will be 5
    * instead of 4, indicating an erroneous format of the ip-address.
    */
    if (sscanf(pDottedQuad, "%u.%u.%u.%u%1s",
               &byte3, &byte2, &byte1, &byte0, dummyString) == 4) {
        if ((byte3 < 256)
            && (byte2 < 256)
            && (byte1 < 256)
            && (byte0 < 256)
            ) {
            pIpAddr[0]  = (uint8_t)byte3;
            pIpAddr[1]  = (uint8_t)byte2;
            pIpAddr[2]  = (uint8_t)byte1;
            pIpAddr[3]  = (uint8_t)byte0;

            return 1;
        }
    }

    return 0;
}


/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
static void ethernet_link_status_updated(struct netif *netif);
/* ETH Variables initialization ----------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
int32_t MX_LWIP_Init(const char *ipSet, const char *netmaskSet, const char *gatwaySet)
{

    if (ipSet == NULL)
        return -1;

    if (netmaskSet == NULL)
        return -1;

    if (gatwaySet == NULL)
        return -1;

    /* IP addresses initialization */
    ipStringToNumber(ipSet, IP_ADDRESS);
    ipStringToNumber(netmaskSet, NETMASK_ADDRESS);
    ipStringToNumber(gatwaySet, GATEWAY_ADDRESS);

//  /* IP addresses initialization */
//  IP_ADDRESS[0] = 192;
//  IP_ADDRESS[1] = 168;
//  IP_ADDRESS[2] = 100;
//  IP_ADDRESS[3] = 108;
//  NETMASK_ADDRESS[0] = 255;
//  NETMASK_ADDRESS[1] = 255;
//  NETMASK_ADDRESS[2] = 255;
//  NETMASK_ADDRESS[3] = 0;
//  GATEWAY_ADDRESS[0] = 0;
//  GATEWAY_ADDRESS[1] = 0;
//  GATEWAY_ADDRESS[2] = 0;
//  GATEWAY_ADDRESS[3] = 0;

/* USER CODE BEGIN IP_ADDRESSES */
/* USER CODE END IP_ADDRESSES */

  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {

RS232Puts("--netif_link_up--\r\n");

    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {

RS232Puts("--netif_link_down--\r\n");

    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);

  /* Create the Ethernet link handler thread */
/* USER CODE BEGIN H7_OS_THREAD_DEF_CREATE_CMSIS_RTOS_V1 */
  osThreadDef(EthLink, ethernet_link_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate (osThread(EthLink), &gnetif);
/* USER CODE END H7_OS_THREAD_DEF_CREATE_CMSIS_RTOS_V1 */

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */
}

/**
  * @brief  Notify the User about the network interface config status
  * @param  netif: the network interface
  * @retval None
  */
static void ethernet_link_status_updated(struct netif *netif)
{
  if (netif_is_up(netif))
  {
/* USER CODE BEGIN 5 */

RS232Puts("--netif_link_status_up--\r\n");

/* USER CODE END 5 */
  }
  else /* netif is down */
  {
/* USER CODE BEGIN 6 */

RS232Puts("--netif_link_status_down--\r\n");

/* USER CODE END 6 */
  }
}



