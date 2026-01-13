#include "globals.h"
#include "Tuner_Patch_Lithio_V102_p224.h"
#ifndef DEEPELEC_DP_66X
#include "Tuner_Patch_Lithio_V205_p512.h"
#endif
#include <Wire.h>

bool Tuner_WriteBuffer(unsigned char *buf, uint16_t len) {
  Wire.beginTransmission(TEF668X_ADDRESS);
  for (uint16_t i = 0; i < len; i++) Wire.write(buf[i]);
  uint8_t r = Wire.endTransmission();
  if (!Data_Accelerator) delay(2);
  return (r == 0) ? true : false;
}

bool Tuner_ReadBuffer(unsigned char *buf, uint16_t len) {
  Wire.requestFrom(TEF668X_ADDRESS, len);
  if (Wire.available() == len) {
    for (uint16_t i = 0; i < len; i++) buf[i] = Wire.read();
    return true;
  }
  return false;
}

static void Tuner_Patch_Load(const unsigned char *pLutBytes, uint16_t size) {
  unsigned char buf[24 + 1];
  uint16_t i, len;
  uint16_t r;
  buf[0] = 0x1b;

  while (size) {
    len = (size > 24) ? 24 : size;
    size -= len;

    for (i = 0; i < len; i++) buf[1 + i] = pgm_read_byte(&pLutBytes[i]);
    pLutBytes += len;

    if (1 != (r = Tuner_WriteBuffer(buf, len + 1))) break;
  }
}

void Tuner_Reset() {
  Wire.beginTransmission(TEF668X_ADDRESS);
  Wire.write(0x1e);
  Wire.write(0x5a);
  Wire.write(0x01);
  Wire.write(0x5a);
  Wire.write(0x5a);
  Wire.endTransmission();
}

void Tuner_Patch(byte TEF) {
  Wire.beginTransmission(TEF668X_ADDRESS);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(TEF668X_ADDRESS);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x74);
  Wire.endTransmission();
  switch (TEF) {
    case 102:
      Tuner_Patch_Load(pPatchBytes102, PatchSize102);
      break;
#ifndef DEEPELEC_DP_66X
    case 205:
      Tuner_Patch_Load(pPatchBytes205, PatchSize205);
      break;
#endif
  }
  Wire.beginTransmission(TEF668X_ADDRESS);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(TEF668X_ADDRESS);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x75);
  Wire.endTransmission();
  switch (TEF) {
    case 102:
      Tuner_Patch_Load(pLutBytes102, LutSize102);
      break;
#ifndef DEEPELEC_DP_66X
    case 205:
      Tuner_Patch_Load(pLutBytes205, LutSize205);
      break;
#endif
  }
  Wire.beginTransmission(TEF668X_ADDRESS);
  Wire.write(0x1c);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
}