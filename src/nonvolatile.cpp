#include "nonvolatile.h"

void StoreFrequency() {
  switch (band) {
    case BAND_LW: freqold = frequency_LW; frequency_AM = frequency_LW; break;
    case BAND_MW: freqold = frequency_MW; frequency_AM = frequency_MW; break;
    case BAND_SW: freqold = frequency_SW; frequency_AM = frequency_SW; break;
  }
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_FM, frequency);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_OIRT, frequency_OIRT);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_AM, frequency_AM);
  EEPROM.writeByte(EE_BYTE_BAND, band);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_LW, frequency_LW);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_MW, frequency_MW);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_SW, frequency_SW);
  EEPROM.commit();
}

void ClearMemoryRange(uint8_t start, uint8_t stop) {
  for (uint8_t pos = start; pos <= stop; pos++) {
    EEPROM.writeByte(pos + EE_PRESETS_BAND_START, BAND_FM);
    EEPROM.writeUInt((pos * 4) + EE_PRESETS_FREQUENCY_START, EE_PRESETS_FREQUENCY);
    EEPROM.writeByte(pos + EE_PRESET_BW_START, 0);
    EEPROM.writeByte(pos + EE_PRESET_MS_START, 1);

    for (int y = 0; y < 9; y++) {
      EEPROM.writeByte((pos * 9) + y + EE_PRESETS_RDSPS_START, '\0');
      presets[pos].RDSPS[y] = '\0';
    }

    for (int y = 0; y < 5; y++) {
      EEPROM.writeByte((pos * 5) + y + EE_PRESETS_RDSPI_START, '\0');
      presets[pos].RDSPI[y] = '\0';
    }

    EEPROM.commit();
    presets[pos].band = BAND_FM;
    presets[pos].frequency = EE_PRESETS_FREQUENCY;
  }
}

void saveData() {
  EEPROM.writeByte(EE_BYTE_VOLSET, VolSet);
  EEPROM.writeUInt(EE_UINT16_CONVERTERSET, ConverterSet);
  EEPROM.writeUInt(EE_UINT16_FMLOWEDGESET, LowEdgeSet);
  EEPROM.writeUInt(EE_UINT16_FMHIGHEDGESET, HighEdgeSet);
  EEPROM.writeByte(EE_BYTE_CONTRASTSET, ContrastSet);
  EEPROM.writeByte(EE_BYTE_STEREOLEVEL, StereoLevel);
  EEPROM.writeByte(EE_BYTE_BANDFM, bandFM);
  EEPROM.writeByte(EE_BYTE_BANDAM, bandAM);
  EEPROM.writeByte(EE_BYTE_HIGHCUTLEVEL, HighCutLevel);
  EEPROM.writeByte(EE_BYTE_HIGHCUTOFFSET, HighCutOffset);
  EEPROM.writeByte(EE_BYTE_LEVELOFFSET, LevelOffset);
  EEPROM.writeByte(EE_BYTE_RTBUFFER, radio.rds.rtbuffer);
  EEPROM.writeByte(EE_BYTE_EDGEBEEP, edgebeep);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEAM, softmuteam);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEFM, softmutefm);
  EEPROM.writeByte(EE_BYTE_LANGUAGE, language);
  EEPROM.writeByte(EE_BYTE_SHOWRDSERRORS, showrdserrors);
  EEPROM.writeByte(EE_BYTE_LOWLEVELSET, LowLevelSet);
  EEPROM.writeByte(EE_BYTE_REGION, radio.rds.region);
  EEPROM.writeByte(EE_BYTE_RDS_UNDERSCORE, radio.underscore);
  EEPROM.writeByte(EE_BYTE_USBMODE, USBmode);
  EEPROM.writeByte(EE_BYTE_WIFI, wifi);
  EEPROM.writeByte(EE_BYTE_SUBNETCLIENT, subnetclient);
  EEPROM.writeByte(EE_BYTE_SHOWSWMIBAND, showSWMIBand);
  EEPROM.writeByte(EE_BYTE_RDS_FILTER, radio.rds.filter);
  EEPROM.writeByte(EE_BYTE_RDS_PIERRORS, radio.rds.pierrors);
  EEPROM.writeByte(EE_BYTE_USESQUELCH, usesquelch);
  EEPROM.writeByte(EE_BYTE_SHOWMODULATION, showmodulation);
  EEPROM.writeByte(EE_BYTE_AM_NB, amnb);
  EEPROM.writeByte(EE_BYTE_FM_NB, fmnb);
  EEPROM.writeByte(EE_BYTE_AUDIOMODE, audiomode);
  EEPROM.writeByte(EE_BYTE_TOUCH_ROTATING, touchrotating);
  EEPROM.writeUInt(EE_UINT16_LOWEDGEOIRTSET, LowEdgeOIRTSet);
  EEPROM.writeUInt(EE_UINT16_HIGHEDGEOIRTSET, HighEdgeOIRTSet);
  EEPROM.writeByte(EE_BYTE_HARDWARE_MODEL, hardwaremodel);
  EEPROM.writeByte(EE_BYTE_POWEROPTIONS, poweroptions);
  EEPROM.writeByte(EE_BYTE_CURRENTTHEME, CurrentTheme);
  EEPROM.writeByte(EE_BYTE_FMDEFAULTSTEPSIZE, fmdefaultstepsize);
  EEPROM.writeByte(EE_BYTE_SCREENSAVERSET, screensaverset);
  EEPROM.writeInt(EE_INT16_AMLEVELOFFSET, AMLevelOffset);
  EEPROM.writeByte(EE_BYTE_UNIT, unit);
  EEPROM.writeByte(EE_BYTE_AF, af);
  EEPROM.writeByte(EE_BYTE_STEREO, StereoToggle);
  EEPROM.writeByte(EE_BYTE_BATTERY_OPTIONS, batteryoptions);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT, amcodect);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT_COUNT, amcodectcount);
  EEPROM.writeByte(EE_BYTE_AM_RF_GAIN, amgain);
  EEPROM.writeByte(EE_BYTE_SORTAF, radio.rds.sortaf);
  EEPROM.writeByte(EE_BYTE_STATIONLISTID, stationlistid);
  EEPROM.writeByte(EE_BYTE_FM_DEEMPHASIS, fmdeemphasis);
  EEPROM.writeByte(EE_BYTE_FASTPS, radio.rds.fastps);
  EEPROM.writeByte(EE_BYTE_TOT, tot);
  EEPROM.writeByte(EE_BYTE_MWREGION, mwstepsize);
  EEPROM.writeByte(EE_BYTE_SPISPEED, spispeed);
  EEPROM.writeByte(EE_BYTE_AMSCANSENS, amscansens);
  EEPROM.writeByte(EE_BYTE_FMSCANSENS, fmscansens);
  EEPROM.writeByte(EE_BYTE_FREQFONT, freqfont);
  EEPROM.writeByte(EE_BYTE_SKIN, CurrentSkin);
  EEPROM.writeByte(EE_BYTE_XDRGTKMUTE, XDRGTKMuteScreen);
  EEPROM.writeByte(EE_BYTE_FMAGC, fmagc);
  EEPROM.writeByte(EE_BYTE_AMAGC, amagc);
  EEPROM.writeByte(EE_BYTE_FMSI, fmsi);
  EEPROM.writeByte(EE_BYTE_SCANSTART, scanstart);
  EEPROM.writeByte(EE_BYTE_SCANSTOP, scanstop);
  EEPROM.writeByte(EE_BYTE_SCANHOLD, scanhold);
  EEPROM.writeByte(EE_BYTE_SCANMEM, scanmem);
  EEPROM.writeByte(EE_BYTE_SCANCANCEL, scancancel);
  EEPROM.writeByte(EE_BYTE_SCANMUTE, scanmute);
  EEPROM.writeByte(EE_BYTE_AUTOSQUELCH, autosquelch);
  EEPROM.writeByte(EE_BYTE_LONGBANDPRESS, longbandpress);
  EEPROM.writeByte(EE_BYTE_SHOWCLOCK, showclock);
  EEPROM.writeByte(EE_BYTE_SHOWLONGPS, showlongps);
  EEPROM.writeUInt(EE_UINT16_MEMSTARTFREQ, memstartfreq);
  EEPROM.writeUInt(EE_UINT16_MEMSTOPFREQ, memstopfreq);
  EEPROM.writeByte(EE_BYTE_MEMSTARTPOS, memstartpos);
  EEPROM.writeByte(EE_BYTE_MEMSTOPPOS, memstoppos);
  EEPROM.writeByte(EE_BYTE_MEMPIONLY, mempionly);
  EEPROM.writeByte(EE_BYTE_MEMDOUBLEPI, memdoublepi);
  EEPROM.writeByte(EE_BYTE_WAITONLYONSIGNAL, scanholdonsignal);
  EEPROM.writeByte(EE_BYTE_NTPOFFSET, NTPoffset);
  EEPROM.writeByte(EE_BYTE_AUTOLOG, autolog);
  EEPROM.writeByte(EE_BYTE_AUTODST, autoDST);
  EEPROM.writeByte(EE_BYTE_CLOCKAMPM, clockampm);
  EEPROM.writeUInt(EE_UINT16_PICTLOCK, radio.rds.PICTlock);
  EEPROM.commit();
}

void loadData() {
    if (EEPROM.readByte(EE_BYTE_CHECKBYTE) != EE_CHECKBYTE_VALUE) DefaultSettings();

  frequency = EEPROM.readUInt(EE_UINT16_FREQUENCY_FM);
  frequency_OIRT = EEPROM.readUInt(EE_UINT16_FREQUENCY_OIRT);
  VolSet = EEPROM.readByte(EE_BYTE_VOLSET);
  ConverterSet = EEPROM.readUInt(EE_UINT16_CONVERTERSET);
  LowEdgeSet = EEPROM.readUInt(EE_UINT16_FMLOWEDGESET);
  HighEdgeSet = EEPROM.readUInt(EE_UINT16_FMHIGHEDGESET);
  ContrastSet = EEPROM.readByte(EE_BYTE_CONTRASTSET);
  StereoLevel = EEPROM.readByte(EE_BYTE_STEREOLEVEL);
  bandFM = EEPROM.readByte(EE_BYTE_BANDFM);
  bandAM = EEPROM.readByte(EE_BYTE_BANDAM);
  HighCutLevel = EEPROM.readByte(EE_BYTE_HIGHCUTLEVEL);
  HighCutOffset = EEPROM.readByte(EE_BYTE_HIGHCUTOFFSET);
  LevelOffset = EEPROM.readByte(EE_BYTE_LEVELOFFSET);
  radio.rds.rtbuffer = EEPROM.readByte(EE_BYTE_RTBUFFER);
  edgebeep = EEPROM.readByte(EE_BYTE_EDGEBEEP);
  softmuteam = EEPROM.readByte(EE_BYTE_SOFTMUTEAM);
  softmutefm = EEPROM.readByte(EE_BYTE_SOFTMUTEFM);
  frequency_AM = EEPROM.readUInt(EE_UINT16_FREQUENCY_AM);
  language = EEPROM.readByte(EE_BYTE_LANGUAGE);
  showrdserrors = EEPROM.readByte(EE_BYTE_SHOWRDSERRORS);
  TEF = EEPROM.readByte(EE_BYTE_TEF);
  displayflip = EEPROM.readByte(EE_BYTE_DISPLAYFLIP);
  rotarymode = EEPROM.readByte(EE_BYTE_ROTARYMODE);
  tunemode = EEPROM.readByte(EE_BYTE_TUNEMODE);
  if (tunemode == TUNE_MAN) stepsize = EEPROM.readByte(EE_BYTE_STEPSIZE); else stepsize = 0;
  iMSset = EEPROM.readByte(EE_BYTE_IMSSET);
  EQset = EEPROM.readByte(EE_BYTE_EQSET);
  band = EEPROM.readByte(EE_BYTE_BAND);
  LowLevelSet = EEPROM.readByte(EE_BYTE_LOWLEVELSET);
  memorypos = EEPROM.readByte(EE_BYTE_MEMORYPOS);
  radio.rds.region = EEPROM.readByte(EE_BYTE_REGION);
  radio.underscore = EEPROM.readByte(EE_BYTE_RDS_UNDERSCORE);
  USBmode = EEPROM.readByte(EE_BYTE_USBMODE);
  wifi = EEPROM.readByte(EE_BYTE_WIFI);
  subnetclient = EEPROM.readByte(EE_BYTE_SUBNETCLIENT);
  showSWMIBand = EEPROM.readByte(EE_BYTE_SHOWSWMIBAND);
  radio.rds.filter = EEPROM.readByte(EE_BYTE_RDS_FILTER);
  radio.rds.pierrors = EEPROM.readByte(EE_BYTE_RDS_PIERRORS);
  frequency_LW = EEPROM.readUInt(EE_UINT16_FREQUENCY_LW);
  frequency_MW = EEPROM.readUInt(EE_UINT16_FREQUENCY_MW);
  frequency_SW = EEPROM.readUInt(EE_UINT16_FREQUENCY_SW);
  XDRGTK_key = EEPROM.readString(EE_STRING_XDRGTK_KEY);
  usesquelch = EEPROM.readByte(EE_BYTE_USESQUELCH);
  showmodulation = EEPROM.readByte(EE_BYTE_SHOWMODULATION);
  amnb = EEPROM.readByte(EE_BYTE_AM_NB);
  fmnb = EEPROM.readByte(EE_BYTE_FM_NB);
  audiomode = EEPROM.readByte(EE_BYTE_AUDIOMODE);
  touchrotating = EEPROM.readByte(EE_BYTE_TOUCH_ROTATING);
  hardwaremodel = EEPROM.readByte(EE_BYTE_HARDWARE_MODEL);
  poweroptions = EEPROM.readByte(EE_BYTE_POWEROPTIONS);
  CurrentTheme = EEPROM.readByte(EE_BYTE_CURRENTTHEME);
  fmdefaultstepsize = EEPROM.readByte(EE_BYTE_FMDEFAULTSTEPSIZE);
  screensaverset = EEPROM.readByte(EE_BYTE_SCREENSAVERSET);
  AMLevelOffset = EEPROM.readInt(EE_INT16_AMLEVELOFFSET);
  unit = EEPROM.readByte(EE_BYTE_UNIT);
  af = EEPROM.readByte(EE_BYTE_AF);
  if (af == 2) radio.rds.afreg = true; else radio.rds.afreg = false;
  StereoToggle = EEPROM.readByte(EE_BYTE_STEREO);
  batteryoptions = EEPROM.readByte(EE_BYTE_BATTERY_OPTIONS);
  amcodect = EEPROM.readByte(EE_BYTE_AM_CO_DECT);
  amcodectcount = EEPROM.readByte(EE_BYTE_AM_CO_DECT_COUNT);
  amgain = EEPROM.readByte(EE_BYTE_AM_RF_GAIN);
  radio.rds.sortaf = EEPROM.readByte(EE_BYTE_SORTAF);
  stationlistid = EEPROM.readByte(EE_BYTE_STATIONLISTID);
  fmdeemphasis = EEPROM.readByte(EE_BYTE_FM_DEEMPHASIS);
  BWsetFM = EEPROM.readByte(EE_BYTE_BWSET_FM);
  BWsetAM = EEPROM.readByte(EE_BYTE_BWSET_AM);
  nowToggleSWMIBand = EEPROM.readByte(EE_BYTE_BANDAUTOSW);
  radio.rds.fastps = EEPROM.readByte(EE_BYTE_FASTPS);
  tot = EEPROM.readByte(EE_BYTE_TOT);
  mwstepsize = EEPROM.readByte(EE_BYTE_MWREGION);
  spispeed = EEPROM.readByte(EE_BYTE_SPISPEED);
  amscansens = EEPROM.readByte(EE_BYTE_AMSCANSENS);
  fmscansens = EEPROM.readByte(EE_BYTE_FMSCANSENS);
  freqfont = EEPROM.readByte(EE_BYTE_FREQFONT);
  CurrentSkin = EEPROM.readByte(EE_BYTE_SKIN);
  XDRGTKMuteScreen = EEPROM.readByte(EE_BYTE_XDRGTKMUTE);
  fmagc = EEPROM.readByte(EE_BYTE_FMAGC);
  amagc = EEPROM.readByte(EE_BYTE_AMAGC);
  fmsi = EEPROM.readByte(EE_BYTE_FMSI);
  scanstart = EEPROM.readByte(EE_BYTE_SCANSTART);
  scanstop = EEPROM.readByte(EE_BYTE_SCANSTOP);
  scanhold = EEPROM.readByte(EE_BYTE_SCANHOLD);
  scanmem = EEPROM.readByte(EE_BYTE_SCANMEM);
  scancancel = EEPROM.readByte(EE_BYTE_SCANCANCEL);
  scanmute = EEPROM.readByte(EE_BYTE_SCANMUTE);
  autosquelch = EEPROM.readByte(EE_BYTE_AUTOSQUELCH);
  longbandpress = EEPROM.readByte(EE_BYTE_LONGBANDPRESS);
  showclock = EEPROM.readByte(EE_BYTE_SHOWCLOCK);
  showlongps = EEPROM.readByte(EE_BYTE_SHOWLONGPS);
  memstartfreq = EEPROM.readUInt(EE_UINT16_MEMSTARTFREQ);
  memstopfreq = EEPROM.readUInt(EE_UINT16_MEMSTOPFREQ);
  memstartpos = EEPROM.readByte(EE_BYTE_MEMSTARTPOS);
  memstoppos = EEPROM.readByte(EE_BYTE_MEMSTOPPOS);
  mempionly = EEPROM.readByte(EE_BYTE_MEMPIONLY);
  memdoublepi = EEPROM.readByte(EE_BYTE_MEMDOUBLEPI);
  scanholdonsignal = EEPROM.readByte(EE_BYTE_WAITONLYONSIGNAL);
  TouchCalData[0] = EEPROM.readUInt(EE_UINT16_CALTOUCH1);
  TouchCalData[1] = EEPROM.readUInt(EE_UINT16_CALTOUCH2);
  TouchCalData[2] = EEPROM.readUInt(EE_UINT16_CALTOUCH3);
  TouchCalData[3] = EEPROM.readUInt(EE_UINT16_CALTOUCH4);
  TouchCalData[4] = EEPROM.readUInt(EE_UINT16_CALTOUCH5);
  invertdisplay = EEPROM.readByte(EE_BYTE_INVERTDISPLAY);
  NTPoffset = EEPROM.readByte(EE_BYTE_NTPOFFSET);
  autolog = EEPROM.readByte(EE_BYTE_AUTOLOG);
  autoDST = EEPROM.readByte(EE_BYTE_AUTODST);
  clockampm = EEPROM.readByte(EE_BYTE_CLOCKAMPM);
  logcounter = EEPROM.readUInt(EE_UINT16_LOGCOUNTER);
  radio.rds.PICTlock = EEPROM.readUInt(EE_UINT16_PICTLOCK);
}

void DefaultSettings() {
  EEPROM.writeByte(EE_BYTE_CHECKBYTE, EE_CHECKBYTE_VALUE);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_FM, 9500);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_OIRT, FREQ_FM_OIRT_START);
  EEPROM.writeByte(EE_BYTE_VOLSET, 0);
  EEPROM.writeUInt(EE_UINT16_CONVERTERSET, 0);
  EEPROM.writeUInt(EE_UINT16_FMLOWEDGESET, 875);
  EEPROM.writeUInt(EE_UINT16_FMHIGHEDGESET, 1080);
  EEPROM.writeByte(EE_BYTE_CONTRASTSET, 50);
  EEPROM.writeByte(EE_BYTE_STEREOLEVEL, 0);
  EEPROM.writeByte(EE_BYTE_BANDFM, FM_BAND_ALL);
  EEPROM.writeByte(EE_BYTE_BANDAM, AM_BAND_ALL);
  EEPROM.writeByte(EE_BYTE_HIGHCUTLEVEL, 70);
  EEPROM.writeByte(EE_BYTE_HIGHCUTOFFSET, 0);
  EEPROM.writeByte(EE_BYTE_LEVELOFFSET, 0);
  EEPROM.writeByte(EE_BYTE_RTBUFFER, 1);
  EEPROM.writeByte(EE_BYTE_EDGEBEEP, 0);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEAM, 1);
  EEPROM.writeByte(EE_BYTE_SOFTMUTEFM, 0);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_AM, 828);
  EEPROM.writeByte(EE_BYTE_LANGUAGE, 0);
  EEPROM.writeByte(EE_BYTE_SHOWRDSERRORS, 1);
  EEPROM.writeByte(EE_BYTE_TEF, 0);
  EEPROM.writeByte(EE_BYTE_STEPSIZE, 0);
  EEPROM.writeByte(EE_BYTE_TUNEMODE, 0);
  EEPROM.writeByte(EE_BYTE_IMSSET, 1);
  EEPROM.writeByte(EE_BYTE_EQSET, 1);
  EEPROM.writeByte(EE_BYTE_BAND, BAND_FM);
  EEPROM.writeByte(EE_BYTE_LOWLEVELSET, -15);
  EEPROM.writeByte(EE_BYTE_MEMORYPOS, 0);
  EEPROM.writeByte(EE_BYTE_REGION, 0);
  EEPROM.writeByte(EE_BYTE_RDS_UNDERSCORE, 0);
  EEPROM.writeByte(EE_BYTE_USBMODE, 0);
  EEPROM.writeByte(EE_BYTE_WIFI, 0);
  EEPROM.writeByte(EE_BYTE_SUBNETCLIENT, 1);
  EEPROM.writeByte(EE_BYTE_SHOWSWMIBAND, 1);
  EEPROM.writeByte(EE_BYTE_RDS_FILTER, 1);
  EEPROM.writeByte(EE_BYTE_RDS_PIERRORS, 0);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_LW, 180);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_MW, 540);
  EEPROM.writeUInt(EE_UINT16_FREQUENCY_SW, 1800);
  EEPROM.writeString(EE_STRING_XDRGTK_KEY, "password");
  EEPROM.writeByte(EE_BYTE_USESQUELCH, 1);
  EEPROM.writeByte(EE_BYTE_SHOWMODULATION, 1);
  EEPROM.writeByte(EE_BYTE_AM_NB, 0);
  EEPROM.writeByte(EE_BYTE_FM_NB, 0);
  EEPROM.writeByte(EE_BYTE_AUDIOMODE, 0);
  EEPROM.writeByte(EE_BYTE_TOUCH_ROTATING, 0);
  EEPROM.writeUInt(EE_UINT16_LOWEDGEOIRTSET, 0);
  EEPROM.writeUInt(EE_UINT16_HIGHEDGEOIRTSET, 0);
  EEPROM.writeByte(EE_BYTE_POWEROPTIONS, 1);
  EEPROM.writeByte(EE_BYTE_CURRENTTHEME, 0);
  EEPROM.writeByte(EE_BYTE_FMDEFAULTSTEPSIZE, 1);
  EEPROM.writeByte(EE_BYTE_SCREENSAVERSET, 0);
  EEPROM.writeInt(EE_INT16_AMLEVELOFFSET, 0);
  EEPROM.writeByte(EE_BYTE_UNIT, 0);
  EEPROM.writeByte(EE_BYTE_AF, 0);
  EEPROM.writeByte(EE_BYTE_STEREO, 1);
  EEPROM.writeByte(EE_BYTE_BATTERY_OPTIONS, BATTERY_VALUE);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT, 100);
  EEPROM.writeByte(EE_BYTE_AM_CO_DECT_COUNT, 3);
  EEPROM.writeByte(EE_BYTE_AM_RF_GAIN, 0);
  EEPROM.writeByte(EE_BYTE_SORTAF, 1);
  EEPROM.writeByte(EE_BYTE_STATIONLISTID, 1);
  EEPROM.writeByte(EE_BYTE_FM_DEEMPHASIS, DEEMPHASIS_50);
  EEPROM.writeByte(EE_BYTE_BWSET_FM, 0);
  EEPROM.writeByte(EE_BYTE_BWSET_AM, 2);
  EEPROM.writeByte(EE_BYTE_BANDAUTOSW, 0);
  EEPROM.writeByte(EE_BYTE_FASTPS, 1);
  EEPROM.writeByte(EE_BYTE_TOT, 0);
  EEPROM.writeByte(EE_BYTE_MWREGION, 0);
  EEPROM.writeByte(EE_BYTE_AMSCANSENS, 4);
  EEPROM.writeByte(EE_BYTE_FMSCANSENS, 4);
  EEPROM.writeByte(EE_BYTE_FREQFONT, 3);
  EEPROM.writeByte(EE_BYTE_SKIN, 0);
  EEPROM.writeByte(EE_BYTE_XDRGTKMUTE, 0);
  EEPROM.writeByte(EE_BYTE_FMAGC, 92);
  EEPROM.writeByte(EE_BYTE_AMAGC, 100);
  EEPROM.writeByte(EE_BYTE_FMSI, 1);
  EEPROM.writeByte(EE_BYTE_SCANSTART, 0);
  EEPROM.writeByte(EE_BYTE_SCANSTOP, 9);
  EEPROM.writeByte(EE_BYTE_SCANHOLD, 5);
  EEPROM.writeByte(EE_BYTE_SCANMEM, 1);
  EEPROM.writeByte(EE_BYTE_SCANCANCEL, 0);
  EEPROM.writeByte(EE_BYTE_SCANMUTE, 0);
  EEPROM.writeByte(EE_BYTE_AUTOSQUELCH, 0);
  EEPROM.writeByte(EE_BYTE_LONGBANDPRESS, 0);
  EEPROM.writeByte(EE_BYTE_SHOWCLOCK, 1);
  EEPROM.writeByte(EE_BYTE_SHOWLONGPS, 1);
  EEPROM.writeUInt(EE_UINT16_MEMSTARTFREQ, 875);
  EEPROM.writeUInt(EE_UINT16_MEMSTOPFREQ, 1080);
  EEPROM.writeByte(EE_BYTE_MEMSTARTPOS, 1);
  EEPROM.writeByte(EE_BYTE_MEMSTOPPOS, 10);
  EEPROM.writeByte(EE_BYTE_MEMPIONLY, 1);
  EEPROM.writeByte(EE_BYTE_MEMDOUBLEPI, 0);
  EEPROM.writeByte(EE_BYTE_WAITONLYONSIGNAL, 1);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH1, 300);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH2, 3450);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH3, 300);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH4, 3450);
  EEPROM.writeUInt(EE_UINT16_CALTOUCH5, 3);
  EEPROM.writeByte(EE_BYTE_NTPOFFSET, 1);
  EEPROM.writeByte(EE_BYTE_AUTOLOG, 1);
  EEPROM.writeByte(EE_BYTE_AUTODST, 1);
  EEPROM.writeByte(EE_BYTE_CLOCKAMPM, 0);
  EEPROM.writeUInt(EE_UINT16_PICTLOCK, 0);

  EEPROM.writeByte(EE_BYTE_SPISPEED, 7);

#ifdef DEEPELEC_DP_66X
  EEPROM.writeByte(EE_BYTE_ROTARYMODE, 1);
  EEPROM.writeByte(EE_BYTE_INVERTDISPLAY, 0);
  EEPROM.writeByte(EE_BYTE_DISPLAYFLIP, 1);
  EEPROM.writeByte(EE_BYTE_HARDWARE_MODEL, PORTABLE_TOUCH_ILI9341);
#else
  EEPROM.writeByte(EE_BYTE_ROTARYMODE, 0);
  EEPROM.writeByte(EE_BYTE_INVERTDISPLAY, 1);
  EEPROM.writeByte(EE_BYTE_DISPLAYFLIP, 0);
  EEPROM.writeByte(EE_BYTE_HARDWARE_MODEL, BASE_ILI9341);
#endif

  for (int i = 0; i < EE_PRESETS_CNT; i++) {
    EEPROM.writeByte(i + EE_PRESETS_BAND_START, BAND_FM);
    EEPROM.writeUInt((i * 4) + EE_PRESETS_FREQUENCY_START, EE_PRESETS_FREQUENCY);
    EEPROM.writeByte(i + EE_PRESET_BW_START, 0);
    EEPROM.writeByte(i + EE_PRESET_MS_START, 1);

    for (int y = 0; y < 9; y++) EEPROM.writeByte((i * 9) + y + EE_PRESETS_RDSPS_START, '\0');

    for (int y = 0; y < 5; y++) EEPROM.writeByte((i * 5) + y + EE_PRESETS_RDSPI_START, '\0');
  }

  EEPROM.commit();

  handleCreateNewLogbook();
}