#pragma once

static const char* const myLanguage[22][300] PROGMEM = {
  { "English", // English
    "Rotary direction changed", // 1
    "Please release button", // 2
    "Screen flipped", // 3
    "Calibrate analog meter", // 4
    "USB mode", // 5
    "Encoder set to optical", // 6
    "Encoder set to standard", // 7
    "Tuner: !None!", // 8
    "Volume", // 9
    "Converter", // 10
    "Low band edge", // 11
    "High band edge", // 12
    "FM RF Level offset", // 13
    "Stereo threshold", // 14
    "High Cut corner", // 15
    "Highcut threshold", // 16
    "Low level threshold", // 17
    "Brightness", // 18
    "Set volume", // 19
    "Converter offset", // 20
    "Low band edge", // 21
    "High band edge", // 22
    "Level offset", // 23
    "Stereo separation threshold", // 24
    "High cut corner frequency", // 25
    "High cut threshold", // 26
    "Low level threshold", // 27
    "Display brightness", // 28
    "Off", // 29
    "On", // 30
    "PRESS MODE TO EXIT AND STORE", // 31
    "PRESS BAND TO CLOSE", // 32
    "SCANNING...", // 33
    "Tuner not detected", // 34
    "AM Antenna gain", // 35
    "FM AGC", // 36
    "Show RDS errors", // 37
    "Language", // 38
    "Choose language", // 39
    "PI locked CT sync", // 40
    "Error! No RDS signal", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Beep at band edge", // 44
    "Region", // 45
    "Europe", // 46
    "USA", // 47
    "Show underscore in RDS", // 48
    "PS only", // 49
    "Wi-Fi active", // 50
    "Configure Wi-Fi", // 51
    "Connect to: ", // 52
    "to configure Wi-Fi", // 53
    "Trying to connect Wi-Fi", // 54
    "FAILED!", // 55
    "CONNECTED!", // 56
    "Stationlist client IP", // 57
    "Show SW wavelength", // 58
    "RDS filter", // 59
    "Show PI errors", // 60
    "Use squelch", // 61
    "Audio level meter", // 62
    "AM Noise blanker", // 63
    "FM Noise blanker", // 64
    "Defaults loaded", // 65
    "Audio output", // 66
    "Allow tuning without\ndeactivating screensaver", // 67
    "Invert display colors", // 68
    "About software", // 69
    "Main code:", // 70
    "Contributors:", // 71
    "Unknown", // 72
    "Screensaver options", // 73
    "Auto Power off", // 74
    "Screen off", // 75
    "Theme", // 76
    "Skin", // 77
    "Detected applications", // 78
    "Min.", // 79
    "SPI speed", // 80
    "Scan sensitivity", // 81
    "NONE", // 82
    "Software version", // 83
    "Frequency font", // 84
    "Auto", // 85
    "No AF available", // 86
    "No EON available", // 87
    "No RT+ available", // 88
    "FM default stepsize", // 80
    "Screensaver", // 81
    "Sec", // 82
    "Clock mode", // 83
    "1% brightness", // 84
    "25% brightness", // 85
    "50% brightness", // 86
    "AM RF Level offset", // 87
    "Signalmeter unit", // 88
    "Use AF", // 89
    "Select FM Band", // 99
    "Select AM Band", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Stop screen wake on tune", // 106
    "Model selector", // 107
    "Base", // 108
    "Portable", // 109
    "Portable touch", // 110
    "Title", // 111
    "Album", // 112
    "Track", // 113
    "Artist", // 114
    "Composition", // 115
    "Movement", // 116
    "Conductor", // 117
    "Composer", // 118
    "Band", // 119
    "Comment", // 120
    "Genre", // 121
    "News", // 122
    "Local news", // 123
    "Stockmarket", // 124
    "Sport", // 125
    "Lottery", // 126
    "Horoscope", // 127
    "Daily Diversion", // 128
    "Health", // 129
    "Event", // 130
    "Scene", // 131
    "Cinema", // 132
    "TV", // 133
    "Date/time", // 134
    "Weather", // 135
    "Traffic", // 136
    "Alarm", // 137
    "Advertisement", // 138
    "Website", // 139
    "Other", // 140
    "Short PS", // 141
    "Long PS", // 142
    "Now", // 143
    "Next", // 144
    "Part", // 145
    "Host", // 146
    "Editor", // 147
    "Frequency", // 148
    "Homepage", // 149
    "Subchannel", // 150
    "Hotline", // 151
    "Studio phone", // 152
    "Phone", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email hotline", // 156
    "Email studio", // 157
    "Email", // 158
    "Chat", // 159
    "Chat centre", // 160
    "Vote question", // 161
    "Vote centre", // 162
    "Place", // 163
    "Appointment", // 164
    "Identifier", // 165
    "Purchase", // 166
    "Get data", // 167
    "MW default stepsize", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Battery options", // 172
    "Show voltage", // 173
    "Show percentage", // 174
    "RT buffering", // 175
    "Main settings", // 176
    "Audio settings", // 177
    "Display settings", // 178
    "RDS settings", // 179
    "FM settings", // 180
    "AM settings", // 181
    "Connectivity & clock", // 182
    "PRESS MODE TO RETURN", // 183
    "CoChannel Detector", // 184
    "CoChannel Detector\nsensitivity", // 185
    "CoChannel Detector\nthreshold", // 186
    "CoChannel Detector\ncounter", // 187
    "MAIN", // 188
    "AUDIO", // 189
    "DISPLAY", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CONNECT", // 194
    "DX MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM deemphasis", // 198
    "Small", // 199
    "Large", // 200
    "All", // 201
    "Fast PS", // 202
    "Default", // 203
    "Mute screen on\nXDRGTK connection", // 204
    "FMSI stereo\nimprovement", // 205
    "Function only available\non TEF6687/6689!", // 206
    "Function not available\non selected skin!", // 207
    "Begin scanning\nmemory channels", // 208
    "Stop scanning\nmemory channels", // 209
    "Wait time", // 210
    "Start DX scanning", // 211
    "FM DX Options", // 212
    "Abort! The start channel\nis marked as skipped.", // 213
    "Auto sort AF", // 214
    "Memory channels", // 215
    "Exclude", // 216
    "Only", // 217
    "Auto cancel scan", // 218
    "Correct PI", // 219
    "Signal", // 220
    "Mute audio\nwhile scanning", // 221
    "AIR", // 222
    "5 sec. bandbutton press", // 223
    "Power off", // 224
    "Screen off", // 225
    "Disabled", // 226
    "None", // 227
    "News", // 228
    "Current Affairs", // 229
    "Information", // 230
    "Sport", // 231
    "Education", // 232
    "Drama", // 233
    "Culture", // 234
    "Science", // 235
    "Varied", // 236
    "Pop Music", // 237
    "Rock Music", // 238
    "Easy Listening", // 239
    "Light Classical", // 240
    "Serious Classical", // 241
    "Other Music", // 242
    "Weather", // 243
    "Finance", // 244
    "Children's Progs", // 245
    "Social Affairs", // 246
    "Religion", // 247
    "Phone-In", // 248
    "Travel", // 249
    "Leisure", // 250
    "Jazz Music", // 251
    "Country Music", // 252
    "National Music", // 253
    "Oldies Music", // 254
    "Folk Music", // 255
    "Documentary", // 256
    "Alarm Test", // 257
    "Alarm!!!", // 258
    "Initial", // 259
    "Always", // 260
    "Show clock", // 261
    "Show long PS", // 262
    "Start frequency", // 263
    "Stop frequency", // 264
    "Start memory channel", // 265
    "Stop memory channel", // 266
    "Only store with RDS", // 267
    "Start auto memory", // 268
    "Auto memory", // 269
    "Stationlist ID", // 270
    "Found", // 271
    "Abort! Out of memorychannels", // 272
    "Abort! User pressed button", // 273
    "Scan finished without errors", // 274
    "Clear memory channels", // 275
    "cleared", // 276
    "Prevent double PI", // 277
    "Range", // 278
    "Full", // 279
    "Wait time on\nsignal only", // 280
    "Press highlighted corner", // 281
    "To calibrate touchscreen", // 282
    "Hold BW button to\nstop DX scanner", // 283
    "Select Bandwidth", // 284
    "Your logbook", // 285
    "Download logbook", // 286
    "Logbook is empty", // 287
    "Go to bottom", // 288
    "Entry added\nto the logbook", // 289
    "Error adding\nentry to the logbook", // 290
    "Clear logbook", // 291
    "Logbook cleared", // 292
    "Clear failed", // 293
    "Can't add entry!\nLogbook is full!", // 294
    "Autologger", // 295
    "Auto DST on NTP time", // 296
    "Set NTP time offset", // 297
    "The logbook contains over 130 entries, which the viewer cannot process. Please download the CSV file to process it.", // 298
    "Canada" // 299
  },

  { "Nederlands", // Dutch
    "Rotary richting aangepast", // 1
    "Laat aub de knop los", // 2
    "Scherm gedraaid", // 3
    "Kalibratie analoge meter", // 4
    "USB mode", // 5
    "encoder ingesteld als optisch", // 6
    "encoder ingesteld als standaard", // 7
    "Tuner: !Geen!", // 8
    "Volume", // 9
    "Converter", // 10
    "Lage bandgrens", // 11
    "Hoge bandgrens", // 12
    "FM RF niveau afwijking", // 13
    "Stereo grens", // 14
    "Hoge tonen afval", // 15
    "Hoog afval grens", // 16
    "Laag signaal grens", // 17
    "Helderheid", // 18
    "Audio volume", // 19
    "Converter offset", // 20
    "Band ondergrens", // 21
    "Band bovengrens", // 22
    "Signaalafwijking", // 23
    "Stereo signaalgrens", // 24
    "Hoge tonen afval frequentie", // 25
    "Hoge tonen afval signaalniveau", // 26
    "Laag signaalgrens", // 27
    "Stel helderheid in", // 28
    "Uit", // 29
    "Aan", // 30
    "DRUK OP MODE OM AF TE SLUITEN", // 31
    "DRUK OP BAND OM AF TE SLUITEN", // 32
    "ZOEKEN...", // 33
    "Tuner niet herkend!", // 34
    "AM antenne versterking", // 35
    "FM AGC", // 36
    "Toon RDS fouten", // 37
    "Taal", // 38
    "Kies taal", // 39
    "PI gebonden CT sync", // 40
    "Fout! Geen RDS!", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Piep op bandeinde", // 44
    "Regio", // 45
    "Europa", // 46
    "USA", // 47
    "Toon underscore in RDS", // 48
    "Alleen PS", // 49
    "Wi-Fi aktief", // 50
    "Configureer Wi-Fi", // 51
    "Verbind met: ", // 52
    "om Wi-Fi in te stellen", // 53
    "Verbinden met Wi-Fi...", // 54
    "MISLUKT!", // 55
    "VERBONDEN!", // 56
    "Stationlist client IP", // 57
    "Toon SW golflengte", // 58
    "RDS filter", // 59
    "Toon PI fouten", // 60
    "Gebruik squelch", // 61
    "Audio niveau meter", // 62
    "AM ruis filter", // 63
    "FM ruis filter", // 64
    "Opnieuw geconfigureerd", // 65
    "Audio uitgang", // 66
    "Afstemmen tijdens\nschermbeveiliging", // 67
    "Inverteer scherm", // 68
    "Over deze software", // 69
    "Basis code:", // 70
    "Bijgedragen:", // 71
    "Onbekend", // 72
    "Screensaver opties", // 73
    "Auto uitschakelen", // 74
    "Scherm uit", // 75
    "Thema", // 76
    "Uiterlijk", // 77
    "Gedetecteerde applicaties", // 78
    "Min.", // 79
    "Dynamische\nSPI snelheid", // 80
    "Zoek gevoeligheid", // 81
    "Geen", // 82
    "Software versie", // 83
    "Frequentie font", // 84
    "Auto", // 85
    "Geen AF data", // 86
    "Geen EON data", // 87
    "Geen RT+ data", // 88
    "FM standaard\nstapgrootte", // 80
    "Schermbeveiliging", // 81
    "sec.", // 82
    "Klok methode", // 83
    "1% helderheid", // 84
    "25% helderheid", // 85
    "50% helderheid", // 86
    "AM RF Level afwijking", // 87
    "Signaalmeter eenheid", // 88
    "Gebruik AF", // 89
    "Selecteer\nFM banden", // 99
    "Selecteer\nAM banden", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Stop scherm ontwaken bij afstemmen", // 106
    "Model keuze", // 107
    "Basis", // 108
    "Portable", // 109
    "Portable touch", // 110
    "Titel", // 111
    "Album", // 112
    "Nummer", // 113
    "Artiest", // 114
    "Compositie", // 115
    "Deel", // 116
    "Dirigent", // 117
    "Componist", // 118
    "Band", // 119
    "Opmerking", // 120
    "Genre", // 121
    "Nieuws", // 122
    "Lokaal nieuws", // 123
    "Financiële beurs", // 124
    "Sport", // 125
    "Loterij", // 126
    "Horoscoop", // 127
    "Dagelijkse grap", // 128
    "Gezondheid", // 129
    "Evenement", // 130
    "Scene", // 131
    "Bioscoop", // 132
    "TV", // 133
    "Datum/tijd", // 134
    "Weer", // 135
    "Verkeer", // 136
    "Alarm", // 137
    "Advertentie", // 138
    "Website", // 139
    "Overige", // 140
    "Korte naam", // 141
    "Lange naam", // 142
    "Nu", // 143
    "Straks", // 144
    "Deel", // 145
    "Host", // 146
    "Regiseur", // 147
    "Frequentie", // 148
    "Homepage", // 149
    "Subkanaal", // 150
    "Hotline", // 151
    "Studio telefoon", // 152
    "Telefoon", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email hotline", // 156
    "Email studio", // 157
    "Email", // 158
    "Chat", // 159
    "Chat centrum", // 160
    "Stemming", // 161
    "Stemcentrum", // 162
    "Plaats", // 163
    "Afspraak", // 164
    "Identificatie", // 165
    "Aankoop", // 166
    "Verkrijg data", // 167
    "MW standaard\nstapgrootte", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Batterij weergave", // 172
    "Toon spanning", // 173
    "Toon percentage", // 174
    "RT buffer", // 175
    "Algemeen", // 176
    "Audio", // 177
    "Weergave", // 178
    "RDS instellingen", // 179
    "FM instellingen", // 180
    "AM instellingen", // 181
    "Connectiviteit", // 182
    "MODE OM TERUG TE GAAN", // 183
    "CoChannel Detectie", // 184
    "CoChannel Detectie\ngevoeligheid", // 185
    "CoChannel Detectie\ndrempel", // 186
    "CoChannel Detectie\nteller", // 187
    "ALGEMEEN", // 188
    "GELUID", // 189
    "SCHERM", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "VERBIND", // 194
    "DX MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM deemphasis", // 198
    "Laag", // 199
    "Middel", // 200
    "Alles", // 201
    "Snelle PS", // 202
    "Standaard", // 203
    "Scherm uit bij\nXDRGTK verbinding", // 204
    "FMSI stereo\nverbetering", // 205
    "Alleen beschikbaar op\nTEF6687/6689!", // 206
    "Niet beschikbaar op\ngekozen uiterlijk!", // 207
    "Begin zoeken\ngeheugenkanaal", // 208
    "Eind zoeken\ngeheugenkanaal", // 209
    "Wachttijd", // 210
    "Start DX zoeken", // 211
    "FM DX Opties", // 212
    "Fout! Startkanaal is\ngemarkeerd als skip", // 213
    "Auto AF sorteren", // 214
    "Geheugenkanalen", // 215
    "Uitsluiten", // 216
    "Enkel", // 217
    "Auto afbreken", // 218
    "Correcte PI", // 219
    "Signaal", // 220
    "Geen geluid\ntijdens scannen", // 221
    "AIR", // 222
    "5 sec.\nbandbutton ingedrukt", // 223
    "Uitschakelen", // 224
    "Scherm uit", // 225
    "Geen functie", // 226
    "Niet gedefinieerd", // 227
    "Nieuws", // 228
    "Actualiteit", // 229
    "Informatie", // 230
    "Sport", // 231
    "Educatie", // 232
    "Drama", // 233
    "Cultuur", // 234
    "Wetenschap", // 235
    "Varia", // 236
    "Popmuziek", // 237
    "Rockmuziek", // 238
    "Melodie", // 239
    "Licht klassiek", // 240
    "Klassieke muziek", // 241
    "Muziek", // 242
    "Het weer", // 243
    "Economie", // 244
    "Voor kinderen", // 245
    "Maatschappelijk", // 246
    "Religie", // 247
    "Doe mee !", // 248
    "Reizen", // 249
    "Vrije tijd", // 250
    "Jazz", // 251
    "Countrymuziek", // 252
    "Nationale muziek", // 253
    "Gouwe ouwe", // 254
    "Volksmuziek", // 255
    "Documentaires", // 256
    "Alarmtest", // 257
    "Alarm ! ALARM !", // 258
    "Initieel", // 259
    "Altijd", // 260
    "Toon klok", // 261
    "Toon long PS", // 262
    "Start frequentie", // 263
    "Stop frequentie", // 264
    "Start geheugen\nkanaal", // 265
    "Stop geheugen\nkanaal", // 266
    "Alleen met\nRDS opslaan", // 267
    "Start auto\ngeheugen kanaal", // 268
    "Auto geheugen kanaal", // 269
    "Stationlist ID", // 270
    "Gevonden", // 271
    "Afgebroken! Geheugen vol", // 272
    "Afgebroken! Knop ingedrukt", // 273
    "Zoeken zonder fouten voltooid", // 274
    "Wis geheugenkanalen", // 275
    "gewist", // 276
    "Voorkom dubbele PI", // 277
    "Bereik", // 278
    "Volledig", // 279
    "Wachttijd alleen\nbij signaal", // 280
    "Druk op aangegeven hoeken", // 281
    "om scherm te calibreren", // 282
    "Houdt BW knop lang\ningedrukt om te stoppen", // 283
    "Selecteer bandbreedte", // 284
    "Jouw logboek", // 285
    "Download logboek", // 286
    "Logboek is leeg", // 287
    "Ga naar einde", // 288
    "Toegevoegd aan\nhet logboek", // 289
    "Toevoegen aan\nlogboek mislukt!", // 290
    "Logboek legen", // 291
    "Logboek geleegd", // 292
    "Legen mislukt", // 293
    "Toevoegen mislukt!\nLogboek is vol!", // 294
    "Automatisch loggen", // 295
    "Auto zomertijd\nop NTP tijd", // 296
    "NTP tijdverschil\ninstellen", // 297
    "Het logboek bevat meer dan 130 items, die de viewer niet kan verwerken. Download alstublieft het CSV-bestand om het te bekijken.", // 298
    "Canada" // 299
  },

  { "Polski", // Polish
    "Kierunek obrotu zmieniony", // 1
    "Zwolnij przycisk", // 2
    "Obrócono ekran", // 3
    "Kalibracja analogowego miernika", // 4
    "Tryb USB", // 5
    "wybrano enkoder optyczny", // 6
    "wybrano standardowy enkoder", // 7
    "Tuner: !Brak!", // 8
    "Głośność", // 9
    "Konwerter", // 10
    "Dolna granica pasma", // 11
    "Górna granica pasma", // 12
    "FM Odchylenie poziomu RF", // 13
    "Próg Stereo", // 14
    "Krawędź dolnoprzep", // 15
    "Próg dolnoprzepust.", // 16
    "Próg niskiego sygnału", // 17
    "Kontrast", // 18
    "Ustaw głośność", // 19
    "Ustaw odchylenie konwertera", // 20
    "Ustaw dolną granicę pasma", // 21
    "Ustaw górną granicę pasma", // 22
    "Ustaw odchylenie sygnału", // 23
    "Ustaw próg separacji Stereo", // 24
    "Częst. gran. filtra dolnoprzep.", // 25
    "Ustaw próg dolnoprzep.", // 26
    "Ustaw dolną granicę sygnału", // 27
    "Ustaw jasność wyświetlacza", // 28
    "Wył.", // 29
    "Włącz.", // 30
    "WCIŚNIJ MODE ABY ZAMKNĄĆ I ZAPISAĆ", // 31
    "NACIŚNIJ BAND ABY ZAMKNĄĆ", // 32
    "SKANOWANIE...", // 33
    "Nie wykryto tunera", // 34
    "Czułość anteny AM", // 35
    "FM AGC", // 36
    "Pokaż błędy RDS", // 37
    "Język", // 38
    "Wybierz swój język", // 39
    "Synch. CT przy zablokowanym PI", // 40
    "Błąd! Brak sygnału RDS", // 41
    "Łagodne wyciszanie FM", // 42
    "Łagodne wyciszanie AM", // 43
    "Ton na krawędzi pasma", // 44
    "Region", // 45
    "Europa", // 46
    "USA", // 47
    "Pokaż podkreślenia w RDS", // 48
    "Tylko PS", // 49
    "Wi-Fi aktywne", // 50
    "Konfiguracja Wi-Fi", // 51
    "Połącz z: ", // 52
    "aby skonfigurować Wi-Fi", // 53
    "Próba połączenia z Wi-Fi", // 54
    "BŁĄD.. WiFi wyłączone", // 55
    "POŁĄCZONO!", // 56
    "IP klienta Stationlist", // 57
    "Pokaż długości fal SW", // 58
    "Filtr RDS", // 59
    "Pokaż błędy PI", // 60
    "Użyj blokady szumów", // 61
    "Wskaźnik modulacji", // 62
    "Reduktor szumów AM", // 63
    "Reduktor szumów FM", // 64
    "Załadowano domyślne", // 65
    "Wyjście dźwięku", // 66
    "Strój gdy ekran wył.", // 67
    "Odwróć kolory wyświetlacza", // 68
    "O oprogramowaniu", // 69
    "Główny kod:", // 70
    "Współtwórcy:", // 71
    "Nieznany", // 72
    "Opcje wygaszacza", // 73
    "Wyłącznik czasowy", // 74
    "Wyłącz ekran", // 75
    "Motyw", // 76
    "Skórka", // 77
    "Wykryte aplikacje", // 78
    "Min.", // 79
    "Prędkość SPI", // 80
    "Czułość skanowania", // 81
    "BRAK", // 82
    "Wersja oprogramowania", // 83
    "Czcionka częstotliwości", // 84
    "Auto", // 85
    "AF niedostępny", // 86
    "EON niedostępny", // 87
    "RT+ niedostępny", // 88
    "Minimalny krok FM", // 80
    "Wygaszacz ekranu", // 81
    "Sek", // 82
    "Tryb zegara", // 83
    "1% jasności", // 84
    "25% jasności", // 85
    "50% jasności", // 86
    "Odchylenie poziomu RF AM", // 87
    "Jednostka sygnału", // 88
    "Używaj AF", // 89
    "Wybierz pasmo FM", // 99
    "Wybierz pasmo AM", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Stroj.gdy ekran wył.", // 106
    "Wybór modelu", // 107
    "Podstawowy", // 108
    "Przenośny", // 109
    "Przenoś. dotyk.", // 110
    "Tytuł", // 111
    "Album", // 112
    "Ścieżka", // 113
    "Wykonawca", // 114
    "Kompozycja", // 115
    "Ruch", // 116
    "Dyrygent", // 117
    "Kompozytor", // 118
    "Grupa", // 119
    "Komentarz", // 120
    "Gatunek", // 121
    "Wiadomości", // 122
    "Lokalne wiadomości", // 123
    "Giełda", // 124
    "Sport", // 125
    "Loteria", // 126
    "Horoskop", // 127
    "Codzienny zwrot", // 128
    "Zdrowie", // 129
    "Wydarzenie", // 130
    "Scena", // 131
    "Kino", // 132
    "TV", // 133
    "Data/Godzina", // 134
    "Pogoda", // 135
    "Wiadom. drogowe", // 136
    "Alarm", // 137
    "Reklama", // 138
    "Strona internet.", // 139
    "Inne", // 140
    "Krótki PS", // 141
    "Długi PS", // 142
    "Teraz", // 143
    "Następnie", // 144
    "Część", // 145
    "Prowadzący", // 146
    "Wydawca", // 147
    "Częstotliwość", // 148
    "Strona domowa", // 149
    "Podkanał", // 150
    "Infolinia", // 151
    "Telefon do studia", // 152
    "Telefon", // 153
    "SMS do studia", // 154
    "SMS", // 155
    "Email do gorącej linii", // 156
    "Email do studia", // 157
    "Email", // 158
    "Czat", // 159
    "Centrum czatu", // 160
    "Pytanie do głosowania", // 161
    "Centrum głosowania", // 162
    "Miejsce", // 163
    "Spotkanie", // 164
    "Identyfikator", // 165
    "Zakup", // 166
    "Pobierz dane", // 167
    "Domyślny krok MW", // 168
    "Mandarynka", // 169
    "Ocean", // 170
    "Indygo", // 171
    "Opcje baterii", // 172
    "Pokaż napięcie", // 173
    "Pokaż procent naład.", // 174
    "Buforowanie RT", // 175
    "Główne ustawienia", // 176
    "Ustawienia dźwięku", // 177
    "Ustawienia wyświetlacza", // 178
    "Ustawienia RDS", // 179
    "Ustawienia FM", // 180
    "Ustawienia AM", // 181
    "Łączność i zegar", // 182
    "NACIŚNIJ MODE ABY WRÓCIĆ", // 183
    "Detek. międzykanałowy AM", // 184
    "Czułość detek. międzykanał.", // 185
    "Próg detek. międzykanałowego AM", // 186
    "Licznik detek. międzykanał.", // 187
    "GŁÓWNE", // 188
    "DŹWIĘK", // 189
    "WYŚWIETLACZ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "ŁĄCZNOŚĆ", // 194
    "TRYB DX", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "Deemfaza FM", // 198
    "Małe", // 199
    "Duże", // 200
    "Wszystkie", // 201
    "Szybki PS", // 202
    "Domyslny", // 203
    "Wylacz ekran gdy\npołączony z XDR-GTK", // 204
    "Ulepszenie \nstereo FMSI", // 205
    "Funkcja dostępna tylko\nna TEF6687/6689!", // 206
    "Funkcja niedostępna\nz wybranym motywem!", // 207
    "Rozpocznij skanowanie\nkanałów pamięci", // 208
    "Zatrzymaj skanowanie\nkanałów pamięci", // 209
    "Czas oczekiwania", // 210
    "Rozpocznij skanowanie DX", // 211
    "Opcje FM DX", // 212
    "Błąd! Początkowy kanał\njest pominięty.", // 213
    "Automat. sortowanie AF", // 214
    "Kanały pamięci", // 215
    "Wyklucz", // 216
    "Uwzględnij", // 217
    "Automatyczne\nanulowanie skanowania", // 218
    "Poprawne PI", // 219
    "Sygnał", // 220
    "Wycisz dźwięk\npodczas skanowania", // 221
    "AIR", // 222
    "5 sek. naciśnięcie przyc. BAND", // 223
    "Wyłącz", // 224
    "Wyłącz ekran", // 225
    "Wyłączone", // 226
    "Niezdefiniowany", // 227
    "Wiadomości", // 228
    "Sprawy bieżące", // 229
    "Informacje", // 230
    "Sport", // 231
    "Edukacja", // 232
    "Dramat", // 233
    "Kultura", // 234
    "Nauka", // 235
    "Różne", // 236
    "Muzyka Popowa", // 237
    "Muzyka Rockowa", // 238
    "Muzyka lekka", // 239
    "Lekka muz. klas.", // 240
    "Muz. klas. pow.", // 241
    "Inna muzyka", // 242
    "Pogoda", // 243
    "Finanse", // 244
    "Prog. dla dzieci", // 245
    "Sprawy społeczne", // 246
    "Religia", // 247
    "Telefon.Udział.", // 248
    "Podróże", // 249
    "Czas wolny", // 250
    "Muzyka Jazz", // 251
    "Muzyka Country", // 252
    "Muzyka narodowa", // 253
    "Stare przeboje", // 254
    "Muzyka ludowa", // 255
    "Dokument", // 256
    "Alarm Test", // 257
    "Alarm", // 258
    "Na start", // 259
    "Zawsze", // 260
    "Pokaż zegar", // 261
    "Pokaż długi PS", // 262
    "Częstotliwość początkowa", // 263
    "Częstotliwość końcowa", // 264
    "Początkowy kanał pamięci", // 265
    "Końcowy kanał pamięci", // 266
    "Zapisuj tylko z RDS", // 267
    "Rozpocznij auto zapamiętywanie", // 268
    "Auto zapamiętywanie", // 269
    "Identyfikator Stationlist", // 270
    "Znaleziono", // 271
    "Przerywam! Brak wolnych kanałów", // 272
    "Przerywam! Naciśnięto przycisk", // 273
    "Skanowanie zakończone pomyślnie", // 274
    "Wyczyść kanały pamięci", // 275
    "wyczyszczono", // 276
    "Unikaj podwójnego PI", // 277
    "Zakres", // 278
    "Pełne", // 279
    "Czekaj tylko gdy\nwykryto stację", // 280
    "Naciśnij zaznaczony róg", // 281
    "Aby skalibrować ekran", // 282
    "Aby zatrzymać,\nprzytrzymaj przycisk BW", // 283
    "Wybierz szer. pasma", // 284
    "Twój wykaz", // 285
    "Pobierz wykaz", // 286
    "Wykaz jest pusty", // 287
    "Przejdź na dół", // 288
    "Dodano do wykazu", // 289
    "Bład aktualizacji!", // 290
    "Wyczyść wykaz", // 291
    "Wykaz wyczyszczony", // 292
    "Błąd wyczyszczenia", // 293
    "Wykaz pełny!", // 294
    "Autologowanie stacji", // 295
    "Auto czas letni wg NTP", // 296
    "Ustaw odchyl. czasu NTP", // 297
    "Wykaz zawiera ponad 130 wpisów, których przeglądarka nie może pokazać. Pobierz plik CSV, aby je przetworzyć.", // 298
    "Kanada" // 299
  },

  { "Hrvatski", // Croatian
    "Smjer enkodera promij.", // 1
    "Otpustite gumb", // 2
    "Zaslon je okrenut", // 3
    "Kalibracija analog. mjerača", // 4
    "Način rada USB-a", // 5
    "Optički enkoder postav.", // 6
    "Standardni enkoder postav.", // 7
    "Prijemnik: !Nije pronađen!", // 8
    "Glasnoća", // 9
    "Konverter", // 10
    "Donja granica opsega", // 11
    "Gornja granica opsega", // 12
    "FM pomak RF razine", // 13
    "Razina stereo signala", // 14
    "Granična frekvencija", // 15
    "Razina granične frekv.", // 16
    "Donja granica za signal", // 17
    "Svjetlina", // 18
    "Postavi glasnoću", // 19
    "Pomak konvertera", // 20
    "Donja granica frekv. opsega", // 21
    "Gornja granica frekv. opsega", // 22
    "Pomak signala", // 23
    "Razina stereo signala", // 24
    "Granična frekvencija", // 25
    "Razina granične frekv.", // 26
    "Donja granica za signal", // 27
    "Svjetlina zaslona", // 28
    "Isključeno", // 29
    "Uključeno", // 30
    "PRITISNITE MODE ZA IZLAZ I SPREMANJE", // 31
    "PRITISNITE BAND ZA IZLAZ", // 32
    "SKENIRANJE...", // 33
    "Prijemnik nije pronađen", // 34
    "AM Antenna gain", // 35
    "FM AGC", // 36
    "Prikaži RDS pogreške", // 37
    "Jezik", // 38
    "Odaberite jezik", // 39
    "PI locked CT sync", // 40
    "Error! No RDS signal", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Beep na rubu frekv. opsega", // 44
    "Regija", // 45
    "Europa", // 46
    "USA", // 47
    "Prikaži donju crtu u RDS-u", // 48
    "PS only", // 49
    "Wi-Fi", // 50
    "Wi-Fi postavke", // 51
    "Spojite se na: ", // 52
    "kako biste konfigurirali Wi-Fi", // 53
    "Spajanje na Wi-Fi mrežu..", // 54
    "NEUSPJEŠNO.. Wi-Fi onemogućen", // 55
    "SPOJENO!", // 56
    "IP klijenta StationList", // 57
    "Prikaži valnu duljinu KV-a", // 58
    "RDS filter", // 59
    "Prikaži PI greške", // 60
    "Prikaži squelch", // 61
    "Mjerač modulacije", // 62
    "AM prigušivač šuma", // 63
    "FM prigušivač šuma", // 64
    "Zadane postavke učitane", // 65
    "Audio izlaz", // 66
    "Omogući podešavanje\n tijekom čuvara zaslona", // 67
    "Invertiraj boju zaslona", // 68
    "O softveru", // 69
    "Glavni izvorni kod:", // 70
    "Suradnici:", // 71
    "Nepoznato", // 72
    "Opcije čuvara zaslona", // 73
    "Time-out timer", // 74
    "Zaslon isključiti", // 75
    "Tema dizajna", // 76
    "Skin", // 77
    "Otkrivene aplikacije", // 78
    "Min.", // 79
    "SPI brzina", // 80
    "Osjetljivost skeniranja", // 81
    "NEMA", // 82
    "Verzija softvera", // 83
    "Font frekvencije", // 84
    "Auto", // 85
    "AF nedostupan", // 86
    "EON nedostupan", // 87
    "RT+ nedostupan", // 88
    "FM min korak", // 80
    "Čuvar zaslona", // 81
    "Sek", // 82
    "Način rada sata", // 83
    "1% svjetlina", // 84
    "25% svjetlina", // 85
    "50% svjetlina", // 86
    "AM pomak RF razine", // 87
    "Jedinica mjerača signala", // 88
    "Koristi AF", // 89
    "Odaberi FM opseg", // 99
    "Odaberi AM opseg", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Okretanje dok je zaslon isključen", // 106
    "Selektor modela", // 107
    "Base", // 108
    "Portable", // 109
    "Portable touch", // 110
    "Naslov", // 111
    "Album", // 112
    "Pjesma", // 113
    "Izvođač", // 114
    "Kompozicija", // 115
    "Pokret", // 116
    "Dirigent", // 117
    "Skladatelj", // 118
    "Bend", // 119
    "Komentar", // 120
    "Žanr", // 121
    "Vijesti", // 122
    "Lokalne vijesti", // 123
    "Burza", // 124
    "Sport", // 125
    "Lutrija", // 126
    "Horoskop", // 127
    "Savjet dana", // 128
    "Zdravlje", // 129
    "Događanja", // 130
    "Scena", // 131
    "Kino", // 132
    "TV", // 133
    "Datum/vrijeme", // 134
    "Vremenska prognoza", // 135
    "Promet", // 136
    "Alarm", // 137
    "Reklama", // 138
    "Web stranica", // 139
    "Ostalo", // 140
    "Kratki PS", // 141
    "Dugi PS", // 142
    "Sada", // 143
    "Sljedi", // 144
    "Dio", // 145
    "Voditelj", // 146
    "Urednik", // 147
    "Frekvencija", // 148
    "Web stranica", // 149
    "Potkanal", // 150
    "Hotline", // 151
    "Telefon studija", // 152
    "Telefon", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email hotline", // 156
    "Email studija", // 157
    "Email", // 158
    "Chat", // 159
    "Chat centar", // 160
    "Pitanje za glasanje", // 161
    "Centar za glasanje", // 162
    "Mjesto", // 163
    "Ugovoreni sastanak", // 164
    "Identifikator", // 165
    "Kupnja", // 166
    "Primi podatke", // 167
    "MW zadana veličina koraka", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Postavke za bateriju", // 172
    "Prikaži napon", // 173
    "Prikaži postotak", // 174
    "RT pufer", // 175
    "Glavno", // 176
    "Zvuk", // 177
    "Zaslon", // 178
    "RDS postavke", // 179
    "FM postavke", // 180
    "AM postavke", // 181
    "Povezivost", // 182
    "PRITISNITE MODE ZA POVRATAK", // 183
    "AM CoChannelDet", // 184
    "CoChannelDet osjetljivost", // 185
    "AM CoChannelDet razina", // 186
    "CoChannelDet brojač", // 187
    "GLAVNO", // 188
    "ZVUK", // 189
    "ZASLON", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "SPOJI", // 194
    "DX NAČIN RADA", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM deemphasis", // 198
    "Malo", // 199
    "Veliko", // 200
    "Sve", // 201
    "Brzi PS", // 202
    "Zadano", // 203
    "Isključi zaslon\ntijekom veze XDRGTK-om", // 204
    "Poboljšanje\nFMSI stereo-a", // 205
    "Funkcija dostupna samo\nna TEF6687/6689!", // 206
    "Funkcija nije dostupna\na odabranoj temi dizajna!", // 207
    "Pokreni skeniranje\nmemorijskih kanala", // 208
    "Zaustavi skeniranje\nmemorijskih kanala", // 209
    "Vrijeme čekanja", // 210
    "Pokreni DX skeniranje", // 211
    "FM DX opcije", // 212
    "Prekid! Početni kanal je\noznačen kao preskočen.", // 213
    "Autom. sortiranje AF-a", // 214
    "Memorijski kanali", // 215
    "Isključeno", // 216
    "Samo", // 217
    "Autom. otkazivanje skeniranja", // 218
    "Točan PI", // 219
    "Signal", // 220
    "Isključi zvuk\ntijekom skeniranja", // 221
    "AIR", // 222
    "Pritisnite tipku band 5 sekundi", // 223
    "Isključivanje", // 224
    "Isključi zaslon", // 225
    "Onemogućeno", // 226
    "Neodređeno", // 227
    "Vijesti", // 228
    "Aktualno", // 229
    "Obavijesti", // 230
    "Sport", // 231
    "Obrazovanje", // 232
    "Drama", // 233
    "Kultura", // 234
    "Znanost", // 235
    "Razno", // 236
    "Pop glazba", // 237
    "Rock glazba", // 238
    "Zabavna glazba", // 239
    "Lagana klasična", // 240
    "Ozbiljna glazba", // 241
    "Ostala glazba", // 242
    "Vrijeme", // 243
    "Financije", // 244
    "Dječji program", // 245
    "Društvena događ", // 246
    "Religija", // 247
    "Kontakt program", // 248
    "Putovanja", // 249
    "Slobodno vrijeme", // 250
    "Jazz glazba", // 251
    "Country glazba", // 252
    "Domaća glazba", // 253
    "Stari hitovi", // 254
    "Narodna glazba", // 255
    "Dokumentarno", // 256
    "Testni alarm!", // 257
    "Alarm !", // 258
    "Početno", // 259
    "Uvijek", // 260
    "Prikaži sat", // 261
    "Prikaži dugi PS", // 262
    "Početna frekvencija", // 263
    "Završna frekvencija", // 264
    "Početni memorijski kanal", // 265
    "Završni memorijski kanal", // 266
    "Samo spremi s RDS-om", // 267
    "Pokreni autom. memoriju", // 268
    "Autom. memorija", // 269
    "ID StationList", // 270
    "Pronađeno", // 271
    "Prekid! Nema memorijskih kanala", // 272
    "Prekid! Korisnik je pritisnuo gumb", // 273
    "Skeniranje završeno bez grešaka", // 274
    "Očisti memorijske kanale", // 275
    "Očišćeno", // 276
    "Spriječi dupli PI", // 277
    "Raspon", // 278
    "Puno", // 279
    "Vrijeme čekanja na\nsignal samo", // 280
    "Pritisnite istaknuti kut", // 281
    "Za kalibraciju zaslona osjetljivog na dodir", // 282
    "Držite BW gumb za\nzaustavljanje DX skenera", // 283
    "Odabir širine pojasa", // 284
    "Vaš dnevnik", // 285
    "Preuzmi dnevnik", // 286
    "Dnevnik je prazan", // 287
    "Idi na dno", // 288
    "Unos dodan u dnevnik", // 289
    "Greška pri dodavanju unosa u dnevnik", // 290
    "Očisti dnevnik", // 291
    "Dnevnik očišćen", // 292
    "Čišćenje nije uspjelo", // 293
    "Ne mogu dodati unos!\nDnevnik je pun!", // 294
    "Autologger", // 295
    "Auto DST na NTP vremenu", // 296
    "Postavi NTP pomak vremena", // 297
    "Dnevnik sadrži preko 130 unosa, koje preglednik ne može obraditi. Preuzmite CSV datoteku za obradu.", // 298
    "Canada" // 299
  },

  { "Ελληνικά", // Greek
    "Η διεύθυνση του ρότορα\nάλλαξε", // 1
    "Ελευθερώστε το πλήκτρο", // 2
    "Η οθόνη\nαναποδογύρισε", // 3
    "Βαθμονόμηση\nαναλογικού μετρητή", // 4
    "USB λειτουργία", // 5
    "Ο κωδικοποιητής\nρυθμίστηκε σε οπτικός", // 6
    "Ο κωδικοποιητής\nρυθμίστηκε σε στάνταρ", // 7
    "Tuner: !Κανένα!", // 8
    "Ένταση ήχου", // 9
    "Μετατροπέας", // 10
    "Κάτω όριο μπάντας", // 11
    "Άνω όριο μπάντας", // 12
    "FM Ολίσθηση\nεπιπέδου RF", // 13
    "Κατώφλι στέρεο", // 14
    "Όριο υψηλής αποκοπής", // 15
    "Κατώφλι υψηλής\nαποκοπής", // 16
    "Κατώφλι χαμηλού\nεπιπέδου", // 17
    "Αντίθεση", // 18
    "Ορισμός έντασης ήχου", // 19
    "Ολίσθηση (offset) κωδικοποιητή", // 20
    "Κάτω όριο μπάντας", // 21
    "Άνω όριο μπάντας", // 22
    "Επίπεδο ολίσθησης", // 23
    "Κατώφλι διαχωρισμού\nστέρεο", // 24
    "Συχνότητα υψηλής αποκοπής", // 25
    "Κατώφλι υψηλής\nαποκοπής", // 26
    "Κατώφλι χαμηλού\nεπιπέδου", // 27
    "Φωτεινότητα οθόνης", // 28
    "Ανενεργό", // 29
    "Ενεργό", // 30
    "ΠΙΕΣΤΕ MODE ΓΙΑ ΕΞΟΔΟ & ΑΠΟΘΗΚΕΥΣΗ", // 31
    "ΠΙΕΣΤΕ BAND ΓΙΑ ΚΛΕΙΣΙΜΟ", // 32
    "ΑΝΙΧΝΕΥΣΗ...", // 33
    "Το tuner δεν εντοπίστηκε", // 34
    "Κέρδος κεραίας AM", // 35
    "FM AGC", // 36
    "Περιθώριο σφαλμάτων\nRDS", // 37
    "Γλώσσα", // 38
    "Επιλογή γλώσσας", // 39
    "Συγχρονισμός CT σε\nκλείδωμα PI", // 40
    "Δεν υπάρχει σήμα RDS!", // 41
    "Ήπια σίγαση FM", // 42
    "Ήπια σίγαση AM", // 43
    "Ηχητικό σήμα στο όριο\nμπάντας", // 44
    "Περιοχή", // 45
    "Ευρώπη", // 46
    "Η.Π.Α.", // 47
    "Εμφάνιση κάτω παύλας\nστο RDS", // 48
    "PS μόνο", // 49
    "Wi-Fi ενεργό", // 50
    "Ρύθμιση Wi-Fi", // 51
    "Συνδεθείτε στο: ", // 52
    "για ρύθμιση του WiFi", // 53
    "Προσπάθεια σύνδεσης\nστο WiFi", // 54
    "ΑΠΟΤΥΧΙΑ..\nWiFi ανενεργό", // 55
    "ΣΕ ΣΥΝΔΕΣΗ!", // 56
    "IP πελάτη λίστας σταθμών", // 57
    "Εμφάνιση μήκους\nκύματος SW", // 58
    "Φίλτρο RDS", // 59
    "Εμφάνιση σφαλμάτων PI", // 60
    "Χρήση squelch", // 61
    "Μετρητής διαμόρφωσης", // 62
    "Απαλοιφή θορύβου AM", // 63
    "Απαλοιφή θορύβου FM", // 64
    "Οι προεπιλογές φορτώθηκαν", // 65
    "Έξοδος ήχου", // 66
    "Συντονισμός σε\nπροστασία οθόνης", // 67
    "Αντιστροφή\nχρωμάτων οθόνης", // 68
    "Πληροφορίες λογισμικού", // 69
    "Κύριος κώδικας:", // 70
    "Συνεργάτες:", // 71
    "Άγνωστο", // 72
    "Επιλογές προστασίας\nοθόνης", // 73
    "Αυτόματο σβήσιμο", // 74
    "Οθόνη σβηστή", // 75
    "Θέμα", // 76
    "Γραφικό\nπεριβάλλον", // 77
    "Εντοπισμένες εφαρμογές", // 78
    "Λεπτά", // 79
    "Δυναμική ταχύτητα\nSPI", // 80
    "Ευαισθησία\nανίχνευσης", // 81
    "KAMIA", // 82
    "Έκδοση λογισμικού", // 83
    "Γραμματοσειρά\nσυχνότητας", // 84
    "Αυτόματο", // 85
    "Μη διαθέσιμο AF", // 86
    "Μη διαθέσιμο EON", // 87
    "Μη διαθέσιμο RT+", // 88
    "Μέγεθος βήματος FM", // 80
    "Προστασία οθόνης", // 81
    "Δευτ.", // 82
    "Λειτουργία ρολογιού", // 83
    "1% φωτεινή", // 84
    "25% φωτεινή", // 85
    "50% φωτεινή", // 86
    "AM Ολίσθηση\nεπιπέδου RF", // 87
    "Μονάδα μέτρησης\nσήματος", // 88
    "Χρήση AF", // 89
    "Επιλογή μπάντας FM", // 99
    "Επιλογή μπάντας AM", // 100
    "LW", // 101
    "AM", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Μη αφύπνιση\nκατά τον συντονισμό", // 106
    "Επιλογή μοντέλου", // 107
    "Βάσης", // 108
    "Φορητό", // 109
    "Φορητό αφής", // 110
    "Τίτλος", // 111
    "Άλμπουμ", // 112
    "Κομμάτι", // 113
    "Καλλιτέχνης", // 114
    "Σύνθεση", // 115
    "Κίνηση", // 116
    "Μαέστρος", // 117
    "Συνθέτης", // 118
    "Μπάντα", // 119
    "Σχόλιο", // 120
    "Είδος", // 121
    "Ειδήσεις", // 122
    "Τοπικές ειδήσεις", // 123
    "Χρηματιστήριο", // 124
    "Αθλητικά", // 125
    "Λοταρία", // 126
    "Ωροσκόπιο", // 127
    "Καθημερινές ασχολίες", // 128
    "Υγεία", // 129
    "Γεγονός", // 130
    "Σκηνή", // 131
    "Σινεμά", // 132
    "TV", // 133
    "Ημερομηνία/ώρα", // 134
    "Καιρός", // 135
    "Κυκλοφορία", // 136
    "Συναγερμός", // 137
    "Διαφήμιση", // 138
    "Ιστότοπος", // 139
    "Άλλο", // 140
    "Σύντομο PS", // 141
    "Μακρύ PS", // 142
    "Τώρα", // 143
    "Στη συνέχεια", // 144
    "Μέρος", // 145
    "Παρουσιαστής", // 146
    "Συντάκτης", // 147
    "Συχνότητα", // 148
    "Ιστοσελίδα", // 149
    "Υποφέρουσα", // 150
    "Τηλ. επικοινωνίας", // 151
    "Τηλ. στούντιο", // 152
    "Τηλέφωνο", // 153
    "SMS στούντιο", // 154
    "SMS", // 155
    "Email επικοινωνίας", // 156
    "Email στούντιο", // 157
    "Email", // 158
    "Chat", // 159
    "Κέντρο chat", // 160
    "Ερώτηση ψηφοφορίας", // 161
    "Κέντρο ψηφοφορίας", // 162
    "Τοποθεσία", // 163
    "Ραντεβού", // 164
    "Αναγνωριστικό", // 165
    "Αγορά", // 166
    "Λήψη δεδομένων", // 167
    "Προεπιλεγμένο μέγεθος\nβήματος ΑΜ", // 168
    "Μανταρινί", // 169
    "Ωκεανός", // 170
    "Ινδικό", // 171
    "Επιλογές μπαταρίας", // 172
    "Εμφάνιση τάσης", // 173
    "Εμφάνιση ποσοστού", // 174
    "RT προφόρτωση", // 175
    "Βασικές ρυθμίσεις", // 176
    "Ρυθμίσεις ήχου", // 177
    "Ρυθμίσεις εμφάνισης", // 178
    "Ρυθμίσεις RDS", // 179
    "Ρυθμίσεις FM", // 180
    "Ρυθμίσεις AM", // 181
    "Συνδεσιμότητα & ρολόι", // 182
    "ΠΙΕΣΤΕ MODE ΓΙΑ ΕΠΙΣΤΡΟΦΗ", // 183
    "Εντοπισμός\nομοκαναλικού AM", // 184
    "Ευαισθησία\nομοκαναλικού AM", // 185
    "Κατώφλι\nομοκαναλικού AM", // 186
    "Μετρητής\nομοκαναλικού AM", // 187
    "ΒΑΣΙΚΕΣ", // 188
    "ΗΧΟΣ", // 189
    "ΕΜΦΑΝΙΣΗ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "ΣΥΝΔΕΣΗ", // 194
    "DX MODE", // 195
    "ΑΥΤ. ΜΝΗΜΗ", // 196
    "AM AGC", // 197
    "FM αποέμφαση", // 198
    "Μικρό", // 199
    "Μεγάλο", // 200
    "Όλα", // 201
    "Γρήγορο PS", // 202
    "Προεπιλογή", // 203
    "Μαύρη οθόνη σε\nσύνδεση XDRGTK", // 204
    "FMSI βελτίωση\nήχου στέρεο", // 205
    "Λειτουργία διαθέσιμη\nμόνο στα TEF6687/6689!", // 206
    "Λειτουργία μη διαθέσιμη\nσε αυτή την εμφάνιση!", // 207
    "Εκκίνηση ανίχνευσης\nκαναλιών μνήμης", // 208
    "Διακοπή ανίχνευσης\nκαναλιών μνήμης", // 209
    "Χρόνος αναμονής", // 210
    "Εκκίνηση DX ανίχνευσης", // 211
    "Επιλογές FM DX", // 212
    "Ματαίωση! Λείπει\nτο κανάλι εκκίνησης.", // 213
    "Αυτόματη ταξινόμηση\nAF", // 214
    "Κανάλια μνήμης", // 215
    "Εξαίρεση", // 216
    "Μόνο", // 217
    "Αυτόματη ακύρωση\nανίχνευσης", // 218
    "Σωστό PI", // 219
    "Σήμα", // 220
    "Σίγαση ήχου\nκατά την ανίχνευση", // 221
    "AIR", // 222
    "Πιέση πλήκτρου\nμπάντας 5 δευτ.", // 223
    "Σβήσιμο", // 224
    "Σβήσιμο οθόνης", // 225
    "Ανενεργό", // 226
    "Κανένα", // 227
    "Ειδήσεις", // 228
    "Επικαιρότητα", // 229
    "Πληροφόρηση", // 230
    "Σπορ", // 231
    "Εκπαίδευση", // 232
    "Δράμα", // 233
    "Πολιτισμός", // 234
    "Επιστήμη", // 235
    "Ποικίλο", // 236
    "Ποπ μουσική", // 237
    "Ροκ μουσική", // 238
    "Εύκολη ακρόαση", // 239
    "Ελαφρά κλασική", // 240
    "Σοβαρή κλασική", // 241
    "Άλλη μουσική", // 242
    "Καιρός", // 243
    "Οικονομικά", // 244
    "Παιδικό πρόγραμμα", // 245
    "Κοινωνικά", // 246
    "Θρησκεία", // 247
    "Τηλεφωνικά", // 248
    "Ταξίδια", // 249
    "Ελεύθερος χρόνος", // 250
    "Τζαζ μουσική", // 251
    "Κάντρι μουσική", // 252
    "Εθνική μουσική", // 253
    "Παλαιές επιτυχίες", // 254
    "Παραδοσιακή μουσική", // 255
    "Ντοκιμαντέρ", // 256
    "Δοκιμή συναγερμού", // 257
    "Συναγερμός!!!", // 258
    "Αρχικό", // 259
    "Πάντα", //261
    "Εμφάνιση ρολογιού", // 261
    "Εμφάνιση μεγάλου PS", // 262
    "Συχνότητα εκκίνησης", // 263
    "Τελική συχνότητα", // 264
    "Αρχικό κανάλι μνήμης", // 265
    "Τελικό κανάλι μνήμης", // 266
    "Αποθήκευση μόνο\nμε RDS", // 267
    "Εκκίνηση\nαυτόματης μνήμης", // 268
    "Αυτόματη μνήμη", // 269
    "ID λίστας σταθμών", // 270
    "Βρέθηκαν", // 271
    "Ματαίωση! Τα κανάλια\nμνήμης εξαντλήθηκαν", // 272
    "Ματαίωση! Πιέση\nπλήκτρου χρήστη", // 273
    "Η ανίχνευση ολοκληρώθηκε\nχωρίς σφάλματα", // 274
    "Εκκαθάριση\nκαναλιών μνήμης", // 275
    "έγινε εκκαθάριση", // 276
    "Αποφυγή διπλού PI", // 277
    "Εύρος", // 278
    "Πλήρης", // 279
    "Αναμονή μόνο σε\nυπάρξη σήματος", // 280
    "Πιέστε την επισημασμένη\nγωνία για βαθμονόμηση", // 281
    "της οθόνης αφής", // 282
    "Κρατήστε το BW για\nδιακοπή της DX σάρωσης", // 283
    "Επιλογή εύρους μπάντας", // 284
    "Το βιβλίο καταγραφών σας", // 285
    "Λήψη βιβλίου καταγραφών", // 286
    "Το βιβλίο καταγραφών είναι άδειο", // 287
    "Μετάβαση στον πάτο", // 288
    "Προστέθηκε\nστις καταγραφές", // 289
    "Σφάλμα προσθήκης\nστις καταγραφές", // 290
    "Εκκαθάριση βιβλίου\nκαταγραφών", // 291
    "Εκκαθαρίστηκε", // 292
    "Αποτυχία εκκαθάρισης", // 293
    "Αδυναμία προσθήκης!\nΒιβλίο γεμάτο!", // 294
    "Αυτόματη καταγραφή", // 295
    "Αυτόματο DST\nσε ώρα NTP", // 296
    "Ολίσθηση ώρας NTP", // 297
    "Το βιβλίο περιέχει πάνω από 130 καταγραφές, και είναι αδύνατη η επεξεργασία τους από το χρήστη. Κάντε λήψη του αρχείου CSV για να το επεξεργαστείτε.", // 298
    "Καναδάς" // 299
  },

  { "Română", // Romanian
    "Direcția rotirii schimbată", // 1
    "Eliberați butonul", // 2
    "Ecran întors", // 3
    "Calibrați instrumentul analog", // 4
    "Mod USB", // 5
    "Encoder setat optic", // 6
    "Encoder setat standard", // 7
    "Tuner: !Niciunul!", // 8
    "Volum", // 9
    "Convertor", // 10
    "Marginea de jos a benzii", // 11
    "Marginea de sus a benzii", // 12
    "Decalaj de nivel RF FM", // 13
    "Pragul stereo", // 14
    "Limită tăiere înalte", // 15
    "Prag tăiere înalte", // 16
    "Prag nivel joase", // 17
    "Luminozitate", // 18
    "Setare volum", // 19
    "Setare decalaj convertor", // 20
    "Setarea limitei de jos a benzii", // 21
    "Setarea limitei de sus a benzii", // 22
    "Setare nivel decalaj", // 23
    "Setarea pragului sep. Stereo", // 24
    "Setare tăiere frecv. înalte", // 25
    "Setare prag tăiere înalte", // 26
    "Setarea pragului la joase", // 27
    "Luminozitate ecran", // 28
    "Oprit", // 29
    "Pornit", // 30
    "APĂSAȚI MODE PENTRU IEȘIRE ȘI SALVARE", // 31
    "APĂSAȚI BAND PENTRU ÎNCHIDERE", // 32
    "SCANEZ...", // 33
    "Tunerul nu este prezent", // 34
    "Reglare nivel antenă AM", // 35
    "FM AGC", // 36
    "Arată erorile RDS", // 37
    "Limbă", // 38
    "Alegeți Limba", // 39
    "PI blocat pe sincronizare CT", // 40
    "Eroare! Fără semnal RDS", // 41
    "Lipsă zgomot FM", // 42
    "Lipsă zgomot AM", // 43
    "Beep la capătul benzii", // 44
    "Regiune", // 45
    "Europa", // 46
    "USA", // 47
    "Afișează subliniere la RDS", // 48
    "Doar PS", // 49
    "Wi-Fi activ", // 50
    "Configurare Wi-Fi", // 51
    "Conectare la: ", // 52
    "Ptr configurare Wi-Fi", // 53
    "Încerc conectarea la Wi-Fi", // 54
    "Fără succes!", // 55
    "Conectat!", // 56
    "Listă adrese IP", // 57
    "Arată lungimea de undă SW", // 58
    "Filtru RDS", // 59
    "Arată erorile PI", // 60
    "Folosește squelch", // 61
    "Contor modulație", // 62
    "Eliminator de zgomot AM", // 63
    "Eliminator de zgomot FM", // 64
    "Revenire la setările de fabrică", // 65
    "Ieșire Audio", // 66
    "Permite reglarea în timp ce ecranul este oprit", // 67
    "Ecran inversat", // 68
    "Despre software", // 69
    "Cod principal:", // 70
    "Contribuitori:", // 71
    "Necunoscut", // 72
    "Opțiuni economizor ecran", // 73
    "Pauză temporizator", // 74
    "Ecran oprit", // 75
    "Temă", // 76
    "Mod vizualizare", // 77
    "Aplicații detectate", // 78
    "Min.", // 79
    "Viteză SPI", // 80
    "Sensibilitatea scanării", // 81
    "NICI UNUL", // 82
    "Versiune Software", // 83
    "Font Frecvență", // 84
    "Auto", // 85
    "AF nu este disponibil", // 86
    "EON nu este disponibil", // 87
    "RT+ nu este disponibil", // 88
    "Mărime pași FM", // 80
    "Screensaver", // 81
    "Sec", // 82
    "Mod ceas", // 83
    "Luminozitate 1%", // 84
    "Luminozitate 25%", // 85
    "Luminozitate 50%", // 86
    "Decalaj de nivel AM RF", // 87
    "Unitați S-metru", // 88
    "Folosește AF", // 89
    "Selectare bandă FM", // 99
    "Selectare bandă AM", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Rotire când ecranul este oprit", // 106
    "Selectare Model", // 107
    "Bază", // 108
    "Portabil", // 109
    "Portabil touch", // 110
    "Titlu", // 111
    "Album", // 112
    "Bandă", // 113
    "Artist", // 114
    "Compoziție", // 115
    "Mișcare", // 116
    "Dirijor", // 117
    "Compozitor", // 118
    "Formație", // 119
    "Comentarii", // 120
    "Gen", // 121
    "Știri", // 122
    "Știri Locale", // 123
    "Bursa de Valori", // 124
    "Sport", // 125
    "Loto", // 126
    "Horoscop", // 127
    "Info zilnice", // 128
    "Sănătate", // 129
    "Evenimente", // 130
    "Teatru", // 131
    "Cinema", // 132
    "TV", // 133
    "Data/ora", // 134
    "Meteo", // 135
    "Trafic", // 136
    "Alarmă", // 137
    "Reclame", // 138
    "Website", // 139
    "Altele", // 140
    "PS scurt", // 141
    "PS lung", // 142
    "Acum", // 143
    "Urmează", // 144
    "Parte", // 145
    "Gazdă", // 146
    "Editor", // 147
    "Frecvența", // 148
    "Pagina start", // 149
    "Sub canal", // 150
    "Linia fierbinte", // 151
    "Telefon Studio", // 152
    "Telefon", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email Linie fierbinte", // 156
    "Email studio", // 157
    "Email", // 158
    "Discuții", // 159
    "Centru discuții", // 160
    "Întrebare la vot", // 161
    "Centru votare", // 162
    "Loc", // 163
    "Întâlnire", // 164
    "Identificare", // 165
    "Cumpără", // 166
    "Preiau date", // 167
    "Pași MW impliciți", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Opțiuni Baterie", // 172
    "Arată tensiunea", // 173
    "Arată procente", // 174
    "Porțiune RT", // 175
    "Setări de bază", // 176
    "Setări Audio", // 177
    "Setări ecran", // 178
    "Setări RDS", // 179
    "Setări FM", // 180
    "Setări AM", // 181
    "Conectivitate și ceas", // 182
    "APĂSAȚI MODE PENTRU REVENIRE", // 183
    "Detector CoChannel", // 184
    "Detecție sensibilitate CoChannel", // 185
    "Detecție prag CoChannel", // 186
    "Detector contor CoChannel", // 187
    "PRINCIPAL", // 188
    "AUDIO", // 189
    "ECRAN", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CONECTARE", // 194
    "MOD DX", // 195
    "AUTO MEM", // 196
    "AGC AM", // 197
    "FM deemphasis", // 198
    "Mic", // 199
    "Mare", // 200
    "Tot", // 201
    "PS rapid", // 202
    "Mod implicit", // 203
    "Ecran oprit, conectat la XDRGTK", // 204
    "FMSI stereo îmbunătățit", // 205
    "Funcție activă doar la\nTEF6687/6689!", // 206
    "Funcție indisponibilă\nla acest skin!", // 207
    "Scanare începută\nmemorare posturi", // 208
    "Scanare terminată\nmemorare posturi", // 209
    "Așteaptă", // 210
    "Începe scanarea DX", // 211
    "FM DX Opțiuni", // 212
    "Terminat! Programul de pornire\neste ignorat.", // 213
    "Sortare automată AF", // 214
    "Memorare programe", // 215
    "Exclude", // 216
    "Doar", // 217
    "Auto oprire scanare", // 218
    "PI corect", // 219
    "Semnal", // 220
    "Audio oprit\ncând scanează", // 221
    "AIR", // 222
    "Apasă 5sec. butonul bandă", // 223
    "Oprit", // 224
    "Ecran oprit", // 225
    "Dezactivat", // 226
    "Nedefinit", // 227
    "Știri", // 228
    "Actualități", // 229
    "Informații", // 230
    "Sport", // 231
    "Educație", // 232
    "Teatru", // 233
    "Cultură", // 234
    "Știință", // 235
    "Varietăți", // 236
    "Muzică pop", // 237
    "Muzică rock", // 238
    "Muzică ușoară", // 239
    "Clasic Lejer", // 240
    "Muzică Clasică", // 241
    "Altă Muzică", // 242
    "Meteo", // 243
    "Finanțe", // 244
    "Copii", // 245
    "Social", // 246
    "Religie", // 247
    "Phone-In", // 248
    "Călătorii", // 249
    "Timp Liber", // 250
    "Muzică de Jazz", // 251
    "Muzică Country", // 252
    "Muzică Națională", // 253
    "Muzica Retro", // 254
    "Muzică Populară", // 255
    "Documentar", // 256
    "Test Alarmă", // 257
    "Alarmă!!!", // 258
    "Inițial", // 259
    "Mereu", // 260
    "Arată ceasul", // 261
    "Arată PS lung", // 262
    "Frecvență de început", // 263
    "Frecvență de oprire", // 264
    "Canal de memorie de început", // 265
    "Canal de memorie de oprire", // 266
    "Stochează doar cu RDS", // 267
    "Începe memoria automată", // 268
    "Memorie automată", // 269
    "Listă ID posturi", // 270
    "Găsit", // 271
    "Anulat! Memoria canalelor este plină", // 272
    "Anulat! Utilizatorul a apăsat un buton", // 273
    "Scanarea s-a terminat fără erori", // 274
    "Șterge canalele de memorie", // 275
    "Șters", // 276
    "Previne dublarea PI", // 277
    "Interval", // 278
    "Complet", // 279
    "Timp de așteptare doar\npe semnal", // 280
    "Apasă colțul evidențiat", // 281
    "Pentru calibrarea ecranului tactil", // 282
    "Ține apăsat BW pentru\noprirea scannerului DX", // 283
    "Selectează lățimea de bandă", // 284
    "Jurnalul tău", // 285
    "Descarcă jurnalul", // 286
    "Jurnalul este gol", // 287
    "Du-te la final", // 288
    "Adăugat în jurnal", // 289
    "Eroare la adăugarea\nîn jurnal", // 290
    "Șterge jurnalul", // 291
    "Jurnal șters", // 292
    "Ștergerea a eșuat", // 293
    "Nu se poate adăuga!\nJurnalul este plin!", // 294
    "Autologger", // 295
    "Auto DST pe ora NTP", // 296
    "Setează offsetul pentru ora NTP", // 297
    "Jurnalul conține peste 130 de intrări, pe care vizualizatorul nu le poate procesa. Te rog descarcă fișierul CSV pentru a le procesa.", // 298
    "Canada" // 299
  },

  { "Deutsch", // German
    "Richtung des Drehimpuls-\ngebers wird umgekehrt", // 1
    "Bitte Taste loslassen", // 2
    "Bildschirm wird\num 180° gedreht", // 3
    "Analogmeter\nwird kalibrieret", // 4
    "USB-Modus", // 5
    "Drehimpulsgeber wechselt\nin optischen Modus", // 6
    "Drehimpulsgeber wechselt\nin Standard-Modus", // 7
    "Tuner: !Keiner!", // 8
    "Lautstärke", // 9
    "Konverter", // 10
    "Unteres Bandende", // 11
    "Oberes Bandende", // 12
    "UKW RF-Pegeloffset", // 13
    "Stereo-Trennschwelle", // 14
    "Tiefpass-Eckfrequenz", // 15
    "Tiefpass-Schwelle", // 16
    "Low-Level-Schwelle", // 17
    "Bildschirmhelligkeit", // 18
    "Lautstärke einstellen", // 19
    "Konverteroffset", // 20
    "Unteres Bandende", // 21
    "Oberes Bandende", // 22
    "Pegeloffset", // 23
    "Stereo-Trennschwelle", // 24
    "Tiefpass-Eckfrequenz", // 25
    "Tiefpass-Schwelle", // 26
    "Low-Level-Schwelle", // 27
    "Bildschirmhelligkeit", // 28
    "Aus", // 29
    "Ein", // 30
    "DRÜCKE MODE ZUM VERLASSEN & SPEICHERN", // 31
    "DRÜCKE BAND ZUM SCHLIESSEN", // 32
    "SCANVORGANG\nLÄUFT...", // 33
    "Tuner nicht erkannt", // 34
    "AM Antennengewinn", // 35
    "UKW AGC", // 36
    "Zeige RDS-Fehler", // 37
    "Sprache", // 38
    "Sprache wählen", // 39
    "PI-Vorgabe für\nCT-Synchronisation", // 40
    "Fehler! Kein RDS-Signal", // 41
    "Sanfte\nStummschaltung UKW", // 42
    "Sanfte\nStummschaltung AM", // 43
    "Piepston am Bandende", // 44
    "Region", // 45
    "Europa", // 46
    "USA", // 47
    "Zeige Unterstrich im RDS", // 48
    "Nur PS", // 49
    "WLAN aktiv", // 50
    "WLAN konfigurieren", // 51
    "Verbinde mit", // 52
    "um WLAN einzustellen", // 53
    "Verbindungsversuch", // 54
    "Fehler!", // 55
    "Verbunden!", // 56
    "Stationlist Rechner-IP", // 57
    "KW-Wellenlänge\nanzeigen", // 58
    "RDS-Filter", // 59
    "Zeige PI-Fehler", // 60
    "Verwende Rauschsperre", // 61
    "Audiopegelanzeige", // 62
    "Rauschunterdrückung", // 63
    "Rauschunterdrückung", // 64
    "Werkseinstellungen\nwurden geladen", // 65
    "Audioausgabe", // 66
    "Bei Bildschirm aus\nTunen erlauben", // 67
    "Anzeigefarben umkehren", // 68
    "Über diese Software", // 69
    "Hauptcode:", // 70
    "Mitwirkende:", // 71
    "Unbekannt", // 72
    "Bildschirmschoner-\nOptionen", // 73
    "Abschaltzeit", // 74
    "Bildschirm aus", // 75
    "Thema", // 76
    "Oberfläche", // 77
    "Erkannte Applikationen", // 78
    "Min.", // 79
    "SPI-Geschw.", // 80
    "Scanempfindlichkeit", // 81
    "KEINE", // 82
    "Softwareversion", // 83
    "Frequenz-Schriftart", // 84
    "Auto", // 85
    "Keine AF verfügbar", // 86
    "Kein EON verfügbar", // 87
    "Kein RT+ verfügbar", // 88
    "UKW\nStandard-Schrittweite", // 80
    "Bildschirmschoner", // 81
    "Sek.", // 82
    "Uhrzeit-Modus", // 83
    "1% Helligkeit", // 84
    "25% Helligkeit", // 85
    "50% Helligkeit", // 86
    "AM RF-Pegeloffset", // 87
    "Einheit des Signalpegels", // 88
    "Verwende AF", // 89
    "Wähle UKW-Band", // 99
    "Wähle AM-Band", // 100
    "LW", // 101
    "MW", // 102
    "KW", // 103
    "UKW", // 104
    "OIRT", // 105
    "Bei Bildschirm aus Tunen erlauben", // 106
    "Modell auswählen", // 107
    "Basismodell", // 108
    "Portabel", // 109
    "Portabel Touch", // 110
    "Titel", // 111
    "Album", // 112
    "Track", // 113
    "Künstler", // 114
    "Komposition", // 115
    "Satz", // 116
    "Dirigent", // 117
    "Komponist", // 118
    "Band", // 119
    "Kommentar", // 120
    "Genre", // 121
    "Nachrichten", // 122
    "Lokalnachrichten", // 123
    "Aktienmarkt", // 124
    "Sport", // 125
    "Lotterie", // 126
    "Horoskop", // 127
    "Tagestipp", // 128
    "Gesundheit", // 129
    "Ereignis", // 130
    "Szene", // 131
    "Kino", // 132
    "TV", // 133
    "Tag/Uhrzeit", // 134
    "Wetter", // 135
    "Verkehr", // 136
    "Alarm", // 137
    "Werbung", // 138
    "Website", // 139
    "Info", // 140
    "Kurzes PS", // 141
    "Langes PS", // 142
    "Es läuft", // 143
    "Es folgt", // 144
    "Thema", // 145
    "Am Mikrofon", // 146
    "Redaktion", // 147
    "Frequenz", // 148
    "Homepage", // 149
    "Subchannel", // 150
    "Telefon Hotline", // 151
    "Telefon Studio", // 152
    "Telefon", // 153
    "SMS Studio", // 154
    "SMS", // 155
    "E-Mail Hotline", // 156
    "E-Mail Studio", // 157
    "E-Mail", // 158
    "Chat", // 159
    "Chat-Zentrale", // 160
    "Abstimmung", // 161
    "Abstimmungs-Zentrale", // 162
    "Ort", // 163
    "Termin", // 164
    "Musik-ID", // 165
    "Bezugsquelle", // 166
    "Empfange Daten", // 167
    "MW\nStandardschrittweite", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Akkuoptionen", // 172
    "Spannung anzeigen", // 173
    "Prozente anzeigen", // 174
    "RT puffern", // 175
    "Haupteinstellungen", // 176
    "Audioeinstellungen", // 177
    "Anzeigeeinstellungen", // 178
    "RDS-Einstellungen", // 179
    "UKW-Einstellungen", // 180
    "AM-Einstellungen", // 181
    "Verbindungen und Uhrzeit", // 182
    "DRÜCKE MODE FÜR ZURÜCK", // 183
    "Gleichkanalerkennung", // 184
    "Gleichkanalerkennung\nEmpfindlichkeit", // 185
    "Gleichkanalerkennung\nSchwelle", // 186
    "Gleichkanalerkennung\nZähler", // 187
    "HAUPT", // 188
    "AUDIO", // 189
    "ANZEIGE", // 190
    "RDS", // 191
    "UKW", // 192
    "AM", // 193
    "VERBIND./UHR", // 194
    "DX-MODUS", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "Entzerrung auf UKW", // 198
    "Wenig", // 199
    "Viel", // 200
    "Maximal", // 201
    "Schnelles PS", // 202
    "Standard", // 203
    "Bildschirm aus bei\nXDRGTK-Verbindung", // 204
    "FMSI Stereo-\nVerbesserung", // 205
    "Funktion nur für\nTEF6687/6689 verfügbar!", // 206
    "Funktion für ausgewählten\nSkin nicht verfügbar!", // 207
    "Starte Scan mit\nSpeicherplatz", // 208
    "Stoppe Scan mit\nSpeicherplatz", // 209
    "Wartezeit", // 210
    "Starte DX-Scan", // 211
    "UKW DX-Optionen", // 212
    "Abbruch! Start-Speicher-\nplatz ist nicht belegt", // 213
    "AF Autosortierung", // 214
    "Speicherplätze scannen", // 215
    "Nein", // 216
    "Ja, ausschließlich", // 217
    "Automatisches\nScan-Ende", // 218
    "Korrekter PI-Code", // 219
    "Signal", // 220
    "Stumm beim Scannen", // 221
    "AIR", // 222
    "BAND-Knopf\n5 Sek. drücken", // 223
    "Ausschalten", // 224
    "Bildschirmschoner", // 225
    "Deaktiviert", // 226
    "Nicht definiert", // 227
    "Nachrichten", // 228
    "Aktuelle Info", // 229
    "Service Programm", // 230
    "Sport", // 231
    "Bildung", // 232
    "Hörspiel + Lit", // 233
    "Kultur+Gesellsch", // 234
    "Wissenschaft", // 235
    "Unterhaltung", // 236
    "Pop Musik", // 237
    "Rock Musik", // 238
    "Unterhalt Musik", // 239
    "Leichte Klassik", // 240
    "Ernste Klassik", // 241
    "Spezielle Musik", // 242
    "Wetter", // 243
    "Wirtschaft", // 244
    "Kinderprogramm", // 245
    "Soziales", // 246
    "Religion", // 247
    "Anrufsendung", // 248
    "Reiseinformation", // 249
    "Freizeit", // 250
    "Jazz", // 251
    "Country Musik", // 252
    "Landesmusik", // 253
    "Oldies Musik", // 254
    "Folklore", // 255
    "Feature", // 256
    "Alarm Test", // 257
    "Alarm! Alarm!", // 258
    "Anfänglich", // 259
    "Immer", // 260
    "Uhr anzeigen", // 261
    "Zeige langes PS", // 262
    "Start-Frequenz", // 263
    "Stopp-Frequenz", // 264
    "Start-Speicherplatz", // 265
    "Stopp-Speicherplatz", // 266
    "Speichern nur mit RDS", // 267
    "Starte Auto-Speichern", // 268
    "Auto-Speichern", // 269
    "Stationlist-ID", // 270
    "gefunden", // 271
    "Abbruch! Speicherplätze voll", // 272
    "Abbruch! Taste wurde gedrückt", // 273
    "Scan ohne Fehler beendet", // 274
    "Lösche Speicherplätze", // 275
    "gelöscht", // 276
    "Doppelte PI verhindern", // 277
    "Bereich", // 278
    "Voll", // 279
    "Wartezeit nur wenn\nSignal vorhanden", // 280
    "Markierte Ecke drücken", // 281
    "zum Kalibrieren des Touchscrees", // 282
    "Zum Stoppen des DX-Scans\nTaste BW lange drücken", // 283
    "Bandbreite wählen", // 284
    "Dein Logbuch", // 285
    "Logbuch herunterladen", // 286
    "Logbuch ist leer", // 287
    "Zum Ende gehen", // 288
    "Logbuch-Eintrag\nwurde erstellt", // 289
    "Logbuch-Eintrag\nist fehlgeschlagen", // 290
    "Logbuch löschen", // 291
    "Logbuch gelöscht", // 292
    "Löschen fehlgeschlagen", // 293
    "Logbucheintrag nicht möglich\nLogbuch ist voll!", // 294
    "Automatisch loggen", // 295
    "Auto-Sommerzeit\nüber NTP", // 296
    "NTP-Zeitversatz\neinstellen", // 297
    "Das Logbuch enthält mehr als 130 Einträge, was das Anzeigeprogramm nicht verarbeiten kann. Bitte lade die CSV-Datei zum Verarbeiten herunter.", // 298
    "Kanada" // 299
  },

  { "Čeština", // Czech
    "Směr enkodéru byl změněn", // 1
    "Prosím, pusťte tlačítko", // 2
    "Obrazovka otočena", // 3
    "Kalibrace analog. měřiče", // 4
    "Režim USB", // 5
    "Enkodér nastaven na optický", // 6
    "Enkodér nastaven na standardní", // 7
    "Tuner: !ŽÁDNÝ!", // 8
    "Hlasitost", // 9
    "Konvertor", // 10
    "Spodek pásma", // 11
    "Vrch pásma", // 12
    "FM offset signálu", // 13
    "Práh pro stereo", // 14
    "Práh frekvence zvuku k utlumení", // 15
    "Práh signálu pro utlumení výšek", // 16
    "Práh nízkého signálu", // 17
    "Jas", // 18
    "Nastavení hlasitosti", // 19
    "Offset frekvence", // 20
    "Spodek pásma", // 21
    "Vrch pásma", // 22
    "Offset signálu", // 23
    "Práh pro stereo", // 24
    "Práh frekvence zvuku k utlumení", // 25
    "Práh signálu pro utlumení výšek", // 26
    "Práh nízkého signálu", // 27
    "Jas obrazovky", // 28
    "Vyp.", // 29
    "Zap.", // 30
    "ZMÁČKNĚTE MODE PRO UKONČENÍ A ULOŽENÍ", // 31
    "ZMÁČKNĚTE BAND PRO ZAVŘENÍ", // 32
    "SKENUJI...", // 33
    "Tuner nedetekován", // 34
    "Zisk AM antény", // 35
    "FM AGC", // 36
    "Zobrazit chyby v RDS", // 37
    "Jazyk", // 38
    "Vyberte jazyk", // 39
    "Synchronizovat CT,\nkdyž je PI uzamčeno", // 40
    "Chyba! Bez RDS!", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Pípnout na konci pásma", // 44
    "Region", // 45
    "Evropa", // 46
    "USA", // 47
    "Zobrazit podtržítka v RDS", // 48
    "Jen PS", // 49
    "Wi-Fi aktivní", // 50
    "Konfigurace Wi-Fi", // 51
    "Připojte se k: ", // 52
    "pro nakonfigurování Wi-Fi", // 53
    "Wi-Fi připojení probíhá...", // 54
    "NEPŘIPOJENO!", // 55
    "PŘIPOJENO!", // 56
    "Stationlist IP", // 57
    "Zobrazit SW pásmo", // 58
    "RDS filtr", // 59
    "Zobrazit chyby v PI", // 60
    "Použít squelch", // 61
    "Graf modulace", // 62
    "Tlumič šumu (AM)", // 63
    "Tlumič šumu (FM)", // 64
    "Výchozí hodnoty načteny", // 65
    "Zvukový výstup", // 66
    "Povolit ladění přes spořič", // 67
    "Invertování barev", // 68
    "O softwaru", // 69
    "Hlavní kód:", // 70
    "Přispěli:", // 71
    "Neznámý", // 72
    "Nastavení\nspořiče obrazovky", // 73
    "Časovač vypnutí", // 74
    "Vypnutí obrazovky", // 75
    "Vzhled", // 76
    "Skin", // 77
    "Detekované aplikace", // 78
    "minut", // 79
    "Rychlost SPI", // 80
    "Citlivost skenu", // 81
    "žádné", // 82
    "Verze softwaru", // 83
    "Font frekvence", // 84
    "Auto", // 85
    "AF nedostupné", // 86
    "EON nedostupný", // 87
    "RT+ nedostupný", // 88
    "Výchozí krok ladění FM", // 80
    "Spořič obrazovky", // 81
    "sec.", // 82
    "Režim hodin", // 83
    "1% jas", // 84
    "25% jas", // 85
    "50% jas", // 86
    "AM offset signálu", // 87
    "Jednotka signálu", // 88
    "Použijte AF", // 89
    "Zvolte FM pásmo", // 99
    "Zvolte AM pásmo", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Ladění při vypnuté obrazovce", // 106
    "Volba modelu", // 107
    "Základní", // 108
    "Přenosný", // 109
    "Přenosný-dotykový(ILI9341)", // 110
    "Název", // 111
    "Album", // 112
    "Stopa", // 113
    "Interpret", // 114
    "Kompozice", // 115
    "Věta", // 116
    "Dirigent", // 117
    "Skladatel", // 118
    "Kapela", // 119
    "Komentář", // 120
    "Žánr", // 121
    "Zprávy", // 122
    "Místní zprávy", // 123
    "Burza", // 124
    "Sport", // 125
    "Loterie", // 126
    "Horoskop", // 127
    "Každodenní zábava", // 128
    "Zdraví", // 129
    "Událost", // 130
    "Scéna", // 131
    "Kino", // 132
    "Televize", // 133
    "Datum/čas", // 134
    "Počasí", // 135
    "Doprava", // 136
    "Alarm", // 137
    "Reklama", // 138
    "Webová stránka", // 139
    "Ostatní", // 140
    "Short PS", // 141
    "Long PS", // 142
    "Nyní", // 143
    "Následuje", // 144
    "Část", // 145
    "Moderátor", // 146
    "Redaktor", // 147
    "Frekvence", // 148
    "Hlavní stránka", // 149
    "Podkanál", // 150
    "Horká linka", // 151
    "Telefon do studia", // 152
    "Telefon", // 153
    "SMS do studia", // 154
    "SMS", // 155
    "Emailová hotline", // 156
    "Email do studia", // 157
    "Email", // 158
    "Chat", // 159
    "Centrum chatu", // 160
    "Hlasovací otázka", // 161
    "Hlasovací centrum", // 162
    "Místo", // 163
    "Schůzka", // 164
    "Identifikátor", // 165
    "Nákup", // 166
    "Získat data", // 167
    "Výchozí krok ladění MW", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Úroveň nabití baterie", // 172
    "Zobrazit napětí", // 173
    "Zobrazit procenta", // 174
    "RT buffering", // 175
    "Hlavní nastavení", // 176
    "Nastavení zvuku", // 177
    "Nastavení displeje", // 178
    "Nastavení RDS", // 179
    "Nastavení FM pásma", // 180
    "Nastavení AM pásma", // 181
    "Konektivita a hodiny", // 182
    "ZMÁČKNĚTE MODE PRO NÁVRAT", // 183
    "AM CoChannelDet", // 184
    "Citlivost CoChannelDet", // 185
    "Práh AM CoChannelDet", // 186
    "CoChannelDet počítadlo", // 187
    "HLAVNÍ", // 188
    "ZVUK", // 189
    "DISPLEJ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "KONEKTIVITA", // 194
    "DX REŽIM", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM deemfáze", // 198
    "Malé", // 199
    "Velké", // 200
    "Vše", // 201
    "Rychlé načtení PS", // 202
    "Výchozí", // 203
    "Vypnout displej při\nXDRGTK připojení", // 204
    "FMSI stereo\nvylepšení", // 205
    "Funkce dostupná pouze\nu TEF6687/6689!", // 206
    "Funkce není dostupná\nu zvoleného skinu!", // 207
    "Začít skenování\nod předvolby č.", // 208
    "Skončit skenování\nu předvolby č.", // 209
    "Doba čekání", // 210
    "Spustit DX skenování", // 211
    "Nastavení FM DX", // 212
    "Přerušeno! Začáteční předvolba\nje označena jako prázdná.", // 213
    "Auto řazení AF", // 214
    "Předvolby paměti", // 215
    "Vyloučit", // 216
    "Pouze", // 217
    "Automaticky zastavit\nskenování", // 218
    "Korektní PI", // 219
    "Signál", // 220
    "Ztlumit zvuk\nběhem skenování", // 221
    "AIR", // 222
    "Po 5 sec. stisknutí\ntl. BAND", // 223
    "Vypnout", // 224
    "Vypnout displej", // 225
    "Zakázáno", // 226
    "Nedefinovaný", // 227
    "Zprávy", // 228
    "Publicistika", // 229
    "Informace", // 230
    "Sport", // 231
    "Vzdělání", // 232
    "Drama", // 233
    "Kultura", // 234
    "Věda", // 235
    "Různé žánry", // 236
    "Popová hudba", // 237
    "Rocková hudba", // 238
    "Oddychová hudba", // 239
    "Klasická hudba", // 240
    "Vážná hudba", // 241
    "Jiná hudba", // 242
    "Počasí", // 243
    "Finance", // 244
    "Dětský program", // 245
    "Společenská tém.", // 246
    "Naboženství", // 247
    "Telefonáty", // 248
    "Cestování", // 249
    "Zábava", // 250
    "Jazzová hudba", // 251
    "Country hudba", // 252
    "Národní hudba", // 253
    "Oldies hudba", // 254
    "Folková hudba", // 255
    "Dokumenty", // 256
    "Alarm Test", // 257
    "Alarm – Alarm !", // 258
    "Počáteční", // 259
    "Vždy", // 260
    "Zobrazit hodiny", // 261
    "Zobrazit long PS", // 262
    "Počáteční frekvence", // 263
    "Poslední frekvence", // 264
    "Počáteční paměťový kanál", // 265
    "Poslední paměťový kanál", // 266
    "Ukládat jen s RDS", // 267
    "Spustit automatickou paměť", // 268
    "Automatická paměť", // 269
    "ID seznamu stanic", // 270
    "Nalezeno", // 271
    "Přerušeno! Nedostatek paměťových kanálů", // 272
    "Přerušeno! Uživatel stiskl tlačítko", // 273
    "Skenování dokončen bez chyb", // 274
    "Vymazat paměťové kanály", // 275
    "vymazáno", // 276
    "Zamezit dvojitému PI", // 277
    "Rozsah", // 278
    "Plný", // 279
    "Čekací doba pouze na\nsignal", // 280
    "Stiskněte zvýrazněný roh", // 281
    "Pro kalibraci dotekové obrazovky", // 282
    "Pro zastavení\npodržte tlačítko BW", // 283
    "Vyberte šířku pásma", // 284
    "Váš logbook", // 285
    "Stáhnout logbook", // 286
    "Logbook je prázdný", // 287
    "Přejít na konec", // 288
    "Přidáno do logu", // 289
    "Chyba při přidávání\npoložky do logbooku", // 290
    "Vymazat logbook", // 291
    "Logbook vymazán", // 292
    "Vymazání se nezdařilo", // 293
    "Nelze přidat položku!\nLogbook je plný!", // 294
    "Automatický logger", // 295
    "Auto DST u NTP času", // 296
    "Nastavte časový posun NTP", // 297
    "Logbook obsahuje více než 130 položek, které prohlížeč nedokáže zpracovat. Pro zpracování všech položek si stáhněte CSV soubor.", // 298
    "Canada" // 299
  },

  { "Magyar", // Hungarian
    "Forgási irány megváltozott", // 1
    "Engedd el a gombot", // 2
    "Képernyő megfordítva", // 3
    "Analóg mérő kalibrálása", // 4
    "USB mód", // 5
    "Kódoló optikaira állítva", // 6
    "Kódoló szabványosra állítva", // 7
    "Tuner: !Nincs!", // 8
    "Hangerő", // 9
    "Átalakító", // 10
    "Legkisebb sávszél", // 11
    "Legnagyobb sávszél", // 12
    "FM RF szinteltolás", // 13
    "Sztereó küszöbérték", // 14
    "Magasak sarokja", // 15
    "Magasak küszöbértéke", // 16
    "Alacsonyak küszöbértéke", // 17
    "Kijelző fényereje", // 18
    "Hangerő beállítása", // 19
    "Átalakító eltolása", // 20
    "Legkisebb sávszél", // 21
    "Legnagyobb sávszél", // 22
    "Szinteltolás", // 23
    "Sztereó szétválasztás küszöbértéke", // 24
    "Magasak sarokfrekvenciája", // 25
    "Magasak küszöbértéke", // 26
    "Alacsony szint küszöbérték", // 27
    "Kijelző fényereje", // 28
    "Ki", // 29
    "Be", // 30
    "MODE A MENTÉSHEZ ÉS BEZÁRÁSHOZ", // 31
    "BAND A BEZÁRÁSHOZ", // 32
    "KERESÉS...", // 33
    "Tuner nem észlelhető", // 34
    "AM Antenna érzékenység", // 35
    "FM AGC", // 36
    "RDS hibák megjelenítése", // 37
    "Nyelv", // 38
    "Válassz nyelvet", // 39
    "PI zárolt\nCT szinkronizálás", // 40
    "Hiba! Nincs RDS jel", // 41
    "FM halkítás", // 42
    "AM halkítás", // 43
    "Sípszó a sáv szélén", // 44
    "Régió", // 45
    "Európa", // 46
    "USA", // 47
    "Aláhúzások megjelenítése\nRDS-ben", // 48
    "Csak PS", // 49
    "Wi-Fi", // 50
    "Wi-Fi beállítása", // 51
    "Csatlakozz ehhez: ", // 52
    "a Wi-Fi beállításához", // 53
    "Wi-Fi kapcsolódási\nkísérlet", // 54
    "SIKERTELEN!", // 55
    "CSATLAKOZVA!", // 56
    "Stationlist kliens IP", // 57
    "SW hullámhossz\nmegjelenítése", // 58
    "RDS szűrő", // 59
    "PI hibák megjelenítése", // 60
    "Zajzár(SQ) használata", // 61
    "Modulációmérő", // 62
    "AM Zajszűrő", // 63
    "FM Zajszűrő", // 64
    "Alapértelmezettek\nbetöltve", // 65
    "Audió kimenet", // 66
    "Hangolás\nképernyővédővel", // 67
    "Inverz kijelző", // 68
    "A szoftverről", // 69
    "Fő kód:", // 70
    "Közreműködők:", // 71
    "Ismeretlen", // 72
    "Képernyővédő opciók", // 73
    "Időtúllépés időzítő", // 74
    "Kijelző ki", // 75
    "Téma", // 76
    "Kinézet", // 77
    "Felismert alkalmazások", // 78
    "Perc", // 79
    "SPI sebesség", // 80
    "Keresés érzékenység", // 81
    "NINCS", // 82
    "Szoftver verzió", // 83
    "Frekvencia betűtípusa", // 84
    "Auto", // 85
    "Nincs elérhető AF", // 86
    "Nincs elérhető EON", // 87
    "Nincs elérhető RT+", // 88
    "FM alap. lépésköz", // 80
    "Képernyővédő", // 81
    "Mp", // 82
    "Óra mód", // 83
    "1% fényerő", // 84
    "25% fényerő", // 85
    "50% fényerő", // 86
    "AM RF szinteltolás", // 87
    "Jelmérő egység", // 88
    "AF használata", // 89
    "FM sáv kiválasztása", // 99
    "AM sáv kiválasztása", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Hangolás kikapcsolt képernyővel", // 106
    "Modellválasztó", // 107
    "Alap", // 108
    "Hordozható", // 109
    "Hord. érintős", // 110
    "Cím", // 111
    "Album", // 112
    "Szám", // 113
    "Előadó", // 114
    "Kompozíció", // 115
    "Tétel", // 116
    "Karmester", // 117
    "Zeneszerző", // 118
    "Zenekar", // 119
    "Megjegyzés", // 120
    "Műfaj", // 121
    "Hírek", // 122
    "Helyi hírek", // 123
    "Tőzsdepiac", // 124
    "Sport", // 125
    "Lottó", // 126
    "Horoszkóp", // 127
    "Napi kikapcsolódás", // 128
    "Egészség", // 129
    "Esemény", // 130
    "Jelenet", // 131
    "Mozi", // 132
    "TV", // 133
    "Dátum/idő", // 134
    "Időjárás", // 135
    "Közlekedés", // 136
    "Riasztás", // 137
    "Hirdetés", // 138
    "Weboldal", // 139
    "Egyéb", // 140
    "Rövid PS", // 141
    "Hosszú PS", // 142
    "Most", // 143
    "Következő", // 144
    "Rész", // 145
    "Házigazda", // 146
    "Szerkesztő", // 147
    "Frekvencia", // 148
    "Kezdőlap", // 149
    "Alcsatorna", // 150
    "Forródrót", // 151
    "Stúdió telefon", // 152
    "Telefon", // 153
    "Stúdió SMS", // 154
    "SMS", // 155
    "Email forródrót", // 156
    "Stúdió Email", // 157
    "Email", // 158
    "Csevegés", // 159
    "Csevegőközpont", // 160
    "Szavazási kérdés", // 161
    "Szavazóközpont", // 162
    "Helyszín", // 163
    "Találkozó", // 164
    "Azonosító", // 165
    "Vásárlás", // 166
    "Adatok lekérése", // 167
    "MW alap. lépésköz", // 168
    "Tangerin", // 169
    "Óceán", // 170
    "Indigó", // 171
    "Akkumulátor jelzése", // 172
    "Feszültség mutatása", // 173
    "Százalék mutatása", // 174
    "RT pufferelés", // 175
    "Fő beállítások", // 176
    "Audió beállítások", // 177
    "Kijelző beállítások", // 178
    "RDS beállítások", // 179
    "FM beállítások", // 180
    "AM beállítások", // 181
    "Csatlakoztathatóság és óra", // 182
    "MODE A VISSZALÉPÉSHEZ", // 183
    "CoChannel Detektor", // 184
    "CoChannel Detektor\nérzékenység", // 185
    "CoChannel Detektor\nküszöbérték", // 186
    "CoChannel Detektor\nszámláló", // 187
    "FŐ", // 188
    "AUDIÓ", // 189
    "KIJELZŐ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CSATLAKOZÁS", // 194
    "DX MÓD", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM kihangolás", // 198
    "Kicsi", // 199
    "Nagy", // 200
    "Mind", // 201
    "Gyors PS", // 202
    "Alap", // 203
    "Képernyő némítása\nXDRGTK kapcsolaton", // 204
    "FMSI sztereó\njavítás", // 205
    "Funkció csak a\nTEF6687/6689-hez!", // 206
    "Funkció nem elérhető\nebben a kinézetben!", // 207
    "Mem. csat. szkennelésének\nelkezdése", // 208
    "Mem. csat. szkennelésének\nbefejezése", // 209
    "Várakozási idő", // 210
    "DX szkennelés indítása", // 211
    "FM DX Opciók", // 212
    "Megszakítva! A kezdőcsatorna\nkihagyottnak van jelölve.", // 213
    "Auto AF rendezés", // 214
    "Memóriacsatornák", // 215
    "Kizárása", // 216
    "Csak", // 217
    "Auto keresés\nmegszakítása", // 218
    "Jó PI esetén", // 219
    "Jel esetén", // 220
    "Hang némítása\nkeresés közben", // 221
    "AIR", // 222
    "5 mp. sávgombnyomás", // 223
    "Kikapcsolás", // 224
    "Képernyőnémítás", // 225
    "Letiltva", // 226
    "Nincs", // 227
    "Hírek", // 228
    "Aktuális események", // 229
    "Információ", // 230
    "Sport", // 231
    "Oktatás", // 232
    "Dráma", // 233
    "Kultúra", // 234
    "Tudomány", // 235
    "Változatos", // 236
    "Pop Zene", // 237
    "Rock Zene", // 238
    "Könnyűzene", // 239
    "Könnyű Klasszikus", // 240
    "Komoly Klasszikus", // 241
    "Egyéb Zene", // 242
    "Időjárás", // 243
    "Pénzügy", // 244
    "Gyermekprogramok", // 245
    "Társadalmi ügyek", // 246
    "Vallás", // 247
    "Betelefonálós", // 248
    "Utazás", // 249
    "Szabadidő", // 250
    "Jazz Zene", // 251
    "Country Zene", // 252
    "Nemzeti Zene", // 253
    "Régi Zene", // 254
    "Népi Zene", // 255
    "Dokumentumfilm", // 256
    "Riasztás Teszt", // 257
    "Riasztás!!!", // 258
    "Kezdeti", // 259
    "Mindig", // 260
    "Óra mutatása", // 261
    "Hosszú PS mutatása", // 262
    "Indítási frekvencia", // 263
    "Befejezési frekvencia", // 264
    "Memóriacsatorna\nmentés indítása", // 265
    "Memóriacsatorna\nmentés leállítása", // 266
    "Csak RDS-el tárolja", // 267
    "Auto memória indítása", // 268
    "Auto memória", // 269
    "Stationlist ID", // 270
    "Megtalálva", // 271
    "Megszakítva! Nincs\nmemóriacsatorna", // 272
    "Megszakítva! Felhasználó\nmegnyomta a gombot", // 273
    "Szkennelés hiba\nnélkül befejeződött", // 274
    "Memóriacsatornák törlése", // 275
    "törölve", // 276
    "Kettős PI\nmegakadályozása", // 277
    "Tartomány", // 278
    "Teljes", // 279
    "Várakozási idő\ncsak jel esetén", // 280
    "Nyomd meg a kiemelt sarkot", // 281
    "Érintőképernyő kalibrálásához", // 282
    "Tartsd lenyomva a\nBW-t a megállításhoz", // 283
    "Sávszélesség választása", // 284
    "A logbookod", // 285
    "Logbook letöltése", // 286
    "A logbook üres", // 287
    "Ugrás az aljára", // 288
    "Hozzáadva a loghoz", // 289
    "Hiba a naplóhoz adáskor", // 290
    "Logbook törlése", // 291
    "Logbook törölve", // 292
    "Sikertelen törlés", // 293
    "Nem adható hozzá!\nLogbook megtelt!", // 294
    "Auto logoló", // 295
    "Auto nyári idő(DST)\nNTP idő szerint", // 296
    "NTP időeltolás beállítása", // 297
    "A napló több mint 130 bejegyzést tartalmaz, amelyeket a megtekintő nem tud feldolgozni. Kérlek, töltsd le a CSV fájlt a megtekintéshez.", // 298
    "Kanada" // 299
  },

  { "Français", // French
    "Sens de rotation modifié", // 1
    "Veuillez relâcher\nle bouton", // 2
    "Écran inversé", // 3
    "Calibrer le mesureur\nanalogique", // 4
    "Mode USB", // 5
    "Codeur réglé\nsur optique", // 6
    "Codeur réglé\nsur standard", // 7
    "Tuner : Aucun !", // 8
    "Volume", // 9
    "Convertisseur", // 10
    "Limite de bande basse", // 11
    "Limite de bande haute", // 12
    "Décalage du niveau RF FM", // 13
    "Seuil minimum\npour la stéréo", // 14
    "Angle du filtre\nà coupe haute", // 15
    "Seuil du filtre\nà coupe haute", // 16
    "Seuil minimum de\nniveau bas", // 17
    "Luminosité", // 18
    "Réglage du volume", // 19
    "Décalage du convertisseur", // 20
    "Limite de bande basse", // 21
    "Limite de bande haute", // 22
    "Décalage du niveau", // 23
    "Seuil de séparation\nstéréo", // 24
    "Fréquence de coupe\nélevée", // 25
    "Seuil de coupe élevée", // 26
    "Seuil de niveau bas", // 27
    "Luminosité de l'affichage", // 28
    "Éteint", // 29
    "Allumé", // 30
    "BUTON MODE POUR SORTIR & SAUVEGARDER", // 31
    "APPUYEZ SUR BAND POUR FERMER", // 32
    "SCAN EN COURS...", // 33
    "Tuner non détecté", // 34
    "Gain de l'antenne AM", // 35
    "FM AGC", // 36
    "Afficher les erreurs\nde décodage RDS", // 37
    "Langage", // 38
    "Choisir la langue", // 39
    "Verrouillage de la\nsync. CT sur PI", // 40
    "Erreur ! Pas de signal RDS", // 41
    "Sourdine douce FM", // 42
    "Sourdine douce AM", // 43
    "Bip à la limite\nde bande", // 44
    "Région", // 45
    "Europe", // 46
    "États-Unis", // 47
    "Afficher les tirets du\nbas sur le PS", // 48
    "PS uniquement", // 49
    "Wi-Fi actif", // 50
    "Configuration du Wi-Fi", // 51
    "Se connecter au réseau", // 52
    "pour configurer\nle Wi-Fi", // 53
    "Tentative de connexion\nen mode Wi-Fi", // 54
    "CONNEXION IMPOSSIBLE...\nDésactivation du Wi-Fi", // 55
    "CONNECTÉ !", // 56
    "IP du client Stationlist", // 57
    "Afficher la longueur\nd'onde SW", // 58
    "Filtre RDS", // 59
    "Afficher les erreurs\nde décodage PI", // 60
    "Utiliser le squelch", // 61
    "Mesure de la modulation", // 62
    "Réducteur de bruit AM", // 63
    "Réducteur de bruit FM", // 64
    "Réglages par défaut\nchargés", // 65
    "Sortie audio", // 66
    "Scanner sans désactiver\nl'économiseur d'écran", // 67
    "Inverser les\ncouleurs de l'écran", // 68
    "À propos du logiciel", // 69
    "Code de base:", // 70
    "Contributeurs:", // 71
    "Inconnu", // 72
    "Options d'écran\nde veille", // 73
    "Arrêt automatique", // 74
    "Arrêt de l'afficheur", // 75
    "Thème", // 76
    "Mode d'affichage (Skin)", // 77
    "Applications détectées", // 78
    "Min.", // 79
    "Vitesse SPI", // 80
    "Sensibilité\ndu scan", // 81
    "AUCUN", // 82
    "Version logicielle", // 83
    "Police d'affichage\npour la fréquence", // 84
    "Auto", // 85
    "AF indisponible", // 86
    "EON indisponible", // 87
    "RT+ indisponible", // 88
    "Valeur de pas FM\npar défaut", // 80
    "Écran de veille", // 81
    "Sec", // 82
    "Mode horloge", // 83
    "Luminosité à 1%", // 84
    "Luminosité à 25%", // 85
    "Luminosité à 50%", // 86
    "Décalage du\nniveau RF AM", // 87
    "Unité de mesure\ndu signal", // 88
    "Utiliser la fonction AF", // 89
    "Sélectionner la\nbande FM", // 99
    "Sélectionner la\nbande AM", // 100
    "LW", // 101
    "AM", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Arrêter la sortie de veille\nlors du scan", // 106
    "Sélection du modèle", // 107
    "Base", // 108
    "Portable", // 109
    "Portable à écran tactile", // 110
    "Titre", // 111
    "Album", // 112
    "Piste", // 113
    "Artiste", // 114
    "Composition", // 115
    "Mouvement", // 116
    "Chef d'orchestre", // 117
    "Compositeur", // 118
    "Groupe", // 119
    "Commentaire", // 120
    "Genre", // 121
    "Actualités", // 122
    "Actualités locales", // 123
    "Bourse", // 124
    "Sport", // 125
    "Loterie", // 126
    "Horoscope", // 127
    "Divertissement quotidien", // 128
    "Santé", // 129
    "Événement", // 130
    "Scène", // 131
    "Cinéma", // 132
    "TV", // 133
    "Date/heure", // 134
    "Météo", // 135
    "Trafic", // 136
    "Alarme", // 137
    "Publicité", // 138
    "Site web", // 139
    "Autre", // 140
    "PS court", // 141
    "PS long", // 142
    "Maintenant", // 143
    "À suivre", // 144
    "Partie", // 145
    "Animateur", // 146
    "Rédacteur", // 147
    "Fréquence", // 148
    "Page d'accueil", // 149
    "Sous-canal", // 150
    "Hotline", // 151
    "Num. Téléphone Studio", // 152
    "Num. Téléphone", // 153
    "Num. SMS Studio", // 154
    "Numéro SMS", // 155
    "Email de la hotline", // 156
    "Email du studio", // 157
    "Email", // 158
    "Chat", // 159
    "Centre de discussion", // 160
    "Question de vote", // 161
    "Centre de vote", // 162
    "Lieu", // 163
    "Rendez-vous", // 164
    "Identifiant", // 165
    "Acheter", // 166
    "Obtenir des données", // 167
    "Valeur de pas AM\npar défaut", // 168
    "Mandarine", // 169
    "Océan", // 170
    "Indigo", // 171
    "Options de batterie", // 172
    "Montrer la tension", // 173
    "Montrer le pourcentage", // 174
    "Mise en mémoire tampon RT", // 175
    "Paramètres principaux", // 176
    "Paramètres audio", // 177
    "Paramètres d'affichage", // 178
    "Paramètres RDS", // 179
    "Paramètres FM", // 180
    "Paramètres AM", // 181
    "Connectivité et horloge", // 182
    "BOUTON MODE POUR REVENIR EN ARRIÈRE", // 183
    "Détecteur de canal adjacent AM", // 184
    "Sensibilité du détecteur\nde canal adjacent", // 185
    "Seuil du détecteur\nde canal adjacent", // 186
    "Compteurdu détecteur\nde canal adjacent", // 187
    "PRINCIPAL", // 188
    "AUDIO", // 189
    "AFFICHAGE", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CONNECTER", // 194
    "MODE DX", // 195
    "MEMORISATION AUTO", // 196
    "AGC AM", // 197
    "Désaccentuation FM", // 198
    "Petites", // 199
    "Grosses", // 200
    "Toutes", // 201
    "PS rapide", // 202
    "Défaut", // 203
    "Arrêt écran à la\nconnexion XDRGTK", // 204
    "Amélioration\nde la stéréo FMSI", // 205
    "Fonction disponible\nsur TEF6687/6689!", // 206
    "Fonction non disponible\nsur ce mode d'affichage !", // 207
    "Début du scan\ndes canaux mémorisés", // 208
    "Fin du scan\ndes canaux mémorisés", // 209
    "Temps d'attente", // 210
    "Démarrer le scan DX", // 211
    "Options FM DX", // 212
    "Abandon ! Le canal de\ndépart est indiqué comme ignoré.", // 213
    "Tri automatique\nde la liste AF", // 214
    "Canaux en mémoire", // 215
    "Exclure", // 216
    "Uniquement", // 217
    "Interrompre le scan\nautomatiquement", // 218
    "Code PI correct", // 219
    "Signal", // 220
    "Mettre l'audio en\nsourdine lors du scan", // 221
    "AIR", // 222
    "Appui de 5 sec.\nsur BAND", // 223
    "Éteindre l'appareil", // 224
    "Éteindre l'écran", // 225
    "Désactivé", // 226
    "Non défini", // 227
    "Actualités", // 228
    "Affaires courantes", // 229
    "Information", // 230
    "Sport", // 231
    "Éducation", // 232
    "Fiction", // 233
    "Culture", // 234
    "Science", // 235
    "Varié", // 236
    "Musique Pop", // 237
    "Musique Rock", // 238
    "Musique légère", // 239
    "Classique Léger", // 240
    "Classique Sérieux", // 241
    "Autre Musique", // 242
    "Météo", // 243
    "Economie", // 244
    "Prog. pour enfants", // 245
    "Affaires sociales", // 246
    "Religion", // 247
    "Ligne ouverte", // 248
    "Voyage", // 249
    "Loisirs", // 250
    "Musique Jazz", // 251
    "Musique Country", // 252
    "Musique Nationale", // 253
    "Musique Rétro", // 254
    "Folklore", // 255
    "Documentaire", // 256
    "TEST d'alerte", // 257
    "Alerte !", // 258
    "Initial", // 259
    "Toujours", // 260
    "Afficher l'horloge", // 261
    "Afficher le PS long", // 262
    "Fréquence initialle", // 263
    "Fréquence finale", // 264
    "Canal\nde mémoire initial", // 265
    "Canal\nde mémoire final", // 266
    "Mémoriser avec\nRDS uniquement", // 267
    "Démarrer la mémorisation\nautomatique", // 268
    "Mémorisation\nautomatique", // 269
    "Identifiant\nStationlist", // 270
    "Trouvé", // 271
    "Abandon! Mémoire de\ncanaux pleine", // 272
    "Abandon! Bouton\npressé par l'utilisateur", // 273
    "Scan terminé\nsans erreurs", // 274
    "Effacer les canaux\nmémorisés", // 275
    "effacé", // 276
    "Empêcher doublon PI", // 277
    "Plage", // 278
    "Complet", // 279
    "Temps d'attente\nuniquement sur le signal", // 280
    "Appuyez sur le coin\nindiqué pour ", // 281
    "calibrer l'écran tactile", // 282
    "Pressez BW pour\narrêter le scanner DX", // 283
    "Sélectionner la bande passante", // 284
    "Votre journal de bord", // 285
    "Télécharger le journal de bord", // 286
    "Journal de bord vide", // 287
    "Aller en bas", // 288
    "Entrée ajoutée au journal de bord", // 289
    "Erreur lors de l'ajout\nau journal de bord", // 290
    "Effacer le journal\nde bord", // 291
    "Journal effacé", // 292
    "Échec lors de\nla suppression", // 293
    "Impossible d'ajouter l'entrée!\nJournal de bord plein!", // 294
    "Log automatique", // 295
    "DST automatique\nsur l'heure NTP", // 296
    "Décalage horaire NTP", // 297
    "Le journal de bord contient plus de 130 entrées que la liste ne peut pas afficher. Veuillez télécharger le fichier CSV.", // 298
    "Canada" // 299
  },

  { "Български", // Bulgarian
    "Посоката е променена", // 1
    "Моля, пуснете бутона", // 2
    "Екранът бе обърнат", // 3
    "Калибрация на\nаналоговото измерване", // 4
    "USB режим", // 5
    "Енкодерът е в\nоптичен режим", // 6
    "Енкодерът е в\nстандартен режим", // 7
    "Тунер: !Няма!", // 8
    "Сила на звука", // 9
    "Конвертор", // 10
    "Нисък край", // 11
    "Висок край", // 12
    "FM RF ниво", // 13
    "Стерео праг", // 14
    "Честота на High Cut", // 15
    "Праг на High Cut", // 16
    "Праг на слаб сигнал", // 17
    "Яркост", // 18
    "Сила на звука", // 19
    "Отместване на\nконвертора", // 20
    "Нисък край на\nчестотната лента", // 21
    "Висок край на\nчестотната лента", // 22
    "Отместване на\nнивото", // 23
    "Праг на стерео разделяне", // 24
    "Честота на висок отрез", // 25
    "Праг на висок отрез", // 26
    "Праг за слаб сигнал", // 27
    "Яркост на дисплея", // 28
    "Изкллючв.", // 29
    "Включв.", // 30
    "НАТИСНЕТЕ MODE ЗА ИЗХОД И ЗАПИС", // 31
    "НАТИСНЕТЕ BAND ЗА ЗАТВАРЯНЕ", // 32
    "СКЕНИРАНЕ...", // 33
    "Тунерът не\nе разпознат!", // 34
    "Усилване на\nAM антена", // 35
    "FM АУР", // 36
    "Показване на\nRDS грешки", // 37
    "Език", // 38
    "Изберете език", // 39
    "PI заключен\nCT синхрон", // 40
    "Няма RDS сигнал", // 41
    "Меко мютиране FM", // 42
    "Меко мютиране AM", // 43
    "Сигнал при граница\nна лента", // 44
    "Регион", // 45
    "Европа", // 46
    "USA", // 47
    "Долна черта в RDS", // 48
    "Само PS", // 49
    "Wi-Fi активен", // 50
    "Конфигуриране на Wi-Fi", // 51
    "Свързване с: ", // 52
    "за конфигуриране на Wi-Fi", // 53
    "Опит за свързване на Wi-Fi", // 54
    "НЕУСПЕШНО!", // 55
    "СВЪРЗАНО!", // 56
    "IP на клиент за станции", // 57
    "SW дължина на вълната", // 58
    "RDS филтър", // 59
    "Показване на\nPI грешки", // 60
    "Използване на сквич", // 61
    "Метър на модулация", // 62
    "AM шумозаглушител", // 63
    "FM шумозаглушител", // 64
    "Настройки по\nподразбиране", // 65
    "Аудио изход", // 66
    "Настройка без деактивиране\nна скрийнсейвър", // 67
    "Инверсия на цветовете", // 68
    "Относно софтуера", // 69
    "Основен код:", // 70
    "Сътрудници:", // 71
    "Неизвестно", // 72
    "Настройки на\nскрийнсейвъра", // 73
    "Автоматично\nизключване", // 74
    "Изключване на екрана", // 75
    "Тема", // 76
    "Облик", // 77
    "Открити приложения", // 78
    "Мин.", // 79
    "Динамична SPI\nскорост", // 80
    "Чувствителност\nпри сканиране", // 81
    "НЯМА", // 82
    "Версия на софтуера", // 83
    "Шрифт на честотата", // 84
    "Автоматично", // 85
    "Няма налични AF", // 86
    "Няма налични EON", // 87
    "Няма налични RT+", // 88
    "Стъпка по\nподразбиране за FM", // 80
    "Скрийнсейвър", // 81
    "Сек.", // 82
    "Режим на часовника", // 83
    "1% яркост", // 84
    "25% яркост", // 85
    "50% яркост", // 86
    "Отместване на AM RF ниво", // 87
    "Единица за сигнал", // 88
    "Използване на AF", // 89
    "Избор на FM лента", // 99
    "Избор на AM лента", // 100
    "ДВ", // 101
    "СВ", // 102
    "КВ", // 103
    "ЧМ", // 104
    "ОИРТ", // 105
    "Спиране на събуждане\nна екрана", // 106
    "Избор на модел", // 107
    "База", // 108
    "Преносим", // 109
    "Преносимо докосване", // 110
    "Заглавие", // 111
    "Албум", // 112
    "Песен", // 113
    "Изпълнител", // 114
    "Композиция", // 115
    "Част", // 116
    "Диригент", // 117
    "Композитор", // 118
    "Група", // 119
    "Коментар", // 120
    "Жанр", // 121
    "Новини", // 122
    "Местни новини", // 123
    "Пазар", // 124
    "Спорт", // 125
    "Лотария", // 126
    "Хороскоп", // 127
    "Дневна програма", // 128
    "Здраве", // 129
    "Събитие", // 130
    "Сцена", // 131
    "Кино", // 132
    "ТВ", // 133
    "Дата/час", // 134
    "Време", // 135
    "Трафик", // 136
    "Аларма", // 137
    "Реклама", // 138
    "Уебсайт", // 139
    "Друго", // 140
    "Кратко PS", // 141
    "Дълго PS", // 142
    "Сега", // 143
    "Следващ", // 144
    "Част", // 145
    "Водещ", // 146
    "Редактор", // 147
    "Честота", // 148
    "Начална страница", // 149
    "Подканал", // 150
    "Гореща линия", // 151
    "Телефон на студио", // 152
    "Телефон", // 153
    "SMS на студио", // 154
    "SMS", // 155
    "Имейл гореща линия", // 156
    "Имейл на студио", // 157
    "Имейл", // 158
    "Чат", // 159
    "Чат център", // 160
    "Въпрос за гласуване", // 161
    "Център за гласуване", // 162
    "Място", // 163
    "Среща", // 164
    "Идентификатор", // 165
    "Покупка", // 166
    "Получаване на данни", // 167
    "Стъпка за AM", // 168
    "Мандарина", // 169
    "Океан", // 170
    "Индиго", // 171
    "Напрежение", // 172
    "Процент", // 173
    "Показване на процент", // 174
    "Буфериране на RT", // 175
    "Основни настройки", // 176
    "Аудио настройки", // 177
    "Настройки на дисплея", // 178
    "RDS настройки", // 179
    "FM настройки", // 180
    "AM настройки", // 181
    "Свързаност и часовник", // 182
    "НАТИСНЕТЕ MODE ЗА ВРЪЩАНЕ", // 183
    "Детектор на CoChannel", // 184
    "Чувствителност\nна CoChannel", // 185
    "Праг на CoChannel", // 186
    "Брояч на CoChannel", // 187
    "ОСНОВНИ", // 188
    "АУДИО", // 189
    "ДИСПЛЕЙ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "СВЪРЗАНЕ", // 194
    "DX РЕЖИМ", // 195
    "АВТОМАТИЧНА ПАМЕТ", // 196
    "AM АУР", // 197
    "FM деемфаза", // 198
    "Малък", // 199
    "Голям", // 200
    "Всички", // 201
    "Бърз PS", // 202
    "По подразбиране", // 203
    "Заглушаване на\nекран при XDRGTK", // 204
    "Подобряване на\nстерео FMSI", // 205
    "Функцията е достъпна\nсамо за TEF6687/6689!", // 206
    "Функцията не е\nдостъпна за облика!", // 207
    "Сканиране на\nканали в паметта", // 208
    "Спиране на\nсканиране на канали", // 209
    "Време за изчакване", // 210
    "Стартиране на DX сканер", // 211
    "Опции за FM DX", // 212
    "Началният канал\nе пропуснат.", // 213
    "Автоматично\nсортиране на AF", // 214
    "Канали в паметта", // 215
    "Изключване", // 216
    "Само", // 217
    "Автоматично прекратяване", // 218
    "Коригиране на PI", // 219
    "Сигнал", // 220
    "Заглушаване на\nзвука при сканиране", // 221
    "Въздушен", // 222
    "5 сек. натискане\nна band бутон", // 223
    "Изключване на\nзахранването", // 224
    "Изключване на екрана", // 225
    "Изключено", // 226
    "Няма", // 227
    "Новини", // 228
    "Актуални събития", // 229
    "Информация", // 230
    "Спорт", // 231
    "Образование", // 232
    "Драма", // 233
    "Култура", // 234
    "Наука", // 235
    "Разнообразие", // 236
    "Поп музика", // 237
    "Рок музика", // 238
    "Лека музика", // 239
    "Лека класика", // 240
    "Сериозна класика", // 241
    "Друга музика", // 242
    "Време", // 243
    "Финанси", // 244
    "Детски програми", // 245
    "Социални въпроси", // 246
    "Религия", // 247
    "Телефонен ефир", // 248
    "Пътувания", // 249
    "Свободно време", // 250
    "Джаз музика", // 251
    "Кънтри музика", // 252
    "Национална музика", // 253
    "Стари хитове", // 254
    "Фолк музика", // 255
    "Документален", // 256
    "Тест на аларма", // 257
    "Аларма!!!", // 258
    "Начален", // 259
    "Винаги", // 260
    "Показване на\nчасовника", // 261
    "Показване на\nдълго PS", // 262
    "Начална честота", // 263
    "Крайна честота", // 264
    "Начален канал\nв паметта", // 265
    "Краен канал в паметта", // 266
    "Запазване само с RDS", // 267
    "Стартиране на\nавтоматична памет", // 268
    "Автоматична памет", // 269
    "ID на списък с станции", // 270
    "Намерено", // 271
    "Няма свободни\nканали.", // 272
    "Потребителят\nнатисна бутон.", // 273
    "Сканирането завърши без грешки.", // 274
    "Изчистване на\nканали в паметта", // 275
    "Изчистено", // 276
    "Предотвратяване на\nдублиране на PI", // 277
    "Обхват", // 278
    "Пълен", // 279
    "Време за изчакване\nпри сигнал", // 280
    "Натиснете маркирания ъгъл", // 281
    "За калибриране\nна докосване", // 282
    "Задръжте BW бутона\nза да спрете сканера", // 283
    "Избор на честотна\nлента", // 284
    "Вашият дневник", // 285
    "Изтегляне на\nдневника", // 286
    "Дневникът е празен", // 287
    "Отидете в края", // 288
    "Записът е добавен\nв дневника", // 289
    "Грешка при добавяне в дневника", // 290
    "Изчистване на дневника", // 291
    "Дневникът е изчистен", // 292
    "Изчистването не\nбе успешно", // 293
    "Не може да се добави запис!\nДневникът е пълен!", // 294
    "Автоматичен дневник", // 295
    "Автоматично лятно\nвреме по NTP", // 296
    "Отместване на NTP", // 297
    "Дневникът съдържа над 130 записа, които не могат да бъдат обработени. Моля, изтеглете CSV файла за обработка.", // 298
    "Канада" // 299
  },

  { "Русский", // Russian
    "Изменено направление", // 1
    "Теперь отпустите кнопку", // 2
    "Экран перевёрнут", // 3
    "Калибровка S-метра", // 4
    "Режим USB", // 5
    "энкодер: оптический", // 6
    "энкодер: стандартный", // 7
    "Тюнер: !Нет!", // 8
    "Громкость", // 9
    "Сдвиг конвертера", // 10
    "Нижняя граница FM", // 11
    "Верхняя граница FM", // 12
    "Калибровка уровня", // 13
    "Порог открытия стерео", // 14
    "Срез ВЧ", // 15
    "Порог среза ВЧ", // 16
    "Порог качества сигнала", // 17
    "Яркость", // 18
    "Линейная громкость", // 19
    "Сдвиг конвертера", // 20
    "Нижняя граница FM", // 21
    "Верхняя граница FM", // 22
    "Калибровка уровня", // 23
    "Порог открытия стереодекодера", // 24
    "Срез ВЧ при слабом сигнале", // 25
    "Порог высокочастотного среза", // 26
    "Порог качества сигнала", // 27
    "Яркость дисплея", // 28
    "Выкл.", // 29
    "Вкл.", // 30
    "НАЖМИТЕ MODE ДЛЯ СОХРАНЕНИЯ", // 31
    "НАЖМИТЕ BAND ДЛЯ ЗАКРЫТИЯ", // 32
    "СКАНИРОВАНИЕ", // 33
    "Тюнер не обнаружен", // 34
    "Аттеньюатор AM-антенны", // 35
    "FM AGC", // 36
    "Уровень ошибок RDS", // 37
    "Язык меню", // 38
    "Язык меню: ", // 39
    "PI locked CT sync", // 40
    "Ошибка! Нет RDS!", // 41
    "FM Soft Mute", // 42
    "AM Soft Mute", // 43
    "Край диапазона (звук): ", // 44
    "Регион", // 45
    "Европа", // 46
    "USA", // 47
    "Показывать _ в RDS", // 48
    "Только PS", // 49
    "Wi-Fi активен", // 50
    "Настройка Wi-Fi", // 51
    "Подключиться к: ", // 52
    "для настройки Wi-Fi", // 53
    "Попытка подключения Wi-Fi", // 54
    "НЕ УДАЛОСЬ... Wi-Fi отключен", // 55
    "Подключено!", // 56
    "IP stationlist-клиента", // 57
    "Отображать длину волны", // 58
    "Фильтр RDS-данных", // 59
    "Ошибки PI-кода", // 60
    "Отображать шумодав", // 61
    "Уровень модуляции", // 62
    "AM Noise blanker", // 63
    "FM Noise blanker", // 64
    "Загружены настройки по умолчанию", // 65
    "Режим вывода аудио", // 66
    "При погашенном экране", // 67
    "Инвертировать дисплей", // 68
    "О программе", // 69
    "Основной код:", // 70
    "Соавторы:", // 71
    "Неизвестно", // 72
    "Действие по таймеру", // 73
    "Глубокий сон", // 74
    "Погасить экран", // 75
    "Тема", // 76
    "Вид (Skin)", // 77
    "Обнаруженные приложения", // 78
    "мин.", // 79
    "Частота шины дисплея SPI", // 80
    "Чувствительность сканирования", // 81
    "Не использовать", // 82
    "Версия прошивки -", // 83
    "Шрифт индикации частоты", // 84
    "Auto", // 85
    "Нет данных", // 86
    "Нет данных", // 87
    "Нет данных", // 88
    "Шаг FM по умолчанию", // 80
    "Таймер", // 81
    "сек", // 82
    "Режим часов", // 83
    "1% яркости", // 84
    "25% яркости", // 85
    "50% яркости", // 86
    "Калибровка уровня", // 87
    "Измерять сигнал в", // 88
    "Переключаться по AF", // 89
    "Активные диапазоны FM", // 99
    "Активные диапазоны AM", // 100
    "ДВ", // 101
    "СВ", // 102
    "КВ", // 103
    "FM", // 104
    "УКВ", // 105
    "Настройка при погашенном экране: ", // 106
    "Выбор модели", // 107
    "Стандартный", // 108
    "Портативный", // 109
    "Сенсорный", // 110
    "Название", // 111
    "Альбом", // 112
    "Трек", // 113
    "Исполнитель", // 114
    "Композиция", // 115
    "Движение", // 116
    "Дирижер", // 117
    "Композитор", // 118
    "Группа", // 119
    "Комментарий", // 120
    "Жанр", // 121
    "Новости", // 122
    "Местные новости", // 123
    "Фондовый рынок", // 124
    "Спорт", // 125
    "Лотерея", // 126
    "Гороскоп", // 127
    "Развлечения", // 128
    "Здоровье", // 129
    "Событие", // 130
    "Сцена", // 131
    "Кинотеатр", // 132
    "Телевидение", // 133
    "Дата/время", // 134
    "Погода", // 135
    "Движение", // 136
    "Будильник", // 137
    "Реклама", // 138
    "Веб-сайт", // 139
    "Другое", // 140
    "Краткое PS", // 141
    "Длинное PS", // 142
    "Сейчас", // 143
    "Следующий", // 144
    "Часть", // 145
    "Ведущий", // 146
    "Редактор", // 147
    "Частота", // 148
    "Домашняя страница", // 149
    "Субканал", // 150
    "Горячая линия", // 151
    "Телефон студии", // 152
    "Телефон", // 153
    "SMS студии", // 154
    "SMS", // 155
    "Email горячей линии", // 156
    "Email студии", // 157
    "Email", // 158
    "Чат", // 159
    "Чат-центр", // 160
    "Вопрос голосования", // 161
    "Голосовой центр", // 162
    "Место", // 163
    "Встреча", // 164
    "Идентификатор", // 165
    "Покупка", // 166
    "Получить данные", // 167
    "Шаг по умолчанию на СВ", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Индикатор батареи", // 172
    "Вольтаж", // 173
    "Процент заряда", // 174
    "Буфер RT", // 175
    "Основные настройки", // 176
    "Звуковые настройки", // 177
    "Настройки отображения", // 178
    "RDS", // 179
    "Настройки FM", // 180
    "Настройки AM", // 181
    "Настройки подключения", // 182
    "НАЖМИТЕ MODE ДЛЯ ВОЗВРАТА", // 183
    "CoChannelDet чувствительность", // 184
    "AM CoChannelDet", // 185
    "CoChannelDet количество", // 186
    "Порог AM CoChannelDet", // 187
    "ОСНОВНЫЕ", // 188
    "ЗВУКОВЫЕ", // 189
    "ДИСПЛЕЙ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "КОННЕКТ", // 194
    "DX MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM deemphasis", // 198
    "Мало", // 199
    "Больше", // 200
    "Все", // 201
    "Быстрый PS", // 202
    "По умолчанию", // 203
    "Гасить дисплей при\nработе с XDRGTK", // 204
    "FMSI стерео\nулучшение", // 205
    "Доступно только\nна TEF6687/6689", // 206
    "Функция недоступна\nна этом оформлении", // 207
    "Стартовый канал\nпамяти", // 208
    "Конечный канал\nпамяти", // 209
    "Время ожидания", // 210
    "Начать DX-сканирование", // 211
    "Настройки FM DX сканера", // 212
    "Прервано! Стартовый канал\nпомечен для пропуска.", // 213
    "Сортировка AF-частот", // 214
    "Каналы памяти", // 215
    "Кроме", // 216
    "Только", // 217
    "Автоматическая отмена\nсканирования", // 218
    "При корректном PI", // 219
    "При наличии сигнала", // 220
    "Отключать аудио\nпри сканировании", // 221
    "Эфир", // 222
    "5 сек. нажатие Band", // 223
    "Отключить тюнер", // 224
    "Отключить экран", // 225
    "Запрещено", // 226
    "Не определено", // 227
    "Новости", // 228
    "Аналитика", // 229
    "Информация", // 230
    "Спорт", // 231
    "Образование", // 232
    "Драма", // 233
    "Культура", // 234
    "Наука", // 235
    "Разное", // 236
    "Поп М", // 237
    "Рок М", // 238
    "Лёгкая музыка", // 239
    "Лёгкая классика", // 240
    "Классика", // 241
    "Другая музыка", // 242
    "Погода", // 243
    "Финансы", // 244
    "Детские прогр", // 245
    "Общество", // 246
    "Религия", // 247
    "Открытая линия", // 248
    "Туризм", // 249
    "Досуг", // 250
    "Джаз", // 251
    "Музыка кантри", // 252
    "Национальная М", // 253
    "Олдис", // 254
    "Народная музыка", // 255
    "Документалистика", // 256
    "Тревога тест", // 257
    "Тревога!", // 258
    "Начальный", // 259
    "Всегда", // 260
    "Показать часы", // 261
    "Показывать длинный PS", // 262
    "Начальная частота", // 263
    "Конечная частота", // 264
    "Начальный канал памяти", // 265
    "Конечный канал памяти", // 266
    "Сохранять только с RDS", // 267
    "Начать автозапоминание", // 268
    "Автозапоминание", // 269
    "Stationlist идентификатор", // 270
    "Найдено", // 271
    "Прервано! Нехватка памяти", // 272
    "Прервано! Нажата кнопка", // 273
    "Сканирование завершено без ошибок", // 274
    "Очистить каналы памяти", // 275
    "очищено", // 276
    "Предотвращать дубль PI", // 277
    "Диапазон", // 278
    "Полный", // 279
    "Время ожидания\nпри сигнале", // 280
    "Нажмите на подсвеченный угол", // 281
    "Калибровка тачскрина", // 282
    "Переключение инверсии экрана", // 283
    "Выбрать ширину полосы", // 284
    "Ваш лог-файл", // 285
    "Скачать лог-файл", // 286
    "Лог-файл пуст", // 287
    "Вниз", // 288
    "Добавлено в лог-файл", // 289
    "Обновление не удалось!", // 290
    "Очистить лог-файл", // 291
    "Лог-файл очищен", // 292
    "Очистить не удалось", // 293
    "Смещение времени NTP", // 294
    "Автологгер", // 295
    "Автоматическое летнее время по NTP", // 296
    "Смещение времени NTP", // 297
    "В логе более 130 записей, которые программа просмотра не может обработать. Загрузите файл CSV для его обработки.", // 298
    "Канада" // 299
  },

  { "Українська", // Ukranian
    "Змінено напрямок обертання", // 1
    "Відпустіть кнопку", // 2
    "Екран перевернуто", // 3
    "Налагодження аналогового індикатора", // 4
    "Режим USB", // 5
    "Енкодер переведено/nв оптичний режим", // 6
    "Енкодер переведено/nв стандартний режим", // 7
    "Тюнер: !Відсутній!", // 8
    "Гучність", // 9
    "Конвертер", // 10
    "Нижня межа діапазону", // 11
    "Верхня межа діапазону", // 12
    "Зсув РЧ рівня FM", // 13
    "Поріг стерео", // 14
    "Відсікання високих частот", // 15
    "Поріг високих частот", // 16
    "Поріг низьких частот", // 17
    "Яскравість", // 18
    "Підсилення гучності", // 19
    "Зсув перетворювача", // 20
    "Нижня межа діапазону", // 21
    "Верхня межа діапазону", // 22
    "Зміщення рівня", // 23
    "Поріг розділення стерео", // 24
    "Частота відсікання", // 25
    "Межа високих частот", // 26
    "Межа низьких частот", // 27
    "Яскравість", // 28
    "Вимк.", // 29
    "Увімк.", // 30
    "Тисни MODE, щоб зберегти й вийти", // 31
    "Тисни BAND, щоб закрити", // 32
    "Пошук...", // 33
    "Тюнер не виявлено", // 34
    "Атенюатор СХ-антени", // 35
    "FM АРП", // 36
    "Показувати/nпомилки RDS", // 37
    "Мова", // 38
    "Виберіть мову", // 39
    "PI заблок./nузгодження CT", // 40
    "Помилка! Відсутній/nсигнал RDS", // 41
    "Приглушення FM", // 42
    "Приглушення СХ", // 43
    "Сигнал на межі/nдіапазону", // 44
    "Регіон", // 45
    "Європа", // 46
    "США", // 47
    "Показувати/nпідкреслення в RDS", // 48
    "Лише в PS", // 49
    "Wi-Fi ввімкнено", // 50
    "Налаштувати Wi-Fi", // 51
    "Підключитися до:", // 52
    "для налаштування Wi-Fi", // 53
    "Підключення до Wi-Fi...", // 54
    "Невдача!", // 55
    "Підключено!", // 56
    "IP клієнта Stationlist", // 57
    "Показувати довжину/nхвилі КХ", // 58
    "Фільтр RDS", // 59
    "Показувати помилки PI", // 60
    "Використовувати затихання", // 61
    "Аудіометр", // 62
    "Подавлення шуму СХ", // 63
    "Подавлення шуму FM", // 64
    "Завантажені типові налаштування", // 65
    "Аудіовихід", // 66
    "Дозволити налаштування\nбез вимкнення екрану", // 67
    "Інвертувати кольори", // 68
    "Версія ПЗ", // 69
    "Основний код:", // 70
    "Учасники:", // 71
    "Невідомо", // 72
    "Налаштування заставки", // 73
    "Автовимкнення", // 74
    "Екран вимкнено", // 75
    "Тема", // 76
    "Обгортка", // 77
    "Виявлені програми", // 78
    "хв.", // 79
    "Динамічна\nшвидкість SPI", // 80
    "Чутливість сканування", // 81
    "НІЧОГО", // 82
    "Версія ПЗ", // 83
    "Шрифт частоти", // 84
    "Авто.", // 85
    "AF недоступний", // 86
    "EON недоступний", // 87
    "RT+ недоступний", // 88
    "Крок FM за\nзамовчуванням", // 80
    "Вимкнення екрану через", // 81
    "с", // 82
    "Режим годинника", // 83
    "Яскравість 1%", // 84
    "Яскравість 25%", // 85
    "Яскравість 50%", // 86
    "Зміщення РЧ рівня СХ", // 87
    "Одиниця вимірювання\nсигналу", // 88
    "Використовувати AF", // 89
    "Вибрати FM-діапазон", // 99
    "Вибрати СХ-діапазон", // 100
    "ДХ", // 101
    "СХ", // 102
    "КХ", // 103
    "FM", // 104
    "OIRT", // 105
    "Не вмик. екран\nпід час налаш.", // 106
    "Вибір моделі", // 107
    "Стандартна", // 108
    "Переносна", // 109
    "Сенсорна", // 110
    "Назва", // 111
    "Альбом", // 112
    "Трек", // 113
    "Виконавець", // 114
    "Композиція", // 115
    "Рух", // 116
    "Диригент", // 117
    "Композитор", // 118
    "Група", // 119
    "Коментар", // 120
    "Жанр", // 121
    "Новини", // 122
    "Місцеві новини", // 123
    "Фондовий ринок", // 124
    "Спорт", // 125
    "Лотерея", // 126
    "Гороскоп", // 127
    "Щоденні розваги", // 128
    "Здоров'я", // 129
    "Подія", // 130
    "Сцена", // 131
    "Кіно", // 132
    "ТБ", // 133
    "Дата/час", // 134
    "Погода", // 135
    "Трафік", // 136
    "Будильник", // 137
    "Реклама", // 138
    "Веб-сайт", // 139
    "Інше", // 140
    "Короткий PS", // 141
    "Довгий PS", // 142
    "Зараз", // 143
    "Далі", // 144
    "Частина", // 145
    "Ведучий", // 146
    "Редактор", // 147
    "Частота", // 148
    "Домашня сторінка", // 149
    "Субканал", // 150
    "Гаряча лінія", // 151
    "Телефон студії", // 152
    "Телефон", // 153
    "SMS студії", // 154
    "SMS", // 155
    "Електронна пошта\nгарячої лінії", // 156
    "Електронна пошта\nстудії", // 157
    "Електронна пошта", // 158
    "Чат", // 159
    "Чат-центр", // 160
    "Питання\nголосування", // 161
    "Центр\nголосування", // 162
    "Місце", // 163
    "Зустріч", // 164
    "Ідентифікатор", // 165
    "Покупка", // 166
    "Отримання даних", // 167
    "Крок СХ за\nзамовчуванням", // 168
    "Помаранчевий", // 169
    "Хвиля", // 170
    "Пурпуровий", // 171
    "Налаштування показу\nакумулятора", // 172
    "Напруга", // 173
    "Відсоток", // 174
    "Буферизація RT", // 175
    "Основні налаштування", // 176
    "Налаштування аудіо", // 177
    "Налаштування екрану", // 178
    "Налаштування RDS", // 179
    "Налаштування FM", // 180
    "Налаштування СХ", // 181
    "З'єднання та годинник", // 182
    "Тисни MODE, щоб повернутися", // 183
    "Детектор CoChannel", // 184
    "Чутливість CoChannel", // 185
    "Поріг CoChannel", // 186
    "Лічильник CoChannel", // 187
    "Загальне", // 188
    "Звук", // 189
    "Екран", // 190
    "RDS", // 191
    "FM", // 192
    "СХ", // 193
    "З'єднання", // 194
    "Режим DX", // 195
    "Автопам'ять", // 196
    "СХ АРП", // 197
    "Деемфаза FM", // 198
    "Мала", // 199
    "Велика", // 200
    "Все", // 201
    "Швидкий PS", // 202
    "За замовчуванням", // 203
    "Вимкнути екран під час\nпідключення XDRGTK", // 204
    "Покращення стерео FMSI", // 205
    "Функція доступна лише\nна TEF6687/6689!", // 206
    "Функція недоступна\nна даній темі!", // 207
    "Почати сканування\nканалів пам'яті", // 208
    "Зупинити сканування\nканалів пам'яті", // 209
    "Час очікування", // 210
    "Почати DX-пошук", // 211
    "Налаштування FM DX", // 212
    "Скасовано! Початковий канал\nпозначено як пропущений", // 213
    "Автоматичне\nсортування AF", // 214
    "Канали пам'яті", // 215
    "Крім", // 216
    "Лише", // 217
    "Автоматичне скасування\nсканування", // 218
    "Виправити PI", // 219
    "Сигнал", // 220
    "Вимкнути звук\nпід час сканування", // 221
    "Ефір", // 222
    "Дія, якщо тримати BAND 5 секунд", // 223
    "Вимкнення приймача", // 224
    "Вимкнення екрану", // 225
    "Відключено", // 226
    "Немає", // 227
    "Новини", // 228
    "Поточні події", // 229
    "Інформація", // 230
    "Спорт", // 231
    "Освіта", // 232
    "Драма", // 233
    "Культура", // 234
    "Наука", // 235
    "Різне", // 236
    "Попмузика", // 237
    "Рок-музика", // 238
    "Легка музика", // 239
    "Легка класика", // 240
    "Класика", // 241
    "Інша музика", // 242
    "Погода", // 243
    "Фінанси", // 244
    "Дитячі програми", // 245
    "Соціальні програми", // 246
    "Релігія", // 247
    "Телефонні вітання", // 248
    "Подорожі", // 249
    "Дозвілля", // 250
    "Джаз", // 251
    "Кантрі", // 252
    "Національна музика", // 253
    "Стара музика", // 254
    "Фольклор", // 255
    "Документалістика", // 256
    "Випробування тривоги", // 257
    "Тривога!!!", // 258
    "Початковий", // 259
    "Завжди", // 260
    "Показати годинник", // 261
    "Показати довгий PS", // 262
    "Початкова частота", // 263
    "Кінцева частота", // 264
    "Початковий канал пам'яті", // 265
    "Кінцевий канал пам'яті", // 266
    "Зберігати лише з RDS", // 267
    "Запуск автопам'яті", // 268
    "Автопам'ять", // 269
    "Stationlist ID", // 270
    "Знайдено", // 271
    "Скасовано! Немає\nканалів пам'яті", // 272
    "Скасовано! Користувач\nнатиснув кнопку", // 273
    "Сканування завершено\nбез помилок", // 274
    "Очистити канали\nпам'яті", // 275
    "очищено", // 276
    "Запобігти подвійному PI", // 277
    "Діапазон", // 278
    "Повний", // 279
    "Час очікування лише за сигналом", // 280
    "Натисніть виділений кут", // 281
    "Щоб налаштувати сенсор", // 282
    "Тримайте BAND, щоб зупинити пошук DX", // 283
    "Вибір пропускної здатності", // 284
    "Твій журнал", // 285
    "Завантажити журнал", // 286
    "Твій журнал є порожнім", // 287
    "Вниз", // 288
    "Додано до журналу", // 289
    "Помилка оновлення!", // 290
    "Очистити журнал", // 291
    "Очищено", // 292
    "Не вдалося очистити", // 293
    "Твій журнал повний!", // 294
    "Автореєстратор", // 295
    "Автоматичний літній\nчас за NTP", // 296
    "Встановити зміщення\nчасу NTP", // 297
    "Журнал містить понад 130 записів, які оглядач не може обробити. Будь ласка, завантажте файл CSV, щоб обробити його.", // 298
    "Канада" // 299
  },

  { "Italiano", // Italian
    "Senso rotaz. cambiato", // 1
    "Per favore rilascia il pulsante", // 2
    "Schermo capovolto", // 3
    "Calibra il misuratore analogico", // 4
    "Modalità USB", // 5
    "Codificatore impostato su ottico", // 6
    "Codificatore impostato su standard", // 7
    "Sintonizzatore: !Nessuno!", // 8
    "Volume", // 9
    "Convertitore", // 10
    "Limite banda bassa", // 11
    "Limite banda alta", // 12
    "Offset livello RF FM", // 13
    "Soglia stereo", // 14
    "Angolo di taglio alto", // 15
    "Soglia di taglio alto", // 16
    "Soglia di basso livello", // 17
    "Luminosità", // 18
    "Imposta volume", // 19
    "Offset convertitore", // 20
    "Limite banda bassa", // 21
    "Limite banda alta", // 22
    "Offset di livello", // 23
    "Soglia separazione stereo", // 24
    "Frequenza angolo di taglio alto", // 25
    "Soglia di taglio alto", // 26
    "Soglia di basso livello", // 27
    "Luminosità display", // 28
    "Off", // 29
    "On", // 30
    "PREMI MODE PER USCIRE E MEMORIZZARE", // 31
    "PREMI BAND PER CHIUDERE", // 32
    "SCANSIONE...", // 33
    "Sintonizzatore non rilevato", // 34
    "Guadagno antenna AM", // 35
    "AGC FM", // 36
    "Mostra errori RDS", // 37
    "Lingua", // 38
    "Scegli lingua", // 39
    "PI bloccato CT sync", // 40
    "Errore! Nessun segnale RDS", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Beeper al limite di banda", // 44
    "Regione", // 45
    "Europa", // 46
    "USA", // 47
    "Mostra sottolineato in RDS", // 48
    "Solo PS", // 49
    "Wi-Fi attivo", // 50
    "Configura Wi-Fi", // 51
    "Connetti a: ", // 52
    "per configurare Wi-Fi", // 53
    "Tentativo di connessione Wi-Fi", // 54
    "FALLITO!", // 55
    "CONNESSO!", // 56
    "IP client stationlist", // 57
    "Mostra lunghezza d'onda SW", // 58
    "Filtro RDS", // 59
    "Mostra errori PI", // 60
    "Usa squelch", // 61
    "Misuratore modulazione", // 62
    "Filtro anti-rumore AM", // 63
    "Filtro anti-rumore FM", // 64
    "Impostazioni predefinite caricate", // 65
    "Uscita audio", // 66
    "Consenti sintonia senza\ndisattivare screensaver", // 67
    "Inverti colori display", // 68
    "Informazioni software", // 69
    "Codice principale:", // 70
    "Contributori:", // 71
    "Sconosciuto", // 72
    "Opzioni screensaver", // 73
    "Spegnimento automatico", // 74
    "Schermo spento", // 75
    "Tema", // 76
    "Skin", // 77
    "Applicazioni rilevate", // 78
    "Min.", // 79
    "Velocità SPI", // 80
    "Sensibilità scansione", // 81
    "NESSUNO", // 82
    "Versione software", // 83
    "Carattere frequenza", // 84
    "Auto", // 85
    "Nessun AF disponibile", // 86
    "Nessun EON disponibile", // 87
    "Nessun RT+ disponibile", // 88
    "Passo FM predefinito", // 80
    "Screensaver", // 81
    "Sec", // 82
    "Modalità orologio", // 83
    "1% luminosità", // 84
    "25% luminosità", // 85
    "50% luminosità", // 86
    "Offset livello RF AM", // 87
    "Unità misuratore segnale", // 88
    "Usa AF", // 89
    "Seleziona banda FM", // 99
    "Seleziona banda AM", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Ferma il risveglio schermo su sintonia", // 106
    "Selettore modello", // 107
    "Base", // 108
    "Portatile", // 109
    "Portatile touch", // 110
    "Titolo", // 111
    "Album", // 112
    "Traccia", // 113
    "Artista", // 114
    "Composizione", // 115
    "Movimento", // 116
    "Direttore d'orchestra", // 117
    "Compositore", // 118
    "Gruppo", // 119
    "Commento", // 120
    "Genere", // 121
    "Notizie", // 122
    "Notizie locali", // 123
    "Mercato azionario", // 124
    "Sport", // 125
    "Lotteria", // 126
    "Oroscopo", // 127
    "Divertimento quotidiano", // 128
    "Salute", // 129
    "Evento", // 130
    "Scena", // 131
    "Cinema", // 132
    "TV", // 133
    "Data/ora", // 134
    "Meteo", // 135
    "Traffico", // 136
    "Allarme", // 137
    "Pubblicità", // 138
    "Sito web", // 139
    "Altro", // 140
    "PS breve", // 141
    "PS lungo", // 142
    "Ora", // 143
    "Prossimo", // 144
    "Parte", // 145
    "Host", // 146
    "Editor", // 147
    "Frequenza", // 148
    "Homepage", // 149
    "Subcanale", // 150
    "Linea diretta", // 151
    "Telefono studio", // 152
    "Telefono", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email hotline", // 156
    "Email studio", // 157
    "Email", // 158
    "Chat", // 159
    "Centro chat", // 160
    "Domanda voto", // 161
    "Centro voto", // 162
    "Luogo", // 163
    "Appuntamento", // 164
    "Identificatore", // 165
    "Acquisto", // 166
    "Ottenere dati", // 167
    "Passo MW predefinito", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Opzioni batteria", // 172
    "Mostra tensione", // 173
    "Mostra percentuale", // 174
    "Buffering RT", // 175
    "Impostazioni principali", // 176
    "Impostazioni audio", // 177
    "Impostazioni display", // 178
    "Impostazioni RDS", // 179
    "Impostazioni FM", // 180
    "Impostazioni AM", // 181
    "Connettività e orologio", // 182
    "PREMI MODE PER RITORNO", // 183
    "Rilevatore CoCanale", // 184
    "Sensibilità rilevatore isofrequenza", // 185
    "Soglia rilevatore isofrequenza", // 186
    "Contatore rilevatore CoCanale", // 187
    "PRINCIPALE", // 188
    "AUDIO", // 189
    "DISPLAY", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CONNETTI", // 194
    "DX MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM de-enfasi", // 198
    "Piccolo", // 199
    "Grande", // 200
    "Tutto", // 201
    "PS veloce", // 202
    "Predefinito", // 203
    "Disattiva schermo su\nconnessione XDRGTK", // 204
    "Miglioramento stereo FMSI", // 205
    "Funzione disponibile solo su TEF6687/6689!", // 206
    "Funzione non disponibile su skin selezionata!", // 207
    "Inizia scansione\ncanali di memoria", // 208
    "Ferma scansione\ncanali di memoria", // 209
    "Tempo di attesa", // 210
    "Inizia scansione DX", // 211
    "Opzioni FM DX", // 212
    "Abortito! Canale iniziale contrass. da saltare", // 213
    "Auto ordina AF", // 214
    "Canali di memoria", // 215
    "Escludi", // 216
    "Solo", // 217
    "Annulla scansione automatica", // 218
    "Correggi PI", // 219
    "Segnale", // 220
    "Disattiva audio\nmentre scansiona", // 221
    "AEREO", // 222
    "Premi per 5 sec. il pulsante di banda", // 223
    "Spegni", // 224
    "Schermo spento", // 225
    "Disabilitato", // 226
    "Nessuno", // 227
    "Notizie", // 228
    "Affari correnti", // 229
    "Informazioni", // 230
    "Sport", // 231
    "Educazione", // 232
    "Dramma", // 233
    "Cultura", // 234
    "Scienza", // 235
    "Varietà", // 236
    "Musica Pop", // 237
    "Musica Rock", // 238
    "Musica facile", // 239
    "Classica leggera", // 240
    "Classica seria", // 241
    "Altra musica", // 242
    "Meteo", // 243
    "Finanza", // 244
    "Programmi per bambini", // 245
    "Affari sociali", // 246
    "Religione", // 247
    "Telefonata", // 248
    "Viaggio", // 249
    "Tempo libero", // 250
    "Musica Jazz", // 251
    "Musica Country", // 252
    "Musica Nazionale", // 253
    "Musica Oldies", // 254
    "Musica Folk", // 255
    "Documentario", // 256
    "Test allarme", // 257
    "Allarme!!!", // 258
    "Iniziale", // 259
    "Sempre", // 260
    "Mostra orologio", // 261
    "Mostra PS lungo", // 262
    "Inizia frequenza", // 263
    "Ferma frequenza", // 264
    "Inizia canale memoria", // 265
    "Ferma canale memoria", // 266
    "Memorizza solo con RDS", // 267
    "Inizia auto memoria", // 268
    "Auto memoria", // 269
    "ID Stationlist", // 270
    "Trovato", // 271
    "Abortito! Canali di memoria esauriti", // 272
    "Abortito! L'utente ha premuto il pulsante", // 273
    "Scansione terminata senza errori", // 274
    "Cancella canali di memoria", // 275
    "Cancellato", // 276
    "Prevenire doppio PI", // 277
    "Range", // 278
    "Completo", // 279
    "Tempo di attesa su\nsolo segnale", // 280
    "Premi angolo evidenziato", // 281
    "Per calibrare touchscreen", // 282
    "Tieni premuto il tasto BW per\nfermare scanner DX", // 283
    "Seleziona larghezza di banda", // 284
    "Il tuo registro dei log", // 285
    "Scarica registro dei log", // 286
    "Registro dei log vuoto", // 287
    "Vai in fondo", // 288
    "Voce aggiunta\nal registro dei log", // 289
    "Errore nell'aggiunta\nvoce al registro dei log", // 290
    "Cancella registro", // 291
    "Registro cancellato", // 292
    "Cancellazione fallita", // 293
    "Impossibile aggiungere voce!\nRegistro dei log pieno!", // 294
    "Autologger", // 295
    "Auto DST su tempo NTP", // 296
    "Imposta offset orario NTP", // 297
    "Registro dei log con + di 130 voci, il visualizzatore non può gestirle. Scarica il file CSV per gestirlo.", // 298
    "Canada" // 299
  },

  { "Simplified Chinese", // Simplified Chinese
    "飞梭方向改变了", // 1
    "请释放按钮", // 2
    "屏幕翻转了", // 3
    "校准模拟S表", // 4
    "USB 连机模式选择", // 5
    "设置为光学编码器", // 6
    "设置为标准编码器", // 7
    "没有收音芯片！", // 8
    "音量", // 9
    "偏移量", // 10
    "FM 最低频率", // 11
    "FM 最高频率", // 12
    "FM信号偏移量", // 13
    "立体声阈值", // 14
    "高切角", // 15
    "高切门限", // 16
    "弱信号门限", // 17
    "屏幕亮度", // 18
    "设置音量", // 19
    "设置频率偏移量", // 20
    "设置 FM 最低频率", // 21
    "设置 FM 最高频率", // 22
    "设置信号值偏移量", // 23
    "设置立体声门限", // 24
    "设置高切角频率", // 25
    "设置高切门限", // 26
    "设置弱信号门限", // 27
    "屏幕亮度", // 28
    "关闭", // 29
    "打开", // 30
    "按 MODE 键退出并保存", // 31
    "时钟模式", // 32
    "搜索中...", // 33
    "未检测到收音芯片", // 34
    "AM 天线增益", // 35
    "FM AGC", // 36
    "显示 RDS 错误", // 37
    "语言", // 38
    "选择语言", // 39
    "从RDS同步时间", // 40
    "错误! 无RDS信号", // 41
    "FM 软静噪", // 42
    "AM 软静噪", // 43
    "频率边界提示音", // 44
    "国家与地区", // 45
    "欧洲", // 46
    "USA", // 47
    "RDS 中显示下划线", // 48
    "只有PS", // 49
    "打开 Wifi", // 50
    "Wi-Fi 配置", // 51
    "连接到：", // 52
    "配置 Wi-Fi", // 53
    "尝试连接 Wifi 到", // 54
    "失败.. 关闭 Wi-Fi", // 55
    "连接成功！", // 56
    "Stationlist 客户端 IP", // 57
    "显示短波米波段", // 58
    "RDS 过滤器", // 59
    "显示 PI 错误", // 60
    "使能静噪功能", // 61
    "主画面显示 M 表", // 62
    "AM 噪声消除", // 63
    "FM 噪声消除", // 64
    "恢复出厂设置", // 65
    "音频输出选择", // 66
    "盲操", // 67
    "屏幕颜色反演", // 68
    "关于", // 69
    "主程序", // 70
    "贡献者", // 71
    "未知", // 72
    "屏保选项", // 73
    "倒计时休眠", // 74
    "关闭屏幕", // 75
    "主题 & 颜色", // 76
    "布局", // 77
    "检测到的应用程序", // 78
    "分钟", // 79
    "调整 SPI 通讯速率", // 80
    "搜索灵敏度", // 81
    "不可用", // 82
    "固件版本", // 83
    "频率字体", // 84
    "自动", // 85
    "AF 不可用", // 86
    "EON 不可用", // 87
    "RT+ 不可用", // 88
    "FM 默认步进", // 80
    "屏保", // 81
    "秒", // 82
    "时钟制式", // 83
    "屏幕 1% 亮度", // 84
    "屏幕 25% 亮度", // 85
    "屏幕 50% 亮度", // 86
    "AM 高频信号偏移", // 87
    "信号单位", // 88
    "使能 AF", // 89
    "选择 FM 波段", // 99
    "选择 AM 波段", // 100
    "长波", // 101
    "中波", // 102
    "短波", // 103
    "调频", // 104
    "OIRT", // 105
    "熄屏操作飞梭", // 106
    "硬件型号选择", // 107
    "原版", // 108
    "便携版", // 109
    "便携电阻触摸版", // 110
    "Title", // 111
    "Album", // 112
    "Track", // 113
    "Artist", // 114
    "Composition", // 115
    "Movement", // 116
    "Conductor", // 117
    "Composer", // 118
    "Band", // 119
    "Comment", // 120
    "Genre", // 121
    "News", // 122
    "Local news", // 123
    "Stockmarket", // 124
    "Sport", // 125
    "Lottery", // 126
    "Horoscope", // 127
    "Daily Diversion", // 128
    "Health", // 129
    "Event", // 130
    "Scene", // 131
    "Cinema", // 132
    "TV", // 133
    "Date/time", // 134
    "Weather", // 135
    "Traffic", // 136
    "Alarm", // 137
    "Advertisement", // 138
    "Website", // 139
    "Other", // 140
    "Short PS", // 141
    "Long PS", // 142
    "Now", // 143
    "Next", // 144
    "Part", // 145
    "Host", // 146
    "Editor", // 147
    "Frequency", // 148
    "Homepage", // 149
    "Subchannel", // 150
    "Hotline", // 151
    "Studio phone", // 152
    "Phone", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email hotline", // 156
    "Email studio", // 157
    "Email", // 158
    "Chat", // 159
    "Chat centre", // 160
    "Vote question", // 161
    "Vote centre", // 162
    "Place", // 163
    "Appointment", // 164
    "Identifier", // 165
    "Purchase", // 166
    "获取数据", // 167
    "中波默认步进", // 168
    "橘红", // 169
    "海蓝", // 170
    "靛青", // 171
    "显示电池", // 172
    "电压值", // 173
    "电池百分比", // 174
    "RT 缓存", // 175
    "主要设置", // 176
    "音频设置", // 177
    "显示设置", // 178
    "RDS 设置", // 179
    "FM 设置", // 180
    "AM 设置", // 181
    "上位机 & 时钟", // 182
    "按 MODE 键返回", // 183
    "AM 载波跟踪", // 184
    "设置跟踪灵敏度", // 185
    "AM 载波跟踪层级", // 186
    "设置跟踪层级", // 187
    "主设置", // 188
    "音频设置", // 189
    "显示设置", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "连接&时钟", // 194
    "DX模式", // 195
    "扫描自动存台", // 196
    "AM AGC", // 197
    "FM去加重", // 198
    "小", // 199
    "大", // 200
    "全部", // 201
    "快速 PS", // 202
    "默认", // 203
    "XDRGTK 连结时冻结屏幕", // 204
    "FMSI 立体声加强", // 205
    "此功能仅限 TEF6687/6689!", // 206
    "此主题不支持该功能!", // 207
    "开始扫描存储信道", // 208
    "停止扫描存储信道", // 209
    "扫描等待时间", // 210
    "开始 DX 扫描", // 211
    "FM DX 选项", // 212
    "取消! 所选信道被跳过.", // 213
    "自动排列 AF", // 214
    "已存储信道", // 215
    "不扫描", // 216
    "只扫描", // 217
    "自动取消扫描", // 218
    "正确的 PI", // 219
    "信号", // 220
    "扫描时静音", // 221
    "航空", // 222
    "按下波段键 5秒", // 223
    "关机", // 224
    "熄屏", // 225
    "禁止", // 226
    "无节目", // 227
    "News", // 228
    "Current Affairs", // 229
    "Information", // 230
    "Sport", // 231
    "Education", // 232
    "Drama", // 233
    "Culture", // 234
    "Science", // 235
    "Varied", // 236
    "Pop Music", // 237
    "Rock Music", // 238
    "Easy Listening", // 239
    "Light Classical", // 240
    "Serious Classical", // 241
    "Other Music", // 242
    "Weather", // 243
    "Finance", // 244
    "Children's Progs", // 245
    "Social Affairs", // 246
    "Religion", // 247
    "Phone-In", // 248
    "Travel", // 249
    "Leisure", // 250
    "Jazz Music", // 251
    "Country Music", // 252
    "National Music", // 253
    "Oldies Music", // 254
    "Folk Music", // 255
    "Documentary", // 256
    "警报测试", // 257
    "警报!!!", // 258
    "仅初次", // 259
    "总是", // 260
    "显示时间", // 261
    "显示长的 PS", // 262
    "开始频率", // 263
    "停止频率", // 264
    "开始的记忆信道", // 265
    "停止的记忆信道", // 266
    "只存储RDS电台", // 267
    "开始搜索并存台", // 268
    "自动搜台并存储", // 269
    "电台列表ID", // 270
    "搜索到", // 271
    "取消! 存储已满", // 272
    "取消! 用户终止了操作", // 273
    "成功完成搜索", // 274
    "删除存储信道", // 275
    "删除了", // 276
    "防止双 PI", // 277
    "一定范围内", // 278
    "全部", // 279
    "无信号不停留", // 280
    "触摸区边沿高亮", // 281
    "校准触摸屏", // 282
    "按住BW按钮停止DX扫描仪", // 283
    "选择带宽", // 284
    "你的 log记录", // 285
    "下载 log记录", // 286
    "log为空", // 287
    "跳到底部", // 288
    "添加到log记录", // 289
    "更新失败!", // 290
    "清除log记录", // 291
    "log记录已清除", // 292
    "清除失败", // 293
    "log记录已满!", // 294
    "自动记录日志", // 295
    "联网自动同步本机时间", // 296
    "设置NTP时区", // 297
    "该日志包含超过130个条目，观看者无法处理。请下载CSV文件进行处理。", // 298
    "加拿大" // 299
  },

  { "Norsk", // Norwegian
    "Rotasjonsretning endret", // 1
    "Vennligst slipp knappen", // 2
    "Skjermen ble snudd", // 3
    "Kalibrer analog meter", // 4
    "USB modus", // 5
    "Encoder satt til optisk", // 6
    "Encoder satt til standard", // 7
    "Tuner: !Ingen!", // 8
    "Lydstyrke", // 9
    "Omformer", // 10
    "Lav båndgrense", // 11
    "Høy båndgrense", // 12
    "FM RF-nivåforskyvning", // 13
    "Stereo terskel", // 14
    "Høyeste grense", // 15
    "Høyeste grense", // 16
    "Laveste grense", // 17
    "Kontrast", // 18
    "Sett lyd", // 19
    "Omformeroffset", // 20
    "Lav båndgrense", // 21
    "Høy båndgrense", // 22
    "Nivåoffset", // 23
    "Stereo separasjons\nterskel", // 24
    "Høy grensefrekvens", // 25
    "Høy grenseterskel", // 26
    "Lav grenseterskel", // 27
    "Skjermlysstyrke", // 28
    "Av", // 29
    "På", // 30
    "TRYKK MODE FOR Å LAGRE OG AVSLUTTE", // 31
    "TRYKK BAND FOR Å LUKKE", // 32
    "SKANNER...", // 33
    "Tuner ikke oppdaget", // 34
    "AM-antenneforsterkning", // 35
    "FM AGC", // 36
    "Vis RDS-feil", // 37
    "Språk", // 38
    "Velg språk", // 39
    "PI-låst CT-synk", // 40
    "Feil! Ingen RDS-signal", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Pip ved båndkant", // 44
    "Region", // 45
    "Europa", // 46
    "USA", // 47
    "Vis understrek i RDS", // 48
    "Kun PS", // 49
    "Wi-Fi aktiv", // 50
    "Sett opp Wi-Fi", // 51
    "Koblet til: ", // 52
    "for å sette opp Wi-Fi", // 53
    "Prøver å koble til Wi-Fi", // 54
    "FEILET!", // 55
    "TILKOBLET!", // 56
    "Stationlist klient-IP", // 57
    "Vis SW-bånde", // 58
    "RDS-filter", // 59
    "Vis PI-feil", // 60
    "Bruk squelch", // 61
    "Modulasjonsmåler", // 62
    "AM-støydemper", // 63
    "FM-støydemper", // 64
    "Standarder lastet", // 65
    "Lydutgang", // 66
    "Tillat tuning uten å\ndeaktivere skjermsparer", // 67
    "Inverter skjermfarger", // 68
    "Om programvaren", // 69
    "Hovedkode:", // 70
    "Bidragsytere:", // 71
    "Ukjent", // 72
    "Skjermspareralternativer", // 73
    "Auto avslåing", // 74
    "Skjerm av", // 75
    "Tema", // 76
    "Skin", // 77
    "Oppdaget applikasjoner", // 78
    "Min.", // 79
    "SPI-hastighet", // 80
    "Skannefølsomhet", // 81
    "INGEN", // 82
    "Programvareversjon", // 83
    "Frekvensskrifttype", // 84
    "Auto", // 85
    "Ingen AF", // 86
    "Ingen EON", // 87
    "Ingen RT+", // 88
    "FM standard\ntrinnstørrelse", // 80
    "Skjermsparer", // 81
    "Sek.", // 82
    "Klokkemodus", // 83
    "1% lysstyrke", // 84
    "25% lysstyrke", // 85
    "50% lysstyrke", // 86
    "AM RF-nivå offset", // 87
    "Signalmeterenhet", // 88
    "Bruk AF", // 89
    "Velg FM-bånd", // 99
    "Velg AM-bånd", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Stopp skjermvekking ved tuning", // 106
    "Modellvelger", // 107
    "Base", // 108
    "Bærbar", // 109
    "Bærbar touch", // 110
    "Tittel", // 111
    "Album", // 112
    "Spor", // 113
    "Artist", // 114
    "Komposisjon", // 115
    "Bevegelse", // 116
    "Dirigent", // 117
    "Komponist", // 118
    "Band", // 119
    "Kommentar", // 120
    "Sjanger", // 121
    "Nyheter", // 122
    "Lokale nyheter", // 123
    "Aksjemarked", // 124
    "Sport", // 125
    "Lotteri", // 126
    "Horoskop", // 127
    "Daglig avledning", // 128
    "Helse", // 129
    "Begivenhet", // 130
    "Scene", // 131
    "Kino", // 132
    "TV", // 133
    "Dato/tid", // 134
    "Vær", // 135
    "Trafikk", // 136
    "Alarm", // 137
    "Annonse", // 138
    "Nettsted", // 139
    "Annet", // 140
    "Kort PS", // 141
    "Lang PS", // 142
    "Nå", // 143
    "Neste", // 144
    "Del", // 145
    "Vert", // 146
    "Redaktør", // 147
    "Frekvens", // 148
    "Hjemmeside", // 149
    "Underkanal", // 150
    "Hotline", // 151
    "Studio telefon", // 152
    "Telefon", // 153
    "SMS studio", // 154
    "SMS", // 155
    "E-post hotline", // 156
    "E-post studio", // 157
    "E-post", // 158
    "Chat", // 159
    "Chattesenter", // 160
    "Stem spørsmål", // 161
    "Stemmesenter", // 162
    "Sted", // 163
    "Avtale", // 164
    "Identifikator", // 165
    "Kjøp", // 166
    "Få data", // 167
    "MW standard\ntrinnstørrelse", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Batterialternativer", // 172
    "Vis spenning", // 173
    "Vis prosent", // 174
    "RT buffring", // 175
    "Hovedinnstillinger", // 176
    "Lydinnstillinger", // 177
    "Skjerminnstillinger", // 178
    "RDS innstillinger", // 179
    "FM innstillinger", // 180
    "AM innstillinger", // 181
    "Tilkobling og klokke", // 182
    "TRYKK MODE FOR Å RETURNERE", // 183
    "CoChannel-detektor", // 184
    "CoChannel-detektor\nfølsomhet", // 185
    "CoChannel-detektor\nterskel", // 186
    "CoChannel-detektor\nteller", // 187
    "HOVED", // 188
    "LYD", // 189
    "SKJERM", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "TILKOBLE", // 194
    "DX MODUS", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM deemphasis", // 198
    "Lite", // 199
    "Mye", // 200
    "Alt", // 201
    "Rask PS", // 202
    "Standard", // 203
    "Demp skjerm under\nXDRGTK-tilkobling", // 204
    "FMSI stereo forbedring", // 205
    "Funksjon kun tilgjengelig\npå TEF6687/6689!", // 206
    "Funksjon ikke tilgjengelig\npå valgt skin!", // 207
    "Start skanning av\nminnekanaler", // 208
    "Stopp skanning av\nminnekanaler", // 209
    "Ventetid", // 210
    "Start DX-skanning", // 211
    "FM DX-alternativer", // 212
    "Avbryt! Startkanalen er\nmerket som hoppet over", // 213
    "Auto-sorter AF", // 214
    "Minnekanaler", // 215
    "Ekskluder", // 216
    "Kun", // 217
    "Auto-avbryt skanning", // 218
    "Korriger PI", // 219
    "Signal", // 220
    "Demp lyd under\nskanning", // 221
    "AIR", // 222
    "Hold BAND-knappen i 5 sek", // 223
    "Slå av", // 224
    "Slå av skjerm", // 225
    "Deaktivert", // 226
    "Ingen", // 227
    "Nyheter", // 228
    "Aktualiteter", // 229
    "Informasjon", // 230
    "Sport", // 231
    "Utdanning", // 232
    "Drama", // 233
    "Kultur", // 234
    "Vitenskap", // 235
    "Underholdning", // 236
    "Popmusikk", // 237
    "Rockmusikk", // 238
    "Lett musikk", // 239
    "Lett klassisk", // 240
    "Klassisk musikk", // 241
    "Annen musikk", // 242
    "Værmelding", // 243
    "Økonomi", // 244
    "Barneprogrammer", // 245
    "Sosiale spørsmål", // 246
    "Religion", // 247
    "Ring inn", // 248
    "Reise", // 249
    "Fritid", // 250
    "Jazz", // 251
    "Countrymusikk", // 252
    "Nasjonal musikk", // 253
    "Gammelpop", // 254
    "Folkemusikk", // 255
    "Dokumentar", // 256
    "Alarmtest", // 257
    "ALARM!", // 258
    "Innledende", // 259
    "Alltid", // 260
    "Vis klokke", // 261
    "Vis lang PS", // 262
    "Startfrekvens", // 263
    "Stoppfrekvens", // 264
    "Start minnekanal", // 265
    "Stopp minnekanal", // 266
    "Lagre kun med RDS", // 267
    "Start auto-minne", // 268
    "Auto-minne", // 269
    "Stationlist-ID", // 270
    "Funnet", // 271
    "Avbrutt! Ingen minnekanaler", // 272
    "Avbrutt! Bruker trykket knapp", // 273
    "Skanning fullført uten feil", // 274
    "Slett minnekanaler", // 275
    "ryddet", // 276
    "Forhindre doble PI-er", // 277
    "Spekter", // 278
    "Fullt", // 279
    "Ventetid kun\nved signal", // 280
    "Trykk på uthevet hjørne", // 281
    "Kalibrer berøringsskjerm", // 282
    "Hold BW-knappen for å\nstoppe DX-skanning", // 283
    "Velg båndbredde", // 284
    "Din loggbok", // 285
    "Last ned loggbok", // 286
    "Loggbok er tom", // 287
    "Gå til bunn", // 288
    "Oppføring lagt til i loggen", // 289
    "Feil ved lagring i loggen", // 290
    "Tøm loggbok", // 291
    "Loggbok tømt", // 292
    "Tømming mislyktes", // 293
    "Kan ikke lagre!\nLoggbok er full!", // 294
    "Autologger", // 295
    "Auto sommertid med NTP", // 296
    "Angi NTP-tidsforskyvning", // 297
    "Loggbok har over 130 oppføringer.\nLast ned CSV for behandling.", // 298
    "Canada" // 299
  },

  { "Español", // Spanish
    "Dirección rotativa\ncambiada", // 1
    "Suelte el botón", // 2
    "Pantalla girada", // 3
    "Calibrar medidor\nanalógico", // 4
    "Modo USB", // 5
    "Codificador configurado\ncomo óptico", // 6
    "Codificador estándar", // 7 (Simplified)
    "Sintonizador: ¡Ninguno!", // 8
    "Volumen", // 9
    "Convertidor", // 10
    "Banda baja", // 11
    "Banda alta", // 12
    "Desplazamiento\nRF FM", // 13
    "Umbral estéreo", // 14
    "Frecuencia de\ncorte alto", // 15
    "Umbral de corte alto", // 16
    "Umbral de nivel bajo", // 17
    "Brillo", // 18
    "Ajustar volumen", // 19
    "Desplazamiento\ndel convertidor", // 20
    "Límite inferior de banda", // 21
    "Límite superior de banda", // 22
    "Desplazamiento de nivel", // 23
    "Umbral separación\nestéreo", // 24
    "Frecuencia de\ncorte alto", // 25
    "Umbral de corte alto", // 26
    "Umbral de nivel bajo", // 27
    "Brillo de pantalla", // 28
    "Apagado", // 29
    "Encendido", // 30
    "PRESIONE MODO PARA GUARDAR Y SALIR", // 31
    "PRESIONE BANDA PARA CERRAR", // 32
    "ESCANEANDO...", // 33
    "Sintonizador no detectado", // 34
    "Ganancia antena AM", // 35
    "AGC FM", // 36
    "Mostrar errores RDS", // 37
    "Idioma", // 38
    "Elegir idioma", // 39
    "Sincronización PI-CT", // 40
    "¡Error! Sin señal RDS", // 41
    "Silencio FM", // 42
    "Silencio AM", // 43
    "Tono en límite de banda", // 44
    "Región", // 45
    "Europa", // 46
    "Estados Unidos", // 47
    "Mostrar guion\nbajo en RDS", // 48
    "Solo PS", // 49
    "Wi-Fi activado", // 50
    "Configurar Wi-Fi", // 51
    "Conectar a: ", // 52
    "para configurar Wi-Fi", // 53
    "Conectando a Wi-Fi...", // 54
    "¡FALLÓ!", // 55
    "¡CONECTADO!", // 56
    "IP cliente\nStationlist", // 57 (Removed line break)
    "Mostrar longitud\nde onda SW", // 58
    "Filtro RDS", // 59
    "Mostrar errores PI", // 60
    "Usar squelch", // 61
    "Medidor de modulación", // 62
    "Supresor de\nruido AM", // 63
    "Supresor de\nruido FM", // 64
    "Valores predeterminados\ncargados", // 65
    "Salida de audio", // 66
    "Sintonizar sin\ndesactivar protector", // 67 (Matched line break)
    "Invertir colores\nde pantalla", // 68
    "Acerca del software", // 69
    "Código principal:", // 70
    "Colaboradores:", // 71
    "Desconocido", // 72
    "Opciones\nde protector", // 73
    "Apagado automático", // 74
    "Pantalla apagada", // 75
    "Tema", // 76
    "Skin", // 77
    "Aplicaciones detectadas", // 78
    "Mín.", // 79
    "Velocidad SPI", // 80
    "Sensibilidad\nde escaneo", // 81
    "NINGUNO", // 82
    "Versión del software", // 83
    "Fuente de frecuencia", // 84
    "Automático", // 85
    "AF no disponible", // 86
    "EON no disponible", // 87
    "RT+ no disponible", // 88
    "Paso predeterminado\nFM", // 80
    "Protector\nde pantalla", // 81
    "Seg.", // 82
    "Modo reloj", // 83
    "Brillo al 1%", // 84
    "Brillo al 25%", // 85
    "Brillo al 50%", // 86
    "Desplazamiento\nRF AM", // 87
    "Unidad de medidor", // 88
    "Usar AF", // 89
    "Seleccionar\nbanda FM", // 99
    "Seleccionar\nbanda AM", // 100
    "LW", // 101
    "AM", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "No reactivar pantalla\nal sintonizar", // 106
    "Selector de modelo", // 107
    "Base", // 108
    "Portátil", // 109
    "Portátil táctil", // 110
    "Título", // 111
    "Álbum", // 112
    "Pista", // 113
    "Artista", // 114
    "Composición", // 115
    "Movimiento", // 116
    "Director", // 117
    "Compositor", // 118
    "Banda", // 119
    "Comentario", // 120
    "Género", // 121
    "Noticias", // 122
    "Noticias locales", // 123
    "Mercado bursátil", // 124
    "Deportes", // 125
    "Lotería", // 126
    "Horóscopo", // 127
    "Entretenimiento diario", // 128
    "Salud", // 129
    "Evento", // 130
    "Escena", // 131
    "Cine", // 132
    "TV", // 133
    "Fecha/hora", // 134
    "Clima", // 135
    "Tráfico", // 136
    "Alarma", // 137
    "Publicidad", // 138
    "Sitio web", // 139
    "Otro", // 140
    "PS corto", // 141
    "PS largo", // 142
    "Ahora", // 143
    "Siguiente", // 144
    "Parte", // 145
    "Presentador", // 146
    "Editor", // 147
    "Frecuencia", // 148
    "Página principal", // 149
    "Subcanal", // 150
    "Línea directa", // 151
    "Teléfono de estudio", // 152
    "Teléfono", // 153
    "SMS de estudio", // 154
    "SMS", // 155
    "Correo de línea directa", // 156
    "Correo de estudio", // 157
    "Correo electrónico", // 158
    "Chat", // 159
    "Centro de chat", // 160
    "Pregunta de votación", // 161
    "Centro de votación", // 162
    "Lugar", // 163
    "Cita", // 164
    "Identificador", // 165
    "Compra", // 166
    "Obtener datos", // 167
    "Paso\npredeterminado AM", // 168
    "Mandarina", // 169
    "Océano", // 170
    "Índigo", // 171
    "Opciones de batería", // 172
    "Mostrar voltaje", // 173
    "Mostrar porcentaje", // 174
    "Buffer RT", // 175
    "Ajustes principales", // 176
    "Ajustes de audio", // 177
    "Ajustes de pantalla", // 178
    "Ajustes RDS", // 179
    "Ajustes FM", // 180
    "Ajustes AM", // 181
    "Conectividad y reloj", // 182
    "PRESIONE MODO PARA VOLVER", // 183
    "Detector Co-Canal", // 184
    "Sensibilidad\nCo-Canal", // 185
    "Umbral Co-Canal", // 186
    "Contador Co-Canal", // 187
    "PRINCIPAL", // 188
    "AUDIO", // 189
    "PANTALLA", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CONECTAR", // 194
    "MODO DX", // 195
    "MEM AUTOMÁTICA", // 196
    "AGC AM", // 197
    "Deénfasis FM", // 198
    "Pequeño", // 199
    "Grande", // 200
    "Todo", // 201
    "PS rápido", // 202
    "Predeterminado", // 203
    "Silenciar pantalla\nen conexión XDRGTK", // 204
    "Mejora estéreo FMSI", // 205
    "¡Función solo para\nTEF6687/6689!", // 206
    "¡Función no disponible\nen skin seleccionada!", // 207
    "Iniciar escaneo\nde memorias", // 208
    "Detener escaneo\nde memorias", // 209
    "Tiempo de espera", // 210
    "Iniciar escaneo DX", // 211
    "Opciones FM DX", // 212
    "¡Abortar! Canal inicial\nmarcado como omitido", // 213
    "Ordenar AF automático", // 214
    "Canales guardados", // 215
    "Excluir", // 216
    "Solo", // 217
    "Cancelaciòn automàtica", // 218
    "PI correcto", // 219
    "Señal", // 220
    "Silenciar audio\ndurante escaneo", // 221
    "AIR", // 222
    "Mantener BANDA 5 seg.", // 223
    "Apagar", // 224
    "Apagar pantalla", // 225
    "Desactivado", // 226
    "Ninguno", // 227
    "Noticias", // 228
    "Magazine", // 229
    "Información", // 230
    "Deportes", // 231
    "Educación", // 232
    "Drama", // 233
    "Cultura", // 234
    "Ciencia", // 235
    "Variedades", // 236
    "Música pop", // 237
    "Música rock", // 238
    "Música ligera", // 239
    "Clásica ligera", // 240
    "Clásica seria", // 241
    "Otros géneros", // 242
    "Meteorología", // 243
    "Finanzas", // 244
    "Infantiles", // 245
    "Sociedad", // 246
    "Religión", // 247
    "Llamadas en vivo", // 248
    "Viajes", // 249
    "Ocio", // 250
    "Jazz", // 251
    "Country", // 252
    "Música nacional", // 253
    "Música de Ayer", // 254
    "Folclórica", // 255
    "Documental", // 256
    "Prueba de alarma", // 257
    "¡ALARMA!", // 258
    "Inicial", // 259
    "Siempre", // 260
    "Mostrar reloj", // 261
    "Mostrar PS largo", // 262
    "Frecuencia inicial", // 263
    "Frecuencia final", // 264
    "Canal inicial", // 265
    "Canal final", // 266
    "Guardar solo\ncon RDS", // 267
    "Iniciar memoria\nautomática", // 268
    "Memoria automática", // 269
    "ID Stationlist", // 270
    "Encontrado", // 271
    "¡Error! Sin memorias", // 272
    "¡Cancelado! Usuario\ndetuvo", // 273
    "Escaneo exitoso", // 274
    "Borrar memorias", // 275
    "borrado", // 276
    "Evitar PI duplicado", // 277
    "Rango", // 278
    "Completo", // 279
    "Esperar solo\ncon señal", // 280
    "Toque la esquina\nresaltada", // 281
    "Calibrar pantalla\ntáctil", // 282
    "Mantenga BW para\ndetener DX", // 283
    "Seleccionar ancho\nde banda", // 284
    "Libro de registro", // 285
    "Descargar registro", // 286
    "Registro vacío", // 287
    "Ir al final", // 288
    "Entrada añadida", // 289
    "Error al añadir", // 290
    "Borrar registro", // 291
    "Registro borrado", // 292
    "Error al borrar", // 293
    "¡Registro lleno!\nNo se puede añadir", // 294
    "Autoregistro", // 295
    "DST automático (NTP)", // 296
    "Ajustar\ndesplazamiento NTP", // 297
    "El registro tiene +130 entradas.\nDescargue el CSV.", // 298
    "Canadá" // 299
  },

  { "Português", // Portuguese
    "Direção rotativa\nalterada", // 1
    "Solte o botão,\npor favor", // 2
    "Tela invertida", // 3
    "Calibrar o medidor\nanalógico", // 4
    "Modo USB", // 5
    "Codificador definido\ncomo óptico", // 6
    "Codificador definido\ncomo padrão", // 7
    "Sintonizador: !Nenhum!", // 8
    "Volume", // 9
    "Conversor", // 10
    "Borda da faixa baixa", // 11
    "Borda da faixa alta", // 12
    "Deslocamento do nível\nde RF FM", // 13
    "Limiar estéreo", // 14
    "Canto de corte alto", // 15
    "Limiar de corte alto", // 16
    "Limiar de nível baixo", // 17
    "Brilho", // 18
    "Definir volume", // 19
    "Definir deslocamento\ndo conversor", // 20
    "Definir borda\nda faixa baixa", // 21
    "Definir borda\nda faixa alta", // 22
    "Definir deslocamento\nde nível", // 23
    "Limiar de separação\nestéreo", // 24
    "Frequência do canto\nde corte alto", // 25
    "Definir limiar\nde corte alto", // 26
    "Definir limiar\nde nível baixo", // 27
    "Definir brilho\nda tela", // 28
    "Desligado", // 29
    "Ligado", // 30
    "PRESSIONE MODE PARA SAIR E SALVAR", // 31
    "PRESSIONE BAND PARA FECHAR", // 32
    "ANALISANDO...", // 33
    "Sintonizador não detectado", // 34
    "AM Ganho da antena", // 35
    "FM AGC", // 36
    "Mostrar erros RDS", // 37
    "Idioma", // 38
    "Escolha o idioma", // 39
    "PI bloqueado CT sync", // 40
    "Erro! Sem sinal RDS", // 41
    "Silenciamento suave FM", // 42
    "Silenciamento suave AM", // 43
    "Bipe na borda da faixa", // 44
    "Região", // 45
    "Europa", // 46
    "Estados Unidos", // 47
    "Mostrar sublinhado\nno RDS", // 48
    "PS apenas", // 49
    "Wi-Fi ativo", // 50
    "Configurar Wi-Fi", // 51
    "Conectar a: ", // 52
    "para configurar\no Wi-Fi", // 53
    "Tentando conectar\nao Wi-Fi", // 54
    "FALHA!", // 55
    "CONECTADO!", // 56
    "IP do cliente da lista\nde estações", // 57
    "Mostrar comprimento\nde onda SW", // 58
    "Filtro RDS", // 59
    "Mostrar erros PI", // 60
    "Usar squelch", // 61
    "Medidor de modulação", // 62
    "Supressor de ruído AM", // 63
    "Supressor de ruído FM", // 64
    "Padrões carregados", // 65
    "Saída de áudio", // 66
    "Permitir sintonização\nsem desativar proteção", // 67
    "Inverter a tela", // 68
    "Sobre o software", // 69
    "Código principal:", // 70
    "Contribuidores:", // 71
    "Desconhecido", // 72
    "Opções de proteção\nde tela", // 73
    "Desligamento automático", // 74
    "Desligar a tela", // 75
    "Tema", // 76
    "Skin", // 77
    "Aplicativos detectados", // 78
    "Min.", // 79
    "Velocidade de SPI", // 80
    "Sensibilidade\nda varredura", // 81
    "NENHUM", // 82
    "Versão do software", // 83
    "Fonte de frequência", // 84
    "Auto", // 85
    "AF não disponível", // 86
    "EON não disponível", // 87
    "RT+ não disponível", // 88
    "Passo padrão FM", // 80
    "Proteção de tela", // 81
    "Segundos", // 82
    "Modo de relógio", // 83
    "1% de brilho", // 84
    "25% de brilho", // 85
    "50% de brilho", // 86
    "Deslocamento de nível\nde RF AM", // 87
    "Unidade de medidor\nde sinal", // 88
    "Usar AF", // 89
    "Selecionar faixa FM", // 99
    "Selecionar faixa AM", // 100
    "LW", // 101
    "AM", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Parar tela de despertar\nao sintonizar", // 106
    "Seletor de modelo", // 107
    "Básico", // 108
    "Portátil", // 109
    "Toque portátil", // 110
    "Título", // 111
    "Álbum", // 112
    "Faixa", // 113
    "Artista", // 114
    "Composição", // 115
    "Movimento", // 116
    "Maestro", // 117
    "Compositor", // 118
    "Banda", // 119
    "Comentário", // 120
    "Gênero", // 121
    "Notícias", // 122
    "Notícias locais", // 123
    "Mercado de ações", // 124
    "Esportes", // 125
    "Loteria", // 126
    "Horóscopo", // 127
    "Diversão diária", // 128
    "Saúde", // 129
    "Evento", // 130
    "Cena", // 131
    "Cinema", // 132
    "TV", // 133
    "Data/hora", // 134
    "Clima", // 135
    "Tráfego", // 136
    "Alarme", // 137
    "Anúncio", // 138
    "Website", // 139
    "Outros", // 140
    "PS curto", // 141
    "PS longo", // 142
    "Agora", // 143
    "Próximo", // 144
    "Parte", // 145
    "Apresentador", // 146
    "Editor", // 147
    "Frequência", // 148
    "Página inicial", // 149
    "Subcanal", // 150
    "Hotline", // 151
    "Telefone do estúdio", // 152
    "Telefone", // 153
    "SMS do estúdio", // 154
    "SMS", // 155
    "E-mail da hotline", // 156
    "E-mail do estúdio", // 157
    "E-mail", // 158
    "Chat", // 159
    "Centro de chat", // 160
    "Pergunta de votação", // 161
    "Centro de votação", // 162
    "Local", // 163
    "Compromisso", // 164
    "Identificador", // 165
    "Compra", // 166
    "Obter dados", // 167
    "Tamanho padrão\nda etapa AM", // 168
    "Tangerina", // 169
    "Oceano", // 170
    "Índigo", // 171
    "Opções de bateria", // 172
    "Mostrar voltagem", // 173
    "Mostrar porcentagem", // 174
    "Buffering RT", // 175
    "Configurações principais", // 176
    "Configurações de áudio", // 177
    "Configurações de exibição", // 178
    "Configurações RDS", // 179
    "Configurações FM", // 180
    "Configurações AM", // 181
    "Conectividade e relógio", // 182
    "PRESSIONE MODO PARA VOLTAR", // 183
    "Detector CoChannel", // 184
    "Sensibilidade do\nCoChannel Det", // 185
    "Limiar do\nCoChannel Det", // 186
    "Contador do\nCoChannel Det", // 187
    "PRINCIPAL", // 188
    "ÁUDIO", // 189
    "EXIBIÇÃO", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "CONECTAR", // 194
    "DX MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "Desênfase FM", // 198
    "Pequeno", // 199
    "Grande", // 200
    "Tudo", // 201
    "PS rápido", // 202
    "Predefinição", // 203
    "Tela mudo na\nconexão XDRGTK", // 204
    "FMSI melhoria\ndo som estéreo", // 205
    "Função disponível apenas\nno TEF6687/6689!", // 206
    "Função não disponível\nno skin selecionado!", // 207
    "Comece a pesquisar\ncanais de memória", // 208
    "Pare de pesquisar\ncanais de memória", // 209
    "Tempo de espera", // 210
    "Comece a pesquisar DX", // 211
    "Opções FM DX", // 212
    "Abortar! O canal\ninicial é ignorado.", // 213
    "Classificar AF\nautomaticamente", // 214
    "Canais memorizados", // 215
    "Excluir", // 216
    "Apenas", // 217
    "Cancelar automatic.\na pesquisa", // 218
    "PI correto", // 219
    "Sinal", // 220
    "Mudo áudio\nenquanto procura", // 221
    "AIR", // 222
    "Pressione o botão\nbanda por 5 seg.", // 223
    "Desligar", // 224
    "Tela desligado", // 225
    "Inativo", // 226
    "Não definido", // 227
    "Notícias", // 228
    "Actualidade", // 229
    "Informação", // 230
    "Desporto", // 231
    "Educação", // 232
    "Rádio – Teatro", // 233
    "Cultura", // 234
    "Ciência", // 235
    "Variado", // 236
    "Música Pop", // 237
    "Música Rock", // 238
    "Música Ligeira", // 239
    "Clássica Ligeira", // 240
    "Música Clássica", // 241
    "Outras Músicas", // 242
    "Meteorologia", // 243
    "Economia", // 244
    "Progr. Infantil", // 245
    "Acontec. Social", // 246
    "Religião", // 247
    "Phone-In", // 248
    "Viagens", // 249
    "Lazer", // 250
    "Música de Jazz", // 251
    "Música Country", // 252
    "Música Nacional", // 253
    "Música Antiga", // 254
    "Música Folk", // 255
    "Documentário", // 256
    "Teste de Alarme", // 257
    "Alarme – Alarme !", // 258
    "Inicial", // 259
    "Sempre", // 260
    "Mostrar relógio", // 261
    "Mostrar PS longo", // 262
    "Iniciar frequência", // 263
    "Parar frequência", // 264
    "Iniciar canal\nde memória", // 265
    "Parar canal de memória", // 266
    "Somente armazenar\ncom RDS", // 267
    "Iniciar memória automática", // 268
    "Memória automática", // 269
    "ID da lista de estações", // 270
    "Encontrado", // 271
    "Abortar! Sem canais\nde memória disponíveis", // 272
    "Abortar! Usuário\npressionou o botão", // 273
    "Verificação concluída\nsem erros", // 274
    "Limpar canais\nde memória", // 275
    "limpo", // 276
    "Prevenir PI duplo", // 277
    "Intervalo", // 278
    "Completo", // 279
    "Espera tempo apenas\nquando sinal", // 280
    "Pressione o\ncanto inscrito", // 281
    "Para calibração\nda tela de toque", // 282
    "Segure BW para parar", // 283
    "Selecione largura de banda", // 284
    "Seu livro de registro", // 285
    "Baixar livro de registro", // 286
    "O livro de registro está vazio", // 287
    "Ir para o final", // 288
    "Adicionado ao registro", // 289
    "Falha na atualização!", // 290
    "Limpar livro\nde registro", // 291
    "Livro limpo", // 292
    "Falha ao limpar", // 293
    "Não é possível adicionar!\nO livro está cheio!", // 294
    "Registrador automático", // 295
    "DST automático\nno horário NTP", // 296
    "Definir deslocamento\nde tempo NTP", // 297
    "O diário de bordo contém mais de 130 entradas que o espectador não consegue processar. Descarregue o ficheiro CSV para processá-lo.", // 298
    "Canadá" // 299
  },

  { "Srpski", // Serbian
    "Smer rotacije promenjen", // 1
    "Pustite dugme", // 2
    "Ekran okrenut", // 3
    "Kalibracija analognog\nmerača", // 4
    "Mod USB", // 5
    "Enkoder postavljen\nna optički", // 6
    "Enkoder postavljen\nna standard", // 7
    "Tuner: !Nema!", // 8
    "Jačina zvuka", // 9
    "Konverter", // 10
    "Donja ivica opsega", // 11
    "Gornja ivica opsega", // 12
    "FM RF nivo\npomeranja", // 13
    "Stereo prag", // 14
    "Visokopropusni kut", // 15
    "Visokopropusni\nprag", // 16
    "Prag za nizak\nnivo", // 17
    "Osvetljenost", // 18
    "Postavite jačinu zvuka", // 19
    "Pomeranje konvertera", // 20
    "Donja ivica opsega", // 21
    "Gornja ivica opsega", // 22
    "Pomeranje nivoa", // 23
    "Stereo odvajanje prag", // 24
    "Frekvencija\nvisokog rezanja", // 25
    "Prag visokog rezanja", // 26
    "Prag niskog nivoa", // 27
    "Osvetljenost ekrana", // 28
    "Isključeno", // 29
    "Uključeno", // 30
    "PRITISNITE MODE ZA IZLAZ I SPAS", // 31
    "PRITISNITE BAND DA ZATVORITE", // 32
    "SKENIRANJE...", // 33
    "Tuner nije\ndetektovan", // 34
    "AM antena\npojačanje", // 35
    "FM AGC", // 36
    "Prikazivanje RDS\ngrešaka", // 37
    "Jezik", // 38
    "Izaberite jezik", // 39
    "PI zaključan\nCT sinhronizacija", // 40
    "Greška! Nema RDS signala!", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Pucketanje na\nivici opsega", // 44
    "Region", // 45
    "Evropa", // 46
    "S.A.D.", // 47
    "Prikazivanje\ndonje crte u RDS", // 48
    "PS samo", // 49
    "Wi-Fi aktivan", // 50
    "Konfiguriši Wi-Fi", // 51
    "Povežite se sa: ", // 52
    "da biste konfigurisali Wi-Fi", // 53
    "Pokušavam da se\npovežem sa Wi-Fi", // 54
    "NEUSPELO!", // 55
    "POVEZANO!", // 56
    "IP Stationlist klijent", // 57
    "Prikazivanje SW\ntalasne dužine", // 58
    "RDS filter", // 59
    "Prikazivanje PI\ngrešaka", // 60
    "Koristi šum", // 61
    "Merač modulisanja", // 62
    "AM Noise blanker", // 63
    "FM Noise blanker", // 64
    "Podrazumevana\npodešavanja učitana", // 65
    "Audio izlaz", // 66
    "Dozvoli podešavanje\nbez screensavera", // 67
    "Invertuj ekran", // 68
    "O softveru", // 69
    "Glavni kod:", // 70
    "Doprinosioci:", // 71
    "Nepoznato", // 72
    "Opcije screensavera", // 73
    "Automatsko\nisključivanje", // 74
    "Isključi ekran", // 75
    "Tema", // 76
    "Koža", // 77
    "Detektovane aplikacije", // 78
    "Min.", // 79
    "SPI brzina", // 80
    "Osetljivost\nskeniranja", // 81
    "NIJE DOSTUPNO", // 82
    "Verzija softvera", // 83
    "Font za\nfrekvenciju", // 84
    "Automatski", // 85
    "Nema AF dostupnog", // 86
    "Nema EON dostupnog", // 87
    "Nema RT+ dostupnog", // 88
    "FM podrazumevani\nkorak", // 80
    "Screensaver", // 81
    "Sek", // 82
    "Način rada sa\nsatom", // 83
    "1% osvetljenost", // 84
    "25% osvetljenost", // 85
    "50% osvetljenost", // 86
    "AM RF nivo\npomeranja", // 87
    "Jedinica\nsignalnog merača", // 88
    "Koristi AF", // 89
    "Izaberi FM\nopseg", // 99
    "Izaberi AM\nopseg", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Zaustavi buđenje ekrana", // 106
    "Selector modela", // 107
    "Baza", // 108
    "Portabilno", // 109
    "Portabilno dodirno", // 110
    "Naslov", // 111
    "Album", // 112
    "Pesma", // 113
    "Izvođač", // 114
    "Kompozicija", // 115
    "Pokret", // 116
    "Dirigent", // 117
    "Kompozitor", // 118
    "Grupa", // 119
    "Komentar", // 120
    "Žanr", // 121
    "Vesti", // 122
    "Lokalne vesti", // 123
    "Berza", // 124
    "Sport", // 125
    "Loterija", // 126
    "Horoskop", // 127
    "Dnevni odmor", // 128
    "Zdravlje", // 129
    "Događaj", // 130
    "Scena", // 131
    "Film", // 132
    "TV", // 133
    "Datum/vreme", // 134
    "Vreme", // 135
    "Saobraćaj", // 136
    "Alarm", // 137
    "Reklama", // 138
    "Web sajt", // 139
    "Ostalo", // 140
    "Kratak PS", // 141
    "Dug PS", // 142
    "Sada", // 143
    "Sledeće", // 144
    "Deo", // 145
    "Domaćin", // 146
    "Uređivač", // 147
    "Frekvencija", // 148
    "Početna stranica", // 149
    "Podkanal", // 150
    "Vruća linija", // 151
    "Studio telefon", // 152
    "Telefon", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Email vruća linija", // 156
    "Email studio", // 157
    "Email", // 158
    "Razgovor", // 159
    "Razgovorni centar", // 160
    "Pitanje za glasanje", // 161
    "Centar za glasanje", // 162
    "Mesto", // 163
    "Sastanak", // 164
    "Identifikator", // 165
    "Kupovina", // 166
    "Preuzmi podatke", // 167
    "MW podrazumevani\nkorak", // 168
    "Tangerine", // 169
    "Okean", // 170
    "Indigo", // 171
    "Opcije baterije", // 172
    "Prikazivanje napona", // 173
    "Prikazivanje procenta", // 174
    "RT keširanje", // 175
    "Glavna podešavanja", // 176
    "Audio podešavanja", // 177
    "Podešavanja ekrana", // 178
    "RDS podešavanja", // 179
    "FM podešavanja", // 180
    "AM podešavanja", // 181
    "Povezivanje i sat", // 182
    "PRITISNITE MODE DA SE VRATITE", // 183
    "CoChannel Detektor", // 184
    "CoChannel Detektor\nosetljivost", // 185
    "CoChannel Detektor\nprag", // 186
    "CoChannel Detektor\nbrojač", // 187
    "GLAVNO", // 188
    "AUDIO", // 189
    "EKRAN", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "POVEZIVANJE", // 194
    "DX MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM de-emfaza", // 198
    "Malo", // 199
    "Veliko", // 200
    "Svi", // 201
    "Brzi PS", // 202
    "Podrazumevano", // 203
    "Utišaj ekran na XDRGTK", // 204
    "FMSI stereo\npoboljšanje", // 205
    "Funkcija dostupna\nsamo na TEF6687/6689!", // 206
    "Funkcija nije dostupna na koži!", // 207
    "Počnite skeniranje kanala", // 208
    "Zaustavite skeniranje kanala", // 209
    "Vreme čekanja", // 210
    "Pokrenite DX\nskeniranje", // 211
    "FM DX Opcije", // 212
    "Abort! Početni kanal preskočen", // 213
    "Automatski sortiraj AF", // 214
    "Memorijski kanali", // 215
    "Isključi", // 216
    "Samo", // 217
    "Automatski otkaži skeniranje", // 218
    "Ispravi PI", // 219
    "Signal", // 220
    "Utišaj zvuk\nu toku skeniranja", // 221
    "ZRAK", // 222
    "5 sekundi pritiska na dugme", // 223
    "Isključivanje", // 224
    "Isključi ekran", // 225
    "Onemogućeno", // 226
    "Nema", // 227
    "Vesti", // 228
    "Aktuelnosti", // 229
    "Informacije", // 230
    "Sport", // 231
    "Obrazovanje", // 232
    "Drama", // 233
    "Kultura", // 234
    "Nauka", // 235
    "Raznovrsno", // 236
    "Pop muzika", // 237
    "Rock muzika", // 238
    "Lagana muzika", // 239
    "Laka klasična", // 240
    "Seriozna klasična", // 241
    "Ostala muzika", // 242
    "Vreme", // 243
    "Finansije", // 244
    "Dečiji programi", // 245
    "Socijalna pitanja", // 246
    "Religija", // 247
    "Telefon na liniji", // 248
    "Putovanja", // 249
    "Odmor", // 250
    "Jazz muzika", // 251
    "Country muzika", // 252
    "Nacionalna muzika", // 253
    "Stari hitovi", // 254
    "Folk muzika", // 255
    "Dokumentarni", // 256
    "Test alarma", // 257
    "Alarm!!!", // 258
    "Početni", // 259
    "Uvek", // 260
    "Prikazivanje\nsata", // 261
    "Prikazivanje\ndugog PS", // 262
    "Početna frekvencija", // 263
    "Krajnja frekvencija", // 264
    "Početni memorijski\nkanal", // 265
    "Krajnji memorijski\nkanal", // 266
    "Samo sa RDS", // 267
    "Pokreni automatsko\nmemorijsko pamćenje", // 268
    "Automatsko memorisanje", // 269
    "Stationlist ID", // 270
    "Pronađeno", // 271
    "Abort! Nedostaju kanali", // 272
    "Abort! Korisnik je\npritisnuo dugme", // 273
    "Skeniranje završeno\nbez grešaka", // 274
    "Očisti memorijske\nkanale", // 275
    "obrisano", // 276
    "Prevencija duplog PI", // 277
    "Opseg", // 278
    "Puno", // 279
    "Vreme čekanja na signal", // 280
    "Pritisnite označeni\nugao", // 281
    "Za kalibraciju\nekrana", // 282
    "Drži BW da staneš", // 283
    "Izaberi širinu\nopsega", // 284
    "Tvoj dnevnik", // 285
    "Preuzmi dnevnik", // 286
    "Dnevnik je prazan", // 287
    "Idi do dna", // 288
    "Dodano u dnevnik", // 289
    "Ažuriranje nije\nuspelo!", // 290
    "Očisti dnevnik", // 291
    "Dnevnik očišćen", // 292
    "Greška pri brisanju", // 293
    "Ne može se dodati!\nDnevnik je pun!", // 294
    "Automatski logger", // 295
    "Auto DST na\nNTP vremenu", // 296
    "Postavite NTP\nvremenski offset", // 297
    "Dnevnik sadrži više od 130 unosa, što viewer ne može obraditi. Preuzmite CSV fajl za obradu.", // 298
    "Canada" // 299
  },

  { "Suomi", // Finnish
    "Pyörimisohjauksen suunta\nmuutettu", // 1
    "Paina nappia vapauttaaksesi", // 2
    "Näyttö käännetty", // 3
    "Kalibroi analoginen mittari", // 4
    "USB-tila", // 5
    "Kooderi asetettu optiseksi", // 6
    "Kooderi asetettu\nstandardiin", // 7
    "Viritin: !Ei mitään!", // 8
    "Äänenvoimakkuus", // 9
    "Muunna", // 10
    "Matalan taajuuden\nraja", // 11
    "Korkean taajuuden\nraja", // 12
    "FM RF-tason siirto", // 13
    "Stereo kynnys", // 14
    "Ylileikkaus kulma", // 15
    "Ylileikkaus kynnys", // 16
    "Matalan tason\nkynnys", // 17
    "Kirkkaus", // 18
    "Aseta äänenvoimakkuus", // 19
    "Muunna siirto", // 20
    "Matalan taajuuden\nraja", // 21
    "Korkean taajuuden\nraja", // 22
    "Tason siirto", // 23
    "Stereoerotus kynnys", // 24
    "Ylileikkaus kulman taajuus", // 25
    "Ylileikkaus kynnys", // 26
    "Matalan tason\nkynnys", // 27
    "Näytön kirkkaus", // 28
    "Pois", // 29
    "Päällä", // 30
    "PAINA MODE POISTU JA TALLENNA", // 31
    "PAINA BAND SULKEAKSESI", // 32
    "SKANNATAAN...", // 33
    "Virittimen havaitseminen\nepäonnistui", // 34
    "AM-antenni vahvistus", // 35
    "FM AGC", // 36
    "Näytä RDS-virheet", // 37
    "Kieli", // 38
    "Valitse kieli", // 39
    "PI lukittu CT synkronointi", // 40
    "Virhe! Ei RDS-signaalia", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Ääni rajalla", // 44
    "Alue", // 45
    "Eurooppa", // 46
    "Yhdysvallat", // 47
    "Näytä alaviiva RDS:ssa", // 48
    "Vain PS", // 49
    "Wi-Fi aktiivinen", // 50
    "Määritä Wi-Fi", // 51
    "Yhdistä: ", // 52
    "Wi-Fi:n määrittämiseksi", // 53
    "Yritetään yhdistää Wi-Fi", // 54
    "EPÄONNISTUI!", // 55
    "YHDISTETTY!", // 56
    "Stationlist-asiakas IP", // 57
    "Näytä SW aallonpituus", // 58
    "RDS-suodatin", // 59
    "Näytä PI-virheet", // 60
    "Käytä squelchiä", // 61
    "Modulaatiomittari", // 62
    "AM Melupeitto", // 63
    "FM Melupeitto", // 64
    "Oletusarvot ladattu", // 65
    "Äänilähtö", // 66
    "Salli viritys ilman\nruutusaverin poistamista", // 67
    "Käännä näytön värit", // 68
    "Tietoja ohjelmistosta", // 69
    "Pääkoodi:", // 70
    "Avustajat:", // 71
    "Tuntematon", // 72
    "Näytönsäästäjäasetukset", // 73
    "Automaattinen virransammutus", // 74
    "Näyttö pois", // 75
    "Teema", // 76
    "Pinta", // 77
    "Havaitut sovellukset", // 78
    "Min.", // 79
    "SPI-nopeus", // 80
    "Skannausherkkyys", // 81
    "EI", // 82
    "Ohjelmistoversio", // 83
    "Taajuusfontti", // 84
    "Automaattinen", // 85
    "Ei AF saatavilla", // 86
    "Ei EON saatavilla", // 87
    "Ei RT+ saatavilla", // 88
    "FM oletusarvoiset askelvälit", // 80
    "Näytönsäästäjä", // 81
    "Sek", // 82
    "Kellotila", // 83
    "1% kirkkaus", // 84
    "25% kirkkaus", // 85
    "50% kirkkaus", // 86
    "AM RF-tason siirto", // 87
    "Signaalimittarin yksikkö", // 88
    "Käytä AF", // 89
    "Valitse FM-taajuusalue", // 99
    "Valitse AM-taajuusalue", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Pysäytä näytön herätys virityksessä", // 106
    "Mallin valitsin", // 107
    "Perus", // 108
    "Kannettava", // 109
    "Koskettimellinen kannettava", // 110
    "Nimi", // 111
    "Albumi", // 112
    "Kappale", // 113
    "Esittäjä", // 114
    "Sävellys", // 115
    "Liike", // 116
    "Kapellimestari", // 117
    "Säveltäjä", // 118
    "Bändi", // 119
    "Kommentti", // 120
    "Genre", // 121
    "Uutiset", // 122
    "Paikalliset uutiset", // 123
    "Osakemarkkinat", // 124
    "Urheilu", // 125
    "Arpajaiset", // 126
    "Horoskooppi", // 127
    "Päivittäinen viihde", // 128
    "Terveys", // 129
    "Tapahtuma", // 130
    "Kohde", // 131
    "Elokuvateatteri", // 132
    "TV", // 133
    "Päivämäärä/aika", // 134
    "Sää", // 135
    "Liikenne", // 136
    "Hälytys", // 137
    "Mainos", // 138
    "Verkkosivusto", // 139
    "Muu", // 140
    "Lyhyt PS", // 141
    "Pitkä PS", // 142
    "Nyt", // 143
    "Seuraava", // 144
    "Osa", // 145
    "Isäntä", // 146
    "Toimittaja", // 147
    "Taajuus", // 148
    "Etusivu", // 149
    "Alikanava", // 150
    "Puh. linja", // 151
    "Studion puhelin", // 152
    "Puhelin", // 153
    "SMS studio", // 154
    "SMS", // 155
    "Sähköposti puhelinlinja", // 156
    "Sähköposti studio", // 157
    "Sähköposti", // 158
    "Chat", // 159
    "Chattikeskus", // 160
    "Äänestyskysymys", // 161
    "Äänestyskeskus", // 162
    "Paikka", // 163
    "Tapaaminen", // 164
    "Tunniste", // 165
    "Osto", // 166
    "Hanki data", // 167
    "MW oletusarvoiset askelvälit", // 168
    "Mandariini", // 169
    "Meri", // 170
    "Indigo", // 171
    "Akkumääritykset", // 172
    "Näytä jännite", // 173
    "Näytä prosentti", // 174
    "RT pusku", // 175
    "Pään asetukset", // 176
    "Ääniasetukset", // 177
    "Näyttöasetukset", // 178
    "RDS-asetukset", // 179
    "FM-asetukset", // 180
    "AM-asetukset", // 181
    "Yhteydet ja kello", // 182
    "PAINA MODE PALATAKSESI", // 183
    "CoChannel-tunnistin", // 184
    "CoChannel-tunnistin\nherkkyys", // 185
    "CoChannel-tunnistin\nkynnys", // 186
    "CoChannel-tunnistin\nlaskuri", // 187
    "PÄÄ", // 188
    "ÄÄNI", // 189
    "NÄYTTÖ", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "YHTEYS", // 194
    "DX-tila", // 195
    "AUTOMAATTINEN MUISTI", // 196
    "AM AGC", // 197
    "FM vähennys", // 198
    "Pieni", // 199
    "Suuri", // 200
    "Kaikki", // 201
    "Nopea PS", // 202
    "Oletus", // 203
    "Hiljennä näyttö\nXDRGTK-yhteyden aikana", // 204
    "FMSI stereo\nparannus", // 205
    "Toiminto saatavilla vain\nTEF6687/6689:ssä!", // 206
    "Toiminto ei saatavilla\nvalitussa pinnassa!", // 207
    "Aloita skannaus\nmuistikanavilla", // 208
    "Pysäytä skannaus\nmuistikanavilla", // 209
    "Odota-aika", // 210
    "Aloita DX-skannaus", // 211
    "FM DX Asetukset", // 212
    "Keskeytä! Aloituskanava\non merkitty ohitettavaksi.", // 213
    "Automaattinen AF lajittelu", // 214
    "Muistikanavat", // 215
    "Sulje", // 216
    "Vain", // 217
    "Automaattinen skannauksen peruutus", // 218
    "Korjaa PI", // 219
    "Signaali", // 220
    "Hiljennä ääni\nskannauksen aikana", // 221
    "ILMA", // 222
    "5 sekunnin bandinäppäimen\npainallus", // 223
    "Virta pois", // 224
    "Näyttö pois", // 225
    "Poistettu", // 226
    "Ei mitään", // 227
    "Uutiset", // 228
    "Ajankohtaiset", // 229
    "Tietoa", // 230
    "Urheilu", // 231
    "Koulutus", // 232
    "Draama", // 233
    "Kulttuuri", // 234
    "Tiede", // 235
    "Vaihteleva", // 236
    "Pop-musiikki", // 237
    "Rock-musiikki", // 238
    "Helppokuuntelu", // 239
    "Kevyt klassinen", // 240
    "Vakava klassinen", // 241
    "Muu musiikki", // 242
    "Sää", // 243
    "Talous", // 244
    "Lasten ohjelmat", // 245
    "Sosiaaliasiat", // 246
    "Uskonto", // 247
    "Puhelinlinja", // 248
    "Matkustus", // 249
    "Vapaa-aika", // 250
    "Jazz-musiikki", // 251
    "Country-musiikki", // 252
    "Kansallismusiikki", // 253
    "Oldies-musiikki", // 254
    "Folk-musiikki", // 255
    "Dokumentti", // 256
    "Hälytyksen testi", // 257
    "Hälytys!!!", // 258
    "Alkuperäinen", // 259
    "Aina", // 260
    "Näytä kello", // 261
    "Näytä pitkä PS", // 262
    "Aloita taajuus", // 263
    "Pysäytä taajuus", // 264
    "Aloita muistikanava", // 265
    "Pysäytä muistikanava", // 266
    "Tallenna vain RDS:n\nkanssa", // 267
    "Aloita\nautomaattinen muistio", // 268
    "Automaattinen muisti", // 269
    "Stationlist ID", // 270
    "Löytyi", // 271
    "Keskeytä! Muistikanavat loppu", // 272
    "Keskeytä! Käyttäjä painoi nappia", // 273
    "Skannaus valmis ilman virheitä", // 274
    "Tyhjennä\nmuistikanavat", // 275
    "tyhjennetty", // 276
    "Estä kaksois-PI", // 277
    "Alue", // 278
    "Täysi", // 279
    "Odota-aika\nvain signaalille", // 280
    "Paina korostettua kulmaa", // 281
    "Kalibroi kosketusnäyttö", // 282
    "Pidä BW-nappia painettuna\npysäyttääksesi DX-skannerin", // 283
    "Valitse\nkaistanleveys", // 284
    "Oma lokikirja", // 285
    "Lataa lokikirja", // 286
    "Lokikirja on tyhjä", // 287
    "Mene alas", // 288
    "Merkintä lisätty\nlokikirjaan", // 289
    "Virhe lisättäessä\nmerkintää lokikirjaan", // 290
    "Tyhjennä lokikirja", // 291
    "Lokikirja\ntyhjennetty", // 292
    "Tyhjennys\nepäonnistui", // 293
    "Ei voida lisätä merkintää!\nLokikirja on täynnä!", // 294
    "Autolokikirja", // 295
    "Auto DST\nNTP-ajassa", // 296
    "Aseta\nNTP-aikasiirto", // 297
    "Lokikirja sisältää yli 130 merkintää, joita katselija ei pysty käsittelemään. Lataa CSV-tiedosto sen käsittelemiseksi.", // 298
    "Kanada" // 299
  },

  { "Dansk", // Danish
    "Rotationsretning ændret", // 1
    "Slip knappen", // 2
    "Skærm vendt", // 3
    "Kalibrer analog måler", // 4
    "USB-tilstand", // 5
    "Encoder indstillet til optisk", // 6
    "Encoder indstillet til standard", // 7
    "Tuner: !Ingen!", // 8
    "Volumen", // 9
    "Konverter", // 10
    "Lavbåndskant", // 11
    "Højbåndskant", // 12
    "FM RF-niveau offset", // 13
    "Stereotærskel", // 14
    "Højfrekvensskæring\nhjørne", // 15
    "Højfrekvensskæring\ntærskel", // 16
    "Lavt niveau tærskel", // 17
    "Lysstyrke", // 18
    "Indstil volumen", // 19
    "Konverter offset", // 20
    "Lavbåndskant", // 21
    "Højbåndskant", // 22
    "Niveau offset", // 23
    "Stereoadskillelsestærskel", // 24
    "Højfrekvensskæring\nfrekvens", // 25
    "Højfrekvensskæring\ntærskel", // 26
    "Lavt niveau tærskel", // 27
    "Visningens lysstyrke", // 28
    "Slukket", // 29
    "Tændt", // 30
    "TRYK MODE FOR AT GEMME", // 31
    "TRYK PÅ BÅND FOR AT LUKKE", // 32
    "SCANNER...", // 33
    "Tuner ikke registreret", // 34
    "AM-antenneforstærkning", // 35
    "FM AGC", // 36
    "Vis RDS-fejl", // 37
    "Sprog", // 38
    "Vælg sprog", // 39
    "PI-låst CT-synkronisering", // 40
    "Fejl! Ingen RDS-signal", // 41
    "Blød dæmpning FM", // 42
    "Blød dæmpning AM", // 43
    "Beep ved båndkant", // 44
    "Region", // 45
    "Europa", // 46
    "USA", // 47
    "Vis understregning\ni RDS", // 48
    "PS kun", // 49
    "Wi-Fi aktiv", // 50
    "Konfigurer Wi-Fi", // 51
    "Forbind til: ", // 52
    "for at konfigurere Wi-Fi", // 53
    "Forsøger at forbinde Wi-Fi", // 54
    "FEJLET!", // 55
    "TILSLUTTET!", // 56
    "Stationliste\nklient IP", // 57
    "Vis SW-bølgelængde", // 58
    "RDS-filter", // 59
    "Vis PI-fejl", // 60
    "Brug squelch", // 61
    "Modulationsmåler", // 62
    "AM-støjfjerner", // 63
    "FM-støjfjerner", // 64
    "Standardindstillinger\nindlæst", // 65
    "Lydudgang", // 66
    "Tillad tuning uden\nat slå pauseskærm fra", // 67
    "Inverter visning", // 68
    "Om software", // 69
    "Hovedkode:", // 70
    "Bidragydere:", // 71
    "Ukendt", // 72
    "Pauseskærmindstillinger", // 73
    "Auto sluk", // 74
    "Skærm slukket", // 75
    "Tema", // 76
    "Hud", // 77
    "Registrerede\napplikationer", // 78
    "Min.", // 79
    "SPI hastighed", // 80
    "Scan-følsomhed", // 81
    "INGEN", // 82
    "Softwareversion", // 83
    "Frekvensskrifttype", // 84
    "Auto", // 85
    "Ingen AF tilgængelig", // 86
    "Ingen EON tilgængelig", // 87
    "Ingen RT+ tilgængelig", // 88
    "FM-standardtrin", // 80
    "Pauseskærm", // 81
    "Sek", // 82
    "Urtilstand", // 83
    "1% lysstyrke", // 84
    "25% lysstyrke", // 85
    "50% lysstyrke", // 86
    "AM RF-niveau offset", // 87
    "Signalmålerenhed", // 88
    "Brug AF", // 89
    "Vælg FM-bånd", // 99
    "Vælg AM-bånd", // 100
    "LW", // 101
    "MW", // 102
    "SW", // 103
    "FM", // 104
    "OIRT", // 105
    "Stop skærmvækning\nved tuning", // 106
    "Modelvælger", // 107
    "Basis", // 108
    "Bærbar", // 109
    "Bærbar touch", // 110
    "Titel", // 111
    "Album", // 112
    "Nummer", // 113
    "Kunstner", // 114
    "Komposition", // 115
    "Bevægelse", // 116
    "Dirigent", // 117
    "Komponist", // 118
    "Bånd", // 119
    "Kommentar", // 120
    "Genre", // 121
    "Nyheder", // 122
    "Lokale nyheder", // 123
    "Aktiemarked", // 124
    "Sport", // 125
    "Lotto", // 126
    "Horoskop", // 127
    "Daglig adspredelse", // 128
    "Sundhed", // 129
    "Begivenhed", // 130
    "Scene", // 131
    "Biograf", // 132
    "TV", // 133
    "Dato/tid", // 134
    "Vejr", // 135
    "Trafik", // 136
    "Alarm", // 137
    "Annonce", // 138
    "Hjemmeside", // 139
    "Andet", // 140
    "Kort PS", // 141
    "Længere PS", // 142
    "Nu", // 143
    "Næste", // 144
    "Del", // 145
    "Vært", // 146
    "Redaktør", // 147
    "Frekvens", // 148
    "Hjemmeside", // 149
    "Underkanal", // 150
    "Hotline", // 151
    "Studiotelefon", // 152
    "Telefon", // 153
    "SMS studie", // 154
    "SMS", // 155
    "E-mail hotline", // 156
    "E-mail studie", // 157
    "E-mail", // 158
    "Chat", // 159
    "Chatcenter", // 160
    "Afstemning spørgsmål", // 161
    "Afstemning center", // 162
    "Sted", // 163
    "Aftale", // 164
    "Identifikator", // 165
    "Køb", // 166
    "Hent data", // 167
    "MW-standardtrin", // 168
    "Tangerine", // 169
    "Hav", // 170
    "Indigo", // 171
    "Batteriindstillinger", // 172
    "Vis spænding", // 173
    "Vis procent", // 174
    "RT-buffering", // 175
    "Hovedindstillinger", // 176
    "Lydindstillinger", // 177
    "Skærmindstillinger", // 178
    "RDS-indstillinger", // 179
    "FM-indstillinger", // 180
    "AM-indstillinger", // 181
    "Tilslutning & ur", // 182
    "TRYK MODE FOR AT VENDE TILBAGE", // 183
    "Sammenkanal-detektor", // 184
    "Sammenkanal-detektor\nfølsomhed", // 185
    "Sammenkanal-detektor\ntærskel", // 186
    "Sammenkanal-detektor\ntæller", // 187
    "HOVED", // 188
    "LYD", // 189
    "SKÆRM", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "TILSLUT", // 194
    "DX-MODE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM-deemphasis", // 198
    "Lille", // 199
    "Stor", // 200
    "Alle", // 201
    "Hurtig PS", // 202
    "Standard", // 203
    "Sluk skærm ved\nXDRGTK-forbindelse", // 204
    "FMSI stereo\nforbedring", // 205
    "Funktion kun tilgængelig\npå TEF6687/6689!", // 206
    "Funktion ikke tilgængelig\npå valgt hud!", // 207
    "Begynd scanning\naf hukommelseskanaler", // 208
    "Stop scanning\naf hukommelseskanaler", // 209
    "Ventetid", // 210
    "Start DX-scanning", // 211
    "FM DX-indstillinger", // 212
    "Afbryd! Startkanal\ner sprunget over.", // 213
    "Auto-sortering AF", // 214
    "Hukommelseskanaler", // 215
    "Udeluk", // 216
    "Kun", // 217
    "Auto-annuller scan", // 218
    "Korrekt PI", // 219
    "Signal", // 220
    "Lyd dæmpet\nunder scanning", // 221
    "LUFT", // 222
    "5 sek. båndknap-tryk", // 223
    "Sluk", // 224
    "Skærm slukket", // 225
    "Deaktiveret", // 226
    "Ingen", // 227
    "Nyheder", // 228
    "Seneste nyt", // 229
    "Oplysninger", // 230
    "Sport", // 231
    "Uddannelse", // 232
    "Drama", // 233
    "Kultur", // 234
    "Videnskab", // 235
    "Variabel", // 236
    "Popmusik", // 237
    "Rockmusik", // 238
    "Nem lytning", // 239
    "Lettelse klassisk", // 240
    "Seriøs klassisk", // 241
    "Anden musik", // 242
    "Vejr", // 243
    "Finans", // 244
    "Børneprogrammer", // 245
    "Sociale anliggender", // 246
    "Religion", // 247
    "Ring ind", // 248
    "Rejse", // 249
    "Fritid", // 250
    "Jazzmusik", // 251
    "Countrymusik", // 252
    "National musik", // 253
    "Gamle hits", // 254
    "Folkemusik", // 255
    "Dokumentar", // 256
    "Alarmtest", // 257
    "Alarm!!!", // 258
    "Initial", // 259
    "Altid", // 260
    "Vis ur", // 261
    "Vis lang PS", // 262
    "Startfrekvens", // 263
    "Stopfrekvens", // 264
    "Start hukommelseskanal", // 265
    "Stop hukommelseskanal", // 266
    "Kun gem med RDS", // 267
    "Start auto-hukommelse", // 268
    "Auto-hukommelse", // 269
    "Stationsliste-ID", // 270
    "Fundet", // 271
    "Afbryd! Uden\nhukommelseskanaler", // 272
    "Afbryd! Brugeren\ntrykkede på knap", // 273
    "Scanning afsluttet uden fejl", // 274
    "Ryd\nhukommelseskanaler", // 275
    "ryddet", // 276
    "Forhindre\ndobbelt PI", // 277
    "Rækkevidde", // 278
    "Fuld", // 279
    "Ventetid på\nkun signal", // 280
    "Tryk på det markerede hjørne", // 281
    "For at kalibrere touchskærmen", // 282
    "Hold BW for at stoppe", // 283
    "Vælg båndbredde", // 284
    "Din logbog", // 285
    "Download logbog", // 286
    "Logbog er tom", // 287
    "Gå til bunden", // 288
    "Tilføjet til log", // 289
    "Opdatering\nmislykkedes!", // 290
    "Ryd logbog", // 291
    "Logbog ryddet", // 292
    "Rydning mislykkedes", // 293
    "Kan ikke tilføje!\nLogbogen er fuld!", // 294
    "Autologger", // 295
    "Auto DST på\nNTP-tid", // 296
    "Indstil NTP-tidsforskel", // 297
    "Logbogen indeholder over 130 poster, som fremviseren ikke kan behandle. Download CSV-filen for at behandle den.", // 298
    "Canada" // 299
  },

  { "Svenska", // Swedish
    "Rotationsriktning ändrad", // 1
    "Var god släpp knappen", // 2
    "Skärmen omvänd", // 3
    "Kalibrera analog mätare", // 4
    "USB-läge", // 5
    "Rotationspulsgivare\nsatt till optisk", // 6
    "Rotationspulsgivare\nsatt till standard", // 7
    "Tuner: !Ingen!", // 8
    "Ljudstyrka", // 9
    "Konverter", // 10
    "Låg bandgräns", // 11
    "Hög bandgräns", // 12
    "FM RF-nivåoffset", // 13
    "Stereoseparations-\ntröskel", // 14
    "Lågpass frekvens", // 15
    "Lågpass tröskelnivå", // 16
    "Tröskel för låg nivå", // 17
    "Ljusstyrka", // 18
    "Sätt ljudstyrka", // 19
    "Konverter-offset", // 20
    "Låg bandgräns", // 21
    "Hög bandgräns", // 22
    "FM RF-nivåoffset", // 23
    "Stereotröskel", // 24
    "Lågpass frekvens", // 25
    "Lågpass tröskelnivå", // 26
    "Tröskel för låg nivå", // 27
    "Skärmljusstyrka", // 28
    "Av", // 29
    "På", // 30
    "TRYCK MODE FÖR ATT SPARA OCH AVSLUTA", // 31
    "TRYCK BAND FÖR ATT STÄNGA", // 32
    "SÖKER...", // 33
    "Tuner hittades inte", // 34
    "AM-antennförstärkning", // 35
    "FM AGC", // 36
    "Visa RDS-fel", // 37
    "Språk", // 38
    "Välj språk", // 39
    "PI-låst CT-synk", // 40
    "Fel! Ingen RDS-signal", // 41
    "Softmute FM", // 42
    "Softmute AM", // 43
    "Pip vid bandkant", // 44
    "Region", // 45
    "Europa", // 46
    "USA", // 47
    "Visa understreck i RDS", // 48
    "Endast PS", // 49
    "Wi-Fi aktiv", // 50
    "Konfigurera Wi-Fi", // 51
    "Anslut till: ", // 52
    "för att konfigurera Wi-Fi", // 53
    "Försöker att ansluta till Wi-Fi", // 54
    "MISSLYCKADES!", // 55
    "ANSLUTEN!", // 56
    "Stationlist klient-IP", // 57
    "Visa våglängd på KV", // 58
    "RDS-filter", // 59
    "Visa PI-fel", // 60
    "Använd brusspärr", // 61
    "Modulationsmätare", // 62
    "AM noise blanker", // 63
    "FM noise blanker", // 64
    "Standarder laddade", // 65
    "Ljudutgång", // 66
    "Väck inte skärmen\nvid avstämning", // 67
    "Invertera skärmfärger", // 68
    "Om programvaran", // 69
    "Huvudkod:", // 70
    "Bidragsgivare:", // 71
    "Okänt", // 72
    "Skärmsläckaralternativ", // 73
    "Auto avstängning", // 74
    "Skärm av", // 75
    "Tema", // 76
    "Skinn", // 77
    "Upptäckta applikationer", // 78
    "Min.", // 79
    "SPI-hastighet", // 80
    "Skanningskänslighet", // 81
    "INGET", // 82
    "Programvaruversion", // 83
    "Frekvenstypsnitt", // 84
    "Auto", // 85
    "Ingen AF tillgänglig", // 86
    "Ingen EON tillgänglig", // 87
    "Ingen RT+ tillgänglig", // 88
    "FM standard\nsteglängd", // 80
    "Skärmsläckare", // 81
    "Sek.", // 82
    "Klockläge", // 83
    "1% ljusstyrka", // 84
    "25% ljusstyrka", // 85
    "50% ljusstyrka", // 86
    "AM RF-nivå offset", // 87
    "Enhet för\nsignalmätare", // 88
    "Använd AF", // 89
    "Välj FM-band", // 99
    "Välj AM-band", // 100
    "LV", // 101
    "MV", // 102
    "KV", // 103
    "FM", // 104
    "OIRT", // 105
    "Väck inte skärmen vid avstämning", // 106
    "Modellväljare", // 107
    "Bas", // 108
    "Portabel", // 109
    "Portabel touch", // 110
    "Titel", // 111
    "Album", // 112
    "Spår", // 113
    "Artist", // 114
    "Komposition", // 115
    "Sats", // 116
    "Dirigent", // 117
    "Kompositör", // 118
    "Grupp", // 119
    "Kommentar", // 120
    "Genre", // 121
    "Nyheter", // 122
    "Lokala nyheter", // 123
    "Aktiemarknad", // 124
    "Sport", // 125
    "Lotteri", // 126
    "Horoskop", // 127
    "Daglig förströelse", // 128
    "Hälsa", // 129
    "Evenemang", // 130
    "Scen", // 131
    "Bio", // 132
    "TV", // 133
    "Datum/tid", // 134
    "Väder", // 135
    "Trafik", // 136
    "Alarm", // 137
    "Reklam", // 138
    "Webbsida", // 139
    "Annat", // 140
    "Kort PS", // 141
    "Lång PS", // 142
    "Nu", // 143
    "Nästa", // 144
    "Del", // 145
    "Värd", // 146
    "Redaktör", // 147
    "Frekvens", // 148
    "Hemsida", // 149
    "Underkanal", // 150
    "Hotline", // 151
    "Studiotelefon", // 152
    "Telefon", // 153
    "SMS studio", // 154
    "SMS", // 155
    "E-post hotline", // 156
    "E-post studio", // 157
    "E-post", // 158
    "Chat", // 159
    "Chattcenter", // 160
    "Röstningsfråga", // 161
    "Röstningscenter", // 162
    "Plats", // 163
    "Möte", // 164
    "Identifikator", // 165
    "Köp", // 166
    "Få data", // 167
    "MV standard\nsteglängd", // 168
    "Tangerine", // 169
    "Ocean", // 170
    "Indigo", // 171
    "Batterialternativ", // 172
    "Visa spänning", // 173
    "Visa procent", // 174
    "RT buffring", // 175
    "Huvudinställningar", // 176
    "Ljudinställningar", // 177
    "Skärminställningar", // 178
    "RDS-inställningar", // 179
    "FM-inställningar", // 180
    "AM-inställningar", // 181
    "Anslutning & klocka", // 182
    "TRYCK MODE FöR ATT GÅ TILLBAKA", // 183
    "Samkanal-detektor", // 184
    "Samkanal-det.\nkänslighet", // 185
    "Samkanal-det.\ntröskel", // 186
    "Samkanal-det.\nräknare", // 187
    "HUVUD", // 188
    "LJUD", // 189
    "SKÄRM", // 190
    "RDS", // 191
    "FM", // 192
    "AM", // 193
    "ANSLUT", // 194
    "DX-LÄGE", // 195
    "AUTO MEM", // 196
    "AM AGC", // 197
    "FM de-emfas", // 198
    "Små", // 199
    "Stora", // 200
    "Allt", // 201
    "Snabb PS", // 202
    "Standard", // 203
    "Släck skärm vid\nXDRGTK-anslutning", // 204
    "FMSI stereo\nförbättring", // 205
    "Funktion endast tillgäng-\nlig på TEF6687/6689!", // 206
    "Funktion ej tillgänglig\npå valt skinn!", // 207
    "Första minnesplats", // 208
    "Sista minnesplats", // 209
    "Väntetid", // 210
    "Starta DX-skanning", // 211
    "FM DX-alternativ", // 212
    "Avbruten! Startkanalen är\nmärkt som överhoppad.", // 213
    "Auto-sortera AF", // 214
    "Minnesplatser", // 215
    "Exkludera", // 216
    "Endast", // 217
    "Auto-avbryt skanning", // 218
    "Korrekt PI", // 219
    "Signal", // 220
    "Tyst under\nskanning", // 221
    "AIR", // 222
    "5 sek. BAND-knapptryck", // 223
    "Stäng av", // 224
    "Stäng av skärm", // 225
    "Inaktiverat", // 226
    "Ingen", // 227
    "Nyheter", // 228
    "Aktualiteter", // 229
    "Information", // 230
    "Sport", // 231
    "Utbildning", // 232
    "Drama", // 233
    "Kultur", // 234
    "Vetenskap", // 235
    "Underhållning", // 236
    "Popmusik", // 237
    "Rockmusik", // 238
    "Lätt musik", // 239
    "Lätt klassisk", // 240
    "Klassisk musik", // 241
    "Annan musik", // 242
    "Väder", // 243
    "Ekonomi", // 244
    "Barnprogram", // 245
    "Sociala frågor", // 246
    "Religion", // 247
    "Ring in", // 248
    "Resor", // 249
    "Fritid", // 250
    "Jazz", // 251
    "Countrymusik", // 252
    "Nationell musik", // 253
    "Oldies musik", // 254
    "Folkmusik", // 255
    "Dokumentär", // 256
    "Alarmtest", // 257
    "Alarm!!!", // 258
    "Inledande", // 259
    "Alltid", // 260
    "Visa klocka", // 261
    "Visa lång PS", // 262
    "Startfrekvens", // 263
    "Stoppfrekvens", // 264
    "Första minnesplats", // 265
    "Sista minnesplats", // 266
    "Spara endast med RDS", // 267
    "Starta auto-minne", // 268
    "Auto-kanalminne", // 269
    "Stationlist-ID", // 270
    "Hittade", // 271
    "Avbruten! Inga minnesplatser", // 272
    "Avbruten! Användare tryckte knapp", // 273
    "Skanning slutförd utan fel", // 274
    "Töm minnesplatser", // 275
    "raderad", // 276
    "Förhindra dubbla PI", // 277
    "Range", // 278
    "Fullt", // 279
    "Väntetid bara\nvid signal", // 280
    "Tryck på markerat hörn", // 281
    "för att kalibrera touchskärm", // 282
    "Håll BW-knappen för att\nstoppa DX-skanning", // 283
    "Välj bandbredd", // 284
    "Din loggbok", // 285
    "Ladda ner loggbok", // 286
    "Loggbok är tom", // 287
    "Gå till botten", // 288
    "Post tillagd\ni loggbok", // 289
    "Fel vid sparande\ni loggbok", // 290
    "Töm loggbok", // 291
    "Loggbok tömd", // 292
    "Tömning misslyckades", // 293
    "Kan inte spara!\nLoggboken är full!", // 294
    "Autologgning", // 295
    "Auto NTP sommartid", // 296
    "Ange NTP tidszon", // 297
    "Loggboken har mer än 130 poster, vilket visaren inte kan hantera. Ladda ner CSV för behandling.", // 298
    "Kanada" // 299
  }
};