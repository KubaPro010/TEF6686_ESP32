#pragma once

#define High_16bto8b(a) (uint8_t)((a) >> 8)
#define Low_16bto8b(a) (uint8_t)(a )
#define Convert8bto16b(a) (uint16_t)(((uint16_t)(*(a))) << 8 |((uint16_t)(*(a+1))))

typedef enum {
  TEF_FM = 32,
  TEF_AM = 33,
  TEF_AUDIO = 48,
  TEF_APPL = 64
} TEF_MODULE;

typedef enum {
  Cmd_Tune_To =  1,
  Cmd_Set_Bandwidth = 10,
  Cmd_Set_RFAGC = 11,
  Cmd_Set_Antenna = 12,
  Cmd_Set_CoChannelDet = 14,
  Cmd_Set_MphSuppression = 20,
  Cmd_Set_ChannelEqualizer = 22,
  Cmd_Set_NoiseBlanker = 23,
  Cmd_Set_NoiseBlanker_Audio = 24,
  Cmd_Set_Deemphasis = 31,
  Cmd_Set_StereoImprovement = 32,
  Cmd_Set_LevelOffset = 39,
  Cmd_Set_Softmute_Max = 45,
  Cmd_Set_Highcut_Level = 52,
  Cmd_Set_Highcut_Noise = 53,
  Cmd_Set_Highcut_Mph = 54,
  Cmd_Set_Highcut_Max = 55,
  Cmd_Set_Stereo_Level = 62,
  Cmd_Set_Stereo_Noise = 63,
  Cmd_Set_Stereo_Mph = 64,
  Cmd_Set_Stereo_Min = 66,
  Cmd_Set_StHiBlend_Level = 72,
  Cmd_Set_StHiBlend_Noise = 73,
  Cmd_Set_StHiBlend_Mph = 74,
  Cmd_Set_StHiBlend_Max = 75,
  Cmd_Set_RDS = 81,
  Cmd_Set_Specials = 85,
  Cmd_Set_Bandwidth_Options = 86,
  Cmd_Set_StBandBlend_Time = 90,
  Cmd_Set_StBandBlend_Gain = 91,
  Cmd_Set_StBandBlend_Bias = 92,
  Cmd_Get_Quality_Status = 128,
  Cmd_Get_Quality_Data = 129,
  Cmd_Get_RDS_Status = 130,
  Cmd_Get_RDS_Data = 131,
  Cmd_Get_Signal_Status = 133,
  Cmd_Get_Processing_Status = 134
} TEF_RADIO_COMMAND;

typedef enum {
  Cmd_Set_Volume = 10,
  Cmd_Set_Mute = 11,
  Cmd_Set_Input = 12,
  Cmd_Set_WaveGen = 24
} TEF_AUDIO_COMMAND;

typedef enum {
  Cmd_Set_OperationMode = 1,
  Cmd_Set_GPIO = 3,
  Cmd_Get_Operation_Status = 128,
  Cmd_Get_Identification = 130
} TEF_APPL_COMMAND;

void devTEF_Set_Cmd(TEF_MODULE module, uint8_t cmd, uint16_t len, ...);
bool devTEF_Get_Cmd(TEF_MODULE module, uint8_t cmd, uint8_t *receive, uint16_t len);
void devTEF_Radio_Set_Wavegen(bool mode, int16_t amplitude, uint16_t freq);

void devTEF_Radio_Get_Quality_Status(uint16_t *status, int16_t *level, uint16_t *usn, uint16_t *wam, int16_t *offset, uint16_t *bandwidth, uint16_t *mod, int8_t *snr);
bool devTEF_Radio_Get_Processing_Status(uint16_t *highcut, uint16_t *stereo, uint16_t *sthiblend, uint8_t *stband_1, uint8_t *stband_2, uint8_t *stband_3, uint8_t *stband_4);
uint8_t devTEF_APPL_Get_Operation_Status();
bool devTEF_Radio_Get_Stereo_Status(uint16_t *status);
void devTEF_Radio_Get_RDS_Status(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error);
bool devTEF_Radio_Get_RDS_Data(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error);