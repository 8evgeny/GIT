/**
  ******************************************************************************
  * @file    Menu_uSDFlash.c
  * @author  Milandr
  * @version v1.0.0
  * @date
  * @brief
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"
#include "Menu.h"
#include "Menu_items.h"
#include "leds.h"
#include "lcd.h"
#include "text.h"
#include "joystick.h"
#include "MDR32F9Qx_it.h"
#include "Menu_uSDFlash.h"
#include "ff.h"
#include "diskio.h"   /* Declarations of low level disk I/O functions */
#include "rtc.h"
#include <string.h>

/* Menu_uSDFlash_Private_Constant------------------------------------------*/
#define     LEN_READ_BUFF       60


/* Menu_uSDFlash_Private_Variables-----------------------------------------*/

uint32_t AccSize;       /* Work register for fs command */
uint16_t AccFiles, AccDirs;
FILINFO Finfo;

FATFS Fatfs[_VOLUMES];        /* File system object for each logical drive */
FATFS *fs;
FIL FileWrite, FileRead;        /* file objects */

static uint8_t rc_str[20][22] =
{
    {"OK"},
    {"DISK_ERR"},
    {"INT_ERR"},
    {"NOT_READY"},
    {"NO_FILE"},
    {"NO_PATH"},
    {"INVALID_NAME"},
    {"DENIED"},
    {"EXIST"},
    {"INVALID_OBJECT"},
    {"WRITE_PROTECTED"},
    {"INVALID_DRIVE"},
    {"NOT_ENABLED"},
    {"NO_FILE_SYSTEM"},
    {"MKFS_ABORTED"},
    {"TIMEOUT"},
    {"LOCKED"},
    {"NOT_ENOUGH_CORE"},
    {"TOO_MANY_OPEN_FILES"}
};

static uint8_t ft[4][4] =
{
    {"0"},
    {"12"},
    {"16"},
    {"32"}
};

/* Menu_uSDFlash_Private_Func_diclaration----------------------------------*/
static void Int_2_Str(uint32_t var_Int, uint8_t* var_Str);


/*******************************************************************************
* Function Name  :
* Description    :
*******************************************************************************/
void ShowVolumeStatusFunc(void)
{
    uint32_t    key = NOKEY;
    int8_t     *ptr;
    int32_t     p1;
    FRESULT     res;
    uint8_t     pos = 1;
    uint8_t     temp_Str[10];
    int8_t      temp_Str1[2];

    /* Clear screen and wait for key up */
    LCD_CLS();
    CurrentMethod = MET_AND;
    CurrentFont = &Font_6x8;
    DisplayMenuTitle("Show Volume Status");
    WAIT_UNTIL_KEY_RELEASED(SEL);

    /* Register work area */
    f_mount((uint8_t)0, &Fatfs[0]);

    temp_Str1[0] = '0';
    temp_Str1[1] = ':';
    ptr = &temp_Str1[0];
    res = f_getfree(ptr, (DWORD*)&p1, &fs);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Read status");
    LCD_PUTS(70, ((CurrentFont->Height) * pos + 4), rc_str[res]);
    pos++;

    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "FAT type = FAT");
    LCD_PUTS(85, ((CurrentFont->Height) * pos + 4), ft[fs->fs_type & 3]);
    pos++;

    Int_2_Str((fs->n_fatent - 2) * (fs->csize / 2), &temp_Str[0]);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), &temp_Str[2]);
    LCD_PUTS(50, ((CurrentFont->Height) * pos + 4), "KB disk space");
    pos++;

    Int_2_Str(p1 * (fs->csize / 2), &temp_Str[0]);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), &temp_Str[2]);
    LCD_PUTS(50, ((CurrentFont->Height) * pos + 4), "KB available");
    pos++;

    /* Unregister work area prior to discard it */
    f_mount(0, NULL);

    /* Key handling loop */
    while ( key != SEL )
    {
        WAIT_UNTIL_ANY_KEY;
        key = GetKey();
    }

    /* SEL is pressed - return to main menu */
    WAIT_UNTIL_KEY_RELEASED(key);
    DisplayMenu();
    return;
}


/*******************************************************************************
* Function Name  : WriteFileFunc
* Description    :
*******************************************************************************/
void WriteFileFunc(void)
{
    uint32_t    key = NOKEY;
    uint8_t     pos = 1;
    FRESULT     res;
    RTC         time_date;

    /* Clear screen and wait for key up */
    LCD_CLS();
    CurrentMethod = MET_AND;
    CurrentFont = &Font_6x8;
    DisplayMenuTitle("Write file");
    WAIT_UNTIL_KEY_RELEASED(SEL);

    /* Register work area */
    f_mount((uint8_t)0, &Fatfs[0]);

    /* Create file on the drive 0 */
    res = f_open(&FileWrite, (int8_t*)"0:tst_file.txt", FA_CREATE_ALWAYS | FA_WRITE);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Open file");
    LCD_PUTS(65, ((CurrentFont->Height) * pos + 4), rc_str[res]);
    pos++;

    if (res == FR_OK)
    {
        rtc_gettime (&time_date);
        f_printf(&FileWrite, (int8_t*)"Test file\n");
        f_printf(&FileWrite, (int8_t*)"Time: %2d %2d %2d\n", time_date.hour, time_date.min, time_date.sec);
        f_printf(&FileWrite, (int8_t*)"Date: %2d %2d %4d\n", time_date.mday, time_date.month, time_date.year);
        LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Write file");
        pos++;
    }

    /* Close files */
    res = f_close(&FileWrite);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Close file");
    LCD_PUTS(65, ((CurrentFont->Height) * pos + 4), rc_str[res]);
    pos++;

    /* Unregister work area prior to discard it */
    f_mount(0, NULL);

    /* Key handling loop */
    while ( key != SEL )
    {
        WAIT_UNTIL_ANY_KEY;
        key = GetKey();
    }

    /* SEL is pressed - return to main menu */
    WAIT_UNTIL_KEY_RELEASED(key);
    DisplayMenu();
    return;
}

/*******************************************************************************
* Function Name  : ReadFileFunc
* Description    :
*******************************************************************************/
void ReadFileFunc(void)
{
    uint32_t    key = NOKEY;
    uint8_t     pos = 1;
    FRESULT     res;
    int8_t      read_buff[LEN_READ_BUFF];

    /* Clear screen and wait for key up */
    LCD_CLS();
    CurrentMethod = MET_AND;
    CurrentFont = &Font_6x8;
    DisplayMenuTitle("Read file");
    WAIT_UNTIL_KEY_RELEASED(SEL);

    /* Register work area */
    f_mount((uint8_t)0, &Fatfs[0]);

    /* Create file on the drive 0 */
    res = f_open(&FileRead, (int8_t*)"0:tst_file.txt", FA_READ);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Open file");
    LCD_PUTS(65, ((CurrentFont->Height) * pos + 4), rc_str[res]);
    pos++;

    if (res == FR_OK)
    {
        LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Read file");
        pos++;
        f_gets(&read_buff[0], LEN_READ_BUFF, &FileRead);
        LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), (uint8_t*)&read_buff[0]);
        pos++;
        f_gets(&read_buff[0], LEN_READ_BUFF, &FileRead);
        LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), (uint8_t*)&read_buff[0]);
        pos++;
        f_gets(&read_buff[0], LEN_READ_BUFF, &FileRead);
        LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), (uint8_t*)&read_buff[0]);
        pos++;
    }

    /* Close files */
    res = f_close(&FileRead);
    LCD_PUTS(0, ((CurrentFont->Height) * pos + 4), "Close file");
    LCD_PUTS(65, ((CurrentFont->Height) * pos + 4), rc_str[res]);
    pos++;

    /* Unregister work area prior to discard it */
    f_mount(0, NULL);

    /* Key handling loop */
    while ( key != SEL )
    {
        WAIT_UNTIL_ANY_KEY;
        key = GetKey();
    }

    /* SEL is pressed - return to main menu */
    WAIT_UNTIL_KEY_RELEASED(key);
    DisplayMenu();
    return;
}

/* Menu_uSDFlash_Private_Func_----------------------------------*/
static void Int_2_Str(uint32_t var_Int, uint8_t* var_Str)
{
/*    uint32_t var_Int_, var_Int_D, var_Int_DD;

  var_Int_   = (var_Int) / 1000000;
  var_Int_D  = (var_Int / 1000) - var_Int_*1000;
  var_Int_DD = (var_Int % 1000);

  var_Str[0] = ( var_Int_ / 100) + 0x30;
  var_Str[1] = ((var_Int_ % 100) / 10) + 0x30;
  var_Str[2] = ((var_Int_ % 100) % 10) + 0x30;
  var_Str[3] = ( var_Int_D / 100) + 0x30;
  var_Str[4] = ((var_Int_D % 100) / 10) + 0x30;
  var_Str[5] = ((var_Int_D % 100) % 10) + 0x30;
  var_Str[6] = ( var_Int_DD / 100) + 0x30;
  var_Str[7] = ((var_Int_DD % 100) / 10) + 0x30;
  var_Str[8] = ((var_Int_DD % 100) % 10) + 0x30;
*/
    uint8_t i;
  var_Str[0] = ( var_Int / 1000000000);\
    var_Int = var_Int - var_Str[0] * 1000000000;
  var_Str[1] = ( var_Int / 100000000);
    var_Int = var_Int - var_Str[1] * 100000000;
  var_Str[2] = ( var_Int / 10000000);
    var_Int = var_Int - var_Str[2] * 10000000;
  var_Str[3] = ( var_Int / 1000000);
    var_Int = var_Int - var_Str[3] * 1000000;
  var_Str[4] = ( var_Int / 100000);
    var_Int = var_Int - var_Str[4] * 100000;
  var_Str[5] = ( var_Int / 10000);
    var_Int = var_Int - var_Str[5] * 10000;
  var_Str[6] = ( var_Int / 1000);
    var_Int = var_Int - var_Str[6] * 1000;
  var_Str[7] = ( var_Int / 100);
    var_Int = var_Int - var_Str[7] * 100;
  var_Str[8] = ( var_Int / 10);
    var_Int = var_Int - var_Str[8] * 10;
  var_Str[9] = ( var_Int % 10);

    for (i=0; i<10; i++)
        var_Str[i] = var_Str[i]  + 0x30;

}

/******************* (C) COPYRIGHT 2012 Milandr *******************
*
* END OF FILE Menu_uSDFlash.c */


