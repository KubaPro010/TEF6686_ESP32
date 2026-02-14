#pragma once

/*
Super Replace stuff:

For the comment number:
Find: ' // \d+\'
Replace: '$$0'
Process: 

(text) => {
  const n = parseInt(text.match(/\d+/)[0], 10);
  return ` // ${n-1}`;
}

For the textUI call:
Find: 'textUI\(\d+\\)'
Replace: same as before
Process:

(text) => {
  const n = parseInt(text.match(/\d+/)[0], 10);
  return `textUI(${n-1})`;
}
*/

static const char* const myLanguage[22][298] PROGMEM = {
  { "English", // English
    "Rotary direction changed", // 1
    "Please release button", // 2
    "Screen flipped", // 3
    "Calibrate analog meter", // 4
    "USB mode", // 5
    "Tuner: !None!", // 6
    "Volume", // 7
    "Converter", // 8
    "Low band edge", // 9
    "High band edge", // 10
    "FM RF Level offset", // 11
    "Stereo threshold", // 12
    "High Cut corner", // 13
    "Highcut threshold", // 14
    "Low level threshold", // 15
    "Brightness", // 16
    "Set volume", // 17
    "Converter offset", // 18
    "Low band edge", // 19
    "High band edge", // 20
    "Level offset", // 21
    "Stereo separation threshold", // 22
    "High cut corner frequency", // 23
    "High cut threshold", // 24
    "Low level threshold", // 25
    "Display brightness", // 26
    "Off", // 27
    "On", // 28
    "PRESS MODE TO EXIT AND STORE", // 29
    "PRESS BAND TO CLOSE", // 30
    "SCANNING...", // 31
    "Tuner not detected", // 32
    "AM Antenna gain", // 33
    "FM AGC", // 34
    "Show RDS errors", // 35
    "Language", // 36
    "Choose language", // 37
    "PI locked CT sync", // 38
    "Error! No RDS signal", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Beep at band edge", // 42
    "Region", // 43
    "Europe", // 44
    "USA", // 45
    "Show underscore in RDS", // 46
    "PS only", // 47
    "Wi-Fi active", // 48
    "Configure Wi-Fi", // 49
    "Connect to: ", // 50
    "to configure Wi-Fi", // 51
    "Trying to connect Wi-Fi", // 52
    "FAILED!", // 53
    "CONNECTED!", // 54
    "Stationlist client IP", // 55
    "Show SW wavelength", // 56
    "RDS filter", // 57
    "Show PI errors", // 58
    "Use squelch", // 59
    "Audio level meter", // 60
    "AM Noise blanker", // 61
    "FM Noise blanker", // 62
    "Defaults loaded", // 63
    "Audio output", // 64
    "Allow tuning without\ndeactivating screensaver", // 65
    "Invert display colors", // 66
    "About software", // 67
    "Main code:", // 68
    "Contributors:", // 69
    "Unknown", // 70
    "Screensaver options", // 71
    "Auto Power off", // 72
    "Screen off", // 73
    "Theme", // 74
    "Skin", // 75
    "Detected applications", // 76
    "Min.", // 77
    "SPI speed", // 78
    "Scan sensitivity", // 79
    "NONE", // 80
    "Software version", // 81
    "Frequency font", // 82
    "Auto", // 83
    "No AF available", // 84
    "No EON available", // 85
    "No RT+ available", // 86
    "FM default stepsize", // 78
    "Screensaver", // 79
    "Sec", // 80
    "Clock mode", // 81
    "1% brightness", // 82
    "25% brightness", // 83
    "50% brightness", // 84
    "AM RF Level offset", // 85
    "Signalmeter unit", // 86
    "Use AF", // 87
    "Select FM Band", // 97
    "Select AM Band", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Stop screen wake on tune", // 104
    "Model selector", // 105
    "Base", // 106
    "Portable", // 107
    "Portable touch", // 108
    "Title", // 109
    "Album", // 110
    "Track", // 111
    "Artist", // 112
    "Composition", // 113
    "Movement", // 114
    "Conductor", // 115
    "Composer", // 116
    "Band", // 117
    "Comment", // 118
    "Genre", // 119
    "News", // 120
    "Local news", // 121
    "Stockmarket", // 122
    "Sport", // 123
    "Lottery", // 124
    "Horoscope", // 125
    "Daily Diversion", // 126
    "Health", // 127
    "Event", // 128
    "Scene", // 129
    "Cinema", // 130
    "TV", // 131
    "Date/time", // 132
    "Weather", // 133
    "Traffic", // 134
    "Alarm", // 135
    "Advertisement", // 136
    "Website", // 137
    "Other", // 138
    "Short PS", // 139
    "Long PS", // 140
    "Now", // 141
    "Next", // 142
    "Part", // 143
    "Host", // 144
    "Editor", // 145
    "Frequency", // 146
    "Homepage", // 147
    "Subchannel", // 148
    "Hotline", // 149
    "Studio phone", // 150
    "Phone", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email hotline", // 154
    "Email studio", // 155
    "Email", // 156
    "Chat", // 157
    "Chat centre", // 158
    "Vote question", // 159
    "Vote centre", // 160
    "Place", // 161
    "Appointment", // 162
    "Identifier", // 163
    "Purchase", // 164
    "Get data", // 165
    "MW default stepsize", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Battery options", // 170
    "Show voltage", // 171
    "Show percentage", // 172
    "RT buffering", // 173
    "Main settings", // 174
    "Audio settings", // 175
    "Display settings", // 176
    "RDS settings", // 177
    "FM settings", // 178
    "AM settings", // 179
    "Connectivity & clock", // 180
    "PRESS MODE TO RETURN", // 181
    "CoChannel Detector", // 182
    "CoChannel Detector\nsensitivity", // 183
    "CoChannel Detector\nthreshold", // 184
    "CoChannel Detector\ncounter", // 185
    "MAIN", // 186
    "AUDIO", // 187
    "DISPLAY", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CONNECT", // 192
    "DX MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM deemphasis", // 196
    "Small", // 197
    "Large", // 198
    "All", // 199
    "Fast PS", // 200
    "Default", // 201 TODO: remove
    "Mute screen on\nXDRGTK connection", // 202
    "FMSI stereo\nimprovement", // 203
    "Function only available\non TEF6687/6689!", // 204
    "Function not available\non selected skin!", // 205
    "Begin scanning\nmemory channels", // 206
    "Stop scanning\nmemory channels", // 207
    "Wait time", // 208
    "Start DX scanning", // 209
    "FM DX Options", // 210
    "Abort! The start channel\nis marked as skipped.", // 211
    "Auto sort AF", // 212
    "Memory channels", // 213
    "Exclude", // 214
    "Only", // 215
    "Auto cancel scan", // 216
    "Correct PI", // 217
    "Signal", // 218
    "Mute audio\nwhile scanning", // 219
    "AIR", // 220
    "5 sec. bandbutton press", // 221
    "Power off", // 222
    "Screen off", // 223
    "Disabled", // 224
    "None", // 225
    "News", // 226
    "Current Affairs", // 227
    "Information", // 228
    "Sport", // 229
    "Education", // 230
    "Drama", // 231
    "Culture", // 232
    "Science", // 233
    "Varied", // 234
    "Pop Music", // 235
    "Rock Music", // 236
    "Easy Listening", // 237
    "Light Classical", // 238
    "Serious Classical", // 239
    "Other Music", // 240
    "Weather", // 241
    "Finance", // 242
    "Children's Progs", // 243
    "Social Affairs", // 244
    "Religion", // 245
    "Phone-In", // 246
    "Travel", // 247
    "Leisure", // 248
    "Jazz Music", // 249
    "Country Music", // 250
    "National Music", // 251
    "Oldies Music", // 252
    "Folk Music", // 253
    "Documentary", // 254
    "Alarm Test", // 255
    "Alarm!!!", // 256
    "Initial", // 257
    "Always", // 258
    "Show clock", // 259
    "Show long PS", // 260
    "Start frequency", // 261
    "Stop frequency", // 262
    "Start memory channel", // 263
    "Stop memory channel", // 264
    "Only store with RDS", // 265
    "Start auto memory", // 266
    "Auto memory", // 267
    "Stationlist ID", // 268
    "Found", // 269
    "Abort! Out of memorychannels", // 270
    "Abort! User pressed button", // 271
    "Scan finished without errors", // 272
    "Clear memory channels", // 273
    "cleared", // 274
    "Prevent double PI", // 275
    "Range", // 276
    "Full", // 277
    "Wait time on\nsignal only", // 278
    "Press highlighted corner", // 279
    "To calibrate touchscreen", // 280
    "Hold BW button to\nstop DX scanner", // 281
    "Select Bandwidth", // 282
    "Your logbook", // 283
    "Download logbook", // 284
    "Logbook is empty", // 285
    "Go to bottom", // 286
    "Entry added\nto the logbook", // 287
    "Error adding\nentry to the logbook", // 288
    "Clear logbook", // 289
    "Logbook cleared", // 290
    "Clear failed", // 291
    "Can't add entry!\nLogbook is full!", // 292
    "Autologger", // 293
    "Auto DST on NTP time", // 294
    "Set NTP time offset", // 295
    "The logbook contains over 130 entries, which the viewer cannot process. Please download the CSV file to process it.", // 296
    "Canada" // 297
  },

  { "Nederlands", // Dutch
    "Rotary richting aangepast", // 1
    "Laat aub de knop los", // 2
    "Scherm gedraaid", // 3
    "Kalibratie analoge meter", // 4
    "USB mode", // 5
    "Tuner: !Geen!", // 6
    "Volume", // 7
    "Converter", // 8
    "Lage bandgrens", // 9
    "Hoge bandgrens", // 10
    "FM RF niveau afwijking", // 11
    "Stereo grens", // 12
    "Hoge tonen afval", // 13
    "Hoog afval grens", // 14
    "Laag signaal grens", // 15
    "Helderheid", // 16
    "Audio volume", // 17
    "Converter offset", // 18
    "Band ondergrens", // 19
    "Band bovengrens", // 20
    "Signaalafwijking", // 21
    "Stereo signaalgrens", // 22
    "Hoge tonen afval frequentie", // 23
    "Hoge tonen afval signaalniveau", // 24
    "Laag signaalgrens", // 25
    "Stel helderheid in", // 26
    "Uit", // 27
    "Aan", // 28
    "DRUK OP MODE OM AF TE SLUITEN", // 29
    "DRUK OP BAND OM AF TE SLUITEN", // 30
    "ZOEKEN...", // 31
    "Tuner niet herkend!", // 32
    "AM antenne versterking", // 33
    "FM AGC", // 34
    "Toon RDS fouten", // 35
    "Taal", // 36
    "Kies taal", // 37
    "PI gebonden CT sync", // 38
    "Fout! Geen RDS!", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Piep op bandeinde", // 42
    "Regio", // 43
    "Europa", // 44
    "USA", // 45
    "Toon underscore in RDS", // 46
    "Alleen PS", // 47
    "Wi-Fi aktief", // 48
    "Configureer Wi-Fi", // 49
    "Verbind met: ", // 50
    "om Wi-Fi in te stellen", // 51
    "Verbinden met Wi-Fi...", // 52
    "MISLUKT!", // 53
    "VERBONDEN!", // 54
    "Stationlist client IP", // 55
    "Toon SW golflengte", // 56
    "RDS filter", // 57
    "Toon PI fouten", // 58
    "Gebruik squelch", // 59
    "Audio niveau meter", // 60
    "AM ruis filter", // 61
    "FM ruis filter", // 62
    "Opnieuw geconfigureerd", // 63
    "Audio uitgang", // 64
    "Afstemmen tijdens\nschermbeveiliging", // 65
    "Inverteer scherm", // 66
    "Over deze software", // 67
    "Basis code:", // 68
    "Bijgedragen:", // 69
    "Onbekend", // 70
    "Screensaver opties", // 71
    "Auto uitschakelen", // 72
    "Scherm uit", // 73
    "Thema", // 74
    "Uiterlijk", // 75
    "Gedetecteerde applicaties", // 76
    "Min.", // 77
    "Dynamische\nSPI snelheid", // 78
    "Zoek gevoeligheid", // 79
    "Geen", // 80
    "Software versie", // 81
    "Frequentie font", // 82
    "Auto", // 83
    "Geen AF data", // 84
    "Geen EON data", // 85
    "Geen RT+ data", // 86
    "FM standaard\nstapgrootte", // 78
    "Schermbeveiliging", // 79
    "sec.", // 80
    "Klok methode", // 81
    "1% helderheid", // 82
    "25% helderheid", // 83
    "50% helderheid", // 84
    "AM RF Level afwijking", // 85
    "Signaalmeter eenheid", // 86
    "Gebruik AF", // 87
    "Selecteer\nFM banden", // 97
    "Selecteer\nAM banden", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Stop scherm ontwaken bij afstemmen", // 104
    "Model keuze", // 105
    "Basis", // 106
    "Portable", // 107
    "Portable touch", // 108
    "Titel", // 109
    "Album", // 110
    "Nummer", // 111
    "Artiest", // 112
    "Compositie", // 113
    "Deel", // 114
    "Dirigent", // 115
    "Componist", // 116
    "Band", // 117
    "Opmerking", // 118
    "Genre", // 119
    "Nieuws", // 120
    "Lokaal nieuws", // 121
    "Financiële beurs", // 122
    "Sport", // 123
    "Loterij", // 124
    "Horoscoop", // 125
    "Dagelijkse grap", // 126
    "Gezondheid", // 127
    "Evenement", // 128
    "Scene", // 129
    "Bioscoop", // 130
    "TV", // 131
    "Datum/tijd", // 132
    "Weer", // 133
    "Verkeer", // 134
    "Alarm", // 135
    "Advertentie", // 136
    "Website", // 137
    "Overige", // 138
    "Korte naam", // 139
    "Lange naam", // 140
    "Nu", // 141
    "Straks", // 142
    "Deel", // 143
    "Host", // 144
    "Regiseur", // 145
    "Frequentie", // 146
    "Homepage", // 147
    "Subkanaal", // 148
    "Hotline", // 149
    "Studio telefoon", // 150
    "Telefoon", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email hotline", // 154
    "Email studio", // 155
    "Email", // 156
    "Chat", // 157
    "Chat centrum", // 158
    "Stemming", // 159
    "Stemcentrum", // 160
    "Plaats", // 161
    "Afspraak", // 162
    "Identificatie", // 163
    "Aankoop", // 164
    "Verkrijg data", // 165
    "MW standaard\nstapgrootte", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Batterij weergave", // 170
    "Toon spanning", // 171
    "Toon percentage", // 172
    "RT buffer", // 173
    "Algemeen", // 174
    "Audio", // 175
    "Weergave", // 176
    "RDS instellingen", // 177
    "FM instellingen", // 178
    "AM instellingen", // 179
    "Connectiviteit", // 180
    "MODE OM TERUG TE GAAN", // 181
    "CoChannel Detectie", // 182
    "CoChannel Detectie\ngevoeligheid", // 183
    "CoChannel Detectie\ndrempel", // 184
    "CoChannel Detectie\nteller", // 185
    "ALGEMEEN", // 186
    "GELUID", // 187
    "SCHERM", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "VERBIND", // 192
    "DX MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM deemphasis", // 196
    "Laag", // 197
    "Middel", // 198
    "Alles", // 199
    "Snelle PS", // 200
    "Standaard", // 201
    "Scherm uit bij\nXDRGTK verbinding", // 202
    "FMSI stereo\nverbetering", // 203
    "Alleen beschikbaar op\nTEF6687/6689!", // 204
    "Niet beschikbaar op\ngekozen uiterlijk!", // 205
    "Begin zoeken\ngeheugenkanaal", // 206
    "Eind zoeken\ngeheugenkanaal", // 207
    "Wachttijd", // 208
    "Start DX zoeken", // 209
    "FM DX Opties", // 210
    "Fout! Startkanaal is\ngemarkeerd als skip", // 211
    "Auto AF sorteren", // 212
    "Geheugenkanalen", // 213
    "Uitsluiten", // 214
    "Enkel", // 215
    "Auto afbreken", // 216
    "Correcte PI", // 217
    "Signaal", // 218
    "Geen geluid\ntijdens scannen", // 219
    "AIR", // 220
    "5 sec.\nbandbutton ingedrukt", // 221
    "Uitschakelen", // 222
    "Scherm uit", // 223
    "Geen functie", // 224
    "Niet gedefinieerd", // 225
    "Nieuws", // 226
    "Actualiteit", // 227
    "Informatie", // 228
    "Sport", // 229
    "Educatie", // 230
    "Drama", // 231
    "Cultuur", // 232
    "Wetenschap", // 233
    "Varia", // 234
    "Popmuziek", // 235
    "Rockmuziek", // 236
    "Melodie", // 237
    "Licht klassiek", // 238
    "Klassieke muziek", // 239
    "Muziek", // 240
    "Het weer", // 241
    "Economie", // 242
    "Voor kinderen", // 243
    "Maatschappelijk", // 244
    "Religie", // 245
    "Doe mee !", // 246
    "Reizen", // 247
    "Vrije tijd", // 248
    "Jazz", // 249
    "Countrymuziek", // 250
    "Nationale muziek", // 251
    "Gouwe ouwe", // 252
    "Volksmuziek", // 253
    "Documentaires", // 254
    "Alarmtest", // 255
    "Alarm ! ALARM !", // 256
    "Initieel", // 257
    "Altijd", // 258
    "Toon klok", // 259
    "Toon long PS", // 260
    "Start frequentie", // 261
    "Stop frequentie", // 262
    "Start geheugen\nkanaal", // 263
    "Stop geheugen\nkanaal", // 264
    "Alleen met\nRDS opslaan", // 265
    "Start auto\ngeheugen kanaal", // 266
    "Auto geheugen kanaal", // 267
    "Stationlist ID", // 268
    "Gevonden", // 269
    "Afgebroken! Geheugen vol", // 270
    "Afgebroken! Knop ingedrukt", // 271
    "Zoeken zonder fouten voltooid", // 272
    "Wis geheugenkanalen", // 273
    "gewist", // 274
    "Voorkom dubbele PI", // 275
    "Bereik", // 276
    "Volledig", // 277
    "Wachttijd alleen\nbij signaal", // 278
    "Druk op aangegeven hoeken", // 279
    "om scherm te calibreren", // 280
    "Houdt BW knop lang\ningedrukt om te stoppen", // 281
    "Selecteer bandbreedte", // 282
    "Jouw logboek", // 283
    "Download logboek", // 284
    "Logboek is leeg", // 285
    "Ga naar einde", // 286
    "Toegevoegd aan\nhet logboek", // 287
    "Toevoegen aan\nlogboek mislukt!", // 288
    "Logboek legen", // 289
    "Logboek geleegd", // 290
    "Legen mislukt", // 291
    "Toevoegen mislukt!\nLogboek is vol!", // 292
    "Automatisch loggen", // 293
    "Auto zomertijd\nop NTP tijd", // 294
    "NTP tijdverschil\ninstellen", // 295
    "Het logboek bevat meer dan 130 items, die de viewer niet kan verwerken. Download alstublieft het CSV-bestand om het te bekijken.", // 296
    "Canada" // 297
  },

  { "Polski", // Polish
    "Kierunek obrotu zmieniony", // 1
    "Zwolnij przycisk", // 2
    "Obrócono ekran", // 3
    "Kalibracja analogowego miernika", // 4
    "Tryb USB", // 5
    "Tuner: !Brak!", // 6
    "Głośność", // 7
    "Konwerter", // 8
    "Dolna granica pasma", // 9
    "Górna granica pasma", // 10
    "FM Odchylenie poziomu RF", // 11
    "Próg Stereo", // 12
    "Krawędź dolnoprzep", // 13
    "Próg dolnoprzepust.", // 14
    "Próg niskiego sygnału", // 15
    "Kontrast", // 16
    "Ustaw głośność", // 17
    "Ustaw odchylenie konwertera", // 18
    "Ustaw dolną granicę pasma", // 19
    "Ustaw górną granicę pasma", // 20
    "Ustaw odchylenie sygnału", // 21
    "Ustaw próg separacji Stereo", // 22
    "Częst. gran. filtra dolnoprzep.", // 23
    "Ustaw próg dolnoprzep.", // 24
    "Ustaw dolną granicę sygnału", // 25
    "Ustaw jasność wyświetlacza", // 26
    "Wył.", // 27
    "Włącz.", // 28
    "WCIŚNIJ MODE ABY ZAMKNĄĆ I ZAPISAĆ", // 29
    "NACIŚNIJ BAND ABY ZAMKNĄĆ", // 30
    "SKANOWANIE...", // 31
    "Nie wykryto tunera", // 32
    "Czułość anteny AM", // 33
    "FM AGC", // 34
    "Pokaż błędy RDS", // 35
    "Język", // 36
    "Wybierz swój język", // 37
    "Synch. CT przy zablokowanym PI", // 38
    "Błąd! Brak sygnału RDS", // 39
    "Łagodne wyciszanie FM", // 40
    "Łagodne wyciszanie AM", // 41
    "Ton na krawędzi pasma", // 42
    "Region", // 43
    "Europa", // 44
    "USA", // 45
    "Pokaż podkreślenia w RDS", // 46
    "Tylko PS", // 47
    "Wi-Fi aktywne", // 48
    "Konfiguracja Wi-Fi", // 49
    "Połącz z: ", // 50
    "aby skonfigurować Wi-Fi", // 51
    "Próba połączenia z Wi-Fi", // 52
    "BŁĄD.. WiFi wyłączone", // 53
    "POŁĄCZONO!", // 54
    "IP klienta Stationlist", // 55
    "Pokaż długości fal SW", // 56
    "Filtr RDS", // 57
    "Pokaż błędy PI", // 58
    "Użyj blokady szumów", // 59
    "Miernik fonii", // 60
    "Reduktor szumów AM", // 61
    "Reduktor szumów FM", // 62
    "Załadowano domyślne", // 63
    "Wyjście dźwięku", // 64
    "Strój gdy ekran wył.", // 65
    "Odwróć kolory wyświetlacza", // 66
    "O oprogramowaniu", // 67
    "Główny kod:", // 68
    "Współtwórcy:", // 69
    "Nieznany", // 70
    "Opcje wygaszacza", // 71
    "Wyłącznik czasowy", // 72
    "Wyłącz ekran", // 73
    "Motyw", // 74
    "Skórka", // 75
    "Wykryte aplikacje", // 76
    "Min.", // 77
    "Prędkość SPI", // 78
    "Czułość skanowania", // 79
    "BRAK", // 80
    "Wersja oprogramowania", // 81
    "Czcionka częstotliwości", // 82
    "Auto", // 83
    "AF niedostępny", // 84
    "EON niedostępny", // 85
    "RT+ niedostępny", // 86
    "Minimalny krok FM", // 78
    "Wygaszacz ekranu", // 79
    "Sek", // 80
    "Tryb zegara", // 81
    "1% jasności", // 82
    "25% jasności", // 83
    "50% jasności", // 84
    "Odchylenie poziomu RF AM", // 85
    "Jednostka sygnału", // 86
    "Używaj AF", // 87
    "Wybierz pasmo FM", // 97
    "Wybierz pasmo AM", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Stroj.gdy ekran wył.", // 104
    "Wybór modelu", // 105
    "Podstawowy", // 106
    "Przenośny", // 107
    "Przenoś. dotyk.", // 108
    "Tytuł", // 109
    "Album", // 110
    "Ścieżka", // 111
    "Wykonawca", // 112
    "Kompozycja", // 113
    "Ruch", // 114
    "Dyrygent", // 115
    "Kompozytor", // 116
    "Grupa", // 117
    "Komentarz", // 118
    "Gatunek", // 119
    "Wiadomości", // 120
    "Lokalne wiadomości", // 121
    "Giełda", // 122
    "Sport", // 123
    "Loteria", // 124
    "Horoskop", // 125
    "Codzienny zwrot", // 126
    "Zdrowie", // 127
    "Wydarzenie", // 128
    "Scena", // 129
    "Kino", // 130
    "TV", // 131
    "Data/Godzina", // 132
    "Pogoda", // 133
    "Wiadom. drogowe", // 134
    "Alarm", // 135
    "Reklama", // 136
    "Strona internet.", // 137
    "Inne", // 138
    "Krótki PS", // 139
    "Długi PS", // 140
    "Teraz", // 141
    "Następnie", // 142
    "Część", // 143
    "Prowadzący", // 144
    "Wydawca", // 145
    "Częstotliwość", // 146
    "Strona domowa", // 147
    "Podkanał", // 148
    "Infolinia", // 149
    "Telefon do studia", // 150
    "Telefon", // 151
    "SMS do studia", // 152
    "SMS", // 153
    "Email do gorącej linii", // 154
    "Email do studia", // 155
    "Email", // 156
    "Czat", // 157
    "Centrum czatu", // 158
    "Pytanie do głosowania", // 159
    "Centrum głosowania", // 160
    "Miejsce", // 161
    "Spotkanie", // 162
    "Identyfikator", // 163
    "Zakup", // 164
    "Pobierz dane", // 165
    "Domyślny krok MW", // 166
    "Mandarynka", // 167
    "Ocean", // 168
    "Indygo", // 169
    "Opcje baterii", // 170
    "Pokaż napięcie", // 171
    "Pokaż procent naład.", // 172
    "Buforowanie RT", // 173
    "Główne ustawienia", // 174
    "Ustawienia dźwięku", // 175
    "Ustawienia wyświetlacza", // 176
    "Ustawienia RDS", // 177
    "Ustawienia FM", // 178
    "Ustawienia AM", // 179
    "Łączność i zegar", // 180
    "NACIŚNIJ MODE ABY WRÓCIĆ", // 181
    "Detek. międzykanałowy AM", // 182
    "Czułość detek. międzykanał.", // 183
    "Próg detek. międzykanałowego AM", // 184
    "Licznik detek. międzykanał.", // 185
    "GŁÓWNE", // 186
    "DŹWIĘK", // 187
    "WYŚWIETLACZ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "ŁĄCZNOŚĆ", // 192
    "TRYB DX", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "Deemfaza FM", // 196
    "Małe", // 197
    "Duże", // 198
    "Wszystkie", // 199
    "Szybki PS", // 200
    "Domyslny", // 201
    "Wylacz ekran gdy\npołączony z XDR-GTK", // 202
    "Ulepszenie \nstereo FMSI", // 203
    "Funkcja dostępna tylko\nna TEF6687/6689!", // 204
    "Funkcja niedostępna\nz wybranym motywem!", // 205
    "Rozpocznij skanowanie\nkanałów pamięci", // 206
    "Zatrzymaj skanowanie\nkanałów pamięci", // 207
    "Czas oczekiwania", // 208
    "Rozpocznij skanowanie DX", // 209
    "Opcje FM DX", // 210
    "Błąd! Początkowy kanał\njest pominięty.", // 211
    "Automat. sortowanie AF", // 212
    "Kanały pamięci", // 213
    "Wyklucz", // 214
    "Uwzględnij", // 215
    "Automatyczne\nanulowanie skanowania", // 216
    "Poprawne PI", // 217
    "Sygnał", // 218
    "Wycisz dźwięk\npodczas skanowania", // 219
    "AIR", // 220
    "5 sek. naciśnięcie przyc. BAND", // 221
    "Wyłącz", // 222
    "Wyłącz ekran", // 223
    "Wyłączone", // 224
    "Niezdefiniowany", // 225
    "Wiadomości", // 226
    "Sprawy bieżące", // 227
    "Informacje", // 228
    "Sport", // 229
    "Edukacja", // 230
    "Dramat", // 231
    "Kultura", // 232
    "Nauka", // 233
    "Różne", // 234
    "Muzyka Popowa", // 235
    "Muzyka Rockowa", // 236
    "Muzyka lekka", // 237
    "Lekka muz. klas.", // 238
    "Muz. klas. pow.", // 239
    "Inna muzyka", // 240
    "Pogoda", // 241
    "Finanse", // 242
    "Prog. dla dzieci", // 243
    "Sprawy społeczne", // 244
    "Religia", // 245
    "Telefon.Udział.", // 246
    "Podróże", // 247
    "Czas wolny", // 248
    "Muzyka Jazz", // 249
    "Muzyka Country", // 250
    "Muzyka narodowa", // 251
    "Stare przeboje", // 252
    "Muzyka ludowa", // 253
    "Dokument", // 254
    "Alarm Test", // 255
    "Alarm", // 256
    "Na start", // 257
    "Zawsze", // 258
    "Pokaż zegar", // 259
    "Pokaż długi PS", // 260
    "Częstotliwość początkowa", // 261
    "Częstotliwość końcowa", // 262
    "Początkowy kanał pamięci", // 263
    "Końcowy kanał pamięci", // 264
    "Zapisuj tylko z RDS", // 265
    "Rozpocznij auto zapamiętywanie", // 266
    "Auto zapamiętywanie", // 267
    "Identyfikator Stationlist", // 268
    "Znaleziono", // 269
    "Przerywam! Brak wolnych kanałów", // 270
    "Przerywam! Naciśnięto przycisk", // 271
    "Skanowanie zakończone pomyślnie", // 272
    "Wyczyść kanały pamięci", // 273
    "wyczyszczono", // 274
    "Unikaj podwójnego PI", // 275
    "Zakres", // 276
    "Pełne", // 277
    "Czekaj tylko gdy\nwykryto stację", // 278
    "Naciśnij zaznaczony róg", // 279
    "Aby skalibrować ekran", // 280
    "Aby zatrzymać,\nprzytrzymaj przycisk BW", // 281
    "Wybierz szer. pasma", // 282
    "Twój wykaz", // 283
    "Pobierz wykaz", // 284
    "Wykaz jest pusty", // 285
    "Przejdź na dół", // 286
    "Dodano do wykazu", // 287
    "Bład aktualizacji!", // 288
    "Wyczyść wykaz", // 289
    "Wykaz wyczyszczony", // 290
    "Błąd wyczyszczenia", // 291
    "Wykaz pełny!", // 292
    "Autologowanie stacji", // 293
    "Auto czas letni wg NTP", // 294
    "Ustaw odchyl. czasu NTP", // 295
    "Wykaz zawiera ponad 130 wpisów, których przeglądarka nie może pokazać. Pobierz plik CSV, aby je przetworzyć.", // 296
    "Kanada" // 297
  },

  { "Hrvatski", // Croatian
    "Smjer enkodera promij.", // 1
    "Otpustite gumb", // 2
    "Zaslon je okrenut", // 3
    "Kalibracija analog. mjerača", // 4
    "Način rada USB-a", // 5
    "Prijemnik: !Nije pronađen!", // 6
    "Glasnoća", // 7
    "Konverter", // 8
    "Donja granica opsega", // 9
    "Gornja granica opsega", // 10
    "FM pomak RF razine", // 11
    "Razina stereo signala", // 12
    "Granična frekvencija", // 13
    "Razina granične frekv.", // 14
    "Donja granica za signal", // 15
    "Svjetlina", // 16
    "Postavi glasnoću", // 17
    "Pomak konvertera", // 18
    "Donja granica frekv. opsega", // 19
    "Gornja granica frekv. opsega", // 20
    "Pomak signala", // 21
    "Razina stereo signala", // 22
    "Granična frekvencija", // 23
    "Razina granične frekv.", // 24
    "Donja granica za signal", // 25
    "Svjetlina zaslona", // 26
    "Isključeno", // 27
    "Uključeno", // 28
    "PRITISNITE MODE ZA IZLAZ I SPREMANJE", // 29
    "PRITISNITE BAND ZA IZLAZ", // 30
    "SKENIRANJE...", // 31
    "Prijemnik nije pronađen", // 32
    "AM Antenna gain", // 33
    "FM AGC", // 34
    "Prikaži RDS pogreške", // 35
    "Jezik", // 36
    "Odaberite jezik", // 37
    "PI locked CT sync", // 38
    "Error! No RDS signal", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Beep na rubu frekv. opsega", // 42
    "Regija", // 43
    "Europa", // 44
    "USA", // 45
    "Prikaži donju crtu u RDS-u", // 46
    "PS only", // 47
    "Wi-Fi", // 48
    "Wi-Fi postavke", // 49
    "Spojite se na: ", // 50
    "kako biste konfigurirali Wi-Fi", // 51
    "Spajanje na Wi-Fi mrežu..", // 52
    "NEUSPJEŠNO.. Wi-Fi onemogućen", // 53
    "SPOJENO!", // 54
    "IP klijenta StationList", // 55
    "Prikaži valnu duljinu KV-a", // 56
    "RDS filter", // 57
    "Prikaži PI greške", // 58
    "Prikaži squelch", // 59
    "Mjerač zvuka", // 60
    "AM prigušivač šuma", // 61
    "FM prigušivač šuma", // 62
    "Zadane postavke učitane", // 63
    "Audio izlaz", // 64
    "Omogući podešavanje\n tijekom čuvara zaslona", // 65
    "Invertiraj boju zaslona", // 66
    "O softveru", // 67
    "Glavni izvorni kod:", // 68
    "Suradnici:", // 69
    "Nepoznato", // 70
    "Opcije čuvara zaslona", // 71
    "Time-out timer", // 72
    "Zaslon isključiti", // 73
    "Tema dizajna", // 74
    "Skin", // 75
    "Otkrivene aplikacije", // 76
    "Min.", // 77
    "SPI brzina", // 78
    "Osjetljivost skeniranja", // 79
    "NEMA", // 80
    "Verzija softvera", // 81
    "Font frekvencije", // 82
    "Auto", // 83
    "AF nedostupan", // 84
    "EON nedostupan", // 85
    "RT+ nedostupan", // 86
    "FM min korak", // 78
    "Čuvar zaslona", // 79
    "Sek", // 80
    "Način rada sata", // 81
    "1% svjetlina", // 82
    "25% svjetlina", // 83
    "50% svjetlina", // 84
    "AM pomak RF razine", // 85
    "Jedinica mjerača signala", // 86
    "Koristi AF", // 87
    "Odaberi FM opseg", // 97
    "Odaberi AM opseg", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Okretanje dok je zaslon isključen", // 104
    "Selektor modela", // 105
    "Base", // 106
    "Portable", // 107
    "Portable touch", // 108
    "Naslov", // 109
    "Album", // 110
    "Pjesma", // 111
    "Izvođač", // 112
    "Kompozicija", // 113
    "Pokret", // 114
    "Dirigent", // 115
    "Skladatelj", // 116
    "Bend", // 117
    "Komentar", // 118
    "Žanr", // 119
    "Vijesti", // 120
    "Lokalne vijesti", // 121
    "Burza", // 122
    "Sport", // 123
    "Lutrija", // 124
    "Horoskop", // 125
    "Savjet dana", // 126
    "Zdravlje", // 127
    "Događanja", // 128
    "Scena", // 129
    "Kino", // 130
    "TV", // 131
    "Datum/vrijeme", // 132
    "Vremenska prognoza", // 133
    "Promet", // 134
    "Alarm", // 135
    "Reklama", // 136
    "Web stranica", // 137
    "Ostalo", // 138
    "Kratki PS", // 139
    "Dugi PS", // 140
    "Sada", // 141
    "Sljedi", // 142
    "Dio", // 143
    "Voditelj", // 144
    "Urednik", // 145
    "Frekvencija", // 146
    "Web stranica", // 147
    "Potkanal", // 148
    "Hotline", // 149
    "Telefon studija", // 150
    "Telefon", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email hotline", // 154
    "Email studija", // 155
    "Email", // 156
    "Chat", // 157
    "Chat centar", // 158
    "Pitanje za glasanje", // 159
    "Centar za glasanje", // 160
    "Mjesto", // 161
    "Ugovoreni sastanak", // 162
    "Identifikator", // 163
    "Kupnja", // 164
    "Primi podatke", // 165
    "MW zadana veličina koraka", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Postavke za bateriju", // 170
    "Prikaži napon", // 171
    "Prikaži postotak", // 172
    "RT pufer", // 173
    "Glavno", // 174
    "Zvuk", // 175
    "Zaslon", // 176
    "RDS postavke", // 177
    "FM postavke", // 178
    "AM postavke", // 179
    "Povezivost", // 180
    "PRITISNITE MODE ZA POVRATAK", // 181
    "AM CoChannelDet", // 182
    "CoChannelDet osjetljivost", // 183
    "AM CoChannelDet razina", // 184
    "CoChannelDet brojač", // 185
    "GLAVNO", // 186
    "ZVUK", // 187
    "ZASLON", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "SPOJI", // 192
    "DX NAČIN RADA", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM deemphasis", // 196
    "Malo", // 197
    "Veliko", // 198
    "Sve", // 199
    "Brzi PS", // 200
    "Zadano", // 201
    "Isključi zaslon\ntijekom veze XDRGTK-om", // 202
    "Poboljšanje\nFMSI stereo-a", // 203
    "Funkcija dostupna samo\nna TEF6687/6689!", // 204
    "Funkcija nije dostupna\na odabranoj temi dizajna!", // 205
    "Pokreni skeniranje\nmemorijskih kanala", // 206
    "Zaustavi skeniranje\nmemorijskih kanala", // 207
    "Vrijeme čekanja", // 208
    "Pokreni DX skeniranje", // 209
    "FM DX opcije", // 210
    "Prekid! Početni kanal je\noznačen kao preskočen.", // 211
    "Autom. sortiranje AF-a", // 212
    "Memorijski kanali", // 213
    "Isključeno", // 214
    "Samo", // 215
    "Autom. otkazivanje skeniranja", // 216
    "Točan PI", // 217
    "Signal", // 218
    "Isključi zvuk\ntijekom skeniranja", // 219
    "AIR", // 220
    "Pritisnite tipku band 5 sekundi", // 221
    "Isključivanje", // 222
    "Isključi zaslon", // 223
    "Onemogućeno", // 224
    "Neodređeno", // 225
    "Vijesti", // 226
    "Aktualno", // 227
    "Obavijesti", // 228
    "Sport", // 229
    "Obrazovanje", // 230
    "Drama", // 231
    "Kultura", // 232
    "Znanost", // 233
    "Razno", // 234
    "Pop glazba", // 235
    "Rock glazba", // 236
    "Zabavna glazba", // 237
    "Lagana klasična", // 238
    "Ozbiljna glazba", // 239
    "Ostala glazba", // 240
    "Vrijeme", // 241
    "Financije", // 242
    "Dječji program", // 243
    "Društvena događ", // 244
    "Religija", // 245
    "Kontakt program", // 246
    "Putovanja", // 247
    "Slobodno vrijeme", // 248
    "Jazz glazba", // 249
    "Country glazba", // 250
    "Domaća glazba", // 251
    "Stari hitovi", // 252
    "Narodna glazba", // 253
    "Dokumentarno", // 254
    "Testni alarm!", // 255
    "Alarm !", // 256
    "Početno", // 257
    "Uvijek", // 258
    "Prikaži sat", // 259
    "Prikaži dugi PS", // 260
    "Početna frekvencija", // 261
    "Završna frekvencija", // 262
    "Početni memorijski kanal", // 263
    "Završni memorijski kanal", // 264
    "Samo spremi s RDS-om", // 265
    "Pokreni autom. memoriju", // 266
    "Autom. memorija", // 267
    "ID StationList", // 268
    "Pronađeno", // 269
    "Prekid! Nema memorijskih kanala", // 270
    "Prekid! Korisnik je pritisnuo gumb", // 271
    "Skeniranje završeno bez grešaka", // 272
    "Očisti memorijske kanale", // 273
    "Očišćeno", // 274
    "Spriječi dupli PI", // 275
    "Raspon", // 276
    "Puno", // 277
    "Vrijeme čekanja na\nsignal samo", // 278
    "Pritisnite istaknuti kut", // 279
    "Za kalibraciju zaslona osjetljivog na dodir", // 280
    "Držite BW gumb za\nzaustavljanje DX skenera", // 281
    "Odabir širine pojasa", // 282
    "Vaš dnevnik", // 283
    "Preuzmi dnevnik", // 284
    "Dnevnik je prazan", // 285
    "Idi na dno", // 286
    "Unos dodan u dnevnik", // 287
    "Greška pri dodavanju unosa u dnevnik", // 288
    "Očisti dnevnik", // 289
    "Dnevnik očišćen", // 290
    "Čišćenje nije uspjelo", // 291
    "Ne mogu dodati unos!\nDnevnik je pun!", // 292
    "Autologger", // 293
    "Auto DST na NTP vremenu", // 294
    "Postavi NTP pomak vremena", // 295
    "Dnevnik sadrži preko 130 unosa, koje preglednik ne može obraditi. Preuzmite CSV datoteku za obradu.", // 296
    "Canada" // 297
  },

  { "Ελληνικά", // Greek
    "Η διεύθυνση του ρότορα\nάλλαξε", // 1
    "Ελευθερώστε το πλήκτρο", // 2
    "Η οθόνη\nαναποδογύρισε", // 3
    "Βαθμονόμηση\nαναλογικού μετρητή", // 4
    "USB λειτουργία", // 5
    "Tuner: !Κανένα!", // 6
    "Ένταση ήχου", // 7
    "Μετατροπέας", // 8
    "Κάτω όριο μπάντας", // 9
    "Άνω όριο μπάντας", // 10
    "FM Ολίσθηση\nεπιπέδου RF", // 11
    "Κατώφλι στέρεο", // 12
    "Όριο υψηλής αποκοπής", // 13
    "Κατώφλι υψηλής\nαποκοπής", // 14
    "Κατώφλι χαμηλού\nεπιπέδου", // 15
    "Αντίθεση", // 16
    "Ορισμός έντασης ήχου", // 17
    "Ολίσθηση (offset) κωδικοποιητή", // 18
    "Κάτω όριο μπάντας", // 19
    "Άνω όριο μπάντας", // 20
    "Επίπεδο ολίσθησης", // 21
    "Κατώφλι διαχωρισμού\nστέρεο", // 22
    "Συχνότητα υψηλής αποκοπής", // 23
    "Κατώφλι υψηλής\nαποκοπής", // 24
    "Κατώφλι χαμηλού\nεπιπέδου", // 25
    "Φωτεινότητα οθόνης", // 26
    "Ανενεργό", // 27
    "Ενεργό", // 28
    "ΠΙΕΣΤΕ MODE ΓΙΑ ΕΞΟΔΟ & ΑΠΟΘΗΚΕΥΣΗ", // 29
    "ΠΙΕΣΤΕ BAND ΓΙΑ ΚΛΕΙΣΙΜΟ", // 30
    "ΑΝΙΧΝΕΥΣΗ...", // 31
    "Το tuner δεν εντοπίστηκε", // 32
    "Κέρδος κεραίας AM", // 33
    "FM AGC", // 34
    "Περιθώριο σφαλμάτων\nRDS", // 35
    "Γλώσσα", // 36
    "Επιλογή γλώσσας", // 37
    "Συγχρονισμός CT σε\nκλείδωμα PI", // 38
    "Δεν υπάρχει σήμα RDS!", // 39
    "Ήπια σίγαση FM", // 40
    "Ήπια σίγαση AM", // 41
    "Ηχητικό σήμα στο όριο\nμπάντας", // 42
    "Περιοχή", // 43
    "Ευρώπη", // 44
    "Η.Π.Α.", // 45
    "Εμφάνιση κάτω παύλας\nστο RDS", // 46
    "PS μόνο", // 47
    "Wi-Fi ενεργό", // 48
    "Ρύθμιση Wi-Fi", // 49
    "Συνδεθείτε στο: ", // 50
    "για ρύθμιση του WiFi", // 51
    "Προσπάθεια σύνδεσης\nστο WiFi", // 52
    "ΑΠΟΤΥΧΙΑ..\nWiFi ανενεργό", // 53
    "ΣΕ ΣΥΝΔΕΣΗ!", // 54
    "IP πελάτη λίστας σταθμών", // 55
    "Εμφάνιση μήκους\nκύματος SW", // 56
    "Φίλτρο RDS", // 57
    "Εμφάνιση σφαλμάτων PI", // 58
    "Χρήση squelch", // 59
    "Μετρητής διαμόρφωσης", // 60
    "Απαλοιφή θορύβου AM", // 61
    "Απαλοιφή θορύβου FM", // 62
    "Οι προεπιλογές φορτώθηκαν", // 63
    "Έξοδος ήχου", // 64
    "Συντονισμός σε\nπροστασία οθόνης", // 65
    "Αντιστροφή\nχρωμάτων οθόνης", // 66
    "Πληροφορίες λογισμικού", // 67
    "Κύριος κώδικας:", // 68
    "Συνεργάτες:", // 69
    "Άγνωστο", // 70
    "Επιλογές προστασίας\nοθόνης", // 71
    "Αυτόματο σβήσιμο", // 72
    "Οθόνη σβηστή", // 73
    "Θέμα", // 74
    "Γραφικό\nπεριβάλλον", // 75
    "Εντοπισμένες εφαρμογές", // 76
    "Λεπτά", // 77
    "Δυναμική ταχύτητα\nSPI", // 78
    "Ευαισθησία\nανίχνευσης", // 79
    "KAMIA", // 80
    "Έκδοση λογισμικού", // 81
    "Γραμματοσειρά\nσυχνότητας", // 82
    "Αυτόματο", // 83
    "Μη διαθέσιμο AF", // 84
    "Μη διαθέσιμο EON", // 85
    "Μη διαθέσιμο RT+", // 86
    "Μέγεθος βήματος FM", // 78
    "Προστασία οθόνης", // 79
    "Δευτ.", // 80
    "Λειτουργία ρολογιού", // 81
    "1% φωτεινή", // 82
    "25% φωτεινή", // 83
    "50% φωτεινή", // 84
    "AM Ολίσθηση\nεπιπέδου RF", // 85
    "Μονάδα μέτρησης\nσήματος", // 86
    "Χρήση AF", // 87
    "Επιλογή μπάντας FM", // 97
    "Επιλογή μπάντας AM", // 98
    "LW", // 99
    "AM", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Μη αφύπνιση\nκατά τον συντονισμό", // 104
    "Επιλογή μοντέλου", // 105
    "Βάσης", // 106
    "Φορητό", // 107
    "Φορητό αφής", // 108
    "Τίτλος", // 109
    "Άλμπουμ", // 110
    "Κομμάτι", // 111
    "Καλλιτέχνης", // 112
    "Σύνθεση", // 113
    "Κίνηση", // 114
    "Μαέστρος", // 115
    "Συνθέτης", // 116
    "Μπάντα", // 117
    "Σχόλιο", // 118
    "Είδος", // 119
    "Ειδήσεις", // 120
    "Τοπικές ειδήσεις", // 121
    "Χρηματιστήριο", // 122
    "Αθλητικά", // 123
    "Λοταρία", // 124
    "Ωροσκόπιο", // 125
    "Καθημερινές ασχολίες", // 126
    "Υγεία", // 127
    "Γεγονός", // 128
    "Σκηνή", // 129
    "Σινεμά", // 130
    "TV", // 131
    "Ημερομηνία/ώρα", // 132
    "Καιρός", // 133
    "Κυκλοφορία", // 134
    "Συναγερμός", // 135
    "Διαφήμιση", // 136
    "Ιστότοπος", // 137
    "Άλλο", // 138
    "Σύντομο PS", // 139
    "Μακρύ PS", // 140
    "Τώρα", // 141
    "Στη συνέχεια", // 142
    "Μέρος", // 143
    "Παρουσιαστής", // 144
    "Συντάκτης", // 145
    "Συχνότητα", // 146
    "Ιστοσελίδα", // 147
    "Υποφέρουσα", // 148
    "Τηλ. επικοινωνίας", // 149
    "Τηλ. στούντιο", // 150
    "Τηλέφωνο", // 151
    "SMS στούντιο", // 152
    "SMS", // 153
    "Email επικοινωνίας", // 154
    "Email στούντιο", // 155
    "Email", // 156
    "Chat", // 157
    "Κέντρο chat", // 158
    "Ερώτηση ψηφοφορίας", // 159
    "Κέντρο ψηφοφορίας", // 160
    "Τοποθεσία", // 161
    "Ραντεβού", // 162
    "Αναγνωριστικό", // 163
    "Αγορά", // 164
    "Λήψη δεδομένων", // 165
    "Προεπιλεγμένο μέγεθος\nβήματος ΑΜ", // 166
    "Μανταρινί", // 167
    "Ωκεανός", // 168
    "Ινδικό", // 169
    "Επιλογές μπαταρίας", // 170
    "Εμφάνιση τάσης", // 171
    "Εμφάνιση ποσοστού", // 172
    "RT προφόρτωση", // 173
    "Βασικές ρυθμίσεις", // 174
    "Ρυθμίσεις ήχου", // 175
    "Ρυθμίσεις εμφάνισης", // 176
    "Ρυθμίσεις RDS", // 177
    "Ρυθμίσεις FM", // 178
    "Ρυθμίσεις AM", // 179
    "Συνδεσιμότητα & ρολόι", // 180
    "ΠΙΕΣΤΕ MODE ΓΙΑ ΕΠΙΣΤΡΟΦΗ", // 181
    "Εντοπισμός\nομοκαναλικού AM", // 182
    "Ευαισθησία\nομοκαναλικού AM", // 183
    "Κατώφλι\nομοκαναλικού AM", // 184
    "Μετρητής\nομοκαναλικού AM", // 185
    "ΒΑΣΙΚΕΣ", // 186
    "ΗΧΟΣ", // 187
    "ΕΜΦΑΝΙΣΗ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "ΣΥΝΔΕΣΗ", // 192
    "DX MODE", // 193
    "ΑΥΤ. ΜΝΗΜΗ", // 194
    "AM AGC", // 195
    "FM αποέμφαση", // 196
    "Μικρό", // 197
    "Μεγάλο", // 198
    "Όλα", // 199
    "Γρήγορο PS", // 200
    "Προεπιλογή", // 201
    "Μαύρη οθόνη σε\nσύνδεση XDRGTK", // 202
    "FMSI βελτίωση\nήχου στέρεο", // 203
    "Λειτουργία διαθέσιμη\nμόνο στα TEF6687/6689!", // 204
    "Λειτουργία μη διαθέσιμη\nσε αυτή την εμφάνιση!", // 205
    "Εκκίνηση ανίχνευσης\nκαναλιών μνήμης", // 206
    "Διακοπή ανίχνευσης\nκαναλιών μνήμης", // 207
    "Χρόνος αναμονής", // 208
    "Εκκίνηση DX ανίχνευσης", // 209
    "Επιλογές FM DX", // 210
    "Ματαίωση! Λείπει\nτο κανάλι εκκίνησης.", // 211
    "Αυτόματη ταξινόμηση\nAF", // 212
    "Κανάλια μνήμης", // 213
    "Εξαίρεση", // 214
    "Μόνο", // 215
    "Αυτόματη ακύρωση\nανίχνευσης", // 216
    "Σωστό PI", // 217
    "Σήμα", // 218
    "Σίγαση ήχου\nκατά την ανίχνευση", // 219
    "AIR", // 220
    "Πιέση πλήκτρου\nμπάντας 5 δευτ.", // 221
    "Σβήσιμο", // 222
    "Σβήσιμο οθόνης", // 223
    "Ανενεργό", // 224
    "Κανένα", // 225
    "Ειδήσεις", // 226
    "Επικαιρότητα", // 227
    "Πληροφόρηση", // 228
    "Σπορ", // 229
    "Εκπαίδευση", // 230
    "Δράμα", // 231
    "Πολιτισμός", // 232
    "Επιστήμη", // 233
    "Ποικίλο", // 234
    "Ποπ μουσική", // 235
    "Ροκ μουσική", // 236
    "Εύκολη ακρόαση", // 237
    "Ελαφρά κλασική", // 238
    "Σοβαρή κλασική", // 239
    "Άλλη μουσική", // 240
    "Καιρός", // 241
    "Οικονομικά", // 242
    "Παιδικό πρόγραμμα", // 243
    "Κοινωνικά", // 244
    "Θρησκεία", // 245
    "Τηλεφωνικά", // 246
    "Ταξίδια", // 247
    "Ελεύθερος χρόνος", // 248
    "Τζαζ μουσική", // 249
    "Κάντρι μουσική", // 250
    "Εθνική μουσική", // 251
    "Παλαιές επιτυχίες", // 252
    "Παραδοσιακή μουσική", // 253
    "Ντοκιμαντέρ", // 254
    "Δοκιμή συναγερμού", // 255
    "Συναγερμός!!!", // 256
    "Αρχικό", // 257
    "Πάντα", //261
    "Εμφάνιση ρολογιού", // 259
    "Εμφάνιση μεγάλου PS", // 260
    "Συχνότητα εκκίνησης", // 261
    "Τελική συχνότητα", // 262
    "Αρχικό κανάλι μνήμης", // 263
    "Τελικό κανάλι μνήμης", // 264
    "Αποθήκευση μόνο\nμε RDS", // 265
    "Εκκίνηση\nαυτόματης μνήμης", // 266
    "Αυτόματη μνήμη", // 267
    "ID λίστας σταθμών", // 268
    "Βρέθηκαν", // 269
    "Ματαίωση! Τα κανάλια\nμνήμης εξαντλήθηκαν", // 270
    "Ματαίωση! Πιέση\nπλήκτρου χρήστη", // 271
    "Η ανίχνευση ολοκληρώθηκε\nχωρίς σφάλματα", // 272
    "Εκκαθάριση\nκαναλιών μνήμης", // 273
    "έγινε εκκαθάριση", // 274
    "Αποφυγή διπλού PI", // 275
    "Εύρος", // 276
    "Πλήρης", // 277
    "Αναμονή μόνο σε\nυπάρξη σήματος", // 278
    "Πιέστε την επισημασμένη\nγωνία για βαθμονόμηση", // 279
    "της οθόνης αφής", // 280
    "Κρατήστε το BW για\nδιακοπή της DX σάρωσης", // 281
    "Επιλογή εύρους μπάντας", // 282
    "Το βιβλίο καταγραφών σας", // 283
    "Λήψη βιβλίου καταγραφών", // 284
    "Το βιβλίο καταγραφών είναι άδειο", // 285
    "Μετάβαση στον πάτο", // 286
    "Προστέθηκε\nστις καταγραφές", // 287
    "Σφάλμα προσθήκης\nστις καταγραφές", // 288
    "Εκκαθάριση βιβλίου\nκαταγραφών", // 289
    "Εκκαθαρίστηκε", // 290
    "Αποτυχία εκκαθάρισης", // 291
    "Αδυναμία προσθήκης!\nΒιβλίο γεμάτο!", // 292
    "Αυτόματη καταγραφή", // 293
    "Αυτόματο DST\nσε ώρα NTP", // 294
    "Ολίσθηση ώρας NTP", // 295
    "Το βιβλίο περιέχει πάνω από 130 καταγραφές, και είναι αδύνατη η επεξεργασία τους από το χρήστη. Κάντε λήψη του αρχείου CSV για να το επεξεργαστείτε.", // 296
    "Καναδάς" // 297
  },

  { "Română", // Romanian
    "Direcția rotirii schimbată", // 1
    "Eliberați butonul", // 2
    "Ecran întors", // 3
    "Calibrați instrumentul analog", // 4
    "Mod USB", // 5
    "Tuner: !Niciunul!", // 6
    "Volum", // 7
    "Convertor", // 8
    "Marginea de jos a benzii", // 9
    "Marginea de sus a benzii", // 10
    "Decalaj de nivel RF FM", // 11
    "Pragul stereo", // 12
    "Limită tăiere înalte", // 13
    "Prag tăiere înalte", // 14
    "Prag nivel joase", // 15
    "Luminozitate", // 16
    "Setare volum", // 17
    "Setare decalaj convertor", // 18
    "Setarea limitei de jos a benzii", // 19
    "Setarea limitei de sus a benzii", // 20
    "Setare nivel decalaj", // 21
    "Setarea pragului sep. Stereo", // 22
    "Setare tăiere frecv. înalte", // 23
    "Setare prag tăiere înalte", // 24
    "Setarea pragului la joase", // 25
    "Luminozitate ecran", // 26
    "Oprit", // 27
    "Pornit", // 28
    "APĂSAȚI MODE PENTRU IEȘIRE ȘI SALVARE", // 29
    "APĂSAȚI BAND PENTRU ÎNCHIDERE", // 30
    "SCANEZ...", // 31
    "Tunerul nu este prezent", // 32
    "Reglare nivel antenă AM", // 33
    "FM AGC", // 34
    "Arată erorile RDS", // 35
    "Limbă", // 36
    "Alegeți Limba", // 37
    "PI blocat pe sincronizare CT", // 38
    "Eroare! Fără semnal RDS", // 39
    "Lipsă zgomot FM", // 40
    "Lipsă zgomot AM", // 41
    "Beep la capătul benzii", // 42
    "Regiune", // 43
    "Europa", // 44
    "USA", // 45
    "Afișează subliniere la RDS", // 46
    "Doar PS", // 47
    "Wi-Fi activ", // 48
    "Configurare Wi-Fi", // 49
    "Conectare la: ", // 50
    "Ptr configurare Wi-Fi", // 51
    "Încerc conectarea la Wi-Fi", // 52
    "Fără succes!", // 53
    "Conectat!", // 54
    "Listă adrese IP", // 55
    "Arată lungimea de undă SW", // 56
    "Filtru RDS", // 57
    "Arată erorile PI", // 58
    "Folosește squelch", // 59
    "Contor modulație", // 60
    "Eliminator de zgomot AM", // 61
    "Eliminator de zgomot FM", // 62
    "Revenire la setările de fabrică", // 63
    "Ieșire Audio", // 64
    "Permite reglarea în timp ce ecranul este oprit", // 65
    "Ecran inversat", // 66
    "Despre software", // 67
    "Cod principal:", // 68
    "Contribuitori:", // 69
    "Necunoscut", // 70
    "Opțiuni economizor ecran", // 71
    "Pauză temporizator", // 72
    "Ecran oprit", // 73
    "Temă", // 74
    "Mod vizualizare", // 75
    "Aplicații detectate", // 76
    "Min.", // 77
    "Viteză SPI", // 78
    "Sensibilitatea scanării", // 79
    "NICI UNUL", // 80
    "Versiune Software", // 81
    "Font Frecvență", // 82
    "Auto", // 83
    "AF nu este disponibil", // 84
    "EON nu este disponibil", // 85
    "RT+ nu este disponibil", // 86
    "Mărime pași FM", // 78
    "Screensaver", // 79
    "Sec", // 80
    "Mod ceas", // 81
    "Luminozitate 1%", // 82
    "Luminozitate 25%", // 83
    "Luminozitate 50%", // 84
    "Decalaj de nivel AM RF", // 85
    "Unitați S-metru", // 86
    "Folosește AF", // 87
    "Selectare bandă FM", // 97
    "Selectare bandă AM", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Rotire când ecranul este oprit", // 104
    "Selectare Model", // 105
    "Bază", // 106
    "Portabil", // 107
    "Portabil touch", // 108
    "Titlu", // 109
    "Album", // 110
    "Bandă", // 111
    "Artist", // 112
    "Compoziție", // 113
    "Mișcare", // 114
    "Dirijor", // 115
    "Compozitor", // 116
    "Formație", // 117
    "Comentarii", // 118
    "Gen", // 119
    "Știri", // 120
    "Știri Locale", // 121
    "Bursa de Valori", // 122
    "Sport", // 123
    "Loto", // 124
    "Horoscop", // 125
    "Info zilnice", // 126
    "Sănătate", // 127
    "Evenimente", // 128
    "Teatru", // 129
    "Cinema", // 130
    "TV", // 131
    "Data/ora", // 132
    "Meteo", // 133
    "Trafic", // 134
    "Alarmă", // 135
    "Reclame", // 136
    "Website", // 137
    "Altele", // 138
    "PS scurt", // 139
    "PS lung", // 140
    "Acum", // 141
    "Urmează", // 142
    "Parte", // 143
    "Gazdă", // 144
    "Editor", // 145
    "Frecvența", // 146
    "Pagina start", // 147
    "Sub canal", // 148
    "Linia fierbinte", // 149
    "Telefon Studio", // 150
    "Telefon", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email Linie fierbinte", // 154
    "Email studio", // 155
    "Email", // 156
    "Discuții", // 157
    "Centru discuții", // 158
    "Întrebare la vot", // 159
    "Centru votare", // 160
    "Loc", // 161
    "Întâlnire", // 162
    "Identificare", // 163
    "Cumpără", // 164
    "Preiau date", // 165
    "Pași MW impliciți", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Opțiuni Baterie", // 170
    "Arată tensiunea", // 171
    "Arată procente", // 172
    "Porțiune RT", // 173
    "Setări de bază", // 174
    "Setări Audio", // 175
    "Setări ecran", // 176
    "Setări RDS", // 177
    "Setări FM", // 178
    "Setări AM", // 179
    "Conectivitate și ceas", // 180
    "APĂSAȚI MODE PENTRU REVENIRE", // 181
    "Detector CoChannel", // 182
    "Detecție sensibilitate CoChannel", // 183
    "Detecție prag CoChannel", // 184
    "Detector contor CoChannel", // 185
    "PRINCIPAL", // 186
    "AUDIO", // 187
    "ECRAN", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CONECTARE", // 192
    "MOD DX", // 193
    "AUTO MEM", // 194
    "AGC AM", // 195
    "FM deemphasis", // 196
    "Mic", // 197
    "Mare", // 198
    "Tot", // 199
    "PS rapid", // 200
    "Mod implicit", // 201
    "Ecran oprit, conectat la XDRGTK", // 202
    "FMSI stereo îmbunătățit", // 203
    "Funcție activă doar la\nTEF6687/6689!", // 204
    "Funcție indisponibilă\nla acest skin!", // 205
    "Scanare începută\nmemorare posturi", // 206
    "Scanare terminată\nmemorare posturi", // 207
    "Așteaptă", // 208
    "Începe scanarea DX", // 209
    "FM DX Opțiuni", // 210
    "Terminat! Programul de pornire\neste ignorat.", // 211
    "Sortare automată AF", // 212
    "Memorare programe", // 213
    "Exclude", // 214
    "Doar", // 215
    "Auto oprire scanare", // 216
    "PI corect", // 217
    "Semnal", // 218
    "Audio oprit\ncând scanează", // 219
    "AIR", // 220
    "Apasă 5sec. butonul bandă", // 221
    "Oprit", // 222
    "Ecran oprit", // 223
    "Dezactivat", // 224
    "Nedefinit", // 225
    "Știri", // 226
    "Actualități", // 227
    "Informații", // 228
    "Sport", // 229
    "Educație", // 230
    "Teatru", // 231
    "Cultură", // 232
    "Știință", // 233
    "Varietăți", // 234
    "Muzică pop", // 235
    "Muzică rock", // 236
    "Muzică ușoară", // 237
    "Clasic Lejer", // 238
    "Muzică Clasică", // 239
    "Altă Muzică", // 240
    "Meteo", // 241
    "Finanțe", // 242
    "Copii", // 243
    "Social", // 244
    "Religie", // 245
    "Phone-In", // 246
    "Călătorii", // 247
    "Timp Liber", // 248
    "Muzică de Jazz", // 249
    "Muzică Country", // 250
    "Muzică Națională", // 251
    "Muzica Retro", // 252
    "Muzică Populară", // 253
    "Documentar", // 254
    "Test Alarmă", // 255
    "Alarmă!!!", // 256
    "Inițial", // 257
    "Mereu", // 258
    "Arată ceasul", // 259
    "Arată PS lung", // 260
    "Frecvență de început", // 261
    "Frecvență de oprire", // 262
    "Canal de memorie de început", // 263
    "Canal de memorie de oprire", // 264
    "Stochează doar cu RDS", // 265
    "Începe memoria automată", // 266
    "Memorie automată", // 267
    "Listă ID posturi", // 268
    "Găsit", // 269
    "Anulat! Memoria canalelor este plină", // 270
    "Anulat! Utilizatorul a apăsat un buton", // 271
    "Scanarea s-a terminat fără erori", // 272
    "Șterge canalele de memorie", // 273
    "Șters", // 274
    "Previne dublarea PI", // 275
    "Interval", // 276
    "Complet", // 277
    "Timp de așteptare doar\npe semnal", // 278
    "Apasă colțul evidențiat", // 279
    "Pentru calibrarea ecranului tactil", // 280
    "Ține apăsat BW pentru\noprirea scannerului DX", // 281
    "Selectează lățimea de bandă", // 282
    "Jurnalul tău", // 283
    "Descarcă jurnalul", // 284
    "Jurnalul este gol", // 285
    "Du-te la final", // 286
    "Adăugat în jurnal", // 287
    "Eroare la adăugarea\nîn jurnal", // 288
    "Șterge jurnalul", // 289
    "Jurnal șters", // 290
    "Ștergerea a eșuat", // 291
    "Nu se poate adăuga!\nJurnalul este plin!", // 292
    "Autologger", // 293
    "Auto DST pe ora NTP", // 294
    "Setează offsetul pentru ora NTP", // 295
    "Jurnalul conține peste 130 de intrări, pe care vizualizatorul nu le poate procesa. Te rog descarcă fișierul CSV pentru a le procesa.", // 296
    "Canada" // 297
  },

  { "Deutsch", // German
    "Richtung des Drehimpuls-\ngebers wird umgekehrt", // 1
    "Bitte Taste loslassen", // 2
    "Bildschirm wird\num 180° gedreht", // 3
    "Analogmeter\nwird kalibrieret", // 4
    "USB-Modus", // 5
    "Tuner: !Keiner!", // 6
    "Lautstärke", // 7
    "Konverter", // 8
    "Unteres Bandende", // 9
    "Oberes Bandende", // 10
    "UKW RF-Pegeloffset", // 11
    "Stereo-Trennschwelle", // 12
    "Tiefpass-Eckfrequenz", // 13
    "Tiefpass-Schwelle", // 14
    "Low-Level-Schwelle", // 15
    "Bildschirmhelligkeit", // 16
    "Lautstärke einstellen", // 17
    "Konverteroffset", // 18
    "Unteres Bandende", // 19
    "Oberes Bandende", // 20
    "Pegeloffset", // 21
    "Stereo-Trennschwelle", // 22
    "Tiefpass-Eckfrequenz", // 23
    "Tiefpass-Schwelle", // 24
    "Low-Level-Schwelle", // 25
    "Bildschirmhelligkeit", // 26
    "Aus", // 27
    "Ein", // 28
    "DRÜCKE MODE ZUM VERLASSEN & SPEICHERN", // 29
    "DRÜCKE BAND ZUM SCHLIESSEN", // 30
    "SCANVORGANG\nLÄUFT...", // 31
    "Tuner nicht erkannt", // 32
    "AM Antennengewinn", // 33
    "UKW AGC", // 34
    "Zeige RDS-Fehler", // 35
    "Sprache", // 36
    "Sprache wählen", // 37
    "PI-Vorgabe für\nCT-Synchronisation", // 38
    "Fehler! Kein RDS-Signal", // 39
    "Sanfte\nStummschaltung UKW", // 40
    "Sanfte\nStummschaltung AM", // 41
    "Piepston am Bandende", // 42
    "Region", // 43
    "Europa", // 44
    "USA", // 45
    "Zeige Unterstrich im RDS", // 46
    "Nur PS", // 47
    "WLAN aktiv", // 48
    "WLAN konfigurieren", // 49
    "Verbinde mit", // 50
    "um WLAN einzustellen", // 51
    "Verbindungsversuch", // 52
    "Fehler!", // 53
    "Verbunden!", // 54
    "Stationlist Rechner-IP", // 55
    "KW-Wellenlänge\nanzeigen", // 56
    "RDS-Filter", // 57
    "Zeige PI-Fehler", // 58
    "Verwende Rauschsperre", // 59
    "Audiopegelanzeige", // 60
    "Rauschunterdrückung", // 61
    "Rauschunterdrückung", // 62
    "Werkseinstellungen\nwurden geladen", // 63
    "Audioausgabe", // 64
    "Bei Bildschirm aus\nTunen erlauben", // 65
    "Anzeigefarben umkehren", // 66
    "Über diese Software", // 67
    "Hauptcode:", // 68
    "Mitwirkende:", // 69
    "Unbekannt", // 70
    "Bildschirmschoner-\nOptionen", // 71
    "Abschaltzeit", // 72
    "Bildschirm aus", // 73
    "Thema", // 74
    "Oberfläche", // 75
    "Erkannte Applikationen", // 76
    "Min.", // 77
    "SPI-Geschw.", // 78
    "Scanempfindlichkeit", // 79
    "KEINE", // 80
    "Softwareversion", // 81
    "Frequenz-Schriftart", // 82
    "Auto", // 83
    "Keine AF verfügbar", // 84
    "Kein EON verfügbar", // 85
    "Kein RT+ verfügbar", // 86
    "UKW\nStandard-Schrittweite", // 78
    "Bildschirmschoner", // 79
    "Sek.", // 80
    "Uhrzeit-Modus", // 81
    "1% Helligkeit", // 82
    "25% Helligkeit", // 83
    "50% Helligkeit", // 84
    "AM RF-Pegeloffset", // 85
    "Einheit des Signalpegels", // 86
    "Verwende AF", // 87
    "Wähle UKW-Band", // 97
    "Wähle AM-Band", // 98
    "LW", // 99
    "MW", // 100
    "KW", // 101
    "UKW", // 102
    "OIRT", // 103
    "Bei Bildschirm aus Tunen erlauben", // 104
    "Modell auswählen", // 105
    "Basismodell", // 106
    "Portabel", // 107
    "Portabel Touch", // 108
    "Titel", // 109
    "Album", // 110
    "Track", // 111
    "Künstler", // 112
    "Komposition", // 113
    "Satz", // 114
    "Dirigent", // 115
    "Komponist", // 116
    "Band", // 117
    "Kommentar", // 118
    "Genre", // 119
    "Nachrichten", // 120
    "Lokalnachrichten", // 121
    "Aktienmarkt", // 122
    "Sport", // 123
    "Lotterie", // 124
    "Horoskop", // 125
    "Tagestipp", // 126
    "Gesundheit", // 127
    "Ereignis", // 128
    "Szene", // 129
    "Kino", // 130
    "TV", // 131
    "Tag/Uhrzeit", // 132
    "Wetter", // 133
    "Verkehr", // 134
    "Alarm", // 135
    "Werbung", // 136
    "Website", // 137
    "Info", // 138
    "Kurzes PS", // 139
    "Langes PS", // 140
    "Es läuft", // 141
    "Es folgt", // 142
    "Thema", // 143
    "Am Mikrofon", // 144
    "Redaktion", // 145
    "Frequenz", // 146
    "Homepage", // 147
    "Subchannel", // 148
    "Telefon Hotline", // 149
    "Telefon Studio", // 150
    "Telefon", // 151
    "SMS Studio", // 152
    "SMS", // 153
    "E-Mail Hotline", // 154
    "E-Mail Studio", // 155
    "E-Mail", // 156
    "Chat", // 157
    "Chat-Zentrale", // 158
    "Abstimmung", // 159
    "Abstimmungs-Zentrale", // 160
    "Ort", // 161
    "Termin", // 162
    "Musik-ID", // 163
    "Bezugsquelle", // 164
    "Empfange Daten", // 165
    "MW\nStandardschrittweite", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Akkuoptionen", // 170
    "Spannung anzeigen", // 171
    "Prozente anzeigen", // 172
    "RT puffern", // 173
    "Haupteinstellungen", // 174
    "Audioeinstellungen", // 175
    "Anzeigeeinstellungen", // 176
    "RDS-Einstellungen", // 177
    "UKW-Einstellungen", // 178
    "AM-Einstellungen", // 179
    "Verbindungen und Uhrzeit", // 180
    "DRÜCKE MODE FÜR ZURÜCK", // 181
    "Gleichkanalerkennung", // 182
    "Gleichkanalerkennung\nEmpfindlichkeit", // 183
    "Gleichkanalerkennung\nSchwelle", // 184
    "Gleichkanalerkennung\nZähler", // 185
    "HAUPT", // 186
    "AUDIO", // 187
    "ANZEIGE", // 188
    "RDS", // 189
    "UKW", // 190
    "AM", // 191
    "VERBIND./UHR", // 192
    "DX-MODUS", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "Entzerrung auf UKW", // 196
    "Wenig", // 197
    "Viel", // 198
    "Maximal", // 199
    "Schnelles PS", // 200
    "Standard", // 201
    "Bildschirm aus bei\nXDRGTK-Verbindung", // 202
    "FMSI Stereo-\nVerbesserung", // 203
    "Funktion nur für\nTEF6687/6689 verfügbar!", // 204
    "Funktion für ausgewählten\nSkin nicht verfügbar!", // 205
    "Starte Scan mit\nSpeicherplatz", // 206
    "Stoppe Scan mit\nSpeicherplatz", // 207
    "Wartezeit", // 208
    "Starte DX-Scan", // 209
    "UKW DX-Optionen", // 210
    "Abbruch! Start-Speicher-\nplatz ist nicht belegt", // 211
    "AF Autosortierung", // 212
    "Speicherplätze scannen", // 213
    "Nein", // 214
    "Ja, ausschließlich", // 215
    "Automatisches\nScan-Ende", // 216
    "Korrekter PI-Code", // 217
    "Signal", // 218
    "Stumm beim Scannen", // 219
    "AIR", // 220
    "BAND-Knopf\n5 Sek. drücken", // 221
    "Ausschalten", // 222
    "Bildschirmschoner", // 223
    "Deaktiviert", // 224
    "Nicht definiert", // 225
    "Nachrichten", // 226
    "Aktuelle Info", // 227
    "Service Programm", // 228
    "Sport", // 229
    "Bildung", // 230
    "Hörspiel + Lit", // 231
    "Kultur+Gesellsch", // 232
    "Wissenschaft", // 233
    "Unterhaltung", // 234
    "Pop Musik", // 235
    "Rock Musik", // 236
    "Unterhalt Musik", // 237
    "Leichte Klassik", // 238
    "Ernste Klassik", // 239
    "Spezielle Musik", // 240
    "Wetter", // 241
    "Wirtschaft", // 242
    "Kinderprogramm", // 243
    "Soziales", // 244
    "Religion", // 245
    "Anrufsendung", // 246
    "Reiseinformation", // 247
    "Freizeit", // 248
    "Jazz", // 249
    "Country Musik", // 250
    "Landesmusik", // 251
    "Oldies Musik", // 252
    "Folklore", // 253
    "Feature", // 254
    "Alarm Test", // 255
    "Alarm! Alarm!", // 256
    "Anfänglich", // 257
    "Immer", // 258
    "Uhr anzeigen", // 259
    "Zeige langes PS", // 260
    "Start-Frequenz", // 261
    "Stopp-Frequenz", // 262
    "Start-Speicherplatz", // 263
    "Stopp-Speicherplatz", // 264
    "Speichern nur mit RDS", // 265
    "Starte Auto-Speichern", // 266
    "Auto-Speichern", // 267
    "Stationlist-ID", // 268
    "gefunden", // 269
    "Abbruch! Speicherplätze voll", // 270
    "Abbruch! Taste wurde gedrückt", // 271
    "Scan ohne Fehler beendet", // 272
    "Lösche Speicherplätze", // 273
    "gelöscht", // 274
    "Doppelte PI verhindern", // 275
    "Bereich", // 276
    "Voll", // 277
    "Wartezeit nur wenn\nSignal vorhanden", // 278
    "Markierte Ecke drücken", // 279
    "zum Kalibrieren des Touchscrees", // 280
    "Zum Stoppen des DX-Scans\nTaste BW lange drücken", // 281
    "Bandbreite wählen", // 282
    "Dein Logbuch", // 283
    "Logbuch herunterladen", // 284
    "Logbuch ist leer", // 285
    "Zum Ende gehen", // 286
    "Logbuch-Eintrag\nwurde erstellt", // 287
    "Logbuch-Eintrag\nist fehlgeschlagen", // 288
    "Logbuch löschen", // 289
    "Logbuch gelöscht", // 290
    "Löschen fehlgeschlagen", // 291
    "Logbucheintrag nicht möglich\nLogbuch ist voll!", // 292
    "Automatisch loggen", // 293
    "Auto-Sommerzeit\nüber NTP", // 294
    "NTP-Zeitversatz\neinstellen", // 295
    "Das Logbuch enthält mehr als 130 Einträge, was das Anzeigeprogramm nicht verarbeiten kann. Bitte lade die CSV-Datei zum Verarbeiten herunter.", // 296
    "Kanada" // 297
  },

  { "Čeština", // Czech
    "Směr enkodéru byl změněn", // 1
    "Prosím, pusťte tlačítko", // 2
    "Obrazovka otočena", // 3
    "Kalibrace analog. měřiče", // 4
    "Režim USB", // 5
    "Tuner: !ŽÁDNÝ!", // 6
    "Hlasitost", // 7
    "Konvertor", // 8
    "Spodek pásma", // 9
    "Vrch pásma", // 10
    "FM offset signálu", // 11
    "Práh pro stereo", // 12
    "Práh frekvence zvuku k utlumení", // 13
    "Práh signálu pro utlumení výšek", // 14
    "Práh nízkého signálu", // 15
    "Jas", // 16
    "Nastavení hlasitosti", // 17
    "Offset frekvence", // 18
    "Spodek pásma", // 19
    "Vrch pásma", // 20
    "Offset signálu", // 21
    "Práh pro stereo", // 22
    "Práh frekvence zvuku k utlumení", // 23
    "Práh signálu pro utlumení výšek", // 24
    "Práh nízkého signálu", // 25
    "Jas obrazovky", // 26
    "Vyp.", // 27
    "Zap.", // 28
    "ZMÁČKNĚTE MODE PRO UKONČENÍ A ULOŽENÍ", // 29
    "ZMÁČKNĚTE BAND PRO ZAVŘENÍ", // 30
    "SKENUJI...", // 31
    "Tuner nedetekován", // 32
    "Zisk AM antény", // 33
    "FM AGC", // 34
    "Zobrazit chyby v RDS", // 35
    "Jazyk", // 36
    "Vyberte jazyk", // 37
    "Synchronizovat CT,\nkdyž je PI uzamčeno", // 38
    "Chyba! Bez RDS!", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Pípnout na konci pásma", // 42
    "Region", // 43
    "Evropa", // 44
    "USA", // 45
    "Zobrazit podtržítka v RDS", // 46
    "Jen PS", // 47
    "Wi-Fi aktivní", // 48
    "Konfigurace Wi-Fi", // 49
    "Připojte se k: ", // 50
    "pro nakonfigurování Wi-Fi", // 51
    "Wi-Fi připojení probíhá...", // 52
    "NEPŘIPOJENO!", // 53
    "PŘIPOJENO!", // 54
    "Stationlist IP", // 55
    "Zobrazit SW pásmo", // 56
    "RDS filtr", // 57
    "Zobrazit chyby v PI", // 58
    "Použít squelch", // 59
    "Graf modulace", // 60
    "Tlumič šumu (AM)", // 61
    "Tlumič šumu (FM)", // 62
    "Výchozí hodnoty načteny", // 63
    "Zvukový výstup", // 64
    "Povolit ladění přes spořič", // 65
    "Invertování barev", // 66
    "O softwaru", // 67
    "Hlavní kód:", // 68
    "Přispěli:", // 69
    "Neznámý", // 70
    "Nastavení\nspořiče obrazovky", // 71
    "Časovač vypnutí", // 72
    "Vypnutí obrazovky", // 73
    "Vzhled", // 74
    "Skin", // 75
    "Detekované aplikace", // 76
    "minut", // 77
    "Rychlost SPI", // 78
    "Citlivost skenu", // 79
    "žádné", // 80
    "Verze softwaru", // 81
    "Font frekvence", // 82
    "Auto", // 83
    "AF nedostupné", // 84
    "EON nedostupný", // 85
    "RT+ nedostupný", // 86
    "Výchozí krok ladění FM", // 78
    "Spořič obrazovky", // 79
    "sec.", // 80
    "Režim hodin", // 81
    "1% jas", // 82
    "25% jas", // 83
    "50% jas", // 84
    "AM offset signálu", // 85
    "Jednotka signálu", // 86
    "Použijte AF", // 87
    "Zvolte FM pásmo", // 97
    "Zvolte AM pásmo", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Ladění při vypnuté obrazovce", // 104
    "Volba modelu", // 105
    "Základní", // 106
    "Přenosný", // 107
    "Přenosný-dotykový(ILI9341)", // 108
    "Název", // 109
    "Album", // 110
    "Stopa", // 111
    "Interpret", // 112
    "Kompozice", // 113
    "Věta", // 114
    "Dirigent", // 115
    "Skladatel", // 116
    "Kapela", // 117
    "Komentář", // 118
    "Žánr", // 119
    "Zprávy", // 120
    "Místní zprávy", // 121
    "Burza", // 122
    "Sport", // 123
    "Loterie", // 124
    "Horoskop", // 125
    "Každodenní zábava", // 126
    "Zdraví", // 127
    "Událost", // 128
    "Scéna", // 129
    "Kino", // 130
    "Televize", // 131
    "Datum/čas", // 132
    "Počasí", // 133
    "Doprava", // 134
    "Alarm", // 135
    "Reklama", // 136
    "Webová stránka", // 137
    "Ostatní", // 138
    "Short PS", // 139
    "Long PS", // 140
    "Nyní", // 141
    "Následuje", // 142
    "Část", // 143
    "Moderátor", // 144
    "Redaktor", // 145
    "Frekvence", // 146
    "Hlavní stránka", // 147
    "Podkanál", // 148
    "Horká linka", // 149
    "Telefon do studia", // 150
    "Telefon", // 151
    "SMS do studia", // 152
    "SMS", // 153
    "Emailová hotline", // 154
    "Email do studia", // 155
    "Email", // 156
    "Chat", // 157
    "Centrum chatu", // 158
    "Hlasovací otázka", // 159
    "Hlasovací centrum", // 160
    "Místo", // 161
    "Schůzka", // 162
    "Identifikátor", // 163
    "Nákup", // 164
    "Získat data", // 165
    "Výchozí krok ladění MW", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Úroveň nabití baterie", // 170
    "Zobrazit napětí", // 171
    "Zobrazit procenta", // 172
    "RT buffering", // 173
    "Hlavní nastavení", // 174
    "Nastavení zvuku", // 175
    "Nastavení displeje", // 176
    "Nastavení RDS", // 177
    "Nastavení FM pásma", // 178
    "Nastavení AM pásma", // 179
    "Konektivita a hodiny", // 180
    "ZMÁČKNĚTE MODE PRO NÁVRAT", // 181
    "AM CoChannelDet", // 182
    "Citlivost CoChannelDet", // 183
    "Práh AM CoChannelDet", // 184
    "CoChannelDet počítadlo", // 185
    "HLAVNÍ", // 186
    "ZVUK", // 187
    "DISPLEJ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "KONEKTIVITA", // 192
    "DX REŽIM", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM deemfáze", // 196
    "Malé", // 197
    "Velké", // 198
    "Vše", // 199
    "Rychlé načtení PS", // 200
    "Výchozí", // 201
    "Vypnout displej při\nXDRGTK připojení", // 202
    "FMSI stereo\nvylepšení", // 203
    "Funkce dostupná pouze\nu TEF6687/6689!", // 204
    "Funkce není dostupná\nu zvoleného skinu!", // 205
    "Začít skenování\nod předvolby č.", // 206
    "Skončit skenování\nu předvolby č.", // 207
    "Doba čekání", // 208
    "Spustit DX skenování", // 209
    "Nastavení FM DX", // 210
    "Přerušeno! Začáteční předvolba\nje označena jako prázdná.", // 211
    "Auto řazení AF", // 212
    "Předvolby paměti", // 213
    "Vyloučit", // 214
    "Pouze", // 215
    "Automaticky zastavit\nskenování", // 216
    "Korektní PI", // 217
    "Signál", // 218
    "Ztlumit zvuk\nběhem skenování", // 219
    "AIR", // 220
    "Po 5 sec. stisknutí\ntl. BAND", // 221
    "Vypnout", // 222
    "Vypnout displej", // 223
    "Zakázáno", // 224
    "Nedefinovaný", // 225
    "Zprávy", // 226
    "Publicistika", // 227
    "Informace", // 228
    "Sport", // 229
    "Vzdělání", // 230
    "Drama", // 231
    "Kultura", // 232
    "Věda", // 233
    "Různé žánry", // 234
    "Popová hudba", // 235
    "Rocková hudba", // 236
    "Oddychová hudba", // 237
    "Klasická hudba", // 238
    "Vážná hudba", // 239
    "Jiná hudba", // 240
    "Počasí", // 241
    "Finance", // 242
    "Dětský program", // 243
    "Společenská tém.", // 244
    "Naboženství", // 245
    "Telefonáty", // 246
    "Cestování", // 247
    "Zábava", // 248
    "Jazzová hudba", // 249
    "Country hudba", // 250
    "Národní hudba", // 251
    "Oldies hudba", // 252
    "Folková hudba", // 253
    "Dokumenty", // 254
    "Alarm Test", // 255
    "Alarm – Alarm !", // 256
    "Počáteční", // 257
    "Vždy", // 258
    "Zobrazit hodiny", // 259
    "Zobrazit long PS", // 260
    "Počáteční frekvence", // 261
    "Poslední frekvence", // 262
    "Počáteční paměťový kanál", // 263
    "Poslední paměťový kanál", // 264
    "Ukládat jen s RDS", // 265
    "Spustit automatickou paměť", // 266
    "Automatická paměť", // 267
    "ID seznamu stanic", // 268
    "Nalezeno", // 269
    "Přerušeno! Nedostatek paměťových kanálů", // 270
    "Přerušeno! Uživatel stiskl tlačítko", // 271
    "Skenování dokončen bez chyb", // 272
    "Vymazat paměťové kanály", // 273
    "vymazáno", // 274
    "Zamezit dvojitému PI", // 275
    "Rozsah", // 276
    "Plný", // 277
    "Čekací doba pouze na\nsignal", // 278
    "Stiskněte zvýrazněný roh", // 279
    "Pro kalibraci dotekové obrazovky", // 280
    "Pro zastavení\npodržte tlačítko BW", // 281
    "Vyberte šířku pásma", // 282
    "Váš logbook", // 283
    "Stáhnout logbook", // 284
    "Logbook je prázdný", // 285
    "Přejít na konec", // 286
    "Přidáno do logu", // 287
    "Chyba při přidávání\npoložky do logbooku", // 288
    "Vymazat logbook", // 289
    "Logbook vymazán", // 290
    "Vymazání se nezdařilo", // 291
    "Nelze přidat položku!\nLogbook je plný!", // 292
    "Automatický logger", // 293
    "Auto DST u NTP času", // 294
    "Nastavte časový posun NTP", // 295
    "Logbook obsahuje více než 130 položek, které prohlížeč nedokáže zpracovat. Pro zpracování všech položek si stáhněte CSV soubor.", // 296
    "Canada" // 297
  },

  { "Magyar", // Hungarian
    "Forgási irány megváltozott", // 1
    "Engedd el a gombot", // 2
    "Képernyő megfordítva", // 3
    "Analóg mérő kalibrálása", // 4
    "USB mód", // 5
    "Tuner: !Nincs!", // 6
    "Hangerő", // 7
    "Átalakító", // 8
    "Legkisebb sávszél", // 9
    "Legnagyobb sávszél", // 10
    "FM RF szinteltolás", // 11
    "Sztereó küszöbérték", // 12
    "Magasak sarokja", // 13
    "Magasak küszöbértéke", // 14
    "Alacsonyak küszöbértéke", // 15
    "Kijelző fényereje", // 16
    "Hangerő beállítása", // 17
    "Átalakító eltolása", // 18
    "Legkisebb sávszél", // 19
    "Legnagyobb sávszél", // 20
    "Szinteltolás", // 21
    "Sztereó szétválasztás küszöbértéke", // 22
    "Magasak sarokfrekvenciája", // 23
    "Magasak küszöbértéke", // 24
    "Alacsony szint küszöbérték", // 25
    "Kijelző fényereje", // 26
    "Ki", // 27
    "Be", // 28
    "MODE A MENTÉSHEZ ÉS BEZÁRÁSHOZ", // 29
    "BAND A BEZÁRÁSHOZ", // 30
    "KERESÉS...", // 31
    "Tuner nem észlelhető", // 32
    "AM Antenna érzékenység", // 33
    "FM AGC", // 34
    "RDS hibák megjelenítése", // 35
    "Nyelv", // 36
    "Válassz nyelvet", // 37
    "PI zárolt\nCT szinkronizálás", // 38
    "Hiba! Nincs RDS jel", // 39
    "FM halkítás", // 40
    "AM halkítás", // 41
    "Sípszó a sáv szélén", // 42
    "Régió", // 43
    "Európa", // 44
    "USA", // 45
    "Aláhúzások megjelenítése\nRDS-ben", // 46
    "Csak PS", // 47
    "Wi-Fi", // 48
    "Wi-Fi beállítása", // 49
    "Csatlakozz ehhez: ", // 50
    "a Wi-Fi beállításához", // 51
    "Wi-Fi kapcsolódási\nkísérlet", // 52
    "SIKERTELEN!", // 53
    "CSATLAKOZVA!", // 54
    "Stationlist kliens IP", // 55
    "SW hullámhossz\nmegjelenítése", // 56
    "RDS szűrő", // 57
    "PI hibák megjelenítése", // 58
    "Zajzár(SQ) használata", // 59
    "Modulációmérő", // 60
    "AM Zajszűrő", // 61
    "FM Zajszűrő", // 62
    "Alapértelmezettek\nbetöltve", // 63
    "Audió kimenet", // 64
    "Hangolás\nképernyővédővel", // 65
    "Inverz kijelző", // 66
    "A szoftverről", // 67
    "Fő kód:", // 68
    "Közreműködők:", // 69
    "Ismeretlen", // 70
    "Képernyővédő opciók", // 71
    "Időtúllépés időzítő", // 72
    "Kijelző ki", // 73
    "Téma", // 74
    "Kinézet", // 75
    "Felismert alkalmazások", // 76
    "Perc", // 77
    "SPI sebesség", // 78
    "Keresés érzékenység", // 79
    "NINCS", // 80
    "Szoftver verzió", // 81
    "Frekvencia betűtípusa", // 82
    "Auto", // 83
    "Nincs elérhető AF", // 84
    "Nincs elérhető EON", // 85
    "Nincs elérhető RT+", // 86
    "FM alap. lépésköz", // 78
    "Képernyővédő", // 79
    "Mp", // 80
    "Óra mód", // 81
    "1% fényerő", // 82
    "25% fényerő", // 83
    "50% fényerő", // 84
    "AM RF szinteltolás", // 85
    "Jelmérő egység", // 86
    "AF használata", // 87
    "FM sáv kiválasztása", // 97
    "AM sáv kiválasztása", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Hangolás kikapcsolt képernyővel", // 104
    "Modellválasztó", // 105
    "Alap", // 106
    "Hordozható", // 107
    "Hord. érintős", // 108
    "Cím", // 109
    "Album", // 110
    "Szám", // 111
    "Előadó", // 112
    "Kompozíció", // 113
    "Tétel", // 114
    "Karmester", // 115
    "Zeneszerző", // 116
    "Zenekar", // 117
    "Megjegyzés", // 118
    "Műfaj", // 119
    "Hírek", // 120
    "Helyi hírek", // 121
    "Tőzsdepiac", // 122
    "Sport", // 123
    "Lottó", // 124
    "Horoszkóp", // 125
    "Napi kikapcsolódás", // 126
    "Egészség", // 127
    "Esemény", // 128
    "Jelenet", // 129
    "Mozi", // 130
    "TV", // 131
    "Dátum/idő", // 132
    "Időjárás", // 133
    "Közlekedés", // 134
    "Riasztás", // 135
    "Hirdetés", // 136
    "Weboldal", // 137
    "Egyéb", // 138
    "Rövid PS", // 139
    "Hosszú PS", // 140
    "Most", // 141
    "Következő", // 142
    "Rész", // 143
    "Házigazda", // 144
    "Szerkesztő", // 145
    "Frekvencia", // 146
    "Kezdőlap", // 147
    "Alcsatorna", // 148
    "Forródrót", // 149
    "Stúdió telefon", // 150
    "Telefon", // 151
    "Stúdió SMS", // 152
    "SMS", // 153
    "Email forródrót", // 154
    "Stúdió Email", // 155
    "Email", // 156
    "Csevegés", // 157
    "Csevegőközpont", // 158
    "Szavazási kérdés", // 159
    "Szavazóközpont", // 160
    "Helyszín", // 161
    "Találkozó", // 162
    "Azonosító", // 163
    "Vásárlás", // 164
    "Adatok lekérése", // 165
    "MW alap. lépésköz", // 166
    "Tangerin", // 167
    "Óceán", // 168
    "Indigó", // 169
    "Akkumulátor jelzése", // 170
    "Feszültség mutatása", // 171
    "Százalék mutatása", // 172
    "RT pufferelés", // 173
    "Fő beállítások", // 174
    "Audió beállítások", // 175
    "Kijelző beállítások", // 176
    "RDS beállítások", // 177
    "FM beállítások", // 178
    "AM beállítások", // 179
    "Csatlakoztathatóság és óra", // 180
    "MODE A VISSZALÉPÉSHEZ", // 181
    "CoChannel Detektor", // 182
    "CoChannel Detektor\nérzékenység", // 183
    "CoChannel Detektor\nküszöbérték", // 184
    "CoChannel Detektor\nszámláló", // 185
    "FŐ", // 186
    "AUDIÓ", // 187
    "KIJELZŐ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CSATLAKOZÁS", // 192
    "DX MÓD", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM kihangolás", // 196
    "Kicsi", // 197
    "Nagy", // 198
    "Mind", // 199
    "Gyors PS", // 200
    "Alap", // 201
    "Képernyő némítása\nXDRGTK kapcsolaton", // 202
    "FMSI sztereó\njavítás", // 203
    "Funkció csak a\nTEF6687/6689-hez!", // 204
    "Funkció nem elérhető\nebben a kinézetben!", // 205
    "Mem. csat. szkennelésének\nelkezdése", // 206
    "Mem. csat. szkennelésének\nbefejezése", // 207
    "Várakozási idő", // 208
    "DX szkennelés indítása", // 209
    "FM DX Opciók", // 210
    "Megszakítva! A kezdőcsatorna\nkihagyottnak van jelölve.", // 211
    "Auto AF rendezés", // 212
    "Memóriacsatornák", // 213
    "Kizárása", // 214
    "Csak", // 215
    "Auto keresés\nmegszakítása", // 216
    "Jó PI esetén", // 217
    "Jel esetén", // 218
    "Hang némítása\nkeresés közben", // 219
    "AIR", // 220
    "5 mp. sávgombnyomás", // 221
    "Kikapcsolás", // 222
    "Képernyőnémítás", // 223
    "Letiltva", // 224
    "Nincs", // 225
    "Hírek", // 226
    "Aktuális események", // 227
    "Információ", // 228
    "Sport", // 229
    "Oktatás", // 230
    "Dráma", // 231
    "Kultúra", // 232
    "Tudomány", // 233
    "Változatos", // 234
    "Pop Zene", // 235
    "Rock Zene", // 236
    "Könnyűzene", // 237
    "Könnyű Klasszikus", // 238
    "Komoly Klasszikus", // 239
    "Egyéb Zene", // 240
    "Időjárás", // 241
    "Pénzügy", // 242
    "Gyermekprogramok", // 243
    "Társadalmi ügyek", // 244
    "Vallás", // 245
    "Betelefonálós", // 246
    "Utazás", // 247
    "Szabadidő", // 248
    "Jazz Zene", // 249
    "Country Zene", // 250
    "Nemzeti Zene", // 251
    "Régi Zene", // 252
    "Népi Zene", // 253
    "Dokumentumfilm", // 254
    "Riasztás Teszt", // 255
    "Riasztás!!!", // 256
    "Kezdeti", // 257
    "Mindig", // 258
    "Óra mutatása", // 259
    "Hosszú PS mutatása", // 260
    "Indítási frekvencia", // 261
    "Befejezési frekvencia", // 262
    "Memóriacsatorna\nmentés indítása", // 263
    "Memóriacsatorna\nmentés leállítása", // 264
    "Csak RDS-el tárolja", // 265
    "Auto memória indítása", // 266
    "Auto memória", // 267
    "Stationlist ID", // 268
    "Megtalálva", // 269
    "Megszakítva! Nincs\nmemóriacsatorna", // 270
    "Megszakítva! Felhasználó\nmegnyomta a gombot", // 271
    "Szkennelés hiba\nnélkül befejeződött", // 272
    "Memóriacsatornák törlése", // 273
    "törölve", // 274
    "Kettős PI\nmegakadályozása", // 275
    "Tartomány", // 276
    "Teljes", // 277
    "Várakozási idő\ncsak jel esetén", // 278
    "Nyomd meg a kiemelt sarkot", // 279
    "Érintőképernyő kalibrálásához", // 280
    "Tartsd lenyomva a\nBW-t a megállításhoz", // 281
    "Sávszélesség választása", // 282
    "A logbookod", // 283
    "Logbook letöltése", // 284
    "A logbook üres", // 285
    "Ugrás az aljára", // 286
    "Hozzáadva a loghoz", // 287
    "Hiba a naplóhoz adáskor", // 288
    "Logbook törlése", // 289
    "Logbook törölve", // 290
    "Sikertelen törlés", // 291
    "Nem adható hozzá!\nLogbook megtelt!", // 292
    "Auto logoló", // 293
    "Auto nyári idő(DST)\nNTP idő szerint", // 294
    "NTP időeltolás beállítása", // 295
    "A napló több mint 130 bejegyzést tartalmaz, amelyeket a megtekintő nem tud feldolgozni. Kérlek, töltsd le a CSV fájlt a megtekintéshez.", // 296
    "Kanada" // 297
  },

  { "Français", // French
    "Sens de rotation modifié", // 1
    "Veuillez relâcher\nle bouton", // 2
    "Écran inversé", // 3
    "Calibrer le mesureur\nanalogique", // 4
    "Mode USB", // 5
    "Tuner : Aucun !", // 6
    "Volume", // 7
    "Convertisseur", // 8
    "Limite de bande basse", // 9
    "Limite de bande haute", // 10
    "Décalage du niveau RF FM", // 11
    "Seuil minimum\npour la stéréo", // 12
    "Angle du filtre\nà coupe haute", // 13
    "Seuil du filtre\nà coupe haute", // 14
    "Seuil minimum de\nniveau bas", // 15
    "Luminosité", // 16
    "Réglage du volume", // 17
    "Décalage du convertisseur", // 18
    "Limite de bande basse", // 19
    "Limite de bande haute", // 20
    "Décalage du niveau", // 21
    "Seuil de séparation\nstéréo", // 22
    "Fréquence de coupe\nélevée", // 23
    "Seuil de coupe élevée", // 24
    "Seuil de niveau bas", // 25
    "Luminosité de l'affichage", // 26
    "Éteint", // 27
    "Allumé", // 28
    "BUTON MODE POUR SORTIR & SAUVEGARDER", // 29
    "APPUYEZ SUR BAND POUR FERMER", // 30
    "SCAN EN COURS...", // 31
    "Tuner non détecté", // 32
    "Gain de l'antenne AM", // 33
    "FM AGC", // 34
    "Afficher les erreurs\nde décodage RDS", // 35
    "Langage", // 36
    "Choisir la langue", // 37
    "Verrouillage de la\nsync. CT sur PI", // 38
    "Erreur ! Pas de signal RDS", // 39
    "Sourdine douce FM", // 40
    "Sourdine douce AM", // 41
    "Bip à la limite\nde bande", // 42
    "Région", // 43
    "Europe", // 44
    "États-Unis", // 45
    "Afficher les tirets du\nbas sur le PS", // 46
    "PS uniquement", // 47
    "Wi-Fi actif", // 48
    "Configuration du Wi-Fi", // 49
    "Se connecter au réseau", // 50
    "pour configurer\nle Wi-Fi", // 51
    "Tentative de connexion\nen mode Wi-Fi", // 52
    "CONNEXION IMPOSSIBLE...\nDésactivation du Wi-Fi", // 53
    "CONNECTÉ !", // 54
    "IP du client Stationlist", // 55
    "Afficher la longueur\nd'onde SW", // 56
    "Filtre RDS", // 57
    "Afficher les erreurs\nde décodage PI", // 58
    "Utiliser le squelch", // 59
    "Mesure de l'intensité sonore", // 60
    "Réducteur de bruit AM", // 61
    "Réducteur de bruit FM", // 62
    "Réglages par défaut\nchargés", // 63
    "Sortie audio", // 64
    "Scanner sans désactiver\nl'économiseur d'écran", // 65
    "Inverser les\ncouleurs de l'écran", // 66
    "À propos du logiciel", // 67
    "Code de base:", // 68
    "Contributeurs:", // 69
    "Inconnu", // 70
    "Options d'écran\nde veille", // 71
    "Arrêt automatique", // 72
    "Arrêt de l'afficheur", // 73
    "Thème", // 74
    "Mode d'affichage (Skin)", // 75
    "Applications détectées", // 76
    "Min.", // 77
    "Vitesse SPI", // 78
    "Sensibilité\ndu scan", // 79
    "AUCUN", // 80
    "Version logicielle", // 81
    "Police d'affichage\npour la fréquence", // 82
    "Auto", // 83
    "AF indisponible", // 84
    "EON indisponible", // 85
    "RT+ indisponible", // 86
    "Valeur de pas FM\npar défaut", // 78
    "Écran de veille", // 79
    "Sec", // 80
    "Mode horloge", // 81
    "Luminosité à 1%", // 82
    "Luminosité à 25%", // 83
    "Luminosité à 50%", // 84
    "Décalage du\nniveau RF AM", // 85
    "Unité de mesure\ndu signal", // 86
    "Utiliser la fonction AF", // 87
    "Sélectionner la\nbande FM", // 97
    "Sélectionner la\nbande AM", // 98
    "LW", // 99
    "AM", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Arrêter la sortie de veille\nlors du scan", // 104
    "Sélection du modèle", // 105
    "Base", // 106
    "Portable", // 107
    "Portable à écran tactile", // 108
    "Titre", // 109
    "Album", // 110
    "Piste", // 111
    "Artiste", // 112
    "Composition", // 113
    "Mouvement", // 114
    "Chef d'orchestre", // 115
    "Compositeur", // 116
    "Groupe", // 117
    "Commentaire", // 118
    "Genre", // 119
    "Actualités", // 120
    "Actualités locales", // 121
    "Bourse", // 122
    "Sport", // 123
    "Loterie", // 124
    "Horoscope", // 125
    "Divertissement quotidien", // 126
    "Santé", // 127
    "Événement", // 128
    "Scène", // 129
    "Cinéma", // 130
    "TV", // 131
    "Date/heure", // 132
    "Météo", // 133
    "Trafic", // 134
    "Alarme", // 135
    "Publicité", // 136
    "Site web", // 137
    "Autre", // 138
    "PS court", // 139
    "PS long", // 140
    "Maintenant", // 141
    "À suivre", // 142
    "Partie", // 143
    "Animateur", // 144
    "Rédacteur", // 145
    "Fréquence", // 146
    "Page d'accueil", // 147
    "Sous-canal", // 148
    "Hotline", // 149
    "Num. Téléphone Studio", // 150
    "Num. Téléphone", // 151
    "Num. SMS Studio", // 152
    "Numéro SMS", // 153
    "Email de la hotline", // 154
    "Email du studio", // 155
    "Email", // 156
    "Chat", // 157
    "Centre de discussion", // 158
    "Question de vote", // 159
    "Centre de vote", // 160
    "Lieu", // 161
    "Rendez-vous", // 162
    "Identifiant", // 163
    "Acheter", // 164
    "Obtenir des données", // 165
    "Valeur de pas AM\npar défaut", // 166
    "Mandarine", // 167
    "Océan", // 168
    "Indigo", // 169
    "Options de batterie", // 170
    "Montrer la tension", // 171
    "Montrer le pourcentage", // 172
    "Mise en mémoire tampon RT", // 173
    "Paramètres principaux", // 174
    "Paramètres audio", // 175
    "Paramètres d'affichage", // 176
    "Paramètres RDS", // 177
    "Paramètres FM", // 178
    "Paramètres AM", // 179
    "Connectivité et horloge", // 180
    "BOUTON MODE POUR REVENIR EN ARRIÈRE", // 181
    "Détecteur de canal adjacent AM", // 182
    "Sensibilité du détecteur\nde canal adjacent", // 183
    "Seuil du détecteur\nde canal adjacent", // 184
    "Compteurdu détecteur\nde canal adjacent", // 185
    "PRINCIPAL", // 186
    "AUDIO", // 187
    "AFFICHAGE", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CONNECTER", // 192
    "MODE DX", // 193
    "MEMORISATION AUTO", // 194
    "AGC AM", // 195
    "Désaccentuation FM", // 196
    "Petites", // 197
    "Grosses", // 198
    "Toutes", // 199
    "PS rapide", // 200
    "Défaut", // 201
    "Arrêt écran à la\nconnexion XDRGTK", // 202
    "Amélioration\nde la stéréo FMSI", // 203
    "Fonction disponible\nsur TEF6687/6689!", // 204
    "Fonction non disponible\nsur ce mode d'affichage !", // 205
    "Début du scan\ndes canaux mémorisés", // 206
    "Fin du scan\ndes canaux mémorisés", // 207
    "Temps d'attente", // 208
    "Démarrer le scan DX", // 209
    "Options FM DX", // 210
    "Abandon ! Le canal de\ndépart est indiqué comme ignoré.", // 211
    "Tri automatique\nde la liste AF", // 212
    "Canaux en mémoire", // 213
    "Exclure", // 214
    "Uniquement", // 215
    "Interrompre le scan\nautomatiquement", // 216
    "Code PI correct", // 217
    "Signal", // 218
    "Mettre l'audio en\nsourdine lors du scan", // 219
    "AIR", // 220
    "Appui de 5 sec.\nsur BAND", // 221
    "Éteindre l'appareil", // 222
    "Éteindre l'écran", // 223
    "Désactivé", // 224
    "Non défini", // 225
    "Actualités", // 226
    "Affaires courantes", // 227
    "Information", // 228
    "Sport", // 229
    "Éducation", // 230
    "Fiction", // 231
    "Culture", // 232
    "Science", // 233
    "Varié", // 234
    "Musique Pop", // 235
    "Musique Rock", // 236
    "Musique légère", // 237
    "Classique Léger", // 238
    "Classique Sérieux", // 239
    "Autre Musique", // 240
    "Météo", // 241
    "Economie", // 242
    "Prog. pour enfants", // 243
    "Affaires sociales", // 244
    "Religion", // 245
    "Ligne ouverte", // 246
    "Voyage", // 247
    "Loisirs", // 248
    "Musique Jazz", // 249
    "Musique Country", // 250
    "Musique Nationale", // 251
    "Musique Rétro", // 252
    "Folklore", // 253
    "Documentaire", // 254
    "TEST d'alerte", // 255
    "Alerte !", // 256
    "Initial", // 257
    "Toujours", // 258
    "Afficher l'horloge", // 259
    "Afficher le PS long", // 260
    "Fréquence initialle", // 261
    "Fréquence finale", // 262
    "Canal\nde mémoire initial", // 263
    "Canal\nde mémoire final", // 264
    "Mémoriser avec\nRDS uniquement", // 265
    "Démarrer la mémorisation\nautomatique", // 266
    "Mémorisation\nautomatique", // 267
    "Identifiant\nStationlist", // 268
    "Trouvé", // 269
    "Abandon! Mémoire de\ncanaux pleine", // 270
    "Abandon! Bouton\npressé par l'utilisateur", // 271
    "Scan terminé\nsans erreurs", // 272
    "Effacer les canaux\nmémorisés", // 273
    "effacé", // 274
    "Empêcher doublon PI", // 275
    "Plage", // 276
    "Complet", // 277
    "Temps d'attente\nuniquement sur le signal", // 278
    "Appuyez sur le coin\nindiqué pour ", // 279
    "calibrer l'écran tactile", // 280
    "Pressez BW pour\narrêter le scanner DX", // 281
    "Sélectionner la bande passante", // 282
    "Votre journal de bord", // 283
    "Télécharger le journal de bord", // 284
    "Journal de bord vide", // 285
    "Aller en bas", // 286
    "Entrée ajoutée au journal de bord", // 287
    "Erreur lors de l'ajout\nau journal de bord", // 288
    "Effacer le journal\nde bord", // 289
    "Journal effacé", // 290
    "Échec lors de\nla suppression", // 291
    "Impossible d'ajouter l'entrée!\nJournal de bord plein!", // 292
    "Log automatique", // 293
    "DST automatique\nsur l'heure NTP", // 294
    "Décalage horaire NTP", // 295
    "Le journal de bord contient plus de 130 entrées que la liste ne peut pas afficher. Veuillez télécharger le fichier CSV.", // 296
    "Canada" // 297
  },

  { "Български", // Bulgarian
    "Посоката е променена", // 1
    "Моля, пуснете бутона", // 2
    "Екранът бе обърнат", // 3
    "Калибрация на\nаналоговото измерване", // 4
    "USB режим", // 5
    "Тунер: !Няма!", // 6
    "Сила на звука", // 7
    "Конвертор", // 8
    "Нисък край", // 9
    "Висок край", // 10
    "FM RF ниво", // 11
    "Стерео праг", // 12
    "Честота на High Cut", // 13
    "Праг на High Cut", // 14
    "Праг на слаб сигнал", // 15
    "Яркост", // 16
    "Сила на звука", // 17
    "Отместване на\nконвертора", // 18
    "Нисък край на\nчестотната лента", // 19
    "Висок край на\nчестотната лента", // 20
    "Отместване на\nнивото", // 21
    "Праг на стерео разделяне", // 22
    "Честота на висок отрез", // 23
    "Праг на висок отрез", // 24
    "Праг за слаб сигнал", // 25
    "Яркост на дисплея", // 26
    "Изкллючв.", // 27
    "Включв.", // 28
    "НАТИСНЕТЕ MODE ЗА ИЗХОД И ЗАПИС", // 29
    "НАТИСНЕТЕ BAND ЗА ЗАТВАРЯНЕ", // 30
    "СКЕНИРАНЕ...", // 31
    "Тунерът не\nе разпознат!", // 32
    "Усилване на\nAM антена", // 33
    "FM АУР", // 34
    "Показване на\nRDS грешки", // 35
    "Език", // 36
    "Изберете език", // 37
    "PI заключен\nCT синхрон", // 38
    "Няма RDS сигнал", // 39
    "Меко мютиране FM", // 40
    "Меко мютиране AM", // 41
    "Сигнал при граница\nна лента", // 42
    "Регион", // 43
    "Европа", // 44
    "USA", // 45
    "Долна черта в RDS", // 46
    "Само PS", // 47
    "Wi-Fi активен", // 48
    "Конфигуриране на Wi-Fi", // 49
    "Свързване с: ", // 50
    "за конфигуриране на Wi-Fi", // 51
    "Опит за свързване на Wi-Fi", // 52
    "НЕУСПЕШНО!", // 53
    "СВЪРЗАНО!", // 54
    "IP на клиент за станции", // 55
    "SW дължина на вълната", // 56
    "RDS филтър", // 57
    "Показване на\nPI грешки", // 58
    "Използване на сквич", // 59
    "Метър на модулация", // 60
    "AM шумозаглушител", // 61
    "FM шумозаглушител", // 62
    "Настройки по\nподразбиране", // 63
    "Аудио изход", // 64
    "Настройка без деактивиране\nна скрийнсейвър", // 65
    "Инверсия на цветовете", // 66
    "Относно софтуера", // 67
    "Основен код:", // 68
    "Сътрудници:", // 69
    "Неизвестно", // 70
    "Настройки на\nскрийнсейвъра", // 71
    "Автоматично\nизключване", // 72
    "Изключване на екрана", // 73
    "Тема", // 74
    "Облик", // 75
    "Открити приложения", // 76
    "Мин.", // 77
    "Динамична SPI\nскорост", // 78
    "Чувствителност\nпри сканиране", // 79
    "НЯМА", // 80
    "Версия на софтуера", // 81
    "Шрифт на честотата", // 82
    "Автоматично", // 83
    "Няма налични AF", // 84
    "Няма налични EON", // 85
    "Няма налични RT+", // 86
    "Стъпка по\nподразбиране за FM", // 78
    "Скрийнсейвър", // 79
    "Сек.", // 80
    "Режим на часовника", // 81
    "1% яркост", // 82
    "25% яркост", // 83
    "50% яркост", // 84
    "Отместване на AM RF ниво", // 85
    "Единица за сигнал", // 86
    "Използване на AF", // 87
    "Избор на FM лента", // 97
    "Избор на AM лента", // 98
    "ДВ", // 99
    "СВ", // 100
    "КВ", // 101
    "ЧМ", // 102
    "ОИРТ", // 103
    "Спиране на събуждане\nна екрана", // 104
    "Избор на модел", // 105
    "База", // 106
    "Преносим", // 107
    "Преносимо докосване", // 108
    "Заглавие", // 109
    "Албум", // 110
    "Песен", // 111
    "Изпълнител", // 112
    "Композиция", // 113
    "Част", // 114
    "Диригент", // 115
    "Композитор", // 116
    "Група", // 117
    "Коментар", // 118
    "Жанр", // 119
    "Новини", // 120
    "Местни новини", // 121
    "Пазар", // 122
    "Спорт", // 123
    "Лотария", // 124
    "Хороскоп", // 125
    "Дневна програма", // 126
    "Здраве", // 127
    "Събитие", // 128
    "Сцена", // 129
    "Кино", // 130
    "ТВ", // 131
    "Дата/час", // 132
    "Време", // 133
    "Трафик", // 134
    "Аларма", // 135
    "Реклама", // 136
    "Уебсайт", // 137
    "Друго", // 138
    "Кратко PS", // 139
    "Дълго PS", // 140
    "Сега", // 141
    "Следващ", // 142
    "Част", // 143
    "Водещ", // 144
    "Редактор", // 145
    "Честота", // 146
    "Начална страница", // 147
    "Подканал", // 148
    "Гореща линия", // 149
    "Телефон на студио", // 150
    "Телефон", // 151
    "SMS на студио", // 152
    "SMS", // 153
    "Имейл гореща линия", // 154
    "Имейл на студио", // 155
    "Имейл", // 156
    "Чат", // 157
    "Чат център", // 158
    "Въпрос за гласуване", // 159
    "Център за гласуване", // 160
    "Място", // 161
    "Среща", // 162
    "Идентификатор", // 163
    "Покупка", // 164
    "Получаване на данни", // 165
    "Стъпка за AM", // 166
    "Мандарина", // 167
    "Океан", // 168
    "Индиго", // 169
    "Напрежение", // 170
    "Процент", // 171
    "Показване на процент", // 172
    "Буфериране на RT", // 173
    "Основни настройки", // 174
    "Аудио настройки", // 175
    "Настройки на дисплея", // 176
    "RDS настройки", // 177
    "FM настройки", // 178
    "AM настройки", // 179
    "Свързаност и часовник", // 180
    "НАТИСНЕТЕ MODE ЗА ВРЪЩАНЕ", // 181
    "Детектор на CoChannel", // 182
    "Чувствителност\nна CoChannel", // 183
    "Праг на CoChannel", // 184
    "Брояч на CoChannel", // 185
    "ОСНОВНИ", // 186
    "АУДИО", // 187
    "ДИСПЛЕЙ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "СВЪРЗАНЕ", // 192
    "DX РЕЖИМ", // 193
    "АВТОМАТИЧНА ПАМЕТ", // 194
    "AM АУР", // 195
    "FM деемфаза", // 196
    "Малък", // 197
    "Голям", // 198
    "Всички", // 199
    "Бърз PS", // 200
    "По подразбиране", // 201
    "Заглушаване на\nекран при XDRGTK", // 202
    "Подобряване на\nстерео FMSI", // 203
    "Функцията е достъпна\nсамо за TEF6687/6689!", // 204
    "Функцията не е\nдостъпна за облика!", // 205
    "Сканиране на\nканали в паметта", // 206
    "Спиране на\nсканиране на канали", // 207
    "Време за изчакване", // 208
    "Стартиране на DX сканер", // 209
    "Опции за FM DX", // 210
    "Началният канал\nе пропуснат.", // 211
    "Автоматично\nсортиране на AF", // 212
    "Канали в паметта", // 213
    "Изключване", // 214
    "Само", // 215
    "Автоматично прекратяване", // 216
    "Коригиране на PI", // 217
    "Сигнал", // 218
    "Заглушаване на\nзвука при сканиране", // 219
    "Въздушен", // 220
    "5 сек. натискане\nна band бутон", // 221
    "Изключване на\nзахранването", // 222
    "Изключване на екрана", // 223
    "Изключено", // 224
    "Няма", // 225
    "Новини", // 226
    "Актуални събития", // 227
    "Информация", // 228
    "Спорт", // 229
    "Образование", // 230
    "Драма", // 231
    "Култура", // 232
    "Наука", // 233
    "Разнообразие", // 234
    "Поп музика", // 235
    "Рок музика", // 236
    "Лека музика", // 237
    "Лека класика", // 238
    "Сериозна класика", // 239
    "Друга музика", // 240
    "Време", // 241
    "Финанси", // 242
    "Детски програми", // 243
    "Социални въпроси", // 244
    "Религия", // 245
    "Телефонен ефир", // 246
    "Пътувания", // 247
    "Свободно време", // 248
    "Джаз музика", // 249
    "Кънтри музика", // 250
    "Национална музика", // 251
    "Стари хитове", // 252
    "Фолк музика", // 253
    "Документален", // 254
    "Тест на аларма", // 255
    "Аларма!!!", // 256
    "Начален", // 257
    "Винаги", // 258
    "Показване на\nчасовника", // 259
    "Показване на\nдълго PS", // 260
    "Начална честота", // 261
    "Крайна честота", // 262
    "Начален канал\nв паметта", // 263
    "Краен канал в паметта", // 264
    "Запазване само с RDS", // 265
    "Стартиране на\nавтоматична памет", // 266
    "Автоматична памет", // 267
    "ID на списък с станции", // 268
    "Намерено", // 269
    "Няма свободни\nканали.", // 270
    "Потребителят\nнатисна бутон.", // 271
    "Сканирането завърши без грешки.", // 272
    "Изчистване на\nканали в паметта", // 273
    "Изчистено", // 274
    "Предотвратяване на\nдублиране на PI", // 275
    "Обхват", // 276
    "Пълен", // 277
    "Време за изчакване\nпри сигнал", // 278
    "Натиснете маркирания ъгъл", // 279
    "За калибриране\nна докосване", // 280
    "Задръжте BW бутона\nза да спрете сканера", // 281
    "Избор на честотна\nлента", // 282
    "Вашият дневник", // 283
    "Изтегляне на\nдневника", // 284
    "Дневникът е празен", // 285
    "Отидете в края", // 286
    "Записът е добавен\nв дневника", // 287
    "Грешка при добавяне в дневника", // 288
    "Изчистване на дневника", // 289
    "Дневникът е изчистен", // 290
    "Изчистването не\nбе успешно", // 291
    "Не може да се добави запис!\nДневникът е пълен!", // 292
    "Автоматичен дневник", // 293
    "Автоматично лятно\nвреме по NTP", // 294
    "Отместване на NTP", // 295
    "Дневникът съдържа над 130 записа, които не могат да бъдат обработени. Моля, изтеглете CSV файла за обработка.", // 296
    "Канада" // 297
  },

  { "Русский", // Russian
    "Изменено направление", // 1
    "Теперь отпустите кнопку", // 2
    "Экран перевёрнут", // 3
    "Калибровка S-метра", // 4
    "Режим USB", // 5
    "Тюнер: !Нет!", // 6
    "Громкость", // 7
    "Сдвиг конвертера", // 8
    "Нижняя граница FM", // 9
    "Верхняя граница FM", // 10
    "Калибровка уровня", // 11
    "Порог открытия стерео", // 12
    "Срез ВЧ", // 13
    "Порог среза ВЧ", // 14
    "Порог качества сигнала", // 15
    "Яркость", // 16
    "Линейная громкость", // 17
    "Сдвиг конвертера", // 18
    "Нижняя граница FM", // 19
    "Верхняя граница FM", // 20
    "Калибровка уровня", // 21
    "Порог открытия стереодекодера", // 22
    "Срез ВЧ при слабом сигнале", // 23
    "Порог высокочастотного среза", // 24
    "Порог качества сигнала", // 25
    "Яркость дисплея", // 26
    "Выкл.", // 27
    "Вкл.", // 28
    "НАЖМИТЕ MODE ДЛЯ СОХРАНЕНИЯ", // 29
    "НАЖМИТЕ BAND ДЛЯ ЗАКРЫТИЯ", // 30
    "СКАНИРОВАНИЕ", // 31
    "Тюнер не обнаружен", // 32
    "Аттеньюатор AM-антенны", // 33
    "FM AGC", // 34
    "Уровень ошибок RDS", // 35
    "Язык меню", // 36
    "Язык меню: ", // 37
    "PI locked CT sync", // 38
    "Ошибка! Нет RDS!", // 39
    "FM Soft Mute", // 40
    "AM Soft Mute", // 41
    "Край диапазона (звук): ", // 42
    "Регион", // 43
    "Европа", // 44
    "USA", // 45
    "Показывать _ в RDS", // 46
    "Только PS", // 47
    "Wi-Fi активен", // 48
    "Настройка Wi-Fi", // 49
    "Подключиться к: ", // 50
    "для настройки Wi-Fi", // 51
    "Попытка подключения Wi-Fi", // 52
    "НЕ УДАЛОСЬ... Wi-Fi отключен", // 53
    "Подключено!", // 54
    "IP stationlist-клиента", // 55
    "Отображать длину волны", // 56
    "Фильтр RDS-данных", // 57
    "Ошибки PI-кода", // 58
    "Отображать шумодав", // 59
    "Уровень модуляции", // 60
    "AM Noise blanker", // 61
    "FM Noise blanker", // 62
    "Загружены настройки по умолчанию", // 63
    "Режим вывода аудио", // 64
    "При погашенном экране", // 65
    "Инвертировать дисплей", // 66
    "О программе", // 67
    "Основной код:", // 68
    "Соавторы:", // 69
    "Неизвестно", // 70
    "Действие по таймеру", // 71
    "Глубокий сон", // 72
    "Погасить экран", // 73
    "Тема", // 74
    "Вид (Skin)", // 75
    "Обнаруженные приложения", // 76
    "мин.", // 77
    "Частота шины дисплея SPI", // 78
    "Чувствительность сканирования", // 79
    "Не использовать", // 80
    "Версия прошивки -", // 81
    "Шрифт индикации частоты", // 82
    "Auto", // 83
    "Нет данных", // 84
    "Нет данных", // 85
    "Нет данных", // 86
    "Шаг FM по умолчанию", // 78
    "Таймер", // 79
    "сек", // 80
    "Режим часов", // 81
    "1% яркости", // 82
    "25% яркости", // 83
    "50% яркости", // 84
    "Калибровка уровня", // 85
    "Измерять сигнал в", // 86
    "Переключаться по AF", // 87
    "Активные диапазоны FM", // 97
    "Активные диапазоны AM", // 98
    "ДВ", // 99
    "СВ", // 100
    "КВ", // 101
    "FM", // 102
    "УКВ", // 103
    "Настройка при погашенном экране: ", // 104
    "Выбор модели", // 105
    "Стандартный", // 106
    "Портативный", // 107
    "Сенсорный", // 108
    "Название", // 109
    "Альбом", // 110
    "Трек", // 111
    "Исполнитель", // 112
    "Композиция", // 113
    "Движение", // 114
    "Дирижер", // 115
    "Композитор", // 116
    "Группа", // 117
    "Комментарий", // 118
    "Жанр", // 119
    "Новости", // 120
    "Местные новости", // 121
    "Фондовый рынок", // 122
    "Спорт", // 123
    "Лотерея", // 124
    "Гороскоп", // 125
    "Развлечения", // 126
    "Здоровье", // 127
    "Событие", // 128
    "Сцена", // 129
    "Кинотеатр", // 130
    "Телевидение", // 131
    "Дата/время", // 132
    "Погода", // 133
    "Движение", // 134
    "Будильник", // 135
    "Реклама", // 136
    "Веб-сайт", // 137
    "Другое", // 138
    "Краткое PS", // 139
    "Длинное PS", // 140
    "Сейчас", // 141
    "Следующий", // 142
    "Часть", // 143
    "Ведущий", // 144
    "Редактор", // 145
    "Частота", // 146
    "Домашняя страница", // 147
    "Субканал", // 148
    "Горячая линия", // 149
    "Телефон студии", // 150
    "Телефон", // 151
    "SMS студии", // 152
    "SMS", // 153
    "Email горячей линии", // 154
    "Email студии", // 155
    "Email", // 156
    "Чат", // 157
    "Чат-центр", // 158
    "Вопрос голосования", // 159
    "Голосовой центр", // 160
    "Место", // 161
    "Встреча", // 162
    "Идентификатор", // 163
    "Покупка", // 164
    "Получить данные", // 165
    "Шаг по умолчанию на СВ", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Индикатор батареи", // 170
    "Вольтаж", // 171
    "Процент заряда", // 172
    "Буфер RT", // 173
    "Основные настройки", // 174
    "Звуковые настройки", // 175
    "Настройки отображения", // 176
    "RDS", // 177
    "Настройки FM", // 178
    "Настройки AM", // 179
    "Настройки подключения", // 180
    "НАЖМИТЕ MODE ДЛЯ ВОЗВРАТА", // 181
    "CoChannelDet чувствительность", // 182
    "AM CoChannelDet", // 183
    "CoChannelDet количество", // 184
    "Порог AM CoChannelDet", // 185
    "ОСНОВНЫЕ", // 186
    "ЗВУКОВЫЕ", // 187
    "ДИСПЛЕЙ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "КОННЕКТ", // 192
    "DX MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM deemphasis", // 196
    "Мало", // 197
    "Больше", // 198
    "Все", // 199
    "Быстрый PS", // 200
    "По умолчанию", // 201
    "Гасить дисплей при\nработе с XDRGTK", // 202
    "FMSI стерео\nулучшение", // 203
    "Доступно только\nна TEF6687/6689", // 204
    "Функция недоступна\nна этом оформлении", // 205
    "Стартовый канал\nпамяти", // 206
    "Конечный канал\nпамяти", // 207
    "Время ожидания", // 208
    "Начать DX-сканирование", // 209
    "Настройки FM DX сканера", // 210
    "Прервано! Стартовый канал\nпомечен для пропуска.", // 211
    "Сортировка AF-частот", // 212
    "Каналы памяти", // 213
    "Кроме", // 214
    "Только", // 215
    "Автоматическая отмена\nсканирования", // 216
    "При корректном PI", // 217
    "При наличии сигнала", // 218
    "Отключать аудио\nпри сканировании", // 219
    "Эфир", // 220
    "5 сек. нажатие Band", // 221
    "Отключить тюнер", // 222
    "Отключить экран", // 223
    "Запрещено", // 224
    "Не определено", // 225
    "Новости", // 226
    "Аналитика", // 227
    "Информация", // 228
    "Спорт", // 229
    "Образование", // 230
    "Драма", // 231
    "Культура", // 232
    "Наука", // 233
    "Разное", // 234
    "Поп М", // 235
    "Рок М", // 236
    "Лёгкая музыка", // 237
    "Лёгкая классика", // 238
    "Классика", // 239
    "Другая музыка", // 240
    "Погода", // 241
    "Финансы", // 242
    "Детские прогр", // 243
    "Общество", // 244
    "Религия", // 245
    "Открытая линия", // 246
    "Туризм", // 247
    "Досуг", // 248
    "Джаз", // 249
    "Музыка кантри", // 250
    "Национальная М", // 251
    "Олдис", // 252
    "Народная музыка", // 253
    "Документалистика", // 254
    "Тревога тест", // 255
    "Тревога!", // 256
    "Начальный", // 257
    "Всегда", // 258
    "Показать часы", // 259
    "Показывать длинный PS", // 260
    "Начальная частота", // 261
    "Конечная частота", // 262
    "Начальный канал памяти", // 263
    "Конечный канал памяти", // 264
    "Сохранять только с RDS", // 265
    "Начать автозапоминание", // 266
    "Автозапоминание", // 267
    "Stationlist идентификатор", // 268
    "Найдено", // 269
    "Прервано! Нехватка памяти", // 270
    "Прервано! Нажата кнопка", // 271
    "Сканирование завершено без ошибок", // 272
    "Очистить каналы памяти", // 273
    "очищено", // 274
    "Предотвращать дубль PI", // 275
    "Диапазон", // 276
    "Полный", // 277
    "Время ожидания\nпри сигнале", // 278
    "Нажмите на подсвеченный угол", // 279
    "Калибровка тачскрина", // 280
    "Переключение инверсии экрана", // 281
    "Выбрать ширину полосы", // 282
    "Ваш лог-файл", // 283
    "Скачать лог-файл", // 284
    "Лог-файл пуст", // 285
    "Вниз", // 286
    "Добавлено в лог-файл", // 287
    "Обновление не удалось!", // 288
    "Очистить лог-файл", // 289
    "Лог-файл очищен", // 290
    "Очистить не удалось", // 291
    "Смещение времени NTP", // 292
    "Автологгер", // 293
    "Автоматическое летнее время по NTP", // 294
    "Смещение времени NTP", // 295
    "В логе более 130 записей, которые программа просмотра не может обработать. Загрузите файл CSV для его обработки.", // 296
    "Канада" // 297
  },

  { "Українська", // Ukranian
    "Змінено напрямок обертання", // 1
    "Відпустіть кнопку", // 2
    "Екран перевернуто", // 3
    "Налагодження аналогового індикатора", // 4
    "Режим USB", // 5
    "Тюнер: !Відсутній!", // 6
    "Гучність", // 7
    "Конвертер", // 8
    "Нижня межа діапазону", // 9
    "Верхня межа діапазону", // 10
    "Зсув РЧ рівня FM", // 11
    "Поріг стерео", // 12
    "Відсікання високих частот", // 13
    "Поріг високих частот", // 14
    "Поріг низьких частот", // 15
    "Яскравість", // 16
    "Підсилення гучності", // 17
    "Зсув перетворювача", // 18
    "Нижня межа діапазону", // 19
    "Верхня межа діапазону", // 20
    "Зміщення рівня", // 21
    "Поріг розділення стерео", // 22
    "Частота відсікання", // 23
    "Межа високих частот", // 24
    "Межа низьких частот", // 25
    "Яскравість", // 26
    "Вимк.", // 27
    "Увімк.", // 28
    "Тисни MODE, щоб зберегти й вийти", // 29
    "Тисни BAND, щоб закрити", // 30
    "Пошук...", // 31
    "Тюнер не виявлено", // 32
    "Атенюатор СХ-антени", // 33
    "FM АРП", // 34
    "Показувати/nпомилки RDS", // 35
    "Мова", // 36
    "Виберіть мову", // 37
    "PI заблок./nузгодження CT", // 38
    "Помилка! Відсутній/nсигнал RDS", // 39
    "Приглушення FM", // 40
    "Приглушення СХ", // 41
    "Сигнал на межі/nдіапазону", // 42
    "Регіон", // 43
    "Європа", // 44
    "США", // 45
    "Показувати/nпідкреслення в RDS", // 46
    "Лише в PS", // 47
    "Wi-Fi ввімкнено", // 48
    "Налаштувати Wi-Fi", // 49
    "Підключитися до:", // 50
    "для налаштування Wi-Fi", // 51
    "Підключення до Wi-Fi...", // 52
    "Невдача!", // 53
    "Підключено!", // 54
    "IP клієнта Stationlist", // 55
    "Показувати довжину/nхвилі КХ", // 56
    "Фільтр RDS", // 57
    "Показувати помилки PI", // 58
    "Використовувати затихання", // 59
    "Аудіометр", // 60
    "Подавлення шуму СХ", // 61
    "Подавлення шуму FM", // 62
    "Завантажені типові налаштування", // 63
    "Аудіовихід", // 64
    "Дозволити налаштування\nбез вимкнення екрану", // 65
    "Інвертувати кольори", // 66
    "Версія ПЗ", // 67
    "Основний код:", // 68
    "Учасники:", // 69
    "Невідомо", // 70
    "Налаштування заставки", // 71
    "Автовимкнення", // 72
    "Екран вимкнено", // 73
    "Тема", // 74
    "Обгортка", // 75
    "Виявлені програми", // 76
    "хв.", // 77
    "Динамічна\nшвидкість SPI", // 78
    "Чутливість сканування", // 79
    "НІЧОГО", // 80
    "Версія ПЗ", // 81
    "Шрифт частоти", // 82
    "Авто.", // 83
    "AF недоступний", // 84
    "EON недоступний", // 85
    "RT+ недоступний", // 86
    "Крок FM за\nзамовчуванням", // 78
    "Вимкнення екрану через", // 79
    "с", // 80
    "Режим годинника", // 81
    "Яскравість 1%", // 82
    "Яскравість 25%", // 83
    "Яскравість 50%", // 84
    "Зміщення РЧ рівня СХ", // 85
    "Одиниця вимірювання\nсигналу", // 86
    "Використовувати AF", // 87
    "Вибрати FM-діапазон", // 97
    "Вибрати СХ-діапазон", // 98
    "ДХ", // 99
    "СХ", // 100
    "КХ", // 101
    "FM", // 102
    "OIRT", // 103
    "Не вмик. екран\nпід час налаш.", // 104
    "Вибір моделі", // 105
    "Стандартна", // 106
    "Переносна", // 107
    "Сенсорна", // 108
    "Назва", // 109
    "Альбом", // 110
    "Трек", // 111
    "Виконавець", // 112
    "Композиція", // 113
    "Рух", // 114
    "Диригент", // 115
    "Композитор", // 116
    "Група", // 117
    "Коментар", // 118
    "Жанр", // 119
    "Новини", // 120
    "Місцеві новини", // 121
    "Фондовий ринок", // 122
    "Спорт", // 123
    "Лотерея", // 124
    "Гороскоп", // 125
    "Щоденні розваги", // 126
    "Здоров'я", // 127
    "Подія", // 128
    "Сцена", // 129
    "Кіно", // 130
    "ТБ", // 131
    "Дата/час", // 132
    "Погода", // 133
    "Трафік", // 134
    "Будильник", // 135
    "Реклама", // 136
    "Веб-сайт", // 137
    "Інше", // 138
    "Короткий PS", // 139
    "Довгий PS", // 140
    "Зараз", // 141
    "Далі", // 142
    "Частина", // 143
    "Ведучий", // 144
    "Редактор", // 145
    "Частота", // 146
    "Домашня сторінка", // 147
    "Субканал", // 148
    "Гаряча лінія", // 149
    "Телефон студії", // 150
    "Телефон", // 151
    "SMS студії", // 152
    "SMS", // 153
    "Електронна пошта\nгарячої лінії", // 154
    "Електронна пошта\nстудії", // 155
    "Електронна пошта", // 156
    "Чат", // 157
    "Чат-центр", // 158
    "Питання\nголосування", // 159
    "Центр\nголосування", // 160
    "Місце", // 161
    "Зустріч", // 162
    "Ідентифікатор", // 163
    "Покупка", // 164
    "Отримання даних", // 165
    "Крок СХ за\nзамовчуванням", // 166
    "Помаранчевий", // 167
    "Хвиля", // 168
    "Пурпуровий", // 169
    "Налаштування показу\nакумулятора", // 170
    "Напруга", // 171
    "Відсоток", // 172
    "Буферизація RT", // 173
    "Основні налаштування", // 174
    "Налаштування аудіо", // 175
    "Налаштування екрану", // 176
    "Налаштування RDS", // 177
    "Налаштування FM", // 178
    "Налаштування СХ", // 179
    "З'єднання та годинник", // 180
    "Тисни MODE, щоб повернутися", // 181
    "Детектор CoChannel", // 182
    "Чутливість CoChannel", // 183
    "Поріг CoChannel", // 184
    "Лічильник CoChannel", // 185
    "Загальне", // 186
    "Звук", // 187
    "Екран", // 188
    "RDS", // 189
    "FM", // 190
    "СХ", // 191
    "З'єднання", // 192
    "Режим DX", // 193
    "Автопам'ять", // 194
    "СХ АРП", // 195
    "Деемфаза FM", // 196
    "Мала", // 197
    "Велика", // 198
    "Все", // 199
    "Швидкий PS", // 200
    "За замовчуванням", // 201
    "Вимкнути екран під час\nпідключення XDRGTK", // 202
    "Покращення стерео FMSI", // 203
    "Функція доступна лише\nна TEF6687/6689!", // 204
    "Функція недоступна\nна даній темі!", // 205
    "Почати сканування\nканалів пам'яті", // 206
    "Зупинити сканування\nканалів пам'яті", // 207
    "Час очікування", // 208
    "Почати DX-пошук", // 209
    "Налаштування FM DX", // 210
    "Скасовано! Початковий канал\nпозначено як пропущений", // 211
    "Автоматичне\nсортування AF", // 212
    "Канали пам'яті", // 213
    "Крім", // 214
    "Лише", // 215
    "Автоматичне скасування\nсканування", // 216
    "Виправити PI", // 217
    "Сигнал", // 218
    "Вимкнути звук\nпід час сканування", // 219
    "Ефір", // 220
    "Дія, якщо тримати BAND 5 секунд", // 221
    "Вимкнення приймача", // 222
    "Вимкнення екрану", // 223
    "Відключено", // 224
    "Немає", // 225
    "Новини", // 226
    "Поточні події", // 227
    "Інформація", // 228
    "Спорт", // 229
    "Освіта", // 230
    "Драма", // 231
    "Культура", // 232
    "Наука", // 233
    "Різне", // 234
    "Попмузика", // 235
    "Рок-музика", // 236
    "Легка музика", // 237
    "Легка класика", // 238
    "Класика", // 239
    "Інша музика", // 240
    "Погода", // 241
    "Фінанси", // 242
    "Дитячі програми", // 243
    "Соціальні програми", // 244
    "Релігія", // 245
    "Телефонні вітання", // 246
    "Подорожі", // 247
    "Дозвілля", // 248
    "Джаз", // 249
    "Кантрі", // 250
    "Національна музика", // 251
    "Стара музика", // 252
    "Фольклор", // 253
    "Документалістика", // 254
    "Випробування тривоги", // 255
    "Тривога!!!", // 256
    "Початковий", // 257
    "Завжди", // 258
    "Показати годинник", // 259
    "Показати довгий PS", // 260
    "Початкова частота", // 261
    "Кінцева частота", // 262
    "Початковий канал пам'яті", // 263
    "Кінцевий канал пам'яті", // 264
    "Зберігати лише з RDS", // 265
    "Запуск автопам'яті", // 266
    "Автопам'ять", // 267
    "Stationlist ID", // 268
    "Знайдено", // 269
    "Скасовано! Немає\nканалів пам'яті", // 270
    "Скасовано! Користувач\nнатиснув кнопку", // 271
    "Сканування завершено\nбез помилок", // 272
    "Очистити канали\nпам'яті", // 273
    "очищено", // 274
    "Запобігти подвійному PI", // 275
    "Діапазон", // 276
    "Повний", // 277
    "Час очікування лише за сигналом", // 278
    "Натисніть виділений кут", // 279
    "Щоб налаштувати сенсор", // 280
    "Тримайте BAND, щоб зупинити пошук DX", // 281
    "Вибір пропускної здатності", // 282
    "Твій журнал", // 283
    "Завантажити журнал", // 284
    "Твій журнал є порожнім", // 285
    "Вниз", // 286
    "Додано до журналу", // 287
    "Помилка оновлення!", // 288
    "Очистити журнал", // 289
    "Очищено", // 290
    "Не вдалося очистити", // 291
    "Твій журнал повний!", // 292
    "Автореєстратор", // 293
    "Автоматичний літній\nчас за NTP", // 294
    "Встановити зміщення\nчасу NTP", // 295
    "Журнал містить понад 130 записів, які оглядач не може обробити. Будь ласка, завантажте файл CSV, щоб обробити його.", // 296
    "Канада" // 297
  },

  { "Italiano", // Italian
    "Senso rotaz. cambiato", // 1
    "Per favore rilascia il pulsante", // 2
    "Schermo capovolto", // 3
    "Calibra il misuratore analogico", // 4
    "Modalità USB", // 5
    "Sintonizzatore: !Nessuno!", // 6
    "Volume", // 7
    "Convertitore", // 8
    "Limite banda bassa", // 9
    "Limite banda alta", // 10
    "Offset livello RF FM", // 11
    "Soglia stereo", // 12
    "Angolo di taglio alto", // 13
    "Soglia di taglio alto", // 14
    "Soglia di basso livello", // 15
    "Luminosità", // 16
    "Imposta volume", // 17
    "Offset convertitore", // 18
    "Limite banda bassa", // 19
    "Limite banda alta", // 20
    "Offset di livello", // 21
    "Soglia separazione stereo", // 22
    "Frequenza angolo di taglio alto", // 23
    "Soglia di taglio alto", // 24
    "Soglia di basso livello", // 25
    "Luminosità display", // 26
    "Off", // 27
    "On", // 28
    "PREMI MODE PER USCIRE E MEMORIZZARE", // 29
    "PREMI BAND PER CHIUDERE", // 30
    "SCANSIONE...", // 31
    "Sintonizzatore non rilevato", // 32
    "Guadagno antenna AM", // 33
    "AGC FM", // 34
    "Mostra errori RDS", // 35
    "Lingua", // 36
    "Scegli lingua", // 37
    "PI bloccato CT sync", // 38
    "Errore! Nessun segnale RDS", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Beeper al limite di banda", // 42
    "Regione", // 43
    "Europa", // 44
    "USA", // 45
    "Mostra sottolineato in RDS", // 46
    "Solo PS", // 47
    "Wi-Fi attivo", // 48
    "Configura Wi-Fi", // 49
    "Connetti a: ", // 50
    "per configurare Wi-Fi", // 51
    "Tentativo di connessione Wi-Fi", // 52
    "FALLITO!", // 53
    "CONNESSO!", // 54
    "IP client stationlist", // 55
    "Mostra lunghezza d'onda SW", // 56
    "Filtro RDS", // 57
    "Mostra errori PI", // 58
    "Usa squelch", // 59
    "Misuratore del suono", // 60
    "Filtro anti-rumore AM", // 61
    "Filtro anti-rumore FM", // 62
    "Impostazioni predefinite caricate", // 63
    "Uscita audio", // 64
    "Consenti sintonia senza\ndisattivare screensaver", // 65
    "Inverti colori display", // 66
    "Informazioni software", // 67
    "Codice principale:", // 68
    "Contributori:", // 69
    "Sconosciuto", // 70
    "Opzioni screensaver", // 71
    "Spegnimento automatico", // 72
    "Schermo spento", // 73
    "Tema", // 74
    "Skin", // 75
    "Applicazioni rilevate", // 76
    "Min.", // 77
    "Velocità SPI", // 78
    "Sensibilità scansione", // 79
    "NESSUNO", // 80
    "Versione software", // 81
    "Carattere frequenza", // 82
    "Auto", // 83
    "Nessun AF disponibile", // 84
    "Nessun EON disponibile", // 85
    "Nessun RT+ disponibile", // 86
    "Passo FM predefinito", // 78
    "Screensaver", // 79
    "Sec", // 80
    "Modalità orologio", // 81
    "1% luminosità", // 82
    "25% luminosità", // 83
    "50% luminosità", // 84
    "Offset livello RF AM", // 85
    "Unità misuratore segnale", // 86
    "Usa AF", // 87
    "Seleziona banda FM", // 97
    "Seleziona banda AM", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Ferma il risveglio schermo su sintonia", // 104
    "Selettore modello", // 105
    "Base", // 106
    "Portatile", // 107
    "Portatile touch", // 108
    "Titolo", // 109
    "Album", // 110
    "Traccia", // 111
    "Artista", // 112
    "Composizione", // 113
    "Movimento", // 114
    "Direttore d'orchestra", // 115
    "Compositore", // 116
    "Gruppo", // 117
    "Commento", // 118
    "Genere", // 119
    "Notizie", // 120
    "Notizie locali", // 121
    "Mercato azionario", // 122
    "Sport", // 123
    "Lotteria", // 124
    "Oroscopo", // 125
    "Divertimento quotidiano", // 126
    "Salute", // 127
    "Evento", // 128
    "Scena", // 129
    "Cinema", // 130
    "TV", // 131
    "Data/ora", // 132
    "Meteo", // 133
    "Traffico", // 134
    "Allarme", // 135
    "Pubblicità", // 136
    "Sito web", // 137
    "Altro", // 138
    "PS breve", // 139
    "PS lungo", // 140
    "Ora", // 141
    "Prossimo", // 142
    "Parte", // 143
    "Host", // 144
    "Editor", // 145
    "Frequenza", // 146
    "Homepage", // 147
    "Subcanale", // 148
    "Linea diretta", // 149
    "Telefono studio", // 150
    "Telefono", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email hotline", // 154
    "Email studio", // 155
    "Email", // 156
    "Chat", // 157
    "Centro chat", // 158
    "Domanda voto", // 159
    "Centro voto", // 160
    "Luogo", // 161
    "Appuntamento", // 162
    "Identificatore", // 163
    "Acquisto", // 164
    "Ottenere dati", // 165
    "Passo MW predefinito", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Opzioni batteria", // 170
    "Mostra tensione", // 171
    "Mostra percentuale", // 172
    "Buffering RT", // 173
    "Impostazioni principali", // 174
    "Impostazioni audio", // 175
    "Impostazioni display", // 176
    "Impostazioni RDS", // 177
    "Impostazioni FM", // 178
    "Impostazioni AM", // 179
    "Connettività e orologio", // 180
    "PREMI MODE PER RITORNO", // 181
    "Rilevatore CoCanale", // 182
    "Sensibilità rilevatore isofrequenza", // 183
    "Soglia rilevatore isofrequenza", // 184
    "Contatore rilevatore CoCanale", // 185
    "PRINCIPALE", // 186
    "AUDIO", // 187
    "DISPLAY", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CONNETTI", // 192
    "DX MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM de-enfasi", // 196
    "Piccolo", // 197
    "Grande", // 198
    "Tutto", // 199
    "PS veloce", // 200
    "Predefinito", // 201
    "Disattiva schermo su\nconnessione XDRGTK", // 202
    "Miglioramento stereo FMSI", // 203
    "Funzione disponibile solo su TEF6687/6689!", // 204
    "Funzione non disponibile su skin selezionata!", // 205
    "Inizia scansione\ncanali di memoria", // 206
    "Ferma scansione\ncanali di memoria", // 207
    "Tempo di attesa", // 208
    "Inizia scansione DX", // 209
    "Opzioni FM DX", // 210
    "Abortito! Canale iniziale contrass. da saltare", // 211
    "Auto ordina AF", // 212
    "Canali di memoria", // 213
    "Escludi", // 214
    "Solo", // 215
    "Annulla scansione automatica", // 216
    "Correggi PI", // 217
    "Segnale", // 218
    "Disattiva audio\nmentre scansiona", // 219
    "AEREO", // 220
    "Premi per 5 sec. il pulsante di banda", // 221
    "Spegni", // 222
    "Schermo spento", // 223
    "Disabilitato", // 224
    "Nessuno", // 225
    "Notizie", // 226
    "Affari correnti", // 227
    "Informazioni", // 228
    "Sport", // 229
    "Educazione", // 230
    "Dramma", // 231
    "Cultura", // 232
    "Scienza", // 233
    "Varietà", // 234
    "Musica Pop", // 235
    "Musica Rock", // 236
    "Musica facile", // 237
    "Classica leggera", // 238
    "Classica seria", // 239
    "Altra musica", // 240
    "Meteo", // 241
    "Finanza", // 242
    "Programmi per bambini", // 243
    "Affari sociali", // 244
    "Religione", // 245
    "Telefonata", // 246
    "Viaggio", // 247
    "Tempo libero", // 248
    "Musica Jazz", // 249
    "Musica Country", // 250
    "Musica Nazionale", // 251
    "Musica Oldies", // 252
    "Musica Folk", // 253
    "Documentario", // 254
    "Test allarme", // 255
    "Allarme!!!", // 256
    "Iniziale", // 257
    "Sempre", // 258
    "Mostra orologio", // 259
    "Mostra PS lungo", // 260
    "Inizia frequenza", // 261
    "Ferma frequenza", // 262
    "Inizia canale memoria", // 263
    "Ferma canale memoria", // 264
    "Memorizza solo con RDS", // 265
    "Inizia auto memoria", // 266
    "Auto memoria", // 267
    "ID Stationlist", // 268
    "Trovato", // 269
    "Abortito! Canali di memoria esauriti", // 270
    "Abortito! L'utente ha premuto il pulsante", // 271
    "Scansione terminata senza errori", // 272
    "Cancella canali di memoria", // 273
    "Cancellato", // 274
    "Prevenire doppio PI", // 275
    "Range", // 276
    "Completo", // 277
    "Tempo di attesa su\nsolo segnale", // 278
    "Premi angolo evidenziato", // 279
    "Per calibrare touchscreen", // 280
    "Tieni premuto il tasto BW per\nfermare scanner DX", // 281
    "Seleziona larghezza di banda", // 282
    "Il tuo registro dei log", // 283
    "Scarica registro dei log", // 284
    "Registro dei log vuoto", // 285
    "Vai in fondo", // 286
    "Voce aggiunta\nal registro dei log", // 287
    "Errore nell'aggiunta\nvoce al registro dei log", // 288
    "Cancella registro", // 289
    "Registro cancellato", // 290
    "Cancellazione fallita", // 291
    "Impossibile aggiungere voce!\nRegistro dei log pieno!", // 292
    "Autologger", // 293
    "Auto DST su tempo NTP", // 294
    "Imposta offset orario NTP", // 295
    "Registro dei log con + di 130 voci, il visualizzatore non può gestirle. Scarica il file CSV per gestirlo.", // 296
    "Canada" // 297
  },

  { "Simplified Chinese", // Simplified Chinese
    "飞梭方向改变了", // 1
    "请释放按钮", // 2
    "屏幕翻转了", // 3
    "校准模拟S表", // 4
    "USB 连机模式选择", // 5
    "没有收音芯片！", // 6
    "音量", // 7
    "偏移量", // 8
    "FM 最低频率", // 9
    "FM 最高频率", // 10
    "FM信号偏移量", // 11
    "立体声阈值", // 12
    "高切角", // 13
    "高切门限", // 14
    "弱信号门限", // 15
    "屏幕亮度", // 16
    "设置音量", // 17
    "设置频率偏移量", // 18
    "设置 FM 最低频率", // 19
    "设置 FM 最高频率", // 20
    "设置信号值偏移量", // 21
    "设置立体声门限", // 22
    "设置高切角频率", // 23
    "设置高切门限", // 24
    "设置弱信号门限", // 25
    "屏幕亮度", // 26
    "关闭", // 27
    "打开", // 28
    "按 MODE 键退出并保存", // 29
    "时钟模式", // 30
    "搜索中...", // 31
    "未检测到收音芯片", // 32
    "AM 天线增益", // 33
    "FM AGC", // 34
    "显示 RDS 错误", // 35
    "语言", // 36
    "选择语言", // 37
    "从RDS同步时间", // 38
    "错误! 无RDS信号", // 39
    "FM 软静噪", // 40
    "AM 软静噪", // 41
    "频率边界提示音", // 42
    "国家与地区", // 43
    "欧洲", // 44
    "USA", // 45
    "RDS 中显示下划线", // 46
    "只有PS", // 47
    "打开 Wifi", // 48
    "Wi-Fi 配置", // 49
    "连接到：", // 50
    "配置 Wi-Fi", // 51
    "尝试连接 Wifi 到", // 52
    "失败.. 关闭 Wi-Fi", // 53
    "连接成功！", // 54
    "Stationlist 客户端 IP", // 55
    "显示短波米波段", // 56
    "RDS 过滤器", // 57
    "显示 PI 错误", // 58
    "使能静噪功能", // 59
    "主画面显示 M 表", // 60
    "AM 噪声消除", // 61
    "FM 噪声消除", // 62
    "恢复出厂设置", // 63
    "音频输出选择", // 64
    "盲操", // 65
    "屏幕颜色反演", // 66
    "关于", // 67
    "主程序", // 68
    "贡献者", // 69
    "未知", // 70
    "屏保选项", // 71
    "倒计时休眠", // 72
    "关闭屏幕", // 73
    "主题 & 颜色", // 74
    "布局", // 75
    "检测到的应用程序", // 76
    "分钟", // 77
    "调整 SPI 通讯速率", // 78
    "搜索灵敏度", // 79
    "不可用", // 80
    "固件版本", // 81
    "频率字体", // 82
    "自动", // 83
    "AF 不可用", // 84
    "EON 不可用", // 85
    "RT+ 不可用", // 86
    "FM 默认步进", // 78
    "屏保", // 79
    "秒", // 80
    "时钟制式", // 81
    "屏幕 1% 亮度", // 82
    "屏幕 25% 亮度", // 83
    "屏幕 50% 亮度", // 84
    "AM 高频信号偏移", // 85
    "信号单位", // 86
    "使能 AF", // 87
    "选择 FM 波段", // 97
    "选择 AM 波段", // 98
    "长波", // 99
    "中波", // 100
    "短波", // 101
    "调频", // 102
    "OIRT", // 103
    "熄屏操作飞梭", // 104
    "硬件型号选择", // 105
    "原版", // 106
    "便携版", // 107
    "便携电阻触摸版", // 108
    "Title", // 109
    "Album", // 110
    "Track", // 111
    "Artist", // 112
    "Composition", // 113
    "Movement", // 114
    "Conductor", // 115
    "Composer", // 116
    "Band", // 117
    "Comment", // 118
    "Genre", // 119
    "News", // 120
    "Local news", // 121
    "Stockmarket", // 122
    "Sport", // 123
    "Lottery", // 124
    "Horoscope", // 125
    "Daily Diversion", // 126
    "Health", // 127
    "Event", // 128
    "Scene", // 129
    "Cinema", // 130
    "TV", // 131
    "Date/time", // 132
    "Weather", // 133
    "Traffic", // 134
    "Alarm", // 135
    "Advertisement", // 136
    "Website", // 137
    "Other", // 138
    "Short PS", // 139
    "Long PS", // 140
    "Now", // 141
    "Next", // 142
    "Part", // 143
    "Host", // 144
    "Editor", // 145
    "Frequency", // 146
    "Homepage", // 147
    "Subchannel", // 148
    "Hotline", // 149
    "Studio phone", // 150
    "Phone", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email hotline", // 154
    "Email studio", // 155
    "Email", // 156
    "Chat", // 157
    "Chat centre", // 158
    "Vote question", // 159
    "Vote centre", // 160
    "Place", // 161
    "Appointment", // 162
    "Identifier", // 163
    "Purchase", // 164
    "获取数据", // 165
    "中波默认步进", // 166
    "橘红", // 167
    "海蓝", // 168
    "靛青", // 169
    "显示电池", // 170
    "电压值", // 171
    "电池百分比", // 172
    "RT 缓存", // 173
    "主要设置", // 174
    "音频设置", // 175
    "显示设置", // 176
    "RDS 设置", // 177
    "FM 设置", // 178
    "AM 设置", // 179
    "上位机 & 时钟", // 180
    "按 MODE 键返回", // 181
    "AM 载波跟踪", // 182
    "设置跟踪灵敏度", // 183
    "AM 载波跟踪层级", // 184
    "设置跟踪层级", // 185
    "主设置", // 186
    "音频设置", // 187
    "显示设置", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "连接&时钟", // 192
    "DX模式", // 193
    "扫描自动存台", // 194
    "AM AGC", // 195
    "FM去加重", // 196
    "小", // 197
    "大", // 198
    "全部", // 199
    "快速 PS", // 200
    "默认", // 201
    "XDRGTK 连结时冻结屏幕", // 202
    "FMSI 立体声加强", // 203
    "此功能仅限 TEF6687/6689!", // 204
    "此主题不支持该功能!", // 205
    "开始扫描存储信道", // 206
    "停止扫描存储信道", // 207
    "扫描等待时间", // 208
    "开始 DX 扫描", // 209
    "FM DX 选项", // 210
    "取消! 所选信道被跳过.", // 211
    "自动排列 AF", // 212
    "已存储信道", // 213
    "不扫描", // 214
    "只扫描", // 215
    "自动取消扫描", // 216
    "正确的 PI", // 217
    "信号", // 218
    "扫描时静音", // 219
    "航空", // 220
    "按下波段键 5秒", // 221
    "关机", // 222
    "熄屏", // 223
    "禁止", // 224
    "无节目", // 225
    "News", // 226
    "Current Affairs", // 227
    "Information", // 228
    "Sport", // 229
    "Education", // 230
    "Drama", // 231
    "Culture", // 232
    "Science", // 233
    "Varied", // 234
    "Pop Music", // 235
    "Rock Music", // 236
    "Easy Listening", // 237
    "Light Classical", // 238
    "Serious Classical", // 239
    "Other Music", // 240
    "Weather", // 241
    "Finance", // 242
    "Children's Progs", // 243
    "Social Affairs", // 244
    "Religion", // 245
    "Phone-In", // 246
    "Travel", // 247
    "Leisure", // 248
    "Jazz Music", // 249
    "Country Music", // 250
    "National Music", // 251
    "Oldies Music", // 252
    "Folk Music", // 253
    "Documentary", // 254
    "警报测试", // 255
    "警报!!!", // 256
    "仅初次", // 257
    "总是", // 258
    "显示时间", // 259
    "显示长的 PS", // 260
    "开始频率", // 261
    "停止频率", // 262
    "开始的记忆信道", // 263
    "停止的记忆信道", // 264
    "只存储RDS电台", // 265
    "开始搜索并存台", // 266
    "自动搜台并存储", // 267
    "电台列表ID", // 268
    "搜索到", // 269
    "取消! 存储已满", // 270
    "取消! 用户终止了操作", // 271
    "成功完成搜索", // 272
    "删除存储信道", // 273
    "删除了", // 274
    "防止双 PI", // 275
    "一定范围内", // 276
    "全部", // 277
    "无信号不停留", // 278
    "触摸区边沿高亮", // 279
    "校准触摸屏", // 280
    "按住BW按钮停止DX扫描仪", // 281
    "选择带宽", // 282
    "你的 log记录", // 283
    "下载 log记录", // 284
    "log为空", // 285
    "跳到底部", // 286
    "添加到log记录", // 287
    "更新失败!", // 288
    "清除log记录", // 289
    "log记录已清除", // 290
    "清除失败", // 291
    "log记录已满!", // 292
    "自动记录日志", // 293
    "联网自动同步本机时间", // 294
    "设置NTP时区", // 295
    "该日志包含超过130个条目，观看者无法处理。请下载CSV文件进行处理。", // 296
    "加拿大" // 297
  },

  { "Norsk", // Norwegian
    "Rotasjonsretning endret", // 1
    "Vennligst slipp knappen", // 2
    "Skjermen ble snudd", // 3
    "Kalibrer analog meter", // 4
    "USB modus", // 5
    "Tuner: !Ingen!", // 6
    "Lydstyrke", // 7
    "Omformer", // 8
    "Lav båndgrense", // 9
    "Høy båndgrense", // 10
    "FM RF-nivåforskyvning", // 11
    "Stereo terskel", // 12
    "Høyeste grense", // 13
    "Høyeste grense", // 14
    "Laveste grense", // 15
    "Kontrast", // 16
    "Sett lyd", // 17
    "Omformeroffset", // 18
    "Lav båndgrense", // 19
    "Høy båndgrense", // 20
    "Nivåoffset", // 21
    "Stereo separasjons\nterskel", // 22
    "Høy grensefrekvens", // 23
    "Høy grenseterskel", // 24
    "Lav grenseterskel", // 25
    "Skjermlysstyrke", // 26
    "Av", // 27
    "På", // 28
    "TRYKK MODE FOR Å LAGRE OG AVSLUTTE", // 29
    "TRYKK BAND FOR Å LUKKE", // 30
    "SKANNER...", // 31
    "Tuner ikke oppdaget", // 32
    "AM-antenneforsterkning", // 33
    "FM AGC", // 34
    "Vis RDS-feil", // 35
    "Språk", // 36
    "Velg språk", // 37
    "PI-låst CT-synk", // 38
    "Feil! Ingen RDS-signal", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Pip ved båndkant", // 42
    "Region", // 43
    "Europa", // 44
    "USA", // 45
    "Vis understrek i RDS", // 46
    "Kun PS", // 47
    "Wi-Fi aktiv", // 48
    "Sett opp Wi-Fi", // 49
    "Koblet til: ", // 50
    "for å sette opp Wi-Fi", // 51
    "Prøver å koble til Wi-Fi", // 52
    "FEILET!", // 53
    "TILKOBLET!", // 54
    "Stationlist klient-IP", // 55
    "Vis SW-bånde", // 56
    "RDS-filter", // 57
    "Vis PI-feil", // 58
    "Bruk squelch", // 59
    "Lydmåler", // 60
    "AM-støydemper", // 61
    "FM-støydemper", // 62
    "Standarder lastet", // 63
    "Lydutgang", // 64
    "Tillat tuning uten å\ndeaktivere skjermsparer", // 65
    "Inverter skjermfarger", // 66
    "Om programvaren", // 67
    "Hovedkode:", // 68
    "Bidragsytere:", // 69
    "Ukjent", // 70
    "Skjermspareralternativer", // 71
    "Auto avslåing", // 72
    "Skjerm av", // 73
    "Tema", // 74
    "Skin", // 75
    "Oppdaget applikasjoner", // 76
    "Min.", // 77
    "SPI-hastighet", // 78
    "Skannefølsomhet", // 79
    "INGEN", // 80
    "Programvareversjon", // 81
    "Frekvensskrifttype", // 82
    "Auto", // 83
    "Ingen AF", // 84
    "Ingen EON", // 85
    "Ingen RT+", // 86
    "FM standard\ntrinnstørrelse", // 78
    "Skjermsparer", // 79
    "Sek.", // 80
    "Klokkemodus", // 81
    "1% lysstyrke", // 82
    "25% lysstyrke", // 83
    "50% lysstyrke", // 84
    "AM RF-nivå offset", // 85
    "Signalmeterenhet", // 86
    "Bruk AF", // 87
    "Velg FM-bånd", // 97
    "Velg AM-bånd", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Stopp skjermvekking ved tuning", // 104
    "Modellvelger", // 105
    "Base", // 106
    "Bærbar", // 107
    "Bærbar touch", // 108
    "Tittel", // 109
    "Album", // 110
    "Spor", // 111
    "Artist", // 112
    "Komposisjon", // 113
    "Bevegelse", // 114
    "Dirigent", // 115
    "Komponist", // 116
    "Band", // 117
    "Kommentar", // 118
    "Sjanger", // 119
    "Nyheter", // 120
    "Lokale nyheter", // 121
    "Aksjemarked", // 122
    "Sport", // 123
    "Lotteri", // 124
    "Horoskop", // 125
    "Daglig avledning", // 126
    "Helse", // 127
    "Begivenhet", // 128
    "Scene", // 129
    "Kino", // 130
    "TV", // 131
    "Dato/tid", // 132
    "Vær", // 133
    "Trafikk", // 134
    "Alarm", // 135
    "Annonse", // 136
    "Nettsted", // 137
    "Annet", // 138
    "Kort PS", // 139
    "Lang PS", // 140
    "Nå", // 141
    "Neste", // 142
    "Del", // 143
    "Vert", // 144
    "Redaktør", // 145
    "Frekvens", // 146
    "Hjemmeside", // 147
    "Underkanal", // 148
    "Hotline", // 149
    "Studio telefon", // 150
    "Telefon", // 151
    "SMS studio", // 152
    "SMS", // 153
    "E-post hotline", // 154
    "E-post studio", // 155
    "E-post", // 156
    "Chat", // 157
    "Chattesenter", // 158
    "Stem spørsmål", // 159
    "Stemmesenter", // 160
    "Sted", // 161
    "Avtale", // 162
    "Identifikator", // 163
    "Kjøp", // 164
    "Få data", // 165
    "MW standard\ntrinnstørrelse", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Batterialternativer", // 170
    "Vis spenning", // 171
    "Vis prosent", // 172
    "RT buffring", // 173
    "Hovedinnstillinger", // 174
    "Lydinnstillinger", // 175
    "Skjerminnstillinger", // 176
    "RDS innstillinger", // 177
    "FM innstillinger", // 178
    "AM innstillinger", // 179
    "Tilkobling og klokke", // 180
    "TRYKK MODE FOR Å RETURNERE", // 181
    "CoChannel-detektor", // 182
    "CoChannel-detektor\nfølsomhet", // 183
    "CoChannel-detektor\nterskel", // 184
    "CoChannel-detektor\nteller", // 185
    "HOVED", // 186
    "LYD", // 187
    "SKJERM", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "TILKOBLE", // 192
    "DX MODUS", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM deemphasis", // 196
    "Lite", // 197
    "Mye", // 198
    "Alt", // 199
    "Rask PS", // 200
    "Standard", // 201
    "Demp skjerm under\nXDRGTK-tilkobling", // 202
    "FMSI stereo forbedring", // 203
    "Funksjon kun tilgjengelig\npå TEF6687/6689!", // 204
    "Funksjon ikke tilgjengelig\npå valgt skin!", // 205
    "Start skanning av\nminnekanaler", // 206
    "Stopp skanning av\nminnekanaler", // 207
    "Ventetid", // 208
    "Start DX-skanning", // 209
    "FM DX-alternativer", // 210
    "Avbryt! Startkanalen er\nmerket som hoppet over", // 211
    "Auto-sorter AF", // 212
    "Minnekanaler", // 213
    "Ekskluder", // 214
    "Kun", // 215
    "Auto-avbryt skanning", // 216
    "Korriger PI", // 217
    "Signal", // 218
    "Demp lyd under\nskanning", // 219
    "AIR", // 220
    "Hold BAND-knappen i 5 sek", // 221
    "Slå av", // 222
    "Slå av skjerm", // 223
    "Deaktivert", // 224
    "Ingen", // 225
    "Nyheter", // 226
    "Aktualiteter", // 227
    "Informasjon", // 228
    "Sport", // 229
    "Utdanning", // 230
    "Drama", // 231
    "Kultur", // 232
    "Vitenskap", // 233
    "Underholdning", // 234
    "Popmusikk", // 235
    "Rockmusikk", // 236
    "Lett musikk", // 237
    "Lett klassisk", // 238
    "Klassisk musikk", // 239
    "Annen musikk", // 240
    "Værmelding", // 241
    "Økonomi", // 242
    "Barneprogrammer", // 243
    "Sosiale spørsmål", // 244
    "Religion", // 245
    "Ring inn", // 246
    "Reise", // 247
    "Fritid", // 248
    "Jazz", // 249
    "Countrymusikk", // 250
    "Nasjonal musikk", // 251
    "Gammelpop", // 252
    "Folkemusikk", // 253
    "Dokumentar", // 254
    "Alarmtest", // 255
    "ALARM!", // 256
    "Innledende", // 257
    "Alltid", // 258
    "Vis klokke", // 259
    "Vis lang PS", // 260
    "Startfrekvens", // 261
    "Stoppfrekvens", // 262
    "Start minnekanal", // 263
    "Stopp minnekanal", // 264
    "Lagre kun med RDS", // 265
    "Start auto-minne", // 266
    "Auto-minne", // 267
    "Stationlist-ID", // 268
    "Funnet", // 269
    "Avbrutt! Ingen minnekanaler", // 270
    "Avbrutt! Bruker trykket knapp", // 271
    "Skanning fullført uten feil", // 272
    "Slett minnekanaler", // 273
    "ryddet", // 274
    "Forhindre doble PI-er", // 275
    "Spekter", // 276
    "Fullt", // 277
    "Ventetid kun\nved signal", // 278
    "Trykk på uthevet hjørne", // 279
    "Kalibrer berøringsskjerm", // 280
    "Hold BW-knappen for å\nstoppe DX-skanning", // 281
    "Velg båndbredde", // 282
    "Din loggbok", // 283
    "Last ned loggbok", // 284
    "Loggbok er tom", // 285
    "Gå til bunn", // 286
    "Oppføring lagt til i loggen", // 287
    "Feil ved lagring i loggen", // 288
    "Tøm loggbok", // 289
    "Loggbok tømt", // 290
    "Tømming mislyktes", // 291
    "Kan ikke lagre!\nLoggbok er full!", // 292
    "Autologger", // 293
    "Auto sommertid med NTP", // 294
    "Angi NTP-tidsforskyvning", // 295
    "Loggbok har over 130 oppføringer.\nLast ned CSV for behandling.", // 296
    "Canada" // 297
  },

  { "Español", // Spanish
    "Dirección rotativa\ncambiada", // 1
    "Suelte el botón", // 2
    "Pantalla girada", // 3
    "Calibrar medidor\nanalógico", // 4
    "Modo USB", // 5
    "Sintonizador: ¡Ninguno!", // 6
    "Volumen", // 7
    "Convertidor", // 8
    "Banda baja", // 9
    "Banda alta", // 10
    "Desplazamiento\nRF FM", // 11
    "Umbral estéreo", // 12
    "Frecuencia de\ncorte alto", // 13
    "Umbral de corte alto", // 14
    "Umbral de nivel bajo", // 15
    "Brillo", // 16
    "Ajustar volumen", // 17
    "Desplazamiento\ndel convertidor", // 18
    "Límite inferior de banda", // 19
    "Límite superior de banda", // 20
    "Desplazamiento de nivel", // 21
    "Umbral separación\nestéreo", // 22
    "Frecuencia de\ncorte alto", // 23
    "Umbral de corte alto", // 24
    "Umbral de nivel bajo", // 25
    "Brillo de pantalla", // 26
    "Apagado", // 27
    "Encendido", // 28
    "PRESIONE MODO PARA GUARDAR Y SALIR", // 29
    "PRESIONE BANDA PARA CERRAR", // 30
    "ESCANEANDO...", // 31
    "Sintonizador no detectado", // 32
    "Ganancia antena AM", // 33
    "AGC FM", // 34
    "Mostrar errores RDS", // 35
    "Idioma", // 36
    "Elegir idioma", // 37
    "Sincronización PI-CT", // 38
    "¡Error! Sin señal RDS", // 39
    "Silencio FM", // 40
    "Silencio AM", // 41
    "Tono en límite de banda", // 42
    "Región", // 43
    "Europa", // 44
    "Estados Unidos", // 45
    "Mostrar guion\nbajo en RDS", // 46
    "Solo PS", // 47
    "Wi-Fi activado", // 48
    "Configurar Wi-Fi", // 49
    "Conectar a: ", // 50
    "para configurar Wi-Fi", // 51
    "Conectando a Wi-Fi...", // 52
    "¡FALLÓ!", // 53
    "¡CONECTADO!", // 54
    "IP cliente\nStationlist", // 55 (Removed line break)
    "Mostrar longitud\nde onda SW", // 56
    "Filtro RDS", // 57
    "Mostrar errores PI", // 58
    "Usar squelch", // 59
    "Medidor de modulación", // 60
    "Supresor de\nruido AM", // 61
    "Supresor de\nruido FM", // 62
    "Valores predeterminados\ncargados", // 63
    "Salida de audio", // 64
    "Sintonizar sin\ndesactivar protector", // 65 (Matched line break)
    "Invertir colores\nde pantalla", // 66
    "Acerca del software", // 67
    "Código principal:", // 68
    "Colaboradores:", // 69
    "Desconocido", // 70
    "Opciones\nde protector", // 71
    "Apagado automático", // 72
    "Pantalla apagada", // 73
    "Tema", // 74
    "Skin", // 75
    "Aplicaciones detectadas", // 76
    "Mín.", // 77
    "Velocidad SPI", // 78
    "Sensibilidad\nde escaneo", // 79
    "NINGUNO", // 80
    "Versión del software", // 81
    "Fuente de frecuencia", // 82
    "Automático", // 83
    "AF no disponible", // 84
    "EON no disponible", // 85
    "RT+ no disponible", // 86
    "Paso predeterminado\nFM", // 78
    "Protector\nde pantalla", // 79
    "Seg.", // 80
    "Modo reloj", // 81
    "Brillo al 1%", // 82
    "Brillo al 25%", // 83
    "Brillo al 50%", // 84
    "Desplazamiento\nRF AM", // 85
    "Unidad de medidor", // 86
    "Usar AF", // 87
    "Seleccionar\nbanda FM", // 97
    "Seleccionar\nbanda AM", // 98
    "LW", // 99
    "AM", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "No reactivar pantalla\nal sintonizar", // 104
    "Selector de modelo", // 105
    "Base", // 106
    "Portátil", // 107
    "Portátil táctil", // 108
    "Título", // 109
    "Álbum", // 110
    "Pista", // 111
    "Artista", // 112
    "Composición", // 113
    "Movimiento", // 114
    "Director", // 115
    "Compositor", // 116
    "Banda", // 117
    "Comentario", // 118
    "Género", // 119
    "Noticias", // 120
    "Noticias locales", // 121
    "Mercado bursátil", // 122
    "Deportes", // 123
    "Lotería", // 124
    "Horóscopo", // 125
    "Entretenimiento diario", // 126
    "Salud", // 127
    "Evento", // 128
    "Escena", // 129
    "Cine", // 130
    "TV", // 131
    "Fecha/hora", // 132
    "Clima", // 133
    "Tráfico", // 134
    "Alarma", // 135
    "Publicidad", // 136
    "Sitio web", // 137
    "Otro", // 138
    "PS corto", // 139
    "PS largo", // 140
    "Ahora", // 141
    "Siguiente", // 142
    "Parte", // 143
    "Presentador", // 144
    "Editor", // 145
    "Frecuencia", // 146
    "Página principal", // 147
    "Subcanal", // 148
    "Línea directa", // 149
    "Teléfono de estudio", // 150
    "Teléfono", // 151
    "SMS de estudio", // 152
    "SMS", // 153
    "Correo de línea directa", // 154
    "Correo de estudio", // 155
    "Correo electrónico", // 156
    "Chat", // 157
    "Centro de chat", // 158
    "Pregunta de votación", // 159
    "Centro de votación", // 160
    "Lugar", // 161
    "Cita", // 162
    "Identificador", // 163
    "Compra", // 164
    "Obtener datos", // 165
    "Paso\npredeterminado AM", // 166
    "Mandarina", // 167
    "Océano", // 168
    "Índigo", // 169
    "Opciones de batería", // 170
    "Mostrar voltaje", // 171
    "Mostrar porcentaje", // 172
    "Buffer RT", // 173
    "Ajustes principales", // 174
    "Ajustes de audio", // 175
    "Ajustes de pantalla", // 176
    "Ajustes RDS", // 177
    "Ajustes FM", // 178
    "Ajustes AM", // 179
    "Conectividad y reloj", // 180
    "PRESIONE MODO PARA VOLVER", // 181
    "Detector Co-Canal", // 182
    "Sensibilidad\nCo-Canal", // 183
    "Umbral Co-Canal", // 184
    "Contador Co-Canal", // 185
    "PRINCIPAL", // 186
    "AUDIO", // 187
    "PANTALLA", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CONECTAR", // 192
    "MODO DX", // 193
    "MEM AUTOMÁTICA", // 194
    "AGC AM", // 195
    "Deénfasis FM", // 196
    "Pequeño", // 197
    "Grande", // 198
    "Todo", // 199
    "PS rápido", // 200
    "Predeterminado", // 201
    "Silenciar pantalla\nen conexión XDRGTK", // 202
    "Mejora estéreo FMSI", // 203
    "¡Función solo para\nTEF6687/6689!", // 204
    "¡Función no disponible\nen skin seleccionada!", // 205
    "Iniciar escaneo\nde memorias", // 206
    "Detener escaneo\nde memorias", // 207
    "Tiempo de espera", // 208
    "Iniciar escaneo DX", // 209
    "Opciones FM DX", // 210
    "¡Abortar! Canal inicial\nmarcado como omitido", // 211
    "Ordenar AF automático", // 212
    "Canales guardados", // 213
    "Excluir", // 214
    "Solo", // 215
    "Cancelaciòn automàtica", // 216
    "PI correcto", // 217
    "Señal", // 218
    "Silenciar audio\ndurante escaneo", // 219
    "AIR", // 220
    "Mantener BANDA 5 seg.", // 221
    "Apagar", // 222
    "Apagar pantalla", // 223
    "Desactivado", // 224
    "Ninguno", // 225
    "Noticias", // 226
    "Magazine", // 227
    "Información", // 228
    "Deportes", // 229
    "Educación", // 230
    "Drama", // 231
    "Cultura", // 232
    "Ciencia", // 233
    "Variedades", // 234
    "Música pop", // 235
    "Música rock", // 236
    "Música ligera", // 237
    "Clásica ligera", // 238
    "Clásica seria", // 239
    "Otros géneros", // 240
    "Meteorología", // 241
    "Finanzas", // 242
    "Infantiles", // 243
    "Sociedad", // 244
    "Religión", // 245
    "Llamadas en vivo", // 246
    "Viajes", // 247
    "Ocio", // 248
    "Jazz", // 249
    "Country", // 250
    "Música nacional", // 251
    "Música de Ayer", // 252
    "Folclórica", // 253
    "Documental", // 254
    "Prueba de alarma", // 255
    "¡ALARMA!", // 256
    "Inicial", // 257
    "Siempre", // 258
    "Mostrar reloj", // 259
    "Mostrar PS largo", // 260
    "Frecuencia inicial", // 261
    "Frecuencia final", // 262
    "Canal inicial", // 263
    "Canal final", // 264
    "Guardar solo\ncon RDS", // 265
    "Iniciar memoria\nautomática", // 266
    "Memoria automática", // 267
    "ID Stationlist", // 268
    "Encontrado", // 269
    "¡Error! Sin memorias", // 270
    "¡Cancelado! Usuario\ndetuvo", // 271
    "Escaneo exitoso", // 272
    "Borrar memorias", // 273
    "borrado", // 274
    "Evitar PI duplicado", // 275
    "Rango", // 276
    "Completo", // 277
    "Esperar solo\ncon señal", // 278
    "Toque la esquina\nresaltada", // 279
    "Calibrar pantalla\ntáctil", // 280
    "Mantenga BW para\ndetener DX", // 281
    "Seleccionar ancho\nde banda", // 282
    "Libro de registro", // 283
    "Descargar registro", // 284
    "Registro vacío", // 285
    "Ir al final", // 286
    "Entrada añadida", // 287
    "Error al añadir", // 288
    "Borrar registro", // 289
    "Registro borrado", // 290
    "Error al borrar", // 291
    "¡Registro lleno!\nNo se puede añadir", // 292
    "Autoregistro", // 293
    "DST automático (NTP)", // 294
    "Ajustar\ndesplazamiento NTP", // 295
    "El registro tiene +130 entradas.\nDescargue el CSV.", // 296
    "Canadá" // 297
  },

  { "Português", // Portuguese
    "Direção rotativa\nalterada", // 1
    "Solte o botão,\npor favor", // 2
    "Tela invertida", // 3
    "Calibrar o medidor\nanalógico", // 4
    "Modo USB", // 5
    "Sintonizador: !Nenhum!", // 6
    "Volume", // 7
    "Conversor", // 8
    "Borda da faixa baixa", // 9
    "Borda da faixa alta", // 10
    "Deslocamento do nível\nde RF FM", // 11
    "Limiar estéreo", // 12
    "Canto de corte alto", // 13
    "Limiar de corte alto", // 14
    "Limiar de nível baixo", // 15
    "Brilho", // 16
    "Definir volume", // 17
    "Definir deslocamento\ndo conversor", // 18
    "Definir borda\nda faixa baixa", // 19
    "Definir borda\nda faixa alta", // 20
    "Definir deslocamento\nde nível", // 21
    "Limiar de separação\nestéreo", // 22
    "Frequência do canto\nde corte alto", // 23
    "Definir limiar\nde corte alto", // 24
    "Definir limiar\nde nível baixo", // 25
    "Definir brilho\nda tela", // 26
    "Desligado", // 27
    "Ligado", // 28
    "PRESSIONE MODE PARA SAIR E SALVAR", // 29
    "PRESSIONE BAND PARA FECHAR", // 30
    "ANALISANDO...", // 31
    "Sintonizador não detectado", // 32
    "AM Ganho da antena", // 33
    "FM AGC", // 34
    "Mostrar erros RDS", // 35
    "Idioma", // 36
    "Escolha o idioma", // 37
    "PI bloqueado CT sync", // 38
    "Erro! Sem sinal RDS", // 39
    "Silenciamento suave FM", // 40
    "Silenciamento suave AM", // 41
    "Bipe na borda da faixa", // 42
    "Região", // 43
    "Europa", // 44
    "Estados Unidos", // 45
    "Mostrar sublinhado\nno RDS", // 46
    "PS apenas", // 47
    "Wi-Fi ativo", // 48
    "Configurar Wi-Fi", // 49
    "Conectar a: ", // 50
    "para configurar\no Wi-Fi", // 51
    "Tentando conectar\nao Wi-Fi", // 52
    "FALHA!", // 53
    "CONECTADO!", // 54
    "IP do cliente da lista\nde estações", // 55
    "Mostrar comprimento\nde onda SW", // 56
    "Filtro RDS", // 57
    "Mostrar erros PI", // 58
    "Usar squelch", // 59
    "Medidor de modulação", // 60
    "Supressor de ruído AM", // 61
    "Supressor de ruído FM", // 62
    "Padrões carregados", // 63
    "Saída de áudio", // 64
    "Permitir sintonização\nsem desativar proteção", // 65
    "Inverter a tela", // 66
    "Sobre o software", // 67
    "Código principal:", // 68
    "Contribuidores:", // 69
    "Desconhecido", // 70
    "Opções de proteção\nde tela", // 71
    "Desligamento automático", // 72
    "Desligar a tela", // 73
    "Tema", // 74
    "Skin", // 75
    "Aplicativos detectados", // 76
    "Min.", // 77
    "Velocidade de SPI", // 78
    "Sensibilidade\nda varredura", // 79
    "NENHUM", // 80
    "Versão do software", // 81
    "Fonte de frequência", // 82
    "Auto", // 83
    "AF não disponível", // 84
    "EON não disponível", // 85
    "RT+ não disponível", // 86
    "Passo padrão FM", // 78
    "Proteção de tela", // 79
    "Segundos", // 80
    "Modo de relógio", // 81
    "1% de brilho", // 82
    "25% de brilho", // 83
    "50% de brilho", // 84
    "Deslocamento de nível\nde RF AM", // 85
    "Unidade de medidor\nde sinal", // 86
    "Usar AF", // 87
    "Selecionar faixa FM", // 97
    "Selecionar faixa AM", // 98
    "LW", // 99
    "AM", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Parar tela de despertar\nao sintonizar", // 104
    "Seletor de modelo", // 105
    "Básico", // 106
    "Portátil", // 107
    "Toque portátil", // 108
    "Título", // 109
    "Álbum", // 110
    "Faixa", // 111
    "Artista", // 112
    "Composição", // 113
    "Movimento", // 114
    "Maestro", // 115
    "Compositor", // 116
    "Banda", // 117
    "Comentário", // 118
    "Gênero", // 119
    "Notícias", // 120
    "Notícias locais", // 121
    "Mercado de ações", // 122
    "Esportes", // 123
    "Loteria", // 124
    "Horóscopo", // 125
    "Diversão diária", // 126
    "Saúde", // 127
    "Evento", // 128
    "Cena", // 129
    "Cinema", // 130
    "TV", // 131
    "Data/hora", // 132
    "Clima", // 133
    "Tráfego", // 134
    "Alarme", // 135
    "Anúncio", // 136
    "Website", // 137
    "Outros", // 138
    "PS curto", // 139
    "PS longo", // 140
    "Agora", // 141
    "Próximo", // 142
    "Parte", // 143
    "Apresentador", // 144
    "Editor", // 145
    "Frequência", // 146
    "Página inicial", // 147
    "Subcanal", // 148
    "Hotline", // 149
    "Telefone do estúdio", // 150
    "Telefone", // 151
    "SMS do estúdio", // 152
    "SMS", // 153
    "E-mail da hotline", // 154
    "E-mail do estúdio", // 155
    "E-mail", // 156
    "Chat", // 157
    "Centro de chat", // 158
    "Pergunta de votação", // 159
    "Centro de votação", // 160
    "Local", // 161
    "Compromisso", // 162
    "Identificador", // 163
    "Compra", // 164
    "Obter dados", // 165
    "Tamanho padrão\nda etapa AM", // 166
    "Tangerina", // 167
    "Oceano", // 168
    "Índigo", // 169
    "Opções de bateria", // 170
    "Mostrar voltagem", // 171
    "Mostrar porcentagem", // 172
    "Buffering RT", // 173
    "Configurações principais", // 174
    "Configurações de áudio", // 175
    "Configurações de exibição", // 176
    "Configurações RDS", // 177
    "Configurações FM", // 178
    "Configurações AM", // 179
    "Conectividade e relógio", // 180
    "PRESSIONE MODO PARA VOLTAR", // 181
    "Detector CoChannel", // 182
    "Sensibilidade do\nCoChannel Det", // 183
    "Limiar do\nCoChannel Det", // 184
    "Contador do\nCoChannel Det", // 185
    "PRINCIPAL", // 186
    "ÁUDIO", // 187
    "EXIBIÇÃO", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "CONECTAR", // 192
    "DX MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "Desênfase FM", // 196
    "Pequeno", // 197
    "Grande", // 198
    "Tudo", // 199
    "PS rápido", // 200
    "Predefinição", // 201
    "Tela mudo na\nconexão XDRGTK", // 202
    "FMSI melhoria\ndo som estéreo", // 203
    "Função disponível apenas\nno TEF6687/6689!", // 204
    "Função não disponível\nno skin selecionado!", // 205
    "Comece a pesquisar\ncanais de memória", // 206
    "Pare de pesquisar\ncanais de memória", // 207
    "Tempo de espera", // 208
    "Comece a pesquisar DX", // 209
    "Opções FM DX", // 210
    "Abortar! O canal\ninicial é ignorado.", // 211
    "Classificar AF\nautomaticamente", // 212
    "Canais memorizados", // 213
    "Excluir", // 214
    "Apenas", // 215
    "Cancelar automatic.\na pesquisa", // 216
    "PI correto", // 217
    "Sinal", // 218
    "Mudo áudio\nenquanto procura", // 219
    "AIR", // 220
    "Pressione o botão\nbanda por 5 seg.", // 221
    "Desligar", // 222
    "Tela desligado", // 223
    "Inativo", // 224
    "Não definido", // 225
    "Notícias", // 226
    "Actualidade", // 227
    "Informação", // 228
    "Desporto", // 229
    "Educação", // 230
    "Rádio – Teatro", // 231
    "Cultura", // 232
    "Ciência", // 233
    "Variado", // 234
    "Música Pop", // 235
    "Música Rock", // 236
    "Música Ligeira", // 237
    "Clássica Ligeira", // 238
    "Música Clássica", // 239
    "Outras Músicas", // 240
    "Meteorologia", // 241
    "Economia", // 242
    "Progr. Infantil", // 243
    "Acontec. Social", // 244
    "Religião", // 245
    "Phone-In", // 246
    "Viagens", // 247
    "Lazer", // 248
    "Música de Jazz", // 249
    "Música Country", // 250
    "Música Nacional", // 251
    "Música Antiga", // 252
    "Música Folk", // 253
    "Documentário", // 254
    "Teste de Alarme", // 255
    "Alarme – Alarme !", // 256
    "Inicial", // 257
    "Sempre", // 258
    "Mostrar relógio", // 259
    "Mostrar PS longo", // 260
    "Iniciar frequência", // 261
    "Parar frequência", // 262
    "Iniciar canal\nde memória", // 263
    "Parar canal de memória", // 264
    "Somente armazenar\ncom RDS", // 265
    "Iniciar memória automática", // 266
    "Memória automática", // 267
    "ID da lista de estações", // 268
    "Encontrado", // 269
    "Abortar! Sem canais\nde memória disponíveis", // 270
    "Abortar! Usuário\npressionou o botão", // 271
    "Verificação concluída\nsem erros", // 272
    "Limpar canais\nde memória", // 273
    "limpo", // 274
    "Prevenir PI duplo", // 275
    "Intervalo", // 276
    "Completo", // 277
    "Espera tempo apenas\nquando sinal", // 278
    "Pressione o\ncanto inscrito", // 279
    "Para calibração\nda tela de toque", // 280
    "Segure BW para parar", // 281
    "Selecione largura de banda", // 282
    "Seu livro de registro", // 283
    "Baixar livro de registro", // 284
    "O livro de registro está vazio", // 285
    "Ir para o final", // 286
    "Adicionado ao registro", // 287
    "Falha na atualização!", // 288
    "Limpar livro\nde registro", // 289
    "Livro limpo", // 290
    "Falha ao limpar", // 291
    "Não é possível adicionar!\nO livro está cheio!", // 292
    "Registrador automático", // 293
    "DST automático\nno horário NTP", // 294
    "Definir deslocamento\nde tempo NTP", // 295
    "O diário de bordo contém mais de 130 entradas que o espectador não consegue processar. Descarregue o ficheiro CSV para processá-lo.", // 296
    "Canadá" // 297
  },

  { "Srpski", // Serbian
    "Smer rotacije promenjen", // 1
    "Pustite dugme", // 2
    "Ekran okrenut", // 3
    "Kalibracija analognog\nmerača", // 4
    "Mod USB", // 5
    "Tuner: !Nema!", // 6
    "Jačina zvuka", // 7
    "Konverter", // 8
    "Donja ivica opsega", // 9
    "Gornja ivica opsega", // 10
    "FM RF nivo\npomeranja", // 11
    "Stereo prag", // 12
    "Visokopropusni kut", // 13
    "Visokopropusni\nprag", // 14
    "Prag za nizak\nnivo", // 15
    "Osvetljenost", // 16
    "Postavite jačinu zvuka", // 17
    "Pomeranje konvertera", // 18
    "Donja ivica opsega", // 19
    "Gornja ivica opsega", // 20
    "Pomeranje nivoa", // 21
    "Stereo odvajanje prag", // 22
    "Frekvencija\nvisokog rezanja", // 23
    "Prag visokog rezanja", // 24
    "Prag niskog nivoa", // 25
    "Osvetljenost ekrana", // 26
    "Isključeno", // 27
    "Uključeno", // 28
    "PRITISNITE MODE ZA IZLAZ I SPAS", // 29
    "PRITISNITE BAND DA ZATVORITE", // 30
    "SKENIRANJE...", // 31
    "Tuner nije\ndetektovan", // 32
    "AM antena\npojačanje", // 33
    "FM AGC", // 34
    "Prikazivanje RDS\ngrešaka", // 35
    "Jezik", // 36
    "Izaberite jezik", // 37
    "PI zaključan\nCT sinhronizacija", // 38
    "Greška! Nema RDS signala!", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Pucketanje na\nivici opsega", // 42
    "Region", // 43
    "Evropa", // 44
    "S.A.D.", // 45
    "Prikazivanje\ndonje crte u RDS", // 46
    "PS samo", // 47
    "Wi-Fi aktivan", // 48
    "Konfiguriši Wi-Fi", // 49
    "Povežite se sa: ", // 50
    "da biste konfigurisali Wi-Fi", // 51
    "Pokušavam da se\npovežem sa Wi-Fi", // 52
    "NEUSPELO!", // 53
    "POVEZANO!", // 54
    "IP Stationlist klijent", // 55
    "Prikazivanje SW\ntalasne dužine", // 56
    "RDS filter", // 57
    "Prikazivanje PI\ngrešaka", // 58
    "Koristi šum", // 59
    "Merač modulisanja", // 60
    "AM Noise blanker", // 61
    "FM Noise blanker", // 62
    "Podrazumevana\npodešavanja učitana", // 63
    "Audio izlaz", // 64
    "Dozvoli podešavanje\nbez screensavera", // 65
    "Invertuj ekran", // 66
    "O softveru", // 67
    "Glavni kod:", // 68
    "Doprinosioci:", // 69
    "Nepoznato", // 70
    "Opcije screensavera", // 71
    "Automatsko\nisključivanje", // 72
    "Isključi ekran", // 73
    "Tema", // 74
    "Koža", // 75
    "Detektovane aplikacije", // 76
    "Min.", // 77
    "SPI brzina", // 78
    "Osetljivost\nskeniranja", // 79
    "NIJE DOSTUPNO", // 80
    "Verzija softvera", // 81
    "Font za\nfrekvenciju", // 82
    "Automatski", // 83
    "Nema AF dostupnog", // 84
    "Nema EON dostupnog", // 85
    "Nema RT+ dostupnog", // 86
    "FM podrazumevani\nkorak", // 78
    "Screensaver", // 79
    "Sek", // 80
    "Način rada sa\nsatom", // 81
    "1% osvetljenost", // 82
    "25% osvetljenost", // 83
    "50% osvetljenost", // 84
    "AM RF nivo\npomeranja", // 85
    "Jedinica\nsignalnog merača", // 86
    "Koristi AF", // 87
    "Izaberi FM\nopseg", // 97
    "Izaberi AM\nopseg", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Zaustavi buđenje ekrana", // 104
    "Selector modela", // 105
    "Baza", // 106
    "Portabilno", // 107
    "Portabilno dodirno", // 108
    "Naslov", // 109
    "Album", // 110
    "Pesma", // 111
    "Izvođač", // 112
    "Kompozicija", // 113
    "Pokret", // 114
    "Dirigent", // 115
    "Kompozitor", // 116
    "Grupa", // 117
    "Komentar", // 118
    "Žanr", // 119
    "Vesti", // 120
    "Lokalne vesti", // 121
    "Berza", // 122
    "Sport", // 123
    "Loterija", // 124
    "Horoskop", // 125
    "Dnevni odmor", // 126
    "Zdravlje", // 127
    "Događaj", // 128
    "Scena", // 129
    "Film", // 130
    "TV", // 131
    "Datum/vreme", // 132
    "Vreme", // 133
    "Saobraćaj", // 134
    "Alarm", // 135
    "Reklama", // 136
    "Web sajt", // 137
    "Ostalo", // 138
    "Kratak PS", // 139
    "Dug PS", // 140
    "Sada", // 141
    "Sledeće", // 142
    "Deo", // 143
    "Domaćin", // 144
    "Uređivač", // 145
    "Frekvencija", // 146
    "Početna stranica", // 147
    "Podkanal", // 148
    "Vruća linija", // 149
    "Studio telefon", // 150
    "Telefon", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Email vruća linija", // 154
    "Email studio", // 155
    "Email", // 156
    "Razgovor", // 157
    "Razgovorni centar", // 158
    "Pitanje za glasanje", // 159
    "Centar za glasanje", // 160
    "Mesto", // 161
    "Sastanak", // 162
    "Identifikator", // 163
    "Kupovina", // 164
    "Preuzmi podatke", // 165
    "MW podrazumevani\nkorak", // 166
    "Tangerine", // 167
    "Okean", // 168
    "Indigo", // 169
    "Opcije baterije", // 170
    "Prikazivanje napona", // 171
    "Prikazivanje procenta", // 172
    "RT keširanje", // 173
    "Glavna podešavanja", // 174
    "Audio podešavanja", // 175
    "Podešavanja ekrana", // 176
    "RDS podešavanja", // 177
    "FM podešavanja", // 178
    "AM podešavanja", // 179
    "Povezivanje i sat", // 180
    "PRITISNITE MODE DA SE VRATITE", // 181
    "CoChannel Detektor", // 182
    "CoChannel Detektor\nosetljivost", // 183
    "CoChannel Detektor\nprag", // 184
    "CoChannel Detektor\nbrojač", // 185
    "GLAVNO", // 186
    "AUDIO", // 187
    "EKRAN", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "POVEZIVANJE", // 192
    "DX MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM de-emfaza", // 196
    "Malo", // 197
    "Veliko", // 198
    "Svi", // 199
    "Brzi PS", // 200
    "Podrazumevano", // 201
    "Utišaj ekran na XDRGTK", // 202
    "FMSI stereo\npoboljšanje", // 203
    "Funkcija dostupna\nsamo na TEF6687/6689!", // 204
    "Funkcija nije dostupna na koži!", // 205
    "Počnite skeniranje kanala", // 206
    "Zaustavite skeniranje kanala", // 207
    "Vreme čekanja", // 208
    "Pokrenite DX\nskeniranje", // 209
    "FM DX Opcije", // 210
    "Abort! Početni kanal preskočen", // 211
    "Automatski sortiraj AF", // 212
    "Memorijski kanali", // 213
    "Isključi", // 214
    "Samo", // 215
    "Automatski otkaži skeniranje", // 216
    "Ispravi PI", // 217
    "Signal", // 218
    "Utišaj zvuk\nu toku skeniranja", // 219
    "ZRAK", // 220
    "5 sekundi pritiska na dugme", // 221
    "Isključivanje", // 222
    "Isključi ekran", // 223
    "Onemogućeno", // 224
    "Nema", // 225
    "Vesti", // 226
    "Aktuelnosti", // 227
    "Informacije", // 228
    "Sport", // 229
    "Obrazovanje", // 230
    "Drama", // 231
    "Kultura", // 232
    "Nauka", // 233
    "Raznovrsno", // 234
    "Pop muzika", // 235
    "Rock muzika", // 236
    "Lagana muzika", // 237
    "Laka klasična", // 238
    "Seriozna klasična", // 239
    "Ostala muzika", // 240
    "Vreme", // 241
    "Finansije", // 242
    "Dečiji programi", // 243
    "Socijalna pitanja", // 244
    "Religija", // 245
    "Telefon na liniji", // 246
    "Putovanja", // 247
    "Odmor", // 248
    "Jazz muzika", // 249
    "Country muzika", // 250
    "Nacionalna muzika", // 251
    "Stari hitovi", // 252
    "Folk muzika", // 253
    "Dokumentarni", // 254
    "Test alarma", // 255
    "Alarm!!!", // 256
    "Početni", // 257
    "Uvek", // 258
    "Prikazivanje\nsata", // 259
    "Prikazivanje\ndugog PS", // 260
    "Početna frekvencija", // 261
    "Krajnja frekvencija", // 262
    "Početni memorijski\nkanal", // 263
    "Krajnji memorijski\nkanal", // 264
    "Samo sa RDS", // 265
    "Pokreni automatsko\nmemorijsko pamćenje", // 266
    "Automatsko memorisanje", // 267
    "Stationlist ID", // 268
    "Pronađeno", // 269
    "Abort! Nedostaju kanali", // 270
    "Abort! Korisnik je\npritisnuo dugme", // 271
    "Skeniranje završeno\nbez grešaka", // 272
    "Očisti memorijske\nkanale", // 273
    "obrisano", // 274
    "Prevencija duplog PI", // 275
    "Opseg", // 276
    "Puno", // 277
    "Vreme čekanja na signal", // 278
    "Pritisnite označeni\nugao", // 279
    "Za kalibraciju\nekrana", // 280
    "Drži BW da staneš", // 281
    "Izaberi širinu\nopsega", // 282
    "Tvoj dnevnik", // 283
    "Preuzmi dnevnik", // 284
    "Dnevnik je prazan", // 285
    "Idi do dna", // 286
    "Dodano u dnevnik", // 287
    "Ažuriranje nije\nuspelo!", // 288
    "Očisti dnevnik", // 289
    "Dnevnik očišćen", // 290
    "Greška pri brisanju", // 291
    "Ne može se dodati!\nDnevnik je pun!", // 292
    "Automatski logger", // 293
    "Auto DST na\nNTP vremenu", // 294
    "Postavite NTP\nvremenski offset", // 295
    "Dnevnik sadrži više od 130 unosa, što viewer ne može obraditi. Preuzmite CSV fajl za obradu.", // 296
    "Canada" // 297
  },

  { "Suomi", // Finnish
    "Pyörimisohjauksen suunta\nmuutettu", // 1
    "Paina nappia vapauttaaksesi", // 2
    "Näyttö käännetty", // 3
    "Kalibroi analoginen mittari", // 4
    "USB-tila", // 5
    "Viritin: !Ei mitään!", // 6
    "Äänenvoimakkuus", // 7
    "Muunna", // 8
    "Matalan taajuuden\nraja", // 9
    "Korkean taajuuden\nraja", // 10
    "FM RF-tason siirto", // 11
    "Stereo kynnys", // 12
    "Ylileikkaus kulma", // 13
    "Ylileikkaus kynnys", // 14
    "Matalan tason\nkynnys", // 15
    "Kirkkaus", // 16
    "Aseta äänenvoimakkuus", // 17
    "Muunna siirto", // 18
    "Matalan taajuuden\nraja", // 19
    "Korkean taajuuden\nraja", // 20
    "Tason siirto", // 21
    "Stereoerotus kynnys", // 22
    "Ylileikkaus kulman taajuus", // 23
    "Ylileikkaus kynnys", // 24
    "Matalan tason\nkynnys", // 25
    "Näytön kirkkaus", // 26
    "Pois", // 27
    "Päällä", // 28
    "PAINA MODE POISTU JA TALLENNA", // 29
    "PAINA BAND SULKEAKSESI", // 30
    "SKANNATAAN...", // 31
    "Virittimen havaitseminen\nepäonnistui", // 32
    "AM-antenni vahvistus", // 33
    "FM AGC", // 34
    "Näytä RDS-virheet", // 35
    "Kieli", // 36
    "Valitse kieli", // 37
    "PI lukittu CT synkronointi", // 38
    "Virhe! Ei RDS-signaalia", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Ääni rajalla", // 42
    "Alue", // 43
    "Eurooppa", // 44
    "Yhdysvallat", // 45
    "Näytä alaviiva RDS:ssa", // 46
    "Vain PS", // 47
    "Wi-Fi aktiivinen", // 48
    "Määritä Wi-Fi", // 49
    "Yhdistä: ", // 50
    "Wi-Fi:n määrittämiseksi", // 51
    "Yritetään yhdistää Wi-Fi", // 52
    "EPÄONNISTUI!", // 53
    "YHDISTETTY!", // 54
    "Stationlist-asiakas IP", // 55
    "Näytä SW aallonpituus", // 56
    "RDS-suodatin", // 57
    "Näytä PI-virheet", // 58
    "Käytä squelchiä", // 59
    "Äänimittari", // 60
    "AM Melupeitto", // 61
    "FM Melupeitto", // 62
    "Oletusarvot ladattu", // 63
    "Äänilähtö", // 64
    "Salli viritys ilman\nruutusaverin poistamista", // 65
    "Käännä näytön värit", // 66
    "Tietoja ohjelmistosta", // 67
    "Pääkoodi:", // 68
    "Avustajat:", // 69
    "Tuntematon", // 70
    "Näytönsäästäjäasetukset", // 71
    "Automaattinen virransammutus", // 72
    "Näyttö pois", // 73
    "Teema", // 74
    "Pinta", // 75
    "Havaitut sovellukset", // 76
    "Min.", // 77
    "SPI-nopeus", // 78
    "Skannausherkkyys", // 79
    "EI", // 80
    "Ohjelmistoversio", // 81
    "Taajuusfontti", // 82
    "Automaattinen", // 83
    "Ei AF saatavilla", // 84
    "Ei EON saatavilla", // 85
    "Ei RT+ saatavilla", // 86
    "FM oletusarvoiset askelvälit", // 78
    "Näytönsäästäjä", // 79
    "Sek", // 80
    "Kellotila", // 81
    "1% kirkkaus", // 82
    "25% kirkkaus", // 83
    "50% kirkkaus", // 84
    "AM RF-tason siirto", // 85
    "Signaalimittarin yksikkö", // 86
    "Käytä AF", // 87
    "Valitse FM-taajuusalue", // 97
    "Valitse AM-taajuusalue", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Pysäytä näytön herätys virityksessä", // 104
    "Mallin valitsin", // 105
    "Perus", // 106
    "Kannettava", // 107
    "Koskettimellinen kannettava", // 108
    "Nimi", // 109
    "Albumi", // 110
    "Kappale", // 111
    "Esittäjä", // 112
    "Sävellys", // 113
    "Liike", // 114
    "Kapellimestari", // 115
    "Säveltäjä", // 116
    "Bändi", // 117
    "Kommentti", // 118
    "Genre", // 119
    "Uutiset", // 120
    "Paikalliset uutiset", // 121
    "Osakemarkkinat", // 122
    "Urheilu", // 123
    "Arpajaiset", // 124
    "Horoskooppi", // 125
    "Päivittäinen viihde", // 126
    "Terveys", // 127
    "Tapahtuma", // 128
    "Kohde", // 129
    "Elokuvateatteri", // 130
    "TV", // 131
    "Päivämäärä/aika", // 132
    "Sää", // 133
    "Liikenne", // 134
    "Hälytys", // 135
    "Mainos", // 136
    "Verkkosivusto", // 137
    "Muu", // 138
    "Lyhyt PS", // 139
    "Pitkä PS", // 140
    "Nyt", // 141
    "Seuraava", // 142
    "Osa", // 143
    "Isäntä", // 144
    "Toimittaja", // 145
    "Taajuus", // 146
    "Etusivu", // 147
    "Alikanava", // 148
    "Puh. linja", // 149
    "Studion puhelin", // 150
    "Puhelin", // 151
    "SMS studio", // 152
    "SMS", // 153
    "Sähköposti puhelinlinja", // 154
    "Sähköposti studio", // 155
    "Sähköposti", // 156
    "Chat", // 157
    "Chattikeskus", // 158
    "Äänestyskysymys", // 159
    "Äänestyskeskus", // 160
    "Paikka", // 161
    "Tapaaminen", // 162
    "Tunniste", // 163
    "Osto", // 164
    "Hanki data", // 165
    "MW oletusarvoiset askelvälit", // 166
    "Mandariini", // 167
    "Meri", // 168
    "Indigo", // 169
    "Akkumääritykset", // 170
    "Näytä jännite", // 171
    "Näytä prosentti", // 172
    "RT pusku", // 173
    "Pään asetukset", // 174
    "Ääniasetukset", // 175
    "Näyttöasetukset", // 176
    "RDS-asetukset", // 177
    "FM-asetukset", // 178
    "AM-asetukset", // 179
    "Yhteydet ja kello", // 180
    "PAINA MODE PALATAKSESI", // 181
    "CoChannel-tunnistin", // 182
    "CoChannel-tunnistin\nherkkyys", // 183
    "CoChannel-tunnistin\nkynnys", // 184
    "CoChannel-tunnistin\nlaskuri", // 185
    "PÄÄ", // 186
    "ÄÄNI", // 187
    "NÄYTTÖ", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "YHTEYS", // 192
    "DX-tila", // 193
    "AUTOMAATTINEN MUISTI", // 194
    "AM AGC", // 195
    "FM vähennys", // 196
    "Pieni", // 197
    "Suuri", // 198
    "Kaikki", // 199
    "Nopea PS", // 200
    "Oletus", // 201
    "Hiljennä näyttö\nXDRGTK-yhteyden aikana", // 202
    "FMSI stereo\nparannus", // 203
    "Toiminto saatavilla vain\nTEF6687/6689:ssä!", // 204
    "Toiminto ei saatavilla\nvalitussa pinnassa!", // 205
    "Aloita skannaus\nmuistikanavilla", // 206
    "Pysäytä skannaus\nmuistikanavilla", // 207
    "Odota-aika", // 208
    "Aloita DX-skannaus", // 209
    "FM DX Asetukset", // 210
    "Keskeytä! Aloituskanava\non merkitty ohitettavaksi.", // 211
    "Automaattinen AF lajittelu", // 212
    "Muistikanavat", // 213
    "Sulje", // 214
    "Vain", // 215
    "Automaattinen skannauksen peruutus", // 216
    "Korjaa PI", // 217
    "Signaali", // 218
    "Hiljennä ääni\nskannauksen aikana", // 219
    "ILMA", // 220
    "5 sekunnin bandinäppäimen\npainallus", // 221
    "Virta pois", // 222
    "Näyttö pois", // 223
    "Poistettu", // 224
    "Ei mitään", // 225
    "Uutiset", // 226
    "Ajankohtaiset", // 227
    "Tietoa", // 228
    "Urheilu", // 229
    "Koulutus", // 230
    "Draama", // 231
    "Kulttuuri", // 232
    "Tiede", // 233
    "Vaihteleva", // 234
    "Pop-musiikki", // 235
    "Rock-musiikki", // 236
    "Helppokuuntelu", // 237
    "Kevyt klassinen", // 238
    "Vakava klassinen", // 239
    "Muu musiikki", // 240
    "Sää", // 241
    "Talous", // 242
    "Lasten ohjelmat", // 243
    "Sosiaaliasiat", // 244
    "Uskonto", // 245
    "Puhelinlinja", // 246
    "Matkustus", // 247
    "Vapaa-aika", // 248
    "Jazz-musiikki", // 249
    "Country-musiikki", // 250
    "Kansallismusiikki", // 251
    "Oldies-musiikki", // 252
    "Folk-musiikki", // 253
    "Dokumentti", // 254
    "Hälytyksen testi", // 255
    "Hälytys!!!", // 256
    "Alkuperäinen", // 257
    "Aina", // 258
    "Näytä kello", // 259
    "Näytä pitkä PS", // 260
    "Aloita taajuus", // 261
    "Pysäytä taajuus", // 262
    "Aloita muistikanava", // 263
    "Pysäytä muistikanava", // 264
    "Tallenna vain RDS:n\nkanssa", // 265
    "Aloita\nautomaattinen muistio", // 266
    "Automaattinen muisti", // 267
    "Stationlist ID", // 268
    "Löytyi", // 269
    "Keskeytä! Muistikanavat loppu", // 270
    "Keskeytä! Käyttäjä painoi nappia", // 271
    "Skannaus valmis ilman virheitä", // 272
    "Tyhjennä\nmuistikanavat", // 273
    "tyhjennetty", // 274
    "Estä kaksois-PI", // 275
    "Alue", // 276
    "Täysi", // 277
    "Odota-aika\nvain signaalille", // 278
    "Paina korostettua kulmaa", // 279
    "Kalibroi kosketusnäyttö", // 280
    "Pidä BW-nappia painettuna\npysäyttääksesi DX-skannerin", // 281
    "Valitse\nkaistanleveys", // 282
    "Oma lokikirja", // 283
    "Lataa lokikirja", // 284
    "Lokikirja on tyhjä", // 285
    "Mene alas", // 286
    "Merkintä lisätty\nlokikirjaan", // 287
    "Virhe lisättäessä\nmerkintää lokikirjaan", // 288
    "Tyhjennä lokikirja", // 289
    "Lokikirja\ntyhjennetty", // 290
    "Tyhjennys\nepäonnistui", // 291
    "Ei voida lisätä merkintää!\nLokikirja on täynnä!", // 292
    "Autolokikirja", // 293
    "Auto DST\nNTP-ajassa", // 294
    "Aseta\nNTP-aikasiirto", // 295
    "Lokikirja sisältää yli 130 merkintää, joita katselija ei pysty käsittelemään. Lataa CSV-tiedosto sen käsittelemiseksi.", // 296
    "Kanada" // 297
  },

  { "Dansk", // Danish
    "Rotationsretning ændret", // 1
    "Slip knappen", // 2
    "Skærm vendt", // 3
    "Kalibrer analog måler", // 4
    "USB-tilstand", // 5
    "Tuner: !Ingen!", // 6
    "Volumen", // 7
    "Konverter", // 8
    "Lavbåndskant", // 9
    "Højbåndskant", // 10
    "FM RF-niveau offset", // 11
    "Stereotærskel", // 12
    "Højfrekvensskæring\nhjørne", // 13
    "Højfrekvensskæring\ntærskel", // 14
    "Lavt niveau tærskel", // 15
    "Lysstyrke", // 16
    "Indstil volumen", // 17
    "Konverter offset", // 18
    "Lavbåndskant", // 19
    "Højbåndskant", // 20
    "Niveau offset", // 21
    "Stereoadskillelsestærskel", // 22
    "Højfrekvensskæring\nfrekvens", // 23
    "Højfrekvensskæring\ntærskel", // 24
    "Lavt niveau tærskel", // 25
    "Visningens lysstyrke", // 26
    "Slukket", // 27
    "Tændt", // 28
    "TRYK MODE FOR AT GEMME", // 29
    "TRYK PÅ BÅND FOR AT LUKKE", // 30
    "SCANNER...", // 31
    "Tuner ikke registreret", // 32
    "AM-antenneforstærkning", // 33
    "FM AGC", // 34
    "Vis RDS-fejl", // 35
    "Sprog", // 36
    "Vælg sprog", // 37
    "PI-låst CT-synkronisering", // 38
    "Fejl! Ingen RDS-signal", // 39
    "Blød dæmpning FM", // 40
    "Blød dæmpning AM", // 41
    "Beep ved båndkant", // 42
    "Region", // 43
    "Europa", // 44
    "USA", // 45
    "Vis understregning\ni RDS", // 46
    "PS kun", // 47
    "Wi-Fi aktiv", // 48
    "Konfigurer Wi-Fi", // 49
    "Forbind til: ", // 50
    "for at konfigurere Wi-Fi", // 51
    "Forsøger at forbinde Wi-Fi", // 52
    "FEJLET!", // 53
    "TILSLUTTET!", // 54
    "Stationliste\nklient IP", // 55
    "Vis SW-bølgelængde", // 56
    "RDS-filter", // 57
    "Vis PI-fejl", // 58
    "Brug squelch", // 59
    "Lydmåler", // 60
    "AM-støjfjerner", // 61
    "FM-støjfjerner", // 62
    "Standardindstillinger\nindlæst", // 63
    "Lydudgang", // 64
    "Tillad tuning uden\nat slå pauseskærm fra", // 65
    "Inverter visning", // 66
    "Om software", // 67
    "Hovedkode:", // 68
    "Bidragydere:", // 69
    "Ukendt", // 70
    "Pauseskærmindstillinger", // 71
    "Auto sluk", // 72
    "Skærm slukket", // 73
    "Tema", // 74
    "Hud", // 75
    "Registrerede\napplikationer", // 76
    "Min.", // 77
    "SPI hastighed", // 78
    "Scan-følsomhed", // 79
    "INGEN", // 80
    "Softwareversion", // 81
    "Frekvensskrifttype", // 82
    "Auto", // 83
    "Ingen AF tilgængelig", // 84
    "Ingen EON tilgængelig", // 85
    "Ingen RT+ tilgængelig", // 86
    "FM-standardtrin", // 78
    "Pauseskærm", // 79
    "Sek", // 80
    "Urtilstand", // 81
    "1% lysstyrke", // 82
    "25% lysstyrke", // 83
    "50% lysstyrke", // 84
    "AM RF-niveau offset", // 85
    "Signalmålerenhed", // 86
    "Brug AF", // 87
    "Vælg FM-bånd", // 97
    "Vælg AM-bånd", // 98
    "LW", // 99
    "MW", // 100
    "SW", // 101
    "FM", // 102
    "OIRT", // 103
    "Stop skærmvækning\nved tuning", // 104
    "Modelvælger", // 105
    "Basis", // 106
    "Bærbar", // 107
    "Bærbar touch", // 108
    "Titel", // 109
    "Album", // 110
    "Nummer", // 111
    "Kunstner", // 112
    "Komposition", // 113
    "Bevægelse", // 114
    "Dirigent", // 115
    "Komponist", // 116
    "Bånd", // 117
    "Kommentar", // 118
    "Genre", // 119
    "Nyheder", // 120
    "Lokale nyheder", // 121
    "Aktiemarked", // 122
    "Sport", // 123
    "Lotto", // 124
    "Horoskop", // 125
    "Daglig adspredelse", // 126
    "Sundhed", // 127
    "Begivenhed", // 128
    "Scene", // 129
    "Biograf", // 130
    "TV", // 131
    "Dato/tid", // 132
    "Vejr", // 133
    "Trafik", // 134
    "Alarm", // 135
    "Annonce", // 136
    "Hjemmeside", // 137
    "Andet", // 138
    "Kort PS", // 139
    "Længere PS", // 140
    "Nu", // 141
    "Næste", // 142
    "Del", // 143
    "Vært", // 144
    "Redaktør", // 145
    "Frekvens", // 146
    "Hjemmeside", // 147
    "Underkanal", // 148
    "Hotline", // 149
    "Studiotelefon", // 150
    "Telefon", // 151
    "SMS studie", // 152
    "SMS", // 153
    "E-mail hotline", // 154
    "E-mail studie", // 155
    "E-mail", // 156
    "Chat", // 157
    "Chatcenter", // 158
    "Afstemning spørgsmål", // 159
    "Afstemning center", // 160
    "Sted", // 161
    "Aftale", // 162
    "Identifikator", // 163
    "Køb", // 164
    "Hent data", // 165
    "MW-standardtrin", // 166
    "Tangerine", // 167
    "Hav", // 168
    "Indigo", // 169
    "Batteriindstillinger", // 170
    "Vis spænding", // 171
    "Vis procent", // 172
    "RT-buffering", // 173
    "Hovedindstillinger", // 174
    "Lydindstillinger", // 175
    "Skærmindstillinger", // 176
    "RDS-indstillinger", // 177
    "FM-indstillinger", // 178
    "AM-indstillinger", // 179
    "Tilslutning & ur", // 180
    "TRYK MODE FOR AT VENDE TILBAGE", // 181
    "Sammenkanal-detektor", // 182
    "Sammenkanal-detektor\nfølsomhed", // 183
    "Sammenkanal-detektor\ntærskel", // 184
    "Sammenkanal-detektor\ntæller", // 185
    "HOVED", // 186
    "LYD", // 187
    "SKÆRM", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "TILSLUT", // 192
    "DX-MODE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM-deemphasis", // 196
    "Lille", // 197
    "Stor", // 198
    "Alle", // 199
    "Hurtig PS", // 200
    "Standard", // 201
    "Sluk skærm ved\nXDRGTK-forbindelse", // 202
    "FMSI stereo\nforbedring", // 203
    "Funktion kun tilgængelig\npå TEF6687/6689!", // 204
    "Funktion ikke tilgængelig\npå valgt hud!", // 205
    "Begynd scanning\naf hukommelseskanaler", // 206
    "Stop scanning\naf hukommelseskanaler", // 207
    "Ventetid", // 208
    "Start DX-scanning", // 209
    "FM DX-indstillinger", // 210
    "Afbryd! Startkanal\ner sprunget over.", // 211
    "Auto-sortering AF", // 212
    "Hukommelseskanaler", // 213
    "Udeluk", // 214
    "Kun", // 215
    "Auto-annuller scan", // 216
    "Korrekt PI", // 217
    "Signal", // 218
    "Lyd dæmpet\nunder scanning", // 219
    "LUFT", // 220
    "5 sek. båndknap-tryk", // 221
    "Sluk", // 222
    "Skærm slukket", // 223
    "Deaktiveret", // 224
    "Ingen", // 225
    "Nyheder", // 226
    "Seneste nyt", // 227
    "Oplysninger", // 228
    "Sport", // 229
    "Uddannelse", // 230
    "Drama", // 231
    "Kultur", // 232
    "Videnskab", // 233
    "Variabel", // 234
    "Popmusik", // 235
    "Rockmusik", // 236
    "Nem lytning", // 237
    "Lettelse klassisk", // 238
    "Seriøs klassisk", // 239
    "Anden musik", // 240
    "Vejr", // 241
    "Finans", // 242
    "Børneprogrammer", // 243
    "Sociale anliggender", // 244
    "Religion", // 245
    "Ring ind", // 246
    "Rejse", // 247
    "Fritid", // 248
    "Jazzmusik", // 249
    "Countrymusik", // 250
    "National musik", // 251
    "Gamle hits", // 252
    "Folkemusik", // 253
    "Dokumentar", // 254
    "Alarmtest", // 255
    "Alarm!!!", // 256
    "Initial", // 257
    "Altid", // 258
    "Vis ur", // 259
    "Vis lang PS", // 260
    "Startfrekvens", // 261
    "Stopfrekvens", // 262
    "Start hukommelseskanal", // 263
    "Stop hukommelseskanal", // 264
    "Kun gem med RDS", // 265
    "Start auto-hukommelse", // 266
    "Auto-hukommelse", // 267
    "Stationsliste-ID", // 268
    "Fundet", // 269
    "Afbryd! Uden\nhukommelseskanaler", // 270
    "Afbryd! Brugeren\ntrykkede på knap", // 271
    "Scanning afsluttet uden fejl", // 272
    "Ryd\nhukommelseskanaler", // 273
    "ryddet", // 274
    "Forhindre\ndobbelt PI", // 275
    "Rækkevidde", // 276
    "Fuld", // 277
    "Ventetid på\nkun signal", // 278
    "Tryk på det markerede hjørne", // 279
    "For at kalibrere touchskærmen", // 280
    "Hold BW for at stoppe", // 281
    "Vælg båndbredde", // 282
    "Din logbog", // 283
    "Download logbog", // 284
    "Logbog er tom", // 285
    "Gå til bunden", // 286
    "Tilføjet til log", // 287
    "Opdatering\nmislykkedes!", // 288
    "Ryd logbog", // 289
    "Logbog ryddet", // 290
    "Rydning mislykkedes", // 291
    "Kan ikke tilføje!\nLogbogen er fuld!", // 292
    "Autologger", // 293
    "Auto DST på\nNTP-tid", // 294
    "Indstil NTP-tidsforskel", // 295
    "Logbogen indeholder over 130 poster, som fremviseren ikke kan behandle. Download CSV-filen for at behandle den.", // 296
    "Canada" // 297
  },

  { "Svenska", // Swedish
    "Rotationsriktning ändrad", // 1
    "Var god släpp knappen", // 2
    "Skärmen omvänd", // 3
    "Kalibrera analog mätare", // 4
    "USB-läge", // 5
    "Tuner: !Ingen!", // 6
    "Ljudstyrka", // 7
    "Konverter", // 8
    "Låg bandgräns", // 9
    "Hög bandgräns", // 10
    "FM RF-nivåoffset", // 11
    "Stereoseparations-\ntröskel", // 12
    "Lågpass frekvens", // 13
    "Lågpass tröskelnivå", // 14
    "Tröskel för låg nivå", // 15
    "Ljusstyrka", // 16
    "Sätt ljudstyrka", // 17
    "Konverter-offset", // 18
    "Låg bandgräns", // 19
    "Hög bandgräns", // 20
    "FM RF-nivåoffset", // 21
    "Stereotröskel", // 22
    "Lågpass frekvens", // 23
    "Lågpass tröskelnivå", // 24
    "Tröskel för låg nivå", // 25
    "Skärmljusstyrka", // 26
    "Av", // 27
    "På", // 28
    "TRYCK MODE FÖR ATT SPARA OCH AVSLUTA", // 29
    "TRYCK BAND FÖR ATT STÄNGA", // 30
    "SÖKER...", // 31
    "Tuner hittades inte", // 32
    "AM-antennförstärkning", // 33
    "FM AGC", // 34
    "Visa RDS-fel", // 35
    "Språk", // 36
    "Välj språk", // 37
    "PI-låst CT-synk", // 38
    "Fel! Ingen RDS-signal", // 39
    "Softmute FM", // 40
    "Softmute AM", // 41
    "Pip vid bandkant", // 42
    "Region", // 43
    "Europa", // 44
    "USA", // 45
    "Visa understreck i RDS", // 46
    "Endast PS", // 47
    "Wi-Fi aktiv", // 48
    "Konfigurera Wi-Fi", // 49
    "Anslut till: ", // 50
    "för att konfigurera Wi-Fi", // 51
    "Försöker att ansluta till Wi-Fi", // 52
    "MISSLYCKADES!", // 53
    "ANSLUTEN!", // 54
    "Stationlist klient-IP", // 55
    "Visa våglängd på KV", // 56
    "RDS-filter", // 57
    "Visa PI-fel", // 58
    "Använd brusspärr", // 59
    "Ljudmätare", // 60
    "AM noise blanker", // 61
    "FM noise blanker", // 62
    "Standarder laddade", // 63
    "Ljudutgång", // 64
    "Väck inte skärmen\nvid avstämning", // 65
    "Invertera skärmfärger", // 66
    "Om programvaran", // 67
    "Huvudkod:", // 68
    "Bidragsgivare:", // 69
    "Okänt", // 70
    "Skärmsläckaralternativ", // 71
    "Auto avstängning", // 72
    "Skärm av", // 73
    "Tema", // 74
    "Skinn", // 75
    "Upptäckta applikationer", // 76
    "Min.", // 77
    "SPI-hastighet", // 78
    "Skanningskänslighet", // 79
    "INGET", // 80
    "Programvaruversion", // 81
    "Frekvenstypsnitt", // 82
    "Auto", // 83
    "Ingen AF tillgänglig", // 84
    "Ingen EON tillgänglig", // 85
    "Ingen RT+ tillgänglig", // 86
    "FM standard\nsteglängd", // 78
    "Skärmsläckare", // 79
    "Sek.", // 80
    "Klockläge", // 81
    "1% ljusstyrka", // 82
    "25% ljusstyrka", // 83
    "50% ljusstyrka", // 84
    "AM RF-nivå offset", // 85
    "Enhet för\nsignalmätare", // 86
    "Använd AF", // 87
    "Välj FM-band", // 97
    "Välj AM-band", // 98
    "LV", // 99
    "MV", // 100
    "KV", // 101
    "FM", // 102
    "OIRT", // 103
    "Väck inte skärmen vid avstämning", // 104
    "Modellväljare", // 105
    "Bas", // 106
    "Portabel", // 107
    "Portabel touch", // 108
    "Titel", // 109
    "Album", // 110
    "Spår", // 111
    "Artist", // 112
    "Komposition", // 113
    "Sats", // 114
    "Dirigent", // 115
    "Kompositör", // 116
    "Grupp", // 117
    "Kommentar", // 118
    "Genre", // 119
    "Nyheter", // 120
    "Lokala nyheter", // 121
    "Aktiemarknad", // 122
    "Sport", // 123
    "Lotteri", // 124
    "Horoskop", // 125
    "Daglig förströelse", // 126
    "Hälsa", // 127
    "Evenemang", // 128
    "Scen", // 129
    "Bio", // 130
    "TV", // 131
    "Datum/tid", // 132
    "Väder", // 133
    "Trafik", // 134
    "Alarm", // 135
    "Reklam", // 136
    "Webbsida", // 137
    "Annat", // 138
    "Kort PS", // 139
    "Lång PS", // 140
    "Nu", // 141
    "Nästa", // 142
    "Del", // 143
    "Värd", // 144
    "Redaktör", // 145
    "Frekvens", // 146
    "Hemsida", // 147
    "Underkanal", // 148
    "Hotline", // 149
    "Studiotelefon", // 150
    "Telefon", // 151
    "SMS studio", // 152
    "SMS", // 153
    "E-post hotline", // 154
    "E-post studio", // 155
    "E-post", // 156
    "Chat", // 157
    "Chattcenter", // 158
    "Röstningsfråga", // 159
    "Röstningscenter", // 160
    "Plats", // 161
    "Möte", // 162
    "Identifikator", // 163
    "Köp", // 164
    "Få data", // 165
    "MV standard\nsteglängd", // 166
    "Tangerine", // 167
    "Ocean", // 168
    "Indigo", // 169
    "Batterialternativ", // 170
    "Visa spänning", // 171
    "Visa procent", // 172
    "RT buffring", // 173
    "Huvudinställningar", // 174
    "Ljudinställningar", // 175
    "Skärminställningar", // 176
    "RDS-inställningar", // 177
    "FM-inställningar", // 178
    "AM-inställningar", // 179
    "Anslutning & klocka", // 180
    "TRYCK MODE FöR ATT GÅ TILLBAKA", // 181
    "Samkanal-detektor", // 182
    "Samkanal-det.\nkänslighet", // 183
    "Samkanal-det.\ntröskel", // 184
    "Samkanal-det.\nräknare", // 185
    "HUVUD", // 186
    "LJUD", // 187
    "SKÄRM", // 188
    "RDS", // 189
    "FM", // 190
    "AM", // 191
    "ANSLUT", // 192
    "DX-LÄGE", // 193
    "AUTO MEM", // 194
    "AM AGC", // 195
    "FM de-emfas", // 196
    "Små", // 197
    "Stora", // 198
    "Allt", // 199
    "Snabb PS", // 200
    "Standard", // 201
    "Släck skärm vid\nXDRGTK-anslutning", // 202
    "FMSI stereo\nförbättring", // 203
    "Funktion endast tillgäng-\nlig på TEF6687/6689!", // 204
    "Funktion ej tillgänglig\npå valt skinn!", // 205
    "Första minnesplats", // 206
    "Sista minnesplats", // 207
    "Väntetid", // 208
    "Starta DX-skanning", // 209
    "FM DX-alternativ", // 210
    "Avbruten! Startkanalen är\nmärkt som överhoppad.", // 211
    "Auto-sortera AF", // 212
    "Minnesplatser", // 213
    "Exkludera", // 214
    "Endast", // 215
    "Auto-avbryt skanning", // 216
    "Korrekt PI", // 217
    "Signal", // 218
    "Tyst under\nskanning", // 219
    "AIR", // 220
    "5 sek. BAND-knapptryck", // 221
    "Stäng av", // 222
    "Stäng av skärm", // 223
    "Inaktiverat", // 224
    "Ingen", // 225
    "Nyheter", // 226
    "Aktualiteter", // 227
    "Information", // 228
    "Sport", // 229
    "Utbildning", // 230
    "Drama", // 231
    "Kultur", // 232
    "Vetenskap", // 233
    "Underhållning", // 234
    "Popmusik", // 235
    "Rockmusik", // 236
    "Lätt musik", // 237
    "Lätt klassisk", // 238
    "Klassisk musik", // 239
    "Annan musik", // 240
    "Väder", // 241
    "Ekonomi", // 242
    "Barnprogram", // 243
    "Sociala frågor", // 244
    "Religion", // 245
    "Ring in", // 246
    "Resor", // 247
    "Fritid", // 248
    "Jazz", // 249
    "Countrymusik", // 250
    "Nationell musik", // 251
    "Oldies musik", // 252
    "Folkmusik", // 253
    "Dokumentär", // 254
    "Alarmtest", // 255
    "Alarm!!!", // 256
    "Inledande", // 257
    "Alltid", // 258
    "Visa klocka", // 259
    "Visa lång PS", // 260
    "Startfrekvens", // 261
    "Stoppfrekvens", // 262
    "Första minnesplats", // 263
    "Sista minnesplats", // 264
    "Spara endast med RDS", // 265
    "Starta auto-minne", // 266
    "Auto-kanalminne", // 267
    "Stationlist-ID", // 268
    "Hittade", // 269
    "Avbruten! Inga minnesplatser", // 270
    "Avbruten! Användare tryckte knapp", // 271
    "Skanning slutförd utan fel", // 272
    "Töm minnesplatser", // 273
    "raderad", // 274
    "Förhindra dubbla PI", // 275
    "Range", // 276
    "Fullt", // 277
    "Väntetid bara\nvid signal", // 278
    "Tryck på markerat hörn", // 279
    "för att kalibrera touchskärm", // 280
    "Håll BW-knappen för att\nstoppa DX-skanning", // 281
    "Välj bandbredd", // 282
    "Din loggbok", // 283
    "Ladda ner loggbok", // 284
    "Loggbok är tom", // 285
    "Gå till botten", // 286
    "Post tillagd\ni loggbok", // 287
    "Fel vid sparande\ni loggbok", // 288
    "Töm loggbok", // 289
    "Loggbok tömd", // 290
    "Tömning misslyckades", // 291
    "Kan inte spara!\nLoggboken är full!", // 292
    "Autologgning", // 293
    "Auto NTP sommartid", // 294
    "Ange NTP tidszon", // 295
    "Loggboken har mer än 130 poster, vilket visaren inte kan hantera. Ladda ner CSV för behandling.", // 296
    "Kanada" // 297
  }
};