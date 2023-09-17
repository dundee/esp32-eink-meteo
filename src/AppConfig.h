#ifndef APPCONFIG__H_
#define APPCONFIG__H_

#define MANAGE_WIFI true

// ================================= konfiguracni portal

  /** Spousteni konfiguracniho portalu tlacitkem - parametry pro wifiStatus_ShouldStartConfig().
   * 0 = tlacitko FLASH (vpravo od konektoru, pri pohledu od kabelu)
   * Pokud se pouziva tlacitko FLASH, je treba v wifiStatus_ShouldStartConfig() povolit delay(),
   * aby uzivatel mohl stisknout tlacitko az po bootu! */
  #define CONFIG_BUTTON 0
  #define CONFIG_BUTTON_START_CONFIG LOW

#include <WiFi.h>
#include <HTTPClient.h>
#include "wifiman/WiFiManager.h"

#include "config.h"
#include "ConfigProvider.h"
#include "DeviceInfo.h"
#include "raLogger.h"
#include "wifi.h"

#endif