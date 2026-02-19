#pragma once
#include <EEPROM.h>

// EEPROM index defines
#define EE_PRESETS_CNT 99    // When set > 99 change the complete EEPROM adressing!
#define EE_CHECKBYTE_VALUE 20     // 0 ~ 255,add new entry, change for new value
#define EE_PRESETS_FREQUENCY 0     // Default value when memory channel should be skipped!
#define EE_TOTAL_CNT 2296  // Total occupied eeprom bytes, we can take 20K

#define EE_PRESETS_BAND_START 0     // 99 * 1 byte
#define EE_PRESET_BW_START 99    // 99 * 1 byte
#define EE_PRESET_MS_START 198   // 99 * 1 byte
#define EE_PRESETS_FREQUENCY_START 297   // 99 * 4 bytes
#define EE_PRESETS_RDSPI_START 693   // 99 * 5 bytes
#define EE_PRESETS_RDSPS_START 1188  // 99 * 9 bytes

#define EE_UINT16_FREQUENCY_FM        2079
#define EE_BYTE_VOLSET                2083
#define EE_BYTE_STEREO                2084
#define EE_BYTE_BANDFM                2085
#define EE_BYTE_BANDAM                2086
#define EE_UINT16_CONVERTERSET        2087
#define EE_UINT16_FMLOWEDGESET        2091
#define EE_UINT16_FMHIGHEDGESET       2095
#define EE_BYTE_CONTRASTSET           2099
#define EE_BYTE_STEREOLEVEL           2100
#define EE_BYTE_HIGHCUTLEVEL          2101
#define EE_BYTE_HIGHCUTOFFSET         2102
#define EE_BYTE_LEVELOFFSET           2103
#define EE_BYTE_RTBUFFER              2104
#define EE_BYTE_SORTAF                2105
#define EE_BYTE_STATIONLISTID         2106
#define EE_BYTE_EDGEBEEP              2107
#define EE_BYTE_SOFTMUTEAM            2108
#define EE_BYTE_SOFTMUTEFM            2109
#define EE_UINT16_FREQUENCY_AM        2110
#define EE_BYTE_LANGUAGE              2114
#define EE_BYTE_SHOWRDSERRORS         2115
#define EE_BYTE_TEF                   2116
#define EE_BYTE_DISPLAYFLIP           2117
#define EE_BYTE_ROTARYMODE            2118
#define EE_BYTE_STEPSIZE              2119
#define EE_BYTE_TUNEMODE              2120
// empty byte
#define EE_BYTE_CHECKBYTE             2122
#define EE_BYTE_IMSSET                2123
#define EE_BYTE_EQSET                 2124
#define EE_BYTE_BAND                  2125
#define EE_BYTE_LOWLEVELSET           2126
#define EE_BYTE_BWSET_FM              2127
#define EE_BYTE_BWSET_AM              2128
#define EE_BYTE_BANDAUTOSW            2129
#define EE_BYTE_MEMORYPOS             2130
#define EE_BYTE_REGION                2131
#define EE_BYTE_RDS_UNDERSCORE        2132
#define EE_BYTE_USBMODE               2133
#define EE_BYTE_WIFI                  2134
#define EE_BYTE_SUBNETCLIENT          2135
#define EE_BYTE_SHOWSWMIBAND          2136
#define EE_BYTE_RDS_FILTER            2137
#define EE_BYTE_RDS_PIERRORS          2138
#define EE_BYTE_USESQUELCH            2139
#define EE_BYTE_SHOWAUDIO             2140
#define EE_BYTE_AM_NB                 2141
#define EE_BYTE_FM_NB                 2142
#define EE_BYTE_AUDIOMODE             2143
#define EE_BYTE_TOUCH_ROTATING        2144
#define EE_BYTE_HARDWARE_MODEL        2145
#define EE_BYTE_POWEROPTIONS          2146
#define EE_BYTE_CURRENTTHEME          2147
#define EE_BYTE_FMDEFAULTSTEPSIZE     2148
#define EE_BYTE_SCREENSAVERSET        2149
#define EE_BYTE_UNIT                  2150
#define EE_BYTE_AF                    2151
#define EE_BYTE_BATTERY_OPTIONS       2152
#define EE_BYTE_AM_CO_DECT            2153
#define EE_BYTE_AM_CO_DECT_COUNT      2154
#define EE_BYTE_AM_RF_GAIN            2155
#define EE_BYTE_FM_DEEMPHASIS         2156
#define EE_UINT16_FREQUENCY_LW        2157
#define EE_UINT16_FREQUENCY_MW        2161
#define EE_UINT16_FREQUENCY_SW        2165
#define EE_UINT16_LOWEDGEOIRTSET      2169
#define EE_UINT16_HIGHEDGEOIRTSET     2173
#define EE_INT16_AMLEVELOFFSET        2177
#define EE_UINT16_FREQUENCY_OIRT      2181
#define EE_STRING_XDRGTK_KEY          2185  // 11 byte
#define EE_BYTE_FASTPS                2196
#define EE_BYTE_TOT                   2197
#define EE_BYTE_MWREGION              2198
#define EE_BYTE_SPISPEED              2199
#define EE_BYTE_AMSCANSENS            2200
#define EE_BYTE_FMSCANSENS            2201
#define EE_BYTE_FREQFONT              2202
// Empty space, this was not used
#define EE_BYTE_XDRGTKMUTE 2204
#define EE_BYTE_FMAGC 2205
#define EE_BYTE_AMAGC 2206
#define EE_BYTE_FMSI 2207
#define EE_BYTE_SCANSTART 2208
#define EE_BYTE_SCANSTOP 2209
#define EE_BYTE_SCANHOLD 2210
#define EE_BYTE_SCANMEM 2211
#define EE_BYTE_SCANCANCEL 2212
#define EE_BYTE_SCANMUTE 2213
#define EE_BYTE_AUTOSQUELCH 2214
#define EE_BYTE_LONGBANDPRESS 2215
#define EE_BYTE_SHOWCLOCK 2216
#define EE_BYTE_SHOWLONGPS 2217
#define EE_UINT16_MEMSTARTFREQ 2218
#define EE_UINT16_MEMSTOPFREQ 2222
#define EE_BYTE_MEMSTARTPOS 2226
#define EE_BYTE_MEMSTOPPOS 2227
#define EE_BYTE_MEMPIONLY 2228
#define EE_BYTE_MEMDOUBLEPI 2229
// blank space
#define EE_BYTE_WAITONLYONSIGNAL 2253
#define EE_UINT16_CALTOUCH1 2254
#define EE_UINT16_CALTOUCH2 2258
#define EE_UINT16_CALTOUCH3 2262
#define EE_UINT16_CALTOUCH4 2266
#define EE_UINT16_CALTOUCH5 2270
#define EE_BYTE_INVERTDISPLAY 2274
#define EE_BYTE_TIMEZONE 2275
#define EE_BYTE_AUTOLOG 2276
#define EE_BYTE_AUTODST 2277
#define EE_BYTE_CLOCKAMPM 2278
#define EE_UINT16_LOGCOUNTER 2279
#define EE_UINT16_PICTLOCK 2283
#define EE_BYTE_CONTROLMODE 2285
// End of EEPROM index defines
#define EE_START_CONTROLMODE_DATA 2286
#define EE_LEN_CONTROLMODE_DATA 8

void StoreFrequency();
void ClearMemoryRange(uint8_t start, uint8_t stop);
void saveData();
void loadData();
void DefaultSettings();