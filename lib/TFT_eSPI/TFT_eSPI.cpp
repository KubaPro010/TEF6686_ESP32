#include "TFT_eSPI.h"
#include <Arduino.h>
#include <SPI.h>

#if !defined (TFT_PARALLEL_8_BIT)
  #ifdef CONFIG_IDF_TARGET_ESP32
    SPIClass spi = SPIClass(VSPI);
  #else
    SPIClass& spi = SPI;
  #endif
#endif

#ifdef ESP32_DMA
  // DMA SPA handle
  spi_device_handle_t dmaHAL;
  #ifdef CONFIG_IDF_TARGET_ESP32
    #define DMA_CHANNEL 1
    #ifdef USE_HSPI_PORT
      spi_host_device_t spi_host = HSPI_HOST;
    #elif defined(USE_FSPI_PORT)
      spi_host_device_t spi_host = SPI_HOST;
    #else // use VSPI port
      spi_host_device_t spi_host = VSPI_HOST;
    #endif
  #else
    #ifdef USE_HSPI_PORT
      #define DMA_CHANNEL SPI_DMA_CH_AUTO
      spi_host_device_t spi_host = (spi_host_device_t) SPI3_HOST; // Draws once then freezes
    #else // use FSPI port
      #define DMA_CHANNEL SPI_DMA_CH_AUTO
      spi_host_device_t spi_host = (spi_host_device_t) SPI2_HOST; // Draws once then freezes
    #endif
  #endif
#endif

#if !defined (TFT_PARALLEL_8_BIT)
  // Volatile for register reads:
  volatile uint32_t* _spi_cmd       = (volatile uint32_t*)(SPI_CMD_REG(SPI_PORT));
  volatile uint32_t* _spi_user      = (volatile uint32_t*)(SPI_USER_REG(SPI_PORT));
  // Register writes only:
  volatile uint32_t* _spi_mosi_dlen = (volatile uint32_t*)(SPI_MOSI_DLEN_REG(SPI_PORT));
  volatile uint32_t* _spi_w         = (volatile uint32_t*)(SPI_W0_REG(SPI_PORT));
#endif

////////////////////////////////////////////////////////////////////////////////////////
#if defined (TFT_SDA_READ)
////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************************
** Function name:           beginSDA - VSPI port only, FPSI port only for S2
** Description:             Detach MOSI and attach MISO to SDA for reads
***************************************************************************************/
void TFT_eSPI::begin_SDA_Read(void)
{
  gpio_set_direction((gpio_num_t)TFT_MOSI, GPIO_MODE_INPUT);
  #ifdef CONFIG_IDF_TARGET_ESP32
    pinMatrixInAttach(TFT_MOSI, VSPIQ_IN_IDX, false);
  #else // S2
    pinMatrixInAttach(TFT_MOSI, FSPIQ_IN_IDX, false);
  #endif
  SET_BUS_READ_MODE;
}

/***************************************************************************************
** Function name:           endSDA - VSPI port only, FPSI port only for S2
** Description:             Attach MOSI to SDA and detach MISO for writes
***************************************************************************************/
void TFT_eSPI::end_SDA_Read(void)
{
  gpio_set_direction((gpio_num_t)TFT_MOSI, GPIO_MODE_OUTPUT);
  #ifdef CONFIG_IDF_TARGET_ESP32
    pinMatrixOutAttach(TFT_MOSI, VSPID_OUT_IDX, false, false);
  #else // S2
    pinMatrixOutAttach(TFT_MOSI, FSPID_OUT_IDX, false, false);
  #endif
  SET_BUS_WRITE_MODE;
}

////////////////////////////////////////////////////////////////////////////////////////
#endif // #if defined (TFT_SDA_READ)
////////////////////////////////////////////////////////////////////////////////////////


/***************************************************************************************
** Function name:           read byte  - supports class functions
** Description:             Read a byte from ESP32 8-bit data port
***************************************************************************************/
// Parallel bus MUST be set to input before calling this function!
uint8_t TFT_eSPI::readByte(void)
{
  uint8_t b = 0xAA;
  return b;
}

////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************************
** Function name:           pushBlock - for ESP32
** Description:             Write a block of pixels of the same colour
***************************************************************************************/
/*
void TFT_eSPI::pushBlock(uint16_t color, uint32_t len){

  uint32_t color32 = (color<<8 | color >>8)<<16 | (color<<8 | color >>8);
  bool empty = true;
  volatile uint32_t* spi_w = (volatile uint32_t*)_spi_w;
  if (len > 31)
  {
    *_spi_mosi_dlen =  511;
    spi_w[0]  = color32;
    spi_w[1]  = color32;
    spi_w[2]  = color32;
    spi_w[3]  = color32;
    spi_w[4]  = color32;
    spi_w[5]  = color32;
    spi_w[6]  = color32;
    spi_w[7]  = color32;
    spi_w[8]  = color32;
    spi_w[9]  = color32;
    spi_w[10] = color32;
    spi_w[11] = color32;
    spi_w[12] = color32;
    spi_w[13] = color32;
    spi_w[14] = color32;
    spi_w[15] = color32;
    while(len>31)
    {
      while ((*_spi_cmd)&SPI_USR);
      *_spi_cmd = SPI_USR;
      len -= 32;
    }
    empty = false;
  }

  if (len)
  {
    if(empty) {
      for (uint32_t i=0; i <= len; i+=2) *spi_w++ = color32;
    }
    len = (len << 4) - 1;
    while (*_spi_cmd&SPI_USR);
    *_spi_mosi_dlen = len;
    *_spi_cmd = SPI_USR;
  }
  while ((*_spi_cmd)&SPI_USR); // Move to later in code to use transmit time usefully?
}
//*/
//*
void TFT_eSPI::pushBlock(uint16_t color, uint32_t len){

  volatile uint32_t* spi_w = _spi_w;
  uint32_t color32 = (color<<8 | color >>8)<<16 | (color<<8 | color >>8);
  uint32_t i = 0;
  uint32_t rem = len & 0x1F;
  len =  len - rem;

  // Start with partial buffer pixels
  if (rem)
  {
    while (*_spi_cmd&SPI_USR);
    for (i=0; i < rem; i+=2) *spi_w++ = color32;
    *_spi_mosi_dlen = (rem << 4) - 1;
    *_spi_cmd = SPI_USR;
    if (!len) return; //{while (*_spi_cmd&SPI_USR); return; }
    i = i>>1; while(i++<16) *spi_w++ = color32;
  }

  while (*_spi_cmd&SPI_USR);
  if (!rem) while (i++<16) *spi_w++ = color32;
  *_spi_mosi_dlen =  511;

  // End with full buffer to maximise useful time for downstream code
  while(len)
  {
    while (*_spi_cmd&SPI_USR);
    *_spi_cmd = SPI_USR;
    len -= 32;
  }

  // Do not wait here
  //while (*_spi_cmd&SPI_USR);
}
//*/
/***************************************************************************************
** Function name:           pushSwapBytePixels - for ESP32
** Description:             Write a sequence of pixels with swapped bytes
***************************************************************************************/
void TFT_eSPI::pushSwapBytePixels(const void* data_in, uint32_t len){

  uint8_t* data = (uint8_t*)data_in;
  uint32_t color[16];

  if (len > 31)
  {
    WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
    while(len>31)
    {
      uint32_t i = 0;
      while(i<16)
      {
        color[i++] = DAT8TO32(data);
        data+=4;
      }
      while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  color[0]);
      WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  color[1]);
      WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  color[2]);
      WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  color[3]);
      WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  color[4]);
      WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  color[5]);
      WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  color[6]);
      WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  color[7]);
      WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  color[8]);
      WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  color[9]);
      WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), color[10]);
      WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), color[11]);
      WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), color[12]);
      WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), color[13]);
      WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), color[14]);
      WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), color[15]);
      SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      len -= 32;
    }
  }

  if (len > 15)
  {
    uint32_t i = 0;
    while(i<8)
    {
      color[i++] = DAT8TO32(data);
      data+=4;
    }
    while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
    WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 255);
    WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  color[0]);
    WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  color[1]);
    WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  color[2]);
    WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  color[3]);
    WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  color[4]);
    WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  color[5]);
    WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  color[6]);
    WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  color[7]);
    SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
    len -= 16;
  }

  if (len)
  {
    while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
    WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
    for (uint32_t i=0; i <= (len<<1); i+=4) {
      WRITE_PERI_REG(SPI_W0_REG(SPI_PORT)+i, DAT8TO32(data)); data+=4;
    }
    SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
  }
  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);

}

/***************************************************************************************
** Function name:           pushPixels - for ESP32
** Description:             Write a sequence of pixels
***************************************************************************************/
void TFT_eSPI::pushPixels(const void* data_in, uint32_t len){

  if(_swapBytes) {
    pushSwapBytePixels(data_in, len);
    return;
  }

  uint32_t *data = (uint32_t*)data_in;

  if (len > 31)
  {
    WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
    while(len>31)
    {
      while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  *data++);
      WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), *data++);
      WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), *data++);
      WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), *data++);
      WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), *data++);
      WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), *data++);
      WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), *data++);
      SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      len -= 32;
    }
  }

  if (len)
  {
    while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
    WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
    for (uint32_t i=0; i <= (len<<1); i+=4) WRITE_PERI_REG((SPI_W0_REG(SPI_PORT) + i), *data++);
    SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
  }
  while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
}
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
#if defined (ESP32_DMA) && !defined (TFT_PARALLEL_8_BIT) //       DMA FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************************
** Function name:           dmaBusy
** Description:             Check if DMA is busy
***************************************************************************************/
bool TFT_eSPI::dmaBusy(void)
{
  if (!DMA_Enabled || !spiBusyCheck) return false;

  spi_transaction_t *rtrans;
  esp_err_t ret;
  uint8_t checks = spiBusyCheck;
  for (int i = 0; i < checks; ++i)
  {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, 0);
    if (ret == ESP_OK) spiBusyCheck--;
  }

  //Serial.print("spiBusyCheck=");Serial.println(spiBusyCheck);
  if (spiBusyCheck ==0) return false;
  return true;
}


/***************************************************************************************
** Function name:           dmaWait
** Description:             Wait until DMA is over (blocking!)
***************************************************************************************/
void TFT_eSPI::dmaWait(void)
{
  if (!DMA_Enabled || !spiBusyCheck) return;
  spi_transaction_t *rtrans;
  esp_err_t ret;
  for (int i = 0; i < spiBusyCheck; ++i)
  {
    ret = spi_device_get_trans_result(dmaHAL, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
  }
  spiBusyCheck = 0;
}


/***************************************************************************************
** Function name:           pushPixelsDMA
** Description:             Push pixels to TFT (len must be less than 32767)
***************************************************************************************/
// This will byte swap the original image if setSwapBytes(true) was called by sketch.
void TFT_eSPI::pushPixelsDMA(uint16_t* image, uint32_t len)
{
  if ((len == 0) || (!DMA_Enabled)) return;

  dmaWait();

  if(_swapBytes) {
    for (uint32_t i = 0; i < len; i++) (image[i] = image[i] << 8 | image[i] >> 8);
  }

  esp_err_t ret;
  static spi_transaction_t trans;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;  //finally send the line data
  trans.length = len * 16;        //Data length, in bits
  trans.flags = 0;                //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
}


/***************************************************************************************
** Function name:           pushImageDMA
** Description:             Push image to a window (w*h must be less than 65536)
***************************************************************************************/
// Fixed const data assumed, will NOT clip or swap bytes
void TFT_eSPI::pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t const* image)
{
  if ((w == 0) || (h == 0) || (!DMA_Enabled)) return;

  uint32_t len = w*h;

  dmaWait();

  setAddrWindow(x, y, w, h);

  esp_err_t ret;
  static spi_transaction_t trans;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = image;   //Data pointer
  trans.length = len * 16;   //Data length, in bits
  trans.flags = 0;           //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
}


/***************************************************************************************
** Function name:           pushImageDMA
** Description:             Push image to a window (w*h must be less than 65536)
***************************************************************************************/
// This will clip and also swap bytes if setSwapBytes(true) was called by sketch
void TFT_eSPI::pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t* image, uint16_t* buffer)
{
  if ((x >= _vpW) || (y >= _vpH) || (!DMA_Enabled)) return;

  int32_t dx = 0;
  int32_t dy = 0;
  int32_t dw = w;
  int32_t dh = h;

  if (x < _vpX) { dx = _vpX - x; dw -= dx; x = _vpX; }
  if (y < _vpY) { dy = _vpY - y; dh -= dy; y = _vpY; }

  if ((x + dw) > _vpW ) dw = _vpW - x;
  if ((y + dh) > _vpH ) dh = _vpH - y;

  if (dw < 1 || dh < 1) return;

  uint32_t len = dw*dh;

  if (buffer == nullptr) {
    buffer = image;
    dmaWait();
  }

  // If image is clipped, copy pixels into a contiguous block
  if ( (dw != w) || (dh != h) ) {
    if(_swapBytes) {
      for (int32_t yb = 0; yb < dh; yb++) {
        for (int32_t xb = 0; xb < dw; xb++) {
          uint32_t src = xb + dx + w * (yb + dy);
          (buffer[xb + yb * dw] = image[src] << 8 | image[src] >> 8);
        }
      }
    }
    else {
      for (int32_t yb = 0; yb < dh; yb++) {
        memcpy((uint8_t*) (buffer + yb * dw), (uint8_t*) (image + dx + w * (yb + dy)), dw << 1);
      }
    }
  }
  // else, if a buffer pointer has been provided copy whole image to the buffer
  else if (buffer != image || _swapBytes) {
    if(_swapBytes) {
      for (uint32_t i = 0; i < len; i++) (buffer[i] = image[i] << 8 | image[i] >> 8);
    }
    else {
      memcpy(buffer, image, len*2);
    }
  }

  if (spiBusyCheck) dmaWait(); // In case we did not wait earlier

  setAddrWindow(x, y, dw, dh);

  esp_err_t ret;
  static spi_transaction_t trans;

  memset(&trans, 0, sizeof(spi_transaction_t));

  trans.user = (void *)1;
  trans.tx_buffer = buffer;  //finally send the line data
  trans.length = len * 16;   //Data length, in bits
  trans.flags = 0;           //SPI_TRANS_USE_TXDATA flag

  ret = spi_device_queue_trans(dmaHAL, &trans, portMAX_DELAY);
  assert(ret == ESP_OK);

  spiBusyCheck++;
}

////////////////////////////////////////////////////////////////////////////////////////
// Processor specific DMA initialisation
////////////////////////////////////////////////////////////////////////////////////////

// The DMA functions here work with SPI only (not parallel)
/***************************************************************************************
** Function name:           dc_callback
** Description:             Toggles DC line during transaction
***************************************************************************************/
extern "C" void dc_callback();

void IRAM_ATTR dc_callback(spi_transaction_t *spi_tx)
{
  if ((bool)spi_tx->user) {DC_D;}
  else {DC_C;}
}

/***************************************************************************************
** Function name:           dma_end_callback
** Description:             Clear DMA run flag to stop retransmission loop
***************************************************************************************/
extern "C" void dma_end_callback();

void IRAM_ATTR dma_end_callback(spi_transaction_t *spi_tx)
{
  WRITE_PERI_REG(SPI_DMA_CONF_REG(spi_host), 0);
}

/***************************************************************************************
** Function name:           initDMA
** Description:             Initialise the DMA engine - returns true if init OK
***************************************************************************************/
bool TFT_eSPI::initDMA(bool ctrl_cs)
{
  if (DMA_Enabled) return false;

  esp_err_t ret;
  spi_bus_config_t buscfg = {
    .mosi_io_num = TFT_MOSI,
    .miso_io_num = TFT_MISO,
    .sclk_io_num = TFT_SCLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .data4_io_num = -1,
    .data5_io_num = -1,
    .data6_io_num = -1,
    .data7_io_num = -1,
    .max_transfer_sz = TFT_WIDTH * TFT_HEIGHT * 2 + 8, // TFT screen size
    .flags = 0,
    .intr_flags = 0
  };

  int8_t pin = -1;
  if (ctrl_cs) pin = TFT_CS;

  spi_device_interface_config_t devcfg = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = TFT_SPI_MODE,
    .duty_cycle_pos = 0,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = SPI_FREQUENCY,
    .input_delay_ns = 0,
    .spics_io_num = pin,
    .flags = SPI_DEVICE_NO_DUMMY, //0,
    .queue_size = 1,
    .pre_cb = 0, //dc_callback, //Callback to handle D/C line
    #ifdef CONFIG_IDF_TARGET_ESP32
      .post_cb = 0
    #else
      .post_cb = dma_end_callback
    #endif
  };
  ret = spi_bus_initialize(spi_host, &buscfg, DMA_CHANNEL);
  ESP_ERROR_CHECK(ret);
  ret = spi_bus_add_device(spi_host, &devcfg, &dmaHAL);
  ESP_ERROR_CHECK(ret);

  DMA_Enabled = true;
  spiBusyCheck = 0;
  return true;
}

/***************************************************************************************
** Function name:           deInitDMA
** Description:             Disconnect the DMA engine from SPI
***************************************************************************************/
void TFT_eSPI::deInitDMA(void)
{
  if (!DMA_Enabled) return;
  spi_bus_remove_device(dmaHAL);
  spi_bus_free(spi_host);
  DMA_Enabled = false;
}

////////////////////////////////////////////////////////////////////////////////////////
#endif // End of DMA FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

#ifndef SPI_BUSY_CHECK
  #define SPI_BUSY_CHECK
#endif

// Clipping macro for pushImage
#define PI_CLIP                                        \
  if (_vpOoB) return;                                  \
  x+= _xDatum;                                         \
  y+= _yDatum;                                         \
                                                       \
  if ((x >= _vpW) || (y >= _vpH)) return;              \
                                                       \
  int32_t dx = 0;                                      \
  int32_t dy = 0;                                      \
  int32_t dw = w;                                      \
  int32_t dh = h;                                      \
                                                       \
  if (x < _vpX) { dx = _vpX - x; dw -= dx; x = _vpX; } \
  if (y < _vpY) { dy = _vpY - y; dh -= dy; y = _vpY; } \
                                                       \
  if ((x + dw) > _vpW ) dw = _vpW - x;                 \
  if ((y + dh) > _vpH ) dh = _vpH - y;                 \
                                                       \
  if (dw < 1 || dh < 1) return;


inline void TFT_eSPI::begin_tft_write(void){
  if (locked) {
    locked = false; // Flag to show SPI access now unlocked
    spi.beginTransaction(SPISettings(spi_write_speed * 1000000, MSBFIRST, TFT_SPI_MODE));
    CS_L;
    SET_BUS_WRITE_MODE;
  }
}

void TFT_eSPI::begin_nin_write(void){
  if (locked) {
    locked = false; // Flag to show SPI access now unlocked
    spi.beginTransaction(SPISettings(spi_write_speed * 1000000, MSBFIRST, TFT_SPI_MODE));
    CS_L;
    SET_BUS_WRITE_MODE;
  }
}

inline void TFT_eSPI::end_tft_write(void){
  if(!inTransaction) {      // Flag to stop ending transaction during multiple graphics calls
    if (!locked) {          // Locked when beginTransaction has been called
      locked = true;        // Flag to show SPI access now locked
      SPI_BUSY_CHECK;       // Check send complete and clean out unused rx data
      CS_H;
      SET_BUS_READ_MODE;    // In case bus has been configured for tx only
      spi.endTransaction();
    }
  }
}

inline void TFT_eSPI::end_nin_write() {
  if(!inTransaction) {      // Flag to stop ending transaction during multiple graphics calls
    if (!locked) {          // Locked when beginTransaction has been called
      locked = true;        // Flag to show SPI access now locked
      SPI_BUSY_CHECK;       // Check send complete and clean out unused rx data
      CS_H;
      SET_BUS_READ_MODE;    // In case SPI has been configured for tx only
      spi.endTransaction();
    }
  }
}

inline void TFT_eSPI::begin_tft_read() {
  dmaWait();
  if (locked) {
    locked = false;
    spi.beginTransaction(SPISettings(SPI_READ_FREQUENCY, MSBFIRST, TFT_SPI_MODE));
    CS_L;
  }
  SET_BUS_READ_MODE;
}

inline void TFT_eSPI::end_tft_read(void){
  if(!inTransaction) {
    if (!locked) {
      locked = true;
      CS_H;
      spi.endTransaction();
    }
  }
  SET_BUS_WRITE_MODE;
}

void TFT_eSPI::setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum)
{
  // Viewport metrics (not clipped)
  _xDatum  = x; // Datum x position in screen coordinates
  _yDatum  = y; // Datum y position in screen coordinates
  _xWidth  = w; // Viewport width
  _yHeight = h; // Viewport height

  // Full size default viewport
  _vpDatum = false; // Datum is at top left corner of screen (true = top left of viewport)
  _vpOoB   = false; // Out of Bounds flag (true is all of viewport is off screen)
  _vpX = 0;         // Viewport top left corner x coordinate
  _vpY = 0;         // Viewport top left corner y coordinate
  _vpW = width();   // Equivalent of TFT width  (Nb: viewport right edge coord + 1)
  _vpH = height();  // Equivalent of TFT height (Nb: viewport bottom edge coord + 1)

  // Clip viewport to screen area
  if (x<0) { w += x; x = 0; }
  if (y<0) { h += y; y = 0; }
  if ((x + w) > width() ) { w = width()  - x; }
  if ((y + h) > height() ) { h = height() - y; }

  // Check if viewport is entirely out of bounds
  if (w < 1 || h < 1) {
    // Set default values and Out of Bounds flag in case of error
    _xDatum = 0;
    _yDatum = 0;
    _xWidth  = width();
    _yHeight = height();
    _vpOoB = true;      // Set Out of Bounds flag to inhibit all drawing
    return;
  }

  if (!vpDatum) {
    _xDatum = 0; // Reset to top left of screen if not using a viewport datum
    _yDatum = 0;
    _xWidth  = width();
    _yHeight = height();
  }

  // Store the clipped screen viewport metrics and datum position
  _vpX = x;
  _vpY = y;
  _vpW = x + w;
  _vpH = y + h;
  _vpDatum = vpDatum;
}

bool TFT_eSPI::checkViewport(int32_t x, int32_t y, int32_t w, int32_t h) {
  if (_vpOoB) return false;
  x+= _xDatum;
  y+= _yDatum;

  if ((x >= _vpW) || (y >= _vpH)) return false;

  int32_t dx = 0;
  int32_t dy = 0;
  int32_t dw = w;
  int32_t dh = h;

  if (x < _vpX) { dx = _vpX - x; dw -= dx; x = _vpX; }
  if (y < _vpY) { dy = _vpY - y; dh -= dy; y = _vpY; }

  if ((x + dw) > _vpW ) dw = _vpW - x;
  if ((y + dh) > _vpH ) dh = _vpH - y;

  if (dw < 1 || dh < 1) return false;

  return true;
}

void TFT_eSPI::setSPISpeed(uint8_t speed) {
  if(speed > 0 && speed < 80) spi_write_speed = speed;
}

void TFT_eSPI::resetViewport() {
  _vpDatum = _vpOoB = false;
  _xDatum = _yDatum = _vpX = _vpY = 0;
  _vpW = _xWidth = width();
  _vpH = _yHeight = height();
}

int32_t  TFT_eSPI::getViewportX() {
  return _xDatum;
}

int32_t  TFT_eSPI::getViewportY() {
  return _yDatum;
}

int32_t TFT_eSPI::getViewportWidth() {
  return _xWidth;
}

int32_t TFT_eSPI::getViewportHeight() {
  return _yHeight;
}

bool TFT_eSPI::getViewportDatum() {
  return _vpDatum;
}

void TFT_eSPI::frameViewport(uint16_t color, int32_t w)
{
  // Save datum position
  bool _dT = _vpDatum;

  // If w is positive the frame is drawn inside the viewport
  // a large positive width will clear the screen inside the viewport
  if (w>0) {
    // Set vpDatum true to simplify coordinate derivation
    _vpDatum = true;
    fillRect(0, 0, _vpW - _vpX, w, color);                // Top
    fillRect(0, w, w, _vpH - _vpY - w - w, color);        // Left
    fillRect(_xWidth - w, w, w, _yHeight - w - w, color); // Right
    fillRect(0, _yHeight - w, _xWidth, w, color);         // Bottom
  }
  else {
    w = -w;

    // Save old values
    int32_t _xT = _vpX; _vpX = 0;
    int32_t _yT = _vpY; _vpY = 0;
    int32_t _wT = _vpW;
    int32_t _hT = _vpH;

    // Set vpDatum false so frame can be drawn outside window
    _vpDatum = false; // When false the full width and height is accessed
    _vpH = height();
    _vpW = width();

    // Draw frame
    fillRect(_xT - w - _xDatum, _yT - w - _yDatum, _wT - _xT + w + w, w, color); // Top
    fillRect(_xT - w - _xDatum, _yT - _yDatum, w, _hT - _yT, color);             // Left
    fillRect(_wT - _xDatum, _yT - _yDatum, w, _hT - _yT, color);                 // Right
    fillRect(_xT - w - _xDatum, _hT - _yDatum, _wT - _xT + w + w, w, color);     // Bottom

    // Restore old values
    _vpX = _xT;
    _vpY = _yT;
    _vpW = _wT;
    _vpH = _hT;
  }

  // Restore vpDatum
  _vpDatum = _dT;
}

bool TFT_eSPI::clipAddrWindow(int32_t *x, int32_t *y, int32_t *w, int32_t *h)
{
  if (_vpOoB) return false; // Area is outside of viewport

  *x+= _xDatum;
  *y+= _yDatum;

  if ((*x >= _vpW) || (*y >= _vpH)) return false;  // Area is outside of viewport

  // Crop drawing area bounds
  if (*x < _vpX) { *w -= _vpX - *x; *x = _vpX; }
  if (*y < _vpY) { *h -= _vpY - *y; *y = _vpY; }

  if ((*x + *w) > _vpW ) *w = _vpW - *x;
  if ((*y + *h) > _vpH ) *h = _vpH - *y;

  if (*w < 1 || *h < 1) return false; // No area is inside viewport

  return true;  // Area is wholly or partially inside viewport
}

bool TFT_eSPI::clipWindow(int32_t *xs, int32_t *ys, int32_t *xe, int32_t *ye)
{
  if (_vpOoB) return false; // Area is outside of viewport

  *xs+= _xDatum;
  *ys+= _yDatum;
  *xe+= _xDatum;
  *ye+= _yDatum;

  if ((*xs >= _vpW) || (*ys >= _vpH)) return false;  // Area is outside of viewport
  if ((*xe <  _vpX) || (*ye <  _vpY)) return false;  // Area is outside of viewport

  // Crop drawing area bounds
  if (*xs < _vpX) *xs = _vpX;
  if (*ys < _vpY) *ys = _vpY;

  if (*xe > _vpW) *xe = _vpW - 1;
  if (*ye > _vpH) *ye = _vpH - 1;

  return true;  // Area is wholly or partially inside viewport
}

TFT_eSPI::TFT_eSPI(int16_t w, int16_t h)
{
  _init_width  = _width  = w;
  _init_height = _height = h;

  resetViewport();

  rotation = padX = 0;
  cursor_y = cursor_x = last_cursor_x = bg_cursor_x = 0;
  textfont = textsize = 1;
  textcolor = bitmap_fg = 0xFFFF;
  textbgcolor = bitmap_bg = 0x0000;

  _fillbg = isDigits = textwrapY = false;
  textwrapX = true;
  textdatum = TL_DATUM;
  fontsloaded = 0;

  _swapBytes = false;

  locked = true;
  inTransaction = false;
  lockTransaction = false;

  _booted = true;
  _cp437 = false;
  _utf8 = true;

  fs_font = true;

  addr_row = 0xFFFF;
  addr_col = 0xFFFF;

  _xPivot = 0;
  _yPivot = 0;

  cspinmask = 0;
  dcpinmask = 0;
  wrpinmask = 0;
  sclkpinmask = 0;

  fontsloaded = 0x0002; // Bit 1 set
  fontsloaded |= 0x8000; // Bit 15 set
}

void TFT_eSPI::initBus(void) {

#ifdef TFT_CS
  if (TFT_CS >= 0) {
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH); // Chip select high (inactive)
  }
#endif

if (TOUCH_CS >= 0) {
  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(TOUCH_CS, HIGH); // Chip select high (inactive)
}

#if defined (TFT_WR)
  if (TFT_WR >= 0) {
    pinMode(TFT_WR, OUTPUT);
    digitalWrite(TFT_WR, HIGH); // Set write strobe high (inactive)
  }
#endif

#ifdef TFT_DC
  if (TFT_DC >= 0) {
    pinMode(TFT_DC, OUTPUT);
    digitalWrite(TFT_DC, HIGH); // Data/Command high = data mode
  }
#endif

#ifdef TFT_RST
  if (TFT_RST >= 0) {
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH); // Set high, do not share pin with another SPI device
  }
#endif
}

void TFT_eSPI::init()
{
  if (_booted) {
    initBus();

    #if defined (TFT_MOSI) && !defined (TFT_SPI_OVERLAP)
      spi.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, -1); // This will set MISO to input
    #else
      spi.begin(); // This will set MISO to input
    #endif
    lockTransaction = false;
    inTransaction = false;
    locked = true;


#if defined (TFT_CS)
  // Set to output once again in case MISO is used for CS
  if (TFT_CS >= 0) {
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH); // Chip select high (inactive)
  }
#endif


  // Set to output once again in case MISO is used for DC
#if defined (TFT_DC)
  if (TFT_DC >= 0) {
    pinMode(TFT_DC, OUTPUT);
    digitalWrite(TFT_DC, HIGH); // Data/Command high = data mode
  }
#endif

    _booted = false;
    end_tft_write();
  } // end of: if just _booted

  // Toggle RST low to reset
#ifdef TFT_RST
  // Set to output once again in case MISO is used for TFT_RST
  if (TFT_RST >= 0) {
    pinMode(TFT_RST, OUTPUT);
  }
  if (TFT_RST >= 0) {
    writecommand(0x00); // Put SPI bus in known state for TFT with CS tied low
    digitalWrite(TFT_RST, HIGH);
    delay(5);
    digitalWrite(TFT_RST, LOW);
    delay(20);
    digitalWrite(TFT_RST, HIGH);
  }
  else writecommand(TFT_SWRST); // Software reset
#else
  writecommand(TFT_SWRST); // Software reset
#endif

  delay(150); // Wait for reset to complete

  begin_tft_write();

  #include "ILI9341_Init.h"

#ifdef TFT_INVERSION_ON
  writecommand(TFT_INVON);
#endif

#ifdef TFT_INVERSION_OFF
  writecommand(TFT_INVOFF);
#endif

  end_tft_write();

  setRotation(rotation);

#if defined (TFT_BL) && defined (TFT_BACKLIGHT_ON)
  if (TFT_BL >= 0) {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
  }
#else
  #if defined (TFT_BL) && defined (M5STACK)
    // Turn on the back-light LED
    if (TFT_BL >= 0) {
      pinMode(TFT_BL, OUTPUT);
      digitalWrite(TFT_BL, HIGH);
    }
  #endif
#endif
}

void TFT_eSPI::setRotation(uint8_t m) {
  begin_tft_write();

  #include "ILI9341_Rotation.h"

  delayMicroseconds(10);

  end_tft_write();

  addr_row = 0xFFFF;
  addr_col = 0xFFFF;

  resetViewport();
}

uint8_t TFT_eSPI::getRotation() {
  return rotation;
}

void TFT_eSPI::setOrigin(int32_t x, int32_t y)
{
  _xDatum = x;
  _yDatum = y;
}

int32_t TFT_eSPI::getOriginX() {
  return _xDatum;
}

int32_t TFT_eSPI::getOriginY() {
  return _yDatum;
}


void TFT_eSPI::commandList (const uint8_t *addr)
{
  uint8_t numCommands = pgm_read_byte(addr++), numArgs, ms;
  while (numCommands--) {
    writecommand(pgm_read_byte(addr++)); // Read, issue command
    numArgs = pgm_read_byte(addr++);     // Number of args to follow
    ms = numArgs & TFT_INIT_DELAY;       // If high bit set, delay follows args
    numArgs &= ~TFT_INIT_DELAY;          // Mask out delay bit

    while (numArgs--) {
      writedata(pgm_read_byte(addr++));  // Read, issue argument
    }

    if (ms) {
      ms = pgm_read_byte(addr++);        // Read post-command delay time (ms)
      delay( (ms==255 ? 500 : ms) );
    }
  }

}

void TFT_eSPI::spiwrite(uint8_t c)
{
  begin_tft_write();
  tft_Write_8(c);
  end_tft_write();
}

void TFT_eSPI::writecommand(uint8_t c)
{
  begin_tft_write();

  DC_C;

  tft_Write_8(c);

  DC_D;

  end_tft_write();
}

void TFT_eSPI::writedata(uint8_t d)
{
  begin_tft_write();

  DC_D;        // Play safe, but should already be in data mode

  tft_Write_8(d);

  CS_L;        // Allow more hold time for low VDI rail

  end_tft_write();
}


/***************************************************************************************
** Function name:           readcommand8
** Description:             Read a 8-bit data value from an indexed command register
***************************************************************************************/
uint8_t TFT_eSPI::readcommand8(uint8_t cmd_function, uint8_t index)
{
  uint8_t reg = 0;
  // Tested with ILI9341 set to Interface II i.e. IM [3:0] = "1101"
  begin_tft_read();
  index = 0x10 + (index & 0x0F);

  DC_C; tft_Write_8(0xD9);
  DC_D; tft_Write_8(index);

  CS_H; // Some displays seem to need CS to be pulsed here, or is just a delay needed?
  CS_L;

  DC_C; tft_Write_8(cmd_function);
  DC_D;
  reg = tft_Read_8();

  end_tft_read();
  return reg;
}

uint16_t TFT_eSPI::readcommand16(uint8_t cmd_function, uint8_t index) {
  uint32_t reg;

  reg  = (readcommand8(cmd_function, index + 0) <<  8);
  reg |= (readcommand8(cmd_function, index + 1) <<  0);

  return reg;
}


uint32_t TFT_eSPI::readcommand32(uint8_t cmd_function, uint8_t index) {
  uint32_t reg;

  reg  = ((uint32_t)readcommand8(cmd_function, index + 0) << 24);
  reg |= ((uint32_t)readcommand8(cmd_function, index + 1) << 16);
  reg |= ((uint32_t)readcommand8(cmd_function, index + 2) <<  8);
  reg |= ((uint32_t)readcommand8(cmd_function, index + 3) <<  0);

  return reg;
}

uint16_t TFT_eSPI::readPixel(int32_t x0, int32_t y0)
{
  if (_vpOoB) return 0;

  x0+= _xDatum;
  y0+= _yDatum;

  // Range checking
  if ((x0 < _vpX) || (y0 < _vpY) ||(x0 >= _vpW) || (y0 >= _vpH)) return 0;
  // This function can get called during anti-aliased font rendering
  // so a transaction may be in progress
  bool wasInTransaction = inTransaction;
  if (inTransaction) { inTransaction= false; end_tft_write();}

  uint16_t color = 0;

  begin_tft_read(); // Sets CS low

  readAddrWindow(x0, y0, 1, 1);

  #ifdef TFT_SDA_READ
    begin_SDA_Read();
  #endif

  // Dummy read to throw away don't care value
  tft_Read_8();

    // Read the 3 RGB bytes, colour is actually only in the top 6 bits of each byte
    // as the TFT stores colours as 18 bits
    uint8_t r = tft_Read_8();
    uint8_t g = tft_Read_8();
    uint8_t b = tft_Read_8();
    color = color565(r, g, b);
  CS_H;

  #ifdef TFT_SDA_READ
    end_SDA_Read();
  #endif

  end_tft_read();

  // Reinstate the transaction if one was in progress
  if(wasInTransaction) { begin_tft_write(); inTransaction = true; }

  return color;
}

void TFT_eSPI::setCallback(getColorCallback getCol) {
  getColor = getCol;
}

void TFT_eSPI::readRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
  PI_CLIP ;

  // This function can get called after a begin_tft_write
  // so a transaction may be in progress
  bool wasInTransaction = inTransaction;
  if (inTransaction) { inTransaction= false; end_tft_write();}

  uint16_t color = 0;

  begin_tft_read();

  readAddrWindow(x, y, dw, dh);

  data += dx + dy * w;

  #ifdef TFT_SDA_READ
    begin_SDA_Read();
  #endif

  // Dummy read to throw away don't care value
  tft_Read_8();

  // Read window pixel 24-bit RGB values
  while (dh--) {
    int32_t lw = dw;
    uint16_t* line = data;
    while (lw--) {

      // Read the 3 RGB bytes, colour is actually only in the top 6 bits of each byte
      // as the TFT stores colours as 18 bits
      uint8_t r = tft_Read_8();
      uint8_t g = tft_Read_8();
      uint8_t b = tft_Read_8();
      color = color565(r, g, b);

      // Swapped colour byte order for compatibility with pushRect()
      *line++ = color << 8 | color >> 8;
    }
    data += w;
  }

  //CS_H;

  #ifdef TFT_SDA_READ
    end_SDA_Read();
  #endif

  end_tft_read();

  // Reinstate the transaction if one was in progress
  if(wasInTransaction) { begin_tft_write(); inTransaction = true; }
}

void TFT_eSPI::pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
  bool swap = _swapBytes; _swapBytes = false;
  pushImage(x, y, w, h, data);
  _swapBytes = swap;
}

void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;

  setWindow(x, y, x + dw - 1, y + dh - 1);

  data += dx + dy * w;

  // Check if whole image can be pushed
  if (dw == w) pushPixels(data, dw * dh);
  else {
    // Push line segments to crop image
    while (dh--) {
      pushPixels(data, dw);
      data += w;
    }
  }

  inTransaction = lockTransaction;
  end_tft_write();
}

void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transp)
{
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;

  data += dx + dy * w;


  uint16_t  lineBuf[dw]; // Use buffer to minimise setWindow call count

  // The little endian transp color must be byte swapped if the image is big endian
  if (!_swapBytes) transp = transp >> 8 | transp << 8;

  while (dh--)
  {
    int32_t len = dw;
    uint16_t* ptr = data;
    int32_t px = x, sx = x;
    bool move = true;
    uint16_t np = 0;

    while (len--) {
      if (transp != *ptr) {
        if (move) { move = false; sx = px; }
        lineBuf[np] = *ptr;
        np++;
      } else {
        move = true;
        if (np) {
          setWindow(sx, y, sx + np - 1, y);
          pushPixels((uint16_t*)lineBuf, np);
          np = 0;
        }
      }
      px++;
      ptr++;
    }
    if (np) { setWindow(sx, y, sx + np - 1, y); pushPixels((uint16_t*)lineBuf, np); }

    y++;
    data += w;
  }

  inTransaction = lockTransaction;
  end_tft_write();
}

void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data)
{
  // Requires 32-bit aligned access, so use PROGMEM 16-bit word functions
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;

  data += dx + dy * w;

  uint16_t  buffer[dw];

  setWindow(x, y, x + dw - 1, y + dh - 1);

  // Fill and send line buffers to TFT
  for (int32_t i = 0; i < dh; i++) {
    for (int32_t j = 0; j < dw; j++) buffer[j] = pgm_read_word(&data[i * w + j]);
    pushPixels(buffer, dw);
  }

  inTransaction = lockTransaction;
  end_tft_write();
}

void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transp)
{
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;

  data += dx + dy * w;

  uint16_t  lineBuf[dw];

  if (!_swapBytes) transp = transp >> 8 | transp << 8;

  while (dh--) {
    int32_t len = dw;
    uint16_t* ptr = (uint16_t*)data;
    int32_t px = x, sx = x;
    bool move = true;

    uint16_t np = 0;

    while (len--) {
      uint16_t color = pgm_read_word(ptr);
      if (transp != color) {
        if (move) { move = false; sx = px; }
        lineBuf[np] = color;
        np++;
      }
      else {
        move = true;
        if (np) {
          setWindow(sx, y, sx + np - 1, y);
          pushPixels(lineBuf, np);
          np = 0;
        }
      }
      px++;
      ptr++;
    }
    if (np) { setWindow(sx, y, sx + np - 1, y); pushPixels(lineBuf, np); }

    y++;
    data += w;
  }

  inTransaction = lockTransaction;
  end_tft_write();
}

/***************************************************************************************
** Function name:           pushImage
** Description:             plot 8-bit or 4-bit or 1 bit image or sprite using a line buffer
***************************************************************************************/
void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap)
{
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;
  bool swap = _swapBytes;

  setWindow(x, y, x + dw - 1, y + dh - 1); // Sets CS low and sent RAMWR

  // Line buffer makes plotting faster
  uint16_t  lineBuf[dw];

  if (bpp8) {
    _swapBytes = false;

    uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table

    _lastColor = -1; // Set to illegal value

    // Used to store last shifted colour
    uint8_t msbColor = 0;
    uint8_t lsbColor = 0;

    data += dx + dy * w;
    while (dh--) {
      uint32_t len = dw;
      uint8_t* ptr = (uint8_t*)data;
      uint8_t* linePtr = (uint8_t*)lineBuf;

      while(len--) {
        uint32_t color = pgm_read_byte(ptr++);

        // Shifts are slow so check if colour has changed first
        if (color != _lastColor) {
          //          =====Green=====     ===============Red==============
          msbColor = (color & 0x1C)>>2 | (color & 0xC0)>>3 | (color & 0xE0);
          //          =====Green=====    =======Blue======
          lsbColor = (color & 0x1C)<<3 | blue[color & 0x03];
          _lastColor = color;
        }

       *linePtr++ = msbColor;
       *linePtr++ = lsbColor;
      }

      pushPixels(lineBuf, dw);

      data += w;
    }
    _swapBytes = swap; // Restore old value
  }
  else if (cmap != nullptr) // Must be 4bpp
  {
    _swapBytes = true;

    w = (w+1) & 0xFFFE;   // if this is a sprite, w will already be even; this does no harm.
    bool splitFirst = (dx & 0x01) != 0; // split first means we have to push a single px from the left of the sprite / image

    if (splitFirst) {
      data += ((dx - 1 + dy * w) >> 1);
    }
    else {
      data += ((dx + dy * w) >> 1);
    }

    while (dh--) {
      uint32_t len = dw;
      uint8_t * ptr = (uint8_t*)data;
      uint16_t *linePtr = lineBuf;
      uint8_t colors; // two colors in one byte
      uint16_t index;

      if (splitFirst) {
        colors = pgm_read_byte(ptr);
        index = (colors & 0x0F);
        *linePtr++ = cmap[index];
        len--;
        ptr++;
      }

      while (len--) {
        colors = pgm_read_byte(ptr);
        index = ((colors & 0xF0) >> 4) & 0x0F;
        *linePtr++ = cmap[index];

        if (len--) {
          index = colors & 0x0F;
          *linePtr++ = cmap[index];
        } else break;

        ptr++;
      }

      pushPixels(lineBuf, dw);
      data += (w >> 1);
    }
    _swapBytes = swap; // Restore old value
  }
  else {
    _swapBytes = false;
    uint8_t * ptr = (uint8_t*)data;
    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    for (int32_t yp = dy;  yp < dy + dh; yp++) {
      uint8_t* linePtr = (uint8_t*)lineBuf;
      for (int32_t xp = dx; xp < dx + dw; xp++) {
        uint16_t col = (pgm_read_byte(ptr + (xp>>3)) & (0x80 >> (xp & 0x7)) );
        if (col) {*linePtr++ = bitmap_fg>>8; *linePtr++ = (uint8_t) bitmap_fg;}
        else     {*linePtr++ = bitmap_bg>>8; *linePtr++ = (uint8_t) bitmap_bg;}
      }
      ptr += ww;
      pushPixels(lineBuf, dw);
    }
  }

  _swapBytes = swap; // Restore old value
  inTransaction = lockTransaction;
  end_tft_write();
}

void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data, bool bpp8,  uint16_t *cmap)
{
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;
  bool swap = _swapBytes;

  setWindow(x, y, x + dw - 1, y + dh - 1); // Sets CS low and sent RAMWR

  // Line buffer makes plotting faster
  uint16_t  lineBuf[dw];

  if (bpp8)
  {
    _swapBytes = false;

    uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table

    _lastColor = -1; // Set to illegal value

    // Used to store last shifted colour
    uint8_t msbColor = 0;
    uint8_t lsbColor = 0;

    data += dx + dy * w;
    while (dh--) {
      uint32_t len = dw;
      uint8_t* ptr = data;
      uint8_t* linePtr = (uint8_t*)lineBuf;

      while(len--) {
        uint32_t color = *ptr++;

        // Shifts are slow so check if colour has changed first
        if (color != _lastColor) {
          //          =====Green=====     ===============Red==============
          msbColor = (color & 0x1C)>>2 | (color & 0xC0)>>3 | (color & 0xE0);
          //          =====Green=====    =======Blue======
          lsbColor = (color & 0x1C)<<3 | blue[color & 0x03];
          _lastColor = color;
        }

       *linePtr++ = msbColor;
       *linePtr++ = lsbColor;
      }

      pushPixels(lineBuf, dw);

      data += w;
    }
    _swapBytes = swap; // Restore old value
  }
  else if (cmap != nullptr) // Must be 4bpp
  {
    _swapBytes = true;

    w = (w+1) & 0xFFFE;   // if this is a sprite, w will already be even; this does no harm.
    bool splitFirst = (dx & 0x01) != 0; // split first means we have to push a single px from the left of the sprite / image

    if (splitFirst) {
      data += ((dx - 1 + dy * w) >> 1);
    }
    else {
      data += ((dx + dy * w) >> 1);
    }

    while (dh--) {
      uint32_t len = dw;
      uint8_t * ptr = data;
      uint16_t *linePtr = lineBuf;
      uint8_t colors; // two colors in one byte
      uint16_t index;

      if (splitFirst) {
        colors = *ptr;
        index = (colors & 0x0F);
        *linePtr++ = cmap[index];
        len--;
        ptr++;
      }

      while (len--)
      {
        colors = *ptr;
        index = ((colors & 0xF0) >> 4) & 0x0F;
        *linePtr++ = cmap[index];

        if (len--)
        {
          index = colors & 0x0F;
          *linePtr++ = cmap[index];
        } else {
          break;  // nothing to do here
        }

        ptr++;
      }

      pushPixels(lineBuf, dw);
      data += (w >> 1);
    }
    _swapBytes = swap; // Restore old value
  }
  else // Must be 1bpp
  {
    _swapBytes = false;

    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    for (int32_t yp = dy;  yp < dy + dh; yp++)
    {
      uint8_t* linePtr = (uint8_t*)lineBuf;
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        uint16_t col = (data[(xp>>3)] & (0x80 >> (xp & 0x7)) );
        if (col) {*linePtr++ = bitmap_fg>>8; *linePtr++ = (uint8_t) bitmap_fg;}
        else     {*linePtr++ = bitmap_bg>>8; *linePtr++ = (uint8_t) bitmap_bg;}
      }
      data += ww;
      pushPixels(lineBuf, dw);
    }
  }

  _swapBytes = swap; // Restore old value
  inTransaction = lockTransaction;
  end_tft_write();
}


/***************************************************************************************
** Function name:           pushImage
** Description:             plot 8 or 4 or 1 bit image or sprite with a transparent colour
***************************************************************************************/
void TFT_eSPI::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data, uint8_t transp, bool bpp8, uint16_t *cmap)
{
  PI_CLIP;

  begin_tft_write();
  inTransaction = true;
  bool swap = _swapBytes;


  // Line buffer makes plotting faster
  uint16_t  lineBuf[dw];

  if (bpp8) { // 8 bits per pixel
    _swapBytes = false;

    data += dx + dy * w;

    uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table

    _lastColor = -1; // Set to illegal value

    // Used to store last shifted colour
    uint8_t msbColor = 0;
    uint8_t lsbColor = 0;

    while (dh--) {
      int32_t len = dw;
      uint8_t* ptr = data;
      uint8_t* linePtr = (uint8_t*)lineBuf;

      int32_t px = x, sx = x;
      bool move = true;
      uint16_t np = 0;

      while (len--) {
        if (transp != *ptr) {
          if (move) { move = false; sx = px; }
          uint8_t color = *ptr;

          // Shifts are slow so check if colour has changed first
          if (color != _lastColor) {
            //          =====Green=====     ===============Red==============
            msbColor = (color & 0x1C)>>2 | (color & 0xC0)>>3 | (color & 0xE0);
            //          =====Green=====    =======Blue======
            lsbColor = (color & 0x1C)<<3 | blue[color & 0x03];
            _lastColor = color;
          }
          *linePtr++ = msbColor;
          *linePtr++ = lsbColor;
          np++;
        }
        else {
          move = true;
          if (np) {
            setWindow(sx, y, sx + np - 1, y);
            pushPixels(lineBuf, np);
            linePtr = (uint8_t*)lineBuf;
            np = 0;
          }
        }
        px++;
        ptr++;
      }

      if (np) { setWindow(sx, y, sx + np - 1, y); pushPixels(lineBuf, np); }
      y++;
      data += w;
    }
  }
  else if (cmap != nullptr) // 4bpp with color map
  {
    _swapBytes = true;

    w = (w+1) & 0xFFFE; // here we try to recreate iwidth from dwidth.
    bool splitFirst = ((dx & 0x01) != 0);
    if (splitFirst) {
      data += ((dx - 1 + dy * w) >> 1);
    }
    else {
      data += ((dx + dy * w) >> 1);
    }

    while (dh--) {
      uint32_t len = dw;
      uint8_t * ptr = data;

      int32_t px = x, sx = x;
      bool move = true;
      uint16_t np = 0;

      uint8_t index;  // index into cmap.

      if (splitFirst) {
        index = (*ptr & 0x0F);  // odd = bits 3 .. 0
        if (index != transp) {
          move = false; sx = px;
          lineBuf[np] = cmap[index];
          np++;
        }
        px++; ptr++;
        len--;
      }

      while (len--)
      {
        uint8_t color = *ptr;

        // find the actual color you care about.  There will be two pixels here!
        // but we may only want one at the end of the row
        uint16_t index = ((color & 0xF0) >> 4) & 0x0F;  // high bits are the even numbers
        if (index != transp) {
          if (move) {
            move = false; sx = px;
          }
          lineBuf[np] = cmap[index];
          np++; // added a pixel
        }
        else {
          move = true;
          if (np) {
            setWindow(sx, y, sx + np - 1, y);
            pushPixels(lineBuf, np);
            np = 0;
          }
        }
        px++;

        if (len--)
        {
          index = color & 0x0F; // the odd number is 3 .. 0
          if (index != transp) {
            if (move) {
              move = false; sx = px;
             }
            lineBuf[np] = cmap[index];
            np++;
          }
          else {
            move = true;
            if (np) {
              setWindow(sx, y, sx + np - 1, y);
              pushPixels(lineBuf, np);
              np = 0;
            }
          }
          px++;
        }
        else {
          break;  // we are done with this row.
        }
        ptr++;  // we only increment ptr once in the loop (deliberate)
      }

      if (np) {
        setWindow(sx, y, sx + np - 1, y);
        pushPixels(lineBuf, np);
        np = 0;
      }
      data += (w>>1);
      y++;
    }
  }
  else { // 1 bit per pixel
    _swapBytes = false;

    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    uint16_t np = 0;

    for (int32_t yp = dy;  yp < dy + dh; yp++)
    {
      int32_t px = x, sx = x;
      bool move = true;
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        if (data[(xp>>3)] & (0x80 >> (xp & 0x7))) {
          if (move) {
            move = false;
            sx = px;
          }
          np++;
        }
        else {
          move = true;
          if (np) {
            setWindow(sx, y, sx + np - 1, y);
            pushBlock(bitmap_fg, np);
            np = 0;
          }
        }
        px++;
      }
      if (np) { setWindow(sx, y, sx + np - 1, y); pushBlock(bitmap_fg, np); np = 0; }
      y++;
      data += ww;
    }
  }
  _swapBytes = swap; // Restore old value
  inTransaction = lockTransaction;
  end_tft_write();
}

/***************************************************************************************
** Function name:           pushMaskedImage
** Description:             Render a 16-bit colour image to TFT with a 1bpp mask
***************************************************************************************/
// Can be used with a 16bpp sprite and a 1bpp sprite for the mask
void TFT_eSPI::pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask)
{
  if (_vpOoB || w < 1 || h < 1) return;

  // To simplify mask handling the window clipping is done by the pushImage function
  // Each mask image line assumed to be padded to an integer number of bytes & padding bits are 0

  begin_tft_write();
  inTransaction = true;

  uint8_t  *mptr = mask;
  uint8_t  *eptr = mask + ((w + 7) >> 3);
  uint16_t *iptr = img;
  uint32_t setCount = 0;

  // For each line in the image
  while (h--) {
    uint32_t xp = 0;
    uint32_t clearCount = 0;
    uint8_t  mbyte= *mptr++;
    uint32_t bits  = 8;
    // Scan through each byte of the bitmap and determine run lengths
    do {
      setCount = 0;

      //Get run length for clear bits to determine x offset
      while ((mbyte & 0x80) == 0x00) {
        // Check if remaining bits in byte are clear (reduce shifts)
        if (mbyte == 0) {
          clearCount += bits;      // bits not always 8 here
          if (mptr >= eptr) break; // end of line
          mbyte = *mptr++;
          bits  = 8;
          continue;
        }
        mbyte = mbyte << 1; // 0's shifted in
        clearCount ++;
        if (--bits) continue;;
        if (mptr >= eptr) break;
        mbyte = *mptr++;
        bits  = 8;
      }

      //Get run length for set bits to determine render width
      while ((mbyte & 0x80) == 0x80) {
        // Check if all bits are set (reduces shifts)
        if (mbyte == 0xFF) {
          setCount += bits;
          if (mptr >= eptr) break;
          mbyte = *mptr++;
          //bits  = 8; // NR, bits always 8 here unless 1's shifted in
          continue;
        }
        mbyte = mbyte << 1; //or mbyte += mbyte + 1 to shift in 1's
        setCount ++;
        if (--bits) continue;
        if (mptr >= eptr) break;
        mbyte = *mptr++;
        bits  = 8;
      }

      // A mask boundary or mask end has been found, so render the pixel line
      if (setCount) {
        xp += clearCount;
        clearCount = 0;
        pushImage(x + xp, y, setCount, 1, iptr + xp);      // pushImage handles clipping
        if (mptr >= eptr) break;
        xp += setCount;
      }
    } while (setCount || mptr < eptr);

    y++;
    iptr += w;
    eptr += ((w + 7) >> 3);
  }

  inTransaction = lockTransaction;
  end_tft_write();
}


/***************************************************************************************
** Function name:           setSwapBytes
** Description:             Used by 16-bit pushImage() to swap byte order in colours
***************************************************************************************/
void TFT_eSPI::setSwapBytes(bool swap)
{
  _swapBytes = swap;
}


/***************************************************************************************
** Function name:           getSwapBytes
** Description:             Return the swap byte order for colours
***************************************************************************************/
bool TFT_eSPI::getSwapBytes(void)
{
  return _swapBytes;
}


/***************************************************************************************
** Function name:           read rectangle (for SPI Interface II i.e. IM [3:0] = "1101")
** Description:             Read RGB pixel colours from a defined area
***************************************************************************************/
// If w and h are 1, then 1 pixel is read, *data array size must be 3 bytes per pixel
void  TFT_eSPI::readRectRGB(int32_t x0, int32_t y0, int32_t w, int32_t h, uint8_t *data) {
  begin_tft_read();

  readAddrWindow(x0, y0, w, h); // Sets CS low

  #ifdef TFT_SDA_READ
    begin_SDA_Read();
  #endif

  // Dummy read to throw away don't care value
  tft_Read_8();

  // Read window pixel 24-bit RGB values, buffer must be set in sketch to 3 * w * h
  uint32_t len = w * h;
  while (len--) {
    // Read the 3 RGB bytes, colour is actually only in the top 6 bits of each byte
    // as the TFT stores colours as 18 bits
    *data++ = tft_Read_8();
    *data++ = tft_Read_8();
    *data++ = tft_Read_8();

    // The 6 colour bits are in MS 6 bits of each byte, but the ILI9488 needs an extra clock pulse
    // so bits appear shifted right 1 bit, so mask the middle 6 bits then shift 1 place left
    *data++ = (tft_Read_8()&0x7E)<<1;
    *data++ = (tft_Read_8()&0x7E)<<1;
    *data++ = (tft_Read_8()&0x7E)<<1;

  }

  CS_H;

  #ifdef TFT_SDA_READ
    end_SDA_Read();
  #endif

  end_tft_read();
}


/***************************************************************************************
** Function name:           drawCircle
** Description:             Draw a circle outline
***************************************************************************************/
// Optimised midpoint circle algorithm
void TFT_eSPI::drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color)
{
  if ( r <= 0 ) return;

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

    int32_t f     = 1 - r;
    int32_t ddF_y = -2 * r;
    int32_t ddF_x = 1;
    int32_t xs    = -1;
    int32_t xe    = 0;
    int32_t len   = 0;

    bool first = true;
    do {
      while (f < 0) {
        ++xe;
        f += (ddF_x += 2);
      }
      f += (ddF_y += 2);

      if (xe-xs>1) {
        if (first) {
          len = 2*(xe - xs)-1;
          drawFastHLine(x0 - xe, y0 + r, len, color);
          drawFastHLine(x0 - xe, y0 - r, len, color);
          drawFastVLine(x0 + r, y0 - xe, len, color);
          drawFastVLine(x0 - r, y0 - xe, len, color);
          first = false;
        }
        else {
          len = xe - xs++;
          drawFastHLine(x0 - xe, y0 + r, len, color);
          drawFastHLine(x0 - xe, y0 - r, len, color);
          drawFastHLine(x0 + xs, y0 - r, len, color);
          drawFastHLine(x0 + xs, y0 + r, len, color);

          drawFastVLine(x0 + r, y0 + xs, len, color);
          drawFastVLine(x0 + r, y0 - xe, len, color);
          drawFastVLine(x0 - r, y0 - xe, len, color);
          drawFastVLine(x0 - r, y0 + xs, len, color);
        }
      }
      else {
        ++xs;
        drawPixel(x0 - xe, y0 + r, color);
        drawPixel(x0 - xe, y0 - r, color);
        drawPixel(x0 + xs, y0 - r, color);
        drawPixel(x0 + xs, y0 + r, color);

        drawPixel(x0 + r, y0 + xs, color);
        drawPixel(x0 + r, y0 - xe, color);
        drawPixel(x0 - r, y0 - xe, color);
        drawPixel(x0 - r, y0 + xs, color);
      }
      xs = xe;
    } while (xe < --r);

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           drawCircleHelper
** Description:             Support function for drawRoundRect()
***************************************************************************************/
void TFT_eSPI::drawCircleHelper( int32_t x0, int32_t y0, int32_t rr, uint8_t cornername, uint32_t color)
{
  if (rr <= 0) return;
  int32_t f     = 1 - rr;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * rr;
  int32_t xe    = 0;
  int32_t xs    = 0;
  int32_t len   = 0;

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  do
  {
    while (f < 0) {
      ++xe;
      f += (ddF_x += 2);
    }
    f += (ddF_y += 2);

    if (xe-xs==1) {
      if (cornername & 0x1) { // left top
        drawPixel(x0 - xe, y0 - rr, color);
        drawPixel(x0 - rr, y0 - xe, color);
      }
      if (cornername & 0x2) { // right top
        drawPixel(x0 + rr    , y0 - xe, color);
        drawPixel(x0 + xs + 1, y0 - rr, color);
      }
      if (cornername & 0x4) { // right bottom
        drawPixel(x0 + xs + 1, y0 + rr    , color);
        drawPixel(x0 + rr, y0 + xs + 1, color);
      }
      if (cornername & 0x8) { // left bottom
        drawPixel(x0 - rr, y0 + xs + 1, color);
        drawPixel(x0 - xe, y0 + rr    , color);
      }
    }
    else {
      len = xe - xs++;
      if (cornername & 0x1) { // left top
        drawFastHLine(x0 - xe, y0 - rr, len, color);
        drawFastVLine(x0 - rr, y0 - xe, len, color);
      }
      if (cornername & 0x2) { // right top
        drawFastVLine(x0 + rr, y0 - xe, len, color);
        drawFastHLine(x0 + xs, y0 - rr, len, color);
      }
      if (cornername & 0x4) { // right bottom
        drawFastHLine(x0 + xs, y0 + rr, len, color);
        drawFastVLine(x0 + rr, y0 + xs, len, color);
      }
      if (cornername & 0x8) { // left bottom
        drawFastVLine(x0 - rr, y0 + xs, len, color);
        drawFastHLine(x0 - xe, y0 + rr, len, color);
      }
    }
    xs = xe;
  } while (xe < rr--);

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}

/***************************************************************************************
** Function name:           fillCircle
** Description:             draw a filled circle
***************************************************************************************/
// Optimised midpoint circle algorithm, changed to horizontal lines (faster in sprites)
// Improved algorithm avoids repetition of lines
void TFT_eSPI::fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color)
{
  int32_t  x  = 0;
  int32_t  dx = 1;
  int32_t  dy = r+r;
  int32_t  p  = -(r>>1);

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  drawFastHLine(x0 - r, y0, dy+1, color);

  while(x<r){

    if(p>=0) {
      drawFastHLine(x0 - x, y0 + r, dx, color);
      drawFastHLine(x0 - x, y0 - r, dx, color);
      dy-=2;
      p-=dy;
      r--;
    }

    dx+=2;
    p+=dx;
    x++;

    drawFastHLine(x0 - r, y0 + x, dy+1, color);
    drawFastHLine(x0 - r, y0 - x, dy+1, color);

  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}

/***************************************************************************************
** Function name:           fillCircleHelper
** Description:             Support function for fillRoundRect()
***************************************************************************************/
// Support drawing roundrects, changed to horizontal lines (faster in sprites)
void TFT_eSPI::fillCircleHelper(int32_t x0, int32_t y0, int32_t r, uint8_t cornername, int32_t delta, uint32_t color)
{
  int32_t f     = 1 - r;
  int32_t ddF_x = 1;
  int32_t ddF_y = -r - r;
  int32_t y     = 0;

  delta++;

  while (y < r) {
    if (f >= 0) {
      if (cornername & 0x1) drawFastHLine(x0 - y, y0 + r, y + y + delta, color);
      if (cornername & 0x2) drawFastHLine(x0 - y, y0 - r, y + y + delta, color);
      r--;
      ddF_y += 2;
      f     += ddF_y;
    }

    y++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) drawFastHLine(x0 - r, y0 + y, r + r + delta, color);
    if (cornername & 0x2) drawFastHLine(x0 - r, y0 - y, r + r + delta, color);
  }
}


/***************************************************************************************
** Function name:           drawEllipse
** Description:             Draw a ellipse outline
***************************************************************************************/
void TFT_eSPI::drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int32_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++) {
    // These are ordered to minimise coordinate changes in x or y
    // drawPixel can then send fewer bounding box commands
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0) {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++) {
    // These are ordered to minimise coordinate changes in x or y
    // drawPixel can then send fewer bounding box commands
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0)
    {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           fillEllipse
** Description:             draw a filled ellipse
***************************************************************************************/
void TFT_eSPI::fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int32_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++) {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0) {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++) {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0) {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           fillScreen
** Description:             Clear the screen to defined colour
***************************************************************************************/
void TFT_eSPI::fillScreen(uint32_t color)
{
  fillRect(0, 0, _width, _height, color);
}


/***************************************************************************************
** Function name:           drawRect
** Description:             Draw a rectangle outline
***************************************************************************************/
// Draw a rectangle
void TFT_eSPI::drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  // Avoid drawing corner pixels twice
  drawFastVLine(x, y+1, h-2, color);
  drawFastVLine(x + w - 1, y+1, h-2, color);

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           drawRoundRect
** Description:             Draw a rounded corner rectangle outline
***************************************************************************************/
// Draw a rounded rectangle
void TFT_eSPI::drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  // smarter version
  drawFastHLine(x + r  , y    , w - r - r, color); // Top
  drawFastHLine(x + r  , y + h - 1, w - r - r, color); // Bottom
  drawFastVLine(x    , y + r  , h - r - r, color); // Left
  drawFastVLine(x + w - 1, y + r  , h - r - r, color); // Right
  // draw four corners
  drawCircleHelper(x + r    , y + r    , r, 1, color);
  drawCircleHelper(x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(x + r    , y + h - r - 1, r, 8, color);

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           fillRoundRect
** Description:             Draw a rounded corner filled rectangle
***************************************************************************************/
// Fill a rounded rectangle, changed to horizontal lines (faster in sprites)
void TFT_eSPI::fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  // smarter version
  fillRect(x, y + r, w, h - r - r, color);

  // draw four corners
  fillCircleHelper(x + r, y + h - r - 1, r, 1, w - r - r - 1, color);
  fillCircleHelper(x + r    , y + r, r, 2, w - r - r - 1, color);

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           drawTriangle
** Description:             Draw a triangle outline using 3 arbitrary points
***************************************************************************************/
// Draw a triangle
void TFT_eSPI::drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           fillTriangle
** Description:             Draw a filled triangle using 3 arbitrary points
***************************************************************************************/
// Fill a triangle - original Adafruit function works well and code footprint is small
void TFT_eSPI::fillTriangle ( int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
{
  int32_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    transpose(y0, y1); transpose(x0, x1);
  }
  if (y1 > y2) {
    transpose(y2, y1); transpose(x2, x1);
  }
  if (y0 > y1) {
    transpose(y0, y1); transpose(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)      a = x1;
    else if (x1 > b) b = x1;
    if (x2 < a)      a = x2;
    else if (x2 > b) b = x2;
    drawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  int32_t
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1,
  sa   = 0,
  sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) last = y1;  // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b) transpose(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for (; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b) transpose(a, b);
    drawFastHLine(a, y, b - a + 1, color);
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           drawBitmap
** Description:             Draw an image stored in an array on the TFT
***************************************************************************************/
void TFT_eSPI::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  int32_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        drawPixel(x + i, y + j, color);
      }
    }
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           drawBitmap
** Description:             Draw an image stored in an array on the TFT
***************************************************************************************/
void TFT_eSPI::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  int32_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))
           drawPixel(x + i, y + j, fgcolor);
      else drawPixel(x + i, y + j, bgcolor);
    }
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}

/***************************************************************************************
** Function name:           drawXBitmap
** Description:             Draw an image stored in an XBM array onto the TFT
***************************************************************************************/
void TFT_eSPI::drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  int32_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i & 7))) {
        drawPixel(x + i, y + j, color);
      }
    }
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           drawXBitmap
** Description:             Draw an XBM image with foreground and background colors
***************************************************************************************/
void TFT_eSPI::drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor)
{
  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  int32_t i, j, byteWidth = (w + 7) / 8;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++ ) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i & 7)))
           drawPixel(x + i, y + j,   color);
      else drawPixel(x + i, y + j, bgcolor);
    }
  }

  inTransaction = lockTransaction;
  end_tft_write();              // Does nothing if Sprite class uses this function
}


/***************************************************************************************
** Function name:           setCursor
** Description:             Set the text cursor x,y position
***************************************************************************************/
void TFT_eSPI::setCursor(int16_t x, int16_t y)
{
  cursor_x = x;
  cursor_y = y;
}


/***************************************************************************************
** Function name:           setCursor
** Description:             Set the text cursor x,y position and font
***************************************************************************************/
void TFT_eSPI::setCursor(int16_t x, int16_t y, uint8_t font)
{
  setTextFont(font);
  cursor_x = x;
  cursor_y = y;
}


/***************************************************************************************
** Function name:           getCursorX
** Description:             Get the text cursor x position
***************************************************************************************/
int16_t TFT_eSPI::getCursorX(void)
{
  return cursor_x;
}

/***************************************************************************************
** Function name:           getCursorY
** Description:             Get the text cursor y position
***************************************************************************************/
int16_t TFT_eSPI::getCursorY(void)
{
  return cursor_y;
}


/***************************************************************************************
** Function name:           setTextSize
** Description:             Set the text size multiplier
***************************************************************************************/
void TFT_eSPI::setTextSize(uint8_t s)
{
  if (s>7) s = 7; // Limit the maximum size multiplier so byte variables can be used for rendering
  textsize = (s > 0) ? s : 1; // Don't allow font size 0
}


/***************************************************************************************
** Function name:           setTextColor
** Description:             Set the font foreground colour (background is transparent)
***************************************************************************************/
void TFT_eSPI::setTextColor(uint16_t c)
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}


/***************************************************************************************
** Function name:           setTextColor
** Description:             Set the font foreground and background colour
***************************************************************************************/
// Smooth fonts use the background colour for anti-aliasing and by default the
// background is not filled. If bgfill = true, then a smooth font background fill will
// be used.
void TFT_eSPI::setTextColor(uint16_t c, uint16_t b, bool bgfill)
{
  textcolor   = c;
  textbgcolor = b;
  _fillbg     = bgfill;
}


/***************************************************************************************
** Function name:           setPivot
** Description:             Set the pivot point on the TFT
*************************************************************************************x*/
void TFT_eSPI::setPivot(int16_t x, int16_t y)
{
  _xPivot = x;
  _yPivot = y;
}


/***************************************************************************************
** Function name:           getPivotX
** Description:             Get the x pivot position
***************************************************************************************/
int16_t TFT_eSPI::getPivotX(void)
{
  return _xPivot;
}


/***************************************************************************************
** Function name:           getPivotY
** Description:             Get the y pivot position
***************************************************************************************/
int16_t TFT_eSPI::getPivotY(void)
{
  return _yPivot;
}


/***************************************************************************************
** Function name:           setBitmapColor
** Description:             Set the foreground foreground and background colour
***************************************************************************************/
void TFT_eSPI::setBitmapColor(uint16_t c, uint16_t b)
{
  if (c == b) b = ~c;
  bitmap_fg = c;
  bitmap_bg = b;
}


/***************************************************************************************
** Function name:           setTextWrap
** Description:             Define if text should wrap at end of line
***************************************************************************************/
void TFT_eSPI::setTextWrap(bool wrapX, bool wrapY)
{
  textwrapX = wrapX;
  textwrapY = wrapY;
}


/***************************************************************************************
** Function name:           setTextDatum
** Description:             Set the text position reference datum
***************************************************************************************/
void TFT_eSPI::setTextDatum(uint8_t d)
{
  textdatum = d;
}


/***************************************************************************************
** Function name:           setTextPadding
** Description:             Define padding width (aids erasing old text and numbers)
***************************************************************************************/
void TFT_eSPI::setTextPadding(uint16_t x_width)
{
  padX = x_width;
}

/***************************************************************************************
** Function name:           setTextPadding
** Description:             Define padding width (aids erasing old text and numbers)
***************************************************************************************/
uint16_t TFT_eSPI::getTextPadding(void)
{
  return padX;
}

/***************************************************************************************
** Function name:           getTextDatum
** Description:             Return the text datum value (as used by setTextDatum())
***************************************************************************************/
uint8_t TFT_eSPI::getTextDatum(void)
{
  return textdatum;
}


/***************************************************************************************
** Function name:           width
** Description:             Return the pixel width of display (per current rotation)
***************************************************************************************/
// Return the size of the display (per current rotation)
int16_t TFT_eSPI::width(void)
{
  if (_vpDatum) return _xWidth;
  return _width;
}


/***************************************************************************************
** Function name:           height
** Description:             Return the pixel height of display (per current rotation)
***************************************************************************************/
int16_t TFT_eSPI::height(void)
{
  if (_vpDatum) return _yHeight;
  return _height;
}


/***************************************************************************************
** Function name:           textWidth
** Description:             Return the width in pixels of a string in a given font
***************************************************************************************/
int16_t TFT_eSPI::textWidth(const String& string)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return textWidth(buffer, textfont);
}

int16_t TFT_eSPI::textWidth(const String& string, uint8_t font)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return textWidth(buffer, font);
}

int16_t TFT_eSPI::textWidth(const char *string)
{
  return textWidth(string, textfont);
}

int16_t TFT_eSPI::textWidth(const char *string, uint8_t font)
{
  int32_t str_width = 0;
  uint16_t uniCode  = 0;

  if(fontLoaded) {
    while (*string) {
      uniCode = decodeUTF8(*string++);
      if (uniCode) {
        if (uniCode == 0x20) str_width += gFont.spaceWidth;
        else {
          uint16_t gNum = 0;
          bool found = getUnicodeIndex(uniCode, &gNum);
          if (found) {
            if(str_width == 0 && gdX[gNum] < 0) str_width -= gdX[gNum];
            if (*string || isDigits) str_width += gxAdvance[gNum];
            else str_width += (gdX[gNum] + gWidth[gNum]);
          }
          else str_width += gFont.spaceWidth + 1;
        }
      }
    }
    isDigits = false;
    return str_width;
  }

  if (font>1 && font<9) {
    char *widthtable = (char *)pgm_read_dword( &(fontdata[font].widthtbl ) ) - 32; //subtract the 32 outside the loop

    while (*string) {
      uniCode = *(string++);
      if (uniCode > 31 && uniCode < 128)
      str_width += pgm_read_byte( widthtable + uniCode); // Normally we need to subtract 32 from uniCode
      else str_width += pgm_read_byte( widthtable + 32); // Set illegal character = space width
    }

  }
  else {
    while (*string++) str_width += 6;
  }
  isDigits = false;
  return str_width * textsize;
}


/***************************************************************************************
** Function name:           fontsLoaded
** Description:             return an encoded 16-bit value showing the fonts loaded
***************************************************************************************/
// Returns a value showing which fonts are loaded (bit N set =  Font N loaded)
uint16_t TFT_eSPI::fontsLoaded(void)
{
  return fontsloaded;
}


/***************************************************************************************
** Function name:           fontHeight
** Description:             return the height of a font (yAdvance for free fonts)
***************************************************************************************/
int16_t TFT_eSPI::fontHeight(uint8_t font)
{
  if (font > 8) return 0;

  if(fontLoaded) return gFont.yAdvance;

  return pgm_read_byte( &fontdata[font].height ) * textsize;
}

int16_t TFT_eSPI::fontHeight(void)
{
  return fontHeight(textfont);
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a single character in the GLCD or GFXFF font
***************************************************************************************/
void TFT_eSPI::drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size)
{
  if (_vpOoB) return;

  int32_t xd = x + _xDatum;
  int32_t yd = y + _yDatum;

  if ((xd >= _vpW)                 || // Clip right
     ( yd >= _vpH)                 || // Clip bottom
     ((xd + 6 * size - 1) < _vpX)  || // Clip left
     ((yd + 8 * size - 1) < _vpY))    // Clip top
    return;

  if (c > 255) return;
  if (!_cp437 && c > 175) c++;

  bool fillbg = (bg != color);
  bool clip = xd < _vpX || xd + 6  * textsize >= _vpW || yd < _vpY || yd + 8 * textsize >= _vpH;

  if ((size==1) && fillbg && !clip) {
    uint8_t column[6];
    uint8_t mask = 0x1;
    begin_tft_write();

    setWindow(xd, yd, xd+5, yd+7);

    for (int8_t i = 0; i < 5; i++ ) column[i] = pgm_read_byte(&font[0] + (c * 5) + i);
    column[5] = 0;

    for (int8_t j = 0; j < 8; j++) {
      for (int8_t k = 0; k < 5; k++ ) {
        if (column[k] & mask) {tft_Write_16(color);}
        else {tft_Write_16(bg);}
      }
      mask <<= 1;
      tft_Write_16(bg);
    }

    end_tft_write();
  }
  else {
    //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
    inTransaction = true;

    for (int8_t i = 0; i < 6; i++ ) {
      uint8_t line;
      if (i == 5)
        line = 0x0;
      else
        line = pgm_read_byte(&font[0] + (c * 5) + i);

      if (size == 1 && !fillbg) { // default size
        for (int8_t j = 0; j < 8; j++) {
          if (line & 0x1) drawPixel(x + i, y + j, color);
          line >>= 1;
        }
      }
      else {  // big size or clipped
        for (int8_t j = 0; j < 8; j++) {
          if (line & 0x1) fillRect(x + (i * size), y + (j * size), size, size, color);
          else if (fillbg) fillRect(x + i * size, y + j * size, size, size, bg);
          line >>= 1;
        }
      }
    }
    inTransaction = lockTransaction;
    end_tft_write();              // Does nothing if Sprite class uses this function
  }
}


/***************************************************************************************
** Function name:           setAddrWindow
** Description:             define an area to receive a stream of pixels
***************************************************************************************/
// Chip select is high at the end of this function
void TFT_eSPI::setAddrWindow(int32_t x0, int32_t y0, int32_t w, int32_t h)
{
  begin_tft_write();

  setWindow(x0, y0, x0 + w - 1, y0 + h - 1);

  end_tft_write();
}


/***************************************************************************************
** Function name:           setWindow
** Description:             define an area to receive a stream of pixels
***************************************************************************************/
// Chip select stays low, call begin_tft_write first. Use setAddrWindow() from sketches
void TFT_eSPI::setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  //begin_tft_write(); // Must be called before setWindow
  addr_row = 0xFFFF;
  addr_col = 0xFFFF;

  #ifdef CGRAM_OFFSET
    x0+=colstart;
    x1+=colstart;
    y0+=rowstart;
    y1+=rowstart;
  #endif

  SPI_BUSY_CHECK;
  DC_C; tft_Write_8(TFT_CASET);
  DC_D; tft_Write_32C(x0, x1);
  DC_C; tft_Write_8(TFT_PASET);
  DC_D; tft_Write_32C(y0, y1);
  DC_C; tft_Write_8(TFT_RAMWR);
  DC_D;
  //end_tft_write(); // Must be called after setWindow
}

/***************************************************************************************
** Function name:           readAddrWindow
** Description:             define an area to read a stream of pixels
***************************************************************************************/
void TFT_eSPI::readAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h)
{
  //begin_tft_write(); // Must be called before readAddrWindow or CS set low

  int32_t xe = xs + w - 1;
  int32_t ye = ys + h - 1;

  addr_col = 0xFFFF;
  addr_row = 0xFFFF;

#ifdef CGRAM_OFFSET
  xs += colstart;
  xe += colstart;
  ys += rowstart;
  ye += rowstart;
#endif

  // Column addr set
  DC_C; tft_Write_8(TFT_CASET);
  DC_D; tft_Write_32C(xs, xe);

  // Row addr set
  DC_C; tft_Write_8(TFT_PASET);
  DC_D; tft_Write_32C(ys, ye);

  // Read CGRAM command
  DC_C; tft_Write_8(TFT_RAMRD);

  DC_D;

  //end_tft_write(); // Must be called after readAddrWindow or CS set high
}


/***************************************************************************************
** Function name:           drawPixel
** Description:             push a single pixel at an arbitrary position
***************************************************************************************/
void TFT_eSPI::drawPixel(int32_t x, int32_t y, uint32_t color)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return;

#ifdef CGRAM_OFFSET
  x+=colstart;
  y+=rowstart;
#endif

#if (defined (MULTI_TFT_SUPPORT) || defined (GC9A01_DRIVER)) && !defined (ILI9225_DRIVER)
  addr_row = 0xFFFF;
  addr_col = 0xFFFF;
#endif

  begin_tft_write();
    SPI_BUSY_CHECK;

  // No need to send x if it has not changed (speeds things up)
  if (addr_col != x) {
    DC_C; tft_Write_8(TFT_CASET);
    DC_D; tft_Write_32D(x);
    addr_col = x;
  }

  // No need to send y if it has not changed (speeds things up)
  if (addr_row != y) {
    DC_C; tft_Write_8(TFT_PASET);
    DC_D; tft_Write_32D(y);
    addr_row = y;
  }

  DC_C; tft_Write_8(TFT_RAMWR);

  DC_D; tft_Write_16N(color);

  end_tft_write();
}

/***************************************************************************************
** Function name:           pushColor
** Description:             push a single pixel
***************************************************************************************/
void TFT_eSPI::pushColor(uint16_t color)
{
  begin_tft_write();

  SPI_BUSY_CHECK;
  tft_Write_16N(color);

  end_tft_write();
}


/***************************************************************************************
** Function name:           pushColor
** Description:             push a single colour to "len" pixels
***************************************************************************************/
void TFT_eSPI::pushColor(uint16_t color, uint32_t len)
{
  begin_tft_write();

  pushBlock(color, len);

  end_tft_write();
}

/***************************************************************************************
** Function name:           startWrite
** Description:             begin transaction with CS low, MUST later call endWrite
***************************************************************************************/
void TFT_eSPI::startWrite(void)
{
  begin_tft_write();
  lockTransaction = true; // Lock transaction for all sequentially run sketch functions
  inTransaction = true;
}

/***************************************************************************************
** Function name:           endWrite
** Description:             end transaction with CS high
***************************************************************************************/
void TFT_eSPI::endWrite(void)
{
  lockTransaction = false; // Release sketch induced transaction lock
  inTransaction = false;
  dmaWait();          // Safety check - user code should have checked this!
  end_tft_write();         // Release SPI bus
}

/***************************************************************************************
** Function name:           writeColor (use startWrite() and endWrite() before & after)
** Description:             raw write of "len" pixels avoiding transaction check
***************************************************************************************/
void TFT_eSPI::writeColor(uint16_t color, uint32_t len)
{
  pushBlock(color, len);
}

/***************************************************************************************
** Function name:           pushColors
** Description:             push an array of pixels for 16-bit raw image drawing
***************************************************************************************/
// Assumed that setAddrWindow() has previously been called
// len is number of bytes, not pixels
void TFT_eSPI::pushColors(uint8_t *data, uint32_t len)
{
  begin_tft_write();

  pushPixels(data, len>>1);

  end_tft_write();
}


/***************************************************************************************
** Function name:           pushColors
** Description:             push an array of pixels, for image drawing
***************************************************************************************/
void TFT_eSPI::pushColors(uint16_t *data, uint32_t len, bool swap)
{
  begin_tft_write();
  if (swap) {swap = _swapBytes; _swapBytes = true; }

  pushPixels(data, len);

  _swapBytes = swap; // Restore old value
  end_tft_write();
}


/***************************************************************************************
** Function name:           drawLine
** Description:             draw a line between 2 arbitrary points
***************************************************************************************/
// Bresenham's algorithm - thx Wikipedia - speed enhanced by Bodmer to use
// an efficient FastH/V Line draw routine for line segments of 2 pixels or more
void TFT_eSPI::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
  if (_vpOoB) return;

  //begin_tft_write();       // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  //x+= _xDatum;             // Not added here, added by drawPixel & drawFastXLine
  //y+= _yDatum;

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    transpose(x0, y0);
    transpose(x1, y1);
  }

  if (x0 > x1) {
    transpose(x0, x1);
    transpose(y0, y1);
  }

  int32_t dx = x1 - x0, dy = abs(y1 - y0);;

  int32_t err = dx >> 1, ystep = -1, xs = x0, dlen = 0;

  if (y0 < y1) ystep = 1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        if (dlen == 1) drawPixel(y0, xs, color);
        else drawFastVLine(y0, xs, dlen, color);
        dlen = 0;
        y0 += ystep; xs = x0 + 1;
        err += dx;
      }
    }
    if (dlen) drawFastVLine(y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        if (dlen == 1) drawPixel(xs, y0, color);
        else drawFastHLine(xs, y0, dlen, color);
        dlen = 0;
        y0 += ystep; xs = x0 + 1;
        err += dx;
      }
    }
    if (dlen) drawFastHLine(xs, y0, dlen, color);
  }

  inTransaction = lockTransaction;
  end_tft_write();
}


/***************************************************************************************
** Description:  Constants for anti-aliased line drawing on TFT and in Sprites
***************************************************************************************/
constexpr float PixelAlphaGain   = 255.0;
constexpr float LoAlphaTheshold  = 1.0/32.0;
constexpr float HiAlphaTheshold  = 1.0 - LoAlphaTheshold;
constexpr float deg2rad      = 3.14159265359/180.0;

/***************************************************************************************
** Function name:           drawPixel (alpha blended)
** Description:             Draw a pixel blended with the screen or bg pixel colour
***************************************************************************************/
uint16_t TFT_eSPI::drawPixel(int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color)
{
  if (bg_color == 0x00FFFFFF) bg_color = readPixel(x, y);
  color = fastBlend(alpha, color, bg_color);
  drawPixel(x, y, color);
  return color;
}


/***************************************************************************************
** Function name:           drawSmoothArc
** Description:             Draw a smooth arc clockwise from 6 o'clock
***************************************************************************************/
void TFT_eSPI::drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds)
// Centre at x,y
// r = arc outer radius, ir = arc inner radius. Inclusive so arc thickness = r - ir + 1
// Angles in range 0-360
// Arc foreground colour anti-aliased with background colour at edges
// anti-aliased roundEnd is optional, default is anti-aliased straight end
// Note: rounded ends extend the arc angle so can overlap, user sketch to manage this.
{
  inTransaction = true;

  if (endAngle != startAngle && (startAngle != 0 || endAngle != 360))
  {
    float sx = -sinf(startAngle * deg2rad);
    float sy = +cosf(startAngle * deg2rad);
    float ex = -sinf(  endAngle * deg2rad);
    float ey = +cosf(  endAngle * deg2rad);

    if (roundEnds)
    { // Round ends
      sx = sx * (r + ir)/2.0 + x;
      sy = sy * (r + ir)/2.0 + y;
      drawSpot(sx, sy, (r - ir)/2.0, fg_color, bg_color);

      ex = ex * (r + ir)/2.0 + x;
      ey = ey * (r + ir)/2.0 + y;
      drawSpot(ex, ey, (r - ir)/2.0, fg_color, bg_color);
    }
    else
    { // Square ends
      float asx = sx * ir + x;
      float asy = sy * ir + y;
      float aex = sx *  r + x;
      float aey = sy *  r + y;
      drawWedgeLine(asx, asy, aex, aey, 0.3, 0.3, fg_color, bg_color);

      asx = ex * ir + x;
      asy = ey * ir + y;
      aex = ex *  r + x;
      aey = ey *  r + y;
      drawWedgeLine(asx, asy, aex, aey, 0.3, 0.3, fg_color, bg_color);
    }

    // Draw arc
    drawArc(x, y, r, ir, startAngle, endAngle, fg_color, bg_color);

  }
  else // Draw full 360
  {
    drawArc(x, y, r, ir, 0, 360, fg_color, bg_color);
  }

  inTransaction = lockTransaction;
  end_tft_write();
}

/***************************************************************************************
** Function name:           sqrt_fraction (private function)
** Description:             Smooth graphics support function for alpha derivation
***************************************************************************************/
// Compute the fixed point square root of an integer and
// return the 8 MS bits of fractional part.
// Quicker than sqrt() for processors that do not have an FPU (e.g. RP2040)
inline uint8_t TFT_eSPI::sqrt_fraction(uint32_t num) {
  if (num > (0x40000000)) return 0;
  uint32_t bsh = 0x00004000;
  uint32_t fpr = 0;
  uint32_t osh = 0;

  // Auto adjust from U8:8 up to U15:16
  while (num>bsh) {bsh <<= 2; osh++;}

  do {
    uint32_t bod = bsh + fpr;
    if(num >= bod)
    {
      num -= bod;
      fpr = bsh + bod;
    }
    num <<= 1;
  } while(bsh >>= 1);

  return fpr>>osh;
}

/***************************************************************************************
** Function name:           drawArc
** Description:             Draw an arc clockwise from 6 o'clock position
***************************************************************************************/
// Centre at x,y
// r = arc outer radius, ir = arc inner radius. Inclusive, so arc thickness = r-ir+1
// Angles MUST be in range 0-360
// Arc foreground fg_color anti-aliased with background colour along sides
// smooth is optional, default is true, smooth=false means no antialiasing
// Note: Arc ends are not anti-aliased (use drawSmoothArc instead for that)
void TFT_eSPI::drawArc(int32_t x, int32_t y, int32_t r, int32_t ir,
                       uint32_t startAngle, uint32_t endAngle,
                       uint32_t fg_color, uint32_t bg_color,
                       bool smooth)
{
  if (endAngle   > 360)   endAngle = 360;
  if (startAngle > 360) startAngle = 360;
  if (_vpOoB || startAngle == endAngle) return;
  if (r < ir) transpose(r, ir);  // Required that r > ir
  if (r <= 0 || ir < 0) return;  // Invalid r, ir can be zero (circle sector)

  if (endAngle < startAngle) {
    // Arc sweeps through 6 o'clock so draw in two parts
    if (startAngle < 360) drawArc(x, y, r, ir, startAngle, 360, fg_color, bg_color, smooth);
    if (endAngle == 0) return;
    startAngle = 0;
  }
  inTransaction = true;

  int32_t xs = 0;        // x start position for quadrant scan
  uint8_t alpha = 0;     // alpha value for blending pixels

  uint32_t r2 = r * r;   // Outer arc radius^2
  if (smooth) r++;       // Outer AA zone radius
  uint32_t r1 = r * r;   // Outer AA radius^2
  int16_t w  = r - ir;   // Width of arc (r - ir + 1)
  uint32_t r3 = ir * ir; // Inner arc radius^2
  if (smooth) ir--;      // Inner AA zone radius
  uint32_t r4 = ir * ir; // Inner AA radius^2

  //     1 | 2
  //    ---¦---    Arc quadrant index
  //     0 | 3
  // Fixed point U16.16 slope table for arc start/end in each quadrant
  uint32_t startSlope[4] = {0, 0, 0xFFFFFFFF, 0};
  uint32_t   endSlope[4] = {0, 0xFFFFFFFF, 0, 0};

  // Ensure maximum U16.16 slope of arc ends is ~ 0x8000 0000
  constexpr float minDivisor = 1.0f/0x8000;

  // Fill in start slope table and empty quadrants
  float fabscos = fabsf(cosf(startAngle * deg2rad));
  float fabssin = fabsf(sinf(startAngle * deg2rad));

  // U16.16 slope of arc start
  uint32_t slope = (fabscos/(fabssin + minDivisor)) * (float)(1UL<<16);

  // Update slope table, add slope for arc start
  if (startAngle <= 90) {
    startSlope[0] =  slope;
  }
  else if (startAngle <= 180) {
    startSlope[1] =  slope;
  }
  else if (startAngle <= 270) {
    startSlope[1] = 0xFFFFFFFF;
    startSlope[2] = slope;
  }
  else {
    startSlope[1] = 0xFFFFFFFF;
    startSlope[2] =  0;
    startSlope[3] = slope;
  }

  // Fill in end slope table and empty quadrants
  fabscos  = fabsf(cosf(endAngle * deg2rad));
  fabssin  = fabsf(sinf(endAngle * deg2rad));

  // U16.16 slope of arc end
  slope   = (uint32_t)((fabscos/(fabssin + minDivisor)) * (float)(1UL<<16));

  // Work out which quadrants will need to be drawn and add slope for arc end
  if (endAngle <= 90) {
    endSlope[0] = slope;
    endSlope[1] =  0;
    startSlope[2] =  0;
  }
  else if (endAngle <= 180) {
    endSlope[1] = slope;
    startSlope[2] =  0;
  }
  else if (endAngle <= 270) {
    endSlope[2] =  slope;
  }
  else {
    endSlope[3] =  slope;
  }

  // Scan quadrant
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    uint32_t len[4] = { 0,  0,  0,  0}; // Pixel run length
    int32_t  xst[4] = {-1, -1, -1, -1}; // Pixel run x start
    uint32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (int32_t cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      uint32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line start and lengths for each quadrant
      else if (hyp >= r3) {
        // Calculate U16.16 slope
        slope = ((r - cy) << 16)/(r - cx);
        if (slope <= startSlope[0] && slope >= endSlope[0]) { // slope hi -> lo
          xst[0] = cx; // Bottom left line end
          len[0]++;
        }
        if (slope >= startSlope[1] && slope <= endSlope[1]) { // slope lo -> hi
          xst[1] = cx; // Top left line end
          len[1]++;
        }
        if (slope <= startSlope[2] && slope >= endSlope[2]) { // slope hi -> lo
          xst[2] = cx; // Bottom right line start
          len[2]++;
        }
        if (slope <= endSlope[3] && slope >= startSlope[3]) { // slope lo -> hi
          xst[3] = cx; // Top right line start
          len[3]++;
        }
        continue; // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant
      uint16_t pcol = fastBlend(alpha, fg_color, bg_color);
      // Check if an AA pixels need to be drawn
      slope = ((r - cy)<<16)/(r - cx);
      if (slope <= startSlope[0] && slope >= endSlope[0]) // BL
        drawPixel(x + cx - r, y - cy + r, pcol);
      if (slope >= startSlope[1] && slope <= endSlope[1]) // TL
        drawPixel(x + cx - r, y + cy - r, pcol);
      if (slope <= startSlope[2] && slope >= endSlope[2]) // TR
        drawPixel(x - cx + r, y + cy - r, pcol);
      if (slope <= endSlope[3] && slope >= startSlope[3]) // BR
        drawPixel(x - cx + r, y - cy + r, pcol);
    }
    // Add line in inner zone
    if (len[0]) drawFastHLine(x + xst[0] - len[0] + 1 - r, y - cy + r, len[0], fg_color); // BL
    if (len[1]) drawFastHLine(x + xst[1] - len[1] + 1 - r, y + cy - r, len[1], fg_color); // TL
    if (len[2]) drawFastHLine(x - xst[2] + r, y + cy - r, len[2], fg_color); // TR
    if (len[3]) drawFastHLine(x - xst[3] + r, y - cy + r, len[3], fg_color); // BR
  }

  // Fill in centre lines
  if (startAngle ==   0 || endAngle == 360) drawFastVLine(x, y + r - w, w, fg_color); // Bottom
  if (startAngle <=  90 && endAngle >=  90) drawFastHLine(x - r + 1, y, w, fg_color); // Left
  if (startAngle <= 180 && endAngle >= 180) drawFastVLine(x, y - r + 1, w, fg_color); // Top
  if (startAngle <= 270 && endAngle >= 270) drawFastHLine(x + r - w, y, w, fg_color); // Right

  inTransaction = lockTransaction;
  end_tft_write();
}

/***************************************************************************************
** Function name:           drawSmoothCircle
** Description:             Draw a smooth circle
***************************************************************************************/
// To have effective anti-aliasing the circle will be 3 pixels thick
void TFT_eSPI::drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color)
{
  drawSmoothRoundRect(x-r, y-r, r, r-1, 0, 0, fg_color, bg_color);
}

/***************************************************************************************
** Function name:           fillSmoothCircle
** Description:             Draw a filled anti-aliased circle
***************************************************************************************/
void TFT_eSPI::fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color)
{
  if (r <= 0) return;

  inTransaction = true;

  drawFastHLine(x - r, y, 2 * r + 1, color);
  int32_t xs = 1;
  int32_t cx = 0;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;

  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      if (bg_color == 0x00FFFFFF) {
        drawPixel(x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(x - cx + r, y + cy - r, color, alpha, bg_color);
        drawPixel(x - cx + r, y - cy + r, color, alpha, bg_color);
        drawPixel(x + cx - r, y - cy + r, color, alpha, bg_color);
      }
      else {
        uint16_t pcol = drawPixel(x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(x - cx + r, y + cy - r, pcol);
        drawPixel(x - cx + r, y - cy + r, pcol);
        drawPixel(x + cx - r, y - cy + r, pcol);
      }
    }
    drawFastHLine(x + cx - r, y + cy - r, 2 * (r - cx) + 1, color);
    drawFastHLine(x + cx - r, y - cy + r, 2 * (r - cx) + 1, color);
  }
  inTransaction = lockTransaction;
  end_tft_write();
}


/***************************************************************************************
** Function name:           drawSmoothRoundRect
** Description:             Draw a rounded rectangle
***************************************************************************************/
// x,y is top left corner of bounding box for a complete rounded rectangle
// r = arc outer corner radius, ir = arc inner radius. Arc thickness = r-ir+1
// w and h are width and height of the bounding rectangle
// If w and h are < radius (e.g. 0,0) a circle will be drawn with centre at x+r,y+r
// Arc foreground fg_color anti-aliased with background colour at edges
// A subset of corners can be drawn by specifying a quadrants mask. A bit set in the
// mask means draw that quadrant (all are drawn if parameter missing):
//   0x1 | 0x2
//    ---¦---    Arc quadrant mask select bits (as in drawCircleHelper fn)
//   0x8 | 0x4
void TFT_eSPI::drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color, uint8_t quadrants)
{
  if (_vpOoB) return;
  if (r < ir) transpose(r, ir); // Required that r > ir
  if (r <= 0 || ir < 0) return; // Invalid

  w -= 2*r;
  h -= 2*r;

  if (w < 0) w = 0;
  if (h < 0) h = 0;

  inTransaction = true;

  x += r;
  y += r;

  uint16_t t = r - ir + 1;
  int32_t xs = 0;
  int32_t cx = 0;

  int32_t r2 = r * r;   // Outer arc radius^2
  r++;
  int32_t r1 = r * r;   // Outer AA zone radius^2

  int32_t r3 = ir * ir; // Inner arc radius^2
  ir--;
  int32_t r4 = ir * ir; // Inner AA zone radius^2

  uint8_t alpha = 0;

  // Scan top left quadrant x y r ir fg_color  bg_color
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t len = 0;  // Pixel run length
    int32_t lxst = 0; // Left side run x start
    int32_t rxst = 0; // Right side run x start
    int32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      int32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line lengths for each quadrant
      else if (hyp >= r3) {
        rxst = cx; // Right side start
        len++;     // Line segment length
        continue;  // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant - TODO
      uint16_t pcol = fastBlend(alpha, fg_color, bg_color);
      if (quadrants & 0x8) drawPixel(x + cx - r, y - cy + r + h, pcol);     // BL
      if (quadrants & 0x1) drawPixel(x + cx - r, y + cy - r, pcol);         // TL
      if (quadrants & 0x2) drawPixel(x - cx + r + w, y + cy - r, pcol);     // TR
      if (quadrants & 0x4) drawPixel(x - cx + r + w, y - cy + r + h, pcol); // BR
    }
    // Fill arc inner zone in each quadrant
    lxst = rxst - len + 1; // Calculate line segment start for left side
    if (quadrants & 0x8) drawFastHLine(x + lxst - r, y - cy + r + h, len, fg_color);     // BL
    if (quadrants & 0x1) drawFastHLine(x + lxst - r, y + cy - r, len, fg_color);         // TL
    if (quadrants & 0x2) drawFastHLine(x - rxst + r + w, y + cy - r, len, fg_color);     // TR
    if (quadrants & 0x4) drawFastHLine(x - rxst + r + w, y - cy + r + h, len, fg_color); // BR
  }

  // Draw sides
  if ((quadrants & 0xC) == 0xC) fillRect(x, y + r - t + h, w + 1, t, fg_color); // Bottom
  if ((quadrants & 0x9) == 0x9) fillRect(x - r + 1, y, t, h + 1, fg_color);     // Left
  if ((quadrants & 0x3) == 0x3) fillRect(x, y - r + 1, w + 1, t, fg_color);     // Top
  if ((quadrants & 0x6) == 0x6) fillRect(x + r - t + w, y, t, h + 1, fg_color); // Right

  inTransaction = lockTransaction;
  end_tft_write();
}

/***************************************************************************************
** Function name:           fillSmoothRoundRect
** Description:             Draw a filled anti-aliased rounded corner rectangle
***************************************************************************************/
void TFT_eSPI::fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, uint32_t bg_color)
{
  inTransaction = true;

  int32_t xs = 0;
  int32_t cx = 0;

  // Limit radius to half width or height
  if (r < 0)   r = 0;
  if (r > w/2) r = w/2;
  if (r > h/2) r = h/2;

  y += r;
  h -= 2*r;
  fillRect(x, y, w, h, color);

  h--;
  x += r;
  w -= 2*r+1;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;

  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      drawPixel(x + cx - r, y + cy - r, color, alpha, bg_color);
      drawPixel(x - cx + r + w, y + cy - r, color, alpha, bg_color);
      drawPixel(x - cx + r + w, y - cy + r + h, color, alpha, bg_color);
      drawPixel(x + cx - r, y - cy + r + h, color, alpha, bg_color);
    }
    drawFastHLine(x + cx - r, y + cy - r, 2 * (r - cx) + 1 + w, color);
    drawFastHLine(x + cx - r, y - cy + r + h, 2 * (r - cx) + 1 + w, color);
  }
  inTransaction = lockTransaction;
  end_tft_write();
}

/***************************************************************************************
** Function name:           drawSpot - maths intensive, so for small filled circles
** Description:             Draw an anti-aliased filled circle at ax,ay with radius r
***************************************************************************************/
// Coordinates are floating point to achieve sub-pixel positioning
void TFT_eSPI::drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color)
{
  // Filled circle can be created by the wide line function with zero line length
  drawWedgeLine( ax, ay, ax, ay, r, r, fg_color, bg_color);
}

/***************************************************************************************
** Function name:           drawWideLine - background colour specified or pixel read
** Description:             draw an anti-aliased line with rounded ends, width wd
***************************************************************************************/
void TFT_eSPI::drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color)
{
  drawWedgeLine( ax, ay, bx, by, wd/2.0, wd/2.0, fg_color, bg_color);
}

/***************************************************************************************
** Function name:           drawWedgeLine - background colour specified or pixel read
** Description:             draw an anti-aliased line with different width radiused ends
***************************************************************************************/
void TFT_eSPI::drawWedgeLine(float ax, float ay, float bx, float by, float ar, float br, uint32_t fg_color, uint32_t bg_color)
{
  if ( (ar < 0.0) || (br < 0.0) )return;
  if ( (fabsf(ax - bx) < 0.01f) && (fabsf(ay - by) < 0.01f) ) bx += 0.01f;  // Avoid divide by zero

  // Find line bounding box
  int32_t x0 = (int32_t)floorf(fminf(ax-ar, bx-br));
  int32_t x1 = (int32_t) ceilf(fmaxf(ax+ar, bx+br));
  int32_t y0 = (int32_t)floorf(fminf(ay-ar, by-br));
  int32_t y1 = (int32_t) ceilf(fmaxf(ay+ar, by+br));

  if (!clipWindow(&x0, &y0, &x1, &y1)) return;

  // Establish x start and y start
  int32_t ys = ay;
  if ((ax-ar)>(bx-br)) ys = by;

  float rdt = ar - br; // Radius delta
  float alpha = 1.0f;
  ar += 0.5;

  uint16_t bg = bg_color;
  float xpax, ypay, bax = bx - ax, bay = by - ay;

  begin_nin_write();
  inTransaction = true;

  int32_t xs = x0;
  // Scan bounding box from ys down, calculate pixel intensity from distance to line
  for (int32_t yp = ys; yp <= y1; yp++) {
    bool swin = true;  // Flag to start new window area
    bool endX = false; // Flag to skip pixels
    ypay = yp - ay;
    for (int32_t xp = xs; xp <= x1; xp++) {
      if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side
      xpax = xp - ax;
      alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
      if (alpha <= LoAlphaTheshold ) continue;
      // Track edge to minimise calculations
      if (!endX) { endX = true; xs = xp; }
      if (alpha > HiAlphaTheshold) {
        #ifdef GC9A01_DRIVER
          drawPixel(xp, yp, fg_color);
        #else
          if (swin) { setWindow(xp, yp, x1, yp); swin = false; }
          pushColor(fg_color);
        #endif
        continue;
      }
      //Blend color with background and plot
      if (bg_color == 0x00FFFFFF) {
        bg = readPixel(xp, yp); swin = true;
      }
      #ifdef GC9A01_DRIVER
        uint16_t pcol = fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
        drawPixel(xp, yp, pcol);
        swin = swin;
      #else
        if (swin) { setWindow(xp, yp, x1, yp); swin = false; }
        pushColor(fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
      #endif
    }
  }

  // Reset x start to left side of box
  xs = x0;
  // Scan bounding box from ys-1 up, calculate pixel intensity from distance to line
  for (int32_t yp = ys-1; yp >= y0; yp--) {
    bool swin = true;  // Flag to start new window area
    bool endX = false; // Flag to skip pixels
    ypay = yp - ay;
    for (int32_t xp = xs; xp <= x1; xp++) {
      if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side of drawn line
      xpax = xp - ax;
      alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
      if (alpha <= LoAlphaTheshold ) continue;
      // Track line boundary
      if (!endX) { endX = true; xs = xp; }
      if (alpha > HiAlphaTheshold) {
        #ifdef GC9A01_DRIVER
          drawPixel(xp, yp, fg_color);
        #else
          if (swin) { setWindow(xp, yp, x1, yp); swin = false; }
          pushColor(fg_color);
        #endif
        continue;
      }
      //Blend colour with background and plot
      if (bg_color == 0x00FFFFFF) {
        bg = readPixel(xp, yp); swin = true;
      }
      #ifdef GC9A01_DRIVER
        uint16_t pcol = fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
        drawPixel(xp, yp, pcol);
        swin = swin;
      #else
        if (swin) { setWindow(xp, yp, x1, yp); swin = false; }
        pushColor(fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg));
      #endif
    }
  }

  inTransaction = lockTransaction;
  end_nin_write();
}


/***************************************************************************************
** Function name:           lineDistance - private helper function for drawWedgeLine
** Description:             returns distance of px,py to closest part of a to b wedge
***************************************************************************************/
inline float TFT_eSPI::wedgeLineDistance(float xpax, float ypay, float bax, float bay, float dr)
{
  float h = fmaxf(fminf((xpax * bax + ypay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
  float dx = xpax - bax * h, dy = ypay - bay * h;
  return sqrtf(dx * dx + dy * dy) + h * dr;
}


/***************************************************************************************
** Function name:           drawFastVLine
** Description:             draw a vertical line
***************************************************************************************/
void TFT_eSPI::drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x < _vpX) || (x >= _vpW) || (y >= _vpH)) return;

  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((y + h) > _vpH) h = _vpH - y;

  if (h < 1) return;

  begin_tft_write();

  setWindow(x, y, x, y + h - 1);

  pushBlock(color, h);

  end_tft_write();
}


/***************************************************************************************
** Function name:           drawFastHLine
** Description:             draw a horizontal line
***************************************************************************************/
void TFT_eSPI::drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((y < _vpY) || (x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }

  if ((x + w) > _vpW) w = _vpW - x;

  if (w < 1) return;

  begin_tft_write();

  setWindow(x, y, x + w - 1, y);

  pushBlock(color, w);

  end_tft_write();
}


/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
***************************************************************************************/
void TFT_eSPI::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }
  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((x + w) > _vpW) w = _vpW - x;
  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  //Serial.print(" _xDatum=");Serial.print( _xDatum);Serial.print(", _yDatum=");Serial.print( _yDatum);
  //Serial.print(", _xWidth=");Serial.print(_xWidth);Serial.print(", _yHeight=");Serial.println(_yHeight);

  //Serial.print(" _vpX=");Serial.print( _vpX);Serial.print(", _vpY=");Serial.print( _vpY);
  //Serial.print(", _vpW=");Serial.print(_vpW);Serial.print(", _vpH=");Serial.println(_vpH);

  //Serial.print(" x=");Serial.print( y);Serial.print(", y=");Serial.print( y);
  //Serial.print(", w=");Serial.print(w);Serial.print(", h=");Serial.println(h);

  begin_tft_write();

  setWindow(x, y, x + w - 1, y + h - 1);

  pushBlock(color, w * h);

  end_tft_write();
}


/***************************************************************************************
** Function name:           fillRectVGradient
** Description:             draw a filled rectangle with a vertical colour gradient
***************************************************************************************/
void TFT_eSPI::fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }
  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((x + w) > _vpW) w = _vpW - x;
  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  begin_nin_write();

  float delta = -255.0/h;
  float alpha = 255.0;
  uint32_t color = color1;

  while (h--) {
    drawFastHLine(x, y++, w, color);
    alpha += delta;
    color = fastBlend((uint8_t)alpha, color1, color2);
  }

  end_nin_write();
}


/***************************************************************************************
** Function name:           fillRectHGradient
** Description:             draw a filled rectangle with a horizontal colour gradient
***************************************************************************************/
void TFT_eSPI::fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }
  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((x + w) > _vpW) w = _vpW - x;
  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  begin_nin_write();

  float delta = -255.0/w;
  float alpha = 255.0;
  uint32_t color = color1;

  while (w--) {
    drawFastVLine(x++, y, h, color);
    alpha += delta;
    color = fastBlend((uint8_t)alpha, color1, color2);
  }

  end_nin_write();
}


/***************************************************************************************
** Function name:           color565
** Description:             convert three 8-bit RGB levels to a 16-bit colour value
***************************************************************************************/
uint16_t TFT_eSPI::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


/***************************************************************************************
** Function name:           color16to8
** Description:             convert 16-bit colour to an 8-bit 332 RGB colour value
***************************************************************************************/
uint8_t TFT_eSPI::color16to8(uint16_t c)
{
  return ((c & 0xE000)>>8) | ((c & 0x0700)>>6) | ((c & 0x0018)>>3);
}


/***************************************************************************************
** Function name:           color8to16
** Description:             convert 8-bit colour to a 16-bit 565 colour value
***************************************************************************************/
uint16_t TFT_eSPI::color8to16(uint8_t color)
{
  uint8_t  blue[] = {0, 11, 21, 31}; // blue 2 to 5-bit colour lookup table
  uint16_t color16 = 0;

  //        =====Green=====     ===============Red==============
  color16  = (color & 0x1C)<<6 | (color & 0xC0)<<5 | (color & 0xE0)<<8;
  //        =====Green=====    =======Blue======
  color16 |= (color & 0x1C)<<3 | blue[color & 0x03];

  return color16;
}

/***************************************************************************************
** Function name:           color16to24
** Description:             convert 16-bit colour to a 24-bit 888 colour value
***************************************************************************************/
uint32_t TFT_eSPI::color16to24(uint16_t color565)
{
  uint8_t r = (color565 >> 8) & 0xF8; r |= (r >> 5);
  uint8_t g = (color565 >> 3) & 0xFC; g |= (g >> 6);
  uint8_t b = (color565 << 3) & 0xF8; b |= (b >> 5);

  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b << 0);
}

/***************************************************************************************
** Function name:           color24to16
** Description:             convert 24-bit colour to a 16-bit 565 colour value
***************************************************************************************/
uint32_t TFT_eSPI::color24to16(uint32_t color888)
{
  uint16_t r = (color888 >> 8) & 0xF800;
  uint16_t g = (color888 >> 5) & 0x07E0;
  uint16_t b = (color888 >> 3) & 0x001F;

  return (r | g | b);
}

/***************************************************************************************
** Function name:           invertDisplay
** Description:             invert the display colours i = 1 invert, i = 0 normal
***************************************************************************************/
void TFT_eSPI::invertDisplay(bool i)
{
  begin_tft_write();
  // Send the command twice as otherwise it does not always work!
  writecommand(i ? TFT_INVON : TFT_INVOFF);
  writecommand(i ? TFT_INVON : TFT_INVOFF);
  end_tft_write();
}


/**************************************************************************
** Function name:           setAttribute
** Description:             Sets a control parameter of an attribute
**************************************************************************/
void TFT_eSPI::setAttribute(uint8_t attr_id, uint8_t param) {
    switch (attr_id) {
            break;
        case CP437_SWITCH:
            _cp437 = param;
            break;
        case UTF8_SWITCH:
            _utf8  = param;
            decoderState = 0;
            break;
        //case 4: // TBD future feature control
        //    _tbd = param;
        //    break;
    }
}


/**************************************************************************
** Function name:           getAttribute
** Description:             Get value of an attribute (control parameter)
**************************************************************************/
uint8_t TFT_eSPI::getAttribute(uint8_t attr_id) {
    switch (attr_id) {
        case CP437_SWITCH: // ON/OFF control of full CP437 character set
            return _cp437;
        case UTF8_SWITCH: // ON/OFF control of UTF-8 decoding
            return _utf8;
        //case 3: // TBD future feature control
        //    return _tbd;
        //    break;
    }

    return false;
}

/***************************************************************************************
** Function name:           decodeUTF8
** Description:             Serial UTF-8 decoder with fall-back to extended ASCII
*************************************************************************************x*/
uint16_t TFT_eSPI::decodeUTF8(uint8_t c)
{
  if (!_utf8) return c;

  // 7-bit Unicode Code Point
  if ((c & 0x80) == 0x00) {
    decoderState = 0;
    return c;
  }

  if (decoderState == 0) {
    // 11-bit Unicode Code Point
    if ((c & 0xE0) == 0xC0) {
      decoderBuffer = ((c & 0x1F)<<6);
      decoderState = 1;
      return 0;
    }
    // 16-bit Unicode Code Point
    if ((c & 0xF0) == 0xE0) {
      decoderBuffer = ((c & 0x0F)<<12);
      decoderState = 2;
      return 0;
    }
    // 21-bit Unicode Code Point not supported so fall-back to extended ASCII
    // if ((c & 0xF8) == 0xF0) return c;
  }
  else {
    if (decoderState == 2) {
      decoderBuffer |= ((c & 0x3F)<<6);
      decoderState--;
      return 0;
    }
    else {
      decoderBuffer |= (c & 0x3F);
      decoderState = 0;
      return decoderBuffer;
    }
  }

  decoderState = 0;

  return c; // fall-back to extended ASCII
}


/***************************************************************************************
** Function name:           decodeUTF8
** Description:             Line buffer UTF-8 decoder with fall-back to extended ASCII
*************************************************************************************x*/
uint16_t TFT_eSPI::decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining)
{
  uint16_t c = buf[(*index)++];
  //Serial.print("Byte from string = 0x"); Serial.println(c, HEX);

  if (!_utf8) return c;

  // 7-bit Unicode
  if ((c & 0x80) == 0x00) return c;

  // 11-bit Unicode
  if (((c & 0xE0) == 0xC0) && (remaining > 1))
    return ((c & 0x1F)<<6) | (buf[(*index)++]&0x3F);

  // 16-bit Unicode
  if (((c & 0xF0) == 0xE0) && (remaining > 2)) {
    c = ((c & 0x0F)<<12) | ((buf[(*index)++]&0x3F)<<6);
    return  c | ((buf[(*index)++]&0x3F));
  }

  // 21-bit Unicode not supported so fall-back to extended ASCII
  // if (((c & 0xF8) == 0xF0) && (remaining > 3)) {
  // c = ((c & 0x07) << 18) | ((buf[(*index)++] & 0x03F) << 12);
  // c |= ((buf[(*index)++] & 0x3F) << 6);
  // return c | ((buf[(*index)++] & 0x3F));

  return c; // fall-back to extended ASCII
}


/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background
*************************************************************************************x*/
uint16_t TFT_eSPI::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
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

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 16bit foreground and background with dither
*************************************************************************************x*/
uint16_t TFT_eSPI::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither)
{
  if (dither) {
    int16_t alphaDither = (int16_t)alpha - dither + random(2*dither+1); // +/-4 randomised
    alpha = (uint8_t)alphaDither;
    if (alphaDither <  0) alpha = 0;
    if (alphaDither >255) alpha = 255;
  }

  return alphaBlend(alpha, fgc, bgc);
}

/***************************************************************************************
** Function name:           alphaBlend
** Description:             Blend 24bit foreground and background with optional dither
*************************************************************************************x*/
uint32_t TFT_eSPI::alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither)
{

  if (dither) {
    int16_t alphaDither = (int16_t)alpha - dither + random(2*dither+1); // +/-dither randomised
    alpha = (uint8_t)alphaDither;
    if (alphaDither <  0) alpha = 0;
    if (alphaDither >255) alpha = 255;
  }

  uint32_t rxx = bgc & 0xFF0000;
  rxx += ((fgc & 0xFF0000) - rxx) * alpha >> 8;
  uint32_t xgx = bgc & 0x00FF00;
  xgx += ((fgc & 0x00FF00) - xgx) * alpha >> 8;
  uint32_t xxb = bgc & 0x0000FF;
  xxb += ((fgc & 0x0000FF) - xxb) * alpha >> 8;
  return (rxx & 0xFF0000) | (xgx & 0x00FF00) | (xxb & 0x0000FF);
}

/***************************************************************************************
** Function name:           write
** Description:             draw characters piped through serial stream
***************************************************************************************/
/* // Not all processors support buffered write
#ifndef ARDUINO_ARCH_ESP8266 // Avoid ESP8266 board package bug
size_t TFT_eSPI::write(const uint8_t *buf, size_t len)
{
  inTransaction = true;

  uint8_t *lbuf = (uint8_t *)buf;
  while(*lbuf !=0 && len--) write(*lbuf++);

  inTransaction = lockTransaction;
  end_tft_write();
  return 1;
}
#endif
*/
/***************************************************************************************
** Function name:           write
** Description:             draw characters piped through serial stream
***************************************************************************************/
size_t TFT_eSPI::write(uint8_t utf8)
{
  if (_vpOoB) return 1;

  uint16_t uniCode = decodeUTF8(utf8);

  if (!uniCode) return 1;

  if (utf8 == '\r') return 1;

  if(fontLoaded) {
    if (uniCode < 32 && utf8 != '\n') return 1;
    drawGlyph(uniCode);
    return 1;
  }

  if (uniCode == '\n') uniCode+=22; // Make it a valid space character to stop errors

  uint16_t cwidth = 0;
  uint16_t cheight = 0;

  if (textfont==1) {
      cwidth =  6;
      cheight = 8;
  }

  cheight = cheight * textsize;

  if (utf8 == '\n') {
    cursor_y += cheight;
    cursor_x  = 0;
  }
  else {
    if (textwrapX && (cursor_x + cwidth * textsize > width())) {
      cursor_y += cheight;
      cursor_x = 0;
    }
    if (textwrapY && (cursor_y >= (int32_t) height())) cursor_y = 0;
    cursor_x += drawChar(uniCode, cursor_x, cursor_y, textfont);
  }

  return 1;
}


/***************************************************************************************
** Function name:           drawChar
** Description:             draw a Unicode glyph onto the screen
***************************************************************************************/
  // TODO: Rationalise with TFT_eSprite
  // Any UTF-8 decoding must be done before calling drawChar()
int16_t TFT_eSPI::drawChar(uint16_t uniCode, int32_t x, int32_t y)
{
  return drawChar(uniCode, x, y, textfont);
}

  // Any UTF-8 decoding must be done before calling drawChar()
int16_t TFT_eSPI::drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font)
{
  if (_vpOoB || !uniCode) return 0;

  if (font==1) return 0;
  if ((font>1) && (font<9) && ((uniCode < 32) || (uniCode > 127))) return 0;

  int32_t width  = 0;
  int32_t height = 0;
  uint32_t flash_address = 0;
  uniCode -= 32;

  int32_t xd = x + _xDatum;
  int32_t yd = y + _yDatum;

  if ((xd + width * textsize < _vpX || xd >= _vpW) && (yd + height * textsize < _vpY || yd >= _vpH)) return width * textsize ;

  int32_t w = width;
  int32_t pX      = 0;
  int32_t pY      = y;
  uint8_t line = 0;
  bool clip = xd < _vpX || xd + width  * textsize >= _vpW || yd < _vpY || yd + height * textsize >= _vpH;

  // Stop warnings
  flash_address = flash_address;
  w = w;
  pX = pX;
  pY = pY;
  line = line;
  clip = clip;

  return width * textsize;    // x +
}


/***************************************************************************************
** Function name:           drawString (with or without user defined font)
** Description :            draw string with padding if it is defined
***************************************************************************************/
// Without font number, uses font set by setTextFont()
int16_t TFT_eSPI::drawString(const String& string, int32_t poX, int32_t poY)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawString(buffer, poX, poY, textfont);
}
// With font number
int16_t TFT_eSPI::drawString(const String& string, int32_t poX, int32_t poY, uint8_t font)
{
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawString(buffer, poX, poY, font);
}

// Without font number, uses font set by setTextFont()
int16_t TFT_eSPI::drawString(const char *string, int32_t poX, int32_t poY)
{
  return drawString(string, poX, poY, textfont);
}

// With font number. Note: font number is over-ridden if a smooth font is loaded
int16_t TFT_eSPI::drawString(const char *string, int32_t poX, int32_t poY, uint8_t font)
{
  if (font > 8) return 0;

  int16_t sumX = 0;
  uint8_t padding = 1, baseline = 0;
  uint16_t cwidth = textWidth(string, font); // Find the pixel width of the string in the font
  uint16_t cheight = 8 * textsize;

  if(fontLoaded) {
    baseline = gFont.maxAscent;
    cheight  = fontHeight();
  } else if (font!=1) {
    baseline = pgm_read_byte( &fontdata[font].baseline ) * textsize;
    cheight = fontHeight(font);
  }

  if (textdatum || padX) {

    switch(textdatum) {
      case TC_DATUM:
        poX -= cwidth/2;
        padding += 1;
        break;
      case TR_DATUM:
        poX -= cwidth;
        padding += 2;
        break;
      case ML_DATUM:
        poY -= cheight/2;
        //padding += 0;
        break;
      case MC_DATUM:
        poX -= cwidth/2;
        poY -= cheight/2;
        padding += 1;
        break;
      case MR_DATUM:
        poX -= cwidth;
        poY -= cheight/2;
        padding += 2;
        break;
      case BL_DATUM:
        poY -= cheight;
        //padding += 0;
        break;
      case BC_DATUM:
        poX -= cwidth/2;
        poY -= cheight;
        padding += 1;
        break;
      case BR_DATUM:
        poX -= cwidth;
        poY -= cheight;
        padding += 2;
        break;
      case L_BASELINE:
        poY -= baseline;
        //padding += 0;
        break;
      case C_BASELINE:
        poX -= cwidth/2;
        poY -= baseline;
        padding += 1;
        break;
      case R_BASELINE:
        poX -= cwidth;
        poY -= baseline;
        padding += 2;
        break;
    }
  }


  int8_t xo = 0;

  uint16_t len = strlen(string);
  uint16_t n = 0;

  if(fontLoaded) {
    setCursor(poX, poY);

    bool fillbg = _fillbg;
    // If padding is requested then fill the text background
    if (padX && !_fillbg) _fillbg = true;

    while (n < len) {
      uint16_t uniCode = decodeUTF8((uint8_t*)string, &n, len - n);
      drawGlyph(uniCode);
    }
    _fillbg = fillbg; // restore state
    sumX += cwidth;
  } else {
    while (n < len) {
      uint16_t uniCode = decodeUTF8((uint8_t*)string, &n, len - n);
      sumX += drawChar(uniCode, poX+sumX, poY, font);
    }
  }

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv DEBUG vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Switch on debugging for the padding areas
//#define PADDING_DEBUG

#ifndef PADDING_DEBUG
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ DEBUG ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  if((padX>cwidth) && (textcolor!=textbgcolor)) {
    int16_t padXc = poX+cwidth+xo;
    switch(padding) {
      case 1:
        fillRect(padXc,poY,padX-cwidth,cheight, textbgcolor);
        break;
      case 2:
        fillRect(padXc,poY,(padX-cwidth)>>1,cheight, textbgcolor);
        padXc = poX - ((padX-cwidth)>>1);
        fillRect(padXc,poY,(padX-cwidth)>>1,cheight, textbgcolor);
        break;
      case 3:
        if (padXc>padX) padXc = padX;
        fillRect(poX + cwidth - padXc,poY,padXc-cwidth,cheight, textbgcolor);
        break;
    }
  }


#else

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv DEBUG vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// This is debug code to show text (green box) and blanked (white box) areas
// It shows that the padding areas are being correctly sized and positioned

  if((padX>sumX) && (textcolor!=textbgcolor)) {
    int16_t padXc = poX+sumX; // Maximum left side padding
    drawRect(poX,poY,sumX,cheight, TFT_GREEN);
    switch(padding) {
      case 1:
        drawRect(padXc,poY,padX-sumX,cheight, TFT_WHITE);
        break;
      case 2:
        drawRect(padXc,poY,(padX-sumX)>>1, cheight, TFT_WHITE);
        padXc = (padX-sumX)>>1;
        drawRect(poX - padXc,poY,(padX-sumX)>>1,cheight, TFT_WHITE);
        break;
      case 3:
        if (padXc>padX) padXc = padX;
        drawRect(poX + sumX - padXc,poY,padXc-sumX,cheight, TFT_WHITE);
        break;
    }
  }
#endif
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ DEBUG ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

return sumX;
}


/***************************************************************************************
** Function name:           drawNumber
** Description:             draw a long integer
***************************************************************************************/
int16_t TFT_eSPI::drawNumber(long long_num, int32_t poX, int32_t poY)
{
  isDigits = true; // Eliminate jiggle in monospaced fonts
  char str[12];
  ltoa(long_num, str, 10);
  return drawString(str, poX, poY, textfont);
}

int16_t TFT_eSPI::drawNumber(long long_num, int32_t poX, int32_t poY, uint8_t font)
{
  isDigits = true; // Eliminate jiggle in monospaced fonts
  char str[12];
  ltoa(long_num, str, 10);
  return drawString(str, poX, poY, font);
}


/***************************************************************************************
** Function name:           drawFloat
** Descriptions:            drawFloat, prints 7 non zero digits maximum
***************************************************************************************/
// Assemble and print a string, this permits alignment relative to a datum
// looks complicated but much more compact and actually faster than using print class
int16_t TFT_eSPI::drawFloat(float floatNumber, uint8_t dp, int32_t poX, int32_t poY)
{
  return drawFloat(floatNumber, dp, poX, poY, textfont);
}

int16_t TFT_eSPI::drawFloat(float floatNumber, uint8_t dp, int32_t poX, int32_t poY, uint8_t font)
{
  isDigits = true;
  char str[14];               // Array to contain decimal string
  uint8_t ptr = 0;            // Initialise pointer for array
  int8_t  digits = 1;         // Count the digits to avoid array overflow
  float rounding = 0.5;       // Round up down delta
  bool negative = false;

  if (dp > 7) dp = 7; // Limit the size of decimal portion

  // Adjust the rounding value
  for (uint8_t i = 0; i < dp; ++i) rounding /= 10.0;

  if (floatNumber < -rounding) {   // add sign, avoid adding - sign to 0.0!
    str[ptr++] = '-'; // Negative number
    str[ptr] = 0; // Put a null in the array as a precaution
    digits = 0;   // Set digits to 0 to compensate so pointer value can be used later
    floatNumber = -floatNumber; // Make positive
    negative = true;
  }

  floatNumber += rounding; // Round up or down

  if (dp == 0) {
    if (negative) floatNumber = -floatNumber;
    return drawNumber((long)floatNumber, poX, poY, font);
  }

  // For error put ... in string and return (all TFT_eSPI library fonts contain . character)
  if (floatNumber >= 2147483647) {
    strcpy(str, "...");
    return drawString(str, poX, poY, font);
  }
  // No chance of overflow from here on

  // Get integer part
  uint32_t temp = (uint32_t)floatNumber;

  // Put integer part into array
  ltoa(temp, str + ptr, 10);

  // Find out where the null is to get the digit count loaded
  while ((uint8_t)str[ptr] != 0) ptr++; // Move the pointer along
  digits += ptr;                  // Count the digits

  str[ptr++] = '.'; // Add decimal point
  str[ptr] = '0';   // Add a dummy zero
  str[ptr + 1] = 0; // Add a null but don't increment pointer so it can be overwritten

  // Get the decimal portion
  floatNumber = floatNumber - temp;

  // Get decimal digits one by one and put in array
  // Limit digit count so we don't get a false sense of resolution
  uint8_t i = 0;
  while ((i < dp) && (digits < 9)) { // while (i < dp) for no limit but array size must be increased
    i++;
    floatNumber *= 10;       // for the next decimal
    temp = floatNumber;      // get the decimal
    ltoa(temp, str + ptr, 10);
    ptr++; digits++;         // Increment pointer and digits count
    floatNumber -= temp;     // Remove that digit
  }

  // Finally we can plot the string and return pixel length
  return drawString(str, poX, poY, font);
}


/***************************************************************************************
** Function name:           setFreeFont
** Descriptions:            Sets the GFX free font to use
***************************************************************************************/

// Alternative to setTextFont() so we don't need two different named functions
void TFT_eSPI::setFreeFont(uint8_t font)
{
  setTextFont(font);
}


/***************************************************************************************
** Function name:           setTextFont
** Description:             Set the font for the print stream
***************************************************************************************/
void TFT_eSPI::setTextFont(uint8_t f)
{
  textfont = (f > 0) ? f : 1; // Don't allow font 0
  textfont = (f > 8) ? 1 : f; // Don't allow font > 8
}

/***************************************************************************************
** Function name:           getSPIinstance
** Description:             Get the instance of the SPI class
***************************************************************************************/
SPIClass& TFT_eSPI::getSPIinstance(void)
{
  return spi;
}


/***************************************************************************************
** Function name:           verifySetupID
** Description:             Compare the ID if USER_SETUP_ID defined in user setup file
***************************************************************************************/
bool TFT_eSPI::verifySetupID(uint32_t id)
{
#if defined (USER_SETUP_ID)
  if (USER_SETUP_ID == id) return true;
#else
  id = id; // Avoid warning
#endif
  return false;
}

/***************************************************************************************
** Function name:           getSetup
** Description:             Get the setup details for diagnostic and sketch access
***************************************************************************************/
void TFT_eSPI::getSetup(setup_t &tft_settings)
{
// tft_settings.version is set in header file

#if defined (USER_SETUP_INFO)
  tft_settings.setup_info = USER_SETUP_INFO;
#else
  tft_settings.setup_info = "NA";
#endif

#if defined (USER_SETUP_ID)
  tft_settings.setup_id = USER_SETUP_ID;
#else
  tft_settings.setup_id = 0;
#endif

#if defined (PROCESSOR_ID)
  tft_settings.esp = PROCESSOR_ID;
#else
  tft_settings.esp = -1;
#endif

  tft_settings.trans = true;

tft_settings.serial = true;
tft_settings.tft_spi_freq = spi_write_speed * 1000000/100000;
#ifdef SPI_READ_FREQUENCY
  tft_settings.tft_rd_freq = SPI_READ_FREQUENCY/100000;
#endif
#ifndef GENERIC_PROCESSOR
  #ifdef TFT_SPI_PORT
    tft_settings.port = TFT_SPI_PORT;
  #else
    tft_settings.port = 255;
  #endif
#endif
#ifdef RP2040_PIO_SPI
  tft_settings.interface = 0x10;
#else
  tft_settings.interface = 0x0;
#endif

#if defined(TFT_SPI_OVERLAP)
  tft_settings.overlap = true;
#else
  tft_settings.overlap = false;
#endif

  tft_settings.tft_driver = TFT_DRIVER;
  tft_settings.tft_width  = _init_width;
  tft_settings.tft_height = _init_height;

#ifdef CGRAM_OFFSET
  tft_settings.r0_x_offset = colstart;
  tft_settings.r0_y_offset = rowstart;
  tft_settings.r1_x_offset = 0;
  tft_settings.r1_y_offset = 0;
  tft_settings.r2_x_offset = 0;
  tft_settings.r2_y_offset = 0;
  tft_settings.r3_x_offset = 0;
  tft_settings.r3_y_offset = 0;
#else
  tft_settings.r0_x_offset = 0;
  tft_settings.r0_y_offset = 0;
  tft_settings.r1_x_offset = 0;
  tft_settings.r1_y_offset = 0;
  tft_settings.r2_x_offset = 0;
  tft_settings.r2_y_offset = 0;
  tft_settings.r3_x_offset = 0;
  tft_settings.r3_y_offset = 0;
#endif

#if defined (TFT_MOSI)
  tft_settings.pin_tft_mosi = TFT_MOSI;
#else
  tft_settings.pin_tft_mosi = -1;
#endif

#if defined (TFT_MISO)
  tft_settings.pin_tft_miso = TFT_MISO;
#else
  tft_settings.pin_tft_miso = -1;
#endif

#if defined (TFT_SCLK)
  tft_settings.pin_tft_clk  = TFT_SCLK;
#else
  tft_settings.pin_tft_clk  = -1;
#endif

#if defined (TFT_CS)
  tft_settings.pin_tft_cs   = TFT_CS;
#else
  tft_settings.pin_tft_cs   = -1;
#endif

#if defined (TFT_DC)
  tft_settings.pin_tft_dc  = TFT_DC;
#else
  tft_settings.pin_tft_dc  = -1;
#endif

#if defined (TFT_RD)
  tft_settings.pin_tft_rd  = TFT_RD;
#else
  tft_settings.pin_tft_rd  = -1;
#endif

#if defined (TFT_WR)
  tft_settings.pin_tft_wr  = TFT_WR;
#else
  tft_settings.pin_tft_wr  = -1;
#endif

#if defined (TFT_RST)
  tft_settings.pin_tft_rst = TFT_RST;
#else
  tft_settings.pin_tft_rst = -1;
#endif

#if defined (TFT_BL)
  tft_settings.pin_tft_led = TFT_BL;
#endif

#if defined (TFT_BACKLIGHT_ON)
  tft_settings.pin_tft_led_on = TFT_BACKLIGHT_ON;
#endif

  tft_settings.pin_tch_cs   = TOUCH_CS;
  tft_settings.tch_spi_freq = SPI_TOUCH_FREQUENCY/100000;
}

void TFT_eSPI::loadFont(const uint8_t array[])
{
  if (array == nullptr) return;
  fontPtr = (uint8_t*) array;
  loadFont("", false);
}

void TFT_eSPI::loadFont(String fontName, fs::FS &ffs)
{
  fontFS = ffs;
  loadFont(fontName, false);
}

void TFT_eSPI::loadFont(String fontName, bool flash)
{
  /*
    The vlw font format does not appear to be documented anywhere, so some reverse
    engineering has been applied!

    Header of vlw file comprises 6 uint32_t parameters (24 bytes total):
      1. The gCount (number of character glyphs)
      2. A version number (0xB = 11 for the one I am using)
      3. The font size (in points, not pixels)
      4. Deprecated mboxY parameter (typically set to 0)
      5. Ascent in pixels from baseline to top of "d"
      6. Descent in pixels from baseline to bottom of "p"

    Next are gCount sets of values for each glyph, each set comprises 7 int32t parameters (28 bytes):
      1. Glyph Unicode stored as a 32-bit value
      2. Height of bitmap bounding box
      3. Width of bitmap bounding box
      4. gxAdvance for cursor (setWidth in Processing)
      5. dY = distance from cursor baseline to top of glyph bitmap (signed value +ve = up)
      6. dX = distance from cursor to left side of glyph bitmap (signed value -ve = left)
      7. padding value, typically 0

    The bitmaps start next at 24 + (28 * gCount) bytes from the start of the file.
    Each pixel is 1 byte, an 8-bit Alpha value which represents the transparency from
    0xFF foreground colour, 0x00 background. The library uses a linear interpolation
    between the foreground and background RGB component colours. e.g.
        pixelRed = ((fgRed * alpha) + (bgRed * (255 - alpha))/255
    To gain a performance advantage fixed point arithmetic is used with rounding and
    division by 256 (shift right 8 bits is faster).

    After the bitmaps is:
       1 byte for font name string length (excludes null)
       a zero terminated character string giving the font name
       1 byte for Postscript name string length
       a zero/one terminated character string giving the font name
       last byte is 0 for non-anti-aliased and 1 for anti-aliased (smoothed)


    Glyph bitmap example is:
    // Cursor coordinate positions for this and next character are marked by 'C'
    // C<------- gxAdvance ------->C  gxAdvance is how far to move cursor for next glyph cursor position
    // |                           |
    // |                           |   ascent is top of "d", descent is bottom of "p"
    // +-- gdX --+             ascent
    // |         +-- gWidth--+     |   gdX is offset to left edge of glyph bitmap
    // |   +     x@.........@x  +  |   gdX may be negative e.g. italic "y" tail extending to left of
    // |   |     @@.........@@  |  |   cursor position, plot top left corner of bitmap at (cursorX + gdX)
    // |   |     @@.........@@ gdY |   gWidth and gHeight are glyph bitmap dimensions
    // |   |     .@@@.....@@@@  |  |
    // | gHeight ....@@@@@..@@  +  +    <-- baseline
    // |   |     ...........@@     |
    // |   |     ...........@@     |   gdY is the offset to the top edge of the bitmap
    // |   |     .@@.......@@. descent plot top edge of bitmap at (cursorY + ascent - gdY)
    // |   +     x..@@@@@@@..x     |   x marks the corner pixels of the bitmap
    // |                           |
    // +---------------------------+   yAdvance is y delta for the next line, font size or (ascent + descent)
    //                                 some fonts can overlay in y direction so may need a user adjust value

  */

  if (fontLoaded) unloadFont();

  if (fontName == "") fs_font = false;
  else { fontPtr = nullptr; fs_font = true; }

  if (fs_font) {
    spiffs = flash; // true if font is in SPIFFS

    if(spiffs) fontFS = SPIFFS;

    // Avoid a crash on the ESP32 if the file does not exist
    if (fontFS.exists("/" + fontName + ".vlw") == false) {
      Serial.println("Font file " + fontName + " not found!");
      return;
    }

    fontFile = fontFS.open( "/" + fontName + ".vlw", "r");

    if(!fontFile) return;

    fontFile.seek(0, fs::SeekSet);
  }

  gFont.gArray   = (const uint8_t*)fontPtr;

  gFont.gCount   = (uint16_t)readInt32(); // glyph count in file
                             readInt32(); // vlw encoder version - discard
  gFont.yAdvance = (uint16_t)readInt32(); // Font size in points, not pixels
                             readInt32(); // discard
  gFont.ascent   = (uint16_t)readInt32(); // top of "d"
  gFont.descent  = (uint16_t)readInt32(); // bottom of "p"

  // These next gFont values might be updated when the Metrics are fetched
  gFont.maxAscent  = gFont.ascent;   // Determined from metrics
  gFont.maxDescent = gFont.descent;  // Determined from metrics
  gFont.yAdvance   = gFont.ascent + gFont.descent;
  gFont.spaceWidth = gFont.yAdvance / 4;  // Guess at space width

  fontLoaded = true;

  // Fetch the metrics for each glyph
  loadMetrics();
}


void TFT_eSPI::loadMetrics(void)
{
  uint32_t headerPtr = 24;
  uint32_t bitmapPtr = headerPtr + gFont.gCount * 28;

  {
    gUnicode  = (uint16_t*)malloc( gFont.gCount * 2); // Unicode 16-bit Basic Multilingual Plane (0-FFFF)
    gHeight   =  (uint8_t*)malloc( gFont.gCount );    // Height of glyph
    gWidth    =  (uint8_t*)malloc( gFont.gCount );    // Width of glyph
    gxAdvance =  (uint8_t*)malloc( gFont.gCount );    // xAdvance - to move x cursor
    gdY       =  (int16_t*)malloc( gFont.gCount * 2); // offset from bitmap top edge from lowest point in any character
    gdX       =   (int8_t*)malloc( gFont.gCount );    // offset for bitmap left edge relative to cursor X
    gBitmap   = (uint32_t*)malloc( gFont.gCount * 4); // seek pointer to glyph bitmap in the file
  }

  if (fs_font) fontFile.seek(headerPtr, fs::SeekSet);

  uint16_t gNum = 0;

  while (gNum < gFont.gCount)
  {
    gUnicode[gNum]  = (uint16_t)readInt32(); // Unicode code point value
    gHeight[gNum]   =  (uint8_t)readInt32(); // Height of glyph
    gWidth[gNum]    =  (uint8_t)readInt32(); // Width of glyph
    gxAdvance[gNum] =  (uint8_t)readInt32(); // xAdvance - to move x cursor
    gdY[gNum]       =  (int16_t)readInt32(); // y delta from baseline
    gdX[gNum]       =   (int8_t)readInt32(); // x delta from cursor
    readInt32(); // ignored

    //Serial.print("Unicode = 0x"); Serial.print(gUnicode[gNum], HEX); Serial.print(", gHeight  = "); Serial.println(gHeight[gNum]);
    //Serial.print("Unicode = 0x"); Serial.print(gUnicode[gNum], HEX); Serial.print(", gWidth  = "); Serial.println(gWidth[gNum]);
    //Serial.print("Unicode = 0x"); Serial.print(gUnicode[gNum], HEX); Serial.print(", gxAdvance  = "); Serial.println(gxAdvance[gNum]);
    //Serial.print("Unicode = 0x"); Serial.print(gUnicode[gNum], HEX); Serial.print(", gdY  = "); Serial.println(gdY[gNum]);

    // Different glyph sets have different ascent values not always based on "d", so we could get
    // the maximum glyph ascent by checking all characters. BUT this method can generate bad values
    // for non-existent glyphs, so we will reply on processing for the value and disable this code for now...
    /*
    if (gdY[gNum] > gFont.maxAscent)
    {
      // Try to avoid UTF coding values and characters that tend to give duff values
      if (((gUnicode[gNum] > 0x20) && (gUnicode[gNum] < 0x7F)) || (gUnicode[gNum] > 0xA0))
      {
        gFont.maxAscent   = gdY[gNum];
#ifdef SHOW_ASCENT_DESCENT
        Serial.print("Unicode = 0x"); Serial.print(gUnicode[gNum], HEX); Serial.print(", maxAscent  = "); Serial.println(gFont.maxAscent);
#endif
      }
    }
    */

    // Different glyph sets have different descent values not always based on "p", so get maximum glyph descent
    if (((int16_t)gHeight[gNum] - (int16_t)gdY[gNum]) > gFont.maxDescent)
    {
      // Avoid UTF coding values and characters that tend to give duff values
      if (((gUnicode[gNum] > 0x20) && (gUnicode[gNum] < 0xA0) && (gUnicode[gNum] != 0x7F)) || (gUnicode[gNum] > 0xFF))
      {
        gFont.maxDescent   = gHeight[gNum] - gdY[gNum];
      }
    }

    gBitmap[gNum] = bitmapPtr;

    bitmapPtr += gWidth[gNum] * gHeight[gNum];

    gNum++;
    yield();
  }

  gFont.yAdvance = gFont.maxAscent + gFont.maxDescent;

  gFont.spaceWidth = (gFont.ascent + gFont.descent) * 2/7;  // Guess at space width
}


/***************************************************************************************
** Function name:           deleteMetrics
** Description:             Delete the old glyph metrics and free up the memory
*************************************************************************************x*/
void TFT_eSPI::unloadFont( void )
{
  if (gUnicode)
  {
    free(gUnicode);
    gUnicode = NULL;
  }

  if (gHeight)
  {
    free(gHeight);
    gHeight = NULL;
  }

  if (gWidth)
  {
    free(gWidth);
    gWidth = NULL;
  }

  if (gxAdvance)
  {
    free(gxAdvance);
    gxAdvance = NULL;
  }

  if (gdY)
  {
    free(gdY);
    gdY = NULL;
  }

  if (gdX)
  {
    free(gdX);
    gdX = NULL;
  }

  if (gBitmap)
  {
    free(gBitmap);
    gBitmap = NULL;
  }

  gFont.gArray = nullptr;

  if (fs_font && fontFile) fontFile.close();

  fontLoaded = false;
}


/***************************************************************************************
** Function name:           readInt32
** Description:             Get a 32-bit integer from the font file
*************************************************************************************x*/
uint32_t TFT_eSPI::readInt32(void)
{
  uint32_t val = 0;

  if (fs_font) {
    val  = (uint32_t)fontFile.read() << 24;
    val |= (uint32_t)fontFile.read() << 16;
    val |= (uint32_t)fontFile.read() << 8;
    val |= (uint32_t)fontFile.read();
  }
  else
  {
    val  = (uint32_t)pgm_read_byte(fontPtr++) << 24;
    val |= (uint32_t)pgm_read_byte(fontPtr++) << 16;
    val |= (uint32_t)pgm_read_byte(fontPtr++) << 8;
    val |= (uint32_t)pgm_read_byte(fontPtr++);
  }

  return val;
}

bool TFT_eSPI::getUnicodeIndex(uint16_t unicode, uint16_t *index)
{
  for (uint16_t i = 0; i < gFont.gCount; i++)
  {
    if (gUnicode[i] == unicode)
    {
      *index = i;
      return true;
    }
  }
  return false;
}

void TFT_eSPI::drawGlyph(uint16_t code)
{
  uint16_t fg = textcolor;
  uint16_t bg = textbgcolor;

  // Check if cursor has moved
  if (last_cursor_x != cursor_x)
  {
    bg_cursor_x = cursor_x;
    last_cursor_x = cursor_x;
  }

  if (code < 0x21)
  {
    if (code == 0x20) {
      if (_fillbg) fillRect(bg_cursor_x, cursor_y, (cursor_x + gFont.spaceWidth) - bg_cursor_x, gFont.yAdvance, bg);
      cursor_x += gFont.spaceWidth;
      bg_cursor_x = cursor_x;
      last_cursor_x = cursor_x;
      return;
    }

    if (code == '\n') {
      cursor_x = 0;
      bg_cursor_x = 0;
      last_cursor_x = 0;
      cursor_y += gFont.yAdvance;
      if (textwrapY && (cursor_y >= height())) cursor_y = 0;
      return;
    }
  }

  uint16_t gNum = 0;
  bool found = getUnicodeIndex(code, &gNum);
  
  if (found)
  {

    if (textwrapX && (cursor_x + gWidth[gNum] + gdX[gNum] > width()))
    {
      cursor_y += gFont.yAdvance;
      cursor_x = 0;
      bg_cursor_x = 0;
    }
    if (textwrapY && ((cursor_y + gFont.yAdvance) >= height())) cursor_y = 0;
    if (cursor_x == 0) cursor_x -= gdX[gNum];

    uint8_t* pbuffer = nullptr;
    const uint8_t* gPtr = (const uint8_t*) gFont.gArray;

    if (fs_font)
    {
      fontFile.seek(gBitmap[gNum], fs::SeekSet);
      pbuffer =  (uint8_t*)malloc(gWidth[gNum]);
    }

    int16_t cy = cursor_y + gFont.maxAscent - gdY[gNum];
    int16_t cx = cursor_x + gdX[gNum];

    //  if (cx > width() && bg_cursor_x > width()) return;
    //  if (cursor_y > height()) return;

    int16_t  fxs = cx;
    uint32_t fl = 0;
    int16_t  bxs = cx;
    uint32_t bl = 0;
    int16_t  bx = 0;
    uint8_t pixel;

    startWrite(); // Avoid slow ESP32 transaction overhead for every pixel

    int16_t fillwidth  = 0;
    int16_t fillheight = 0;

    // Fill area above glyph
    if (_fillbg) {
      fillwidth  = (cursor_x + gxAdvance[gNum]) - bg_cursor_x;
      if (fillwidth > 0) {
        fillheight = gFont.maxAscent - gdY[gNum];
        // Could be negative
        if (fillheight > 0) {
          fillRect(bg_cursor_x, cursor_y, fillwidth, fillheight, textbgcolor);
        }
      }
      else {
        // Could be negative
        fillwidth = 0;
      }

      // Fill any area to left of glyph                              
      if (bg_cursor_x < cx) fillRect(bg_cursor_x, cy, cx - bg_cursor_x, gHeight[gNum], textbgcolor);
      // Set x position in glyph area where background starts
      if (bg_cursor_x > cx) bx = bg_cursor_x - cx;
      // Fill any area to right of glyph
      if (cx + gWidth[gNum] < cursor_x + gxAdvance[gNum]) {
        fillRect(cx + gWidth[gNum], cy, (cursor_x + gxAdvance[gNum]) - (cx + gWidth[gNum]), gHeight[gNum], textbgcolor);
      }
    }

    for (int32_t y = 0; y < gHeight[gNum]; y++)
    {
      if (fs_font) {
        if (spiffs)
        {
          fontFile.read(pbuffer, gWidth[gNum]);
          //Serial.println("SPIFFS");
        }
        else
        {
          endWrite();    // Release SPI for SD card transaction
          fontFile.read(pbuffer, gWidth[gNum]);
          startWrite();  // Re-start SPI for TFT transaction
          //Serial.println("Not SPIFFS");
        }
      }

      for (int32_t x = 0; x < gWidth[gNum]; x++)
      {
        if (fs_font) pixel = pbuffer[x];
        else
        pixel = pgm_read_byte(gPtr + gBitmap[gNum] + x + gWidth[gNum] * y);

        if (pixel)
        {
          if (bl) { drawFastHLine( bxs, y + cy, bl, bg); bl = 0; }
          if (pixel != 0xFF)
          {
            if (fl) {
              if (fl==1) drawPixel(fxs, y + cy, fg);
              else drawFastHLine( fxs, y + cy, fl, fg);
              fl = 0;
            }
            if (getColor) bg = getColor(x + cx, y + cy);
            drawPixel(x + cx, y + cy, alphaBlend(pixel, fg, bg));
          }
          else
          {
            if (fl==0) fxs = x + cx;
            fl++;
          }
        }
        else
        {
          if (fl) { drawFastHLine( fxs, y + cy, fl, fg); fl = 0; }
          if (_fillbg) {
            if (x >= bx) {
              if (bl==0) bxs = x + cx;
              bl++;
            }
          }
        }
      }
      if (fl) { drawFastHLine( fxs, y + cy, fl, fg); fl = 0; }
      if (bl) { drawFastHLine( bxs, y + cy, bl, bg); bl = 0; }
    }

    // Fill area below glyph
    if (fillwidth > 0) {
      fillheight = (cursor_y + gFont.yAdvance) - (cy + gHeight[gNum]);
      if (fillheight > 0) {
        fillRect(bg_cursor_x, cy + gHeight[gNum], fillwidth, fillheight, textbgcolor);
      }
    }

    if (pbuffer) free(pbuffer);
    cursor_x += gxAdvance[gNum];
    endWrite();
  }
  else
  {
    // Point code not in font so draw a rectangle and move on cursor
    drawRect(cursor_x, cursor_y + gFont.maxAscent - gFont.ascent, gFont.spaceWidth, gFont.ascent, fg);
    cursor_x += gFont.spaceWidth + 1;
  }
  bg_cursor_x = cursor_x;
  last_cursor_x = cursor_x;
}

/***************************************************************************************
** Function name:           showFont
** Description:             Page through all characters in font, td ms between screens
*************************************************************************************x*/
void TFT_eSPI::showFont(uint32_t td)
{
  if(!fontLoaded) return;

  int16_t cursorX = width(); // Force start of new page to initialise cursor
  int16_t cursorY = height();// for the first character
  uint32_t timeDelay = 0;    // No delay before first page

  fillScreen(textbgcolor);
  
  for (uint16_t i = 0; i < gFont.gCount; i++) {
    // Check if this will need a new screen
    if (cursorX + gdX[i] + gWidth[i] >= width())  {
      cursorX = -gdX[i];

      cursorY += gFont.yAdvance;
      if (cursorY + gFont.maxAscent + gFont.descent >= height()) {
        cursorX = -gdX[i];
        cursorY = 0;
        delay(timeDelay);
        timeDelay = td;
        fillScreen(textbgcolor);
      }
    }

    setCursor(cursorX, cursorY);
    drawGlyph(gUnicode[i]);
    cursorX += gxAdvance[i];
    yield();
  }

  delay(timeDelay);
  fillScreen(textbgcolor);
}

// The following touch screen support code by maxpautsch was merged 1/10/17
// https://github.com/maxpautsch

// A demo is provided in examples Generic folder

// Additions by Bodmer to double sample, use Z value to improve detection reliability
// and to correct rotation handling

// See license in root directory.

// Define a default pressure threshold
#ifndef Z_THRESHOLD
  #define Z_THRESHOLD 350 // Touch pressure threshold for validating touches
#endif

/***************************************************************************************
** Function name:           begin_touch_read_write - was spi_begin_touch
** Description:             Start transaction and select touch controller
***************************************************************************************/
// The touch controller has a low SPI clock rate
inline void TFT_eSPI::begin_touch_read_write(void){
  dmaWait();
  CS_H; // Just in case it has been left low
  if (locked) {locked = false; spi.beginTransaction(SPISettings(SPI_TOUCH_FREQUENCY, MSBFIRST, SPI_MODE0));}
  SET_BUS_READ_MODE;
  digitalWrite(TOUCH_CS, LOW);
}

/***************************************************************************************
** Function name:           end_touch_read_write - was spi_end_touch
** Description:             End transaction and deselect touch controller
***************************************************************************************/
inline void TFT_eSPI::end_touch_read_write(void){
  digitalWrite(TOUCH_CS, HIGH);
  if(!inTransaction) {if (!locked) {locked = true; spi.endTransaction();}}
}

/***************************************************************************************
** Function name:           Legacy - deprecated
** Description:             Start/end transaction
***************************************************************************************/
void TFT_eSPI::spi_begin_touch() {begin_touch_read_write();}
void TFT_eSPI::spi_end_touch()   {  end_touch_read_write();}

/***************************************************************************************
** Function name:           getTouchRaw
** Description:             read raw touch position.  Always returns true.
***************************************************************************************/
uint8_t TFT_eSPI::getTouchRaw(uint16_t *x, uint16_t *y){
  uint16_t tmp;

  begin_touch_read_write();
  
  // Start YP sample request for x position, read 4 times and keep last sample
  spi.transfer(0xd0);                    // Start new YP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0xd0);                    // Read last 8 bits and start new YP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0xd0);                    // Read last 8 bits and start new YP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0xd0);                    // Read last 8 bits and start new YP conversion

  tmp = spi.transfer(0);                   // Read first 8 bits
  tmp = tmp <<5;
  tmp |= 0x1f & (spi.transfer(0x90)>>3);   // Read last 8 bits and start new XP conversion

  *x = tmp;

  // Start XP sample request for y position, read 4 times and keep last sample
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0x90);                    // Read last 8 bits and start new XP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0x90);                    // Read last 8 bits and start new XP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0x90);                    // Read last 8 bits and start new XP conversion

  tmp = spi.transfer(0);                 // Read first 8 bits
  tmp = tmp <<5;
  tmp |= 0x1f & (spi.transfer(0)>>3);    // Read last 8 bits

  *y = tmp;

  end_touch_read_write();

  return true;
}

/***************************************************************************************
** Function name:           getTouchRawZ
** Description:             read raw pressure on touchpad and return Z value. 
***************************************************************************************/
uint16_t TFT_eSPI::getTouchRawZ(void){

  begin_touch_read_write();

  // Z sample request
  int16_t tz = 0xFFF;
  spi.transfer(0xb0);               // Start new Z1 conversion
  tz += spi.transfer16(0xc0) >> 3;  // Read Z1 and start Z2 conversion
  tz -= spi.transfer16(0x00) >> 3;  // Read Z2

  end_touch_read_write();

  if (tz == 4095) tz = 0;

  return (uint16_t)tz;
}

/***************************************************************************************
** Function name:           validTouch
** Description:             read validated position. Return false if not pressed. 
***************************************************************************************/
#define _RAWERR 20 // Deadband error allowed in successive position samples
uint8_t TFT_eSPI::validTouch(uint16_t *x, uint16_t *y, uint16_t threshold){
  uint16_t x_tmp, y_tmp, x_tmp2, y_tmp2;

  // Wait until pressure stops increasing to debounce pressure
  uint16_t z1 = 1;
  uint16_t z2 = 0;
  while (z1 > z2)
  {
    z2 = z1;
    z1 = getTouchRawZ();
    delay(1);
  }

  //  Serial.print("Z = ");Serial.println(z1);

  if (z1 <= threshold) return false;
    
  getTouchRaw(&x_tmp,&y_tmp);

  //  Serial.print("Sample 1 x,y = "); Serial.print(x_tmp);Serial.print(",");Serial.print(y_tmp);
  //  Serial.print(", Z = ");Serial.println(z1);

  delay(1); // Small delay to the next sample
  if (getTouchRawZ() <= threshold) return false;

  delay(2); // Small delay to the next sample
  getTouchRaw(&x_tmp2,&y_tmp2);
  
  //  Serial.print("Sample 2 x,y = "); Serial.print(x_tmp2);Serial.print(",");Serial.println(y_tmp2);
  //  Serial.print("Sample difference = ");Serial.print(abs(x_tmp - x_tmp2));Serial.print(",");Serial.println(abs(y_tmp - y_tmp2));

  if (abs(x_tmp - x_tmp2) > _RAWERR) return false;
  if (abs(y_tmp - y_tmp2) > _RAWERR) return false;
  
  *x = x_tmp;
  *y = y_tmp;
  
  return true;
}
  
/***************************************************************************************
** Function name:           getTouch
** Description:             read callibrated position. Return false if not pressed. 
***************************************************************************************/
uint8_t TFT_eSPI::getTouch(uint16_t *x, uint16_t *y, uint16_t threshold){
  uint16_t x_tmp, y_tmp;
  
  if (threshold<20) threshold = 20;
  if (_pressTime > millis()) threshold=20;

  uint8_t n = 5;
  uint8_t valid = 0;
  while (n--)
  {
    if (validTouch(&x_tmp, &y_tmp, threshold)) valid++;;
  }

  if (valid<1) { _pressTime = 0; return false; }
  
  _pressTime = millis() + 50;

  convertRawXY(&x_tmp, &y_tmp);

  if (x_tmp >= _width || y_tmp >= _height) return false;

  _pressX = x_tmp;
  _pressY = y_tmp;
  *x = _pressX;
  *y = _pressY;
  return valid;
}

/***************************************************************************************
** Function name:           convertRawXY
** Description:             convert raw touch x,y values to screen coordinates 
***************************************************************************************/
void TFT_eSPI::convertRawXY(uint16_t *x, uint16_t *y)
{
  uint16_t x_tmp = *x, y_tmp = *y, xx, yy;

  if(!touchCalibration_rotate){
    xx=(x_tmp-touchCalibration_x0)*_width/touchCalibration_x1;
    yy=(y_tmp-touchCalibration_y0)*_height/touchCalibration_y1;
    if(touchCalibration_invert_x)
      xx = _width - xx;
    if(touchCalibration_invert_y)
      yy = _height - yy;
  } else {
    xx=(y_tmp-touchCalibration_x0)*_width/touchCalibration_x1;
    yy=(x_tmp-touchCalibration_y0)*_height/touchCalibration_y1;
    if(touchCalibration_invert_x)
      xx = _width - xx;
    if(touchCalibration_invert_y)
      yy = _height - yy;
  }
  *x = xx;
  *y = yy;
}

/***************************************************************************************
** Function name:           calibrateTouch
** Description:             generates calibration parameters for touchscreen. 
***************************************************************************************/
void TFT_eSPI::calibrateTouch(uint16_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size){
  int16_t values[] = {0,0,0,0,0,0,0,0};
  uint16_t x_tmp, y_tmp;



  for(uint8_t i = 0; i<4; i++){
    fillRect(0, 0, size+1, size+1, color_bg);
    fillRect(0, _height-size-1, size+1, size+1, color_bg);
    fillRect(_width-size-1, 0, size+1, size+1, color_bg);
    fillRect(_width-size-1, _height-size-1, size+1, size+1, color_bg);

    if (i == 5) break; // used to clear the arrows
    
    switch (i) {
      case 0: // up left
        drawLine(0, 0, 0, size, color_fg);
        drawLine(0, 0, size, 0, color_fg);
        drawLine(0, 0, size , size, color_fg);
        break;
      case 1: // bot left
        drawLine(0, _height-size-1, 0, _height-1, color_fg);
        drawLine(0, _height-1, size, _height-1, color_fg);
        drawLine(size, _height-size-1, 0, _height-1 , color_fg);
        break;
      case 2: // up right
        drawLine(_width-size-1, 0, _width-1, 0, color_fg);
        drawLine(_width-size-1, size, _width-1, 0, color_fg);
        drawLine(_width-1, size, _width-1, 0, color_fg);
        break;
      case 3: // bot right
        drawLine(_width-size-1, _height-size-1, _width-1, _height-1, color_fg);
        drawLine(_width-1, _height-1-size, _width-1, _height-1, color_fg);
        drawLine(_width-1-size, _height-1, _width-1, _height-1, color_fg);
        break;
      }

    // user has to get the chance to release
    if(i>0) delay(1000);

    for(uint8_t j= 0; j<8; j++){
      // Use a lower detect threshold as corners tend to be less sensitive
      while(!validTouch(&x_tmp, &y_tmp, Z_THRESHOLD/2));
      values[i*2  ] += x_tmp;
      values[i*2+1] += y_tmp;
      }
    values[i*2  ] /= 8;
    values[i*2+1] /= 8;
  }


  // from case 0 to case 1, the y value changed. 
  // If the measured delta of the touch x axis is bigger than the delta of the y axis, the touch and TFT axes are switched.
  touchCalibration_rotate = false;
  if(abs(values[0]-values[2]) > abs(values[1]-values[3])){
    touchCalibration_rotate = true;
    touchCalibration_x0 = (values[1] + values[3])/2; // calc min x
    touchCalibration_x1 = (values[5] + values[7])/2; // calc max x
    touchCalibration_y0 = (values[0] + values[4])/2; // calc min y
    touchCalibration_y1 = (values[2] + values[6])/2; // calc max y
  } else {
    touchCalibration_x0 = (values[0] + values[2])/2; // calc min x
    touchCalibration_x1 = (values[4] + values[6])/2; // calc max x
    touchCalibration_y0 = (values[1] + values[5])/2; // calc min y
    touchCalibration_y1 = (values[3] + values[7])/2; // calc max y
  }

  // in addition, the touch screen axis could be in the opposite direction of the TFT axis
  touchCalibration_invert_x = false;
  if(touchCalibration_x0 > touchCalibration_x1){
    values[0]=touchCalibration_x0;
    touchCalibration_x0 = touchCalibration_x1;
    touchCalibration_x1 = values[0];
    touchCalibration_invert_x = true;
  }
  touchCalibration_invert_y = false;
  if(touchCalibration_y0 > touchCalibration_y1){
    values[0]=touchCalibration_y0;
    touchCalibration_y0 = touchCalibration_y1;
    touchCalibration_y1 = values[0];
    touchCalibration_invert_y = true;
  }

  // pre calculate
  touchCalibration_x1 -= touchCalibration_x0;
  touchCalibration_y1 -= touchCalibration_y0;

  if(touchCalibration_x0 == 0) touchCalibration_x0 = 1;
  if(touchCalibration_x1 == 0) touchCalibration_x1 = 1;
  if(touchCalibration_y0 == 0) touchCalibration_y0 = 1;
  if(touchCalibration_y1 == 0) touchCalibration_y1 = 1;

  // export parameters, if pointer valid
  if(parameters != NULL){
    parameters[0] = touchCalibration_x0;
    parameters[1] = touchCalibration_x1;
    parameters[2] = touchCalibration_y0;
    parameters[3] = touchCalibration_y1;
    parameters[4] = touchCalibration_rotate | (touchCalibration_invert_x <<1) | (touchCalibration_invert_y <<2);
  }
}


/***************************************************************************************
** Function name:           setTouch
** Description:             imports calibration parameters for touchscreen. 
***************************************************************************************/
void TFT_eSPI::setTouch(uint16_t *parameters){
  touchCalibration_x0 = parameters[0];
  touchCalibration_x1 = parameters[1];
  touchCalibration_y0 = parameters[2];
  touchCalibration_y1 = parameters[3];

  if(touchCalibration_x0 == 0) touchCalibration_x0 = 1;
  if(touchCalibration_x1 == 0) touchCalibration_x1 = 1;
  if(touchCalibration_y0 == 0) touchCalibration_y0 = 1;
  if(touchCalibration_y1 == 0) touchCalibration_y1 = 1;

  touchCalibration_rotate = parameters[4] & 0x01;
  touchCalibration_invert_x = parameters[4] & 0x02;
  touchCalibration_invert_y = parameters[4] & 0x04;
}

TFT_eSprite::TFT_eSprite(TFT_eSPI *tft)
{
  _tft = tft;     // Pointer to tft class so we can call member functions

  _iwidth    = 0; // Initialise width and height to 0 (it does not exist yet)
  _iheight   = 0;
  _bpp = 16;
  _swapBytes = false;   // Do not swap pushImage colour bytes by default

  _created = false;
  _vpOoB   = true;

  _xs = 0;  // window bounds for pushColor
  _ys = 0;
  _xe = 0;
  _ye = 0;

  _xptr = 0; // pushColor coordinate
  _yptr = 0;

  _colorMap = nullptr;
  
  // Ensure end_tft_write() does nothing in inherited functions.
  lockTransaction = true;
}


/***************************************************************************************
** Function name:           createSprite
** Description:             Create a sprite (bitmap) of defined width and height
***************************************************************************************/
// cast returned value to (uint8_t*) for 8-bit or (uint16_t*) for 16-bit colours
void* TFT_eSprite::createSprite(int16_t w, int16_t h, uint8_t frames)
{

  if ( _created ) return _img8_1;

  if ( w < 1 || h < 1 ) return nullptr;

  _iwidth  = _dwidth  = _bitwidth = w;
  _iheight = _dheight = h;

  cursor_x = 0;
  cursor_y = 0;

  // Default scroll rectangle and gap fill colour
  _sx = 0;
  _sy = 0;
  _sw = w;
  _sh = h;
  _scolor = TFT_BLACK;

  _img8   = (uint8_t*) callocSprite(w, h, frames);
  _img8_1 = _img8;
  _img8_2 = _img8;
  _img    = (uint16_t*) _img8;
  _img4   = _img8;

  if ( (_bpp == 16) && (frames > 1) ) {
    _img8_2 = _img8 + (w * h * 2 + 1);
  }

  // ESP32 only 16bpp check
  //if (esp_ptr_dma_capable(_img8_1)) Serial.println("DMA capable Sprite pointer _img8_1");
  //else Serial.println("Not a DMA capable Sprite pointer _img8_1");
  //if (esp_ptr_dma_capable(_img8_2)) Serial.println("DMA capable Sprite pointer _img8_2");
  //else Serial.println("Not a DMA capable Sprite pointer _img8_2");

  if ( (_bpp == 8) && (frames > 1) ) {
    _img8_2 = _img8 + (w * h + 1);
  }

  // This is to make it clear what pointer size is expected to be used
  // but casting in the user sketch is needed due to the use of void*
  if ( (_bpp == 1) && (frames > 1) )
  {
    w = (w+7) & 0xFFF8;
    _img8_2 = _img8 + ( (w>>3) * h + 1 );
  }

  if (_img8)
  {
    _created = true;
    if ( (_bpp == 4) && (_colorMap == nullptr)) createPalette(default_4bit_palette);

    rotation = 0;
    setViewport(0, 0, _dwidth, _dheight);
    setPivot(_iwidth/2, _iheight/2);
    return _img8_1;
  }

  return nullptr;
}

/***************************************************************************************
** Function name:           getPointer
** Description:             Returns pointer to start of sprite memory area
***************************************************************************************/
void* TFT_eSprite::getPointer(void)
{
  if (!_created) return nullptr;
  return _img8_1;
}


/***************************************************************************************
** Function name:           created
** Description:             Returns true if sprite has been created
***************************************************************************************/
bool TFT_eSprite::created(void)
{
  return _created;
}


/***************************************************************************************
** Function name:           ~TFT_eSprite
** Description:             Class destructor
***************************************************************************************/
TFT_eSprite::~TFT_eSprite(void)
{
  deleteSprite();
  if(fontLoaded) unloadFont();
}


/***************************************************************************************
** Function name:           callocSprite
** Description:             Allocate a memory area for the Sprite and return pointer
***************************************************************************************/
void* TFT_eSprite::callocSprite(int16_t w, int16_t h, uint8_t frames)
{
  // Add one extra "off screen" pixel to point out-of-bounds setWindow() coordinates
  // this means push/writeColor functions do not need additional bounds checks and
  // hence will run faster in normal circumstances.
  uint8_t* ptr8 = nullptr;

  if (frames > 2) frames = 2; // Currently restricted to 2 frame buffers
  if (frames < 1) frames = 1;

  if (_bpp == 16)
  {
    {
      ptr8 = ( uint8_t*) calloc(frames * w * h + frames, sizeof(uint16_t));
      //Serial.println("Normal RAM");
    }
  }

  else if (_bpp == 8)
  {
    ptr8 = ( uint8_t*) calloc(frames * w * h + frames, sizeof(uint8_t));
  }

  else if (_bpp == 4)
  {
    w = (w+1) & 0xFFFE; // width needs to be multiple of 2, with an extra "off screen" pixel
    _iwidth = w;
    ptr8 = ( uint8_t*) calloc(((frames * w * h) >> 1) + frames, sizeof(uint8_t));
  }

  else // Must be 1 bpp
  {
    //_dwidth   Display width+height in pixels always in rotation 0 orientation
    //_dheight  Not swapped for sprite rotations
    // Note: for 1bpp _iwidth and _iheight are swapped during Sprite rotations

    w =  (w+7) & 0xFFF8; // width should be the multiple of 8 bits to be compatible with epdpaint
    _iwidth = w;         // _iwidth is rounded up to be multiple of 8, so might not be = _dwidth
    _bitwidth = w;       // _bitwidth will not be rotated whereas _iwidth may be

    ptr8 = ( uint8_t*) calloc(frames * (w>>3) * h + frames, sizeof(uint8_t));
  }

  return ptr8;
}


/***************************************************************************************
** Function name:           createPalette (from RAM array)
** Description:             Set a palette for a 4-bit per pixel sprite
***************************************************************************************/
void TFT_eSprite::createPalette(uint16_t colorMap[], uint8_t colors)
{
  if (!_created) return;

  if (colorMap == nullptr)
  {
    // Create a color map using the default FLASH map
    createPalette(default_4bit_palette);
    return;
  }

  // Allocate and clear memory for 16 color map
  if (_colorMap == nullptr) _colorMap = (uint16_t *)calloc(16, sizeof(uint16_t));

  if (colors > 16) colors = 16;

  // Copy map colors
  for (uint8_t i = 0; i < colors; i++)
  {
    _colorMap[i] = colorMap[i];
  }
}


/***************************************************************************************
** Function name:           createPalette (from FLASH array)
** Description:             Set a palette for a 4-bit per pixel sprite
***************************************************************************************/
void TFT_eSprite::createPalette(const uint16_t colorMap[], uint8_t colors)
{
  if (!_created) return;

  if (colorMap == nullptr)
  {
    // Create a color map using the default FLASH map
    colorMap = default_4bit_palette;
  }

  // Allocate and clear memory for 16 color map
  if (_colorMap == nullptr) _colorMap = (uint16_t *)calloc(16, sizeof(uint16_t));

  if (colors > 16) colors = 16;

  // Copy map colors
  for (uint8_t i = 0; i < colors; i++)
  {
    _colorMap[i] = pgm_read_word(colorMap++);
  }
}


/***************************************************************************************
** Function name:           frameBuffer
** Description:             For 1 bpp Sprites, select the frame used for graphics
***************************************************************************************/
// Frames are numbered 1 and 2
void* TFT_eSprite::frameBuffer(int8_t f)
{
  if (!_created) return nullptr;

  if ( f == 2 ) _img8 = _img8_2;
  else          _img8 = _img8_1;

  if (_bpp == 16) _img = (uint16_t*)_img8;

  //if (_bpp == 8) _img8 = _img8;

  if (_bpp == 4) _img4 = _img8;

  return _img8;
}


/***************************************************************************************
** Function name:           setColorDepth
** Description:             Set bits per pixel for colour (1, 8 or 16)
***************************************************************************************/
void* TFT_eSprite::setColorDepth(int8_t b)
{
  // Do not re-create the sprite if the colour depth does not change
  if (_bpp == b) return _img8_1;

  // Validate the new colour depth
  if ( b > 8 ) _bpp = 16;  // Bytes per pixel
  else if ( b > 4 ) _bpp = 8;
  else if ( b > 1 ) _bpp = 4;
  else _bpp = 1;

  // Can't change an existing sprite's colour depth so delete and create a new one
  if (_created) {
    deleteSprite();
    return createSprite(_dwidth, _dheight);
  }

  return nullptr;
}


/***************************************************************************************
** Function name:           getColorDepth
** Description:             Get bits per pixel for colour (1, 8 or 16)
***************************************************************************************/
int8_t TFT_eSprite::getColorDepth(void)
{
  if (_created) return _bpp;
  else return 0;
}


/***************************************************************************************
** Function name:           setBitmapColor
** Description:             Set the 1bpp foreground foreground and background colour
***************************************************************************************/
void TFT_eSprite::setBitmapColor(uint16_t c, uint16_t b)
{
  if (c == b) b = ~c;
  _tft->bitmap_fg = c;
  _tft->bitmap_bg = b;
}


/***************************************************************************************
** Function name:           setPaletteColor
** Description:             Set the 4bpp palette color at the given index
***************************************************************************************/
void TFT_eSprite::setPaletteColor(uint8_t index, uint16_t color)
{
  if (_colorMap == nullptr || index > 15) return; // out of bounds

  _colorMap[index] = color;
}


/***************************************************************************************
** Function name:           getPaletteColor
** Description:             Return the palette color at 4bpp index, or 0 on error.
***************************************************************************************/
uint16_t TFT_eSprite::getPaletteColor(uint8_t index)
{
  if (_colorMap == nullptr || index > 15) return 0; // out of bounds

  return _colorMap[index];
}


/***************************************************************************************
** Function name:           deleteSprite
** Description:             Delete the sprite to free up memory (RAM)
***************************************************************************************/
void TFT_eSprite::deleteSprite(void)
{
  if (_colorMap != nullptr)
  {
    free(_colorMap);
    _colorMap = nullptr;
  }

  if (_created)
  {
    free(_img8_1);
    _img8 = nullptr;
    _created = false;
    _vpOoB   = true;  // TFT_eSPI class write() uses this to check for valid sprite
  }
}


/***************************************************************************************
** Function name:           pushRotated - Fast fixed point integer maths version
** Description:             Push rotated Sprite to TFT screen
***************************************************************************************/
#define FP_SCALE 10
bool TFT_eSprite::pushRotated(int16_t angle, uint32_t transp)
{
  if ( !_created || _tft->_vpOoB) return false;

  // Bounding box parameters
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Get the bounding box of this rotated source Sprite relative to Sprite pivot
  if ( !getRotatedBounds(angle, &min_x, &min_y, &max_x, &max_y) ) return false;

  uint16_t sline_buffer[max_x - min_x + 1];

  int32_t xt = min_x - _tft->_xPivot;
  int32_t yt = min_y - _tft->_yPivot;
  uint32_t xe = _dwidth << FP_SCALE;
  uint32_t ye = _dheight << FP_SCALE;
  uint16_t tpcolor = (uint16_t)transp;

  if (transp != 0x00FFFFFF) {
    if (_bpp == 4) tpcolor = _colorMap[transp & 0x0F];
    tpcolor = tpcolor>>8 | tpcolor<<8; // Working with swapped color bytes
  }
  _tft->startWrite(); // Avoid transaction overhead for every tft pixel

  // Scan destination bounding box and fetch transformed pixels from source Sprite
  for (int32_t y = min_y; y <= max_y; y++, yt++) {
    int32_t x = min_x;
    uint32_t xs = (_cosra * xt - (_sinra * yt - (_xPivot << FP_SCALE)) + (1 << (FP_SCALE - 1)));
    uint32_t ys = (_sinra * xt + (_cosra * yt + (_yPivot << FP_SCALE)) + (1 << (FP_SCALE - 1)));

    while ((xs >= xe || ys >= ye) && x < max_x) { x++; xs += _cosra; ys += _sinra; }
    if (x == max_x) continue;

    uint32_t pixel_count = 0;
    do {
      uint32_t rp;
      int32_t xp = xs >> FP_SCALE;
      int32_t yp = ys >> FP_SCALE;
      if (_bpp == 16) {rp = _img[xp + yp * _iwidth]; }
      else { rp = readPixel(xp, yp); rp = (uint16_t)(rp>>8 | rp<<8); }
      if (transp != 0x00FFFFFF && tpcolor == rp) {
        if (pixel_count) {
          // TFT window is already clipped, so this is faster than pushImage()
          _tft->setWindow(x - pixel_count, y, x - 1, y);
          _tft->pushPixels(sline_buffer, pixel_count);
          pixel_count = 0;
        }
      }
      else {
        sline_buffer[pixel_count++] = rp;
      }
    } while (++x < max_x && (xs += _cosra) < xe && (ys += _sinra) < ye);
    if (pixel_count) {
      // TFT window is already clipped, so this is faster than pushImage()
      _tft->setWindow(x - pixel_count, y, x - 1, y);
      _tft->pushPixels(sline_buffer, pixel_count);
    }
  }

  _tft->endWrite(); // End transaction

  return true;
}


/***************************************************************************************
** Function name:           pushRotated - Fast fixed point integer maths version
** Description:             Push a rotated copy of the Sprite to another Sprite
***************************************************************************************/
// Not compatible with 4bpp
bool TFT_eSprite::pushRotated(TFT_eSprite *spr, int16_t angle, uint32_t transp)
{
  if ( !_created  || _bpp == 4) return false; // Check this Sprite is created
  if ( !spr->_created  || spr->_bpp == 4) return false;  // Ckeck destination Sprite is created

  // Bounding box parameters
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Get the bounding box of this rotated source Sprite
  if ( !getRotatedBounds(spr, angle, &min_x, &min_y, &max_x, &max_y) ) return false;

  uint16_t sline_buffer[max_x - min_x + 1];

  int32_t xt = min_x - spr->_xPivot;
  int32_t yt = min_y - spr->_yPivot;
  uint32_t xe = _dwidth << FP_SCALE;
  uint32_t ye = _dheight << FP_SCALE;
  uint16_t tpcolor = (uint16_t)transp;
  
  if (transp != 0x00FFFFFF) {
    if (_bpp == 4) tpcolor = _colorMap[transp & 0x0F];
    tpcolor = tpcolor>>8 | tpcolor<<8; // Working with swapped color bytes
  }

  bool oldSwapBytes = spr->getSwapBytes();
  spr->setSwapBytes(false);

  // Scan destination bounding box and fetch transformed pixels from source Sprite
  for (int32_t y = min_y; y <= max_y; y++, yt++) {
    int32_t x = min_x;
    uint32_t xs = (_cosra * xt - (_sinra * yt - (_xPivot << FP_SCALE)) + (1 << (FP_SCALE - 1)));
    uint32_t ys = (_sinra * xt + (_cosra * yt + (_yPivot << FP_SCALE)) + (1 << (FP_SCALE - 1)));

    while ((xs >= xe || ys >= ye) && x < max_x) { x++; xs += _cosra; ys += _sinra; }
    if (x == max_x) continue;

    uint32_t pixel_count = 0;
    do {
      uint32_t rp;
      int32_t xp = xs >> FP_SCALE;
      int32_t yp = ys >> FP_SCALE;
      if (_bpp == 16) rp = _img[xp + yp * _iwidth];
      else { rp = readPixel(xp, yp); rp = (uint16_t)(rp>>8 | rp<<8); }
      if (transp != 0x00FFFFFF && tpcolor == rp) {
        if (pixel_count) {
          spr->pushImage(x - pixel_count, y, pixel_count, 1, sline_buffer);
          pixel_count = 0;
        }
      }
      else {
        sline_buffer[pixel_count++] = rp;
      }
    } while (++x < max_x && (xs += _cosra) < xe && (ys += _sinra) < ye);
    if (pixel_count) spr->pushImage(x - pixel_count, y, pixel_count, 1, sline_buffer);
  }
  spr->setSwapBytes(oldSwapBytes);
  return true;
}


/***************************************************************************************
** Function name:           getRotatedBounds
** Description:             Get TFT bounding box of a rotated Sprite wrt pivot
***************************************************************************************/
bool TFT_eSprite::getRotatedBounds(int16_t angle, int16_t *min_x, int16_t *min_y,
                                                  int16_t *max_x, int16_t *max_y)
{
  // Get the bounding box of this rotated source Sprite relative to Sprite pivot
  getRotatedBounds(angle, width(), height(), _xPivot, _yPivot, min_x, min_y, max_x, max_y);

  // Move bounding box so source Sprite pivot coincides with TFT pivot
  *min_x += _tft->_xPivot;
  *max_x += _tft->_xPivot;
  *min_y += _tft->_yPivot;
  *max_y += _tft->_yPivot;

  // Return if bounding box is outside of TFT viewport
  if (*min_x > _tft->_vpW) return false;
  if (*min_y > _tft->_vpH) return false;
  if (*max_x < _tft->_vpX) return false;
  if (*max_y < _tft->_vpY) return false;

  // Clip bounding box to be within TFT viewport
  if (*min_x < _tft->_vpX) *min_x = _tft->_vpX;
  if (*min_y < _tft->_vpY) *min_y = _tft->_vpY;
  if (*max_x > _tft->_vpW) *max_x = _tft->_vpW;
  if (*max_y > _tft->_vpH) *max_y = _tft->_vpH;

  return true;
}


/***************************************************************************************
** Function name:           getRotatedBounds
** Description:             Get destination Sprite bounding box of a rotated Sprite wrt pivot
***************************************************************************************/
bool TFT_eSprite::getRotatedBounds(TFT_eSprite *spr, int16_t angle, int16_t *min_x, int16_t *min_y,
                                                                    int16_t *max_x, int16_t *max_y)
{
  // Get the bounding box of this rotated source Sprite relative to Sprite pivot
  getRotatedBounds(angle, width(), height(), _xPivot, _yPivot, min_x, min_y, max_x, max_y);

  // Move bounding box so source Sprite pivot coincides with destination Sprite pivot
  *min_x += spr->_xPivot;
  *max_x += spr->_xPivot;
  *min_y += spr->_yPivot;
  *max_y += spr->_yPivot;

  // Test only to show bounding box
  // spr->fillSprite(TFT_BLACK);
  // spr->drawRect(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, TFT_GREEN);

  // Return if bounding box is completely outside of destination Sprite
  if (*min_x > spr->width()) return true;
  if (*min_y > spr->height()) return true;
  if (*max_x < 0) return true;
  if (*max_y < 0) return true;

  // Clip bounding box to Sprite boundaries
  // Clipping to a viewport will be done by destination Sprite pushImage function
  if (*min_x < 0) min_x = 0;
  if (*min_y < 0) min_y = 0;
  if (*max_x > spr->width())  *max_x = spr->width();
  if (*max_y > spr->height()) *max_y = spr->height();

  return true;
}


/***************************************************************************************
** Function name:           rotatedBounds
** Description:             Get bounding box of a rotated Sprite wrt pivot
***************************************************************************************/
void TFT_eSprite::getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp, int16_t yp,
                                   int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y)
{
  // Trig values for the rotation
  float radAngle = -angle * 0.0174532925; // Convert degrees to radians
  float sina = sin(radAngle);
  float cosa = cos(radAngle);

  w -= xp; // w is now right edge coordinate relative to xp
  h -= yp; // h is now bottom edge coordinate relative to yp

  // Calculate new corner coordinates
  int16_t x0 = -xp * cosa - yp * sina;
  int16_t y0 =  xp * sina - yp * cosa;

  int16_t x1 =  w * cosa - yp * sina;
  int16_t y1 = -w * sina - yp * cosa;

  int16_t x2 =  h * sina + w * cosa;
  int16_t y2 =  h * cosa - w * sina;

  int16_t x3 =  h * sina - xp * cosa;
  int16_t y3 =  h * cosa + xp * sina;

  // Find bounding box extremes, enlarge box to accomodate rounding errors
  *min_x = x0-2;
  if (x1 < *min_x) *min_x = x1-2;
  if (x2 < *min_x) *min_x = x2-2;
  if (x3 < *min_x) *min_x = x3-2;

  *max_x = x0+2;
  if (x1 > *max_x) *max_x = x1+2;
  if (x2 > *max_x) *max_x = x2+2;
  if (x3 > *max_x) *max_x = x3+2;

  *min_y = y0-2;
  if (y1 < *min_y) *min_y = y1-2;
  if (y2 < *min_y) *min_y = y2-2;
  if (y3 < *min_y) *min_y = y3-2;

  *max_y = y0+2;
  if (y1 > *max_y) *max_y = y1+2;
  if (y2 > *max_y) *max_y = y2+2;
  if (y3 > *max_y) *max_y = y3+2;

  _sinra = round(sina * (1<<FP_SCALE));
  _cosra = round(cosa * (1<<FP_SCALE));
}


/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y
***************************************************************************************/
void TFT_eSprite::pushSprite(int32_t x, int32_t y)
{
  if (!_created) return;

  if (_bpp == 16)
  {
    bool oldSwapBytes = _tft->getSwapBytes();
    _tft->setSwapBytes(false);
    _tft->pushImage(x, y, _dwidth, _dheight, _img );
    _tft->setSwapBytes(oldSwapBytes);
  }
  else if (_bpp == 4)
  {
    _tft->pushImage(x, y, _dwidth, _dheight, _img4, false, _colorMap);
  }
  else _tft->pushImage(x, y, _dwidth, _dheight, _img8, (bool)(_bpp == 8));
}


/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y with transparent colour
***************************************************************************************/
void TFT_eSprite::pushSprite(int32_t x, int32_t y, uint16_t transp)
{
  if (!_created) return;

  if (_bpp == 16)
  {
    bool oldSwapBytes = _tft->getSwapBytes();
    _tft->setSwapBytes(false);
    _tft->pushImage(x, y, _dwidth, _dheight, _img, transp );
    _tft->setSwapBytes(oldSwapBytes);
  }
  else if (_bpp == 8)
  {
    transp = (uint8_t)((transp & 0xE000)>>8 | (transp & 0x0700)>>6 | (transp & 0x0018)>>3);
    _tft->pushImage(x, y, _dwidth, _dheight, _img8, (uint8_t)transp, (bool)true);
  }
  else if (_bpp == 4)
  {
    _tft->pushImage(x, y, _dwidth, _dheight, _img4, (uint8_t)(transp & 0x0F), false, _colorMap);
  }
  else _tft->pushImage(x, y, _dwidth, _dheight, _img8, 0, (bool)false);
}


/***************************************************************************************
** Function name:           pushToSprite
** Description:             Push the sprite to another sprite at x, y
***************************************************************************************/
// Note: The following sprite to sprite colour depths are currently supported:
//    Source    Destination
//    16bpp  -> 16bpp
//    16bpp  ->  8bpp
//     8bpp  ->  8bpp
//     4bpp  ->  4bpp (note: color translation depends on the 2 sprites palette colors)
//     1bpp  ->  1bpp (note: color translation depends on the 2 sprites bitmap colors)

bool TFT_eSprite::pushToSprite(TFT_eSprite *dspr, int32_t x, int32_t y)
{
  if (!_created) return false;
  if (!dspr->created()) return false;

  // Check destination sprite compatibility
  int8_t ds_bpp = dspr->getColorDepth();
  if (_bpp == 16 && ds_bpp != 16 && ds_bpp !=  8) return false;
  if (_bpp ==  8 && ds_bpp !=  8) return false;
  if (_bpp ==  4 && ds_bpp !=  4) return false;
  if (_bpp ==  1 && ds_bpp !=  1) return false;

  bool oldSwapBytes = dspr->getSwapBytes();
  dspr->setSwapBytes(false);
  dspr->pushImage(x, y, _dwidth, _dheight, _img, _bpp);
  dspr->setSwapBytes(oldSwapBytes);

  return true;
}


/***************************************************************************************
** Function name:           pushToSprite
** Description:             Push the sprite to another sprite at x, y with transparent colour
***************************************************************************************/
// Note: The following sprite to sprite colour depths are currently supported:
//    Source    Destination
//    16bpp  -> 16bpp
//    16bpp  ->  8bpp
//     8bpp  ->  8bpp
//     1bpp  ->  1bpp

bool TFT_eSprite::pushToSprite(TFT_eSprite *dspr, int32_t x, int32_t y, uint16_t transp)
{
  if ( !_created  || !dspr->_created) return false; // Check Sprites exist

  // Check destination sprite compatibility
  int8_t ds_bpp = dspr->getColorDepth();
  if (_bpp == 16 && ds_bpp != 16 && ds_bpp !=  8) return false;
  if (_bpp ==  8 && ds_bpp !=  8) return false;
  if (_bpp ==  4 || ds_bpp ==  4) return false;
  if (_bpp ==  1 && ds_bpp !=  1) return false;

  bool oldSwapBytes = dspr->getSwapBytes();
  uint16_t sline_buffer[width()];

  transp = transp>>8 | transp<<8;

  // Scan destination bounding box and fetch transformed pixels from source Sprite
  for (int32_t ys = 0; ys < height(); ys++) {
    int32_t ox = x;
    uint32_t pixel_count = 0;

    for (int32_t xs = 0; xs < width(); xs++) {
      uint16_t rp = 0;
      if (_bpp == 16) rp = _img[xs + ys * width()];
      else { rp = readPixel(xs, ys); rp = rp>>8 | rp<<8; }
      //dspr->drawPixel(xs, ys, rp);

      if (transp == rp) {
        if (pixel_count) {
          dspr->pushImage(ox, y, pixel_count, 1, sline_buffer);
          ox += pixel_count;
          pixel_count = 0;
        }
        ox++;
      }
      else {
        sline_buffer[pixel_count++] = rp;
      }
    }
    if (pixel_count) dspr->pushImage(ox, y, pixel_count, 1, sline_buffer);
    y++;
  }
  dspr->setSwapBytes(oldSwapBytes);
  return true;
}


/***************************************************************************************
** Function name:           pushSprite
** Description:             Push a cropped sprite to the TFT at tx, ty
***************************************************************************************/
bool TFT_eSprite::pushSprite(int32_t tx, int32_t ty, int32_t sx, int32_t sy, int32_t sw, int32_t sh)
{
  if (!_created) return false;

  // Perform window boundary checks and crop if needed
  setWindow(sx, sy, sx + sw - 1, sy + sh - 1);

  /* These global variables are now populated for the sprite
  _xs = x start coordinate
  _ys = y start coordinate
  _xe = x end coordinate (inclusive)
  _ye = y end coordinate (inclusive)
  */

  // Calculate new sprite window bounding box width and height
  sw = _xe - _xs + 1;
  sh = _ye - _ys + 1;

  if (_ys >= _iheight) return false;

  if (_bpp == 16)
  {
    bool oldSwapBytes = _tft->getSwapBytes();
    _tft->setSwapBytes(false);

    // Check if a faster block copy to screen is possible
    if ( sx == 0 && sw == _dwidth)
      _tft->pushImage(tx, ty, sw, sh, _img + _iwidth * _ys );
    else // Render line by line
      while (sh--)
        _tft->pushImage(tx, ty++, sw, 1, _img + _xs + _iwidth * _ys++ );

    _tft->setSwapBytes(oldSwapBytes);
  }
  else if (_bpp == 8)
  {
    // Check if a faster block copy to screen is possible
    if ( sx == 0 && sw == _dwidth)
      _tft->pushImage(tx, ty, sw, sh, _img8 + _iwidth * _ys, (bool)true );
    else // Render line by line
    while (sh--)
      _tft->pushImage(tx, ty++, sw, 1, _img8 + _xs + _iwidth * _ys++, (bool)true );
  }
  else if (_bpp == 4)
  {
    // Check if a faster block copy to screen is possible
    if ( sx == 0 && sw == _dwidth)
      _tft->pushImage(tx, ty, sw, sh, _img4 + (_iwidth>>1) * _ys, false, _colorMap );
    else // Render line by line
    {
      int32_t ds = _xs&1; // Odd x start pixel

      int32_t de = 0;     // Odd x end pixel
      if ((sw > ds) && (_xe&1)) de = 1;

      uint32_t dm = 0;     // Midsection pixel count
      if (sw > (ds+de)) dm = sw - ds - de;
      sw--;

      uint32_t yp = (_xs + ds + _iwidth * _ys)>>1;
      _tft->startWrite();
      while (sh--)
      {
        if (ds) _tft->drawPixel(tx, ty, readPixel(_xs, _ys) );
        if (dm) _tft->pushImage(tx + ds, ty, dm, 1, _img4 + yp, false, _colorMap );
        if (de) _tft->drawPixel(tx + sw, ty, readPixel(_xe, _ys) );
        _ys++;
        ty++;
        yp += (_iwidth>>1);
      }
      _tft->endWrite();
    }
  }
  else // 1bpp
  {
    // Check if a faster block copy to screen is possible
    if ( sx == 0 && sw == _dwidth)
      _tft->pushImage(tx, ty, sw, sh, _img8 + (_bitwidth>>3) * _ys, (bool)false );
    else // Render line by line
    {
      _tft->startWrite();
      while (sh--)
      {
        _tft->pushImage(tx, ty++, sw, 1, _img8 + (_bitwidth>>3) * _ys++, (bool)false );
      }
      _tft->endWrite();
    }
  }

  return true;
}


/***************************************************************************************
** Function name:           readPixelValue
** Description:             Read the color map index of a pixel at defined coordinates
***************************************************************************************/
uint16_t TFT_eSprite::readPixelValue(int32_t x, int32_t y)
{
  if (_vpOoB  || !_created) return 0xFF;

  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return 0xFF;

  if (_bpp == 16)
  {
    // Return the pixel colour
    return readPixel(x - _xDatum, y - _yDatum);
  }

  if (_bpp == 8)
  {
    // Return the pixel byte value
    return _img8[x + y * _iwidth];
  }

  if (_bpp == 4)
  {
    if (x >= _dwidth) return 0xFF;
    if ((x & 0x01) == 0)
      return _img4[((x+y*_iwidth)>>1)] >> 4;   // even index = bits 7 .. 4
    else
      return _img4[((x+y*_iwidth)>>1)] & 0x0F; // odd index = bits 3 .. 0.
  }

  if (_bpp == 1)
  {
    // Note: _dwidth and _dheight bounds not checked (rounded up -iwidth and _iheight used)
    if (rotation == 1)
    {
      uint16_t tx = x;
      x = _dheight - y - 1;
      y = tx;
    }
    else if (rotation == 2)
    {
      x = _dwidth - x - 1;
      y = _dheight - y - 1;
    }
    else if (rotation == 3)
    {
      uint16_t tx = x;
      x = y;
      y = _dwidth - tx - 1;
    }
    // Return 1 or 0
    return (_img8[(x + y * _bitwidth)>>3] >> (7-(x & 0x7))) & 0x01;
  }

  return 0;
}

/***************************************************************************************
** Function name:           readPixel
** Description:             Read 565 colour of a pixel at defined coordinates
***************************************************************************************/
uint16_t TFT_eSprite::readPixel(int32_t x, int32_t y)
{
  if (_vpOoB  || !_created) return 0xFFFF;

  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return 0xFFFF;

  if (_bpp == 16)
  {
    uint16_t color = _img[x + y * _iwidth];
    return (color >> 8) | (color << 8);
  }

  if (_bpp == 8)
  {
    uint16_t color = _img8[x + y * _iwidth];
    if (color != 0)
    {
    uint8_t  blue[] = {0, 11, 21, 31};
      color =   (color & 0xE0)<<8 | (color & 0xC0)<<5
              | (color & 0x1C)<<6 | (color & 0x1C)<<3
              | blue[color & 0x03];
    }
    return color;
  }

  if (_bpp == 4)
  {
    if (x >= _dwidth) return 0xFFFF;
    uint16_t color;
    if ((x & 0x01) == 0)
      color = _colorMap[_img4[((x+y*_iwidth)>>1)] >> 4];   // even index = bits 7 .. 4
    else
      color = _colorMap[_img4[((x+y*_iwidth)>>1)] & 0x0F]; // odd index = bits 3 .. 0.
    return color;
  }

  // Note: Must be 1bpp
  // _dwidth and _dheight bounds not checked (rounded up -iwidth and _iheight used)
  if (rotation == 1)
  {
    uint16_t tx = x;
    x = _dheight - y - 1;
    y = tx;
  }
  else if (rotation == 2)
  {
    x = _dwidth - x - 1;
    y = _dheight - y - 1;
  }
  else if (rotation == 3)
  {
    uint16_t tx = x;
    x = y;
    y = _dwidth - tx - 1;
  }

  uint16_t color = (_img8[(x + y * _bitwidth)>>3] << (x & 0x7)) & 0x80;

  if (color) return _tft->bitmap_fg;
  else       return _tft->bitmap_bg;
}


/***************************************************************************************
** Function name:           pushImage
** Description:             push image into a defined area of a sprite
***************************************************************************************/
void  TFT_eSprite::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint8_t sbpp)
{
  if (data == nullptr || !_created) return;

  PI_CLIP;

  if (_bpp == 16) // Plot a 16 bpp image into a 16 bpp Sprite
  {
    // Pointer within original image
    uint8_t *ptro = (uint8_t *)data + ((dx + dy * w) << 1);
    // Pointer within sprite image
    uint8_t *ptrs = (uint8_t *)_img + ((x + y * _iwidth) << 1);

    if(_swapBytes)
    {
      while (dh--)
      {
        // Fast copy with a 1 byte shift
        memcpy(ptrs+1, ptro, (dw<<1) - 1);
        // Now correct just the even numbered bytes
        for (int32_t xp = 0; xp < (dw<<1); xp+=2)
        {
          ptrs[xp] = ptro[xp+1];;
        }
        ptro += w<<1;
        ptrs += _iwidth<<1;
      }
    }
    else
    {
      while (dh--)
      {
        memcpy(ptrs, ptro, dw<<1);
        ptro += w << 1;
        ptrs += _iwidth << 1;
      }
    }
  }
  else if (_bpp == 8 && sbpp == 8) // Plot a 8 bpp image into a 8 bpp Sprite
  {
    // Pointer within original image
    uint8_t *ptro = (uint8_t *)data + (dx + dy * w);
    // Pointer within sprite image
    uint8_t *ptrs = (uint8_t *)_img + (x + y * _iwidth);

    while (dh--)
    {
      memcpy(ptrs, ptro, dw);
      ptro += w;
      ptrs += _iwidth;
    }
  }
  else if (_bpp == 8) // Plot a 16 bpp image into a 8 bpp Sprite
  {
    uint16_t lastColor = 0;
    uint8_t  color8    = 0;
    for (int32_t yp = dy; yp < dy + dh; yp++)
    {
      int32_t xyw = x + y * _iwidth;
      int32_t dxypw = dx + yp * w;
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        uint16_t color = data[dxypw++];
        if (color != lastColor) {
          // When data source is a sprite, the bytes are already swapped
          if(!_swapBytes) color8 = (uint8_t)((color & 0xE0) | (color & 0x07)<<2 | (color & 0x1800)>>11);
          else color8 = (uint8_t)((color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3);
        }
        lastColor = color;
        _img8[xyw++] = color8;
      }
      y++;
    }
  }
  else if (_bpp == 4)
  {
    // The image is assumed to be 4-bit, where each byte corresponds to two pixels.
    // much faster when aligned to a byte boundary, because the alternative is slower, requiring
    // tedious bit operations.

    int sWidth = (_iwidth >> 1);
    uint8_t *ptr = (uint8_t *)data;

    if ((x & 0x01) == 0 && (dx & 0x01) == 0 && (dw & 0x01) == 0)
    {
      x = (x >> 1) + y * sWidth;
      dw = (dw >> 1);
      dx = (dx >> 1) + dy * (w>>1);
      while (dh--)
      {
        memcpy(_img4 + x, ptr + dx, dw);
        dx += (w >> 1);
        x += sWidth;
      }
    }
    else  // not optimized
    {
      for (int32_t yp = dy; yp < dy + dh; yp++)
      {
        int32_t ox = x;
        for (int32_t xp = dx; xp < dx + dw; xp++)
        {
          uint32_t color;
          if ((xp & 0x01) == 0)
            color = (ptr[((xp+yp*w)>>1)] & 0xF0) >> 4; // even index = bits 7 .. 4
          else
            color = ptr[((xp-1+yp*w)>>1)] & 0x0F;      // odd index = bits 3 .. 0.
          drawPixel(ox, y, color);
          ox++;
        }
        y++;
      }
    }
  }

  else // 1bpp
  {
    // Plot a 1bpp image into a 1bpp Sprite
    uint32_t ww =  (w+7)>>3; // Width of source image line in bytes
    uint8_t *ptr = (uint8_t *)data;
    for (int32_t yp = dy;  yp < dy + dh; yp++)
    {
      uint32_t yw = yp * ww;              // Byte starting the line containing source pixel
      int32_t ox = x;
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        uint16_t readPixel = (ptr[(xp>>3) + yw] & (0x80 >> (xp & 0x7)) );
        drawPixel(ox++, y, readPixel);
      }
      y++;
    }
  }
}


/***************************************************************************************
** Function name:           pushImage
** Description:             push 565 colour FLASH (PROGMEM) image into a defined area
***************************************************************************************/
void  TFT_eSprite::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data)
{
#ifdef ESP32
  pushImage(x, y, w, h, (uint16_t*) data);
#else
  // Partitioned memory FLASH processor
  if (data == nullptr || !_created) return;

  PI_CLIP;

  if (_bpp == 16) // Plot a 16 bpp image into a 16 bpp Sprite
  {
    for (int32_t yp = dy; yp < dy + dh; yp++)
    {
      int32_t ox = x;
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        uint16_t color = pgm_read_word(data + xp + yp * w);
        if(_swapBytes) color = color<<8 | color>>8;
        _img[ox + y * _iwidth] = color;
        ox++;
      }
      y++;
    }
  }

  else if (_bpp == 8) // Plot a 16 bpp image into a 8 bpp Sprite
  {
    for (int32_t yp = dy; yp < dy + dh; yp++)
    {
      int32_t ox = x;
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        uint16_t color = pgm_read_word(data + xp + yp * w);
        if(_swapBytes) color = color<<8 | color>>8;
        _img8[ox + y * _iwidth] = (uint8_t)((color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3);
        ox++;
      }
      y++;
    }
  }

  else if (_bpp == 4)
  {
    #ifdef TFT_eSPI_DEBUG
    Serial.println("TFT_eSprite::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data) not implemented");
    #endif
    return;
  }

  else // Plot a 1bpp image into a 1bpp Sprite
  {
    x-= _xDatum;   // Remove offsets, drawPixel will add
    y-= _yDatum;
    uint16_t bsw =  (w+7) >> 3; // Width in bytes of source image line
    uint8_t *ptr = ((uint8_t*)data) + dy * bsw;
    
    while (dh--) {
      int32_t odx = dx;
      int32_t ox  = x;
      while (odx < dx + dw) {
        uint8_t pbyte = pgm_read_byte(ptr + (odx>>3));
        uint8_t mask = 0x80 >> (odx & 7);
        while (mask) {
          uint8_t p = pbyte & mask;
          mask = mask >> 1;
          drawPixel(ox++, y, p);
          odx++;
        }
      }
      ptr += bsw;
      y++;
    }
  }
#endif // if ESP32 check
}


/***************************************************************************************
** Function name:           setWindow
** Description:             Set the bounds of a window in the sprite
***************************************************************************************/
// Intentionally not constrained to viewport area, does not manage 1bpp rotations
void TFT_eSprite::setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  if (x0 > x1) transpose(x0, x1);
  if (y0 > y1) transpose(y0, y1);
  
  int32_t w = width();
  int32_t h = height();

  if ((x0 >= w) || (x1 < 0) || (y0 >= h) || (y1 < 0))
  { // Point to that extra "off screen" pixel
    _xs = 0;
    _ys = _dheight;
    _xe = 0;
    _ye = _dheight;
  }
  else
  {
    if (x0 < 0) x0 = 0;
    if (x1 >= w) x1 = w - 1;
    if (y0 < 0) y0 = 0;
    if (y1 >= h) y1 = h - 1;

    _xs = x0;
    _ys = y0;
    _xe = x1;
    _ye = y1;
  }

  _xptr = _xs;
  _yptr = _ys;
}


/***************************************************************************************
** Function name:           pushColor
** Description:             Send a new pixel to the set window
***************************************************************************************/
void TFT_eSprite::pushColor(uint16_t color)
{
  if (!_created ) return;

  // Write the colour to RAM in set window
  if (_bpp == 16)
    _img [_xptr + _yptr * _iwidth] = (uint16_t) (color >> 8) | (color << 8);

  else  if (_bpp == 8)
    _img8[_xptr + _yptr * _iwidth] = (uint8_t )((color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3);

  else if (_bpp == 4)
  {
    uint8_t c = (uint8_t)color & 0x0F;
    if ((_xptr & 0x01) == 0) {
      _img4[(_xptr + _yptr * _iwidth)>>1] = (c << 4) | (_img4[(_xptr + _yptr * _iwidth)>>1] & 0x0F);  // new color is in bits 7 .. 4
    }
    else {
      _img4[(_xptr + _yptr * _iwidth)>>1] = (_img4[(_xptr + _yptr * _iwidth)>>1] & 0xF0) | c; // new color is the low bits
    }
  }

  else drawPixel(_xptr, _yptr, color);

  // Increment x
  _xptr++;

  // Wrap on x and y to start, increment y if needed
  if (_xptr > _xe)
  {
    _xptr = _xs;
    _yptr++;
    if (_yptr > _ye) _yptr = _ys;
  }

}


/***************************************************************************************
** Function name:           pushColor
** Description:             Send a "len" new pixels to the set window
***************************************************************************************/
void TFT_eSprite::pushColor(uint16_t color, uint32_t len)
{
  if (!_created ) return;

  uint16_t pixelColor;

  if (_bpp == 16)
    pixelColor = (uint16_t) (color >> 8) | (color << 8);

  else  if (_bpp == 8)
    pixelColor = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;

  else pixelColor = (uint16_t) color; // for 1bpp or 4bpp

  while(len--) writeColor(pixelColor);
}


/***************************************************************************************
** Function name:           writeColor
** Description:             Write a pixel with pre-formatted colour to the set window
***************************************************************************************/
void TFT_eSprite::writeColor(uint16_t color)
{
  if (!_created ) return;

  // Write 16-bit RGB 565 encoded colour to RAM
  if (_bpp == 16) _img [_xptr + _yptr * _iwidth] = color;

  // Write 8-bit RGB 332 encoded colour to RAM
  else if (_bpp == 8) _img8[_xptr + _yptr * _iwidth] = (uint8_t) color;

  else if (_bpp == 4)
  {
    uint8_t c = (uint8_t)color & 0x0F;
    if ((_xptr & 0x01) == 0)
      _img4[(_xptr + _yptr * _iwidth)>>1] = (c << 4) | (_img4[(_xptr + _yptr * _iwidth)>>1] & 0x0F);  // new color is in bits 7 .. 4
    else
      _img4[(_xptr + _yptr * _iwidth)>>1] = (_img4[(_xptr + _yptr * _iwidth)>>1] & 0xF0) | c; // new color is the low bits (x is odd)
  }

  else drawPixel(_xptr, _yptr, color);

  // Increment x
  _xptr++;

  // Wrap on x and y to start, increment y if needed
  if (_xptr > _xe)
  {
    _xptr = _xs;
    _yptr++;
    if (_yptr > _ye) _yptr = _ys;
  }
}


/***************************************************************************************
** Function name:           setScrollRect
** Description:             Set scroll area within the sprite and the gap fill colour
***************************************************************************************/
// Intentionally not constrained to viewport area
void TFT_eSprite::setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
{
  if ((x >= _iwidth) || (y >= _iheight) || !_created ) return;

  if (x < 0) { w += x; x = 0; }
  if (y < 0) { h += y; y = 0; }

  if ((x + w) > _iwidth ) w = _iwidth  - x;
  if ((y + h) > _iheight) h = _iheight - y;

  if ( w < 1 || h < 1) return;

  _sx = x;
  _sy = y;
  _sw = w;
  _sh = h;

  _scolor = color;
}


/***************************************************************************************
** Function name:           scroll
** Description:             Scroll dx,dy pixels, positive right,down, negative left,up
***************************************************************************************/
void TFT_eSprite::scroll(int16_t dx, int16_t dy)
{
  if (abs(dx) >= _sw || abs(dy) >= _sh)
  {
    fillRect (_sx, _sy, _sw, _sh, _scolor);
    return;
  }

  // Fetch the scroll area width and height set by setScrollRect()
  uint32_t w  = _sw - abs(dx); // line width to copy
  uint32_t h  = _sh - abs(dy); // lines to copy
  int32_t iw  = _iwidth;       // rounded up width of sprite

  // Fetch the x,y origin set by setScrollRect()
  uint32_t tx = _sx; // to x
  uint32_t fx = _sx; // from x
  uint32_t ty = _sy; // to y
  uint32_t fy = _sy; // from y

  // Adjust for x delta
  if (dx <= 0) fx -= dx;
  else tx += dx;

  // Adjust for y delta
  if (dy <= 0) fy -= dy;
  else
  { // Scrolling down so start copy from bottom
    ty = ty + _sh - 1; // "To" pointer
    iw = -iw;          // Pointer moves backwards
    fy = ty - dy;      // "From" pointer
  }

  // Calculate "from y" and "to y" pointers in RAM
  uint32_t fyp = fx + fy * _iwidth;
  uint32_t typ = tx + ty * _iwidth;

  // Now move the pixels in RAM
  if (_bpp == 16)
  {
    while (h--)
    { // move pixel lines (to, from, byte count)
      memmove( _img + typ, _img + fyp, w<<1);
      typ += iw;
      fyp += iw;
    }
  }
  else if (_bpp == 8)
  {
    while (h--)
    { // move pixel lines (to, from, byte count)
      memmove( _img8 + typ, _img8 + fyp, w);
      typ += iw;
      fyp += iw;
    }
  }
  else if (_bpp == 4)
  {
    // could optimize for scrolling by even # pixels using memove (later)
    if (dx >  0) { tx += w; fx += w; } // Start from right edge
    while (h--)
    { // move pixels one by one
      for (uint16_t xp = 0; xp < w; xp++)
      {
        if (dx <= 0) drawPixel(tx + xp, ty, readPixelValue(fx + xp, fy));
        if (dx >  0) drawPixel(tx - xp, ty, readPixelValue(fx - xp, fy));
      }
      if (dy <= 0)  { ty++; fy++; }
      else  { ty--; fy--; }
    }
  }
  else if (_bpp == 1 )
  {
    if (dx >  0) { tx += w; fx += w; } // Start from right edge
    while (h--)
    { // move pixels one by one
      for (uint16_t xp = 0; xp < w; xp++)
      {
        if (dx <= 0) drawPixel(tx + xp, ty, readPixelValue(fx + xp, fy));
        if (dx >  0) drawPixel(tx - xp, ty, readPixelValue(fx - xp, fy));
      }
      if (dy <= 0)  { ty++; fy++; }
      else  { ty--; fy--; }
    }
  }
  else return; // Not 1, 4, 8 or 16 bpp

  // Fill the gap left by the scrolling
  if (dx > 0) fillRect(_sx, _sy, dx, _sh, _scolor);
  if (dx < 0) fillRect(_sx + _sw + dx, _sy, -dx, _sh, _scolor);
  if (dy > 0) fillRect(_sx, _sy, _sw, dy, _scolor);
  if (dy < 0) fillRect(_sx, _sy + _sh + dy, _sw, -dy, _scolor);
}


/***************************************************************************************
** Function name:           fillSprite
** Description:             Fill the whole sprite with defined colour
***************************************************************************************/
void TFT_eSprite::fillSprite(uint32_t color)
{
  if (!_created || _vpOoB) return;

  // Use memset if possible as it is super fast
  if(_xDatum == 0 && _yDatum == 0  &&  _xWidth == width())
  {
    if(_bpp == 16) {
      if ( (uint8_t)color == (uint8_t)(color>>8) ) {
        memset(_img,  (uint8_t)color, _iwidth * _yHeight * 2);
      }
      else fillRect(_vpX, _vpY, _xWidth, _yHeight, color);
    }
    else if (_bpp == 8)
    {
      color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
      memset(_img8, (uint8_t)color, _iwidth * _yHeight);
    }
    else if (_bpp == 4)
    {
      uint8_t c = ((color & 0x0F) | (((color & 0x0F) << 4) & 0xF0));
      memset(_img4, c, (_iwidth * _yHeight) >> 1);
    }
    else if (_bpp == 1)
    {
      if(color) memset(_img8, 0xFF, (_bitwidth>>3) * _dheight + 1);
      else      memset(_img8, 0x00, (_bitwidth>>3) * _dheight + 1);
    }
  }
  else fillRect(_vpX - _xDatum, _vpY - _yDatum, _xWidth, _yHeight, color);
}


/***************************************************************************************
** Function name:           width
** Description:             Return the width of sprite
***************************************************************************************/
// Return the size of the sprite
int16_t TFT_eSprite::width(void)
{
  if (!_created ) return 0;

  if (_bpp > 1) {
    if (_vpDatum) return _xWidth;
    return _dwidth;
  }

  if (rotation & 1) {
    if (_vpDatum) return _xWidth;
    return _dheight;
  }

  if (_vpDatum) return _xWidth;
  return _dwidth;
}


/***************************************************************************************
** Function name:           height
** Description:             Return the height of sprite
***************************************************************************************/
int16_t TFT_eSprite::height(void)
{
  if (!_created ) return 0;

  if (_bpp > 1) {
    if (_vpDatum) return _yHeight;
    return _dheight;
  }

  if (rotation & 1) {
    if (_vpDatum) return _yHeight;
    return _dwidth;
  }

  if (_vpDatum) return _yHeight;
  return _dheight;
}


/***************************************************************************************
** Function name:           setRotation
** Description:             Rotate coordinate frame for 1bpp sprite
***************************************************************************************/
// Does nothing for 4, 8 and 16 bpp sprites.
void TFT_eSprite::setRotation(uint8_t r)
{
  if (_bpp != 1) return;

  rotation = r;
  
  if (rotation&1) {
    resetViewport();
  }
  else {
    resetViewport();
  }
}


/***************************************************************************************
** Function name:           getRotation
** Description:             Get rotation for 1bpp sprite
***************************************************************************************/
uint8_t TFT_eSprite::getRotation(void)
{
  return rotation;
}


/***************************************************************************************
** Function name:           drawPixel
** Description:             push a single pixel at an arbitrary position
***************************************************************************************/
void TFT_eSprite::drawPixel(int32_t x, int32_t y, uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    _img[x+y*_iwidth] = (uint16_t) color;
  }
  else if (_bpp == 8)
  {
    _img8[x+y*_iwidth] = (uint8_t)((color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3);
  }
  else if (_bpp == 4)
  {
    uint8_t c = color & 0x0F;
    int index = (x+y*_iwidth)>>1;;
    if ((x & 0x01) == 0) {
      _img4[index] = (uint8_t)((c << 4) | (_img4[index] & 0x0F));
    }
    else {
      _img4[index] =  (uint8_t)(c | (_img4[index] & 0xF0));
    }
  }
  else // 1 bpp
  {
    if (rotation == 1)
    {
      uint16_t tx = x;
      x = _dwidth - y - 1;
      y = tx;
    }
    else if (rotation == 2)
    {
      x = _dwidth - x - 1;
      y = _dheight - y - 1;
    }
    else if (rotation == 3)
    {
      uint16_t tx = x;
      x = y;
      y = _dheight - tx - 1;
    }

    if (color) _img8[(x + y * _bitwidth)>>3] |=  (0x80 >> (x & 0x7));
    else       _img8[(x + y * _bitwidth)>>3] &= ~(0x80 >> (x & 0x7));
  }
}


/***************************************************************************************
** Function name:           drawLine
** Description:             draw a line between 2 arbitrary points
***************************************************************************************/
void TFT_eSprite::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
  if (!_created || _vpOoB) return;

  //_xDatum and _yDatum Not added here, it is added by drawPixel & drawFastxLine

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    transpose(x0, y0);
    transpose(x1, y1);
  }

  if (x0 > x1) {
    transpose(x0, x1);
    transpose(y0, y1);
  }

  int32_t dx = x1 - x0, dy = abs(y1 - y0);;

  int32_t err = dx >> 1, ystep = -1, xs = x0, dlen = 0;

  if (y0 < y1) ystep = 1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(y0, xs, color);
        else drawFastVLine(y0, xs, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastVLine(y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(xs, y0, color);
        else drawFastHLine(xs, y0, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastHLine(xs, y0, dlen, color);
  }
}


/***************************************************************************************
** Function name:           drawFastVLine
** Description:             draw a vertical line
***************************************************************************************/
void TFT_eSprite::drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x < _vpX) || (x >= _vpW) || (y >= _vpH)) return;

  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((y + h) > _vpH) h = _vpH - y;

  if (h < 1) return;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    int32_t yp = x + _iwidth * y;
    while (h--) {_img[yp] = (uint16_t) color; yp += _iwidth;}
  }
  else if (_bpp == 8)
  {
    color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
    while (h--) _img8[x + _iwidth * y++] = (uint8_t) color;
  }
  else if (_bpp == 4)
  {
    if ((x & 0x01) == 0)
    {
      uint8_t c = (uint8_t) (color & 0xF) << 4;
      while (h--) {
        _img4[(x + _iwidth * y)>>1] = (uint8_t) (c | (_img4[(x + _iwidth * y)>>1] & 0x0F));
        y++;
      }
    }
    else {
      uint8_t c = (uint8_t)color & 0xF;
      while (h--) {
        _img4[(x + _iwidth * y)>>1] = (uint8_t) (c | (_img4[(x + _iwidth * y)>>1] & 0xF0)); // x is odd; new color goes into the low bits.
        y++;
      }
    }
  }
  else
  {
    x -= _xDatum; // Remove any offset as it will be added by drawPixel
    y -= _yDatum;
    while (h--)
    {
      drawPixel(x, y++, color);
    }
  }
}


/***************************************************************************************
** Function name:           drawFastHLine
** Description:             draw a horizontal line
***************************************************************************************/
void TFT_eSprite::drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((y < _vpY) || (x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }

  if ((x + w) > _vpW) w = _vpW - x;

  if (w < 1) return;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    while (w--) _img[_iwidth * y + x++] = (uint16_t) color;
  }
  else if (_bpp == 8)
  {
    color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
    memset(_img8+_iwidth * y + x, (uint8_t)color, w);
  }
  else if (_bpp == 4)
  {
    uint8_t c = (uint8_t)color & 0x0F;
    uint8_t c2 = (c | ((c << 4) & 0xF0));
    if ((x & 0x01) == 1)
    {
      drawPixel(x - _xDatum, y - _yDatum, color);
      x++; w--;
      if (w < 1)
        return;
    }

    if (((w + x) & 0x01) == 1)
    {
      // handle the extra one at the other end
      drawPixel(x - _xDatum + w - 1, y - _yDatum, color);
      w--;
      if (w < 1) return;
    }
    memset(_img4 + ((_iwidth * y + x) >> 1), c2, (w >> 1));
  }
  else {
    x -= _xDatum; // Remove any offset as it will be added by drawPixel
    y -= _yDatum;

    while (w--)
    {
      drawPixel(x++, y, color);
    }
  }
}


/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
***************************************************************************************/
void TFT_eSprite::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }
  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((x + w) > _vpW) w = _vpW - x;
  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  int32_t yp = _iwidth * y + x;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    uint32_t iw = w;
    int32_t ys = yp;
    if(h--)  {while (iw--) _img[yp++] = (uint16_t) color;}
    yp = ys;
    while (h--)
    {
      yp += _iwidth;
      memcpy( _img+yp, _img+ys, w<<1);
    }
  }
  else if (_bpp == 8)
  {
    color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
    while (h--)
    {
      memset(_img8 + yp, (uint8_t)color, w);
      yp += _iwidth;
    }
  }
  else if (_bpp == 4)
  {
    uint8_t c1 = (uint8_t)color & 0x0F;
    uint8_t c2 = c1 | ((c1 << 4) & 0xF0);
    if ((x & 0x01) == 0 && (w & 0x01) == 0)
    {
      yp = (yp >> 1);
      while (h--)
      {
        memset(_img4 + yp, c2, (w>>1));
        yp += (_iwidth >> 1);
      }
    }
    else if ((x & 0x01) == 0)
    {

      // same as above but you have a hangover on the right.
      yp = (yp >> 1);
      while (h--)
      {
        if (w > 1)
          memset(_img4 + yp, c2, (w-1)>>1);
        // handle the rightmost pixel by calling drawPixel
        drawPixel(x+w-1-_xDatum, y+h-_yDatum, c1);
        yp += (_iwidth >> 1);
      }
    }
    else if ((w & 0x01) == 1)
    {
      yp = (yp + 1) >> 1;
      while (h--) {
        drawPixel(x-_xDatum, y+h-_yDatum, color & 0x0F);
        if (w > 1)
          memset(_img4 + yp, c2, (w-1)>>1);
        // same as above but you have a hangover on the left instead
        yp += (_iwidth >> 1);
      }
    }
    else
    {
      yp = (yp + 1) >> 1;
      while (h--) {
        drawPixel(x-_xDatum, y+h-_yDatum, color & 0x0F);
        if (w > 1) drawPixel(x+w-1-_xDatum, y+h-_yDatum, color & 0x0F);
        if (w > 2)
          memset(_img4 + yp, c2, (w-2)>>1);
        // maximal hacking, single pixels on left and right.
        yp += (_iwidth >> 1);
      }
    }
  }
  else
  {
    x -= _xDatum;
    y -= _yDatum;
    while (h--)
    {
      int32_t ww = w;
      int32_t xx = x;
      while (ww--) drawPixel(xx++, y, color);
      y++;
    }
  }
}


/***************************************************************************************
** Function name:           drawChar
** Description:             draw a single character in the Adafruit GLCD or freefont
***************************************************************************************/
void TFT_eSprite::drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size)
{
  if ( _vpOoB || !_created ) return;

  if (c < 32) return;
  if ((x >= _vpW - _xDatum) || // Clip right
      (y >= _vpH - _yDatum))   // Clip bottom
    return;

  if (((x + 6 * size - 1) < (_vpX - _xDatum)) || // Clip left
      ((y + 8 * size - 1) < (_vpY - _yDatum)))   // Clip top
    return;

  if (c > 255) return;
  if (!_cp437 && c > 175) c++;

  bool fillbg = (bg != color);

  if ((size==1) && fillbg)
  {
    uint8_t column[6];
    uint8_t mask = 0x1;

    for (int8_t i = 0; i < 5; i++ ) column[i] = pgm_read_byte(font + (c * 5) + i);
    column[5] = 0;

    int8_t j, k;
    for (j = 0; j < 8; j++) {
      for (k = 0; k < 5; k++ ) {
        if (column[k] & mask) {
          drawPixel(x + k, y + j, color);
        }
        else {
          drawPixel(x + k, y + j, bg);
        }
      }

      mask <<= 1;

      drawPixel(x + k, y + j, bg);
    }
  }
  else
  {
    for (int8_t i = 0; i < 6; i++ ) {
      uint8_t line;
      if (i == 5)
        line = 0x0;
      else
        line = pgm_read_byte(font + (c * 5) + i);

      if (size == 1) // default size
      {
        for (int8_t j = 0; j < 8; j++) {
          if (line & 0x1) drawPixel(x + i, y + j, color);
          line >>= 1;
        }
      } else {  // big size
        for (int8_t j = 0; j < 8; j++) {
          if (line & 0x1) fillRect(x + (i * size), y + (j * size), size, size, color);
          else if (fillbg) fillRect(x + i * size, y + j * size, size, size, bg);
          line >>= 1;
        }
      }
    }
  }
}

int16_t TFT_eSprite::drawChar(uint16_t uniCode, int32_t x, int32_t y) {
  return drawChar(uniCode, x, y, textfont);
}

int16_t TFT_eSprite::drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font) {
  if (_vpOoB || !uniCode) return 0;

  if (font==1) return 0;

  if ((font>1) && (font<9) && ((uniCode < 32) || (uniCode > 127))) return 0;

  int32_t width  = 0;
  int32_t height = 0;
  uint32_t flash_address = 0;
  uniCode -= 32;

  int32_t xd = x + _xDatum;
  int32_t yd = y + _yDatum;

  if ((xd + width * textsize < _vpX || xd >= _vpW) && (yd + height * textsize < _vpY || yd >= _vpH)) return width * textsize ;

  int32_t w = width;
  int32_t pX      = 0;
  int32_t pY      = y;
  uint8_t line = 0;
  bool clip = xd < _vpX || xd + width  * textsize >= _vpW || yd < _vpY || yd + height * textsize >= _vpH;

  flash_address = flash_address;
  w = w;
  pX = pX;
  pY = pY;
  line = line;
  clip = clip;

  return width * textsize;    // x +
}

void TFT_eSprite::drawGlyph(uint16_t code)
{
  uint16_t fg = textcolor;
  uint16_t bg = textbgcolor;
  bool getBG  = false;
  if (fg == bg) getBG = true;

  // Check if cursor has moved
  if (last_cursor_x != cursor_x) {
    bg_cursor_x = cursor_x;
    last_cursor_x = cursor_x;
  }

  if (code < 0x21) {
    if (code == 0x20) {
      if (_fillbg) fillRect(bg_cursor_x, cursor_y, (cursor_x + gFont.spaceWidth) - bg_cursor_x, gFont.yAdvance, bg);
      cursor_x += gFont.spaceWidth;
      bg_cursor_x = cursor_x;
      last_cursor_x = cursor_x;
      return;
    }

    if (code == '\n') {
      cursor_x = 0;
      bg_cursor_x = 0;
      last_cursor_x = 0;
      cursor_y += gFont.yAdvance;
      if (textwrapY && (cursor_y >= height())) cursor_y = 0;
      return;
    }
  }

  uint16_t gNum = 0;
  bool found = getUnicodeIndex(code, &gNum);

  if (found) {
    bool newSprite = !_created;
    if (newSprite) {
      createSprite(gWidth[gNum], gFont.yAdvance);
      if(fg != bg) fillSprite(bg);
      cursor_x = -gdX[gNum];
      bg_cursor_x = cursor_x;
      last_cursor_x = cursor_x;
      cursor_y = 0;
    } else {
      if( textwrapX && ((cursor_x + gWidth[gNum] + gdX[gNum]) > width())) {
        cursor_y += gFont.yAdvance;
        cursor_x = 0;
        bg_cursor_x = 0;
        last_cursor_x = 0;
      }

      if( textwrapY && ((cursor_y + gFont.yAdvance) > height())) cursor_y = 0;
      if ( cursor_x == 0) cursor_x -= gdX[gNum];
    }

    uint8_t* pbuffer = nullptr;
    const uint8_t* gPtr = (const uint8_t*) gFont.gArray;

    if (fs_font) {
      fontFile.seek(gBitmap[gNum], fs::SeekSet);
      pbuffer = (uint8_t*)malloc(gWidth[gNum]);
    }

    int16_t cy = cursor_y + gFont.maxAscent - gdY[gNum];
    int16_t cx = cursor_x + gdX[gNum];

    int16_t  fxs = cx;
    uint32_t fl = 0;
    int16_t  bxs = cx;
    uint32_t bl = 0;
    int16_t  bx = 0;
    uint8_t pixel = 0;

    int16_t fillwidth  = 0;
    int16_t fillheight = 0;

    // Fill area above glyph
    if (_fillbg) {
      fillwidth  = (cursor_x + gxAdvance[gNum]) - bg_cursor_x;
      if (fillwidth > 0) {
        fillheight = gFont.maxAscent - gdY[gNum];
        if (fillheight > 0) fillRect(bg_cursor_x, cursor_y, fillwidth, fillheight, textbgcolor);
      }
      else fillwidth = 0;

      // Fill any area to left of glyph                              
      if (bg_cursor_x < cx) fillRect(bg_cursor_x, cy, cx - bg_cursor_x, gHeight[gNum], textbgcolor);
      // Set x position in glyph area where background starts
      if (bg_cursor_x > cx) bx = bg_cursor_x - cx;
      // Fill any area to right of glyph
      if (cx + gWidth[gNum] < cursor_x + gxAdvance[gNum]) fillRect(cx + gWidth[gNum], cy, (cursor_x + gxAdvance[gNum]) - (cx + gWidth[gNum]), gHeight[gNum], textbgcolor);
    }

    for (int32_t y = 0; y < gHeight[gNum]; y++) {
      if (fs_font) fontFile.read(pbuffer, gWidth[gNum]);

      for (int32_t x = 0; x < gWidth[gNum]; x++) {
        if (fs_font) pixel = pbuffer[x];
        else pixel = pgm_read_byte(gPtr + gBitmap[gNum] + x + gWidth[gNum] * y);

        if (pixel) {
          if (bl) { drawFastHLine( bxs, y + cy, bl, bg); bl = 0; }
          if (pixel != 0xFF)
          {
            if (fl) {
              if (fl==1) drawPixel(fxs, y + cy, fg);
              else drawFastHLine( fxs, y + cy, fl, fg);
              fl = 0;
            }
            if (getBG) bg = readPixel(x + cx, y + cy);
            drawPixel(x + cx, y + cy, alphaBlend(pixel, fg, bg));
          } else {
            if (fl==0) fxs = x + cx;
            fl++;
          }
        } else {
          if (fl) { drawFastHLine( fxs, y + cy, fl, fg); fl = 0; }
          if (_fillbg) {
            if (x >= bx) {
              if (bl==0) bxs = x + cx;
              bl++;
            }
          }
        }
      }
      if (fl) { drawFastHLine( fxs, y + cy, fl, fg); fl = 0; }
      if (bl) { drawFastHLine( bxs, y + cy, bl, bg); bl = 0; }
    }

    // Fill area below glyph
    if (fillwidth > 0) {
      fillheight = (cursor_y + gFont.yAdvance) - (cy + gHeight[gNum]);
      if (fillheight > 0) fillRect(bg_cursor_x, cy + gHeight[gNum], fillwidth, fillheight, textbgcolor);
    }

    if (pbuffer) free(pbuffer);
    cursor_x += gxAdvance[gNum];

    if (newSprite) {
      pushSprite(cx, cursor_y);
      deleteSprite();
    }
  }
  else {
    drawRect(cursor_x, cursor_y + gFont.maxAscent - gFont.ascent, gFont.spaceWidth, gFont.ascent, fg);
    cursor_x += gFont.spaceWidth + 1;
  }
  bg_cursor_x = cursor_x;
  last_cursor_x = cursor_x;
}

void TFT_eSprite::printToSprite(String string)
{
  if(!fontLoaded) return;
  printToSprite((char*)string.c_str(), string.length());
}

void TFT_eSprite::printToSprite(char *cbuffer, uint16_t len) //String string)
{
  if(!fontLoaded) return;

  uint16_t n = 0;
  bool newSprite = !_created;
  int16_t  cursorX = _tft->cursor_x;

  if (newSprite) {
    int16_t sWidth = 0;
    uint16_t index = 0;
    bool first = true;
    while (n < len) {
      uint16_t unicode = decodeUTF8((uint8_t*)cbuffer, &n, len - n);
      if (getUnicodeIndex(unicode, &index)) {
        if (first) {
          first = false;
          sWidth -= gdX[index];
          cursorX += gdX[index];
        }
        if (n == len) sWidth += ( gWidth[index] + gdX[index]);
        else sWidth += gxAdvance[index];
      }
      else sWidth += gFont.spaceWidth + 1;
    }

    createSprite(sWidth, gFont.yAdvance);

    if (textcolor != textbgcolor) fillSprite(textbgcolor);
  }

  n = 0;

  while(n < len) drawGlyph(decodeUTF8((uint8_t*)cbuffer, &n, len - n));

  if (newSprite) {
    pushSprite(cursorX, _tft->cursor_y);
    deleteSprite();
  }
}

int16_t TFT_eSprite::printToSprite(int16_t x, int16_t y, uint16_t index) {
  bool newSprite = !_created;
  int16_t sWidth = gWidth[index];

  if (newSprite) {
    createSprite(sWidth, gFont.yAdvance);

    if (textcolor != textbgcolor) fillSprite(textbgcolor);

    drawGlyph(gUnicode[index]);

    pushSprite(x + gdX[index], y, textbgcolor);
    deleteSprite();
  }

  else drawGlyph(gUnicode[index]);

  return gxAdvance[index];
}
