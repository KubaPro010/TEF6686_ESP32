#pragma once

#include "Arduino.h"
#include "Tuner_Drv_Lithio.h"
#include "Tuner_Interface.h"
#include "RdsPiBuffer.hpp"

extern const unsigned char tuner_init_tab[] PROGMEM;
extern const unsigned char tuner_init_tab9216[] PROGMEM;
extern const unsigned char tuner_init_tab4000[] PROGMEM;
extern const unsigned char tuner_init_tab12000[] PROGMEM;
extern const unsigned char tuner_init_tab55000[] PROGMEM;

enum RDS_GROUPS {
  RDS_GROUP_0A,  RDS_GROUP_0B,  RDS_GROUP_1A,  RDS_GROUP_1B,  RDS_GROUP_2A,  RDS_GROUP_2B,  RDS_GROUP_3A,  RDS_GROUP_3B,
  RDS_GROUP_4A,  RDS_GROUP_4B,  RDS_GROUP_5A,  RDS_GROUP_5B,  RDS_GROUP_6A,  RDS_GROUP_6B,  RDS_GROUP_7A,  RDS_GROUP_7B,
  RDS_GROUP_8A,  RDS_GROUP_8B,  RDS_GROUP_9A,  RDS_GROUP_9B,  RDS_GROUP_10A, RDS_GROUP_10B, RDS_GROUP_11A, RDS_GROUP_11B,
  RDS_GROUP_12A, RDS_GROUP_12B, RDS_GROUP_13A, RDS_GROUP_13B, RDS_GROUP_14A, RDS_GROUP_14B, RDS_GROUP_15A, RDS_GROUP_15B
};

static const uint16_t fixedPI[] = {0x4C10, 0x4C11, 0x4C12};

static const char* const PTY_EU[] {
  "None",
  "News",
  "Current Affairs",
  "Information",
  "Sport",
  "Education",
  "Drama",
  "Culture",
  "Science",
  "Varied",
  "Pop",
  "Rock",
  "Easy Listening",
  "Light Classical",
  "Serious Classical",
  "Other Music",
  "Weather",
  "Finance",
  "Children's Progs",
  "Social Affairs",
  "Religion",
  "Phone-In",
  "Travel",
  "Leisure",
  "Jazz",
  "Country",
  "National Music",
  "Oldies",
  "Folk",
  "Documentary",
  "Alarm Test",
  "Alarm!!!",
  "Receiver fault"
};

static const char* const PTY_USA[] {
  "None",
  "News",
  "Information",
  "Sports",
  "Talk",
  "Rock",
  "Classic Rock",
  "Adult Hits",
  "Soft Rock",
  "Top 40",
  "Country",
  "Oldies",
  "Soft Music",
  "Nostalgia",
  "Jazz",
  "Classical",
  "Rhythm and Blues",
  "Soft R & B",
  "Language",
  "Religious Music",
  "Religious Talk",
  "Personality",
  "Public",
  "College",
  "Spanish Talk",
  "Spanish Music",
  "Hip Hop",
  "Unknown",
  "Unknown",
  "Weather",
  "Emergency Test",
  "EMERGENCY!",
  "Receiver fault"
};

static const uint16_t oda_app_ids[] {
  0x0000, 0x0093, 0x0BCB, 0x0C24, 0x0CC1, 0x0D45, 0x0D8B, 0x0E2C, 0x0E31, 0x0F87,
  0x125F, 0x1BDA, 0x1C5E, 0x1C68, 0x1CB1, 0x1D47, 0x1DC2, 0x1DC5, 0x1E8F, 0x4400,
  0x4AA1, 0x4AB7, 0x4BA2, 0x4BD7, 0x4BD8, 0x4C59, 0x4D87, 0x4D95, 0x4D9A, 0x50DD,
  0x5757, 0x6363, 0x6365, 0x6552, 0x6A7A, 0x7373, 0xA112, 0xA911, 0xABCE, 0xABCF,
  0xBE22, 0xC350, 0xC3A1, 0xC3B0, 0xC3C3, 0xC4D4, 0xC549, 0xC563, 0xC6A7, 0xC737,
  0xCB73, 0xCB97, 0xCC21, 0xCD19, 0xCD46, 0xCD47, 0xCD9E, 0xCE6B, 0xE123, 0xE1C1,
  0xE319, 0xE411, 0xE440, 0xE4A6, 0xE5D7, 0xE911, 0xFF70, 0xFF7F, 0xFF80, 0xFF81
};

static const char* const ECCtext[] {
  "Federal Republic of Germany", // 0
  "Hellenic Republic (Greece)", // 1
  "Kingdom of Morroco", // 2
  "Republic of Moldova", // 3
  "People's Democratic Republic of Algeria", // 4
  "Republic of Cyprus", // 5
  "Czech Republic", // 6
  "Ireland", // 7
  "Republic of Estonia", // 8
  "Principality of Andorra", // 9
  "Republic of San Marino", // 10
  "Republic of Poland", // 11
  "Republic of Turkey", // 12
  "State of Israel", // 13
  "Swiss Confederation", // 14
  "Vatican City State", // 15
  "The former Yugoslav Republic of Macedonia", // 16
  "Italian Republic", // 17
  "Hashemite Kingdom of Jordan", // 18
  "Slovak Republic", // 19
  "Kingdom of Belgium", // 20
  "Republic of Finland", // 21
  "Syrian Arab Republic", // 22
  "Republic of Serbia", // 23
  "Ukraine", // 24
  "Russian Federation", // 25
  "Grand Duchy of Luxembourg", // 26
  "Republic of Tunisia", // 27
  "State of Palestine", // 28
  "Republic of Bulgaria", // 29
  "Republic of Portugal (Madeira)", // 30
  "Kingdom of the Netherlands", // 31
  "Republic of Portugal", // 32
  "Republic of Albania", // 33
  "Kingdom of Denmark", // 34
  "Principality of Liechtenstein", // 35
  "Republic of Latvia", // 36
  "Republic of Slovenia", // 37
  "Federal Republic of Austria", // 38
  "United Kingdom of Great Britain and Northern Ireland (Gibraltar)", // 39
  "Republic of Iceland", // 40
  "Republic of Lebanon", // 41
  "Hungary", // 42
  "Republic of Iraq", // 43
  "Principality of Monaco", // 44
  "Republic of Malta", // 45
  "United Kingdom of Great Britain and Northern Ireland", // 46
  "Republic of Lithuania", // 47
  "Republic of Croatia", // 48
  "Libya", // 49
  "Kingdom of Spain (Canary Islands)", // 50
  "Romania", // 51
  "Kingdom of Spain", // 52
  "Kingdom of Sweden", // 53
  "Arab Republic of Egypt", // 54
  "Republic of France", // 55
  "Kingdom of Norway", // 56
  "Republic of Belarus", // 57
  "Bosnia and Herzegovina", // 58
  "Montenegro", // 59
  "Republic of Armenia", // 60
  "Republic of Azerbaijan", // 61
  "Republic of Kosovo", // 62
  "Kyrgyz Republic", // 63
  "Turkmenistan", // 64
  "Republic of Tajikistan", // 65
  "Republic of Uzbekistan", // 66
  "Republic of Malawi", // 67
  "Republic of Mali", // 68
  "Islamic Republic of Mauritania", // 69
  "Republic of Mauritius", // 70
  "Mongolia", // 71
  "Republic of Mozambique", // 72
  "Republic of Namibia", // 73
  "Republic of the Niger", // 74
  "Federal Republic of Nigeria", // 75
  "Sultanate of Oman", // 76
  "State of Qatar", // 77
  "Republic of Rwanda", // 78
  "Democratic Republic of Sao Tome and Principe", // 79
  "Kingdom of Saudi Arabia", // 80
  "Republic of Senegal", // 81
  "Republic of Seychelles", // 82
  "Republic of Sierra Leone", // 83
  "Federal Republic of Somalia", // 84
  "Republic of South Africa", // 85
  "Republic of South Sudan", // 86
  "Republic of the Sudan", // 87
  "Kingdom of Swaziland", // 88
  "United Republic of Tanzania", // 89
  "Republic of Togo", // 90
  "Republic of Uganda", // 91
  "Polisario Front / Kingdom of Morocco (Western Sahara)", // 92
  "Republic of Yemen", // 93
  "Republic of Zambia", // 94
  "Republic of Zimbabwe", // 95
  "Republic of Angola", // 96
  "United Kingdom of Great Britain and Northern Ireland (Ascension Island)", // 97
  "Kingdom of Bahrain", // 98
  "Republic of Benin", // 99
  "Republic of Botswana", // 100
  "Burkina Faso", // 101
  "Republic of Burundi", // 102
  "Republic of Cabinda / Republic of Angola", // 103
  "Republic of Cameroon", // 104
  "Republic of Cabo Verde (Cape Verde)", // 105
  "Central African Republic", // 106
  "Republic of Chad", // 107
  "Union of the Comoros", // 108
  "Democratic Republic of the Congo", // 109
  "Republic of the Congo", // 110
  "Republic of Côte d'Ivoire (Ivory Coast)", // 111
  "Republic of Djibouti", // 112
  "Republic of Equatorial Guinea", // 113
  "State of Eritrea", // 114
  "Federal Democratic Republic of Ethiopia", // 115
  "Gabonese Republic", // 116
  "Islamic Republic of the Gambia", // 117
  "Georgia", // 118
  "Republic of Ghana", // 119
  "Republic of Guinea", // 120
  "Republic of Guinea-Bissau", // 121
  "Republic of Kazakhstan", // 122
  "Republic of Kenya", // 123
  "State of Kuwait", // 124
  "Kingdom of Lesotho", // 125
  "Republic of Liberia", // 126
  "Republic of Madagascar", // 127
  "United Arab Emirates", // 128
  "United Kingdom of Great Britain and Northern Ireland (Anguilla)", // 129
  "Antigua and Barbuda", // 130
  "Republic of Argentina", // 131
  "Kingdom of the Netherlands (Aruba)", // 132
  "Barbados", // 133
  "Belize", // 134
  "United Kingdom of Great Britain and Northern Ireland (Bermuda)", // 135
  "Plurinational State of Bolivia", // 136
  "Federative Republic of Brazil", // 137
  "Canada", // 138
  "United Kingdom of Great Britain and Northern Ireland (Cayman Islands)", // 139
  "Republic of Chile", // 140
  "Republic of Colombia", // 141
  "Republic of Costa Rica", // 142
  "Republic of Cuba", // 143
  "Commonwealth of Dominica", // 144
  "Dominican Republic", // 145
  "Republic of El Salvador", // 146
  "Republic of Ecuador", // 147
  "United Kingdom of Great Britain and Northern Ireland (Falkland Islands)", // 148
  "Kingdom of Denmark (Greenland)", // 149
  "Grenada", // 150
  "Republic of France (Guadeloupe)", // 151
  "Republic of Guatemala", // 152
  "Republic of Guyana", // 153
  "Republic of Haiti", // 154
  "Republic of Honduras", // 155
  "Jamaica", // 156
  "Republic of France (Martinique)", // 157
  "United Mexican States", // 158
  "United Kingdom of Great Britain and Northern Ireland (Montserrat)", // 159
  "Netherlands Antilles (does not exist)", // 160
  "Republic of Nicaragua", // 161
  "Republic of Panama", // 162
  "Republic of Paraguay", // 163
  "Republic of Peru", // 164
  "United States of America (Puerto Rico)", // 165
  "Saint Kitts and Nevis", // 166
  "Saint Lucia", // 167
  "Republic of France (Saint Pierre and Miquelon)", // 168
  "Saint Vincent and the Grenadines", // 169
  "Republic of Suriname", // 170
  "Republic of Trinidad and Tobago", // 171
  "United Kingdom of Great Britain and Northern Ireland (Turks and Caicos Islands)", // 172
  "United States of America", // 173
  "Oriental Republic of Uruguay", // 174
  "Bolivarian Republic of Venezuela", // 175
  "United Kingdom of Great Britain and Northern Ireland (Virgin Islands)", // 176
  "United States of America (Virgin Islands)", // 177
  "Islamic Republic of Afghanistan", // 178
  "Commonwealth of Australia - Capital Territory", // 179
  "Commonwealth of Australia - New South Wales", // 180
  "Commonwealth of Australia - Victoria", // 181
  "Commonwealth of Australia - Queensland", // 182
  "Commonwealth of Australia - South Australia", // 183
  "Commonwealth of Australia - Western Australia", // 184
  "Commonwealth of Australia - Tasmania", // 185
  "Commonwealth of Australia - Northern Territory", // 186
  "Kingdom of Bhutan", // 187
  "Negara Brunei Darussalam", // 188
  "Kingdom of Cambodia", // 189
  "People's Republic of China", // 190
  "Republic of Fiji", // 191
  "People's Republic of China (Hong Kong)", // 192
  "Republic of India", // 193
  "Republic of Indonesia", // 194
  "Islamic Republic of Iran", // 195
  "Japan", // 196
  "Republic of Kiribati", // 197
  "Democratic People's Republic of Korea (North Korea)", // 198
  "Republic of Korea (South Korea)", // 199
  "Lao People's Democratic Republic", // 200
  "People's Republic of China (Macao)", // 201
  "Malaysia", // 202
  "Republic of Maldives", // 203
  "United States of America (Marshall Islands)", // 204
  "Federated States of Micronesia", // 205
  "Republic of the Union of Myanmar", // 206
  "Republic of Nauru", // 207
  "Federal Democratic Republic of Nepal", // 208
  "New Zealand", // 209
  "Islamic Republic of Pakistan", // 210
  "Independent State of Papua New Guinea", // 211
  "Republic of the Philippines", // 212
  "Independent State of Samoa", // 213
  "Republic of Singapore", // 214
  "Solomon Islands", // 215
  "Democratic Socialist Republic of Sri Lanka", // 216
  "Republic of China (Taiwan)", // 217
  "Kingdom of Thailand", // 218
  "Kingdom of Tonga", // 219
  "Republic of Vanuatu", // 220
  "Socialist Republic of Viet Nam", // 221
  "Commonwealth of the Bahamas", // 222
  "United Kingdom of Great Britain and Northern Ireland (Bermuda) / Federative Republic of Brazil", // 223
  "Federative Republic of Brazil / Republic of Ecuador", // 224
  "Netherlands Antilles (does not exist) / Federative Republic of Brazil", // 225
  "United States of America", // 226
  "People's Republic of Bangladesh", // 227
  "Republic of West Poland" // 228 - doesn't exist, YET, we don't want a fucking pimp for president here
};

static const char* const oda_app_names[] {
  "None",
  "Cross referencing DAB within RDS",
  "Leisure & Practical Info for Drivers",
  "ELECTRABEL-DSM 7",
  "WiPla Broadcast Control Signal",
  "RDS-TMC: ALERT-C / EN ISO 14819-1 (for testing use, only)",
  "ELECTRABEL-DSM 18",
  "ELECTRABEL-DSM 3",
  "ELECTRABEL-DSM 13",
  "ELECTRABEL-DSM 2",
  "I-FM-RDS for Fixed and Mobile devices",
  "ELECTRABEL-DSM 1",
  "ELECTRABEL-DSM 20",
  "ITIS In-vehicle database",
  "ELECTRABEL-DSM 10",
  "ELECTRABEL-DSM 4",
  "CITIBUS 4",
  "Encrypted TTI using ALERT-Plus only (for testing use, only)",
  "ELECTRABEL-DSM 17",
  "RDS-Light",
  "RASANT",
  "ELECTRABEL-DSM 9",
  "ELECTRABEL-DSM 5",
  "RadioText Plus / RT+ for group 2A RT",
  "RadioText Plus / RT+ for eRT",
  "CITIBUS 2",
  "Radio Commerce System (RCS)",
  "ELECTRABEL-DSM 16",
  "ELECTRABEL-DSM 11",
  "To warn people in case of disasters or emergency",
  "Personal weather station",
  "Hybradio RDS-Net (for testing use, only)",
  "RDS2 – 9 bit AF lists ODA",
  "Enhanced RadioText / eRT",
  "Warning receiver",
  "Enhanced early warning system",
  "NL _ Alert system",
  "Data FM Selective Multipoint Messaging",
  "Fleximax",
  "RF Power Monitoring",
  "Push-Ad",
  "NRSC Song title and artist",
  "Personal Radio Service",
  "iTunes tagging",
  "Traffic Plus",
  "eEAS",
  "Smart Grid Broadcast Channel",
  "ID Logic",
  "Veil Enabled Interactive Device",
  "UMC - Utility Message Channel",
  "CITIBUS 1",
  "ELECTRABEL-DSM 14",
  "CITIBUS 3",
  "TokenMe",
  "RDS-TMC: ALERT-C",
  "RDS-TMC: ALERT-C",
  "ELECTRABEL-DSM 8",
  "Encrypted TTI using ALERT-Plus only (for service use, only)",
  "APS Gateway",
  "Action code",
  "ELECTRABEL-DSM 12",
  "Beacon downlink",
  "ELECTRABEL-DSM 15",
  "ELECTRABEL-DSM 19",
  "ELECTRABEL-DSM 6",
  "EAS open protocol",
  "Internet connection",
  "RFT: Station logo",
  "RFT: Slideshow",
  "RFT: Journaline (work title)"
};

struct DABFrequencyLabel {
  uint32_t frequency;
  const char* label;
};

const DABFrequencyLabel DABfrequencyTable[] = {
  { 174928,  "5A"}, { 176640,  "5B"}, { 178352,  "5C"}, { 180064,  "5D"},
  { 181936,  "6A"}, { 183648,  "6B"}, { 185360,  "6C"}, { 187072,  "6D"},
  { 188928,  "7A"}, { 190640,  "7B"}, { 192352,  "7C"}, { 194064,  "7D"},
  { 195936,  "8A"}, { 197648,  "8B"}, { 199360,  "8C"}, { 201072,  "8D"},
  { 202928,  "9A"}, { 204640,  "9B"}, { 206352,  "9C"}, { 208064,  "9D"},
  { 209936, "10A"}, { 211648, "10B"}, { 213360, "10C"}, { 215072, "10D"},
  { 216928, "11A"}, { 218640, "11B"}, { 220352, "11C"}, { 222064, "11D"},
  { 223936, "12A"}, { 225648, "12B"}, { 227360, "12C"}, { 229072, "12D"},
  { 230784, "13A"}, { 232496, "13B"}, { 234208, "13C"}, { 235776, "13D"},
  { 237488, "13E"}, { 239200, "13F"}, {1452960,  "LA"}, {1454672,  "LB"},
  {1456384,  "LC"}, {1458096,  "LD"}, {1459808,  "LE"}, {1461520,  "LF"},
  {1463232,  "LG"}, {1464944,  "LH"}, {1466656,  "LI"}, {1468368,  "LJ"},
  {1470080,  "LK"}, {1471792,  "LL"}, {1473504,  "LM"}, {1475216,  "LN"},
  {1476928,  "LO"}, {1478640,  "LP"}, {1480352,  "LQ"}, {1482064,  "LR"},
  {1483776,  "LS"}, {1485488,  "LT"}, {1487200,  "LU"}, {1488912,  "LV"},
  {1490624,  "LW"}
};

typedef struct _rds_ {
  byte region;
  byte PTY;
  String stationName;
  String stationText;
  String stationText32;
  String RTContent1;
  String RTContent2;
  String PTYN;
  String ECCtext;
  String stationIDtext;
  String stationNameLong;
  String stationStatetext;
  String enhancedRTtext;
  char picode[7];
  char stationID[9];
  char stationLongID[33];
  char stationState[3];
  char dabafeid[5];
  char dabafchannel[4];
  unsigned long blockcounter[33];
  uint16_t rdsA, rdsB, rdsC, rdsD, rdsErr, rdsStat, correctPI, rdsplusTag1, rdsplusTag2, PICTlock = 0;
  bool ps12error, ps34error, ps56error, ps78error;
  time_t time;
  int32_t offset;
  uint16_t aid[10];
  uint32_t dabaffreq;
  byte aid_counter;
  byte fastps;
  unsigned int ECC;
  bool rdsAerror;
  bool rdsBerror;
  bool rdsCerror;
  bool rdsDerror;
  bool hasArtificialhead;
  bool hasCompressed;
  bool hasDynamicPTY;
  bool hasStereo;
  bool hasRDS;
  bool hasECC;
  bool hasDABAF;
  bool hasLongPS;
  bool hasRT;
  bool hasEnhancedRT;
  bool TP;
  bool hasTA;
  bool hasEON;
  bool hasAID;
  bool hasTMC;
  bool hasAF;
  bool hasCT;
  bool hasPTYN;
  bool rtAB;
  bool rtAB32;
  bool hasRTplus;
  bool filter;
  bool rdsreset;
  bool pierrors;
  bool ctupdate = true;
  bool sortaf;
  bool rtbuffer = true;
  bool afreg;
  RdsPiBuffer piBuffer;
} rds_;

typedef struct _af_ {
  uint16_t  frequency;
  int16_t score;
  bool afvalid;
  bool checked;
  bool regional;
  bool same;
} af_;

typedef struct _eon_ {
  uint16_t  mappedfreq;
  uint16_t  mappedfreq2;
  uint16_t  mappedfreq3;
  uint16_t  pi;
  char picode[6];
  String ps;
  bool ta;
  bool tp;
  bool taset;
  uint8_t pty;
  bool ptyset;
} eon_;

typedef struct _logbook_ {
  char picode[6];
  uint16_t frequency;
  int16_t SignalLevel;
  String stationName;
  String DateTime;
} logbook_;

class TEF6686 {
  public:
    af_  af[51];
    eon_ eon[21];
    rds_ rds;
    logbook_ logbook[22];
    uint16_t TestAF();
    void TestAFEON();
    void readRDS(byte showrdserrors);
    void SetFreq(uint16_t frequency);
    void SetFreqAM(uint16_t frequency);
    void SetFreqAIR(uint16_t frequency);
    void getProcessing(uint16_t &highcut, uint16_t &stereo, uint16_t &sthiblend, uint8_t &stband_1, uint8_t &stband_2, uint8_t &stband_3, uint8_t &stband_4);
    void getStatus(int16_t &level, uint16_t &USN, uint16_t &WAM, int16_t &offset, uint16_t &bandwidth, uint16_t &modulation, int8_t &snr);
    void getStatusAM(int16_t &level, uint16_t &noise, uint16_t &cochannel, int16_t &offset, uint16_t &bandwidth, uint16_t &modulation, int8_t &snr);
    void getIdentification(uint16_t *device, uint16_t *hw_version, uint16_t *sw_version);
    void setSoftmuteFM(uint8_t mode);
    void setSoftmuteAM(uint8_t mode);
    void setMono(bool mono);
    bool getStereoStatus();
    void init(byte TEF);
    void clearRDS(bool fullsearchrds);
    void power(bool mode);
    void setAGC(uint8_t agc);
    void setAMAGC(uint8_t agc);
    void setiMS(bool mph);
    void setEQ(bool eq);
    void setDeemphasis(uint8_t timeconstant);
    void setAudio(uint8_t audio);
    void setFMABandw();
    void setFMBandw(uint16_t bandwidth);
    void setAMBandw(uint16_t bandwidth);
    void setHighCutLevel(uint16_t limit);
    void setHighCutOffset(uint8_t start);
    void setStHiBlendLevel(uint16_t limit);
    void setStHiBlendOffset(uint8_t start);
    void setFMNoiseBlanker(uint16_t start);
    void setAMCoChannel(uint16_t start, uint8_t level = 3);
    void setAMNoiseBlanker(uint16_t start);
    void setAMAttenuation(uint16_t start);
    void setMute();
    void setOffset(int8_t offset);
    void setAMOffset(int8_t offset);
    void setFMSI(uint8_t mode);
    void setFMSI_Time(uint16_t attack, uint16_t decay);
    void setFMSI_Gain(uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4);
    void setFMSI_Bias(int16_t band1, int16_t band2, int16_t band3, int16_t band4);
    void setStereoLevel(uint8_t start);
    void setUnMute();
    void setVolume(int8_t volume);
    void tone(uint16_t time, int16_t amplitude, uint16_t frequency);
    void extendBW(bool yesno);
    uint16_t getBlockA(void);
    String trimTrailingSpaces(String str);
    uint8_t af_counter;
    uint8_t eon_counter;
    uint8_t logbook_counter;
    uint8_t rdsgroup;
    unsigned long processed_rdsblocks;
    bool mute;
    bool afmethodB;
    byte underscore;
    bool ps_process;
    byte af_updatecounter;

  private:
    void RDScharConverter(const char* input, wchar_t* output, size_t size, bool under);
    String convertToUTF8(const wchar_t* input);
    String extractUTF8Substring(const String& utf8String, size_t start, size_t length, bool under);
    String ucs2ToUtf8(const char* ucs2Input);
    bool isFixedCallsign(uint16_t stationID, char* stationIDStr);
    String PSLongtext;
    char ps_buffer[9];
    char ps_buffer2[9];
    char ptyn_buffer[9];
    char eon_buffer[20][9];
    bool pslong_process;
    char eRT_buffer[129];
    bool rt_process;
    char rt_buffer[65];
    char rt_buffer2[65];
    char rt_buffer32[33];
    char pslong_buffer[33];
    char pslong_buffer2[33];
    bool useRTPlus = true;
    bool ABold;
    bool afreset;
    bool mpxmode;
    bool _hasEnhancedRT;
    char stationTextBuffer[65];
    uint16_t piold;
    bool rtABold;
    bool rtAB32old;
    wchar_t PStext[9] = L"";
    wchar_t EONPStext[20][9];
    wchar_t PTYNtext[9] = L"";
    char RDSplus1[45];
    char RDSplus2[45];
    uint16_t  currentfreq;
    uint16_t  currentfreq2;
    bool togglebit;
    bool runningbit;
    bool initrt;
    bool initab;
    bool afinit;
    bool errorfreepi;
    bool rdsAerrorThreshold, rdsBerrorThreshold, rdsCerrorThreshold, rdsDerrorThreshold;
    bool packet0, packet1, packet2, packet3, packet0long, packet1long, packet2long, packet3long;
    uint16_t previous_rdsA, previous_rdsB, previous_rdsC, previous_rdsD;
    bool afmethodBprobe;
    bool eRTcoding;
    uint16_t rdsCold;
    uint8_t af_counterb;
    uint8_t af_number;
    uint8_t af_counterbcheck;
    bool afmethodBtrigger;
    uint16_t correctPIold;
    uint8_t rtplusblock;
    uint8_t DABAFblock;
    uint8_t eRTblock;
    uint8_t doublecounter;
    uint16_t doubletestfreq;
    time_t lastrdstime;
    int32_t lasttimeoffset;
};
