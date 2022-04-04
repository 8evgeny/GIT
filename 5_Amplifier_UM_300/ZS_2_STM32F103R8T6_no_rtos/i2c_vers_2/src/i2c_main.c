#include "delay.h"
#include "LiquidCrystal_I2C.h"
#include "main.h"

char line1[] = "                    ";
char line2[] = "                    ";
char line3[] = "                    ";
char line4[] = "                    ";

void stringToLcd(const char* str)
{
    char tmp[] = "                    ";
    memcpy (tmp, str, strlen(str));
    memcpy (line4, tmp, 20);
    LCDI2C_write_String(line1);
    LCDI2C_write_String(line2);
    LCDI2C_write_String(line3);
    LCDI2C_write_String(line4);
    memcpy (line1, line3, 20);
    memcpy (line3, line2, 20);
    memcpy (line2, line4, 20);
}

void i2c_main_vers2(void *args)
{
    (void)args;

#ifdef useDisplay
  LCDI2C_init(0x27,20,4);
  LCDI2C_backlight(); // finish with backlight on
  LCDI2C_clear();
#endif

  for (;;)
  {
//      vTaskDelay(pdMS_TO_TICKS(50));
  }

}
