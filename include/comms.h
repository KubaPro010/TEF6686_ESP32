#pragma once

#include "globals.h"
#include "nonvolatile.h"
#include <Hash.h>
#include "main.h"

void Communication();
void XDRGTKRoutine();
void passwordcrypt();
void tryWiFi();
void total_pc_control();

extern void BuildDisplay();
extern void BuildAdvancedRDS();
extern void printLogbookCSV();
extern void NTPupdate();
extern void handleRoot();
extern void handleDownloadCSV();
extern void handleLogo();
extern void Infoboxprint(const char* input);
