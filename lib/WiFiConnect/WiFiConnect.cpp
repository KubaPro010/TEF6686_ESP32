#include "WiFiConnect.h"

WiFiConnect::WiFiConnect() {
  _apName[0]='\0';
  _apPassword[0]='\0';
}

void WiFiConnect::setAPModeTimeoutMins(int mins) {
  if (mins > 0) _apTimeoutMins = mins;
}

void WiFiConnect::setAPName(const char *apName) {
  if(strlen(apName)>32) return;
  if(strlen(apName) == 0 || (apName == NULL)) {
    String ssid = "ESP_" + String(ESP_getChipId());
    strcpy(_apName,ssid.c_str());
  } else if (apName != NULL &&  strlen(apName)>0) strcpy(_apName,apName);
}

const char* WiFiConnect::getAPName() {
  if ((_apName == NULL ) || strlen(_apName)==0) setAPName(NULL);
  return _apName;
}

void WiFiConnect::addParameter(WiFiConnectParam *p) {
  _params[_paramsCount] = p;
  _paramsCount++;
}

boolean WiFiConnect::startConfigurationPortal() {
  return startConfigurationPortal(_apName, _apPassword);
}

boolean WiFiConnect::startConfigurationPortal(const char  *apName, const char  *apPassword, bool paramsMode) {
  _lastAPPage = millis();

  delay(50); // Huh?

  if (WiFi.status() != WL_CONNECTED) {
	  if (paramsMode) startConfigurationPortal(apName, apPassword, false);
	  else WiFi.mode(WIFI_AP);
  } else WiFi.mode(WIFI_AP_STA); // start an access point on the same channel we're already connected to.
  dnsServer.reset(new DNSServer());
  server.reset(new WebServer(80));
  setAPName(apName);

  if (strlen(apPassword)>0){
    if (strlen(apPassword) < 8 || strlen(apPassword) > 63) apPassword = NULL;
    strcpy(_apPassword,apPassword);
  }

  if (_ap_static_ip) WiFi.softAPConfig(_ap_static_ip, _ap_static_gw, _ap_static_sn);

  if(strlen(apPassword) > 0) WiFi.softAP(_apName, _apPassword);//password option
  else WiFi.softAP(_apName);

  delay(450);

  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(53, "*", WiFi.softAPIP());

  if (paramsMode) {
	  server->on("/", std::bind(&WiFiConnect::handleParamRoot, 			this));
	  server->on("/param", std::bind(&WiFiConnect::handleParams, 		this));
	  server->on("/params", std::bind(&WiFiConnect::handleParams, 		this));
	  server->on("/wifisave", std::bind(&WiFiConnect::handleWifiSave, 	this));
	  server->on("/i", std::bind(&WiFiConnect::handleInfo, 				this));
	  server->on("/fwlink", std::bind(&WiFiConnect::handleParamRoot, 	this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
	  server->on("/hotspot_detect.html", std::bind(&WiFiConnect::handleParamRoot, this));
  } else  { // Config portal mode
	  server->on("/", 			std::bind(&WiFiConnect::handleRoot, this));
	  server->on("/wifi", 		std::bind(&WiFiConnect::handleWifi, this, true));   // Auto Scan of APs
	  server->on("/0wifi", 		std::bind(&WiFiConnect::handleWifi, this, false)); // Manual entry form only
	  server->on("/wifisave", 	std::bind(&WiFiConnect::handleWifiSave, this));
	  server->on("/i", 			std::bind(&WiFiConnect::handleInfo, this)); // Not of interest - commented out in static HTML
	  server->on("/r", 			std::bind(&WiFiConnect::handleReset, this));
	  server->on("/fwlink", 	std::bind(&WiFiConnect::handleRoot, this));  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
	  server->on("/hotspot_detect.html", std::bind(&WiFiConnect::handleRoot, this));
  } // config portal

  server->onNotFound(std::bind(&WiFiConnect::handleNotFound, this));
  server->begin(); // Web server start

  _lastAPPage = millis();
  _readyToConnect = false;
  while (true) {
    dnsServer->processNextRequest();
    server->handleClient();
    if (_readyToConnect) {
      _readyToConnect = false;

      if (autoConnect(_ssid.c_str(), _password.c_str(), WIFI_AP_STA)) {

        WiFi.mode(WIFI_STA);
        delay(500);
        break; // we connected!
      }
    } // ready to connect

    yield();
  }

  //teardown??
  server->close();
  server.reset();

  dnsServer.reset();
  boolean con = (WiFi.status() == WL_CONNECTED);
  if (!con) WiFi.mode(WIFI_STA);
  return con;
}

boolean WiFiConnect::autoConnect() {
  return autoConnect(NULL, NULL, WIFI_STA);
}

boolean WiFiConnect::autoConnect(char const *ssidName, char const *ssidPassword, WiFiMode_t acWiFiMode) {
  WiFi.mode(acWiFiMode);
  if (WiFi.status() == WL_CONNECTED) return true;
  if (_sta_static_ip) WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn);
  int c = 0;
  while (c < _retryAttempts) {
    long ms = millis();
    if (ssidName == NULL || strlen(ssidName)==0) {
      wifi_config_t conf;
      esp_wifi_get_config(WIFI_IF_STA, &conf);

      String stored_ssid = String(reinterpret_cast<const char*>(conf.sta.ssid));
      if (stored_ssid == "") return false;
      WiFi.begin(); // persistence is on by default, so if this WiFi connection should happen automatically
    } else WiFi.begin(ssidName, ssidPassword);

    while (millis() - (unsigned long int)ms < ( (unsigned int)_connectionTimeoutSecs * 1000)) {
      int ws = WiFi.status();
      if (ws == WL_CONNECTED) {
        delay(500);//wait for ip to refresh
        return true;
      } else if (ws == WL_CONNECT_FAILED) {
        delay(500);
      } else {
        delay(200);
        yield();
      }
    } c++;
  } // attemps
  return false;
}

void WiFiConnect::setRetryAttempts(int attempts) {
  if (attempts >= 1) {
    _retryAttempts = attempts;
  }
}

void WiFiConnect::setConnectionTimeoutSecs(int timeout) {
  if (timeout >= 1) _connectionTimeoutSecs = timeout;
}

void WiFiConnect::resetSettings() {
  WiFi.disconnect(true);
  esp_wifi_restore();
  WiFi.begin("0", "0");
  delay(1000);
  ESP.restart();
  delay(2000);
}

const char* WiFiConnect::statusToString(int state) {
  switch (state) {
    case WL_CONNECTED:
      return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
      return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
      return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
      return "WL_DISCONNECTED";
    case WL_SCAN_COMPLETED:
      return "WL_SCAN_COMPLETED";
    case WL_NO_SSID_AVAIL:
      return "WL_NO_SSID_AVAIL";
    case WL_IDLE_STATUS:
      return "WL_IDLE_STATUS";
    case WL_NO_SHIELD:
      return "WL_NO_SHIELD";
    default:
      return "UNKNOWN CODE";
  }
}

/**************************************************************************/
/*!
    @brief  Handles the web request for root in the Parameters Portal and
            sends back the html menu
*/
/**************************************************************************/
void WiFiConnect::handleParamRoot() {
  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }
  _lastAPPage = millis();
  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Options");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);
  page += String(F("<h1>"));
  page += _apName;
  page += String(F("</h1>"));
//  page += String(F("<h3>WiFiConnect</h3>"));
  page += FPSTR(AP_HTTP_PORTAL_PARAM_OPTIONS);
  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);

}
/**************************************************************************/
/*!
    @brief  Handles the web request for displaying custom parameters
            in the Parameters Portal and sends back the html
*/
/**************************************************************************/
void WiFiConnect::handleParams() {
  _lastAPPage = millis();

  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Config Params");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);
  page += FPSTR(AP_HTTP_FORM_PARAM_START);
  char parLength[4];
  // add the extra parameters to the form
  for (int i = 0; i < _paramsCount; i++) {
    if (_params[i] == NULL) {
      break;
    }

    String pitem = FPSTR(AP_HTTP_FORM_PARAM);
    if (_params[i]->getID() != NULL) {
      pitem.replace("{i}", _params[i]->getID());
      pitem.replace("{n}", _params[i]->getID());
      pitem.replace("{p}", _params[i]->getPlaceholder());
      snprintf(parLength, 4, "%d", _params[i]->getValueLength());
      pitem.replace("{l}", parLength);
      pitem.replace("{v}", _params[i]->getValue());
      pitem.replace("{c}", _params[i]->getCustomHTML());
    } else {
      pitem = _params[i]->getCustomHTML();
    }

    page += pitem;
  }
  if (_params[0] != NULL) {
    page += "<br/>";
  }

  page += FPSTR(AP_HTTP_FORM_END);

  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
}
/**************************************************************************/
/*!
    @brief  Handles the web request for displaying the menu in the
            Access Point portal.
*/
/**************************************************************************/
void WiFiConnect::handleRoot() {
  if (captivePortal()) { // If captive portal redirect instead of displaying the page.
    return;
  }
  _lastAPPage = millis();
  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Options");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);
  page += String(F("<h1>"));
  page += _apName;
  page += String(F("</h1>"));
  //page += String(F("<h3>WiFiConnect</h3>"));
  page += FPSTR(AP_HTTP_PORTAL_OPTIONS);
  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);


}

/**************************************************************************/
/*!
    @brief  Handles the web request for entering the SSID and Password of
            the network you want to connect too.
            Optionally scans and shows available networks.
    @param  scan
            Whether to scan for networks or not.
*/
/**************************************************************************/
void WiFiConnect::handleWifi(boolean scan) {

  _lastAPPage = millis();

  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Select WiFi");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);

  if (scan) {
    int n = WiFi.scanNetworks();
    if (n == 0) {
      page += F("No networks found. Refresh to scan again.");
    } else {

      //sort networks
      int indices[n];
      for (int i = 0; i < n; i++) {
        indices[i] = i;
      }

      // RSSI SORT

      // old sort
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }

      String cssid;
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue;
        cssid = WiFi.SSID(indices[i]);
        for (int j = i + 1; j < n; j++) {
          if (cssid == WiFi.SSID(indices[j])) {
            indices[j] = -1; // set dup aps to index -1
          }
        }
      }

      //display networks in page
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue; // skip dups
        int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));

        if (_minimumQuality == -1 || _minimumQuality < quality) {
          String item = FPSTR(AP_HTTP_ITEM);
          String rssiQ;
          rssiQ += quality;
          item.replace("{v}", WiFi.SSID(indices[i]));
          item.replace("{r}", rssiQ);
#if defined(ESP8266)
          if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
#else
          if (WiFi.encryptionType(indices[i]) != WIFI_AUTH_OPEN) {
#endif
            item.replace("{i}", "l");
          } else {
            item.replace("{i}", "");
          }
          //DEBUG_WM(item);
          page += item;
          delay(0);
        }
      }
      page += "<br/>";
    }
  }

  page += FPSTR(AP_HTTP_FORM_START);
  char parLength[4];
  // add the extra parameters to the form
  for (int i = 0; i < _paramsCount; i++) {
    if (_params[i] == NULL) {
      break;
    }

    String pitem = FPSTR(AP_HTTP_FORM_PARAM);
    if (_params[i]->getID() != NULL) {
      pitem.replace("{i}", _params[i]->getID());
      pitem.replace("{n}", _params[i]->getID());
      pitem.replace("{p}", _params[i]->getPlaceholder());
      snprintf(parLength, 4, "%d", _params[i]->getValueLength());
      pitem.replace("{l}", parLength);
      pitem.replace("{v}", _params[i]->getValue());
      pitem.replace("{c}", _params[i]->getCustomHTML());
    } else {
      pitem = _params[i]->getCustomHTML();
    }

    page += pitem;
  }
  if (_params[0] != NULL) {
    page += "<br/>";
  }

  if (_sta_static_ip) {

    String item = FPSTR(AP_HTTP_FORM_PARAM);
    item.replace("{i}", "ip");
    item.replace("{n}", "ip");
    item.replace("{p}", "Static IP");
    item.replace("{l}", "15");
    item.replace("{v}", _sta_static_ip.toString());

    page += item;

    item = FPSTR(AP_HTTP_FORM_PARAM);
    item.replace("{i}", "gw");
    item.replace("{n}", "gw");
    item.replace("{p}", "Static Gateway");
    item.replace("{l}", "15");
    item.replace("{v}", _sta_static_gw.toString());

    page += item;

    item = FPSTR(AP_HTTP_FORM_PARAM);
    item.replace("{i}", "sn");
    item.replace("{n}", "sn");
    item.replace("{p}", "Subnet");
    item.replace("{l}", "15");
    item.replace("{v}", _sta_static_sn.toString());

    page += item;

    page += "<br/>";
  }

  page += FPSTR(AP_HTTP_FORM_END);
  page += FPSTR(AP_HTTP_SCAN_LINK);

  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
}
void WiFiConnect::handleWifiSave() {
  _lastAPPage = millis();
  _ssid = server->arg("s").c_str();
  _ssid.trim();
  _password = server->arg("p").c_str();
  _password.trim();
  //parameters
  for (int i = 0; i < _paramsCount; i++) {
    if (_params[i] == NULL) {
      break;
    }
    //read parameter
    String value = server->arg(_params[i]->getID()).c_str();
    //store it in array
    value.toCharArray(_params[i]->_value, _params[i]->_length);
  }

  if (server->arg("ip") != "") {
    //_sta_static_ip.fromString(server->arg("ip"));
    String ip = server->arg("ip");
    optionalIPFromString(&_sta_static_ip, ip.c_str());
  }
  if (server->arg("gw") != "") {
    String gw = server->arg("gw");
    optionalIPFromString(&_sta_static_gw, gw.c_str());
  }
  if (server->arg("sn") != "") {
    String sn = server->arg("sn");
    optionalIPFromString(&_sta_static_sn, sn.c_str());
  }

  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Credentials Saved");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);
  page += FPSTR(AP_HTTP_SAVED);
  page.replace("{ap}", _ssid);
  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
  _readyToConnect = true;
}
/**************************************************************************/
/*!
    @brief  Handles the web request and shows basic information about the chip.
            TODO: ESP32 information still needs completing.
*/
/**************************************************************************/
void WiFiConnect::handleInfo() {
  _lastAPPage = millis();

  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);
  page += F("<dl>");
  page += F("<dt>Chip ID</dt><dd>");
  page += ESP_getChipId();
  page += F("</dd>");
  page += F("<dt>Flash Chip ID</dt><dd>");
#if defined(ESP8266)
  page += ESP.getFlashChipId();
#else
  // TODO
  page += F("TODO");
#endif
  page += F("</dd>");
  page += F("<dt>IDE Flash Size</dt><dd>");
  page += ESP.getFlashChipSize();
  page += F(" bytes</dd>");
  page += F("<dt>Real Flash Size</dt><dd>");
#if defined(ESP8266)
  page += ESP.getFlashChipRealSize();
#else
  // TODO
  page += F("TODO");
#endif
  page += F(" bytes</dd>");
  page += F("<dt>Soft AP IP</dt><dd>");
  page += WiFi.softAPIP().toString();
  page += F("</dd>");
  page += F("<dt>Soft AP MAC</dt><dd>");
  page += WiFi.softAPmacAddress();
  page += F("</dd>");
  page += F("<dt>Station MAC</dt><dd>");
  page += WiFi.macAddress();
  page += F("</dd>");
  page += F("</dl>");
  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);
}
/**************************************************************************/
/*!
    @brief  Handles the reset web request and and restarts the chip.
            Does not clear settings.
*/
/**************************************************************************/
void WiFiConnect::handleReset() {
  _lastAPPage = millis();

  String page = FPSTR(AP_HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(AP_HTTP_SCRIPT);
  page += FPSTR(AP_HTTP_STYLE);
  page += FPSTR(AP_HTTP_HEAD_END);
  page += F("Module will reset in a few seconds.");
  page += FPSTR(AP_HTTP_END);

  server->sendHeader("Content-Length", String(page.length()));
  server->send(200, "text/html", page);

  delay(5000);
  ESP.restart();
  delay(2000);
}
/**************************************************************************/
/*!
    @brief  Handles the 204 web request from Android devices
*/
/**************************************************************************/
void WiFiConnect::handle204() {
  _lastAPPage = millis();
  handleRoot();
}
/**************************************************************************/
/*!
    @brief  Handles any web requests that are not found.
*/
/**************************************************************************/
void WiFiConnect::handleNotFound() {
  if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
    return;
  }
  _lastAPPage = millis();
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->sendHeader("Content-Length", String(message.length()));
  server->send ( 404, "text/plain", message );
}

/**************************************************************************/
/*!
    @brief  Redirect to captive portal if we get a request for another domain.
            Return true in that case so the page handler do not try to
            handle the request again.
    @return True if we have redirected to our portal, else false and contimue
            to handle request
*/
/**************************************************************************/
boolean WiFiConnect::captivePortal() {
  if (!isIp(server->hostHeader()) ) {
    _lastAPPage = millis();
    String msg = "redirect\n";
    server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
    server->sendHeader("Content-Length", String(msg.length()));
    server->send ( 302, "text/plain", msg); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    //server->client().stop(); // Stop is needed because we sent no content length

    return true;
  }
  return false;
}

int WiFiConnect::getRSSIasQuality(int RSSI) {
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

boolean WiFiConnect::isIp(String str) {
  for (unsigned int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String WiFiConnect::toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

void WiFiConnect::setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _ap_static_ip = ip;
  _ap_static_gw = gw;
  _ap_static_sn = sn;
}

void WiFiConnect::setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
  _sta_static_ip = ip;
  _sta_static_gw = gw;
  _sta_static_sn = sn;
}
