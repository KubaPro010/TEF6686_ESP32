#pragma once

#define FORBIDDEN_TUNER(x) ((x) != 102 && (x) != 205)

void Tuner_Patch(byte TEF);
bool Tuner_WriteBuffer(unsigned char *buf, uint16_t len);
bool Tuner_ReadBuffer(unsigned char *buf, uint16_t len);
void Tuner_Reset();