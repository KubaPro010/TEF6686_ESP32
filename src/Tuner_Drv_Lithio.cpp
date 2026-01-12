#include "TEF6686.h"
#include <stdarg.h>

void devTEF_Set_Cmd(TEF_MODULE module, uint8_t cmd, uint16_t len, ...) {
  uint16_t i, temp;
  uint8_t buf[20];
  va_list vArgs;
  va_start(vArgs, len);
  buf[0] = module;
  buf[1] = cmd;
  buf[2] = 1;

  for (i = 3; i < len; i++) {
    temp = va_arg(vArgs, int);
    buf[i++] = High_16bto8b(temp);
    buf[i] = Low_16bto8b(temp);
  }

  va_end(vArgs);
  Tuner_WriteBuffer(buf, len);
}

bool devTEF_Get_Cmd(TEF_MODULE module, uint8_t cmd, uint8_t *receive, uint16_t len) {
  uint8_t buf[3];
  buf[0] = module;
  buf[1] = cmd;
  buf[2] = 1;

  Tuner_WriteBuffer(buf, 3);
  return Tuner_ReadBuffer(receive, len);
}

uint8_t devTEF_APPL_Get_Operation_Status() {
  uint8_t buf[2];
  while(!devTEF_Get_Cmd(TEF_APPL, Cmd_Get_Operation_Status, buf, sizeof(buf))) delay(1);
  return Convert8bto16b(buf);
}

bool devTEF_Radio_Get_Processing_Status(uint16_t *highcut, uint16_t *stereo, uint16_t *sthiblend, uint8_t *stband_1, uint8_t *stband_2, uint8_t *stband_3, uint8_t *stband_4) {
  uint8_t buf[12];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Processing_Status, buf, sizeof(buf));
  *highcut = Convert8bto16b(buf + 2) / 10;
  *stereo = Convert8bto16b(buf + 4) / 10;
  *sthiblend = Convert8bto16b(buf + 6) / 10;
  uint16_t stband_1_2 = Convert8bto16b(buf + 8);
  uint16_t stband_3_4 = Convert8bto16b(buf + 10);
  *stband_1 = High_16bto8b(stband_1_2);
  *stband_2 = Low_16bto8b(stband_1_2);
  *stband_3 = High_16bto8b(stband_3_4);
  *stband_4 = Low_16bto8b(stband_3_4);
  return r;
}

void devTEF_Radio_Get_Quality_Status(uint16_t *status, int16_t *level, uint16_t *usn, uint16_t *wam, int16_t *offset, uint16_t *bandwidth, uint16_t *mod, int8_t *snr) {
  uint8_t buf[14];
  devTEF_Get_Cmd(TEF_FM, Cmd_Get_Quality_Data, buf, sizeof(buf));
  if(status != NULL) *status = Convert8bto16b(buf);
  if(level != NULL) *level = Convert8bto16b(buf + 2);
  if(usn != NULL) *usn = Convert8bto16b(buf + 4);
  if(wam != NULL) *wam = Convert8bto16b(buf + 6);
  if(offset != NULL) *offset = Convert8bto16b(buf + 8);
  if(bandwidth != NULL) *bandwidth = Convert8bto16b(buf + 10) / 10;
  if(mod != NULL) *mod = Convert8bto16b(buf + 12) / 10;
  if (*level < -200) *level = -200;
  if (*level > 1200) *level = 1200;
  if(snr != NULL) *snr = int(0.46222375 * (float)(*level) / 10 - 0.082495118 * (float)(*usn) / 10) + 10;
}

void devTEF_Radio_Get_RDS_Status(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error) {
  uint8_t buf[12];
  devTEF_Get_Cmd(TEF_FM, Cmd_Get_RDS_Status, buf, sizeof(buf));
  if(status != NULL) *status = Convert8bto16b(buf);
  if(A_block != NULL) *A_block = Convert8bto16b(buf + 2);
  if(B_block != NULL) *B_block = Convert8bto16b(buf + 4);
  if(C_block != NULL) *C_block = Convert8bto16b(buf + 6);
  if(D_block != NULL) *D_block = Convert8bto16b(buf + 8);
  if(dec_error != NULL) *dec_error = Convert8bto16b(buf + 10);
}

bool devTEF_Radio_Get_RDS_Data(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error) {
  uint8_t buf[12];
  uint8_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_RDS_Data, buf, sizeof(buf));
  *status = Convert8bto16b(buf);
  *A_block = Convert8bto16b(buf + 2);
  *B_block = Convert8bto16b(buf + 4);
  *C_block = Convert8bto16b(buf + 6);
  *D_block = Convert8bto16b(buf + 8);
  *dec_error = Convert8bto16b(buf + 10);
  return r;
}

bool devTEF_Radio_Get_Stereo_Status(uint16_t *status) {
  uint8_t buf[2];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Signal_Status, buf, sizeof(buf));

  *status = Convert8bto16b(buf);
  return r;
}

void devTEF_Radio_Set_Wavegen(bool mode, int16_t amplitude, uint16_t freq) {
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Input, 5, mode ? 240 : 0);
  if (mode) devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_WaveGen, 15, 5, 0, amplitude * 10, freq, amplitude * 10, freq);
  else devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_WaveGen, 15, 0);
}