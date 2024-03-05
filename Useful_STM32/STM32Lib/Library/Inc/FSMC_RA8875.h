/*
 * FSMC_RA8855.h
 *
 *  Created on: 20 дек. 2021 г.
 *      Author: User
 */

#ifndef INC_FSMC_RA8875_H_
#define INC_FSMC_RA8875_H_

#include "STM32.h"
#include "Colors.h"
#include "Print.h"
#include "math.h"
#include "gfxfont.h"
#include "functions.h"
/*
 * Установки для "железа"
 */
#define FSMC_ACCESS_MODE_A 0b00;
#define FSMC_ACCESS_MODE_B 0b01;
#define FSMC_ACCESS_MODE_C 0b10;
#define FSMC_ACCESS_MODE_D 0b11;

#define TOUCSRCAL_XLOW  79
#define TOUCSRCAL_YLOW  107
#define TOUCSRCAL_XHIGH 914
#define TOUCSRCAL_YHIGH 843

  const static uint8_t fontDimPar[4][5] = {
    {8,16,2,4,0},// INT font
    {8,16,3,0,0},// ROM X16
    {12,24,2,2,0},//ROM X24
    {16,32,2,2,0},//ROM X32
  };

enum FSMC_Ax
{
  FSMC_A0,
  FSMC_A1,
  FSMC_A2,
  FSMC_A3,
  FSMC_A4,
  FSMC_A5,
  FSMC_A6,
  FSMC_A7,
  FSMC_A8,
  FSMC_A9,
  FSMC_A10,
  FSMC_A11,
  FSMC_A12,
  FSMC_A13,
  FSMC_A14,
  FSMC_A15,
  FSMC_A16,
  FSMC_A17,
  FSMC_A18,
  FSMC_A19,
  FSMC_A20,
  FSMC_A21,
  FSMC_A22,
  FSMC_A23
};

enum FSMC_NE
{
  FSMC_NE1,
  FSMC_NE2,
  FSMC_NE3,
  FSMC_NE4,
};

enum RA8875sizes
{
  RA8875_480x80,  /*!<  480x80 Pixel Display */
  RA8875_480x128, /*!< 480x128 Pixel Display */
  RA8875_480x272, /*!< 480x272 Pixel Display */
  RA8875_800x480  /*!< 800x480 Pixel Display */
};

struct RA8875_TypeDef
{
  uint16_t LCD_REG;
  uint16_t LCD_RAM;
};

typedef struct __PRGMTAG_ {
    const uint8_t   *data;
    uint8_t     image_width;
    int       image_datalen;
} tImage;

typedef struct {
    uint8_t     char_code;
    const tImage  *image;
} tChar;

typedef struct {
    uint8_t     length;
    const tChar   *chars;
    uint8_t     font_width;
    uint8_t     font_height;
    bool      rle;
} tFont;

enum RA8875tcursor {    NOCURSOR=0,IBEAM,UNDER,BLOCK };//0,1,2,3
enum RA8875tsize {      X16=0,X24,X32 };//0,1,2
enum RA8875fontSource {   INT=0, EXT };//0,1
enum RA8875fontCoding {   ISO_IEC_8859_1, ISO_IEC_8859_2, ISO_IEC_8859_3, ISO_IEC_8859_4 };
enum RA8875extRomType {   GT21L16T1W, GT21H16T1W, GT23L16U2W, GT30L16U2W, GT30H24T3Y, GT23L24T3Y, GT23L24M1Z, GT23L32S4W, GT30H32S4W, GT30L32S4W, ER3303_1, ER3304_1, ER3301_1 };
enum RA8875extRomCoding {   GB2312, GB12345, BIG5, UNICODE, ASCII, UNIJIS, JIS0208, LATIN };
enum RA8875extRomFamily {   STANDARD, ARIAL, ROMAN, BOLD };
enum RA8875boolean {    LAYER1, LAYER2, TRANSPARENT, LIGHTEN, OR, AND, FLOATING };
enum RA8875writes {     L1=0, L2, CGRAM, PATTERN, CURSOR };
enum RA8875scrollMode{    SIMULTANEOUS, LAYER1ONLY, LAYER2ONLY, BUFFERED };
enum RA8875pattern{     P8X8, P16X16 };
enum RA8875btedatam{    CONT, RECT };
enum RA8875btelayer{    SOURCE, DEST };
enum RA8875intlist{     BTE=1,TOUCH=2, DMA=3,KEY=4 };

#define CENTER        9998
#define ARC_ANGLE_MAX     360
#define ARC_ANGLE_OFFSET  -90
#define ANGLE_OFFSET    -90

class FSMC_RA8875 : public Print
{
  public:
    FSMC_RA8875(uint8_t Ax_Pin, uint8_t NEx_Pin, GPIO_TypeDef* ResetPort, uint16_t ResetPin,
        GPIO_TypeDef* BlackOutPort, uint16_t BlackOutPin); // Конструктор

    // Инициализация дисплея и системные функции
    bool FSMC_Init(enum RA8875sizes Size);                                                    // Инициализация дисплея
    void FSMC16_Speed(uint8_t AddressSetupTime, uint8_t AddressHoldTime, uint8_t DataSetupTime, uint8_t BusTurnAroundDuration,
                      uint8_t CLKDivision, uint8_t DataLatency, uint8_t AccessMode);          // Установка таймингов FSMC
    void FSMC16_GPIO_Speed(uint8_t Speed);                                                    // Установка скорости портов

    /* Управление подсветкой дисплея */
    void GPIOX(bool on);
    void PWM1config(bool on, uint8_t clock);
    void PWM2config(bool on, uint8_t clock);
    void PWM1out(uint8_t p);
    void PWM2out(uint8_t p);
    void displayOn(bool on);

    // Вывод графики
    void drawPixel(int16_t x, int16_t y, uint16_t color);
//    void      drawPixels(uint16_t p[], uint16_t count, int16_t x, int16_t y);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    uint16_t  getPixel(int16_t x, int16_t y);
    //void    getPixels(uint16_t * p, uint32_t count, int16_t x, int16_t y);
    void fillWindow(uint16_t color=COLOR_BLACK);  // Заполняется активное окно
    void clearScreen(uint16_t color=COLOR_BLACK); // Стирается весь экран
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
//    void      drawLineAngle(int16_t x, int16_t y, int16_t angle, uint16_t length, uint16_t color,int offset = -90);
//    void      drawLineAngle(int16_t x, int16_t y, int16_t angle, uint16_t start, uint16_t length, uint16_t color,int offset = -90);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
    void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
    void drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color);
    void fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);//ok
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
//    void      drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color);
//    void      fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color, bool triangled=true);
    void drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot, uint16_t color);
//    void      drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t spacing,uint16_t color);
//    void      setArcParams(float arcAngleMax, int arcAngleOffset);
//    void      setAngleOffset(int16_t angleOffset);

    // Управление цветом
    void  setForegroundColor(uint16_t color);//color of objects in 16bit
//    void    setForegroundColor(uint8_t R,uint8_t G,uint8_t B);//color of objects in 8+8+8bit
    void setBackgroundColor(uint16_t color);//color of objects background in 16bit
    void setBackgroundColor(uint8_t R,uint8_t G,uint8_t B);//color of objects background in 8+8+8bit
//    void    setTransparentColor(uint16_t color);//the current transparent color in 16bit
//    void    setTransparentColor(uint8_t R,uint8_t G,uint8_t B);//the current transparent color in 8+8+8bit
//    void    setColor(uint16_t fcolor,uint16_t bcolor,bool bcolorTraspFlag=false);
    uint16_t  colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div=100);
    uint16_t  colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div=100);

    // Конвертация цвета
    inline uint16_t Color565(uint8_t r,uint8_t g,uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }
    inline uint16_t Color24To565(int32_t color_) { return ((((color_ >> 16) & 0xFF) / 8) << 11) | ((((color_ >> 8) & 0xFF) / 4) << 5) | (((color_) &  0xFF) / 8);}
    inline uint16_t htmlTo565(int32_t color_) { return (uint16_t)(((color_ & 0xF80000) >> 8) | ((color_ & 0x00FC00) >> 5) | ((color_ & 0x0000F8) >> 3));}
    inline void   Color565ToRGB(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b){r = (((color & 0xF800) >> 11) * 527 + 23) >> 6; g = (((color & 0x07E0) >> 5) * 259 + 33) >> 6; b = ((color & 0x001F) * 527 + 23) >> 6;}

    // Управление ориентацией и окнами
    void setActiveWindow(int16_t XL,int16_t XR,int16_t YT,int16_t YB);//The working area where to draw on
    void setActiveWindow(void);
//    void      getActiveWindow(int16_t &XL,int16_t &XR ,int16_t &YT ,int16_t &YB);
//    void      clearActiveWindow(bool full=false);//it clears the active window
//    uint16_t  width(bool absolute=false) const;//the phisical display width
//    uint16_t  height(bool absolute=false) const;//the phisical display height
    void setRotation(uint8_t rotation); //rotate text and graphics
    uint8_t   getRotation(void) {return _rotation;} //return the current rotation 0-3
    uint16_t  getHeight(void) {return _height;}
    uint16_t  getWidth(void) {return _width;}
//    boolean   isPortrait(void);

    //-------------- TEXT -----------------------------------------------------------------------
    void TestTXT(void);
    void setTextColor(uint16_t c) { textcolor = textbgcolor = c; }
    void setTextColor(uint16_t c, uint16_t bg) {textcolor   = c; textbgcolor = bg;}
    void cp437(bool x=true) { _cp437 = x; }

    void    setFontScale(uint8_t scale);//global font scale (w+h)
    void    setFontScale(uint8_t xscale,uint8_t yscale);//font scale separated by w and h
    void    setCursor(uint16_t x, uint16_t y);
    int16_t getCursorX(void) const { return _cursorX; }
    int16_t getCursorY(void) const { return _cursorY; }

//      void    setFontSpacing(uint8_t spc);//0:disabled ... 63:pix max
//      void    setFontInterline(uint8_t pix);//0...63 pix
//      void    setFontFullAlign(bool align);//mmmm... doesn't do nothing! Have to investigate
//      uint8_t   getFontWidth(bool inColums=false);
//      uint8_t   getFontHeight(bool inRows=false);
      //-------------- GRAPHIC POSITION --------------------------------------------------------------
    void    setXY(int16_t x, int16_t y);//graphic set location
    void    setX(int16_t x);
    void    setY(int16_t y) ;

    //-------------- Текст, использование библиотеки Print  -------------------------
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
      // Функции из ардуины
    virtual size_t write(uint8_t);
    inline  size_t write(unsigned long n) { return write((uint8_t)n); }
    inline  size_t write(long n) { return write((uint8_t)n); }
    inline  size_t write(unsigned int n) { return write((uint8_t)n); }
    inline  size_t write(int n) { return write((uint8_t)n); }
    using   Print::write; // используется для write(str) и write(buf, size) как Print

/*
 * ToDo функции тачскрина на резистивной матрице
 */
    void TP_IntEnable(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin);               // Инициализация прерывания от тачскрина в контроллере и на внешний вход
    bool TP_isTouch(void);
    void TP_GetCoordinate(uint16_t *X_Coord, uint16_t *Y_Coord);
//    void TP_IntEnable(bool IE);


  private:
    /*
     * Вся ботва от функции резистивной матрицы
     */
//    void MidValueCalculate(void);
//    uint Get_X_ADC_Value_10bit(void);
//    uint Get_Y_ADC_Value_10bit(void);
//    uint Manual_Get_X_ADC_Value_10bit(void);
//    uint Manual_Get_Y_ADC_Value_10bit(void);
    GPIO_TypeDef  *_GPIO_Tp_Port;
    uint16_t      _GPIO_Tp_Pin;

    bool
      wrap,           ///< If set, 'wrap' text at right edge of display
      _cp437;         ///< If set, use correct CP437 charset (default is off)
      GFXfont
      *gfxFont;         ///< Pointer to special font

    // Инициализация дисплея и системные функции
    void      WR_REG(uint16_t regval);
    void      WR_DATA(uint16_t data);
    void      WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
    uint16_t  RD_DATA(void);
    uint16_t  ReadReg(uint16_t LCD_Reg);
    void      FSMC_GPIO_Init(void);
    void      _setBaseAddress(uint32_t Base, uint32_t Data);
    void      Init(void);
    bool      WaitPoll(uint8_t r, uint8_t f);
    void      _waitBusy(uint8_t res=0x80);//0x80, 0x40(BTE busy), 0x01(DMA busy)

    // Вспомогательная графика
    void      _circle_helper(int16_t x0, int16_t y0, int16_t r, uint16_t color, bool filled);
    void      _rect_helper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled);
    void      _roundRect_helper(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, bool filled);
    void      _triangle_helper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled);
    void      _ellipseCurve_helper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis,uint8_t curvePart, uint16_t color, bool filled);
    void      _drawArc_helper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float startAngle, float endAngle, uint16_t color);
    void      _line_addressing(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void      _curve_addressing(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    float     _cosDeg_helper(float angle);
    float     _sinDeg_helper(float angle);

    // Функции работы с текстом
    void      _textWrite(const char* buffer, uint16_t len=0);//thanks to Paul Stoffregen for the initial version of this one
    void      _charWrite(const char c,uint8_t offset);
    void      _charWriteR(const char c,uint8_t offset,uint16_t fcolor,uint16_t bcolor);
    int       _getCharCode(uint8_t ch);
    void      _drawChar_unc(int16_t x,int16_t y,int16_t w,const uint8_t *data,uint16_t fcolor,uint16_t bcolor);
//    void      _drawChar_unc(int16_t x,int16_t y,int16_t w,const uint8_t *data,uint16_t fcolor,uint16_t bcolor);
    //void    _drawChar_com(int16_t x,int16_t y,int16_t w,const uint8_t *data);
    void      _textPosition(int16_t x, int16_t y,bool update);
    void      _setFNTdimensions(uint8_t index);
    int16_t   _STRlen_helper(const char* buffer,uint16_t len=0);
    void      fontRomSpeed(uint8_t sp);

    // Вспомогательные системные функции
    void      _updateActiveWindow(bool full);
    void      _setTextMode(bool m);
    void      _scanDirection(bool invertH, bool invertV);
    inline __attribute__((always_inline))
      void _checkLimits_helper(int16_t &x,int16_t &y){
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x >= WIDTH) x = WIDTH - 1;
        if (y >= HEIGHT) y = HEIGHT -1;
        x = x;
        y = y;
      }
    inline __attribute__((always_inline))
      void _center_helper(int16_t &x, int16_t &y){
        if (x == CENTER) x = _width/2;
        if (y == CENTER) y = _height/2;
      }

    // Register containers -----------------------------------------
    // this needed to  prevent readRegister from chip that it's slow.

    uint8_t      _DPCR_Reg;  ////Display Configuration          [0x20]
    uint8_t      _FNCR0_Reg; //Font Control Register 0          [0x21]
    uint8_t      _FNCR1_Reg; //Font Control Register1         [0x22]
    uint8_t      _FWTSET_Reg; //Font Write Type Setting Register   [0x2E]
    uint8_t      _SFRSET_Reg; //Serial Font ROM Setting           [0x2F]
    uint8_t      _INTC1_Reg; //Interrupt Control Register1      [0xF0]
    uint8_t     _MWCR0_Reg;

    //text vars-------------------------------------------------
    uint8_t             _FNTspacing;
    uint8_t             _FNTinterline;
    enum RA8875extRomFamily     _EXTFNTfamily;
    enum RA8875extRomType       _EXTFNTrom;
    enum RA8875extRomCoding     _EXTFNTcoding;
    enum RA8875tsize        _EXTFNTsize;//X16,X24,X32
    enum RA8875fontSource       _FNTsource;
    enum RA8875tcursor        _FNTcursorType;

    // Переменные управления цветом
    uint16_t  _foreColor;
    uint16_t  _backColor;
    bool      _backTransparent;
    uint8_t   _colorIndex;
    uint16_t  _TXTForeColor;
    uint16_t  _TXTBackColor;
    bool      _TXTrecoverColor;
    //color space-----------------------------
    uint8_t   _color_bpp;//8=256, 16=64K colors
    uint8_t   _brightness;

    // Переменные экрана
    uint16_t      WIDTH, HEIGHT;//absolute
    uint16_t      _width, _height;
    uint8_t       _rotation;
    bool          _portrait;
    volatile bool _textMode;

    // Переменные тачскрина
    uint16_t      _TP_MinX = TOUCSRCAL_XLOW;
    uint16_t      _TP_MinY = TOUCSRCAL_YLOW;
    uint16_t      _TP_MaxX = TOUCSRCAL_XHIGH;
    uint16_t      _TP_MaxY = TOUCSRCAL_YHIGH;

    // Системные переменные
    uint8_t   _AddressSetupTime;
    uint8_t   _AddressHoldTime;
    uint8_t   _DataSetupTime;
    uint8_t   _BusTurnAroundDuration;
    uint8_t   _CLKDivision;
    uint8_t   _DataLatency;
    uint8_t   _AccessMode;
    uint8_t   _voffset;
    enum      RA8875sizes _size;
    int16_t   _activeWindowXL, _activeWindowXR,_activeWindowYT,_activeWindowYB;


    uint8_t   _FSMC_GPIO_Speed;

    uint8_t   _FSMC_Initialized;
    uint8_t   _FSMC_Ax, _FSMC_NEx;
    uint32_t  _TFT_Base, _TFT_Ax;
    uint8_t   _BTCR = 0;
    uint8_t   _BWTR = 0;

    GPIO_TypeDef *_GPIO_Blackout;
    uint16_t      _GPIO_Pin_Blackout;
//    bool          Flag_Blackout;
//    bool          Direction_Blackout;

    GPIO_TypeDef *_GPIO_Reset;
    uint16_t      _GPIO_Pin_Reset;
//    bool          Flag_Reset;
//    bool          Direction_Reset;

    uint8_t       _GPIO_Speed = Speed_Low;

    // Центровка
    bool              _relativeCenter;
    bool              _absoluteCenter;

  protected:
    volatile uint8_t  _TXTparameters;
    int16_t           _cursorX,        _cursorY;
    uint8_t           _scaleX = 1,         _scaleY = 1;
    bool              _scaling;
    uint8_t           _FNTwidth,       _FNTheight;
    uint8_t           _FNTbaselineLow, _FNTbaselineTop;
    bool              _FNTgrandient;
    uint16_t          _FNTgrandientColor1;
    uint16_t          _FNTgrandientColor2;
    bool              _FNTcompression;
    int               _spaceCharWidth;

    uint16_t
      textcolor,      ///< 16-bit цвет фона для функции print()
      textbgcolor;    ///< 16-bit цвет текста для функции print()

};

// Регистры и биты
// Регистры контроллера дисплея
#define RA8875_DPCR             0x20              // Конфигурация дисплея
#define RA8875_FNCR1            0x22              // Контроллер шрифта 2

// Регистры цвета
#define RA8875_BGCR0            0x60              // Цвет фона, регистр 0 (R)
#define RA8875_FGCR0            0x63              // Цвет переднего плана, регистр 0 (R)
#define RA8875_BGTR0            0x67              // Цвет альфаканала, регистр 0 (R)

// Регистры отрисовки графики
#define RA8875_DLHSR0           0x91              // Линия/прямоугольник. Стартовый адрес по горизонтали. Регистр 0
#define RA8875_DLVSR0           0x93              // Линия/прямоугольник. Vertical Start Address Register0
#define RA8875_DLHER0           0x95              // Линия/прямоугольник. Horizontal End Address Register0
#define RA8875_DLVER0           0x97              // Линия/прямоугольник. Vertical End Address Register0
#define RA8875_DCHR0            0x99              // Регистр рисования окружностей
#define RA8875_DCVR0            0x9B              //Draw Circle Center Vertical Address Register0
#define RA8875_DCRR             0x9D              //Draw Circle Radius Register
#define RA8875_ELLIPSE          0xA0              //Draw Ellipse/Ellipse Curve/Circle Square Control Register
#define RA8875_ELLIPSE_STATUS   0x80
#define RA8875_ELL_A0           0xA1              //Draw Ellipse/Circle Square Long axis Setting Register0
#define RA8875_ELL_B0           0xA3              //Draw Ellipse/Circle Square Short axis Setting Register0
#define RA8875_DEHR0            0xA5              //Draw Ellipse/Circle Square Center Horizontal Address Register0
#define RA8875_DEVR0            0xA7              //Draw Ellipse/Circle Square Center Vertical Address Register0
#define RA8875_DTPH0            0xA9              //Draw Triangle Point 2 Horizontal Address Register0
#define RA8875_DTPV0            0xAB              //Draw Triangle Point 2 Vertical Address Register0

#define RA8875_F_CURXL          0x2A//Font Write Cursor Horizontal Position Register 0
#define RA8875_F_CURXH          0x2B//Font Write Cursor Horizontal Position Register 1
#define RA8875_F_CURYL          0x2C//Font Write Cursor Vertical Position Register 0
#define RA8875_F_CURYH          0x2D//Font Write Cursor Vertical Position Register 1
#define RA8875_FWTSET               0x2E//Font Write Type Setting Register
#define RA8875_SFRSET               0x2F//Serial Font ROM Setting

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                            DMA REGISTERS
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define RA8875_SSAR0          0xB0//Source Starting Address REG 0
//#define RA8875_SSAR1          0xB1//Source Starting Address REG 1
//#define RA8875_SSAR2          0xB2//Source Starting Address REG 2
//#define RA8875_????         0xB3//???????????

//#define RA8875_DTNR0          0xB4//Block Width REG 0(BWR0) / DMA Transfer Number REG 0
//#define RA8875_BWR1           0xB5//Block Width REG 1
//#define RA8875_DTNR1          0xB6//Block Height REG 0(BHR0) /DMA Transfer Number REG 1
//#define RA8875_BHR1           0xB7//Block Height REG 1
//#define RA8875_DTNR2          0xB8//Source Picture Width REG 0(SPWR0) / DMA Transfer Number REG 2
//#define RA8875_SPWR1          0xB9//Source Picture Width REG 1
#define RA8875_DMACR          0xBF//DMA Configuration REG


#define RA8875_PWRR             0x01              ///< See datasheet
#define RA8875_PWRR_DISPON      0x80              ///< See datasheet
#define RA8875_PWRR_DISPOFF     0x00              ///< See datasheet
//#define RA8875_PWRR_SLEEP       0x02              ///< See datasheet
#define RA8875_PWRR_NORMAL      0x00              ///< See datasheet
//#define RA8875_PWRR_SOFTRESET   0x01              ///< See datasheet

#define RA8875_MRWC             0x02              ///< See datasheet

#define RA8875_GPIOX            0xC7              ///< See datasheet

#define RA8875_PLLC1            0x88              ///< See datasheet
#define RA8875_PLLC1_PLLDIV2    0x80              ///< See datasheet
#define RA8875_PLLC1_PLLDIV1    0x00              ///< See datasheet

#define RA8875_PLLC2            0x89              ///< See datasheet
//#define RA8875_PLLC2_DIV1       0x00              ///< See datasheet
//#define RA8875_PLLC2_DIV2       0x01              ///< See datasheet
#define RA8875_PLLC2_DIV4       0x02              ///< See datasheet
//#define RA8875_PLLC2_DIV8       0x03              ///< See datasheet
//#define RA8875_PLLC2_DIV16      0x04              ///< See datasheet
//#define RA8875_PLLC2_DIV32      0x05              ///< See datasheet
//#define RA8875_PLLC2_DIV64      0x06              ///< See datasheet
//#define RA8875_PLLC2_DIV128     0x07              ///< See datasheet

#define RA8875_SYSR 0x10       ///< See datasheet
//#define RA8875_SYSR_8BPP 0x00  ///< See datasheet
#define RA8875_SYSR_16BPP 0x0C ///< See datasheet
#define RA8875_SYSR_MCU8 0x00  ///< See datasheet
//#define RA8875_SYSR_MCU16 0x03 ///< See datasheet

#define RA8875_PCSR 0x04       ///< See datasheet
//#define RA8875_PCSR_PDATR 0x00 ///< See datasheet
#define RA8875_PCSR_PDATL 0x80 ///< See datasheet
//#define RA8875_PCSR_CLK 0x00   ///< See datasheet
#define RA8875_PCSR_2CLK 0x01  ///< See datasheet
#define RA8875_PCSR_4CLK 0x02  ///< See datasheet
//#define RA8875_PCSR_8CLK 0x03  ///< See datasheet

#define RA8875_HDWR 0x14 ///< See datasheet

#define RA8875_HNDFTR 0x15         ///< See datasheet
#define RA8875_HNDFTR_DE_HIGH 0x00 ///< See datasheet
//#define RA8875_HNDFTR_DE_LOW 0x80  ///< See datasheet

#define RA8875_HNDR 0x16      ///< See datasheet
#define RA8875_HSTR 0x17      ///< See datasheet
#define RA8875_HPWR 0x18      ///< See datasheet
#define RA8875_HPWR_LOW 0x00  ///< See datasheet
//#define RA8875_HPWR_HIGH 0x80 ///< See datasheet

#define RA8875_VDHR0 0x19     ///< See datasheet
#define RA8875_VDHR1 0x1A     ///< See datasheet
#define RA8875_VNDR0 0x1B     ///< See datasheet
#define RA8875_VNDR1 0x1C     ///< See datasheet
#define RA8875_VSTR0 0x1D     ///< See datasheet
#define RA8875_VSTR1 0x1E     ///< See datasheet
#define RA8875_VPWR 0x1F      ///< See datasheet
#define RA8875_VPWR_LOW 0x00  ///< See datasheet
#define RA8875_VPWR_HIGH 0x80 ///< See datasheet

#define RA8875_HSAW0 0x30 ///< See datasheet
#define RA8875_HSAW1 0x31 ///< See datasheet
#define RA8875_VSAW0 0x32 ///< See datasheet
#define RA8875_VSAW1 0x33 ///< See datasheet

#define RA8875_HEAW0 0x34 ///< See datasheet
#define RA8875_HEAW1 0x35 ///< See datasheet
#define RA8875_VEAW0 0x36 ///< See datasheet
#define RA8875_VEAW1 0x37 ///< See datasheet

#define RA8875_MCLR 0x8E            ///< See datasheet
#define RA8875_MCLR_START 0x80      ///< See datasheet
#define RA8875_MCLR_STOP 0x00       ///< See datasheet
#define RA8875_MCLR_READSTATUS 0x80 ///< See datasheet
#define RA8875_MCLR_FULL 0x00       ///< See datasheet
#define RA8875_MCLR_ACTIVE 0x40     ///< See datasheet

#define RA8875_DCR 0x90                   ///< See datasheet
#define RA8875_DCR_LINESQUTRI_START 0x80  ///< See datasheet
#define RA8875_DCR_LINESQUTRI_STOP 0x00   ///< See datasheet
#define RA8875_DCR_LINESQUTRI_STATUS 0x80 ///< See datasheet
#define RA8875_DCR_CIRCLE_START 0x40      ///< See datasheet
#define RA8875_DCR_CIRCLE_STATUS 0x40     ///< See datasheet
#define RA8875_DCR_CIRCLE_STOP 0x00       ///< See datasheet
#define RA8875_DCR_FILL 0x20              ///< See datasheet
#define RA8875_DCR_NOFILL 0x00            ///< See datasheet
#define RA8875_DCR_DRAWLINE 0x00          ///< See datasheet
#define RA8875_DCR_DRAWTRIANGLE 0x01      ///< See datasheet
#define RA8875_DCR_DRAWSQUARE 0x10        ///< See datasheet

//#define RA8875_ELLIPSE 0xA0        ///< See datasheet
//#define RA8875_ELLIPSE_STATUS 0x80 ///< See datasheet

#define RA8875_MWCR0 0x40         ///< See datasheet
#define RA8875_MWCR0_GFXMODE 0x00 ///< See datasheet
#define RA8875_MWCR0_TXTMODE 0x80 ///< See datasheet
#define RA8875_MWCR0_CURSOR 0x40  ///< See datasheet
#define RA8875_MWCR0_BLINK 0x20   ///< See datasheet

//#define RA8875_MWCR0_DIRMASK 0x0C ///< Bitmask for Write Direction
//#define RA8875_MWCR0_LRTD 0x00    ///< Left->Right then Top->Down
//#define RA8875_MWCR0_RLTD 0x04    ///< Right->Left then Top->Down
//#define RA8875_MWCR0_TDLR 0x08    ///< Top->Down then Left->Right
//#define RA8875_MWCR0_DTLR 0x0C    ///< Down->Top then Left->Right

//#define RA8875_BTCR 0x44  ///< See datasheet
#define RA8875_CURH0 0x46 ///< See datasheet
//#define RA8875_CURH1 0x47 ///< See datasheet
#define RA8875_CURV0 0x48 ///< See datasheet
//#define RA8875_CURV1 0x49 ///< See datasheet

#define RA8875_P1CR 0x8A         ///< See datasheet
#define RA8875_P1CR_ENABLE 0x80  ///< See datasheet
#define RA8875_P1CR_DISABLE 0x00 ///< See datasheet
//#define RA8875_P1CR_CLKOUT 0x10  ///< See datasheet
//#define RA8875_P1CR_PWMOUT 0x00  ///< See datasheet

#define RA8875_P1DCR 0x8B ///< See datasheet

#define RA8875_P2CR 0x8C         ///< See datasheet
#define RA8875_P2CR_ENABLE 0x80  ///< See datasheet
#define RA8875_P2CR_DISABLE 0x00 ///< See datasheet
//#define RA8875_P2CR_CLKOUT 0x10  ///< See datasheet
//#define RA8875_P2CR_PWMOUT 0x00  ///< See datasheet

#define RA8875_P2DCR 0x8D ///< See datasheet

//#define RA8875_PWM_CLK_DIV1 0x00     ///< See datasheet
//#define RA8875_PWM_CLK_DIV2 0x01     ///< See datasheet
//#define RA8875_PWM_CLK_DIV4 0x02     ///< See datasheet
//#define RA8875_PWM_CLK_DIV8 0x03     ///< See datasheet
//#define RA8875_PWM_CLK_DIV16 0x04    ///< See datasheet
//#define RA8875_PWM_CLK_DIV32 0x05    ///< See datasheet
//#define RA8875_PWM_CLK_DIV64 0x06    ///< See datasheet
//#define RA8875_PWM_CLK_DIV128 0x07   ///< See datasheet
//#define RA8875_PWM_CLK_DIV256 0x08   ///< See datasheet
//#define RA8875_PWM_CLK_DIV512 0x09   ///< See datasheet
#define RA8875_PWM_CLK_DIV1024 0x0A  ///< See datasheet
//#define RA8875_PWM_CLK_DIV2048 0x0B  ///< See datasheet
//#define RA8875_PWM_CLK_DIV4096 0x0C  ///< See datasheet
//#define RA8875_PWM_CLK_DIV8192 0x0D  ///< See datasheet
//#define RA8875_PWM_CLK_DIV16384 0x0E ///< See datasheet
//#define RA8875_PWM_CLK_DIV32768 0x0F ///< See datasheet

#define RA8875_INT_IE                     0xF0              // Порт управления прерываниями
#define RA8875_KeyScan_IE                 0x10              // Разрешение прерывания от сканера клавиатуры
#define RA8875_DMA_IE                     0x08              // Разрешение прерывания от DMA
#define RA8875_TouchPanel_IE              0x04              // Разрешение прерывания от ТачПанели
#define RA8875_BTEComplete_IE             0x02              // Разрешение прерывания по окончании работы модуля BTE
#define RA8875_BTE_or_Font_IE             0x01              // Разрешение прерывания от BTE или по окончанию вывода текста

#define RA8875_INT_IF                     0xF1              // Порт флагов и очистки флагов прерываний
#define RA8875_KeyScan_IFC                0x10              // Флаг сканера клавиатуры
#define RA8875_DMA_IFC                    0x08              // Флаг DMA
#define RA8875_TouchPanel_IFC             0x04              // Флаг ТачПанели
#define RA8875_BTEComplete_IFC            0x02              // Флаг окончания работы BLE
#define RA8875_BTE_or_Font_IFC            0x01              // Флаг BLE или окончание вывода текста

#define RA8875_TPCR0                      0x70              // Порт управление тачпанелью №1
#define RA8875_TPCR1                      0x71              // Порт управление тачпанелью №2

#define RA8875_TPCR0_EN                   0x80              // бит включения ТачСкрина
#define RA8875_TPCR0_WAIT_512CLK          0x00              // Периоды ожидания данных от АЦП
#define RA8875_TPCR0_WAIT_1024CLK         0x10              //
#define RA8875_TPCR0_WAIT_2048CLK         0x20              //
#define RA8875_TPCR0_WAIT_4096CLK         0x30              //
#define RA8875_TPCR0_WAIT_8192CLK         0x40              //
#define RA8875_TPCR0_WAIT_16384CLK        0x50              //
#define RA8875_TPCR0_WAIT_32768CLK        0x60              //
#define RA8875_TPCR0_WAIT_65536CLK        0x70              //
#define RA8875_TPCR0_WAKE_EN              0x08              // Бит разрешения вывода дисплея из спячки от ТачСкрина
#define RA8875_TPCR0_ADCCLK_DIV1          0x00              // Делитель тактового генератора АЦП
#define RA8875_TPCR0_ADCCLK_DIV2          0x01              //
#define RA8875_TPCR0_ADCCLK_DIV4          0x02              //
#define RA8875_TPCR0_ADCCLK_DIV8          0x03              //
#define RA8875_TPCR0_ADCCLK_DIV16         0x04              //
#define RA8875_TPCR0_ADCCLK_DIV32         0x05              //
#define RA8875_TPCR0_ADCCLK_DIV64         0x06              //
#define RA8875_TPCR0_ADCCLK_DIV128        0x07              //

#define RA8875_TPCR1_MANUAL               0x40              // Бит включения ручного управления ТачСкрином
#define RA8875_TPCR1_VREFEXT              0x20              // Бит управления опорным напряжением
#define RA8875_TPCR1_DEBOUNCE             0x04              // Бит управления регистром "дребезга"
// Регистры ручного режима
#define RA8875_TPCR1_IDLE                 0x00              //
#define RA8875_TPCR1_WAIT                 0x01              //
#define RA8875_TPCR1_LATCHX               0x02              //
#define RA8875_TPCR1_LATCHY               0x03              //

#define RA8875_TPXH                       0x72              // Старший байт АЦП по оси Х
#define RA8875_TPYH                       0x73              // Старший байт АЦП по оси Y
#define RA8875_TPXYL                      0x74              // Младшие биты АЦП по обеим осям

//#define RA8875_SCROLL_BOTH                0x00   ///< See datasheet
//#define RA8875_SCROLL_LAYER1              0x40 ///< See datasheet
//#define RA8875_SCROLL_LAYER2              0x80 ///< See datasheet
//#define RA8875_SCROLL_BUFFER              0xC0 ///< See datasheet


#endif /* INC_FSMC_RA8875_H_ */
