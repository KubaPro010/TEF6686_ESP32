#include "logbook.h"
#include <EEPROM.h>

String UDPlogold = "";

void handleRoot() {
  fs::File file = SPIFFS.open("/logbook.csv", "r");
  if (!file) {
    webserver.send(500, "text/plain", "Failed to open logbook");
    return;
  }

  String html = "<!DOCTYPE html><html lang=\"en\"><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

  html += "<style>";
  html += "body {background-color: rgb(32, 34, 40); color: white; font-family: 'Arial', sans-serif;}";
  html += "h1 {text-align: center; margin-top: 20px;font-size: 32px;}";
  html += "img {display: block; margin: 0 auto; max-width: 100%; height: auto; padding-top: 20px; cursor: pointer;}";
  html += "table {width: 90%; max-width: 1500px; margin: 0 auto; border-radius: 15px; overflow: auto; padding: 20px; background-color: #2e5049; border: 0; border-collapse: separate !important; border-spacing: 0;}";
  html += "thead th {font-size: 18px; cursor: pointer; position: relative; user-select: none; background-color: #3c7f6a;}";
  html += "th, td {padding: 10px; text-align: center;}";
  html += "thead th:first-child, tbody th:first-child {border-radius: 15px 0 0 15px;}";
  html += "thead th:nth-last-of-type(1), tbody th:nth-last-of-type(1) {border-radius: 0 15px 15px 0;}";
  html += "tbody td:nth-child(3) {font-weight: 700;}";
  html += "tbody td:nth-child(4) {color: #5bd6ab;}";
  html += "tbody td:nth-child(5) {color: #ddd;}";
  html += "tbody td:nth-child(11) {color: #5bd6ab;font-weight: bold;}";
  html += "thead th:nth-child(1) {width: 12%;} thead th:nth-child(2) {width: 5%;} thead th:nth-child(3) {width: 10%;} thead th:nth-child(4) {width: 5%;} thead th:nth-child(5) {width: 10%;} thead th:nth-child(6) {width: 5%;} thead th:nth-child(6) {width: 5%;}";
  html += "button {background-color: #4db691;font-family: 'Arial', sans-serif;border: 0;padding: 15px 20px;font-size: 14px;text-transform: uppercase;cursor: pointer;border-radius: 15px;font-weight: bold;color: rgb(32, 34, 40);display: block;margin: 20px auto;transition: 0.3s ease background-color;}";
  html += "button:hover {background-color: #5bd6ab;}";
  html += ".go-to-bottom {position: fixed;bottom: 30px;right: 30px;z-index: 100;}";
  html += ".sort-icon {position: absolute;right: 10px;top: 50%;transform: translateY(-50%);color: #ccc;}";
  html += "@media (max-width: 768px) {table {width: 100%;}th, td {font-size: 14px;padding: 8px;}}";
  html += "a { text-decoration:none };";
  html += "</style>";
  html += "</head><body>";

  html += "<a href=\"https://fmdx.org/\" target=\"_blank\">";
  html += "<img src=\"/logo.png\" alt=\"FMDX website\">";
  html += "</a>";

  html += "<h1>" + String(textUI(286)) + "</h1>";
  html += "<button onclick=\"window.location.href='/downloadCSV'\">" + String(textUI(287)) + "</button>";
  html += "<button class=\"go-to-bottom\" onclick=\" window.scrollTo(0, document.body.scrollHeight);\">" + String(textUI(289)) + "</button>";

  // Sorting function with icons
  html += "<script>";
  html += "function sortTable(columnIndex) {";
  html += "var table = document.getElementById('logbookTable');";
  html += "var rows = Array.from(table.rows).slice(1);";
  html += "var isAscending = table.rows[0].cells[columnIndex].classList.toggle('asc');";
  html += "Array.from(table.rows[0].cells).forEach((th, index) => {";
  html += "if (index !== columnIndex) th.classList.remove('asc', 'desc');});";
  html += "rows.sort((a, b) => {";
  html += "var cellA = a.cells[columnIndex].textContent.trim();";
  html += "var cellB = b.cells[columnIndex].textContent.trim();";
  html += "return isAscending ? cellA.localeCompare(cellB) : cellB.localeCompare(cellA);});";
  html += "rows.forEach(row => table.appendChild(row));";
  html += "updateSortIcons(columnIndex, isAscending);}";

  // Function to update the sort icons
  html += "function updateSortIcons(columnIndex, isAscending) {";
  html += "document.querySelectorAll('th').forEach((th, index) => {";
  html += "th.querySelector('.sort-icon').textContent = '';";
  html += "if (index === columnIndex) {";
  html += "th.querySelector('.sort-icon').textContent = isAscending ? '▲' : '▼';}});}";
  html += "</script>";

  html += "<table id=\"logbookTable\"><thead>";

  String header = "";
  if (file.available()) {
    header = file.readStringUntil('\n');
    html += "<tr>";
    int startIndex = 0;
    int columnIndex = 0;

    // Generate table headers with sorting functionality
    while (startIndex < header.length()) {
      int endIndex = header.indexOf(',', startIndex);
      if (endIndex == -1) endIndex = header.length();
      String column = header.substring(startIndex, endIndex);

      // Add clickable headers for sorting
      html += "<th onclick=\"sortTable(" + String(columnIndex) + ")\">" + column;
      html += "<span class=\"sort-icon\"></span></th>";  // Sorting icon placeholder

      startIndex = endIndex + 1;
      columnIndex++;
    }
    html += "<th></th></tr><thead><tbody>";
  }

  bool hasData = false;
  int piCodeIndex = -1, frequencyIndex = -1;

  // Identify column indices for PI code and Frequency
  int startIndex = 0, columnIndex = 0;
  while (startIndex < header.length()) {
    int endIndex = header.indexOf(',', startIndex);
    if (endIndex == -1) endIndex = header.length();
    String column = header.substring(startIndex, endIndex);

    if (column.equalsIgnoreCase("PI")) piCodeIndex = columnIndex;
    if (column.equalsIgnoreCase("Frequency")) frequencyIndex = columnIndex;

    startIndex = endIndex + 1;
    columnIndex++;
  }

  if (file.available()) hasData = true;

  if (logcounter < 130) {
    // Generate rows
    while (file.available()) {
      String line = file.readStringUntil('\n');
      if (line.length() > 0) {
        html += "<tr>";

        String piCode = "", frequency = "";
        startIndex = 0, columnIndex = 0;

        while (startIndex < line.length()) {
          int endIndex = line.indexOf(',', startIndex);
          if (endIndex == -1) endIndex = line.length();
          String cell = line.substring(startIndex, endIndex);

          if (columnIndex == piCodeIndex) piCode = cell;
          if (columnIndex == frequencyIndex) frequency = cell;

          html += "<td>" + cell + "</td>";
          startIndex = endIndex + 1;
          columnIndex++;
        }

        frequency.replace(" MHz", "");

        html += "<td><a href =\"https://maps.fmdx.org/#qth=&freq=" + frequency + "&findPi=" + piCode + "\"target=\"_blank\">🌐</a></td>";
        html += "</tr>";
      }
    }

    file.close();
  } else html += "<tr><td colspan=\"100%\" style=\"text-align: center; color: red;\">" + String(textUI(299)) + "</td></tr>";

  if (!hasData) html += "<tr><td colspan=\"100%\" style=\"text-align: center; color: red;\">" + String(textUI(288)) + "</td></tr>";

  html += "</table>";
  html += "</body></html>";

  webserver.send(200, "text/html", html);
}

void handleDownloadCSV() {
  fs::File file = SPIFFS.open("/logbook.csv", "r");

  if (!file) {
    webserver.send(500, "text/plain", "Failed to open logbook for download");
    return;
  }

  webserver.sendHeader("Content-Type", "text/csv");
  webserver.sendHeader("Content-Disposition", "attachment; filename=logbook.csv");

  webserver.streamFile(file, "text/csv");

  file.close();
}

bool handleCreateNewLogbook() {
  if (SPIFFS.exists("/logbook.csv")) {
    if (!SPIFFS.remove("/logbook.csv")) return false;
  }

  fs::File file = SPIFFS.open("/logbook.csv", "w");
  if (!file) return false;

  file.print("Date,Time,Frequency,PI,Signal,Stereo,TA,TP,PTY,ECC,PS,Radiotext\n");

  file.flush();
  file.close();

  logcounter = 0;
  EEPROM.writeUInt(EE_UINT16_LOGCOUNTER, logcounter);
  EEPROM.commit();

  return true;
}

byte addRowToCSV() {
  if (SPIFFS.totalBytes() - SPIFFS.usedBytes() < 150 || logcounter > 1500) return 2;

  fs::File file = SPIFFS.open("/logbook.csv", "a");

  if (!file) return 1;

  String currentDateTime = getCurrentDateTime(false);
  if (currentDateTime == "") currentDateTime = "-,-";

  int freqInt = (band == BAND_OIRT) ? (int)frequency_OIRT : (int)frequency;
  int adjustedFreq = freqInt + (band != BAND_OIRT ? ConverterSet * 100 : 0);
  String frequencyFormatted = String(adjustedFreq / 100) + "." + ((adjustedFreq % 100 < 10) ? "0" : "") + String(adjustedFreq % 100) + " MHz";

  // Calculate signal strength based on the selected unit
  int SStatusPrint = 0;
  if (unit == 0) SStatusPrint = SStatus;  // dBμV
  else if (unit == 1) SStatusPrint = ((SStatus * 100) + 10875) / 100;  // dBf
  else if (unit == 2) SStatusPrint = round((float(SStatus) / 10.0 - 10.0 * log10(75) - 90.0) * 10.0);  // dBm

  String signal = String(SStatusPrint / 10) + "." + String(abs(SStatusPrint % 10));
  if (unit == 0) signal += " dBμV";
  else if (unit == 1) signal += " dBf";
  else if (unit == 2) signal += " dBm";

  String radioText = String(radio.rds.stationText + " " + radio.rds.stationText32);
  if (radio.rds.hasEnhancedRT) radioText += " eRT: " + String(radio.rds.enhancedRTtext);

  String stationName = radio.rds.stationName;
  String radioTextModified = (scanhold > 4 ? radioText : "");
  stationName.replace(",", " ");
  radioTextModified.replace(",", " ");

  String TA = radio.rds.TA ? "•" : " ";
  String TP = radio.rds.TP ? "•" : " ";
  String Stereo = radio.getStereoStatus() ? "•" : " ";
  String pty = String(radio.rds.PTY);
  String ECC = "--";
  if (radio.rds.hasECC) {
    char eccBuffer[3];
    snprintf(eccBuffer, sizeof(eccBuffer), "%02X", radio.rds.ECC.get());  // Format ECC as uppercase 2-digit hex
    ECC = String(eccBuffer);
  }

  String row = currentDateTime + "," + frequencyFormatted + "," + String(radio.rds.picode).substring(0, 4) + "," + signal + "," + Stereo + "," + TA + "," + TP + "," + pty + "," + ECC + "," + stationName + "," + radioTextModified + "\n";

  if (file.print(row)) {
    file.close();
    logcounter++;
    EEPROM.writeUInt(EE_UINT16_LOGCOUNTER, logcounter);
    EEPROM.commit();
    return 0;
  } else {
    file.close();
    return 1;
  }
}

String getCurrentDateTime(bool inUTC) {
  if (!rtcset) return "-,-";

  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) return "-,-";

  time_t currentEpoch = mktime(&timeInfo);
  int utcOffsetHours = 0;

  if (!inUTC) {
    currentEpoch += (NTPupdated ? NTPoffset * 3600 : radio.rds.offset); // Apply GMT offset if NTPupdated, else RDS offset

    if (NTPupdated && autoDST) {
      struct tm tempTimeInfo;
      localtime_r(&currentEpoch, &tempTimeInfo);
      if (isDST(mktime(&tempTimeInfo))) currentEpoch += 3600;
    }
  } else utcOffsetHours = (NTPupdated ? NTPoffset : radio.rds.offset / 3600);

  localtime_r(&currentEpoch, &timeInfo);

  char buf[20];

  if (clockampm) {
    strftime(buf, sizeof(buf), "%m/%d/%Y", &timeInfo);

    int hour = timeInfo.tm_hour;
    String ampm = (hour >= 12) ? "PM" : "AM";
    if (hour == 0) hour = 12;
    else if (hour > 12) hour -= 12;

    String timeWithAMPM = String(hour) + ":" + (timeInfo.tm_min < 10 ? "0" : "") + String(timeInfo.tm_min) + ":" + (timeInfo.tm_sec < 10 ? "0" : "") + String(timeInfo.tm_sec) + " " + ampm;

    if (inUTC) {
      String utcOffsetStr = ", " + String(utcOffsetHours) + ",";
      return String(buf) + "," + timeWithAMPM + utcOffsetStr;
    }

    return String(buf) + "," + timeWithAMPM;
  } else {
    strftime(buf, sizeof(buf), "%d-%m-%Y", &timeInfo);
    String timeEuropean = String(timeInfo.tm_hour) + ":" + (timeInfo.tm_min < 10 ? "0" : "") + String(timeInfo.tm_min) + ":" + (timeInfo.tm_sec < 10 ? "0" : "") + String(timeInfo.tm_sec); // Format time with leading zero if needed

    if (inUTC) {
      String utcOffsetStr = ", " + String(utcOffsetHours) + ",";
      return String(buf) + "," + timeEuropean + utcOffsetStr;
    }

    return String(buf) + "," + timeEuropean;
  }
}

bool isDST(time_t t) {
  struct tm timeInfo;
  localtime_r(&t, &timeInfo);

  int month = timeInfo.tm_mon + 1;
  int day = timeInfo.tm_mday;     
  int hour = timeInfo.tm_hour;    
  int weekday = timeInfo.tm_wday; 

  if (month == 3) {
    int lastSunday = 31 - ((weekday + 31 - day) % 7);
    if (day > lastSunday || (day == lastSunday && hour >= 2)) return true;
  }

  if (month == 10) {
    int lastSunday = 31 - ((weekday + 31 - day) % 7);
    if (day < lastSunday || (day == lastSunday && hour < 3)) return false;
  }

  if (month > 3 && month < 10) return true;
  return false;
}

void handleLogo() {
  fs::File file = SPIFFS.open("/logo.png", "r");
  if (!file) {
    webserver.send(404, "text/plain", "Logo not found");
    return;
  }
  webserver.streamFile(file, "image/png");
  file.close();
}

void printLogbookCSV() {
  fs::File file = SPIFFS.open("/logbook.csv", "r");

  if (!file) {
    Serial.println("Failed to open logbook!");
    return;
  }

  Serial.println("===== Start of logbook.csv =====");

  while (file.available()) {
    String line = file.readStringUntil('\n');
    Serial.println(line);
  }

  file.close();

  Serial.println("===== End of logbook.csv =====");
}

void sendUDPlog() {
  String currentDateTime = getCurrentDateTime(true);

  if (currentDateTime == "") currentDateTime = "-,-";

  int freqInt = (band == BAND_OIRT) ? (int)frequency_OIRT : (int)frequency;
  int adjustedFreq = freqInt + (band != BAND_OIRT ? ConverterSet * 100 : 0);
  String frequencyFormatted = String(adjustedFreq / 100) + "." + ((adjustedFreq % 100 < 10) ? "0" : "") + String(adjustedFreq % 100);

  int SStatusPrint = 0;
  if (unit == 0) SStatusPrint = SStatus;  // dBμV
  else if (unit == 1) SStatusPrint = ((SStatus * 100) + 10875) / 100;  // dBf
  else if (unit == 2) SStatusPrint = round((float(SStatus) / 10.0 - 10.0 * log10(75) - 90.0) * 10.0);  // dBm

  String signal = String(SStatusPrint / 10) + "." + String(abs(SStatusPrint % 10));
  if (unit == 0) signal += " dBμV";
  else if (unit == 1) signal += " dBf";
  else if (unit == 2) signal += " dBm";

  String radioText = String(radio.rds.stationText + " " + radio.rds.stationText32);
  if (radio.rds.hasEnhancedRT) radioText += " eRT: " + String(radio.rds.enhancedRTtext);

  String stationName = radio.rds.stationName;
  stationName.replace(",", " ");
  String radioTextModified = radioText;
  radioTextModified.replace(",", " ");

  String ECC = "";
  if (radio.rds.hasECC) {
    char eccBuffer[3];
    snprintf(eccBuffer, sizeof(eccBuffer), "%02X", radio.rds.ECC.get());
    ECC = String(eccBuffer);
  }

  String AF = "";
  if (radio.rds.hasAF && radio.af_counter > 0) {
    for (byte i = 0; i < radio.af_counter; i++) AF += String(radio.af[i].frequency / 100) + "." + String((radio.af[i].frequency % 100) / 10) + (i == radio.af_counter - 1 ? "" : ";");
  }

  String EON = "";
  if (radio.eon_counter > 0) {
    for (byte i = 0; i < radio.eon_counter; i++) {
      EON += String(radio.eon[i].picode) +
             (radio.eon[i].ps.length() > 0 ? String(";" + radio.eon[i].ps) : ";") +
             (radio.eon[i].mappedfreq > 0 ? String(";" + String(radio.eon[i].mappedfreq / 100) + "." + String((radio.eon[i].mappedfreq % 100) / 10)) : ";") +
             (radio.eon[i].mappedfreq2 > 0 ? String(";" + String(radio.eon[i].mappedfreq2 / 100) + "." + String((radio.eon[i].mappedfreq2 % 100) / 10)) : ";") +
             (radio.eon[i].mappedfreq3 > 0 ? String(";" + String(radio.eon[i].mappedfreq3 / 100) + "." + String((radio.eon[i].mappedfreq3 % 100) / 10)) : ";") +
             (i == radio.eon_counter - 1 ? "" : ";");
    }
  }

  String CHIP = "";
  switch (chipmodel) {
    case 0: CHIP = "TEF6686"; break;
#ifndef DEEPELEC_DP_66X
    case 1: CHIP = "TEF6687"; break;
    case 2: CHIP = "TEF6688"; break;
    case 3: CHIP = "TEF6689"; break;
#endif
  }

  String RTPLUS = "";
  if (radio.rds.hasRTplus) RTPLUS += radio.rds.RTContent1 + ";" + radio.rds.RTContent2;

  // Construct the data row to send via UDP
  String row = CHIP + "," + VERSION + "," + String(scandxmode) + "," +
               currentDateTime + "," + frequencyFormatted + "," + String(radio.rds.picode).substring(0, 4) + "," +
               signal + "," + String(radio.getStereoStatus()) + "," + String(radio.rds.TA) + "," +
               String(radio.rds.TP) + "," + String(radio.rds.hasTMC) + "," + String(radio.rds.PTY) + "," +
               ECC + "," + stationName + "," + radioTextModified + "," +
               AF + "," + EON + "," + RTPLUS + "\n";

  // Send the data via UDP if it's new
  if (UDPlogold != row) {
    IPAddress broadcastIP = makeBroadcastAddress(remoteip);
    Udp.beginPacket(broadcastIP, 9100);
    Udp.print(row);
    Udp.endPacket();
    UDPlogold = row;
  }
}

IPAddress makeBroadcastAddress(IPAddress ip) {
  // Assuming a typical subnet mask of 255.255.255.0
  return IPAddress(ip[0], ip[1], ip[2], 255);
}