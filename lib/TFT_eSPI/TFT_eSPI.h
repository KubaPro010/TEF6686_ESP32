#pragma once

#define TFT_ESPI_VERSION "2.5.43"

#include <Arduino.h>
#include <Print.h>
#include <SPI.h>

#include <User_Setup_Select.h>

#include <pgmspace.h>

// Processor ID reported by getSetup()
#define PROCESSOR_ID 0x32

#ifdef USE_HSPI_PORT
  #ifdef CONFIG_IDF_TARGET_ESP32
    #define SPI_PORT HSPI  //HSPI is port 2 on ESP32
  #else
    #define SPI_PORT 3     //HSPI is port 3 on ESP32 S2
  #endif
#elif defined(USE_FSPI_PORT)
    #define SPI_PORT 2 //FSPI(ESP32 S2)
#else
  #ifdef CONFIG_IDF_TARGET_ESP32
    #define SPI_PORT VSPI
  #else
    #define SPI_PORT 2 //FSPI(ESP32 S2)
  #endif
#endif

// Include processor specific header
#include "soc/spi_reg.h"
#include "driver/spi_master.h"
#include "hal/gpio_ll.h"

#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32)
  #define CONFIG_IDF_TARGET_ESP32
#endif

#if (TFT_SPI_MODE == SPI_MODE1) || (TFT_SPI_MODE == SPI_MODE2)
  #define SET_BUS_WRITE_MODE *_spi_user = SPI_USR_MOSI | SPI_CK_OUT_EDGE
  #define SET_BUS_READ_MODE  *_spi_user = SPI_USR_MOSI | SPI_USR_MISO | SPI_DOUTDIN | SPI_CK_OUT_EDGE
#else
  #define SET_BUS_WRITE_MODE *_spi_user = SPI_USR_MOSI
  #define SET_BUS_READ_MODE  *_spi_user = SPI_USR_MOSI | SPI_USR_MISO | SPI_DOUTDIN
#endif

#define ESP32_DMA 

#define SPI_BUSY_CHECK while (*_spi_cmd&SPI_USR)

// If smooth font is used then it is likely SPIFFS will be needed
// Call up the SPIFFS (SPI FLASH Filing System) for the anti-aliased fonts
#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h" // ESP32 only

#ifndef TFT_DC
  #define DC_C // No macro allocated so it generates no code
  #define DC_D // No macro allocated so it generates no code
#else
  #if (TFT_DC >= 32)
    #ifdef RPI_DISPLAY_TYPE  // RPi displays need a slower DC change
      #define DC_C GPIO.out1_w1ts.val = (1 << (TFT_DC - 32)); \
                    GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))
      #define DC_D GPIO.out1_w1tc.val = (1 << (TFT_DC - 32)); \
                    GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))
    #else
      #define DC_C GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))//;GPIO.out1_w1tc.val = (1 << (TFT_DC - 32))
      #define DC_D GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))//;GPIO.out1_w1ts.val = (1 << (TFT_DC - 32))
    #endif
  #elif (TFT_DC >= 0)
    #if defined (RPI_DISPLAY_TYPE)
      #if defined (ILI9486_DRIVER)
        // RPi ILI9486 display needs a slower DC change
        #define DC_C GPIO.out_w1tc = (1 << TFT_DC); \
                      GPIO.out_w1tc = (1 << TFT_DC)
        #define DC_D GPIO.out_w1tc = (1 << TFT_DC); \
                      GPIO.out_w1ts = (1 << TFT_DC)
      #else
        // Other RPi displays need a slower C->D change
        #define DC_C GPIO.out_w1tc = (1 << TFT_DC)
        #define DC_D GPIO.out_w1tc = (1 << TFT_DC); \
                      GPIO.out_w1ts = (1 << TFT_DC)
      #endif
    #else
      #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
    #endif
  #else
    #define DC_C
    #define DC_D
  #endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the CS (TFT chip select) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#ifndef TFT_CS
  #define TFT_CS -1  // Keep DMA code happy
  #define CS_L       // No macro allocated so it generates no code
  #define CS_H       // No macro allocated so it generates no code
#else
  #if (TFT_CS >= 32)
    #define CS_L GPIO.out1_w1tc.val = (1 << (TFT_CS - 32)); GPIO.out1_w1tc.val = (1 << (TFT_CS - 32))
    #define CS_H GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))//;GPIO.out1_w1ts.val = (1 << (TFT_CS - 32))
  #elif (TFT_CS >= 0)
    #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
    #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
  #else
    #define CS_L
    #define CS_H
  #endif
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the WR (TFT Write) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#if defined (TFT_WR)
  #if (TFT_WR >= 32)
    // Note: it will be ~1.25x faster if the TFT_WR pin uses a GPIO pin lower than 32
    #define WR_L GPIO.out1_w1tc.val = (1 << (TFT_WR - 32))
    #define WR_H GPIO.out1_w1ts.val = (1 << (TFT_WR - 32))
  #elif (TFT_WR >= 0)
    // TFT_WR, for best performance, should be in range 0-31 for single register parallel write
    #define WR_L GPIO.out_w1tc = (1 << TFT_WR)
    #define WR_H GPIO.out_w1ts = (1 << TFT_WR)
  #else
    #define WR_L
    #define WR_H
  #endif
#else
  #define WR_L
  #define WR_H
#endif

#ifndef TFT_MISO
  #define TFT_MISO -1
#endif

#ifndef TFT_MOSI
  #define TFT_MOSI 23
#endif
#if (TFT_MOSI == -1)
  #undef TFT_MOSI
  #define TFT_MOSI 23
#endif

#ifndef TFT_SCLK
  #define TFT_SCLK 18
#endif
#if (TFT_SCLK == -1)
  #undef TFT_SCLK
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

static const unsigned char font[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
	0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x1C, 0x57, 0x7D, 0x57, 0x1C,
	0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
	0x00, 0x18, 0x3C, 0x18, 0x00,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x00, 0x18, 0x24, 0x18, 0x00,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0x30, 0x48, 0x3A, 0x06, 0x0E,
	0x26, 0x29, 0x79, 0x29, 0x26,
	0x40, 0x7F, 0x05, 0x05, 0x07,
	0x40, 0x7F, 0x05, 0x25, 0x3F,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0x7F, 0x3E, 0x1C, 0x1C, 0x08,
	0x08, 0x1C, 0x1C, 0x3E, 0x7F,
	0x14, 0x22, 0x7F, 0x22, 0x14,
	0x5F, 0x5F, 0x00, 0x5F, 0x5F,
	0x06, 0x09, 0x7F, 0x01, 0x7F,
	0x00, 0x66, 0x89, 0x95, 0x6A,
	0x60, 0x60, 0x60, 0x60, 0x60,
	0x94, 0xA2, 0xFF, 0xA2, 0x94,
	0x08, 0x04, 0x7E, 0x04, 0x08,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x08, 0x08, 0x2A, 0x1C, 0x08,
	0x08, 0x1C, 0x2A, 0x08, 0x08,
	0x1E, 0x10, 0x10, 0x10, 0x10,
	0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
	0x30, 0x38, 0x3E, 0x38, 0x30,
	0x06, 0x0E, 0x3E, 0x0E, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
	0x1E, 0xA1, 0xA1, 0x61, 0x12,
	0x3A, 0x40, 0x40, 0x20, 0x7A,
	0x38, 0x54, 0x54, 0x55, 0x59,
	0x21, 0x55, 0x55, 0x79, 0x41,
	0x21, 0x54, 0x54, 0x78, 0x41,
	0x21, 0x55, 0x54, 0x78, 0x40,
	0x20, 0x54, 0x55, 0x79, 0x40,
	0x0C, 0x1E, 0x52, 0x72, 0x12,
	0x39, 0x55, 0x55, 0x55, 0x59,
	0x39, 0x54, 0x54, 0x54, 0x59,
	0x39, 0x55, 0x54, 0x54, 0x58,
	0x00, 0x00, 0x45, 0x7C, 0x41,
	0x00, 0x02, 0x45, 0x7D, 0x42,
	0x00, 0x01, 0x45, 0x7C, 0x40,
	0xF0, 0x29, 0x24, 0x29, 0xF0,
	0xF0, 0x28, 0x25, 0x28, 0xF0,
	0x7C, 0x54, 0x55, 0x45, 0x00,
	0x20, 0x54, 0x54, 0x7C, 0x54,
	0x7C, 0x0A, 0x09, 0x7F, 0x49,
	0x32, 0x49, 0x49, 0x49, 0x32,
	0x32, 0x48, 0x48, 0x48, 0x32,
	0x32, 0x4A, 0x48, 0x48, 0x30,
	0x3A, 0x41, 0x41, 0x21, 0x7A,
	0x3A, 0x42, 0x40, 0x20, 0x78,
	0x00, 0x9D, 0xA0, 0xA0, 0x7D,
	0x39, 0x44, 0x44, 0x44, 0x39,
	0x3D, 0x40, 0x40, 0x40, 0x3D,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x48, 0x7E, 0x49, 0x43, 0x66,
	0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
	0xFF, 0x09, 0x29, 0xF6, 0x20,
	0xC0, 0x88, 0x7E, 0x09, 0x03,
	0x20, 0x54, 0x54, 0x79, 0x41,
	0x00, 0x00, 0x44, 0x7D, 0x41,
	0x30, 0x48, 0x48, 0x4A, 0x32,
	0x38, 0x40, 0x40, 0x22, 0x7A,
	0x00, 0x7A, 0x0A, 0x0A, 0x72,
	0x7D, 0x0D, 0x19, 0x31, 0x7D,
	0x26, 0x29, 0x29, 0x2F, 0x28,
	0x26, 0x29, 0x29, 0x29, 0x26,
	0x30, 0x48, 0x4D, 0x40, 0x20,
	0x38, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x38,
	0x2F, 0x10, 0xC8, 0xAC, 0xBA,
	0x2F, 0x10, 0x28, 0x34, 0xFA,
	0x00, 0x00, 0x7B, 0x00, 0x00,
	0x08, 0x14, 0x2A, 0x14, 0x22,
	0x22, 0x14, 0x2A, 0x14, 0x08,
	0x55, 0x00, 0x55, 0x00, 0x55,    // #176 (25% block) missing in old code
	0xAA, 0x55, 0xAA, 0x55, 0xAA,    // 50% block
	0xFF, 0x55, 0xFF, 0x55, 0xFF,    // 75% block
	0x00, 0x00, 0x00, 0xFF, 0x00,
	0x10, 0x10, 0x10, 0xFF, 0x00,
	0x14, 0x14, 0x14, 0xFF, 0x00,
	0x10, 0x10, 0xFF, 0x00, 0xFF,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x14, 0x14, 0x14, 0xFC, 0x00,
	0x14, 0x14, 0xF7, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x14, 0x14, 0xF4, 0x04, 0xFC,
	0x14, 0x14, 0x17, 0x10, 0x1F,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0x1F, 0x00,
	0x10, 0x10, 0x10, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0xF0, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0xFF, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x14,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x1F, 0x10, 0x17,
	0x00, 0x00, 0xFC, 0x04, 0xF4,
	0x14, 0x14, 0x17, 0x10, 0x17,
	0x14, 0x14, 0xF4, 0x04, 0xF4,
	0x00, 0x00, 0xFF, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x14, 0xF7, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x17, 0x14,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0xF4, 0x14,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x00, 0x00, 0x1F, 0x10, 0x1F,
	0x00, 0x00, 0x00, 0x1F, 0x14,
	0x00, 0x00, 0x00, 0xFC, 0x14,
	0x00, 0x00, 0xF0, 0x10, 0xF0,
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0x7C, 0x2A, 0x2A, 0x3E, 0x14,
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
	0x40, 0x7E, 0x20, 0x1E, 0x20,
	0x06, 0x02, 0x7E, 0x02, 0x02,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x1C, 0x2A, 0x49, 0x2A, 0x1C,
	0x4C, 0x72, 0x01, 0x72, 0x4C,
	0x30, 0x4A, 0x4D, 0x4D, 0x30,
	0x30, 0x48, 0x78, 0x48, 0x30,
	0xBC, 0x62, 0x5A, 0x46, 0x3D,
	0x3E, 0x49, 0x49, 0x49, 0x00,
	0x7E, 0x01, 0x01, 0x01, 0x7E,
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
	0x44, 0x44, 0x5F, 0x44, 0x44,
	0x40, 0x51, 0x4A, 0x44, 0x40,
	0x40, 0x44, 0x4A, 0x51, 0x40,
	0x00, 0x00, 0xFF, 0x01, 0x03,
	0xE0, 0x80, 0xFF, 0x00, 0x00,
	0x08, 0x08, 0x6B, 0x6B, 0x08,
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00
};

// Create a null default font in case some fonts not used (to prevent crash)
const  uint8_t widtbl_null[1] = {0};
PROGMEM const uint8_t chr_null[1] = {0};
PROGMEM const uint8_t* const chrtbl_null[1] = {chr_null};

// This is a structure to conveniently hold information on the default fonts
// Stores pointer to font character image address table, width table and height
typedef struct {
    const uint8_t *chartbl;
    const uint8_t *widthtbl;
    uint8_t height;
    uint8_t baseline;
} fontinfo;

// Now fill the structure
const PROGMEM fontinfo fontdata [] = {
   { (const uint8_t *)font, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 8, 7 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 }
};

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

typedef struct
{
  String  version = TFT_ESPI_VERSION;
  String  setup_info;  // Setup reference name available to use in a user setup
  uint32_t setup_id;   // ID available to use in a user setup
  int32_t esp;         // Processor code
  uint8_t trans;       // SPI transaction support
  uint8_t serial;      // Serial (SPI)
  #ifndef GENERIC_PROCESSOR
  uint8_t  port;       // SPI port
  #endif
  uint8_t overlap;     // ESP8266 overlap mode
  uint8_t interface;   // Interface type

  uint16_t tft_driver; // Hexadecimal code
  uint16_t tft_width;  // Rotation 0 width and height
  uint16_t tft_height;

  uint8_t r0_x_offset; // Display offsets, not all used yet
  uint8_t r0_y_offset;
  uint8_t r1_x_offset;
  uint8_t r1_y_offset;
  uint8_t r2_x_offset;
  uint8_t r2_y_offset;
  uint8_t r3_x_offset;
  uint8_t r3_y_offset;

  int8_t pin_tft_mosi; // SPI pins
  int8_t pin_tft_miso;
  int8_t pin_tft_clk;
  int8_t pin_tft_cs;

  int8_t pin_tft_dc;   // Control pins
  int8_t pin_tft_rd;
  int8_t pin_tft_wr;
  int8_t pin_tft_rst;

  int8_t pin_tft_led;
  int8_t pin_tft_led_on;

  int8_t pin_tch_cs;   // Touch chip select pin

  int16_t tft_spi_freq;// TFT write SPI frequency
  int16_t tft_rd_freq; // TFT read  SPI frequency
  int16_t tch_spi_freq;// Touch controller read/write SPI frequency
} setup_t;

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/

// Callback prototype for smooth font pixel colour read
typedef uint16_t (*getColorCallback)(uint16_t x, uint16_t y);

// Class functions and variables
class TFT_eSPI : public Print { friend class TFT_eSprite; // Sprite class has access to protected members

 //--------------------------------------- public ------------------------------------//
 public:
  void setSPISpeed(uint8_t speed_Mhz);
  TFT_eSPI(int16_t _W = TFT_WIDTH, int16_t _H = TFT_HEIGHT);
  void init();

  virtual void drawPixel(int32_t x, int32_t y, uint32_t color),
                   drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size),
                   drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color),
                   drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
                   drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),
                   fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

  virtual int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
                   drawChar(uint16_t uniCode, int32_t x, int32_t y),
                   height(void),
                   width(void);
  virtual uint16_t readPixel(int32_t x, int32_t y);
  virtual void     setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);
  virtual void     pushColor(uint16_t color);
  virtual void     begin_nin_write();
  virtual void     end_nin_write();

  void     setRotation(uint8_t r);
  uint8_t  getRotation();
  void     setOrigin(int32_t x, int32_t y);
  int32_t  getOriginX();
  int32_t  getOriginY();
  void     invertDisplay(bool i);
  void     setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h); // Note: start coordinates + width and height
  void     setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum = true);
  bool     checkViewport(int32_t x, int32_t y, int32_t w, int32_t h);
  int32_t  getViewportX();
  int32_t  getViewportY();
  int32_t  getViewportWidth();
  int32_t  getViewportHeight();
  bool     getViewportDatum();
  void     frameViewport(uint16_t color, int32_t w);
  void     resetViewport();
  bool     clipAddrWindow(int32_t* x, int32_t* y, int32_t* w, int32_t* h);
  bool     clipWindow(int32_t* xs, int32_t* ys, int32_t* xe, int32_t* ye);
  void     pushColor(uint16_t color, uint32_t len),  // Deprecated, use pushBlock()
           pushColors(uint16_t  *data, uint32_t len, bool swap = true), // With byte swap option
           pushColors(uint8_t  *data, uint32_t len); // Deprecated, use pushPixels()
  void     pushBlock(uint16_t color, uint32_t len);
  void     pushPixels(const void * data_in, uint32_t len);

           // Support for half duplex (bi-directional SDA) SPI bus where MOSI must be switched to input
           #ifdef TFT_SDA_READ
             #if defined (TFT_eSPI_ENABLE_8_BIT_READ)
  uint8_t  tft_Read_8(void);     // Read 8-bit value from TFT command register
             #endif
  void     begin_SDA_Read(void); // Begin a read on a half duplex (bi-directional SDA) SPI bus - sets MOSI to input
  void     end_SDA_Read(void);   // Restore MOSI to output
           #endif


  // Graphics drawing
  void     fillScreen(uint32_t color),
           drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color),
           drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color),
           fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);

  void     fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2);
  void     fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2);

  void     drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color),
           fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color),

           drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),
           fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),

           //                 Corner 1               Corner 2               Corner 3
           drawTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color),
           fillTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color);


  // Smooth (anti-aliased) graphics drawing
           // Draw a pixel blended with the background pixel colour (bg_color) specified,  return blended colour
           // If the bg_color is not specified, the background pixel colour will be read from TFT or sprite
  uint16_t drawPixel(int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color = 0x00FFFFFF);

           // Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
           // By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
           // Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
           // The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
  void     drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds = false);

           // As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
           // arc segments and ensures clean segment joints. 
           // The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
  void     drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool smoothArc = true);

           // Draw an anti-aliased filled circle at x, y with radius r
           // Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
           //       this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
  void     drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color);
  
           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color = 0x00FFFFFF);

           // Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
           // The outer corner radius is r, inner corner radius is ir
           // The inside and outside of the border are anti-aliased
  void     drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF, uint8_t quadrants = 0xF);

           // Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color, uint32_t bg_color = 0x00FFFFFF);

           // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);

           // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);
  void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);
  void     setSwapBytes(bool swap);
  bool     getSwapBytes();
  void     drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
           drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
           drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
           drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
           setBitmapColor(uint16_t fgcolor, uint16_t bgcolor); // Define the 2 colours for 1bpp sprites

  void     setPivot(int16_t x, int16_t y);
  int16_t  getPivotX(), // Get pivot x
           getPivotY(); // Get pivot y

  void     readRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
           // Write a block of pixels to the screen which have been read by readRect()
  void     pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);

           // These are used to render images or sprites stored in RAM arrays (used by Sprite class for 16bpp Sprites)
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transparent);

           // These are used to render images stored in FLASH (PROGMEM)
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transparent);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

           // These are used by Sprite class pushSprite() member function for 1, 4 and 8 bits per pixel (bpp) colours
           // They are not intended to be used with user sketches (but could be)
           // Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8 = true, uint16_t *cmap = nullptr);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8 = true, uint16_t *cmap = nullptr);
           // FLASH version
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap = nullptr);

           // Render a 16-bit colour image with a 1bpp mask
  void     pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask);

           // This next function has been used successfully to dump the TFT screen to a PC for documentation purposes
           // It reads a screen area and returns the 3 RGB 8-bit colour values of each pixel in the buffer
           // Set w and h to 1 to read 1 pixel's colour. The data buffer must be at least w * h * 3 bytes
  void     readRectRGB(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data);


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
  void     setCursor(int16_t x, int16_t y),                 // Set cursor for tft.print()
           setCursor(int16_t x, int16_t y, uint8_t font);   // Set cursor and font number for tft.print()

  int16_t  getCursorX(void),                                // Read current cursor x position (moves with tft.print())
           getCursorY(void);                                // Read current cursor y position

  void     setTextColor(uint16_t color),                    // Set character (glyph) color only (background not over-written)
           setTextColor(uint16_t fgcolor, uint16_t bgcolor, bool bgfill = false),  // Set character (glyph) foreground and background colour, optional background fill for smooth fonts
           setTextSize(uint8_t size);                       // Set character size multiplier (this increases pixel size)

  void     setTextWrap(bool wrapX, bool wrapY = false);     // Turn on/off wrapping of text in TFT width and/or height

  void     setTextDatum(uint8_t datum);                     // Set text datum position (default is top left), see Section 5 above
  uint8_t  getTextDatum(void);

  void     setTextPadding(uint16_t x_width);                // Set text padding (background blanking/over-write) width in pixels
  uint16_t getTextPadding(void);                            // Get text padding

  void     setFreeFont(uint8_t font),                       // Not used, historical fix to prevent an error
           setTextFont(uint8_t font);                       // Set the font number to use in future
  int16_t  textWidth(const char *string, uint8_t font),     // Returns pixel width of string in specified font
           textWidth(const char *string),                   // Returns pixel width of string in current font
           textWidth(const String& string, uint8_t font),   // As above for String types
           textWidth(const String& string),
           fontHeight(uint8_t font),                        // Returns pixel height of specified font
           fontHeight(void);                                // Returns pixel height of current font

           // Used by library and Smooth font class to extract Unicode point codes from a UTF8 encoded string
  uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining),
           decodeUTF8(uint8_t c);

           // Support function to UTF8 decode and draw characters piped through print stream
  size_t   write(uint8_t);
           // size_t   write(const uint8_t *buf, size_t len);

           // Used by Smooth font class to fetch a pixel colour for the anti-aliasing
  void     setCallback(getColorCallback getCol);

  uint16_t fontsLoaded(void); // Each bit in returned value represents a font type that is loaded - used for debug/error handling only


  // Low level read/write
  void     spiwrite(uint8_t);        // legacy support only
  void     writecommand(uint8_t c);  // Send an 8-bit command, function resets DC/RS high ready for data
  void     writedata(uint8_t d);     // Send data with DC/RS set high

  void     commandList(const uint8_t *addr); // Send a initialisation sequence to TFT stored in FLASH

  uint8_t  readcommand8( uint8_t cmd_function, uint8_t index = 0); // read 8 bits from TFT
  uint16_t readcommand16(uint8_t cmd_function, uint8_t index = 0); // read 16 bits from TFT
  uint32_t readcommand32(uint8_t cmd_function, uint8_t index = 0); // read 32 bits from TFT


  // Colour conversion
           // Convert 8-bit red, green and blue to 16 bits
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

           // Convert 8-bit colour to 16 bits
  uint16_t color8to16(uint8_t color332);
           // Convert 16-bit colour to 8 bits
  uint8_t  color16to8(uint16_t color565);

           // Convert 16-bit colour to/from 24-bit, R+G+B concatenated into LS 24 bits
  uint32_t color16to24(uint16_t color565);
  uint32_t color24to16(uint32_t color888);

           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

           // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24-bit colour alphaBlend with optional alpha dither
  uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);

  // Direct Memory Access (DMA) support functions
  // These can be used for SPI writes when using the ESP32 (original) or STM32 processors.
  // DMA also works on a RP2040 processor with PIO based SPI interfaces
           // Bear in mind DMA will only be of benefit in particular circumstances and can be tricky
           // to manage by noobs. The functions have however been designed to be noob friendly and
           // avoid a few DMA behaviour "gotchas".
           //
           // At best you will get a 2x TFT rendering performance improvement when using DMA because
           // this library handles the SPI bus so efficiently during normal (non DMA) transfers. The best
           // performance improvement scenario is the DMA transfer time is exactly the same as the time it
           // takes for the processor to prepare the next image buffer and initiate another DMA transfer.
           //
           // DMA transfer to the TFT is done while the processor moves on to handle other tasks. Bear
           // this in mind and watch out for "gotchas" like the image buffer going out of scope as the
           // processor leaves a function or its content being changed while the DMA engine is reading it.
           //
           // The compiler MAY change the implied scope of a buffer which has been set aside by creating
           // an array. For example a buffer defined before a "for-next" loop may get de-allocated when
           // the loop ends. To avoid this use, for example, malloc() and free() to take control of when
           // the buffer space is available and ensure it is not released until DMA is complete.
           //
           // Clearly you should not modify a buffer that is being DMA'ed to the TFT until the DMA is over.
           // Use the dmaBusy() function to check this.  Use tft.startWrite() before invoking DMA so the
           // TFT chip select stays low. If you use tft.endWrite() before DMA is complete then the endWrite
           // function will wait for the DMA to complete, so this may defeat any DMA performance benefit.
           //

  bool     initDMA(bool ctrl_cs = false);  // Initialise the DMA engine and attach to SPI bus - typically used in setup()
                                           // Parameter "true" enables DMA engine control of TFT chip select (ESP32 only)
                                           // For ESP32 only, TFT reads will not work if parameter is true
  void     deInitDMA(void);   // De-initialise the DMA engine and detach from SPI bus - typically not used

           // Push an image to the TFT using DMA, buffer is optional and grabs (double buffers) a copy of the image
           // Use the buffer if the image data will get over-written or destroyed while DMA is in progress
           //
           // Note 1: If swapping colour bytes is defined, and the double buffer option is NOT used, then the bytes
           // in the original image buffer content will be byte swapped by the function before DMA is initiated.
           //
           // Note 2: If part of the image will be off screen or outside of a set viewport, then the the original
           // image buffer content will be altered to a correctly clipped image before DMA is initiated.
           //
           // The function will wait for the last DMA to complete if it is called while a previous DMA is still
           // in progress, this simplifies the sketch and helps avoid "gotchas".
  void     pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t* data, uint16_t* buffer = nullptr);

#if defined (ESP32) // ESP32 only at the moment
           // For case where pointer is a const and the image data must not be modified (clipped or byte swapped)
  void     pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t const* data);
#endif
           // Push a block of pixels into a window set up using setAddrWindow()
  void     pushPixelsDMA(uint16_t* image, uint32_t len);

           // Check if the DMA is complete - use while(tft.dmaBusy); for a blocking wait
  bool     dmaBusy(void); // returns true if DMA is still in progress
  void     dmaWait(void); // wait until DMA is complete

  bool     DMA_Enabled = false;   // Flag for DMA enabled state
  uint8_t  spiBusyCheck = 0;      // Number of ESP32 transfer buffers to check

  // Bare metal functions
  void     startWrite(void);                         // Begin SPI transaction
  void     writeColor(uint16_t color, uint32_t len); // Deprecated, use pushBlock()
  void     endWrite(void);                           // End SPI transaction

  // Set/get an arbitrary library configuration attribute or option
  //       Use to switch ON/OFF capabilities such as UTF8 decoding - each attribute has a unique ID
  //       id = 0: reserved - may be used in future to reset all attributes to a default state
  //       id = 1: Turn on (a=true) or off (a=false) GLCD cp437 font character error correction
  //       id = 2: Turn on (a=true) or off (a=false) UTF8 decoding
           #define CP437_SWITCH 1
           #define UTF8_SWITCH  2
  void     setAttribute(uint8_t id = 0, uint8_t a = 0); // Set attribute value
  uint8_t  getAttribute(uint8_t id = 0);                // Get attribute value

           // Used for diagnostic sketch to see library setup adopted by compiler, see Section 7 above
  void     getSetup(setup_t& tft_settings); // Sketch provides the instance to populate
  bool     verifySetupID(uint32_t id);

  // Global variables
  static   SPIClass& getSPIinstance(void); // Get SPI class handle
  uint32_t textcolor, textbgcolor;         // Text foreground and background colours

  uint32_t bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

  uint8_t  textfont,  // Current selected font number
           textsize,  // Current font size multiplier
           textdatum, // Text reference datum
           rotation;  // Display rotation (0-3)

  uint8_t  decoderState = 0;   // UTF8 decoder state        - not for user access
  uint16_t decoderBuffer;      // Unicode code-point buffer - not for user access

  void     loadFont(const uint8_t array[]);
  void     loadFont(String fontName, fs::FS &ffs);
  void     loadFont(String fontName, bool flash = true);
  void     unloadFont( void );
  bool     getUnicodeIndex(uint16_t unicode, uint16_t *index);

  virtual void drawGlyph(uint16_t code);

  void     showFont(uint32_t td);  

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

  fontMetrics gFont = { nullptr, 0, 0, 0, 0, 0, 0, 0 };

  // These are for the metrics for each individual glyph (so we don't need to seek this in file and waste time)
  uint16_t* gUnicode = NULL;  //UTF-16 code, the codes are searched so do not need to be sequential
  uint8_t*  gHeight = NULL;   //cheight
  uint8_t*  gWidth = NULL;    //cwidth
  uint8_t*  gxAdvance = NULL; //setWidth
  int16_t*  gdY = NULL;       //topExtent
  int8_t*   gdX = NULL;       //leftExtent
  uint32_t* gBitmap = NULL;   //file pointer to greyscale bitmap

  bool     fontLoaded = false; // Flags when a anti-aliased font is loaded

  fs::File fontFile;
  fs::FS   &fontFS  = SPIFFS;
  bool     spiffs   = true;
  bool     fs_font = false;

 //--------------------------------------- private ------------------------------------//
 private:
           // Legacy begin and end prototypes - deprecated TODO: delete
  void     spi_begin();
  void     spi_end();

  void     spi_begin_read();
  void     spi_end_read();

           // New begin and end prototypes
           // begin/end a TFT write transaction
           // For SPI bus the transmit clock rate is set
  inline void begin_tft_write() __attribute__((always_inline));
  inline void end_tft_write()   __attribute__((always_inline));

           // begin/end a TFT read transaction
           // For SPI bus: begin lowers SPI clock rate, end reinstates transmit clock rate
  inline void begin_tft_read()  __attribute__((always_inline));
  inline void end_tft_read()    __attribute__((always_inline));

           // Initialise the data bus GPIO and hardware interfaces
  void     initBus(void);

           // Temporary  library development function  TODO: remove need for this
  void     pushSwapBytePixels(const void* data_in, uint32_t len);

           // Same as setAddrWindow but exits with CGRAM in read mode
  void     readAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h);

           // Byte read prototype
  uint8_t  readByte(void);

           // GPIO parallel bus input/output direction control
  void     busDir(uint32_t mask, uint8_t mode);

           // Single GPIO input/output direction control
  void     gpioMode(uint8_t gpio, uint8_t mode);

           // Smooth graphics helper
  uint8_t  sqrt_fraction(uint32_t num);

           // Helper function: calculate distance of a point from a finite length line between two points
  float    wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);

           // Display variant settings
  uint8_t  tabcolor,                   // ST7735 screen protector "tab" colour (now invalid)
           colstart = 0, rowstart = 0; // Screen display area to CGRAM area coordinate offsets

           // Port and pin masks for control signals (ESP826 only) - TODO: remove need for this
  volatile uint32_t *dcport, *csport;
  uint32_t cspinmask, dcpinmask, wrpinmask, sclkpinmask;

  //uint32_t lastColor = 0xFFFF; // Last colour - used to minimise bit shifting overhead

  getColorCallback getColor = nullptr; // Smooth font callback function pointer

  bool     locked, inTransaction, lockTransaction; // SPI transaction and mutex lock flags

  void     loadMetrics(void);
  uint32_t readInt32(void);

  uint8_t* fontPtr = nullptr;

 //-------------------------------------- protected ----------------------------------//
 protected:

uint8_t spi_write_speed;  // SPI write speed
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

  int32_t  cursor_x, cursor_y, padX;       // Text cursor x,y and padding setting
  int32_t  bg_cursor_x;                    // Background fill cursor
  int32_t  last_cursor_x;                  // Previous text cursor position when fill used

  uint32_t fontsloaded;               // Bit field of fonts loaded

  uint8_t  glyph_ab,   // Smooth font glyph delta Y (height) above baseline
           glyph_bb;   // Smooth font glyph delta Y (height) below baseline

  bool     isDigits;   // adjust bounding box for numbers to reduce visual jiggling
  bool     textwrapX, textwrapY;  // If set, 'wrap' text at right and optionally bottom edge of display
  bool     _swapBytes; // Swap the byte order for TFT pushImage()

  bool     _booted;    // init() or begin() has already run once

                       // User sketch manages these via set/getAttribute()
  bool     _cp437;        // If set, use correct CP437 charset (default is OFF)
  bool     _utf8;         // If set, use UTF-8 decoder in print stream 'write()' function (default ON)

  uint32_t _lastColor; // Buffered value of last colour used

  bool     _fillbg;    // Fill background flag (just for for smooth fonts at the moment)

 public:
           // Get raw x,y ADC values from touch controller
  uint8_t  getTouchRaw(uint16_t *x, uint16_t *y);
           // Get raw z (i.e. pressure) ADC value from touch controller
  uint16_t getTouchRawZ(void);
           // Convert raw x,y values to calibrated and correctly rotated screen coordinates
  void     convertRawXY(uint16_t *x, uint16_t *y);
           // Get the screen touch coordinates, returns true if screen has been touched
           // if the touch coordinates are off screen then x and y are not updated
           // The returned value can be treated as a bool type, false or 0 means touch not detected
           // In future the function may return an 8-bit "quality" (jitter) value.
           // The threshold value is optional, this must be higher than the bias level for z (pressure)
           // reported by Test_Touch_Controller when the screen is NOT touched. When touched the z value
           // must be higher than the threshold for a touch to be detected.
  uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);

           // Run screen calibration and test, report calibration values to the serial port
  void     calibrateTouch(uint16_t *data, uint32_t color_fg, uint32_t color_bg, uint8_t size);
           // Set the screen calibration values
  void     setTouch(uint16_t *data);

 private:
           // Legacy support only - deprecated TODO: delete
  void     spi_begin_touch();
  void     spi_end_touch();

           // Handlers for the touch controller bus settings
  inline void begin_touch_read_write() __attribute__((always_inline));
  inline void end_touch_read_write()   __attribute__((always_inline));

           // Private function to validate a touch, allow settle time and reduce spurious coordinates
  uint8_t  validTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);

           // Initialise with example calibration values so processor does not crash if setTouch() not called in setup()
  uint16_t touchCalibration_x0 = 300, touchCalibration_x1 = 3600, touchCalibration_y0 = 300, touchCalibration_y1 = 3600;
  uint8_t  touchCalibration_rotate = 1, touchCalibration_invert_x = 2, touchCalibration_invert_y = 0;

  uint32_t _pressTime;        // Press and hold time-out
  uint16_t _pressX, _pressY;  // For future use (last sampled calibrated coordinates)
};

class TFT_eSprite : public TFT_eSPI {
 public:
  explicit TFT_eSprite(TFT_eSPI *tft);
  ~TFT_eSprite(void);

           // Create a sprite of width x height pixels, return a pointer to the RAM area
           // Sketch can cast returned value to (uint16_t*) for 16-bit depth if needed
           // RAM required is:
           //  - 1 bit per pixel for 1 bit colour depth
           //  - 1 nibble per pixel for 4-bit colour (with palette table)
           //  - 1 byte per pixel for 8-bit colour (332 RGB format)
           //  - 2 bytes per pixel for 16-bit color depth (565 RGB format)
  void*    createSprite(int16_t width, int16_t height, uint8_t frames = 1);

           // Returns a pointer to the sprite or nullptr if not created, user must cast to pointer type
  void*    getPointer(void);

           // Returns true if sprite has been created
  bool     created(void);

           // Delete the sprite to free up the RAM
  void     deleteSprite(void);

           // Select the frame buffer for graphics write (for 2 colour ePaper and DMA toggle buffer)
           // Returns a pointer to the Sprite frame buffer
  void*    frameBuffer(int8_t f);
  
           // Set or get the colour depth to 1, 4, 8 or 16 bits. Can be used to change depth an existing
           // sprite, but clears it to black, returns a new pointer if sprite is re-created.
  void*    setColorDepth(int8_t b);
  int8_t   getColorDepth(void);

           // Set the palette for a 4-bit depth sprite.  Only the first 16 colours in the map are used.
  void     createPalette(uint16_t *palette = nullptr, uint8_t colors = 16);       // Palette in RAM
  void     createPalette(const uint16_t *palette = nullptr, uint8_t colors = 16); // Palette in FLASH

           // Set a single palette index to the given color
  void     setPaletteColor(uint8_t index, uint16_t color);

           // Get the color at the given palette index
  uint16_t getPaletteColor(uint8_t index);

           // Set foreground and background colours for 1 bit per pixel Sprite
  void     setBitmapColor(uint16_t fg, uint16_t bg);

           // Draw a single pixel at x,y
  void     drawPixel(int32_t x, int32_t y, uint32_t color);

           // Draw a single character in the GLCD or GFXFF font
  void     drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size),

           // Fill Sprite with a colour
           fillSprite(uint32_t color),

           // Define a window to push 16-bit colour pixels into in a raster order
           // Colours are converted to the set Sprite colour bit depth
           setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1),
           // Push a color (aka singe pixel) to the sprite's set window area
           pushColor(uint16_t color),
           // Push len colors (pixels) to the sprite's set window area
           pushColor(uint16_t color, uint32_t len),
           // Push a pixel pre-formatted as a 1, 4, 8 or 16-bit colour (avoids conversion overhead)
           writeColor(uint16_t color),

           // Set the scroll zone, top left corner at x,y with defined width and height
           // The colour (optional, black is default) is used to fill the gap after the scroll
           setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color = TFT_BLACK),
           // Scroll the defined zone dx,dy pixels. Negative values left,up, positive right,down
           // dy is optional (default is 0, so no up/down scroll).
           // The sprite coordinate frame does not move because pixels are moved
           scroll(int16_t dx, int16_t dy = 0),

           // Draw lines
           drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color),
           drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
           drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),

           // Fill a rectangular area with a color (aka draw a filled rectangle)
           fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

           // Set the coordinate rotation of the Sprite (for 1bpp Sprites only)
           // Note: this uses coordinate rotation and is primarily for ePaper which does not support
           // CGRAM rotation (like TFT drivers do) within the displays internal hardware
  void     setRotation(uint8_t rotation);
  uint8_t  getRotation(void);

           // Push a rotated copy of Sprite to TFT with optional transparent colour
  bool     pushRotated(int16_t angle, uint32_t transp = 0x00FFFFFF);
           // Push a rotated copy of Sprite to another different Sprite with optional transparent colour
  bool     pushRotated(TFT_eSprite *spr, int16_t angle, uint32_t transp = 0x00FFFFFF);

           // Get the TFT bounding box for a rotated copy of this Sprite
  bool     getRotatedBounds(int16_t angle, int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);
           // Get the destination Sprite bounding box for a rotated copy of this Sprite
  bool     getRotatedBounds(TFT_eSprite *spr, int16_t angle, int16_t *min_x, int16_t *min_y,
                                                             int16_t *max_x, int16_t *max_y);
           // Bounding box support function
  void     getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp, int16_t yp,
                            int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);

           // Read the colour of a pixel at x,y and return value in 565 format 
  uint16_t readPixel(int32_t x0, int32_t y0);

           // return the numerical value of the pixel at x,y (used when scrolling)
           // 16bpp = colour, 8bpp = byte, 4bpp = colour index, 1bpp = 1 or 0
  uint16_t readPixelValue(int32_t x, int32_t y);

           // Write an image (colour bitmap) to the sprite.
  void     pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, uint16_t *data, uint8_t sbpp = 0);
  void     pushImage(int32_t x0, int32_t y0, int32_t w, int32_t h, const uint16_t *data);

           // Push the sprite to the TFT screen, this fn calls pushImage() in the TFT class.
           // Optionally a "transparent" colour can be defined, pixels of that colour will not be rendered
  void     pushSprite(int32_t x, int32_t y);
  void     pushSprite(int32_t x, int32_t y, uint16_t transparent);

           // Push a windowed area of the sprite to the TFT at tx, ty
  bool     pushSprite(int32_t tx, int32_t ty, int32_t sx, int32_t sy, int32_t sw, int32_t sh);

           // Push the sprite to another sprite at x,y. This fn calls pushImage() in the destination sprite (dspr) class.
  bool     pushToSprite(TFT_eSprite *dspr, int32_t x, int32_t y);
  bool     pushToSprite(TFT_eSprite *dspr, int32_t x, int32_t y, uint16_t transparent);

           // Draw a single character in the selected font
  int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
           drawChar(uint16_t uniCode, int32_t x, int32_t y);

           // Return the width and height of the sprite
  int16_t  width(void),
           height(void);

           // Functions associated with anti-aliased fonts
           // Draw a single Unicode character using the loaded font
  void     drawGlyph(uint16_t code);
           // Print string to sprite using loaded font at cursor position
  void     printToSprite(String string);
           // Print char array to sprite using loaded font at cursor position
  void     printToSprite(char *cbuffer, uint16_t len);
           // Print indexed glyph to sprite using loaded font at x,y
  int16_t  printToSprite(int16_t x, int16_t y, uint16_t index);

 private:

  TFT_eSPI *_tft;

           // Reserve memory for the Sprite and return a pointer
  void*    callocSprite(int16_t width, int16_t height, uint8_t frames = 1);

           // Override the non-inlined TFT_eSPI functions
  void     begin_nin_write(void) { ; }
  void     end_nin_write(void) { ; }

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

  bool     _created; // A Sprite has been created and memory reserved
  bool     _gFont = false; 

  int32_t  _xs, _ys, _xe, _ye, _xptr, _yptr; // for setWindow
  int32_t  _sx, _sy; // x,y for scroll zone
  uint32_t _sw, _sh; // w,h for scroll zone
  uint32_t _scolor;  // gap fill colour for scroll zone

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