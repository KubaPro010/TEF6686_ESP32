#pragma once

#define TFT_ESPI_VERSION "2.5.43"

#include <Arduino.h>
#include <SPI.h>

#include <User_Setup_Select.h>

#include <pgmspace.h>

#define PROCESSOR_ID 0x32
#define SPI_PORT VSPI
#include "soc/spi_reg.h"
#include "driver/spi_master.h"
#include "hal/gpio_ll.h"

#define SET_BUS_WRITE_MODE *_spi_user = SPI_USR_MOSI
#define SET_BUS_READ_MODE  *_spi_user = SPI_USR_MOSI | SPI_USR_MISO | SPI_DOUTDIN

#define SPI_BUSY_CHECK while (*_spi_cmd&SPI_USR)

#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h" // ESP32 only

#define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
#define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)

#define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
#define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)

#define WR_L
#define WR_H

#ifndef TFT_MISO
  #define TFT_MISO -1
#endif

#ifndef TFT_MOSI
  #define TFT_MOSI 23
#endif

#ifndef TFT_SCLK
  #define TFT_SCLK 18
#endif

#define TFT_WRITE_BITS(D, B) *_spi_mosi_dlen = B-1;    \
                              *_spi_w = D;             \
                              *_spi_cmd = SPI_USR;      \
                      while (*_spi_cmd & SPI_USR);

// Write 8 bits
#define tft_Write_8(C) TFT_WRITE_BITS(C, 8)

// Write 16 bits with corrected endianness for 16-bit colours
#define tft_Write_16(C) TFT_WRITE_BITS((C)<<8 | (C)>>8, 16)

// Future option for transfer without wait
#define tft_Write_16N(C) *_spi_mosi_dlen = 16-1;       \
                          *_spi_w = ((C)<<8 | (C)>>8); \
                          *_spi_cmd = SPI_USR;

// Write 16 bits
#define tft_Write_16S(C) TFT_WRITE_BITS(C, 16)

// Write 32 bits
#define tft_Write_32(C) TFT_WRITE_BITS(C, 32)

// Write two address coordinates
#define tft_Write_32C(C,D)  TFT_WRITE_BITS((uint16_t)((D)<<8 | (D)>>8)<<16 | (uint16_t)((C)<<8 | (C)>>8), 32)

// Write same value twice
#define tft_Write_32D(C) TFT_WRITE_BITS((uint16_t)((C)<<8 | (C)>>8)<<16 | (uint16_t)((C)<<8 | (C)>>8), 32)

////////////////////////////////////////////////////////////////////////////////////////
// Macros to read from display using SPI or software SPI
////////////////////////////////////////////////////////////////////////////////////////
#if !defined (TFT_PARALLEL_8_BIT)
  #define tft_Read_8() spi.transfer(0)
#endif

#define DAT8TO32(P) ( (uint32_t)P[0]<<8 | P[1] | P[2]<<24 | P[3]<<16 )

#ifndef TFT_SPI_MODE
  #define TFT_SPI_MODE SPI_MODE0
#endif

//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

// Next is a special 16-bit colour value that encodes to 8 bits
// and will then decode back to the same 16-bit value.
// Convenient for 8-bit and 16-bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green

// Default palette for 4-bit colour sprites
static const uint16_t default_4bit_palette[] PROGMEM = {
  TFT_BLACK,    //  0  ^
  TFT_BROWN,    //  1  |
  TFT_RED,      //  2  |
  TFT_ORANGE,   //  3  |
  TFT_YELLOW,   //  4  Colours 0-9 follow the resistor colour code!
  TFT_GREEN,    //  5  |
  TFT_BLUE,     //  6  |
  TFT_PURPLE,   //  7  |
  TFT_DARKGREY, //  8  |
  TFT_WHITE,    //  9  v
  TFT_CYAN,     // 10  Blue+green mix
  TFT_MAGENTA,  // 11  Blue+red mix
  TFT_MAROON,   // 12  Darker red colour
  TFT_DARKGREEN,// 13  Darker green colour
  TFT_NAVY,     // 14  Darker blue colour
  TFT_PINK      // 15
};

typedef uint16_t (*getColorCallback)(uint16_t x, uint16_t y);

class TFT_eSPI { friend class TFT_eSprite;

 public:
  void setSPISpeed(uint8_t speed_Mhz);
  TFT_eSPI(int16_t _W = TFT_WIDTH, int16_t _H = TFT_HEIGHT);
  void init();

  virtual void drawPixel(int32_t x, int32_t y, uint32_t color),
                   drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color),
                   drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
                   drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),
                   fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

  virtual int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
                   drawChar(uint16_t uniCode, int32_t x, int32_t y),
                   height(),
                   width();
  virtual uint16_t readPixel(int32_t x, int32_t y);
  virtual void setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);
  virtual void pushColor(uint16_t color);
  virtual void begin_nin_write();
  virtual void end_nin_write();

  void setRotation(uint8_t r);
  void invertDisplay(bool i);
  void setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);
  void setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum = true);
  void resetViewport();
  bool clipWindow(int32_t* xs, int32_t* ys, int32_t* xe, int32_t* ye);
  void pushColor(uint16_t color, uint32_t len);
  void pushColors(uint16_t  *data, uint32_t len, bool swap = true);
  void pushBlock(uint16_t color, uint32_t len);
  void pushPixels(const void * data_in, uint32_t len);
  void fillScreen(uint32_t color),
           drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color),
           drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color),
           fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);
  void drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color),
           fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color),
           drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),
           fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),
           fillTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color);
  uint16_t drawPixel(int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color = 0x00FFFFFF);
  void     drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool smoothArc = true);
  void     drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color);
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color = 0x00FFFFFF);
  void     drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF, uint8_t quadrants = 0xF);
  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color, uint32_t bg_color = 0x00FFFFFF);
  void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);
  void     setSwapBytes(bool swap);
  bool     getSwapBytes();
  void     drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
           drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
           setBitmapColor(uint16_t fgcolor, uint16_t bgcolor);
  void     setPivot(int16_t x, int16_t y);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transparent);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transparent);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8 = true, uint16_t *cmap = nullptr);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8 = true, uint16_t *cmap = nullptr);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap = nullptr);

  // Text rendering - value returned is the pixel width of the rendered text
  int16_t  drawNumber(long intNumber, int32_t x, int32_t y, uint8_t font), // Draw integer using specified font number
           drawNumber(long intNumber, int32_t x, int32_t y),               // Draw integer using current font

           // Decimal is the number of decimal places to render
           // Use with setTextDatum() to position values on TFT, and setTextPadding() to blank old displayed values
           drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y, uint8_t font), // Draw float using specified font number
           drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y),               // Draw float using current font

           // Handle char arrays
           // Use with setTextDatum() to position string on TFT, and setTextPadding() to blank old displayed strings
           drawString(const char *string, int32_t x, int32_t y, uint8_t font),  // Draw string using specified font number
           drawString(const char *string, int32_t x, int32_t y),                // Draw string using current font
           drawString(const String& string, int32_t x, int32_t y, uint8_t font),// Draw string using specified font number
           drawString(const String& string, int32_t x, int32_t y);              // Draw string using current font

  // Text rendering and font handling support functions
  void     setCursor(int16_t x, int16_t y),
           setCursor(int16_t x, int16_t y, uint8_t font);

  void     setTextColor(uint16_t color),                    // Set character (glyph) color only (background not over-written)
           setTextColor(uint16_t fgcolor, uint16_t bgcolor, bool bgfill = false),  // Set character (glyph) foreground and background colour, optional background fill for smooth fonts
           setTextSize(uint8_t size);                       // Set character size multiplier (this increases pixel size)

  void     setTextWrap(bool wrapX, bool wrapY = false);     // Turn on/off wrapping of text in TFT width and/or height

  void     setTextDatum(uint8_t datum);                     // Set text datum position (default is top left), see Section 5 above

  void     setTextPadding(uint16_t x_width);                // Set text padding (background blanking) width in pixels

  void setTextFont(uint8_t font);                       // Set the font number to use in future
  int16_t  textWidth(const char *string, uint8_t font),     // Returns pixel width of string in specified font
           textWidth(const char *string),                   // Returns pixel width of string in current font
           textWidth(const String& string, uint8_t font),   // As above for String types
           textWidth(const String& string),
           fontHeight(uint8_t font),                        // Returns pixel height of specified font
           fontHeight();                                // Returns pixel height of current font

           // Used by library and Smooth font class to extract Unicode point codes from a UTF8 encoded string
  uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining),
           decodeUTF8(uint8_t c);

  void     spiwrite(uint8_t);        // legacy support only
  void     writecommand(uint8_t c);  // Send an 8-bit command, function resets DC/RS high ready for data
  void     writedata(uint8_t d);     // Send data with DC/RS set high

  // Colour conversion
           // Convert 8-bit red, green and blue to 16 bits
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

           // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);

  bool     initDMA(bool ctrl_cs = false);  // Initialise the DMA engine and attach to SPI bus - typically used in setup()
  void     dmaWait(); // wait until DMA is complete

  bool     DMA_Enabled = false;   // Flag for DMA enabled state
  uint8_t  spiBusyCheck = 0;      // Number of ESP32 transfer buffers to check

  // Bare metal functions
  void     startWrite();                         // Begin SPI transaction
  void     writeColor(uint16_t color, uint32_t len); // Deprecated, use pushBlock()
  void     endWrite();                           // End SPI transaction

  // Global variables
  uint32_t textcolor, textbgcolor;         // Text foreground and background colours

  uint32_t bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

  uint8_t  textfont,  // Current selected font number
           textsize,  // Current font size multiplier
           textdatum, // Text reference datum
           rotation;  // Display rotation (0-3)

  uint8_t  decoderState = 0;   // UTF8 decoder state        - not for user access
  uint16_t decoderBuffer;      // Unicode code-point buffer - not for user access

  void     loadFont(const uint8_t array[], uint8_t font);
  void     unloadFont(uint8_t font);
  bool     getUnicodeIndex(uint16_t unicode, uint16_t *index, uint16_t font);

  virtual void drawGlyph(uint16_t code, uint16_t font);

  bool     booted;

  typedef struct
  {
    const uint8_t* gArray;           //array start pointer
    uint16_t gCount;                 // Total number of characters
    uint16_t yAdvance;               // Line advance
    uint16_t spaceWidth;             // Width of a space character
    int16_t  ascent;                 // Height of top of 'd' above baseline, other characters may be taller
    int16_t  descent;                // Offset to bottom of 'p', other characters may have a larger descent
    uint16_t maxAscent;              // Maximum ascent found in font
    uint16_t maxDescent;             // Maximum descent found in font
  } fontMetrics;

  fontMetrics gFonts[7] = {
    { nullptr, 0, 0, 0, 0, 0, 0, 0 },
    { nullptr, 0, 0, 0, 0, 0, 0, 0 },
    { nullptr, 0, 0, 0, 0, 0, 0, 0 },
    { nullptr, 0, 0, 0, 0, 0, 0, 0 },
    { nullptr, 0, 0, 0, 0, 0, 0, 0 },
    { nullptr, 0, 0, 0, 0, 0, 0, 0 },
    { nullptr, 0, 0, 0, 0, 0, 0, 0 }
  };

  // These are for the metrics for each individual glyph (so we don't need to seek this in file and waste time)
  uint16_t* gUnicode[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};  //UTF-16 code, the codes are searched so do not need to be sequential
  uint8_t*  gHeight[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};   //cheight
  uint8_t*  gWidth[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};    //cwidth
  uint8_t*  gxAdvance[7] = {NULL, NULL, NULL, NULL, NULL, NULL}; //setWidth
  int16_t*  gdY[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};       //topExtent
  int8_t*   gdX[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};       //leftExtent
  uint32_t* gBitmap[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};   //file pointer to greyscale bitmap

  uint8_t  getTouchRaw(uint16_t *x, uint16_t *y);
  uint16_t getTouchRawZ();
  void     convertRawXY(uint16_t *x, uint16_t *y);
  uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);

  void     calibrateTouch(uint16_t *data, uint32_t color_fg, uint32_t color_bg, uint8_t size);
  void     setTouch(uint16_t *data);

 //--------------------------------------- private ------------------------------------//
 private:
  inline void begin_tft_write() __attribute__((always_inline));
  inline void end_tft_write()   __attribute__((always_inline));

  inline void begin_tft_read()  __attribute__((always_inline));
  inline void end_tft_read()    __attribute__((always_inline));

  void initBus();

  void pushSwapBytePixels(const void* data_in, uint32_t len);

  void readAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);

  uint8_t readByte();

  void busDir(uint32_t mask, uint8_t mode);

  void gpioMode(uint8_t gpio, uint8_t mode);

  uint8_t sqrt_fraction(uint32_t num);

  float wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);

  getColorCallback getColor = nullptr; // Smooth font callback function pointer

  bool locked, inTransaction, lockTransaction; // SPI transaction and mutex lock flags

  void loadMetrics(uint8_t font); // Function of Fear, which is Unhandled Exception, writing to 0x000000000
  uint32_t readInt32();

  uint8_t* fontPtr = nullptr;

 //-------------------------------------- protected ----------------------------------//
 protected:

uint8_t spi_write_speed;
  //int32_t  win_xe, win_ye;          // Window end coords - not needed

  int32_t  _init_width, _init_height; // Display w/h as input, used by setRotation()
  int32_t  _width, _height;           // Display w/h as modified by current rotation
  int32_t  addr_row, addr_col;        // Window position - used to minimise window commands

  int16_t  _xPivot;   // TFT x pivot point coordinate for rotated Sprites
  int16_t  _yPivot;   // TFT x pivot point coordinate for rotated Sprites

  // Viewport variables
  int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
  int32_t  _xDatum;
  int32_t  _yDatum;
  int32_t  _xWidth;
  int32_t  _yHeight;
  bool     _vpDatum;
  bool     _vpOoB;

  int32_t  cursor_x, cursor_y, padX;
  int32_t  bg_cursor_x;
  int32_t  last_cursor_x;

  bool     isDigits;
  bool     textwrapX, textwrapY;
  bool     _swapBytes;

  uint32_t _lastColor;

  bool     _fillbg;

 private:
  inline void begin_touch_read_write() __attribute__((always_inline));
  inline void end_touch_read_write()   __attribute__((always_inline));

  uint8_t  validTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);

  uint16_t touchCalibration_x0 = 300, touchCalibration_x1 = 3600, touchCalibration_y0 = 300, touchCalibration_y1 = 3600;
  uint8_t  touchCalibration_rotate = 1, touchCalibration_invert_x = 2, touchCalibration_invert_y = 0;

  uint32_t _pressTime;
};

class TFT_eSprite : public TFT_eSPI {
 public:
  explicit TFT_eSprite(TFT_eSPI *tft);
  ~TFT_eSprite();
  void* createSprite(int16_t width, int16_t height, uint8_t frames = 1);
  void* getPointer();
  bool created();
  void deleteSprite();
  void* setColorDepth(int8_t b);
  int8_t getColorDepth();
  void createPalette(uint16_t *palette = nullptr, uint8_t colors = 16);
  void createPalette(const uint16_t *palette = nullptr, uint8_t colors = 16);
  void setPaletteColor(uint8_t index, uint16_t color);
  uint16_t getPaletteColor(uint8_t index);
  void setBitmapColor(uint16_t fg, uint16_t bg);
  void drawPixel(int32_t x, int32_t y, uint32_t color);
  void         fillSprite(uint32_t color),
           setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1),
           pushColor(uint16_t color),
           pushColor(uint16_t color, uint32_t len),
           writeColor(uint16_t color),
           drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color),
           drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
           drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),
           fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
  uint16_t readPixel(int32_t x0, int32_t y0);
  uint16_t readPixelValue(int32_t x, int32_t y);
  void     pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data, uint8_t sbpp = 0);
  void     pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, const uint16_t *data);
  void     pushSprite(int32_t x, int32_t y);
  void     pushSprite(int32_t x, int32_t y, uint16_t transparent);
  bool     pushSprite(int32_t tx, int32_t ty, int32_t sx, int32_t sy, int32_t sw, int32_t sh);
  bool     pushToSprite(TFT_eSprite *dspr, int32_t x, int32_t y);
  bool     pushToSprite(TFT_eSprite *dspr, int32_t x, int32_t y, uint16_t transparent);
  int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
           drawChar(uint16_t uniCode, int32_t x, int32_t y);
  int16_t  width(),
           height();
  void     drawGlyph(uint16_t code, uint16_t font);

 private:

  TFT_eSPI *_tft;

  void*    callocSprite(int16_t width, int16_t height, uint8_t frames = 1);

 protected:

  uint8_t  _bpp;     // bits per pixel (1, 4, 8 or 16)
  uint16_t *_img;    // pointer to 16-bit sprite
  uint8_t  *_img8;   // pointer to  1 and 8-bit sprite frame 1 or frame 2
  uint8_t  *_img4;   // pointer to  4-bit sprite (uses color map)
  uint8_t  *_img8_1; // pointer to frame 1
  uint8_t  *_img8_2; // pointer to frame 2

  uint16_t *_colorMap; // color map pointer: 16 entries, used with 4-bit color map.

  int32_t  _sinra;   // Sine of rotation angle in fixed point
  int32_t  _cosra;   // Cosine of rotation angle in fixed point

  bool     _created;

  int32_t  _xs, _ys, _xe, _ye, _xptr, _yptr;
  int32_t  _sx, _sy;
  uint32_t _sw, _sh;
  uint32_t _scolor;

  int32_t  _iwidth, _iheight; // Sprite memory image bit width and height (swapped during rotations)
  int32_t  _dwidth, _dheight; // Real sprite width and height (for <8bpp Sprites)
  int32_t  _bitwidth;         // Sprite image bit width for drawPixel (for <8bpp Sprites, not swapped)
};

template <typename T> static inline void transpose(T& a, T& b) { T t = a; a = b; b = t; }
template <typename A, typename F, typename B> static inline uint16_t fastBlend(A alpha, F fgc, B bgc)
{
  // Split out and blend 5-bit red and blue channels
  uint32_t rxb = bgc & 0xF81F;
  rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
  // Split out and blend 6-bit green channel
  uint32_t xgx = bgc & 0x07E0;
  xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
  // Recombine channels
  return (rxb & 0xF81F) | (xgx & 0x07E0);
}