#include "TEF6686.h"
#include <map>
#include <Arduino.h>
#include <SPIFFS.h>
#include "constants.h"
#include "utils.h"

uint8_t dropped_groups = 0;
bool lastBitState = false;

uint16_t TEF6686::getBlockA() {
  uint16_t blockA;
  devTEF_Radio_Get_RDS_Status(NULL, &blockA, NULL, NULL, NULL, NULL);
  return blockA;
}

void TEF6686::TestAFEON() {
  uint16_t status, afusn, afwam;
  int16_t aflevel, afoffset;
  byte timing;

  if (af_counter != 0) {
    setMute();
    for (int x = 0; x < af_counter; x++) {
      timing = 0;
      devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 3, af[x].frequency);
      while (timing == 0 && !bitRead(timing, 15)) {
        devTEF_Radio_Get_Quality_Status(&status, &aflevel, &afusn, &afwam, &afoffset, NULL, NULL, NULL);
        timing = lowByte(status);
      }
      if (afoffset > -125 || afoffset < 125) {
        devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 4, af[x].frequency);
        delay(187);
        devTEF_Radio_Get_RDS_Status(&rds.rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);

        if (bitRead(rds.rdsStat, 9)) {
          if ((afmethodB && rds.afreg ? (((rds.rdsA >> 8) & 0xF) > 2 && ((rds.correctPI >> 8) & 0xF) > 2 && ((rds.rdsA >> 12) & 0xF) == ((rds.correctPI >> 12) & 0xF) && (rds.rdsA & 0xFF) == (rds.correctPI & 0xFF)) || rds.rdsA == rds.correctPI : rds.rdsA == rds.correctPI)) {
            af[x].checked = true;
            af[x].afvalid = true;
          } else {
            af[x].checked = false;
            af[x].afvalid = false;
          }
        } else af[x].checked = false;
      }
    }
  }
  devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 4, currentfreq);
  setUnMute();
}

uint16_t TEF6686::TestAF() {
  if (af_counter != 0) {
    uint16_t status, afusn, afwam, currentusn, currentwam;
    int16_t aflevel, afoffset, currentoffset, currentlevel;
    byte timing;

    devTEF_Radio_Get_Quality_Status(&status, &currentlevel, &currentusn, &currentwam, &currentoffset, NULL, NULL, NULL);
    devTEF_Radio_Get_RDS_Status(&rds.rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);

    for (int x = 0; x < af_counter; x++) {
      timing = 0;
      devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 3, af[x].frequency);
      while (timing == 0 && !bitRead(timing, 15)) {
        devTEF_Radio_Get_Quality_Status(&status, &aflevel, &afusn, &afwam, &afoffset, NULL, NULL, NULL);
        timing = lowByte(status);
      }
      af[x].score = aflevel - afusn - afwam;
      if (afoffset < -125 || afoffset > 125) af[x].score = -32767;
    }

    int16_t highestValue = af[0].score;
    int highestIndex = 0;

    for (int i = 1; i < af_counter; i++) {
      if (af[i].score > highestValue) {
        highestValue = af[i].score;
        highestIndex = i;
      }
    }

    if (af_counter != 0 && af[highestIndex].afvalid && af[highestIndex].score > (currentlevel - currentusn - currentwam) && (af[highestIndex].score - (currentlevel - currentusn - currentwam)) >= 70) {
      devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 4, af[highestIndex].frequency);
      delay(187);
      devTEF_Radio_Get_RDS_Status(&rds.rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);
      if (bitRead(rds.rdsStat, 9)) {
        if ((afmethodB && rds.afreg ? (((rds.rdsA >> 8) & 0xF) > 2 && ((rds.correctPI >> 8) & 0xF) > 2 && ((rds.rdsA >> 12) & 0xF) == ((rds.correctPI >> 12) & 0xF) && (rds.rdsA & 0xFF) == (rds.correctPI & 0xFF)) || rds.rdsA == rds.correctPI : rds.rdsA == rds.correctPI)) {
          currentfreq = af[highestIndex].frequency;
          for (byte y = 0; y < 50; y++) {
            af[y].frequency = 0;
            af[y].score = -32767;
            af[y].afvalid = true;
            af[y].checked = false;
          }
          af_counter = 0;
        } else {
          af[highestIndex].afvalid = false;
          devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 4, currentfreq);
        }
      } else devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 4, currentfreq);
    }
  }
  return currentfreq;
}

void TEF6686::init(byte TEF) {
  Tuner_Reset();

  while(devTEF_APPL_Get_Operation_Status() != 0) delay(2);

  uint32_t clock = 12000000;

#ifndef DEEPELEC_DP_66X
  int xtalMV = analogReadMilliVolts(15);

  if (xtalMV < 0 + XTAL_MV_TOL) clock = 9216000;
  else if (xtalMV > 1000 - XTAL_MV_TOL && xtalMV < 1000 + XTAL_MV_TOL);
  else if (xtalMV > 2000 - XTAL_MV_TOL && xtalMV < 2000 + XTAL_MV_TOL) clock = 55466670;
  else clock = 4000000;
#endif

  // Send the firmware to the device
  Tuner_Patch(TEF);

  // Start the firmware
  devTEF_Set_Cmd(TEF_INIT, 0, 0);

  while(devTEF_APPL_Get_Operation_Status() != 1) delay(2); // Wait for it to load

  if(clock != 9216000) devTEF_Set_Cmd(TEF_APPL, Cmd_Set_ReferenceClock, 6, (clock >> 16) & 0xffff, clock & 0xffff, (clock == 55466670) ? 1 : 0);
  devTEF_Set_Cmd(TEF_APPL, Cmd_Set_Activate, 2, 1); // Setup done, start radio

  while(devTEF_APPL_Get_Operation_Status() != 2) delay(2); // Wait for it to start

  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Mph, 6, 0, 360, 300);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Max, 4, 0, 4000);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_LowCut_Max, 4, 0, 100);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Time, 8, 60, 120, 100, 200);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Time, 8, 500, 2000, 200, 200);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Level, 6, 0, 600, 240);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Noise, 6, 0, 160, 140);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Mph, 6, 0, 160, 140);
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Max, 4, 0, 4000);
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Ana_Out, 4, 128, 1);
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Output_Source, 4, 128, 224);
}

void TEF6686::getIdentification(uint16_t *device, uint16_t *hw_version, uint16_t *sw_version) {
  uint8_t buf[6];
  devTEF_Get_Cmd(TEF_APPL, Cmd_Get_Identification, buf, sizeof(buf));

  *device = Convert8bto16b(buf);
  *hw_version = Convert8bto16b(buf + 2);
  *sw_version = Convert8bto16b(buf + 4);
}

void TEF6686::power(bool mode) {
  devTEF_Set_Cmd(TEF_APPL, Cmd_Set_OperationMode, 2, mode);
}

void TEF6686::extendBW(bool yesno) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Bandwidth_Options, 2, (yesno ? 400 : 950));
}

void TEF6686::SetFreq(uint16_t frequency) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 4, 1, frequency);
  currentfreq = ((frequency + 5) / 10) * 10;
  currentfreq2 = frequency;
}

void TEF6686::SetFreqAM(uint16_t frequency) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Tune_To, 4, 1, frequency);
}

void TEF6686::setOffset(int8_t offset) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_LevelOffset, 2, (offset * 10) - 70);
}

void TEF6686::setAMOffset(int8_t offset) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_LevelOffset, 2, (offset * 10) - 70);
}

void TEF6686::setFMBandw(uint16_t bandwidth) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Bandwidth, 4, 0, bandwidth * 10);
}

void TEF6686::setAMBandw(uint16_t bandwidth) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_Bandwidth, 4, 0, bandwidth * 10);
}

void TEF6686::setAMCoChannel(uint16_t start, uint8_t level) {
  uint8_t mode = 1;
  if(start == 0) mode = 0;
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_CoChannelDet, 8, mode, 2, start * 10, 1000, level);
}

void TEF6686::setSoftmuteAM(uint8_t mode) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_Softmute_Max, 4, mode, 250);
}

void TEF6686::setSoftmuteFM(uint8_t mode) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Softmute_Max, 4, mode, 200);
}

void TEF6686::setAMNoiseBlanker(uint16_t start) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_NoiseBlanker, 4, (start == 0) ? 0 : 1, (start == 0) ? 1000 : start * 10);
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_NoiseBlanker_Audio, 4, (start == 0) ? 0 : 1, 1000);
}

void TEF6686::setAMAttenuation(uint16_t start) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_Antenna, 2, start * 10);
}

void TEF6686::setFMABandw() {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Bandwidth, 4, 1, 3110);
}

void TEF6686::setiMS(bool mph) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_MphSuppression, 2, mph);
}

void TEF6686::setEQ(bool eq) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_ChannelEqualizer, 2, eq);
}

bool TEF6686::getStereoStatus() {
  uint16_t status;
  bool stereo = 0;
  if (1 == devTEF_Radio_Get_Stereo_Status(&status)) stereo = ((status >> 15) & 1) ? 1 : 0;
  return stereo;
}

void TEF6686::setMono(bool mono) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Min, 4, mono ? 2 : 0);
}

void TEF6686::setVolume(int8_t volume) {
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Volume, 2, volume * 10);
}

void TEF6686::setMute() {
  mute = true;
  if (mpxmode) devTEF_Set_Cmd(TEF_FM, Cmd_Set_Specials, 2, 0);
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Mute, 2, 1);
}

void TEF6686::setUnMute() {
  mute = false;
  if (mpxmode) devTEF_Set_Cmd(TEF_FM, Cmd_Set_Specials, 2, 1);
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Mute, 2, 0);
}

void TEF6686::setAGC(uint8_t agc) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_RFAGC, 4, agc * 10, 0);
}

void TEF6686::setAMAGC(uint8_t agc) {
  devTEF_Set_Cmd(TEF_AM, Cmd_Set_RFAGC, 4, agc * 10, 0);
}

void TEF6686::setDeemphasis(uint8_t timeconstant) {
  switch (timeconstant) {
    case 1: devTEF_Set_Cmd(TEF_FM, Cmd_Set_Deemphasis, 2, 500); break;
    case 2: devTEF_Set_Cmd(TEF_FM, Cmd_Set_Deemphasis, 2, 750); break;
    default: devTEF_Set_Cmd(TEF_FM, Cmd_Set_Deemphasis, 2, 0); break;
  }
}

void TEF6686::setAudio(uint8_t audio) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Specials, 2, audio);
  mpxmode = (audio != 0);
}

void TEF6686::setFMSI(uint8_t mode) {
  if(mode > 2) mode = 2;
  if(mode < 1) mode = 1;
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StereoImprovement, 2, mode-1);
}

void TEF6686::setFMSI_Time(uint16_t attack, uint16_t decay) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StBandBlend_Time, 4, attack, decay);
}

void TEF6686::setFMSI_Gain(uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StBandBlend_Gain, 8, band1 * 10, band2 * 10, band3 * 10, band4 * 10);
}

void TEF6686::setFMSI_Bias(int16_t band1, int16_t band2, int16_t band3, int16_t band4) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StBandBlend_Bias, 8, band1 - 250, band2 - 250, band3 - 250, band4 - 250);
}

void TEF6686::setFMNoiseBlanker(uint16_t start) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_NoiseBlanker, 4, (start == 0) ? 0 : 1, (start == 0) ? 1000 : (start * 10));
}

void TEF6686::setStereoLevel(uint8_t start) {
  if (start == 0) {
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Level, 6, 0, start * 10, 60);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Noise, 6, 0, 240, 200);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Mph, 6, 0, 240, 200);
  } else {
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Level, 6, 3, start * 10, 60);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Noise, 6, 3, 240, 200);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Mph, 6, 3, 240, 200);
  }
}

void TEF6686::setHighCutOffset(uint8_t start) {
  if (start == 0) {
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Level, 6, 0, start * 10, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Noise, 6, 0, 360, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Mph, 6, 0, 360, 300);
  } else {
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Level, 6, 3, start * 10, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Noise, 6, 3, 360, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Mph, 6, 3, 360, 300);
  }
}

void TEF6686::setHighCutLevel(uint16_t limit) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Max, 4, 1, limit * 100);
}

void TEF6686::setStHiBlendLevel(uint16_t limit) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Max, 4, 1, limit * 100);
}

void TEF6686::setStHiBlendOffset(uint8_t start) {
  if (start == 0) {
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Level, 6, 0, start * 10, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Noise, 6, 0, 360, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Mph, 6, 0, 360, 300);
  } else {
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Level, 6, 3, start * 10, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Noise, 6, 3, 360, 300);
    devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Mph, 6, 3, 360, 300);
  }
}

void TEF6686::getStatus(int16_t *level, uint16_t *USN, uint16_t *WAM, int16_t *offset, uint16_t *bandwidth, uint16_t *modulation, int8_t *snr) {
  devTEF_Radio_Get_Quality_Status(NULL, level, USN, WAM, offset, bandwidth, modulation, snr);
}

void TEF6686::getStatusAM(int16_t *level, uint16_t *noise, uint16_t *cochannel, int16_t *offset, uint16_t *bandwidth, uint16_t *modulation, int8_t *snr) {
  uint8_t buf[14];
  devTEF_Get_Cmd(TEF_AM, Cmd_Get_Quality_Data, buf, sizeof(buf));
  *level = Convert8bto16b(buf + 2);
  *noise = Convert8bto16b(buf + 4);
  *cochannel = Convert8bto16b(buf + 6);
  *offset = Convert8bto16b(buf + 8);
  *bandwidth = Convert8bto16b(buf + 10) / 10;
  *modulation = Convert8bto16b(buf + 12) / 10;
  if (*level < -200) *level = -200;
  if (*level > 1200) *level = 1200;
  *snr = int(0.46222375 * (float)(*level) / 10 - 0.082495118 * (float)(*noise / 50) / 10) + 10;
}

void TEF6686::readRDS(byte showrdserrors) {
  if(rds.filter && ps_process) devTEF_Radio_Get_RDS_Status(&rds.rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);
  else {
    for(int i = 0; i < 4; i++) {
      devTEF_Radio_Get_RDS_Data(&rds.rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);
      delay(1);
      if(bitRead(rds.rdsStat, 15)) break;
    }
  }

  if(bitRead(rds.rdsStat, 9)) {
    rds.hasRDS = true;
    dropped_groups = 0;
  } else {
    if(dropped_groups > 7) rds.hasRDS = false;
    dropped_groups++;
    return; // No sync means no data, ever! Unless sync status changes of course
  }

  if(bitRead(rds.rdsStat, 15) == 0) return; // No data, no fucks

  rdsAerrorThreshold = ((rds.rdsErr >> 14) & 0x03) > showrdserrors;
  rdsBerrorThreshold = ((rds.rdsErr >> 12) & 0x03) > showrdserrors;
  rdsCerrorThreshold = ((rds.rdsErr >> 10) & 0x03) > showrdserrors;
  rdsDerrorThreshold = ((rds.rdsErr >> 8) & 0x03) > showrdserrors;

  uint16_t pi = 0;
  if(!rdsAerrorThreshold) pi = rds.rdsA; // Standard, if we have group A with block A available
  else if(bitRead(rds.rdsStat, 12) && !rdsCerrorThreshold) {
    // Little less standard. RDS type B groups always have PI in blocks A and C, it helps that block c of group b checkword is diffrent from the group a checkword, the tef has flag for that
    // If a station broadcasts B groups often then their PI code could get received easier in bad conditions
    pi = rds.rdsC;
  }

  if ((rds.rdsA != previous_rdsA || rds.rdsB != previous_rdsB || rds.rdsC != previous_rdsC || rds.rdsD != previous_rdsD)) {
    rds.rdsAerror = ((rds.rdsErr >> 14) & 0x03) > 1;
    rds.rdsBerror = ((rds.rdsErr >> 12) & 0x03) > 1;
    rds.rdsCerror = ((rds.rdsErr >> 10) & 0x03) > 1;
    rds.rdsDerror = ((rds.rdsErr >> 8) & 0x03) > 1;

    if (pi != 0 && afreset) {
      rds.correctPI = pi;
      afreset = false;
    }

    if (pi != 0 || (rds.pierrors && !errorfreepi)) {
      if (piold == 0 || pi != piold) {
        piold = pi;
        rds.picode[0] = (pi >> 12) & 0xF;
        rds.picode[1] = (pi >> 8) & 0xF;
        rds.picode[2] = (pi >> 4) & 0xF;
        rds.picode[3] = pi & 0xF;
        for (int i = 0; i < 4; i++) {
          if (rds.picode[i] < 10) rds.picode[i] += '0';
          else rds.picode[i] += 'A' - 10;
        }
      }

      if (ps_process && ((rds.rdsErr >> 14) & 0x03) == 0) {
        rds.picode[4] = ' ';
        rds.picode[5] = ' ';
        errorfreepi = true;
      }

      if (!errorfreepi) {
        if (((rds.rdsErr >> 14) & 0x03) > 1) rds.picode[5] = '?'; else rds.picode[5] = ' ';
        if (((rds.rdsErr >> 14) & 0x03) > 0) rds.picode[4] = '?'; else rds.picode[4] = ' ';
      } else {
        rds.picode[4] = ' ';
        rds.picode[5] = ' ';
      }
      rds.picode[6] = '\0';

      if (strncmp(rds.picode, "0000", 4) == 0 && rds.rdsB == 0 && rds.stationName.length() == 0) memset(rds.picode, 0, sizeof(rds.picode));

      // USA Station callsign decoder GLORY TO TRUMP AND AMERICA RAHHHH (actually hang him and tear up the constititution, not sure what's more of a dictartorship, Venezuela or Trump?)
      if ((rds.region == 1 ? ps_process : true) && rds.correctPI != 0 && rds.region > 0 && correctPIold != rds.correctPI) {
        bool foundMatch = false;
        fs::File file;

        if (rds.region == 1 && SPIFFS.begin(true)) {
          delay(2);
          if (currentfreq2 < 9000) file = SPIFFS.open("/USA_87-90.csv");
          else if (currentfreq2 > 9000 && currentfreq2 < 9200) file = SPIFFS.open("/USA_90-92.csv");
          else if (currentfreq2 > 9200 && currentfreq2 < 9400) file = SPIFFS.open("/USA_92-94.csv");
          else if (currentfreq2 > 9400 && currentfreq2 < 9600) file = SPIFFS.open("/USA_94-96.csv");
          else if (currentfreq2 > 9600 && currentfreq2 < 9800) file = SPIFFS.open("/USA_96-98.csv");
          else if (currentfreq2 > 9800 && currentfreq2 < 10000) file = SPIFFS.open("/USA_98-100.csv");
          else if (currentfreq2 > 10000 && currentfreq2 < 10200) file = SPIFFS.open("/USA_100-102.csv");
          else if (currentfreq2 > 10200 && currentfreq2 < 10400) file = SPIFFS.open("/USA_102-104.csv");
          else if (currentfreq2 > 10400 && currentfreq2 < 10600) file = SPIFFS.open("/USA_104-106.csv");
          else if (currentfreq2 > 10600) file = SPIFFS.open("/USA_106-108.csv");

          delay(2);
          if (file) {
            int i = 0;
            while (file.available() && !isprint(file.peek())) {
              file.read();
              i++;
            }

            char buffer[25];
            while (file.available()) {
              int bytesRead = file.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
              buffer[bytesRead] = '\0';

              char *token = strtok(buffer, ";");

              int firstColumnValue = 0;
              uint16_t frequencyValue = 0;
              char stationID[8];
              char stationState[8];

              if (token) {
                firstColumnValue = atoi(token);
                token = strtok(NULL, ";");
                frequencyValue = atoi(token);
                token = strtok(NULL, ";");
                strncpy(stationID, token, sizeof(stationID) - 1);
                stationID[sizeof(stationID) - 1] = '\0';
                token = strtok(NULL, ";");
                strncpy(stationState, token, sizeof(stationState) - 1);
                stationState[sizeof(stationState) - 1] = '\0';
              }

              if (frequencyValue == currentfreq2 && static_cast<uint16_t>(firstColumnValue) == rds.correctPI) {
                strncpy(rds.stationID, stationID, 7);
                strncpy(rds.stationState, stationState, 2);
                foundMatch = true;
                break;
              }
            }
          }
        }

        if (!foundMatch) {
          uint16_t stationID = pi;

          // If stationID is greater than 4096
          if (stationID > 4096) {

            // Adjust stationID based on specific conditions
            if (stationID > 21671) {
              if ((stationID & 0xF00U) == 0) stationID = ((uint16_t)(0xA0 + ((stationID & 0xF000U) >> 12)) << 8) + lowByte(stationID); // C0DE -> ACDE
              else if (lowByte(stationID) == 0) stationID = 0xAF00 + uint8_t(highByte(stationID)); // CD00 -> AFCD
            }

            // Check if the station has a fixed callsign for Canada
            // Here we are skipping the `isFixed` and directly checking if the station is in Canada
            if (!(rds.region == 3 && stationID == 0xAF00)) { // Example condition for a fixed callsign (AF00 is just an example)
              // Determine prefix: 'W' or 'K' for USA, 'C' for Canada
              if (rds.region == 1 || rds.region == 2) rds.stationID[0] = (stationID > 21671) ? 'W' : 'K';
              else if (rds.region == 3) rds.stationID[0] = 'C'; // Canadian stations start with 'C'

              // Adjust stationID based on the region and PI code
              if (stationID < 39247) stationID -= (stationID > 21671) ? 21672 : 4096;
              else stationID -= 4835;

              // Decode the last 3 letters of the callsign
              rds.stationID[1] = char(stationID / 676 + 'A');  // Using 'A' for readability
              stationID %= 676; // Reduce stationID to fit within the last 676 options
              rds.stationID[2] = char(stationID / 26 + 'A');  // Using 'A' for readability
              rds.stationID[3] = char(stationID % 26 + 'A');   // Using 'A' for readability
            }
          }

          // Validate callsign and handle faulty IDs
          bool faultyID = false;
          for (byte i = 0; i < 4; i++) {
            if (!(rds.stationID[i] >= 'A' && rds.stationID[i] <= 'Z')) {
              faultyID = true;
              break;
            }
          }

          // If any character is faulty, mark it as "Unknown"
          if (faultyID) strcpy(rds.stationID, "Unknown");
          else rds.stationID[7] = '?'; // If not faulty, append '?'

          rds.stationID[8] = '\0'; // Null terminate the callsign
        }
        correctPIold = rds.correctPI;
        rds.stationIDtext = rds.stationID;
        rds.stationStatetext = rds.stationState;
      }
    }

    // B errors mean we can't reliably tell what this is, don't risk it.
    // And also make sure we have data in the first place, then the decoder first starts it could send just the pi code with nothing more
    if ((rds.rdsBerror && showrdserrors != 3) && bitRead(rds.rdsStat, 13) == 0) goto end;

    rdsgroup = rds.rdsB >> 11; // Includes version bit as LSB, better for the switch statement
    if(bitRead(rds.rdsStat, 12) && (rdsgroup & 1) == 0) goto end; // Modulation says type B, but data says otherwise? Dunno what to treat it as, thus skip

    rds.TP = bitRead(rds.rdsB, 10);
    rds.PTY = ((rds.rdsB >> 5) & 0x1F);

    rds.blockcounter[rdsgroup]++;
    processed_rdsblocks++;

    switch (rdsgroup) {
      case RDS_GROUP_0A:
      case RDS_GROUP_0B:
        {
          if (showrdserrors == 3 || !rdsDerrorThreshold) {
            uint8_t segment = rds.rdsB & 0x03;

            switch (segment) {
              case 0: if (((rds.rdsErr >> 8) & 0x03) > 1) rds.ps12error = true; else rds.ps12error = false; break;
              case 1: if (((rds.rdsErr >> 8) & 0x03) > 1) rds.ps34error = true; else rds.ps34error = false; break;
              case 2: if (((rds.rdsErr >> 8) & 0x03) > 1) rds.ps56error = true; else rds.ps56error = false; break;
              case 3: if (((rds.rdsErr >> 8) & 0x03) > 1) rds.ps78error = true; else rds.ps78error = false; break;
            }

            ps_buffer2[(segment * 2) + 0] = ps_buffer[(segment * 2) + 0];
            ps_buffer2[(segment * 2) + 1] = ps_buffer[(segment * 2) + 1];
            ps_buffer2[8] = '\0';

            ps_buffer[(segment * 2)  + 0] = rds.rdsD >> 8;
            ps_buffer[(segment * 2)  + 1] = rds.rdsD & 0xFF;
            ps_buffer[8] = '\0';

            if (ps_process || rds.fastps == 0) {
              if (segment == 0) {
                packet0 = true;
                packet1 = false;
                packet2 = false;
                packet3 = false;
              }
              if (segment == 1) packet1 = true;
              if (segment == 2) packet2 = true;
              if (segment == 3) packet3 = true;
            }

            if (packet0 && packet1 && packet2 && packet3 && (ps_process || (rds.fastps == 0 && rds.fastps != 2))) {
              if (strcmp(ps_buffer, ps_buffer2) == 0) {
                ps_process = true;
                RDScharConverter(ps_buffer2, PStext, sizeof(PStext) / sizeof(wchar_t), (underscore > 0 ? true : false));
                String utf8String = convertToUTF8(PStext);
                rds.stationName = extractUTF8Substring(utf8String, 0, 8, (underscore > 0 ? true : false));
                for (byte x = 0; x < 8; x++) {
                  ps_buffer[x] = '\0';
                  ps_buffer2[x] = '\0';
                }
              }
            }

            if ((!ps_process && rds.fastps > 0 && rds.fastps != 2) || rds.fastps == 2) {
              if (segment == 0) packet0 = true;
              if (segment == 1) packet1 = true;
              if (segment == 2) packet2 = true;
              if (segment == 3) packet3 = true;
              RDScharConverter(ps_buffer, PStext, sizeof(PStext) / sizeof(wchar_t), (underscore > 0 ? true : false));
              String utf8String = convertToUTF8(PStext);
              rds.stationName = extractUTF8Substring(utf8String, 0, 8, (underscore > 0 ? true : false));
              if (packet0 && packet1 && packet2 && packet3) ps_process = true;
            }

            if(segment == 0) rds.hasDynamicPTY = bitRead(rds.rdsB, 2) & 0x1F;
            else if(segment == 1) rds.hasCompressed = (bitRead(rds.rdsB, 2) & 0x1F);
            else if(segment == 2) rds.hasArtificialhead = (bitRead(rds.rdsB, 2) & 0x1F);
            else if(segment == 3) rds.hasStereo = (bitRead(rds.rdsB, 2) & 0x1F);
          }

          rds.TA = bitRead(rds.rdsB, 4);

          if (!rdsCerrorThreshold && rdsgroup == RDS_GROUP_0A && rds.rdsC != rdsCold) {
            if ((rds.rdsC >> 8) > 224 && (rds.rdsC >> 8) < 250) {
              afinit = true;
              rds.hasAF = true;
            }

            if (afinit) {
              if ((rds.rdsC >> 8) > 224 && (rds.rdsC >> 8) < 250 && ((rds.rdsC & 0xFF) * 10 + 8750) == currentfreq && rds.hasAF) {
                if (afmethodBtrigger) afmethodB = true;
                afmethodBprobe = true;
                af_counterb = (rds.rdsC >> 8) - 224;
                af_number = (rds.rdsC >> 8) - 224;
                af_counterbcheck = 1;
                doublecounter = 0;
                doubletestfreq = (rds.rdsC & 0xFF) * 10 + 8750;
              } else if ((rds.rdsC >> 8) > 224 && (rds.rdsC >> 8) < 250 && ((rds.rdsC & 0xFF) * 10 + 8750) != currentfreq && rds.hasAF) {
                afmethodBprobe = false;
                afmethodBtrigger = true;
                af_counterb = 0;
                af_number = (rds.rdsC >> 8) - 224;
                af_counterbcheck = 0;
                doublecounter = 0;
                doubletestfreq = (rds.rdsC & 0xFF) * 10 + 8750;
              }

              if ((((rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) && ((rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205)) && afmethodBprobe) af_counterbcheck += 2;

              if ((rds.rdsB >> 11) == 0 && af_counter < 50) {
                uint16_t buffer0;
                uint16_t buffer1;

                if ((rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) buffer0 = (rds.rdsC >> 8) * 10 + 8750; else buffer0 = 0;
                if ((rds.rdsC & 0xFF) > 0 && (rds.rdsC & 0xFF) < 205) buffer1 = (rds.rdsC & 0xFF) * 10 + 8750; else buffer1 = 0;

                if (((rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) && (buffer0 == doubletestfreq || buffer1 == doubletestfreq)) doublecounter++;
                if (doublecounter > (af_number / 2)) afmethodB = true;

                if (afmethodBprobe && af_counterbcheck > af_counterb) afmethodBprobe = false;

                if (afmethodBprobe) {
                  if (buffer1 == currentfreq && buffer0 > buffer1) {
                    for (int x = 0; x < af_counter; x++) {
                      if (af[x].frequency == buffer0 && !af[x].regional) {
                        af[x].regional = true;
                        af_updatecounter++;
                        break;
                      }
                    }
                  } else if (buffer1 == currentfreq && buffer0 < buffer1) {
                    for (int x = 0; x < af_counter; x++) {
                      if (af[x].frequency == buffer0 && !af[x].same) {
                        af[x].same = true;
                        af_updatecounter++;
                        break;
                      }
                    }
                  }

                  if (buffer0 == currentfreq && buffer0 > buffer1) {
                    for (int x = 0; x < af_counter; x++) {
                      if (af[x].frequency == buffer1 && !af[x].regional) {
                        af[x].regional = true;
                        af_updatecounter++;
                        break;
                      }
                    }
                  } else if (buffer0 == currentfreq && buffer0 < buffer1) {
                    for (int x = 0; x < af_counter; x++) {
                      if (af[x].frequency == buffer1 && !af[x].same) {
                        af[x].same = true;
                        af_updatecounter++;
                        break;
                      }
                    }
                  }
                }

                if (buffer0 != currentfreq && buffer1 != currentfreq && afmethodB && afmethodBprobe) {
                  afmethodBprobe = false;
                  for (int x = 0; x < af_counter; x++) {
                    if (af[x].frequency == buffer0 || af[x].frequency == buffer1) {
                      if (af[x].same) {
                        af[x].same = false;
                        af_updatecounter++;
                      }
                      if (af[x].regional) {
                        af[x].regional = false;
                        af_updatecounter++;
                      }
                    }
                    break;
                  }
                }

                bool isValuePresent = false;
                for (int i = 0; i < 50; i++) {
                  if ((rds.sortaf && (buffer0 == currentfreq)) || buffer0 == 0 || af[i].frequency == buffer0) {
                    isValuePresent = true;
                    break;
                  }
                }

                if (!isValuePresent) {
                  af[af_counter].frequency = buffer0;
                  if (af_counter < 50) af_counter++;
                  af_updatecounter++;
                }

                isValuePresent = false;
                for (int i = 0; i < 50; i++) {
                  if ((rds.sortaf && (buffer1 == currentfreq)) || buffer1 == 0 || af[i].frequency == buffer1) {
                    isValuePresent = true;
                    break;
                  }
                }

                if (!isValuePresent) {
                  af[af_counter].frequency = buffer1;
                  if (af_counter < 50) af_counter++;
                  af_updatecounter++;
                }

                if (rds.sortaf) {
                  for (int i = 0; i < 50; i++) {
                    for (int j = 0; j < 50 - i; j++) {
                      if (af[j].frequency == 0) continue;
                      if (af[j].frequency > af[j + 1].frequency && af[j + 1].frequency != 0) {
                        uint16_t temp = af[j].frequency;
                        bool temp3 = af[j].afvalid;
                        bool temp4 = af[j].checked;
                        bool temp5 = af[j].regional;
                        bool temp6 = af[j].same;
                        af[j].frequency = af[j + 1].frequency;
                        af[j].afvalid = af[j + 1].afvalid;
                        af[j].checked = af[j + 1].checked;
                        af[j].regional = af[j + 1].regional;
                        af[j].same = af[j + 1].same;
                        af[j + 1].frequency = temp;
                        af[j + 1].afvalid = temp3;
                        af[j + 1].checked = temp4;
                        af[j + 1].regional = temp5;
                        af[j + 1].same = temp6;
                      }
                    }
                  }
                }
              }
            }
            rdsCold = rds.rdsC;
          }
      } break;

      case RDS_GROUP_1A: {
          if (!rdsCerrorThreshold && ((rds.rdsC >> 12) & 0x07) == 0) {
            rds.ECC = (rds.rdsC & 0xff);
            rds.hasECC = true;

            switch (rds.picode[0]) {
                case '1': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[129];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[136];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[152];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[104];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[73];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[83];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[0];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[1];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[2];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[59];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[3];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[179];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[197];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[124];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[200];
                    break;
                  }
                case '2': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[130];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[141];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[155];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[106];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[126];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[95];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[4];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[5];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[6];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[7];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[8];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[180];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[187];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[77];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[218];
                    break;
                  }
                case '3': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[224];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[156];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[132];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[112];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[119];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[72];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[92];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[9];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[10];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[11];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[12];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[16];
                    else if (rds.ECC == 0xE5) rds.ECCtext = ECCtext[63];
                    else if (rds.ECC == 0xE6) rds.ECCtext = ECCtext[228];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[181];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[227];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[189];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[219];
                    break;
                  }
                case '4': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[148];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[157];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[127];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[69];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[91];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[103];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[13];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[14];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[15];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[182];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[210];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[213];
                    break;
                  }
                case '5': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[133];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[159];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[68];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[79];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[88];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[78];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[17];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[18];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[19];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[65];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[183];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[191];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[193];
                    break;
                  }
                case '6': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[134];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[163];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[171];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[96];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[105];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[123];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[125];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[20];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[21];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[22];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[24];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[184];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[76];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[201];
                    break;
                  }
                case '7': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[139];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[161];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[164];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[113];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[81];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[84];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[25];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[26];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[27];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[62];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[185];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[207];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[221];
                    break;
                  }
                case '8': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[142];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[170];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[116];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[117];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[74];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[82];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[28];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[29];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[30];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[31];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[32];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[186];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[195];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[212];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[190];
                    break;
                  }
                case '9': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[143];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[162];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[174];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[120];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[102];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[107];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[33];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[34];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[35];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[36];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[37];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[80];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[209];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[196];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[211];
                    else if (rds.ECC == 0xF4) rds.ECCtext = ECCtext[190];
                    break;
                  }
                case 'A': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[131];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[144];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[166];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[85];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[97];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[121];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[70];
                    else if (rds.ECC == 0xD4) rds.ECCtext = ECCtext[86];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[38];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[39];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[40];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[41];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[60];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[178];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[215];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[214];
                    break;
                  }
                case 'B': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA1) rds.ECCtext = ECCtext[138];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[137];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[145];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[167];
                    else if (rds.ECC == 0xA5) rds.ECCtext = ECCtext[158];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[101];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[100];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[109];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[42];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[43];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[44];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[61];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[66];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[206];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[188];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[203];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[93];
                    else if (rds.ECC == 0xF4) rds.ECCtext = ECCtext[204];
                    break;
                  }
                case 'C': {
                    if (rds.ECC == 0xA1) rds.ECCtext = ECCtext[138];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[223];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[140];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[146];
                    else if (rds.ECC == 0xA5) rds.ECCtext = ECCtext[169];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[110];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[108];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[111];
                    else if (rds.ECC == 0xD3) rds.ECCtext = ECCtext[87];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[45];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[46];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[47];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[48];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[118];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[190];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[216];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[194];
                    break;
                  }
                case 'D': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA1) rds.ECCtext = ECCtext[138];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[225];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[150];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[154];
                    else if (rds.ECC == 0xA5) rds.ECCtext = ECCtext[158];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[90];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[89];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[0];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[49];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[23];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[122];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[198];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[217];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[128];
                    break;
                  }
                case 'E': {
                    if (rds.ECC == 0xA0) rds.ECCtext = ECCtext[226];
                    else if (rds.ECC == 0xA1) rds.ECCtext = ECCtext[138];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[151];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[172];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[175];
                    else if (rds.ECC == 0xA5) rds.ECCtext = ECCtext[158];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[99];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[115];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[94];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[50];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[51];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[52];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[53];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[64];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[98];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[199];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[208];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[205];
                    break;
                  }
                case 'F': {
                    if (rds.ECC == 0xA1) rds.ECCtext = ECCtext[149];
                    else if (rds.ECC == 0xA2) rds.ECCtext = ECCtext[222];
                    else if (rds.ECC == 0xA3) rds.ECCtext = ECCtext[153];
                    else if (rds.ECC == 0xA4) rds.ECCtext = ECCtext[176];
                    else if (rds.ECC == 0xA5) rds.ECCtext = ECCtext[158];
                    else if (rds.ECC == 0xA6) rds.ECCtext = ECCtext[168];
                    else if (rds.ECC == 0xD0) rds.ECCtext = ECCtext[67];
                    else if (rds.ECC == 0xD1) rds.ECCtext = ECCtext[75];
                    else if (rds.ECC == 0xD2) rds.ECCtext = ECCtext[114];
                    else if (rds.ECC == 0xE0) rds.ECCtext = ECCtext[54];
                    else if (rds.ECC == 0xE1) rds.ECCtext = ECCtext[55];
                    else if (rds.ECC == 0xE2) rds.ECCtext = ECCtext[56];
                    else if (rds.ECC == 0xE3) rds.ECCtext = ECCtext[57];
                    else if (rds.ECC == 0xE4) rds.ECCtext = ECCtext[58];
                    else if (rds.ECC == 0xF0) rds.ECCtext = ECCtext[202];
                    else if (rds.ECC == 0xF1) rds.ECCtext = ECCtext[192];
                    else if (rds.ECC == 0xF2) rds.ECCtext = ECCtext[220];
                    else if (rds.ECC == 0xF3) rds.ECCtext = ECCtext[71];
                    break;
                  }
                default: {
                  rds.ECCtext = "";
                  break;
                }
              }
          }
      } break;

      case RDS_GROUP_2A: {
          if (showrdserrors == 3 || !(rdsCerrorThreshold && rdsDerrorThreshold)) {
            rds.hasRT = true;
            rds.rtAB = bitRead(rds.rdsB, 4);

            if (initab) {
              rtABold = rds.rtAB;
              initab = false;
            }

              if (rds.rtAB != rtABold) {
                if (rds.rtbuffer) {
                  char rt_buffer_temp[129];
                  strcpy(rt_buffer_temp, rt_buffer);

                  byte endmarkerRT64 = 64;
                  for (byte i = 0; i < endmarkerRT64; i++) {
                      if (rt_buffer_temp[i] == 0x0d) { endmarkerRT64 = i; break; }
                  }
                  rt_buffer_temp[endmarkerRT64] = '\0';

                  wchar_t RTtext[65] = L"";
                  RDScharConverter(rt_buffer_temp, RTtext, sizeof(RTtext) / sizeof(wchar_t), (underscore > 1 ? true : false));
                  rds.stationText = trimTrailingSpaces(convertToUTF8(RTtext));
                }

                memset(rt_buffer, 0x20, 64);
                rt_buffer[64] = '\0';
                memset(segments_received, 99, sizeof(segments_received));
                rtABold = rds.rtAB;
              }

              uint8_t segment_address = (rds.rdsB & 0xf);

              if(segments_received[segment_address] >= (rds.rdsCerror + rds.rdsBerror)) {
                  segments_received[segment_address] = rds.rdsCerror + rds.rdsBerror;

                  uint8_t offset = segment_address * 4;
                  rt_buffer[offset + 0] = rds.rdsC >> 8;
                  rt_buffer[offset + 1] = rds.rdsC & 0xff;
                  rt_buffer[offset + 2] = rds.rdsD >> 8;
                  rt_buffer[offset + 3] = rds.rdsD & 0xff;

                  if (!rds.rtbuffer) {
                      char rt_buffer_temp[129];
                      strcpy(rt_buffer_temp, rt_buffer);

                      byte endmarkerRT64 = 64;
                      for (byte i = 0; i < endmarkerRT64; i++) {
                          if (rt_buffer_temp[i] == 0x0d) { endmarkerRT64 = i; break; }
                      }
                      rt_buffer_temp[endmarkerRT64] = '\0';

                      wchar_t RTtext[65] = L"";
                      RDScharConverter(rt_buffer_temp, RTtext, sizeof(RTtext) / sizeof(wchar_t), (underscore > 1 ? true : false));
                      rds.stationText = trimTrailingSpaces(convertToUTF8(RTtext));
                  }
              }
          }
      } break;

      case RDS_GROUP_2B: {
          if (showrdserrors == 3 || (!rdsBerrorThreshold && !rdsDerrorThreshold)) {
            rds.hasRT = true;
            rds.rtAB32 = (bitRead(rds.rdsB, 4));

            if (rds.rtAB32 != rtAB32old) {
              for (byte i = 0; i < 33; i++) rt_buffer32[i] = 0x20;
              rt_buffer32[32] = '\0';
              rtAB32old = rds.rtAB32;
            }

            uint16_t segment = (rds.rdsB & 0xf) * 2;
            rt_buffer32[segment + 0] = rds.rdsD >> 8;
            rt_buffer32[segment + 1] = rds.rdsD & 0xff;

            byte endmarkerRT32 = 32;
            for (byte i = 0; i < endmarkerRT32; i++) {
              if (rt_buffer[i] == 0x0d) {
                endmarkerRT32 = i;
                break;
              }
            }

            char rt_buffer_temp[129];
            strcpy(rt_buffer_temp, rt_buffer32);

            for (int i = 0; i < 129; i++) {
              if (rt_buffer_temp[i] == '\r') {
                rt_buffer_temp[i] = '\0';
                break;
              }
            }

            wchar_t RTtext[33] = L"";
            RDScharConverter(rt_buffer_temp, RTtext, sizeof(RTtext) / sizeof(wchar_t), (underscore > 1 ? true : false));
            rds.stationText32 = convertToUTF8(RTtext);
            rds.stationText32 = extractUTF8Substring(rds.stationText32, 0, endmarkerRT32, (underscore > 1 ? true : false));
            rds.stationText32 = trimTrailingSpaces(rds.stationText32);
          }
        } break;

      case RDS_GROUP_3A: {
          if (!rdsDerrorThreshold) {
            if (rds.rdsD != 0) rds.hasAID = true;

            bool isValuePresent = false;
            for (int i = 0; i < 10; i++) {
              if (rds.aid[i] == rds.rdsD) {
                isValuePresent = true;
                break;
              }
            }

            if (!isValuePresent) {
              rds.aid[rds.aid_counter] = rds.rdsD;
              rds.aid_counter++;
            }

            if (rds.rdsD == 0xCD46) rds.hasTMC = true;

            if (rds.rdsD == 0x4BD7) {
              rds.hasRTplus = true;
              rtplusblock = rds.rdsB & 0x1E;
            }

            if (rds.rdsD == 0x0093) {
              rds.hasDABAF = true;
              DABAFblock = rds.rdsB & 0x1E;
            }

            if (rds.rdsD == 0x6552) {
              _hasEnhancedRT = true;
              eRTblock = rds.rdsB & 0x1E;
              eRTcoding = rds.rdsC & 1;
            }
          }
        } break;
      case RDS_GROUP_4A: {
          if (!rdsBerrorThreshold && !rdsCerrorThreshold && !rdsDerrorThreshold && rds.ctupdate && (rds.PICTlock == pi || rds.PICTlock == 0)) {
            auto rtc_time = rtc.getEpoch();
            uint32_t mjd = (rds.rdsB & 0x03) << 15 | ((rds.rdsC >> 1) & 0x7FFF);
            uint16_t hour, minute, day = 5, month = 1, year = 2026;
            int32_t timeoffset;

            long J = mjd + 2400001 + 68569;
            long C = 4 * J / 146097;
            J = J - (146097 * C + 3) / 4;
            long Y = 4000 * (J + 1) / 1461001;
            J = J - 1461 * Y / 4 + 31;
            long M = 80 * J / 2447;

            if ((J - 2447 * M / 80) < 32) day = J - 2447 * M / 80;
            J = M / 11;

            if ((M +  2 - (12 * J)) < 13) month = M +  2 - (12 * J);
            if ((100 * (C - 49) + Y + J) > 2022) year = 100 * (C - 49) + Y + J;

            hour = ((rds.rdsD >> 12) & 0xF) | ((rds.rdsC << 4) & 0x10);
            timeoffset = rds.rdsD & 0x1F;
            if (bitRead(rds.rdsD, 5)) timeoffset *= -1;
            timeoffset *= 1800;
            minute = (rds.rdsD & 0xFC0) >> 6;

            if (year < 2026 || hour > 23 || minute > 59 || timeoffset > 55800 || timeoffset < -55800) break;

            struct tm tm;
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            tm.tm_hour = hour;
            tm.tm_min = minute;
            tm.tm_sec = 0;
            tm.tm_isdst = -1;
            time_t rdstime = mktime(&tm);

            if (((rdstime == lastrdstime + 60 && timeoffset == lasttimeoffset) || (lastrdstime == 0 && lasttimeoffset == 0)) || showrdserrors != 0) {
              rds.hasCT = true;
              rds.offset = timeoffset;
              rtcset = true;

              time_t rds_utc_time = rdstime + timeoffset;
              int32_t current_correction = rtc_time - rds_utc_time;
              rds.clock_correction = current_correction;

              if (!NTPupdated) set_time(rds_utc_time - (current_correction / 2));
            } else rds.hasCT = false;
            lastrdstime = rdstime;
            lasttimeoffset = timeoffset;
          }
        } break;
      case RDS_GROUP_10A: {
          if (!rdsCerrorThreshold && !rdsDerrorThreshold) {
            uint8_t segment = bitRead(rds.rdsB, 0);
            if (rds.rdsC != 0 && rds.rdsD != 0) {
              ptyn_buffer[(segment * 4) + 0] = rds.rdsC >> 8;
              ptyn_buffer[(segment * 4) + 1] = rds.rdsC & 0xFF;
              ptyn_buffer[(segment * 4) + 2] = rds.rdsD >> 8;
              ptyn_buffer[(segment * 4) + 3] = rds.rdsD & 0xFF;
              for (byte i = 0; i < 8; i++) PTYNtext[i] = 0;
              RDScharConverter(ptyn_buffer, PTYNtext, sizeof(PTYNtext) / sizeof(wchar_t), false);
              rds.PTYN = extractUTF8Substring(convertToUTF8(PTYNtext), 0, 8, false);
              rds.hasPTYN = true;
            }
          }
        } break;

      case RDS_GROUP_5A:
      case RDS_GROUP_6A:
      case RDS_GROUP_7A:
      case RDS_GROUP_8A:
      case RDS_GROUP_9A:
      case RDS_GROUP_11A:
      case RDS_GROUP_12A:
      case RDS_GROUP_13A: {
          if ((!rdsBerrorThreshold && !rdsCerrorThreshold && !rdsDerrorThreshold) && rtplusblock == rdsgroup && rds.hasRTplus) {
            rds.rdsplusTag1 = ((rds.rdsB & 0x07) << 3) + (rds.rdsC >> 13);
            rds.rdsplusTag2 = ((rds.rdsC & 0x01) << 5) + (rds.rdsD >> 11);
            uint16_t start_marker_1 = (rds.rdsC >> 7) & 0x3F;
            uint16_t length_marker_1 = (rds.rdsC >> 1) & 0x3F;
            uint16_t start_marker_2 = (rds.rdsD >> 5) & 0x3F;
            uint16_t length_marker_2 = (rds.rdsD & 0x1F);
            togglebit = bitRead(rds.rdsB, 4);
            runningbit = bitRead(rds.rdsB, 3);

            switch (rds.rdsplusTag1) {
              case 0: rds.rdsplusTag1 = 168; break;
              case 1 ... 53: rds.rdsplusTag1 += 110; break;
              case 59 ... 63: rds.rdsplusTag1 += 104; break;
              default: rds.rdsplusTag1 = 168; break;
            }

            switch (rds.rdsplusTag2) {
              case 0: rds.rdsplusTag2 = 168; break;
              case 1 ... 53: rds.rdsplusTag2 += 110; break;
              case 59 ... 63: rds.rdsplusTag2 += 104; break;
              default: rds.rdsplusTag2 = 168; break;
            }

            if (togglebit) {
              for (int i = 0; i < 44; i++) {
                RDSplus1[i] = 0x20;
                RDSplus2[i] = 0x20;
              }
              RDSplus1[44] = 0;
              RDSplus2[44] = 0;
            }

            if (rds.rtAB == rtABold) {
              for (int i = 0; i <= length_marker_1; i++)RDSplus1[i] = rt_buffer[i + start_marker_1];
              RDSplus1[length_marker_1 + 1] = 0;

              for (int i = 0; i <= length_marker_2; i++)RDSplus2[i] = rt_buffer[i + start_marker_2];
              RDSplus2[length_marker_2 + 1] = 0;
            }

            wchar_t RTtext1[45] = L"";
            RDScharConverter(RDSplus1, RTtext1, sizeof(RTtext1) / sizeof(wchar_t), false);
            rds.RTContent1 = convertToUTF8(RTtext1);
            rds.RTContent1 = extractUTF8Substring(rds.RTContent1, 0, 44, false);

            wchar_t RTtext2[45] = L"";
            RDScharConverter(RDSplus2, RTtext2, sizeof(RTtext2) / sizeof(wchar_t), false);
            rds.RTContent2 = convertToUTF8(RTtext2);
            rds.RTContent2 = extractUTF8Substring(rds.RTContent2, 0, 44, false);
          }

          if ((!rdsBerrorThreshold && !rdsCerrorThreshold && !rdsDerrorThreshold) && eRTblock == rdsgroup && _hasEnhancedRT) {
            uint8_t offset = (rds.rdsB & 0x1f) * 4;
            eRT_buffer[offset + 0] = rds.rdsC >> 8;
            eRT_buffer[offset + 1] = rds.rdsC & 0xff;
            eRT_buffer[offset + 2] = rds.rdsD >> 8;
            eRT_buffer[offset + 3] = rds.rdsD & 0xff;
            eRT_buffer[127] = '\0';

            byte endmarkereRT = 127;
            bool foundendmarker = false;
            for (byte i = 0; i < endmarkereRT; i++) {
              if (eRT_buffer[i] == 0x0d) {
                foundendmarker = true;
                endmarkereRT = i;
                break;
              }
            }

            if (offset == 0 || foundendmarker) {
              if (eRTcoding) rds.enhancedRTtext = eRT_buffer; else rds.enhancedRTtext = ucs2ToUtf8(eRT_buffer);
              rds.enhancedRTtext = trimTrailingSpaces(rds.enhancedRTtext);
              if (rds.enhancedRTtext.length() > 0) rds.hasEnhancedRT = true;
            }
          }

          if (!rdsBerrorThreshold && rdsgroup == RDS_GROUP_8A && (bitRead(rds.rdsB, 15))) rds.hasTMC = true;

          if ((!rdsBerrorThreshold && !rdsCerrorThreshold && !rdsDerrorThreshold) && DABAFblock == rdsgroup && rds.hasDABAF) {
            rds.dabaffreq = (rds.rdsC * 16);

            for (size_t i = 0; i < sizeof(DABfrequencyTable) / sizeof(DABfrequencyTable[0]); ++i) {
              if (DABfrequencyTable[i].frequency == rds.dabaffreq) strcpy(rds.dabafchannel, DABfrequencyTable[i].label);
            }

            rds.dabafeid[0] = (rds.rdsD >> 12) & 0xF;
            rds.dabafeid[1] = (rds.rdsD >> 8) & 0xF;
            rds.dabafeid[2] = (rds.rdsD >> 4) & 0xF;
            rds.dabafeid[3] = rds.rdsD & 0xF;
            for (int i = 0; i < 4; i++) {
              if (rds.dabafeid[i] < 10) rds.dabafeid[i] += '0';
              else rds.dabafeid[i] += 'A' - 10;
            }
            rds.dabafeid[4] = 0;
          }
        }
        break;

      case RDS_GROUP_14A: {
          if (!rdsAerrorThreshold && !rdsBerrorThreshold && !rdsCerrorThreshold && !rdsDerrorThreshold) {
            rds.hasEON = true;

            bool isValuePresent = false;
            int eonIndex = -1;
            int i = 0;
            for (; i < 20; i++) {
              if (eon[i].pi == rds.rdsD || rds.rdsA == rds.rdsD) {
                isValuePresent = true;
                eonIndex = i;
                break;
              }
            }

            if (!isValuePresent) {
              eon[eon_counter].picode[0] = (rds.rdsD >> 12) & 0xF;
              eon[eon_counter].picode[1] = (rds.rdsD >> 8) & 0xF;
              eon[eon_counter].picode[2] = (rds.rdsD >> 4) & 0xF;
              eon[eon_counter].picode[3] = rds.rdsD & 0xF;
              for (int j = 0; j < 4; j++) {
                if (eon[eon_counter].picode[j] < 10) eon[eon_counter].picode[j] += '0';
                else eon[eon_counter].picode[j] += 'A' - 10;
              }

              eon[eon_counter].pi = rds.rdsD;
              if (eon_counter < 20) eon_counter++;
              eonIndex = eon_counter - 1;
            } else eonIndex = i;

            uint8_t offset = rds.rdsB & 0x0F;

            if (offset < 4 && eon[eonIndex].pi == rds.rdsD) {
              eon_buffer[eonIndex][(offset * 2) + 0] = rds.rdsC >> 8;
              eon_buffer[eonIndex][(offset * 2) + 1] = rds.rdsC & 0xFF;
            }

            if (eon[eonIndex].pi == rds.rdsD) {
              RDScharConverter(eon_buffer[eonIndex], EONPStext[eonIndex], sizeof(EONPStext[eonIndex]) / sizeof(wchar_t), false);
              eon[eonIndex].ps = extractUTF8Substring(convertToUTF8(EONPStext[eonIndex]), 0, 8, false);
            }

            if (offset == 13 && eon[eonIndex].pi == rds.rdsD) {
              eon[eonIndex].taset = true;
              eon[eonIndex].ta = bitRead(rds.rdsC, 0);
              eon[eonIndex].pty = (rds.rdsC >> 11) & 0xF;
              eon[eonIndex].ptyset = true;
            }

            if (bitRead(rds.rdsB, 4) && eon[eonIndex].pi == rds.rdsD) eon[eonIndex].tp = true;

            if (offset > 4 && offset < 9 && eon[eonIndex].pi == rds.rdsD) {
              if (((rds.rdsC >> 8) * 10 + 8750) == currentfreq && (rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) {
                if (eon[eonIndex].mappedfreq == 0) eon[eonIndex].mappedfreq = ((rds.rdsC & 0xFF) * 10 + 8750);
                else if (eon[eonIndex].mappedfreq2 == 0 && eon[eonIndex].mappedfreq != ((rds.rdsC & 0xFF) * 10 + 8750) && (rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) {
                  eon[eonIndex].mappedfreq2 = ((rds.rdsC & 0xFF) * 10 + 8750);
                } else if (eon[eonIndex].mappedfreq3 == 0 && eon[eonIndex].mappedfreq2 != ((rds.rdsC & 0xFF) * 10 + 8750) && eon[eonIndex].mappedfreq != ((rds.rdsC & 0xFF) * 10 + 8750) && (rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) {
                  eon[eonIndex].mappedfreq3 = ((rds.rdsC & 0xFF) * 10 + 8750);
                }
              }
            }

            for (int i = 0; i < 20; i++) {
              for (int j = 0; j < 20 - i - 1; j++) {
                if (eon[j].pi == 0) continue;

                if (eon[j].pi > eon[j + 1].pi && eon[j + 1].pi != 0) {
                  std::swap(eon[j].pi, eon[j + 1].pi);
                  std::swap(eon[j].mappedfreq, eon[j + 1].mappedfreq);
                  std::swap(eon[j].mappedfreq2, eon[j + 1].mappedfreq2);
                  std::swap(eon[j].mappedfreq3, eon[j + 1].mappedfreq3);
                  char temp6[6];
                  strncpy(temp6, eon[j].picode, sizeof(temp6) - 1);
                  temp6[sizeof(temp6) - 1] = '\0';
                  strncpy(eon[j].picode, eon[j + 1].picode, sizeof(eon[j].picode) - 1);
                  eon[j].picode[sizeof(eon[j].picode) - 1] = '\0';
                  strncpy(eon[j + 1].picode, temp6, sizeof(eon[j + 1].picode) - 1);
                  eon[j + 1].picode[sizeof(eon[j + 1].picode) - 1] = '\0';
                  std::swap(eon[j].pty, eon[j + 1].pty);
                  std::swap(eon[j].ps, eon[j + 1].ps);
                  std::swap(eon[j].ta, eon[j + 1].ta);
                  std::swap(eon[j].tp, eon[j + 1].tp);
                  std::swap(eon[j].taset, eon[j + 1].taset);
                  std::swap(eon[j].ptyset, eon[j + 1].ptyset);
                  std::swap(eon_buffer[j], eon_buffer[j + 1]);
                  std::swap(EONPStext[j], EONPStext[j + 1]);
                }
              }
            }
          }
        }
        break;

      case RDS_GROUP_15A: {
          if (showrdserrors == 3 || (!rdsBerrorThreshold && !rdsCerrorThreshold && !rdsDerrorThreshold)) {
            if (pslong_process && rds.stationNameLong.length() > 0) rds.hasLongPS = true;
            uint8_t offset = (rds.rdsB & 7) * 4;

            pslong_buffer2[offset + 0] = pslong_buffer[offset + 0];
            pslong_buffer2[offset + 1] = pslong_buffer[offset + 1];
            pslong_buffer2[offset + 2] = pslong_buffer[offset + 2];
            pslong_buffer2[offset + 3] = pslong_buffer[offset + 3];
            pslong_buffer2[32] = '\0';

            pslong_buffer[offset + 0] = rds.rdsC >> 8;
            pslong_buffer[offset + 1] = rds.rdsC & 0xff;
            pslong_buffer[offset + 2] = rds.rdsD >> 8;
            pslong_buffer[offset + 3] = rds.rdsD & 0xff;
            pslong_buffer[32] = '\0';

            byte endmarkerLPS = 32;
            bool foundendmarker = false;
            for (byte i = 0; i < endmarkerLPS; i++) {
              if (pslong_buffer[i] == 0x0d) {
                foundendmarker = true;
                endmarkerLPS = i;
                break;
              }
            }

            if ((offset == 0 || foundendmarker) && (pslong_process || !rds.fastps)) {
              if (strcmp(pslong_buffer, pslong_buffer2) == 0) {
                pslong_process = true;
                PSLongtext = pslong_buffer;
                rds.stationNameLong = extractUTF8Substring(pslong_buffer, 0, endmarkerLPS, true);
                rds.stationNameLong = trimTrailingSpaces(rds.stationNameLong);
              }
            }

            if (!pslong_process && rds.fastps) {
              if (offset == 0) packet0long = true;
              if (offset == 4) packet1long = true;
              if (offset == 8) packet2long = true;
              if (offset == 16) packet3long = true;
              PSLongtext = pslong_buffer;
              rds.stationNameLong = extractUTF8Substring(pslong_buffer, 0, endmarkerLPS, true);
              rds.stationNameLong = trimTrailingSpaces(rds.stationNameLong);
              if ((packet0long && packet1long && packet2long && packet3long) || foundendmarker) pslong_process = true;
            }
          }
        }
        break;
    }
  end:
    previous_rdsA = rds.rdsA;
    previous_rdsB = rds.rdsB;
    previous_rdsC = rds.rdsC;
    previous_rdsD = rds.rdsD;
  }
}

void TEF6686::clearRDS(bool fullsearchrds) {
  devTEF_Set_Cmd(TEF_FM, Cmd_Set_RDS, 6, fullsearchrds ? 3 : 1, 1, 0);
  rds.piBuffer.clear();
  rds.stationName = rds.stationText = rds.stationNameLong = "";
  rds.PTYN = rds.stationText32 = rds.RTContent1 = rds.RTContent2 = "";;
  rds.ECCtext = rds.stationIDtext = "";
  rds.stationStatetext = rds.enhancedRTtext = PSLongtext = "";

  for (uint8_t i = 0; i < 8; i++) {
    ps_buffer[i] = ptyn_buffer[i] = 0x20;
    PStext[i] = PTYNtext[i] = 0;
  }
  ps_buffer[8] = ptyn_buffer[8] = PStext[8] = PTYNtext[8] = 0;

  for (uint8_t i = 0; i < 64; i++) rt_buffer[i] = 0x20;
  rt_buffer[64] = 0;

  for (uint8_t i = 0; i < 128; i++) eRT_buffer[i] = 0x20;
  eRT_buffer[128] = 0;

  for (uint8_t i = 0; i < 32; i++) rt_buffer32[i] = pslong_buffer[i] = pslong_buffer2[i] = 0x20;
  rt_buffer32[32] = 0;
  pslong_buffer[32] = 0;

  for (uint8_t i = 0; i < 6; i++) rds.picode[i] = 0x20;
  rds.picode[6] = 0;

  for (uint8_t i = 0; i < 4; i++) rds.dabafeid[i] = 0x20;
  rds.dabafeid[4] = 0;

  for (uint8_t i = 0; i < 3; i++) rds.dabafchannel[i] = 0x20;
  rds.dabafchannel[3] = 0;

  for (uint8_t i = 0; i < 8; i++) rds.stationID[i] = 0x20;
  rds.stationID[8] = 0;

  for (uint8_t i = 0; i < 2; i++) rds.stationState[i] = 0x20;
  rds.stationState[2] = 0;

  for (uint8_t i = 0; i < 50; i++) {
    af[i].frequency = 0;
    af[i].score = -32767;
    af[i].afvalid = true;
    af[i].checked = af[i].regional = af[i].same = false;
  }

  for (uint8_t i = 0; i < 20; i++) {
    eon[i].pty = 254;
    eon[i].ps = "";
    eon[i].pi = eon[i].mappedfreq = eon[i].mappedfreq2 = eon[i].mappedfreq3 = 0;
    eon[i].ta = eon[i].tp = eon[i].taset = eon[i].ptyset = false;
    for (int y = 0; y < 5; y++) eon[i].picode[y] = 0;
    for (int j = 0; j < 9; j++) EONPStext[i][j] = eon_buffer[i][j] = 0;
  }

  for (uint8_t i = 0; i < 45; i++) RDSplus1[i] = RDSplus2[i] = 0;

  for (uint8_t i = 0; i < 33; i++) rds.blockcounter[i] = 0;

  for (uint8_t i = 0; i < 10; i++) rds.aid[i] = 0;

  rdsgroup = 254;
  rds.ECC = rds.PTY = 0;
  rds.dabaffreq = rds.correctPI = correctPIold = processed_rdsblocks = piold = 0;
  rds.hasECC = rds.hasRT = rds.hasRDS = false;
  rds.hasAF = false;
  rds.hasCT = rds.hasRTplus = rds.hasTMC = rds.hasEON = false;
  rds.hasAID = rds.hasPTYN = rds.hasLongPS = false;
  rds.TA = rds.TP = rds.hasCompressed = rds.hasArtificialhead = rds.hasStereo = false;
  rds.hasDABAF = rds.hasEnhancedRT = false;
  rt_process = ps_process = pslong_process = false;
  rds.rdsreset = true;
  rds.hasDynamicPTY = false;
  af_counter = af_updatecounter = eon_counter = 0;
  afreset = rds.rdsAerror = rds.rdsBerror = rds.rdsCerror = rds.rdsDerror = true;
  initab = true;
  rds.rdsplusTag1 = 169;
  rds.rdsplusTag2 = 169;
  afinit = afmethodB = errorfreepi = false;
  packet0 = packet1 = packet2 = packet3 = false;
  packet0long = packet1long = packet2long = packet3long = false;
  rds.aid_counter = af_number = 0;
  afmethodBprobe = afmethodBtrigger = _hasEnhancedRT = false;
  rds.ps12error = rds.ps34error = rds.ps56error = rds.ps78error = true;
  memset(segments_received, 99, sizeof(segments_received));
}

void TEF6686::tone(uint16_t time, int16_t amplitude, uint16_t frequency) {
  devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Mute, 2, 0);
  devTEF_Radio_Set_Wavegen(1, amplitude, frequency);
  delay(time);
  devTEF_Radio_Set_Wavegen(0, 0, 0);
}

void TEF6686::RDScharConverter(const char* input, wchar_t* output, size_t size, bool underscore) {
  for (size_t i = 0; i < size - 1; i++) {
    char currentChar = input[i];
    switch (currentChar) {
      case 0x0A: if (underscore) output[i] = L'_'; else output[i] = L' '; break;
      case 0x20: if (underscore) output[i] = L'_'; else output[i] = L' '; break;
      case 0x21 ... 0x5D: output[i] = static_cast<wchar_t>(currentChar); break;
      case 0x5E: output[i] = L'―'; break;
      case 0x5F: output[i] = L'_'; break;
      case 0x60: output[i] = L'`'; break;
      case 0x61 ... 0x7d: output[i] = static_cast<wchar_t>(currentChar); break;
      case 0x7E: output[i] = L'¯'; break;
      case 0x7F: output[i] = L' '; break;
      case 0x80: output[i] = L'á'; break;
      case 0x81: output[i] = L'à'; break;
      case 0x82: output[i] = L'é'; break;
      case 0x83: output[i] = L'è'; break;
      case 0x84: output[i] = L'í'; break;
      case 0x85: output[i] = L'ì'; break;
      case 0x86: output[i] = L'ó'; break;
      case 0x87: output[i] = L'ò'; break;
      case 0x88: output[i] = L'ú'; break;
      case 0x89: output[i] = L'ù'; break;
      case 0x8A: output[i] = L'Ñ'; break;
      case 0x8B: output[i] = L'Ç'; break;
      case 0x8C: output[i] = L'Ş'; break;
      case 0x8D: output[i] = L'β'; break;
      case 0x8E: output[i] = L'¡'; break;
      case 0x8F: output[i] = L'Ĳ'; break;
      case 0x90: output[i] = L'â'; break;
      case 0x91: output[i] = L'ä'; break;
      case 0x92: output[i] = L'ê'; break;
      case 0x93: output[i] = L'ë'; break;
      case 0x94: output[i] = L'î'; break;
      case 0x95: output[i] = L'ï'; break;
      case 0x96: output[i] = L'ô'; break;
      case 0x97: output[i] = L'ö'; break;
      case 0x98: output[i] = L'û'; break;
      case 0x99: output[i] = L'ü'; break;
      case 0x9A: output[i] = L'ñ'; break;
      case 0x9B: output[i] = L'ç'; break;
      case 0x9C: output[i] = L'ş'; break;
      case 0x9D: output[i] = L'ǧ'; break;
      case 0x9E: output[i] = L'\x131'; break;
      case 0x9F: output[i] = L'ĳ'; break;
      case 0xA0: output[i] = L'ª'; break;
      case 0xA1: output[i] = L'\x3b1'; break;
      case 0xA2: output[i] = L'©'; break;
      case 0xA3: output[i] = L'‰'; break;
      case 0xA4: output[i] = L'Ǧ'; break;
      case 0xA5: output[i] = L'ě'; break;
      case 0xA6: output[i] = L'ň'; break;
      case 0xA7: output[i] = L'ő'; break;
      case 0xA8: output[i] = L'π'; break;
      case 0xA9: output[i] = L'€'; break;
      case 0xAA: output[i] = L'£'; break;
      case 0xAB: output[i] = L'$'; break;
      case 0xAC: output[i] = L'←'; break;
      case 0xAD: output[i] = L'↑'; break;
      case 0xAE: output[i] = L'→'; break;
      case 0xAF: output[i] = L'↓'; break;
      case 0xB0: output[i] = L'º'; break;
      case 0xB1: output[i] = L'¹'; break;
      case 0xB2: output[i] = L'²'; break;
      case 0xB3: output[i] = L'³'; break;
      case 0xB4: output[i] = L'±'; break;
      case 0xB5: output[i] = L'İ'; break;
      case 0xB6: output[i] = L'ń'; break;
      case 0xB7: output[i] = L'ű'; break;
      case 0xB8: output[i] = L'µ'; break;
      case 0xB9: output[i] = L'¿'; break;
      case 0xBA: output[i] = L'÷'; break;
      case 0xBB: output[i] = L'°'; break;
      case 0xBC: output[i] = L'¼'; break;
      case 0xBD: output[i] = L'½'; break;
      case 0xBE: output[i] = L'¾'; break;
      case 0xBF: output[i] = L'§'; break;
      case 0xC0: output[i] = L'Á'; break;
      case 0xC1: output[i] = L'À'; break;
      case 0xC2: output[i] = L'É'; break;
      case 0xC3: output[i] = L'È'; break;
      case 0xC4: output[i] = L'Í'; break;
      case 0xC5: output[i] = L'Ì'; break;
      case 0xC6: output[i] = L'Ó'; break;
      case 0xC7: output[i] = L'Ò'; break;
      case 0xC8: output[i] = L'Ú'; break;
      case 0xC9: output[i] = L'Ù'; break;
      case 0xCA: output[i] = L'Ř'; break;
      case 0xCB: output[i] = L'Č'; break;
      case 0xCC: output[i] = L'Š'; break;
      case 0xCD: output[i] = L'Ž'; break;
      case 0xCE: output[i] = L'Ð'; break;
      case 0xCF: output[i] = L'Ŀ'; break;
      case 0xD0: output[i] = L'Â'; break;
      case 0xD1: output[i] = L'Ä'; break;
      case 0xD2: output[i] = L'Ê'; break;
      case 0xD3: output[i] = L'Ë'; break;
      case 0xD4: output[i] = L'Î'; break;
      case 0xD5: output[i] = L'Ï'; break;
      case 0xD6: output[i] = L'Ô'; break;
      case 0xD7: output[i] = L'Ö'; break;
      case 0xD8: output[i] = L'Û'; break;
      case 0xD9: output[i] = L'Ü'; break;
      case 0xDA: output[i] = L'ř'; break;
      case 0xDB: output[i] = L'č'; break;
      case 0xDC: output[i] = L'š'; break;
      case 0xDD: output[i] = L'ž'; break;
      case 0xDE: output[i] = L'đ'; break;
      case 0xDF: output[i] = L'ŀ'; break;
      case 0xE0: output[i] = L'Ã'; break;
      case 0xE1: output[i] = L'Å'; break;
      case 0xE2: output[i] = L'Æ'; break;
      case 0xE3: output[i] = L'Œ'; break;
      case 0xE4: output[i] = L'ŷ'; break;
      case 0xE5: output[i] = L'Ý'; break;
      case 0xE6: output[i] = L'Õ'; break;
      case 0xE7: output[i] = L'Ø'; break;
      case 0xE8: output[i] = L'Þ'; break;
      case 0xE9: output[i] = L'Ŋ'; break;
      case 0xEA: output[i] = L'Ŕ'; break;
      case 0xEB: output[i] = L'Ć'; break;
      case 0xEC: output[i] = L'Ś'; break;
      case 0xED: output[i] = L'Ź'; break;
      case 0xEE: output[i] = L'Ŧ'; break;
      case 0xEF: output[i] = L'ð'; break;
      case 0xF0: output[i] = L'ã'; break;
      case 0xF1: output[i] = L'å'; break;
      case 0xF2: output[i] = L'æ'; break;
      case 0xF3: output[i] = L'œ'; break;
      case 0xF4: output[i] = L'ŵ'; break;
      case 0xF5: output[i] = L'ý'; break;
      case 0xF6: output[i] = L'õ'; break;
      case 0xF7: output[i] = L'ø'; break;
      case 0xF8: output[i] = L'þ'; break;
      case 0xF9: output[i] = L'ŋ'; break;
      case 0xFA: output[i] = L'ŕ'; break;
      case 0xFB: output[i] = L'ć'; break;
      case 0xFC: output[i] = L'ś'; break;
      case 0xFD: output[i] = L'ź'; break;
      case 0xFE: output[i] = L'ŧ'; break;
      case 0xFF: output[i] = L' '; break;
    }
  }
  output[size - 1] = 0;
}

static const char* fixedCalls[] = {"CBLA", "CBFM", "CBOT"};
bool TEF6686::isFixedCallsign(uint16_t stationID, char* stationIDStr) {
  for (int i = 0; i < sizeof(fixedPI) / sizeof(fixedPI[0]); i++) {
    if (stationID == fixedPI[i]) {
      strcpy(stationIDStr, fixedCalls[i]);
      return true;
    }
  }
  return false;
}