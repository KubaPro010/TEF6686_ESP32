#pragma once

#define ILI9341_DRIVER
#define TFT_DMA_SUPPORT
#define TFT_CS          5
#define TFT_DC          17
#define TFT_RST         16
#define TOUCH_CS	32

#define SPI_FREQUENCY       7500000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000
#define SMOOTH_FONT

#ifdef ARS
#define TFT_RGB_ORDER TFT_RGB
#endif