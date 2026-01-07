#pragma once

#include <FS.h>
using fs::FS;
#include <WebServer.h>
#include <SPIFFS.h>
#include "globals.h"

void handleRoot();
void handleDownloadCSV();
bool handleCreateNewLogbook();
byte addRowToCSV();
String getCurrentDateTime(bool inUTC);
bool isDST(time_t t);
void handleLogo();
void printLogbookCSV();
void sendUDPlog();
IPAddress makeBroadcastAddress(IPAddress ip);

extern const char* textUI(uint16_t number);
