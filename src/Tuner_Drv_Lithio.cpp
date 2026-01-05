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

void devTEF_Radio_Set_Noiseblanker_AM(uint8_t mode, uint16_t start) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_NoiseBlanker, 7, mode, start);
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_NoiseBlanker_Audio, 7, mode, 1000);
}

uint8_t devTEF_APPL_Get_Operation_Status() {
  uint8_t buf[2];
  devTEF_Get_Cmd(TEF_APPL, Cmd_Get_Operation_Status, buf, sizeof(buf));
  return Convert8bto16b(buf);
}

bool devTEF_Radio_Get_Processing_Status (uint16_t *highcut, uint16_t *stereo, uint16_t *sthiblend, uint8_t *stband_1, uint8_t *stband_2, uint8_t *stband_3, uint8_t *stband_4) {
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

bool devTEF_Radio_Get_Quality_Status (uint16_t *status, int16_t *level, uint16_t *usn, uint16_t *wam, int16_t *offset, uint16_t *bandwidth, uint16_t *mod, int8_t *snr) {
  uint8_t buf[14];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Quality_Data, buf, sizeof(buf));
  *status = Convert8bto16b(buf);
  *level = Convert8bto16b(buf + 2);
  *usn = Convert8bto16b(buf + 4);
  *wam = Convert8bto16b(buf + 6);
  *offset = Convert8bto16b(buf + 8);
  *bandwidth = Convert8bto16b(buf + 10) / 10;
  *mod = Convert8bto16b(buf + 12) / 10;
  if (*level < -200) *level = -200;
  if (*level > 1200) *level = 1200;
  *snr = int(0.46222375 * (float)(*level) / 10 - 0.082495118 * (float)(*usn) / 10) + 10;
  return r;
}

bool devTEF_Radio_Get_Quality_Status_AM(int16_t *level, uint16_t *noise, uint16_t *cochannel, int16_t *offset, uint16_t *bandwidth, uint16_t *mod, int8_t *snr) {
  uint8_t buf[14];
  uint16_t r = devTEF_Get_Cmd(TEF_AM, Cmd_Get_Quality_Data, buf, sizeof(buf));

  *level = Convert8bto16b(buf + 2);
  *noise = Convert8bto16b(buf + 4);
  *cochannel = Convert8bto16b(buf + 6);
  *offset = Convert8bto16b(buf + 8);
  *bandwidth = Convert8bto16b(buf + 10) / 10;
  *mod = Convert8bto16b(buf + 12) / 10;
  if (*level < -200) *level = -200;
  if (*level > 1200) *level = 1200;
  *snr = int(0.46222375 * (float)(*level) / 10 - 0.082495118 * (float)(*noise / 50) / 10) + 10;
  return r;
}


bool devTEF_Radio_Get_RDS_Status (uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error) {
  uint8_t buf[12];
  uint8_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_RDS_Status, buf, sizeof(buf));
  *status = Convert8bto16b(buf);
  *A_block = Convert8bto16b(buf + 2);
  *B_block = Convert8bto16b(buf + 4);
  *C_block = Convert8bto16b(buf + 6);
  *D_block = Convert8bto16b(buf + 8);
  *dec_error = Convert8bto16b(buf + 10);
  return r;
}

bool devTEF_Radio_Get_RDS_Data (uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error) {
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

void devTEF_Radio_Set_GPIO(uint8_t mode) {
  if (mode == 0) devTEF_Set_Cmd(TEF_APPL, Cmd_Set_GPIO, 9, 0, 33, 2);
  if (mode == 1) devTEF_Set_Cmd(TEF_APPL, Cmd_Set_GPIO, 9, 0, 33, 3);
  if (mode == 2) devTEF_Set_Cmd(TEF_APPL, Cmd_Set_GPIO, 9, 0, 32, 2);
  if (mode == 3) devTEF_Set_Cmd(TEF_APPL, Cmd_Set_GPIO, 9, 0, 32, 3);
}