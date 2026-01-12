#pragma once

#ifndef DEEPELEC_DP_66X
#define FORBIDDEN_TUNER(x) ((x) != 102 && (x) != 205)
#else
#define FORBIDDEN_TUNER(x) ((x) != 102)
#endif

void Tuner_Patch(byte TEF);
bool Tuner_WriteBuffer(unsigned char *buf, uint16_t len);
bool Tuner_ReadBuffer(unsigned char *buf, uint16_t len);
void Tuner_Reset();