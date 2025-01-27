#include "globals.h"
#include "wifi_common.h"
#include "mykeyboard.h"   // usinf keyboard when calling rename
#include "display.h"      // using displayRedStripe  and loop options
#include "settings.h"
#include "eeprom.h"
#include "powerSave.h"

/***************************************************************************************
** Function name: wifiConnect
** Description:   Connects to wifiNetwork
***************************************************************************************/
bool wifiConnect(String ssid, int encryptation, bool isAP) {
  if(!isAP) {
    int tmz;
    tmz = read_eeprom(EEPROM_TMZ);        // read timezone
    if(tmz>8) tmz=0;
    bool wrongPass = false;
    getConfigs();
    JsonObject wifiList = settings[0]["wifi"];

    String password = wifiList[ssid];
    password = read_eeprom_string(EEPROM_PWD); //password

    bool found = false;
    // if the key doesn't exist json lib returns null instead of an empty string
    if( password != "null" ){
      found = true;
    } else {
      password.clear();
    }

  Retry:
    if (!found || wrongPass) {
      delay(200);
      // Set default SSID & password
      if (ssid == "Mobile-AP")
        pwd = "mobile-ap";
      else if (encryptation > 0) password = keyboard(password, 63, "Network Password:");
    }

    drawMainBorder();
    tft.setCursor(7,27);
    tft.print("Connecting to: " + ssid + ".");
    int i=0;
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      if(tft.getCursorY()!=27 && tft.getCursorX()==0) tft.setCursor(7,tft.getCursorY());
      tft.print(".");
      i++;
      if(i>20) {
        displayError("Wifi Offline");
        delay(500);
        break;
      }
      delay(500);
    }

    if(WiFi.status() == WL_CONNECTED) {
      // update globals
      wifiConnected=true;
      pwd = password; // atm we have 2 duplication globals, TODO: fix
      wifiIP = WiFi.localIP().toString();

      // update stored wifi password
      wifiList[ssid] = password;
      saveConfigs();
      write_eeprom_string(EEPROM_PWD, password);

      timeClient.begin();
      timeClient.update();
      if(tmz==0) timeClient.setTimeOffset(-3 * 3600);
      if(tmz==1) timeClient.setTimeOffset(-2 * 3600);
      if(tmz==2) timeClient.setTimeOffset(-4 * 3600);
      if(tmz==3) timeClient.setTimeOffset(1 * 3600);
      if(tmz==4) timeClient.setTimeOffset(8 * 3600);
      if(tmz==5) timeClient.setTimeOffset(10 * 3600);
      if(tmz==6) timeClient.setTimeOffset(9 * 3600);
      if(tmz==7) timeClient.setTimeOffset(3 * 3600);
      if(tmz==8) timeClient.setTimeOffset(2 * 3600);
      localTime = myTZ.toLocal(timeClient.getEpochTime());
      #if !defined(HAS_RTC)
        rtc.setTime(timeClient.getEpochTime());
        updateTimeStr(rtc.getTimeStruct());
        clock_set=true;
      #endif
      return true;
    } else {
      bool ret=false;
      wrongPass = true;
      wakeUpScreen();
      options = {
        {"Retry", [&]() { ret=true; }},
        {"Cancel", [=]() { backToMenu(); }}
      };
      loopOptions(options);
      delay(200);
      if(ret) goto Retry;
      else {
        wifiDisconnect();
        return false;
      }

    }

  } else { //Running in Access point mode
    IPAddress AP_GATEWAY(172, 0, 0, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_GATEWAY, AP_GATEWAY, IPAddress(255, 255, 255, 0));
    getConfigs();
    WiFi.softAP(ap_ssid, ap_pwd, 6,0,4,false);
    wifiIP = WiFi.softAPIP().toString(); // update global var
    Serial.print("IP: "); Serial.println(wifiIP);
    wifiConnected=true;
    return true;
  }
  delay(200);
  sprite.deleteSprite();
  //returnToMenu=true;
  return true;
}


/***************************************************************************************
** Function name: wifiDisconnect
** Description:   disconnects and turn off the WIFI module
***************************************************************************************/
void wifiDisconnect() {
  WiFi.softAPdisconnect(true); // turn off AP mode
  WiFi.disconnect(true,true);  // turn off STA mode
  WiFi.mode(WIFI_OFF);         // enforces WIFI_OFF mode
  pwd.clear();
  wifiConnected=false;
  returnToMenu=true;
}


/***************************************************************************************
** Function name: wifiConnectMenu
** Description:   Opens a menu to connect to a wifi
***************************************************************************************/
bool wifiConnectMenu(bool isAP) {
  if(isAP) {
    wifiConnect("",0,true);
  }
  else if (WiFi.status() != WL_CONNECTED) {
    int nets;
    WiFi.mode(WIFI_MODE_STA);
    displayRedStripe("Scanning..",TFT_WHITE,FGCOLOR);
    nets=WiFi.scanNetworks();
    options = { };
    for(int i=0; i<nets; i++){
      options.push_back({WiFi.SSID(i).c_str(), [=]() { wifiConnect(WiFi.SSID(i).c_str(),int(WiFi.encryptionType(i)), false); }});
    }
    options.push_back({"Main Menu", [=]() { backToMenu(); }});
    delay(200);
    loopOptions(options);
    delay(200);
  }

  if (returnToMenu) return false;
  return wifiConnected;
}

void checkMAC() {
  drawMainBorderWithTitle("MAC ADDRESS");
  padprintln("\n");
  padprintln(WiFi.macAddress());

  delay(200);
  while(!checkAnyKeyPress()) { delay(80); }
}
